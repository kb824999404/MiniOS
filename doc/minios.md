# MiniOS —— 操作系统课程设计文档
<center><img src="imgs/MiniOS.png" width="600"></img></center>

**项目成员：**
* 1851197 周楷彬 42028701
* 1851352 伏瑞 42028702
* 1851199 庄泽杨 42028703
* 1851494 王宇其 42028701
* 1851198 吕嘉尧 42028701

​**指导老师：** 王冬青老师
## 目录
[TOC]
## 1.项目概述
### 1.1 项目简介
1. 我们的项目基于Orange's的样例代码，主要参考资料为《Orange'S 一个操作系统的实现》，在上面进行了添加，删除和修改，同时参考了《30天自制操作系统》
2. 项目的命令行系统参考自linux，我们对其进行了模仿 
3. 在样例代码的基础上，我们的项目实现了`320x200x8`位彩色的图形模式，增加了**图形管理模块**，能够读取并显示BMP格式图片，并提供了一组绘图接口，增加了**4个系统级应用**，包括多用户登录、进程管理、多功能控制台、文件管理。同时，还实现了**10个用户级应用**，包括画图、计算器、日历和7个小游戏（2048，五子棋，猜数字，拼图，推箱子，MoveGame，贪吃蛇）
### 1.2 开发环境
* 平台：ubuntu 14.04.5 (32bit)
* 语言：汇编语言，C语言
* 工具：gcc, nasm,make,bochs 2.6.11
### 1.3 项目分工
* 周楷彬：图形管理模块，多用户登录，画图，代码整合
* 伏瑞：进程管理，推箱子，猜数字
* 庄泽杨：文件管理，推箱子，五子棋
* 王宇其：多功能控制台，拼图，MoveGame
* 吕嘉尧：计算器，日历，2048，贪吃蛇
---

## 2.操作指南
| 命令         | 说明                                          |
| :----------- | :-------------------------------------------- |
| help | 显示所有命令 |
| clear | 清屏 |
| colormode | 切换颜色模式(彩色/灰阶) |
| bmp | 显示BMP图片 |
| logout | 登出 |
| sudo | 切换到管理员用户 |
| users | 显示所有用户 |
| useradd [username] [password] | 添加用户`username`,密码为`password` |
| userdel [username] | 删除用户`username`|
| passwd [username] | 修改用户`username`的密码|
| proc | 显示所有进程|
| kill [pid] | 结束pid值为`pid`的进程|
| pause [pid]|暂停pid值为`pid`的进程|
| resume [pid]|恢复pid值为`pid`的进程|
| ↑↓键 |切换执行过的历史命令|
| `TAB`键 |命令智能补全|
| ls |显示所有文件|
| mkfile [filename] |创建文件`filename`|
| cat [filename]|读取并显示文件`filename`|
| rm [filename]|删除文件`filename`|
| wt [filename]|编辑文件`filename`,覆盖其原来的内容|
| wt+ [filename]|编辑文件`filename`,追加内容|
| draw |运行画图程序|
| calculator|运行计算器程序|
| calendar|运行日历程序|
| 2048 |运行2048游戏|
| chess|运行五子棋游戏|
| guessnum|运行猜数字游戏|
| pintu|运行拼图游戏|
| pushbox|运行推箱子游戏|
| movegame|运行MoveGame游戏|
| tanchishe|运行贪吃蛇游戏|
---
## 3.功能展示
### 3.1 图形界面
* 运行操作系统，完成加载工作后，进入图形模式，显示开机画面后，进入登录界面，初始状态只有管理员账户`Admin`，密码为`admin`
![start](imgs/start.png)
* 输入正确密码后成功登录，进入主界面，在控制台输入指令后，操作系统就会执行相应的程序
![main](imgs/main.png)
* 帮助：输入`help`指令，显示所有的指令及其用法
![help1](imgs/help1.png)
![help2](imgs/help2.png)
![help3](imgs/help3.png)
* 清屏：输入`clear`指令，清空控制台输出
![clear](imgs/clear指令.png)
![clear](imgs/clear界面02.png)
* 切换颜色模式：输入`colormode`指令，系统会切换图形模式下使用的色盘，从而切换颜色模式(彩色/灰阶)
![colormode](imgs/colormode灰阶.png)
* 显示图片：输入`bmp`指令，系统会读取并显示一张使用8位索引颜色模式的BMP格式图片
![BMP](imgs/bmp指令.png)
### 3.2 系统级应用——多用户登录
我们的系统增加了多用户登录及权限功能，不同的用户有不同的权限和自己的文件分区，除了管理员用户，普通用户只能访问和修改自己的文件，只能修改自己的密码
* 登出：输入`logout`指令，退出登录，系统要求重新登录
![logout](imgs/logout.png)
* 切换到管理员用户：输入`sudo`指令，能够从普通用户切换到管理员用户
![sudo](imgs/sudo.png)
* 显示所有用户：输入`users`指令，显示系统中的用户数量和所有用户名
![users](imgs/users指令.png)
* 添加用户：输入`useradd`指令，添加相应的用户，只有管理员用户能够添加用户，普通用户要求添加用户会输出没有权限的错误
![useradd](imgs/useradd指令.png)
![useradd1](imgs/useradd1.png)
* 删除用户：输入`userdel`指令，删除相应的用户，只有管理员用户能够删除用户
![userdel](imgs/userdel指令.png)
* 修改密码：输入`passwd`指令，修改相应用户的密码，普通用户只能修改自己的密码，管理员用户能够修改所有用户的密码
![passwd](imgs/passwd指令.png)
### 3.3 系统级应用——进程管理
* proc 
![proc](imgs/proc指令.png)
* kill 
![kill](imgs/kill指令.png)
* pause 
![pause](imgs/pause指令.png)
* resume
![resume](imgs/resume指令.png)
### 3.4 系统级应用——多功能控制台
* ↑↓键:'↑'和'↓'可以查看上（下）一条运行的指令
* `TAB`键:智能补全指令
### 3.5 系统级应用——文件管理
我们将Orange'S原本的扁平化文件系统改为了多用户分区的文件系统，不同用户拥有不同分区，且只能访问自己的分区，在各自分区下的文件系统进行各自的文件系统操作，具体操作如下：
* 显示所有文件：使用`ls`指令查看当前目录下的所有文件
![ls](imgs/ls指令.png)
* 创建文件：输入`mkfile [filename]`指令创建文件名为`filename`的文件，且需输入该文件的初始内容
![mkfile](imgs/mkfile指令.png)
* 读取并显示文件：输入`cat [filename]`指令读取文件名为`filename`的文件，并将该文件的内容显示出来
![cat](imgs/cat指令.png)
* 删除文件：输入`rm [filename]`指令删除文件名为`filename`的文件，同时将其从文件目录中删除
![rm](imgs/rm指令.png)
* 写文件覆盖内容：输入`wt [filename]`指令编辑文件名为`filename`的文件，同时输入新的内容将原来文件的内容覆盖
![wt](imgs/wt指令.png)
* 写文件追加内容：输入`wt+ [filename]`指令编辑文件名为`filename`的文件，同时输入新的内容追加该内容到原来文件内容中
![wt+](imgs/wt+指令.png)    
### 3.6 用户级应用——画图
* 输入`draw`指令，执行画图程序，该程序基于图形管理模块提供的画图接口，能够绘制像素点、文字、直线、矩形、圆形
![draw](imgs/draw指令.png)
### 3.7 用户级应用——计算器
![calculator](imgs/calculator.png)
### 3.8 用户级应用——日历
![calendar](imgs/calendar指令.png)
### 3.9 用户级应用——2048
![2048](imgs/2048指令.png)
### 3.10 用户级应用——五子棋
* **游戏规则：** 调用指令 `chess` 启动五子棋游戏后，可以看到五子棋的游戏界面。玩家的游戏棋子为白子，电脑AI的游戏棋子为黑子。玩家通过方向键 `W`、`A`、`S`、`D` 控制白棋在界面进行移动，使用按键 `SPACE` 进行落子确认，与电脑ai进行五子棋对战。当有一方的同一颜色的棋子在横行竖行斜行连成五个子，游戏结束。可以通过按键 `q\Q` 退出游戏
![chess](imgs/chess指令.png)
### 3.11 用户级应用——猜数字
![guessnum](imgs/guessnum.png)
### 3.12 用户级应用——拼图
* **游戏规则：**`*`是被控制的位置，`awsd`控制`*`的移动。当所有数字顺序排放并且`*`位于右下角时，游戏结束。
![pintu](imgs/pintu指令.png)
### 3.13 用户级应用——推箱子
* **游戏规则：** 调用指令 `pushbox` 启动推箱子游戏后，可以看到推箱子的游戏界面。红色方块为玩家控制的人物，蓝色方块为游戏中等待推动的箱子，黄色方块为游戏中箱子推送的目的地，绿色方块为成功推送到目的地后的箱子，紫色方块为进入目的地的玩家控制的人，黑色方块为游戏中的墙壁，白色方块为游戏中可以移动的空白区域。玩家通过方向键 `W`、`A`、`S`、`D` 控制游戏人物进行上下左右的移动推动箱子进入目的地。游戏界面的分数显示成功推进目的地的箱子个数，箱子全部推进则游戏结束。可以通过按键 `q\Q` 退出游戏
![pushbox](imgs/pushbox指令.png)
### 3.14 用户级应用——MoveGame
* **游戏规则：**`w`和`d`控制小球的运动，红色条踩了会立即消失，蓝色条加速向左，绿色条加速向右，黑色条可以正常踩。玩家每移动一次小球，游戏运行一帧。当小球落到最底部时游戏结束。
![movegame](imgs/movegame.png)
### 3.15 用户级应用——贪吃蛇
![tanchishe](imgs/tanchishe指令.png)

---
## 4.功能实现
### 4.1 图形界面
* 图形管理模块(graphics/main.c):
![graphics](codes/graphics1.png)
![graphics](codes/graphics2.png)
* 绘图函数(lib/graphics.c):
![draw](codes/draw1.png)
![draw](codes/draw2.png)
![draw](codes/draw3.png)
* 显示BMP图片(graphics/main.c graphics/graphics.c):
![bmp](codes/bmp1.png)
![bmp](codes/bmp2.png)
### 4.2 系统级应用——多用户登录
* kernel/tools/user.c:
![user](codes/user.png)
### 4.3 系统级应用——进程管理
* kernel/tools/process.c:
![process](codes/process.png)
### 4.4 系统级应用——多功能控制台
* kernel/tty.c:
![tty](codes/tty1.png)
* kernel/console.c
![console](codes/console1.png)
![console](codes/console2.png)
* kernel/main.c/shabby_shell:
![shell](codes/shell.png)
### 4.5 系统级应用——文件管理
* kernel/tools/fs.c:
![fs](codes/fs1.png)
![fs](codes/fs2.png)
### 4.6 用户级应用——画图
* command/draw.c:
![draw](codes/draw.png)
### 4.7 用户级应用——计算器
* kernel/tools/calculator.c:
![calculator](codes/calculator1.png)
![calculator](codes/calculator2.png)
### 4.8 用户级应用——日历
* kernel/tools/calendar.c:
![calendar](codes/calendar.png)
### 4.9 用户级应用——2048
* kernel/gmae/2048.c:
![2048](codes/2048.png)
### 4.10 用户级应用——五子棋
* kernel/gmae/chess.c:
![chess](codes/chess1.png)
![chess](codes/chess2.png)
### 4.11 用户级应用——猜数字
* kernel/gmae/guessnum.c:
![guessnum](codes/guessnum.png)
### 4.12 用户级应用——拼图
* kernel/gmae/pintu.c:
![pintu](codes/pintu.png)
### 4.13 用户级应用——推箱子
* kernel/gmae/pushbox.c:
![pushbox](codes/pushbox.png)
### 4.14 用户级应用——MoveGame
* kernel/gmae/smgame.c:
![movegame](codes/movegame.png)
### 4.15 用户级应用——贪吃蛇
* kernel/gmae/tanchishe.c:
![tanchishe](codes/tanchishe1.png)
![tanchishe](codes/tanchishe2.png)
---