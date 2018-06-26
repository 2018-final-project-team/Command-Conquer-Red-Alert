#include <stdio.h>
#include "Client.h"
#include "MessageCode.h"

int ipindex;                                ///能正确连接的ip地址在ip列表里的索引
int serverIndex;                            ///server所在的ip地址在ip列表里的索引
int client_mode;                            ///当前client的运行模式

std::string                 ipOut;          ///能成功建立连接的ip地址字符串
std::string                 currentIp;      ///当前连接的ip地址字符串
std::vector<std::string>    ipList;         ///所有连接到该网段的设备的ip地址表
std::vector<std::string>    serverIpList;   ///能够连接上的, 存在server的ip地址表


static Client*              this_client = nullptr;      ///指向client对象的指针

/**
            * @name MAC平台下的标识码
            * @{
            */
            #define MAC 1
            /** @} */

/**
            * @name WINDOWS平台下的标识码
            * @{
            */
            #define WIN 2
            /** @} */

#ifdef _WIN32
int system_type = WIN;
#else
int system_type = MAC;
#endif

void chat_client::handle_connect(const boost::system::error_code& error)
{

    if (!error)
    {
        serverIndex = ipindex;
        serverIpList.push_back(ipOut);
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    }
    else{
        
    }
}

void chat_client::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
        write_msgs_.pop_front();
        if (!write_msgs_.empty())
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front().data(),
                                                         write_msgs_.front().length()),
                                     boost::bind(&chat_client::handle_write, this,
                                                 boost::asio::placeholders::error)); 
        }
    }
    else
    {
        do_close();
    }
}


void chat_client::handle_read_header(const boost::system::error_code& error)
{
    if (!error && read_msg_.decode_header())
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                boost::bind(&chat_client::handle_read_body, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        do_close();
    }
}

void chat_client::handle_read_body(const boost::system::error_code& error)
{
    if (!error)
    {
        std::string temp1(read_msg_.body());
        if(temp1 == ipOut){
            serverIndex = ipindex;
//            std::cout << ipOut << " succeed \n";
//            std::cout << "==========================\n";
        }
        this_client->t_lock.lock();
        std::string temp (read_msg_.body(), read_msg_.body_length());
//        std::cout << "client get: " << temp << "==========";
        
        this_client->_orderList.push_back(temp);
        this_client->t_lock.unlock();
        std::cout.write(read_msg_.body(), read_msg_.body_length()); 
//        std::cout << "\n";

        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length), 
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        do_close();
    }
}

void chat_client::do_write(chat_message msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 boost::bind(&chat_client::handle_write, this,
                                             boost::asio::placeholders::error));
    }
}

int Client::client(void)
{
    if (system(NULL)){
        puts ("Ok");
    }else{
        exit (EXIT_FAILURE);
    }
    
    if(client_mode == 2)
    {
        system("arp -a > arp.txt");
//        std::cout << "arp done" << std::endl;
        
        std::ifstream in("arp.txt");
        if(!in.is_open()){
//            std::cout << "Search Error! \n";
        }
        
        if(system_type == MAC)
        {
            while (!in.eof() )
            {
                std::string temp;
                getline (in, temp);
                if(in.eof()){
                    break;
                }
                int i = temp.find(')');
                temp = temp.substr(3,i - 3);
//                std::cout << temp << std::endl;
                currentIp = temp;
                ipList.push_back(temp);
            }
        }
        else
        {
			std::string temp;
			getline(in, temp);
			getline(in, temp);

			int index = temp.find('.');
			index = temp.find('.', index + 1);
			index = temp.find('.', index + 1);
			temp = temp.substr(6, index - 5);
			//                std::cout << temp << std::endl;
			for (int j = 0; j < 255; ++j)
			{
				char num[4];
				sprintf(num, "%d", j);
				ipList.push_back(temp + num);
			}

        }
        
    }
    
    
    std::vector<chat_client> cc;
    std::vector<boost::thread> t;
    
    for (int i = 0; i < ipList.size() - 1; i++)
    {
        
        if(client_mode == 1)
        {
            break;
        }
 
        
        boost::asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(ipList.at(i), "11332");
        ipOut = ipList.at(i);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        
        chat_client c(io_service, iterator);
        
        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
        
        ipindex = i;
        

		Sleep(50);

        
        
        c.close();
        t.join();
    }
    
    if(ipList.size() != 0){
//        std::cout << ">>>>>>>>" << ipList.at(serverIndex) << ">>>>>>>>>>\n";
    }

    
    try
    {

        boost::asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        
        std::string ip;
        
        if(client_mode == 1)
        {
            ip = _landlordIP;
        }
        else{
            ip = ipList.at(serverIndex);
        }
        _search_finished = true;
        tcp::resolver::query query(ip, "11332"); 
        tcp::resolver::iterator iterator = resolver.resolve(query);
        
        chat_client c(io_service, iterator);
        _clientInstance = &c;
        this_client = this;
        
        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
        
        char line[chat_message::max_body_length + 1];
        while (1)
        {
            ;
        }
        
        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
    
}

void Client::runClient(int mode)
{
    client_mode = mode;
    _search_finished = false;
    _filter_mode = false;
    std::thread t(&Client::client, this);
    t.detach();
}

void Client::sendMessage(const std::string & code, const std::string & message)
{
    chat_message msg;
    std::string temp;
    
    if(_filter_mode == true && code[0] != ANSWER_FOR_ROOM[0])
    {
        temp.append(sensitive_word.substr(0, 4));
    }
    temp.append(code);
    temp.append(message);
    msg.body_length(temp.size());
    memcpy(msg.body(), temp.c_str(), msg.body_length());
    msg.encode_header();
    _clientInstance->write(msg);
}

std::string Client::executeOrder (void)
{
    t_lock.lock();
    std::string temp;
    
    if(_orderList.size() != 0){
//        std::cout << "order list :" << _orderList.front() << std::endl;
        temp = _orderList.front();
        _orderList.pop_front();
        if(_filter_mode == true){
            std::string filter_word = temp.substr(0,4);
            if(filter_word == sensitive_word.substr(0,4)){
                std::string real_order = temp.substr(4, temp.size() - 4);
                temp = real_order;
            }
            else if(temp[0] != QUERY_FOR_ROOM[0])
            {
                temp = "no";
            }
        }
    }
    else{
        temp = "no";
    }
    t_lock.unlock();
    
    
    return temp;
}
