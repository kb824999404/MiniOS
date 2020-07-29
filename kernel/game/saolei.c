#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

//MINE是定义翻开格子中的‘*'号
#define MINE   42
#define ESC    27
#define ENTER   13
#define SPACE   32
#define UP    72
#define DOWN   80
#define LEFT   75
#define RIGHT   77
//定义类型状态，方便后续判断
#define bool int
#define true 1
#define false 0
#define ROW    10
#define COLUMN   10
#define ALL_MINES  15
//当前位置的结构体
typedef struct currentPosition_struct
{
    int x;
    int y;
} currentPosition;
//每一个小格的结构体
typedef struct blockCondition_struct
{
    //是否被覆盖了
    bool beCovered;
    //以它为中心周围的雷数
    int minesNum;
} blockCondition;
//光标的位置数组
currentPosition cursorPos[ROW][COLUMN];
//雷区地图的数组
blockCondition minesMap[ROW][COLUMN];
//剩下的格子数
int leftBlocksNum = ROW * COLUMN;
//光标在光标位置、雷区地图中的下标
int index_x = 0, index_y = 0;
//设置窗口前后背景色
void setColor(unsigned short color);
//开头的欢迎“动画”
void welcomeToMyGame();
//游戏地图初始化
void gameInitailize();
//以某格子为中心计算雷数
void countMines();
//获取键盘的输入
void keyBoardInput();
//指定光标的位置
void setCurPos(int y, int x);
//移动光标的位置
void moveCursor(int y, int x);
//检测每一步的结果
bool checkResult(int y, int x);
//输出游戏界面
void printMap();
//游戏退出后的“动画”
void gameOver(char* str);
//删除窗口中一行的缓冲
void delLine(int y);
int main()
{
    setColor(10);
    system("cls");
    welcomeToMyGame();
    gameInitailize();
    countMines();
    printMap();
    for (;;)
    {
        setCurPos(cursorPos[index_y][index_x].y, cursorPos[index_y][index_x].x);
        keyBoardInput();
    }
    return EXIT_SUCCESS;
}
void setColor(unsigned short color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    //对设置之后的输出有效
    SetConsoleTextAttribute(hCon, color);
};
void welcomeToMyGame()
{
    int i = 0;
    char introductions0[] = "LittleMines";
    char introductions1[] = "--";
    char introductions2[] = "Author:Fu Rui";
    char introductions3[] = "Thank ╮(￣▽￣)╭";
    //控制台窗口默认大小是80*25，所以能达到最大的位置是[79,24]
    for (i = 0; i <= 5; ++i)
    {
        //每次输出之前都清屏，就会有看起来是动的效果
        system("cls");
        //纵坐标不断加，形成向下效果
        setCurPos(i, (80 - strlen(introductions0)) / 2);
        printf("%s", introductions0);
      
        Sleep(50);
    }
    //为了对称，从边边78开始到中间39好了
    for (i = 78; i >= 39; --i)
    {
        
        setCurPos(7, i);
        printf("%s", introductions1);
        setCurPos(7, 78 - i);
        printf("%s", introductions1);
        Sleep(40);
    }
    //从左边一步步进入屏幕中间
    for (i = 0; i <= (80 - strlen(introductions2)) / 2; ++i)
    {
        //要删除这一行缓冲的原因：
       
        delLine(9);
  
        Sleep(10);
        setCurPos(9, i);
        printf("%s", introductions2);
        Sleep(50);
    }
    //从底部进入
    for (i = 24; i >= 12; --i)
    {
        setCurPos(i, (80 - strlen(introductions3)) / 2);
        printf("%s", introductions3);
        Sleep(20);
        //删除上一次的缓冲，不加1的话最后一行就会残留，其它都不见了
        delLine(i + 1);
        Sleep(50);
    }
    Sleep(500);
    char help0[] = "移动：←↑↓→";
    char help1[] = "控制：Space / Enter ";
    char help2[] = "退出：Esc ";

    setCurPos(14, (80 - strlen(help0)) / 2);
    setColor(14);
    printf("%s", help0);
    setCurPos(15, (80 - strlen(help1)) / 2);
    printf("%s", help1);
    setCurPos(16, (80 - strlen(help2)) / 2);
    printf("%s", help2);
   
    setColor(15);
  
    _getch();
}
void gameInitailize()
{
    int i = 0, j = 0;
    int allMines = ALL_MINES;
    //设置随机值
    srand((unsigned int)time(NULL));
    //雷区地图初始化
    for (i = 0; i < ROW; ++i)
    {
        for (j = 0; j < COLUMN; ++j)
        {
            minesMap[i][j].beCovered = true;
            minesMap[i][j].minesNum = 0;
        }
    }
    //放雷！
    while (allMines)
    {
        i = rand() % ROW;
        j = rand() % COLUMN;
        if (minesMap[i][j].minesNum == 0)
        {
            //这个‘-1'作为判断雷的依据
            minesMap[i][j].minesNum = -1;
            --allMines;
        }
    }
    //光标位置初始化
    for (i = 0; i < ROW; ++i)
    {
        for (j = 0; j < COLUMN; ++j)
        {
            cursorPos[i][j].x = j * 6 + 3;
            cursorPos[i][j].y = i * 2 + 1;
        }
    }
}
void countMines()
{
    int i = 0, j = 0, m = 0, n = 0;
    //以格子为中心周围的雷数
    int minesNum = 0;
    for (i = 0; i < ROW; ++i)
    {
        for (j = 0; j < COLUMN; ++j)
        {
            if (minesMap[i][j].minesNum == -1)
                continue;
            minesNum = 0;
          
            for (m = -1; m <= 1; ++m)
            {
            
                if (i + m < 0 || i + m >= ROW)
                {
                    continue;
                }
                for (n = -1; n <= 1; ++n)
                {
                    if (j + n < 0 || j + n >= COLUMN)
                    {
                        continue;
                    }
                   
                    if (minesMap[i + m][j + n].minesNum == -1)
                    {
                        ++minesNum;
                    }
                }
            }
            minesMap[i][j].minesNum = minesNum;
        }
    }
}
void keyBoardInput()
{
    bool lose;
    int key1 = _getch();
    /*****************************
   方向键两个字节
    第一个字节ASCII 0x00e0 224
    第二个字节分别是：
    上：0x0048 72
    下：0x0050 80
    左：0x012b 75
    右：0x012d 77
    *****************************/
    if (key1 == 224)
    {
        int key2 = _getch();
        switch (key2)
        {
        case UP:
            moveCursor(index_y - 1, index_x);
            break;
        case DOWN:
            moveCursor(index_y + 1, index_x);
            break;
        case LEFT:
            moveCursor(index_y, index_x - 1);
            break;
        case RIGHT:
            moveCursor(index_y, index_x + 1);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (key1)
        {
        case ENTER:
        case SPACE:
            lose = checkResult(index_y, index_x);
            system("cls");
            printMap();
            if (lose)
            {
                setColor(13);

                printf("|     按\"r\"重玩，Esc退出。     |\n");
                printf("[%c]-------------------------------------------------------[%c]\n", MINE, MINE);
                setColor(10);
                Sleep(1000);
                char key3 = _getch();
                if (key3 == 'r' || key3 == 'R')
                {
                    //重来，跟main中过程是一样的
                    setColor(10);
                    gameInitailize();
                    countMines();
                    printMap();
                }
            }
            //剩余的格子比雷还要多，可以继续玩
            else if (leftBlocksNum > ALL_MINES)
            {
                setColor(13);
                printf("|     哎哟，挺不错哦~ (￣0 ￣)    |\n");
                printf("[%c]-------------------------------------------------------[%c]\n", MINE, MINE);
                setColor(10);
            }
            //来到这你已经赢了
            else
            {
                setColor(13);
                printf("|    You win(/≧▽≦/)    |\n");
                printf("|     按\"r\"重玩，Esc退出     |\n");
                printf("[%c]-------------------------------------------------------[%c]\n", MINE, MINE);
                setColor(10);
                Sleep(1000);
                char key3 = _getch();
                if (key3 == 'r' || key3 == 'R')
                {
                    setColor(10);
                    gameInitailize();
                    countMines();
                    printMap();
                }
            }
            break;
        case ESC:
            system("cls");

        default:
            break;
        }
    }
}
void setCurPos(int y, int x)
{
    //在窗口缓冲中定义每个位置的状态
    COORD currentPosition;
    currentPosition.Y = y;
    currentPosition.X = x;
    //所以现在的位置是在{y,x}
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), currentPosition);
}
void moveCursor(int y, int x)
{
    //限定能走的地方
    if ((x >= 0 && x < COLUMN) && (y >= 0 && y < ROW))
    {
        setCurPos(cursorPos[y][x].y, cursorPos[y][x].x);
        index_x = x;
        index_y = y;
    }
}
bool checkResult(int y, int x)
{
    int i = 0, j = 0;
    //检测有没有溢出地图了
    if (x < 0 || x >= COLUMN || y < 0 || y >= ROW)
    {
        return false;
    }
   
    minesMap[y][x].beCovered = false;
    //踩雷
    if (minesMap[y][x].minesNum == -1)
    {
        minesMap[y][x].minesNum = 9;
        return true;
    }
  
    if (minesMap[y][x].minesNum > 0 && minesMap[y][x].minesNum < 9)
    {
        return false;
    }
    //九宫格，3x3
    for (i = -1; i <= 1; ++i)
    {
       
        if (y + i < 0 || y + i >= ROW)
        {
            continue;
        }
        for (j = -1; j <= 1; ++j)
        {
            if (x + j < 0 || x + j >= COLUMN)
            {
                continue;
            }
            //如果下一个是没开过的，就检查它吧
            if (minesMap[y + i][x + j].beCovered)
            {
                minesMap[y + i][x + j].beCovered = false;
                checkResult(y + i, x + j);
            }
        }
    }
    return false;
}
void printMap()
{
    system("cls");
    char help0[] = "←↑↓→";
    char help1[] = "移动";
    char help2[] = "Space / Enter";
    char help3[] = "控制";
    char help4[] = "Esc 退出";
    //因为要输出提示，所以地图不能太大了，10x10就差不多了
    setColor(14);
    setCurPos(4, 62);
    printf("%s", help0);
    setCurPos(6, 62);
    printf("%s", help1);
    setCurPos(9, 62);
    printf("%s", help2);
    setCurPos(11, 62);
    printf("%s", help3);
    setCurPos(14, 62);
    printf("%s", help4);
    setCurPos(0, 0);
    setColor(10);
    int i = 0, j = 0, k = 0;
    leftBlocksNum = 0;
    setColor(11);
    printf("[操]--");
    setColor(10);
    for (k = 1; k < COLUMN - 1; ++k)
    {
        printf("+-----");
    }
    setColor(11);
    printf("+--[作]\n");
    setColor(10);
    for (i = 0; i < ROW; ++i)
    {
        for (j = 0; j < COLUMN; ++j)
        {
            if (minesMap[i][j].beCovered)
            {
                ++leftBlocksNum;
                //这个输出的就是格子被覆盖的时候输出的图形，可以换成1-6试试
                //1-4是正方形的4个角，5-6是双竖线和双横线
                printf("| %c ", 3);
            }
            else if (minesMap[i][j].minesNum == -1 || minesMap[i][j].minesNum == 9)
            {
                printf("| %c ", MINE);
            }
            else if (minesMap[i][j].minesNum == 0)
            {
                printf("| %c ", ' ');
            }
            else
            {
                printf("| %d ", minesMap[i][j].minesNum);
            }
        }
        printf("|\n");
        if (i < ROW - 1)
        {
            for (k = 0; k < COLUMN; ++k)
            {
                printf("+-----");
            }
            printf("+\n");
        }
    }
    setColor(11);
    printf("[系]--");
    setColor(10);
    for (k = 1; k < COLUMN - 1; ++k)
    {
        printf("+-----");
    }
    setColor(11);
    printf("+--[统]\n");
    setColor(10);
}
void gameOver(char* str)
{
    setColor(12);
    system("cls");
    setCurPos(10, 0);
    int i = 0;
    do
    {
        //逐字输出
        printf("%c", str[i]);
        Sleep(60);
    } while (str[i++]);
    setColor(15);
    system("pause");
    //随意终止程序并返回给OS，0是正常的
    exit(0);
}
void delLine(int y)
{
    HANDLE hOutput;
    //窗口缓存信息
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    DWORD len, nw;
    //用MSDN上的TCHAR类型跪了，换成char就好
    char fillchar = ' ';
    //定位光标
    COORD startPosition = { 0, y };
    //获取输出句柄
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    //获取窗口缓冲中的信息
    GetConsoleScreenBufferInfo(hOutput, &sbi);
    //窗口缓冲的位置，这里取得X值
    len = sbi.dwSize.X;
    //从特定的位置用特定的字符去填充窗口的缓冲特定次数
    //成功返回非0值，一般都成功，就不判断了
    FillConsoleOutputCharacter(hOutput, fillchar, len, startPosition, &nw);
}