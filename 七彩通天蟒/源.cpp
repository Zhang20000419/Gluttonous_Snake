/*
项目名称：七彩通天蟒
更改日期：2019.12.17
更改人：张哲川
注意：关闭sdl检查
*/
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<iostream> 
#include<windows.h> 
#include<Mmsystem.h> 
#include<math.h> 
#pragma comment(lib,"winmm.lib") 
using namespace std; 
#define r 20
#define LENTH sizeof(struct snake) 
#define PAI 3.14159

void menu();	  				//菜单 
void gamebegin();				//游戏开始 
void gameset();					//游戏设置 
void dirtrol();					//方向控制
void creatfood();				//食物生成
void putpicture();				//输出图片
void death();					//死亡判定
void putscore();				//分数输出
void putsnake();				//打印蛇 
void imageload();				//加载图片
void game();					//游戏主体
void grow();					//判断是否成长
void grass();					//生成毒草
void boom();					//生成炸弹
void magic();					//生成神奇的食物
void victory();					//游戏胜利
void save();					//保存游戏进度
void write();					//读取游戏进度
void goon();					//继续游戏
void read_ranking_list();		//读取排行榜
void write_ranking_list();		//存储排行榜
//void music();					//播放音乐

typedef struct snake			//构成链表的结构体，用来画蛇  
{
	float x; 
	float y; 
	struct snake* next; 
	struct snake* before;
} Snake; 
Snake* head, * tail, * p;	//定义指向蛇头和蛇尾的指针 
int comp=0;						//设置游戏结束条件 
int over;						//死亡判定量 
int score;						//分数  
int time0;						//初始间隔时间
int xs[13], ys[13];				//储存食物，毒草，智慧草的坐标
/*
0:食物坐标
1:尾节点坐标
2~8:毒草坐标
9~11:炸弹坐标
12:智慧草坐标
*/
int create_food = 1, create_magic; //判断是否执行生成
float angle;					//角度
int level;					//游戏难度
int pass=1;						//关卡数
int order1=5, order2=4, order3=5;//设置里的参数 order1:音乐 order2:难度 order3:地图
int num1,num2;					//num1:毒草个数  num2:炸弹个数
int num_grass;					//确定要生成的毒草的编号
int num_boom;					//确定要生成的炸弹的编号
int number1, number2;			//num1和num2的副本
int all;						//蛇身总节数
int exist;						//判断智慧草是否生成
int magic_action;				//神奇的效果
int action;						//申城效果  1:左右反向  2:首位交换
int time_all;					//确定智慧草生成消失时间
int all_copy = 0;				//确定是否重新打印分数
int grass_exist;				//毒草存在
int time_all_grass;				//确定是否生成毒草
int win = 0;					//确认是否胜利
int ranking_list[5];			//排行榜
//int music_put;				//判定是否播放音乐

IMAGE img1[6];					//菜单背景图 
IMAGE img2[12];					//设置背景图
IMAGE img3[5];					//游戏背景图 
IMAGE img4[12];					//蛇图
IMAGE img5[8];					//食物以及炸弹图
IMAGE img6[7];					//关卡

int main() 
{ 
	imageload();
	initgraph(700, 700);
	putimage(0, 0, &img1[0]);
	srand((unsigned)time(NULL));
	setbkmode(TRANSPARENT);
	settextstyle(40, 40, _T("宋体"));
	Sleep(3000);
	initgraph(1200, 700);
	setbkmode(TRANSPARENT);
	while (comp == 0) menu();
	putimage(0, 0, &img1[5]);
	Sleep(3000);
	closegraph();
	return 0;
}

void menu()
{
	putimage(0, 0, &img1[1]);
	read_ranking_list();
	int i;
	putimage(700, 0, &img6[0]);
	settextcolor(RED);
	static TCHAR s[4];
	settextstyle(40, 40, _T("宋体"));
	for (i = 0; i < 5; i++)
	{
		_stprintf_s(s, _T("%d"), ranking_list[i]);
		outtextxy(950, 225+51*i, s);
	}
	//music();
	int line = 1;
	char button;
	int conj=0;					//设置循环结束条件
	while (conj == 0)
	{
		button = _getch();
		if (button == 13)
		{
			switch (line)
			{
			case 1:gamebegin(); break;
			case 2:goon(); break;
			case 3:gameset(); break;
			case 4:comp = 1; break;
			}
			conj = 1;
		}
		else
		{
			if (button == 72)
			{
				if (line == 1) line = 4;
				else line -= 1;
			}
			if (button == 80)
			{
				if (line == 4) line = 1;
				else line += 1;
			}
			putimage(0, 0, &img1[line]);
		}
	}
}

void gamebegin()				//游戏开始
{	
	if (order1 == 5)order1 = rand() % 4 + 1;
	if (order2 == 4)order2 = rand() % 3 + 1;
	if (order3 == 5)order3 = rand() % 4 + 1;
	over = 0;
	all = 4;
	exist = 0;
	win = 0;
	over = 0;
	pass = 1;
	num_grass = 2;
	num_boom = 9;
	magic_action = 0;
	action = 0;
	time_all = 0;
	all_copy = 0;
	grass_exist = 1;
	time_all_grass = 0;
	//music_put = 0;
	Snake* p1;
	if (order2 < 4)
	{
		num1 = 2 * order2 + 1;
		num2 = order2;
	}
	else
	{
		num1 = rand() % 3 * 2 + 3;
		num2 = rand() % 3 + 1;
	}
	number1 = num1;
	number2 = num2;
	angle = PAI / 2;
	score = -1;
	p = (Snake*)malloc(LENTH);
	p->x = 350,p->y = 300;
	p->before = NULL;
	p1 = p;
	head = p;
	p->next = (Snake*)malloc(LENTH);
	p = p->next;
	p->x = 350, p->y = 300 + r / 2;
	p->before = p1;
	p1 = p1->next;
	p->next = (Snake*)malloc(LENTH);
	p = p->next;
	p->x = 350, p->y = 300 + r;
	p->before = p1;
	p1 = p1->next;
	p->next = (Snake*)malloc(LENTH);
	p = p->next;
	p->before = p1;
	p1 = p1->next;
	p->x = 350, p->y = 300 + 3 * r / 2;
	p->next = NULL;
	tail = p;
	game();
}

void gameset()					//游戏设置
{
	order1 = 5;
	order2 = 4;
	order3 = 5;
	putimage(0, 0, &img2[0]);
	settextcolor(RED);
	outtextxy(300, 75, _T("随机"));
	settextcolor(BLUE);
	outtextxy(300, 270, _T("随机"));
	outtextxy(300, 475, _T("随机"));
	int judge = 1;
	int line = 1;
	char ch;
	while (judge)
	{
		ch = _getch();
		if (ch == 13)
		{
			judge = 0;
		}
		else
		{
			if (ch == 80 || ch == 72)
			{
				if (ch == 80)
				{
					if (line != 3)line++;
					else line = 1;
				}
				else
				{
					if (line != 1)line--;
					else line = 3;
				}
			}
			else if (ch == 77 || ch == 75)
			{
				if (ch == 77)
				{
					switch (line)
					{
					case 1:if (order1 != 5)order1++;
						  else order1 = 1; break;
					case 2:if (order2 != 4)order2++;
						  else order2 = 1; break;
					case 3:if (order3 != 5)order3++;
						  else order3 = 1; break;
					}
				}
				else
				{
					switch (line)
					{
					case 1:if (order1 != 1)order1--;
						  else order1 = 5; break;
					case 2:if (order2 != 1)order2--;
						  else order2 = 4; break;
					case 3:if (order3 != 1)order3--;
						  else order3 = 5; break;
					}
				}
			}
			putimage(0, 0, &img2[0]);
			TCHAR str1[5], str2[5], str3[5];
			if (order1 != 5)_stprintf_s(str1, _T("%d"), order1);
			if (order2 != 4)_stprintf_s(str2, _T("%d"), order2);
			if (order3 != 5)_stprintf_s(str3, _T("%d"), order3);
			switch (line)
			{
			case 1:settextcolor(RED);
				if (order1 != 5)outtextxy(360, 75, str1);
				else outtextxy(300, 75, _T("随机"));
				settextcolor(BLUE);
				if (order2 != 4)outtextxy(360, 270, str2);
				else outtextxy(300, 270, _T("随机"));
				if (order3 != 5)outtextxy(360, 475, str3);
				else outtextxy(300, 475, _T("随机"));
				break;
			case 2:settextcolor(BLUE);
				if (order1 != 5)outtextxy(360, 75, str1);
				else outtextxy(300, 75, _T("随机"));
				setcolor(RED);
				if (order2 != 4)outtextxy(360, 270, str2);
				else outtextxy(300, 270, _T("随机"));
				setcolor(BLUE);
				if (order3 != 5)outtextxy(360, 475, str3);
				else outtextxy(300, 475, _T("随机"));
				break;
			case 3:settextcolor(BLUE);
				if (order1 != 5)outtextxy(360, 75, str1);
				else outtextxy(300, 75, _T("随机"));
				if (order2 != 4)outtextxy(360, 270, str2);
				else outtextxy(300, 270, _T("随机"));
				setcolor(RED);
				if (order3 != 5)outtextxy(360, 475, str3);
				else outtextxy(300, 475, _T("随机"));
				break;
			}
		}
	}
}

void dirtrol()					//方向控制 
{
	char button;
	while (_kbhit())
	{
		button = _getch();
		if (magic_action != 2)
		{
			if (button == 75)angle += PAI / 6;
			else if (button == 77)angle -= PAI / 6;
			else if (button == 72)time0 = time0 - 40;
			else if (button == ' ')
			{
				putimage(150, 150, &img6[5]);
				char ch;
				ch = _getch();
				if (ch == 13)
				{
					//保存游戏进度
					save();
				}
			}
		}
		else
		{
			if (button == 75)angle -= PAI / 6;
			else if (button == 77)angle += PAI / 6;
			else if (button == 72)time0 = time0 - 40;
			else if (button == ' ')
			{
				putimage(150, 150, &img6[5]);
				char ch;
				ch = _getch();
				if (ch == 13)
				{
					//保存游戏进度
					save();
				}
			}
		}
	}
	p = head;
	float x1, y1;
	x1 = p->x + r / 2 * cos(angle), y1 = p->y - r / 2 * sin(angle);
	head = (Snake*)malloc(LENTH);
	head->x = x1, head->y = y1;
	head->next = p;
	p->before = head;
	head->before = NULL;
	xs[1] = tail->x;
	ys[1] = tail->y;
	tail = tail->before;
	free(tail->next);
	tail->next = NULL;
}

void creatfood()
{
	if (create_food == 1)
	{
		int grasses;				//用来遍历毒草
		int booms;					//用来遍历炸弹
		Snake* p;
		int x, y;
		int judge1;
		srand((unsigned)time(NULL));
		do
		{
			judge1 = 1;
			int i;
			x = rand() % (700 - 3*r)+r;
			y = rand() % (700 - 3*r)+r;
			p = head;
			while (p && judge1)				//遍历排除蛇身
			{
				if (sqrt(pow(p->x - x, 2) + pow(p->y - y, 2)) < r)
				{
					judge1 = 0;
				}
				else p = p->next;
			}
			if (judge1 == 1)
			{
				if (sqrt(pow(xs[12] - x, 2) + pow(ys[12] - y, 2)) < r)			//遍历排除智慧草
				{
					judge1 = 0;
				}
				else
				{
					for (grasses = num1, i = 2; grasses > 1 && judge1 == 1; grasses--, i++)		//遍历排除毒草
					{
						if (sqrt(pow(xs[i] - x, 2) + pow(ys[i] - y, 2)) < r)
						{
							judge1 = 0;
						}
					}
					for (booms = num2, i = 9; booms >= 1 && judge1 == 1; booms--, i++)			//遍历排除炸弹
					{
						if (sqrt(pow(xs[i] - x, 2) + pow(ys[i] - y, 2)) < r)
						{
							judge1 = 0;
						}
					}
				}
			}
		}
		while (judge1==0);
		xs[0] = x;
		ys[0] = y;
	}
	create_food = 0;
}

void putpicture()
{
	putimage(xs[0], ys[0], &img5[1], SRCAND);
	putimage(xs[0], ys[0], &img5[0], SRCPAINT);
	int i;
	static int bling;
	if (grass_exist)
	{
		if (bling)
		{
			bling = 0;
			for (i = 0; i < num1; i++)
			{
				putimage(xs[i + 2], ys[i + 2], &img5[5], SRCAND);
				putimage(xs[i + 2], ys[i + 2], &img5[4], SRCPAINT);
			}
		}
		else
		{
			bling = 1;
		}
	}
	for (i = 0; i < num2; i++)
	{
		putimage(xs[i + 9], ys[i + 9], &img5[3], SRCAND);
		putimage(xs[i + 9], ys[i + 9], &img5[2], SRCPAINT);
	}
	if (exist)
	{
		putimage(xs[12], ys[12], &img5[7], SRCAND);
		putimage(xs[12], ys[12], &img5[6], SRCPAINT);
	}
}

void death()
{
	p = head;
	if (p->x < 0 || p->y < 0 || p->x>700 - r || p->y>700 - r)
	{
		over = 1;
	}
	else if(all>=4)
	{
		p = head->next;
		p = p->next;
		p = p->next;
		for (; p && over == 0;)
		{
			if (sqrt(pow(head->x - p->x, 2) + pow(head->y - p->y, 2)) < r)
			{
				over = 1;
			}
			else
			{
				p = p->next;
			}
		}
	}
}

void putsnake()					//打印蛇
{
	p = tail;
	int color = 1;
	while (p)
	{
		color = rand() % 11 + 1;
		putimage(p->x, p->y, &img4[0], SRCAND);
		putimage(p->x, p->y, &img4[color], SRCPAINT);
		p = p->before;
	}
}

void imageload()				//加载图片
{
	//菜单背景图
	loadimage(&img1[0], L"./菜单背景图/进入界面.jpg", 700, 700);
	loadimage(&img1[1], L"./菜单背景图/开始游戏.jpg", 700, 700);
	loadimage(&img1[2], L"./菜单背景图/继续游戏.jpg", 700, 700);
	loadimage(&img1[3], L"./菜单背景图/游戏设置.jpg", 700, 700);
	loadimage(&img1[4], L"./菜单背景图/退出游戏.jpg", 700, 700);
	loadimage(&img1[5], L"./菜单背景图/退出界面.jpg", 700, 700);
	//设置背景图
	loadimage(&img2[0], L"./设置的背景图/0.jpg", 700, 700);
	//游戏背景图
	loadimage(&img3[0], L"./游戏背景图/分数显示区背景图.jpg", 500, 700);
	loadimage(&img3[1], L"./游戏背景图/1.jpg", 700, 700);
	loadimage(&img3[2], L"./游戏背景图/2.jpg", 700, 700);
	loadimage(&img3[3], L"./游戏背景图/3.jpg", 700, 700);
	loadimage(&img3[4], L"./游戏背景图/4.jpg", 700, 700);
	//蛇身的颜色图
	loadimage(&img4[0], L"./蛇图/0.jpg", r, r);
	loadimage(&img4[1], L"./蛇图/1.jpg", r, r);
	loadimage(&img4[2], L"./蛇图/2.jpg", r, r);
	loadimage(&img4[3], L"./蛇图/3.jpg", r, r);
	loadimage(&img4[4], L"./蛇图/4.jpg", r, r);
	loadimage(&img4[5], L"./蛇图/5.jpg", r, r);
	loadimage(&img4[6], L"./蛇图/6.jpg", r, r);
	loadimage(&img4[7], L"./蛇图/7.jpg", r, r);
	loadimage(&img4[8], L"./蛇图/8.jpg", r, r);
	loadimage(&img4[9], L"./蛇图/9.jpg", r, r);
	loadimage(&img4[10], L"./蛇图/10.jpg", r, r);
	loadimage(&img4[11], L"./蛇图/11.jpg", r, r);
	//食物以及炸弹
	loadimage(&img5[0], L"./游戏图片/食物.jpg", r, r);
	loadimage(&img5[1], L"./游戏图片/食物0.jpg", r, r);
	loadimage(&img5[2], L"./游戏图片/炸弹.jpg", r, r);
	loadimage(&img5[3], L"./游戏图片/炸弹0.jpg", r, r);
	loadimage(&img5[4], L"./游戏图片/毒草.jpg", r, r);
	loadimage(&img5[5], L"./游戏图片/毒草0.jpg", r, r);
	loadimage(&img5[6], L"./游戏图片/星星.jpg", r, r);
	loadimage(&img5[7], L"./游戏图片/星星0.jpg", r, r);
	//关卡
	loadimage(&img6[0], L"./关卡和分数/排行榜.jpg", 500, 700);
	loadimage(&img6[1], L"./关卡和分数/第一关.jpg", 700, 700);
	loadimage(&img6[2], L"./关卡和分数/第二关.jpg", 700, 700);
	loadimage(&img6[3], L"./关卡和分数/第三关.jpg", 700, 700);
	loadimage(&img6[4], L"./关卡和分数/初始.jpg", 500, 700);
	loadimage(&img6[5], L"./关卡和分数/保存界面.jpg", 400, 400);
	loadimage(&img6[6], L"./关卡和分数/游戏胜利.jpg", 700, 700);
}

void game()
{
	while (pass <= 3 && over == 0)
	{
		
		switch (pass)
		{
		case 1:
			putimage(0, 0, &img6[1]);
			putscore();
			Sleep(2000);
			while (over == 0 && pass == 1)
			{
				/*if (music_put)
				{
					music();
					music_put = 0;
				}*/
				putimage(0, 0, &img3[order3]);
				time0 = 200 - all / 6 * 35 - 20 * pass;
				creatfood();
				grass();
				boom();
				magic();
				dirtrol();
				putsnake();
				putpicture();
				death();
				grow();
				putscore();
				victory();
				Sleep(time0);
			}
			break;
		case 2:
			putimage(0, 0, &img6[2]);
			score--;
			putscore();
			Sleep(2000);
			while (over == 0 && pass == 2)
			{
				/*if (music_put)
				{
					music();
					music_put = 0;
				}*/
				putimage(0, 0, &img3[order3]);
				time0 = 200 - all / 6 * 35;
				creatfood();
				grass();
				boom();
				magic();
				dirtrol();
				putsnake();
				putpicture();
				death();
				grow();
				putscore();
				victory();
				Sleep(time0);
			}
			break;
		case 3:
			putimage(0, 0, &img6[3]);
			score--;
			putscore();
			Sleep(2000);
			while (over == 0 && pass == 3)
			{
				/*if (music_put)
				{
					music();
					music_put = 0;
				}*/
				putimage(0, 0, &img3[order3]);
				time0 = 200 - all / 6 * 35;
				creatfood();
				grass();
				boom();
				magic();
				dirtrol();
				putsnake();
				putpicture();
				death();
				grow();
				putscore();
				victory();
				Sleep(time0);
			}
			break;
		}
		Snake* p1;
		p = head;
		for (; p->next;)
		{
			p1 = p;
			p = p->next;
			free(p1);
		}
		free(p);
		if (pass <= 3)
		{
			all = 4;
			exist = 0;
			num_grass = 2;
			num_boom = 9;
			magic_action = 0;
			action = 0;
			time_all = 0;
			all_copy = 0;
			grass_exist = 1;
			time_all_grass = 0;
			Snake* p1;
			angle = PAI / 2;
			p = (Snake*)malloc(LENTH);
			p->x = 350, p->y = 300;
			p->before = NULL;
			p1 = p;
			head = p;
			p->next = (Snake*)malloc(LENTH);
			p = p->next;
			p->x = 350, p->y = 300 + r / 2;
			p->before = p1;
			p1 = p1->next;
			p->next = (Snake*)malloc(LENTH);
			p = p->next;
			p->x = 350, p->y = 300 + r;
			p->before = p1;
			p1 = p1->next;
			p->next = (Snake*)malloc(LENTH);
			p = p->next;
			p->before = p1;
			p1 = p1->next;
			p->x = 350, p->y = 300 + 3 * r / 2;
			p->next = NULL;
			tail = p;
		}
	}
	if (win)
	{
		putimage(0, 0, &img6[6]);
		Sleep(3000);
	}
	int i, j;
	for (i = 0; i < 5; i++)
	{
		if (ranking_list[i] < score)
		{
			for (j = 4; j > i; j--)
			{
				ranking_list[j] = ranking_list[j - 1];
			}
			ranking_list[i] = score;
			write_ranking_list();
			i = 5;
		}
	}
}

void grow()
{
	Snake* p1;
	if (sqrt(pow(head->x - xs[0], 2) + (int)pow(head->y - ys[0], 2)) < r)			//吃到食物
	{
		PlaySound(TEXT("吃到食物.wav"), NULL, SND_FILENAME | SND_ASYNC);
		//music_put = 1;
		create_food = 1;
		all++;
		p = tail;
		tail = (Snake*)malloc(LENTH);
		tail->x = xs[1];
		tail->y = ys[1];
		p->next = tail;
		tail->before = p;
		tail->next = NULL;

	}
	int i,j;
	int jj;
	j = 0;
	for (i = 2; grass_exist &&i < num1 + 2 && j==0; i++)
	{
		if (sqrt(pow(xs[i] - head->x, 2) + pow(ys[i] - head->y, 2)) < r)			//吃到毒草
		{
			PlaySound(TEXT("吃到毒草.wav"), NULL, SND_FILENAME | SND_ASYNC);
			//music_put = 1;
			j = 1;
			if (all == 1)
			{
				over = 1;
			}
			else
			{
				num_grass = i;
				all--;
				number1 = 1;
				tail = tail->before;
				free(tail->next);
				tail->next = NULL;
			}
		}
	}
	jj = 1;
	for (i = 9; i < num2 + 9 && jj; i++)
	{
		if (sqrt(pow(xs[i] - head->x, 2) + pow(ys[i] - head->y, 2)) < r)			//吃到炸弹
		{
			PlaySound(TEXT("吃到炸弹.wav"), NULL, SND_FILENAME | SND_ASYNC);
			//music_put = 1;
			if (all == 1) over = 1;
			else
			{
				number2 = 1;
				num_boom = i;
				p = head;
				jj = 0;

				all = all / 2;
				for (j =1; j < all; j++)
				{
					p = p->next;
				}
				tail = p;
				p = p->next;
				tail->next = NULL;
				while (p)
				{
					p1 = p->next;
					free(p);
					p = p1;
				}
			}
		}
	}
	if (exist)
	{
		if (sqrt(pow(xs[12] - head->x, 2) + pow(ys[12] - head->y, 2)) < r)				//吃到智慧草
		{
			PlaySound(TEXT("吃到食物.wav"), NULL, SND_FILENAME | SND_ASYNC);
			score = score + 2;
			all_copy--;
			magic_action = action;
			time_all = 0;
			exist = 0;
		}
		else time_all = time_all + time0;
		if (time_all > 15000)
		{
			time_all = 0;
			exist = 0;
		}
	}
	else
	{
		time_all = time_all + time0;
		if (time_all > 15000)
		{
			exist = 1;
			time_all = 0;
			create_magic = 1;
		}
	}
	if (time_all_grass < 10000)
	{
		time_all_grass += time0;
	}
	else
	{
		time_all_grass = 0;
		if (grass_exist)
		{
			grass_exist = 0;
			number1 = num1;
		}
		else grass_exist = 1;           
	}
	if (magic_action == 1)
	{
		Snake* p1,*p2;
		p = head->next;
		p2 = head;
		for (; p;)
		{
			p = head->next;
			p1 = head->before;
			head->next = p1;
			head->before = p;
			if (p)
			{
				head = head->before;
			}
		}
		tail = p2;
		angle = angle + PAI;
		magic_action = 0;
	}
}

void grass()
{
	int i = num_grass;							//num_grass标记被吃掉毒草所在数组的下标
	int judge2;
	int j;
	for (; number1 >= 1;number1--,i++)					//number表示要新生成的毒草个数
	{
		judge2 = 0;
		while (judge2 == 0)
		{
			judge2 = 1;
			xs[i] = rand() % (700 - 3 * r) + r;
			ys[i] = rand() % (700 - 3 * r) + r;
			if (sqrt(pow(xs[i] - xs[0], 2) + pow(ys[i] - ys[0], 2)) < r)		//排除食物
			{
				judge2 = 0;
			}
			else if (sqrt(pow(xs[i] - xs[12], 2) + pow(ys[i] - ys[12], 2)) < r)	judge2 = 0;	//排除智慧草
			else
			{
				for (j = 2; j < 2 + number1 && judge2==1; j++)						//排除其他毒草
				{
					if (i != j && sqrt(pow(xs[i] - xs[j], 2) + pow(ys[i] - ys[j], 2)) < r)
					{
						judge2 = 0;
					}
				}
				if (judge2 == 1)									//排除蛇身
				{
					p = head;
					while (p && judge2)
					{
						if (sqrt(pow(p->x - xs[i], 2) + pow(p->y - ys[i], 2)) < r)
						{
							judge2 = 0;
						}
						else p = p->next;
					}
				}
			}
		}
	}
	number1 = 0;
}

void boom()
{
	int i;
	int j;
	int judge_boom;
	for (i = num_boom; number2 >= 1; number2--, i++)
	{
		judge_boom = 0;
		while (judge_boom == 0)
		{
			judge_boom = 1;
			xs[i] = rand() % (700 - 3 * r) + r;
			ys[i] = rand() % (700 - 3 * r) + r;
			if (sqrt(pow(xs[i] - xs[0], 2) + pow(ys[i] - ys[0], 2)) < r)		//排除食物
			{
				judge_boom = 0;
			}
			else if (sqrt(pow(xs[i] - xs[12], 2) + pow(ys[i] - ys[12], 2)) < r)			//排除智慧草
			{
				judge_boom = 0;
			}
			else
			{
				for (j = 0; j < num1 && judge_boom == 1; j++)				//排除毒草
				{
					if (sqrt(pow(xs[i] - xs[j + 2], 2) + pow(ys[i] - ys[j + 2], 2)) < r)
					{
						judge_boom = 0;
					}
				}
				if (judge_boom && num2 != 1)								//排除炸弹
				{
					for (j = 0; j < num2 && judge_boom == 1; j++)
					{
						if (j + 9 != i && sqrt(pow(xs[i] - xs[j + 9], 2) + pow(ys[i] - ys[j + 9], 2)) < r)
						{
							judge_boom = 0; 
						}
					}
				}
				if (judge_boom)													//排除蛇身
				{
					p = head;
					while (p && judge_boom)
					{
						if (sqrt(pow(p->x - xs[i], 2) + pow(p->y - ys[i], 2)) < r)
						{
							judge_boom = 0;
						}
						else p = p->next;
					}
				}
			}
		}
	}
}

void magic()
{
	int i;
	int judge_magic;
	if (create_magic == 1)
	{
		magic_action = 0;
		action = rand() % 2 + 1;
		judge_magic = 0;
		while (judge_magic == 0)
		{
			judge_magic = 1;
			xs[12] = rand() % (700 - 3 * r) + r;
			ys[12] = rand() % (700 - 3 * r) + r;
			if (sqrt(pow(xs[12] - xs[0], 2) + pow(ys[12] - ys[0], 2)) <r)			//排除食物
			{
				judge_magic = 0;
			}
			else
			{
				for (i = 2; i < num1 + 2 && judge_magic == 1; i++)					//排除毒草
				{
					if (sqrt(pow(xs[i] - xs[12], 2) + pow(ys[i] - ys[12], 2)) < r)
					{
						judge_magic = 0;
					}
				}
				if (judge_magic == 1)							//排除炸弹
				{
					for (i = 9; i < num2 + 9 && judge_magic == 1; i++)
					{
						if (sqrt(pow(xs[i] - xs[12], 2) + pow(ys[i] - ys[12], 2)) < r)
						{
							judge_magic = 0;
						}
					}
					if (judge_magic)
					{
						p = head;
						while (p && judge_magic)
						{
							if (sqrt(pow(xs[12] - p->x, 2) + pow(ys[12] - p->y, 2)) < r)
							{
								judge_magic = 0;
							}
							else p = p->next;
						}
					}
				}
			}
		}
		create_magic = 0;
	}
}

void victory()
{
	if (all >= 15+5*pass)
	{
		if (pass <= 2)
		{
			pass++;
		}
		else
		{
			pass++;
			win = 1;
			over = 1;
		}

	}
}

void putscore()
{
	if (all != all_copy)
	{
		putimage(700, 0, &img3[0]);
		if (all > all_copy)
		{
			score = score + 1;
		}
		else
		{
			score = score - 2 * (all_copy - all);
		}
		settextcolor(BLACK);
		static TCHAR s[4];
		setbkmode(TRANSPARENT);
		settextstyle(40, 40, _T("宋体"));
		_stprintf_s(s, _T("%d"), score);
		outtextxy(900, 200, s);
		all_copy = all;
	}
}

void save()
{
	int i;
	FILE* q;
	errno_t err;
	if ((err = fopen_s(&q, "./游戏进度", "wt+")) == 0)
	{
		fprintf(q, "%d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", comp, over, score, time0, create_food, create_magic, angle, level, pass, order1, order2, order3, num1, num2, num_grass, num_boom, number1, number2, all, exist, magic_action, action, time_all, grass_exist, time_all_grass, win);
		for (i = 0; i < 13; i++)
		{
			fprintf(q, " %d", xs[i]);
		}
		for (i = 0; i < 13; i++)
		{
			fprintf(q, " %d", ys[i]);
		}
		p = head;
		for (; p;)
		{
			fprintf(q, " %f %f", p->x, p->y);
			p = p->next;
		}
		fclose(q);
	}
}

void write()
{
	Snake* p1;
	FILE* q;
	errno_t err;
	int i;
	if ((err = fopen_s(&q, "./游戏进度", "r")) == 0)
	{
		fscanf(q, "%d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &comp, &over, &score, &time0, &create_food, &create_magic, &angle, &level, &pass, &order1, &order2, &order3, &num1, &num2, &num_grass, &num_boom, &number1, &number2, &all, &exist, &magic_action, &action, &time_all, &grass_exist, &time_all_grass, &win);
		for (i = 0; i < 13; i++)
		{
			fscanf(q, " %d", &xs[i]);
		}
		for (i = 0; i < 13; i++)
		{
			fscanf(q, " %d", &ys[i]);
		}
		
		head = (Snake*)malloc(LENTH);
		fscanf(q, " %f %f", &head->x, &head->y);
		head->before = NULL;
		p1 = head;
		for (i = 1; i < all; i++)
		{
			p = (Snake*)malloc(LENTH);
			fscanf(q, " %f %f", &(p->x), &(p->y));
			p->before = p1;
			p1->next = p;
			p1 = p;
		}
		p->next = NULL;
		tail = p;
		fclose(q);
	}
}

void goon()
{
	write();
	game();
}

void write_ranking_list()
{
	FILE* q;
	errno_t err;
	if ((err = fopen_s(&q, "./排行榜", "wt+")) == 0)
	{
		fprintf(q, "%d %d %d %d %d", ranking_list[0], ranking_list[1], ranking_list[2], ranking_list[3], ranking_list[4]);
		fclose(q);
	}
}

void read_ranking_list()
{
	FILE* q;
	errno_t err;
	if ((err = fopen_s(&q, "./排行榜", "r")) == 0)
	{
		fscanf(q, "%d %d %d %d %d", &ranking_list[0], &ranking_list[1], &ranking_list[2], &ranking_list[3], &ranking_list[4]);
		fclose(q);
	}
}
/*void music()
{
	switch (order1)
	{
	case 1:PlaySound(TEXT("背景音乐1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	case 2:PlaySound(TEXT("背景音乐2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	case 3:PlaySound(TEXT("背景音乐3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	case 4:PlaySound(TEXT("背景音乐4.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
}*/