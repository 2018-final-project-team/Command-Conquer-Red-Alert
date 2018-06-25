
#include <stdio.h>
#include "Scene/RoomScene.h"
#include "ui/CocosGUI.h"
#include "Scene/SearchScene.h"
#include "NetWork/MessageCode.h"
#include "Util/GameAudio.h"

USING_NS_CC;
using namespace ui;

int loop = 0;                                       ///用于计时，每100帧向网络中发送一次查询信息

static Client *             client = nullptr;       ///指向client的指针，用于传输数据和接受指令
static std::string          _static_player_name;    ///玩家名称
std::vector<std::string>    room_owner;             ///房间拥有者的昵称

Scene* SearchScene::createScene(Client* c, std::string _playerName)
{
	client = c;
	_static_player_name = _playerName;

	auto scene = Scene::create();
	auto layer = SearchScene::create();

	scene->addChild(layer);

	return scene;
}

bool SearchScene::init()
{
	_player_name = _static_player_name;

	if (!Layer::init())
	{
		return false;
	}

	this->addChild(client);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto RoomListBg = Sprite::create("RoomList_bg.png");
	RoomListBg->setScale(0.7f, 0.7f);
	RoomListBg->setPosition(origin / 2 + visibleSize / 2);
	_room_list_bg = RoomListBg;
	this->addChild(RoomListBg, 1);

	auto bg = Sprite::create("background.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	auto backButton = Button::create("backNormal.png", "backSelected.png");
	backButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5));
	backButton->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			GameAudio::getInstance()->playEffect("Sound/button.mp3");

			Director::getInstance()->pushScene(TransitionFade::create(1, NetMenu::createScene(_static_player_name)));
		}
	});
	RoomListBg->addChild(backButton);

	this->scheduleUpdate();

	return true;
}

void SearchScene::update(float delta)
{

	if (client->_search_finished)
	{
		loop++;

		if (loop / 100 > 1) {
			loop = 0;
			client->sendMessage(QUERY_FOR_ROOM, "|||||||||||");
		}

		std::string temp = client->executeOrder();

		if (temp != "no") {

			//            std::cout << "search: " << temp << std::endl;

			if (temp[0] == ANSWER_FOR_ROOM[0]) {

				//                std::cout << "Find you\n";
				std::string owner_name = temp.substr(1, temp.size() - 1);

				bool if_exist = false;
				for (int i = 0; i < room_owner.size(); i++) {
					if (owner_name == room_owner.at(i)) {
						if_exist = true;
					}
				}

				if (!if_exist)
				{
					room_owner.push_back(owner_name);

					auto board = Button::create("PlayerBar.png", "PlayerBar.png");
					auto in_button = Button::create("button_normal.png", "button_selected.png");
                    in_button->setTitleText("Connect");
					board->addChild(in_button);

					in_button->setPosition(Vec2(board->getContentSize().width / 6 * 5, board->getContentSize().height / 2));
                    in_button->setTitleFontSize(25);
					in_button->setScale(0.7);

					button_list.pushBack(in_button);
					std::string button_owner_name = owner_name;

					in_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
						if (type == Widget::TouchEventType::ENDED) {

							GameAudio::getInstance()->playEffect("Sound/button.mp3");

							client->_filter_mode = true;
							client->sensitive_word = button_owner_name;

							auto transition = TransitionSlideInL::create(0.5, RoomScene::createScene(client, 1, _player_name));
							Director::getInstance()->pushScene(transition);

						}
					});
					board->setTitleText(owner_name);
					board->setTitleFontSize(30);
					_room_list_bg->addChild(board);

					board->setPosition(Vec2(
						_room_list_bg->getContentSize().width / 2,
						(_room_list_bg->getContentSize().height - 90) / 5 * (5 - room_owner.size())));
				}
			}
		}
	}

}
