## Red Alert项目小组


### 组员：

- WL
- WXZ
- HLR
- CZD

-----------

### 项目选题：

- 红色警戒

---

### 项目进度：

- [x] 环境配置
- [x] 初步入门
- [x] 尝试编写小游戏
- [x] 讨论架构
- [x] 完成demo
- [x] 完善功能
- [x] 代码重构
- [x] 网络对战功能搭建
- [x] 游戏UI美化
- [x] 优化游戏整体体验


---
### 基础功能：
- [x] 支持地图绘制、建筑物绘制、兵种绘制等
- [x] 支持鼠标和键盘操作交互
- [x] 支持五种基本建筑物（基地，电厂，矿场，兵营，战车工厂）
- [x] 支持两种基本步兵（警犬，大兵）
- [x] 支持一种基本坦克（坦克）
- [x] 支持动画效果
- [x] 实现服务端：支持局域网联机对战（自由对抗模式），且支持多人在同一个地图游戏

### 扩展功能：
- [x] 支持多个国家（不同国家建筑物，兵种不同）
- [x] 支持更多兵种，如狙击兵，导弹车
- [x] 支持聊天（组队聊天和所有人聊天）
- [x] 支持房间列表
- [x] 支持 >= 2 张地图

---
### C++特性使用情况：
- [x] 初始化列表
- [x] 类型推断
- [x] 基于范围的for循环
- [x] Lambda表达式
- [x] 智能指针
- [x] 常量表达式 ( constexpr )
- [x] 右值引用
- [x] 字符串字面量
- [x] 其他C++11或更高的新特性

---

### 其他：
- [x] 跨桌面平台（Windows、Linux、MacOS 中至少两个）
- [x] 恰当合理地使用了boost库
- [x] 没有内存泄漏

---
# 项目规范
>  请开始之前，请阅读[编程的智慧](http://www.yinwang.org/blog-cn/2015/11/21/programming-philosophy)，规范自己的习惯。

## 花括号：

**规定花括号放置的位置**

- 主要包括 if条件语句，函数体:  
  1. 使用if条件判断必须使用花括号，不允许**单行不使用括号**.

  2. **另起一行放置括号(Lambda表达式除外)**.

     ​

## 功能块间距：

**杜绝所有的代码之间没有任何回车！！**

- 主要包括不同函数defination之间与函数内部不同功能块之间的空行:  
  1. 不同函数体之间至少有三行回车.

  2. 注释与上一行代码之间至少有一行空行，即不允许出现 **代码-注释-代码** 紧紧相邻的情况.

  3. 同一函数功能类似的代码应该用多个空行隔开，便于阅读.  ​

## Coding：
**建立自己的本地Git仓库，请使用命令行进行上传代码**  

- 主要包括建立自己的branch与一些上传的规范

  1.  **请保证每次push的质量**， commit 包含且仅包含一项功能，严禁只改两三行就commit（除非是上次push时的版本有重大bug或过程当中发生了疏忽导致版本存在严重问题）,在没有接近DDL之前,以后这种行为视为刷commit.

  2. **上传的描述统一使用中文** .

  3. **练习使用Git请使用Github上的[repositoryForHomework](https://github.com/2018-final-project-team/repositoryForHomework)**.

  4. 及时pull，检查自己修改的版本是不是**基于远程仓库上的最新版本**.

  5. 上传时请检查文件路径，不要把文件上传到错误的文件夹或是上传了本地的其他临时文件.

    
## 命名规范：

- class以大写开头，大写分割单词，比如`GameScene`

- 变量以_开头，接驼峰命名法，例如`_playerName`

- 函数使用驼峰命名法，例如`loadSource()`​

## 注释：
**请使用中文注释，编码格式使用utf-8编码**  

- 相似功能块之间可以用 `//===功能块的名称==== ` 与其他功能隔开，比如人物的攻击，人物的移动等.
- 如果有现阶段无法完成，但是需要在以后所进行完善的地方，请标注`//=========== 待完善 : 添加 xxxxx ============`
- 在新的功能函数前，如果函数名不能讲清楚函数的作用，请写明这个函数的主要用途。当输入输出的变量名难以解释它的含义时，也请标注出来。（如输入内容为`int role`，则建议标注一下role为何值的时候代表哪个role，如果传入为`std::string role`时，则无需额外标注）。
- 对函数的注释应在`.h`文件中，注释格式统一使用：

	`/**` <br>
	`@brief 函数功能` <br>
    `@param 变量名 参数1 含义` <br>
    `@param 变量名 参数2 含义` <br>
    `@param 变量名 参数3 含义` <br>
    `……` <br>
    `@return  返回值` <br>
    `*/`

	



