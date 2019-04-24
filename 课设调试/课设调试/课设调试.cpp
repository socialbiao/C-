#include "pch.h"
#include<iostream>
#include<vector>
#include<string>
#include<fstream>//读写文件头函数
#include<random>
#include<time.h>
#include<stdlib.h>
#include<windows.h>//sleep()函数调用
#include<stdio.h>
using namespace std;


//全局常量的定义
 int wall, castle, monster, brick, jewel, output, huihe, cardnumber;
 int  Original_Wall = 10;
 int Original_Castle = 25;
 int Original_Monster = 10, Original_Brick = 10, Original_Jewel = 10;
 char File_Name_Deck[20] = "..\\txt\\deck.txt";//牌
 int OriginalOutput = 2;
 int Original_CardNum = 6;
 int RoundsSum = 60;
 int Line = 90;
 int winrecouce = 50;
 int wincastle = 50;
 char playername1[100], playername2[100];
 int n;//选择模式
//全局变量的定义
//卡牌结构的定义
struct MagicCard {
	string CardName = "无";
	int M_Castle = 0;//卡牌对我方城堡的效果
	int M_Wall = 0;//卡牌对我方城墙的效果
	int M_Monster = 0;//卡牌对我方怪兽的加成
	int M_Brick = 0;//卡牌对我方砖头的加成
	int M_Jewel = 0;//卡牌对我方宝石的加成
	int O_Castle = 0;//
	int O_Wall = 0;//
	int O_Monster = 0;//
	int O_Brick = 0;//
	int O_Jewel = 0;//
	int Attack = 0;
	bool Is_PlayAgain = 0;//
	bool IS_Special = 0;//
	int SpecialNumber = 0;//对于Is_Special的完善——由于字符串匹配存在问题只能再新开一个
	string Function = "无";//效果的文字表述，用于展示，
};
vector<MagicCard> deck;//游戏中的81张卡的卡组
//玩家结构的定义
struct Player {
	int BrickOutput = OriginalOutput;
	int JewelOutput = OriginalOutput;
	int MonsterOutput = OriginalOutput;
	int PlayerMonster = Original_Monster;
	int PlayerBrick = Original_Brick;
	int PlayerJewel = Original_Jewel;
	int PlayerCastle = Original_Castle;
	int PlayerWall = Original_Wall;
	vector<MagicCard> Hand;//手牌
};
//初始化读取卡组文件(已完成)
void Read_The_Card(vector<MagicCard> &Card) {
	ifstream Infile(File_Name_Deck, ios::in);
	if (!Infile.is_open())
		cout << "未能成功找到卡组文件！" << endl;
	string Newline_variable;//换行变量；
	getline(Infile, Newline_variable);//先换一行
	MagicCard TempCard;
	while (!Infile.eof()) //判断是否读取到文件末尾
	{
		Infile >> TempCard.CardName >> TempCard.M_Castle >> TempCard.M_Wall >> TempCard.M_Monster >> TempCard.M_Brick >> TempCard.M_Jewel >> TempCard.O_Castle >> TempCard.O_Wall >> TempCard.O_Monster >> TempCard.O_Brick >> TempCard.O_Jewel >> TempCard.Is_PlayAgain >> TempCard.IS_Special >> TempCard.Attack >> TempCard.SpecialNumber >> TempCard.Function;
		Card.push_back(TempCard);
	}
	Card.pop_back();
	Infile.close();
};
//发牌（已完成）
void Deal_Card(vector<MagicCard> &Hand) {
	if (n == 1 || n == 2)
	{
		unsigned int DeckNumber = deck.size();
		int RandomNumber = rand() % DeckNumber;
		MagicCard TempCard = deck[RandomNumber];
		Hand.push_back(TempCard);
	}
	if (n == 3)
	{
		unsigned int DeckNumber = deck.size();
		for (int i = 0; i < DeckNumber; i++)
		{
			MagicCard TempCard = deck[i];
			Hand.push_back(TempCard);
		}
	}
}
//初始化（已完成）
void  Initialization(Player &Player1, Player &Player2) {
	Read_The_Card(deck);
	for (int i = 0; i < Original_CardNum; i++) {
		Deal_Card(Player1.Hand);
	}
	for (int i = 0; i < Original_CardNum; i++) {
		Deal_Card(Player2.Hand);
	}
}
//展示玩家的情况（已完成）
void Show_Player_Situation(Player Player1, Player Player2, int RoundsNum) {
	cout << playername1 << "						" << "回合" << "(" << RoundsNum << "/" << RoundsSum << ")" << "						" << playername2 << endl;
	cout << "砖头  " << Player1.PlayerBrick << "(+" << Player1.BrickOutput << ")  ";
	cout << "宝石  " << Player1.PlayerJewel << "(+" << Player1.JewelOutput << ")  ";
	cout << "怪物  " << Player1.PlayerMonster << "(+" << Player1.MonsterOutput << ")  ";
	cout << "					";
	cout << "砖头  " << Player2.PlayerBrick << "(+" << Player2.BrickOutput << ")  ";
	cout << "宝石  " << Player2.PlayerJewel << "(+" << Player2.JewelOutput << ")  ";
	cout << "怪物  " << Player2.PlayerMonster << "(+" << Player2.MonsterOutput << ")  " << endl;
}
//创建txt文件 d:\\situation.txt
void CreatTxt(MagicCard LastCard, int RoundsNum)
{
	ofstream outfile("..\\txt\\situation.txt", ios::app);

	if (outfile) { // 如果创建成功
		int flag = RoundsNum % 2 == 0 ? 1 : 2;

		outfile << "Player" << flag << "第" << flag << "回合出牌：" << "<" << LastCard.CardName << ">" << LastCard.Function << endl;
		outfile.close();  // 执行完操作后关闭文件句柄
	}
}
//展示上回合的出牌（已完成）
void Show_The_last_Card(MagicCard LastCard, int RoundsNum) {
	int flag = RoundsNum % 2 == 0 ? 1 : 2;
	cout << "				" << "Player" << flag << "上回合出牌：" << "<" << LastCard.CardName << ">" << LastCard.Function << endl;
	cout << endl << endl;
	CreatTxt(LastCard, RoundsNum);
}
//上取整的函数(已完成)在下面的函数中有用到
int Ceil(double x) {
	int x1 = int(x);
	if (x - x1 == 0)
		return x1;
	else
		return x1 + 1;
}
//展示双方的城堡和城墙（已完成）
void Show_Player_Blood(Player Player1, Player Player2) {
	int P1CLine = Ceil(double(Player1.PlayerCastle) / 5.0);
	int P2CLine = Ceil(double(Player2.PlayerCastle) / 5.0);
	int P1WLine = Ceil(double(Player1.PlayerWall) / 5.0);
	int P2WLine = Ceil(double(Player2.PlayerWall) / 5.0);
	int Cmax = P1CLine > P2CLine ? P1CLine : P2CLine;
	int Wmax = P1WLine > P2WLine ? P1WLine : P2WLine;
	int max = Cmax > Wmax ? Cmax : Wmax;
	for (int i = max; i > 0; i--) {
		cout << "   ";
		if (i > P1CLine)
			cout << "     ";
		else if (i == P1CLine) {
			int temp = Player1.PlayerCastle % 5;
			if (temp == 0)
				cout << "TTTTT";
			else {
				for (int temp1 = 0; temp1 < temp; temp1++)
					cout << "T";
				for (int temp2 = 0; temp2 < 5 - temp; temp2++)
					cout << " ";
			}

		}
		else {
			cout << "TTTTT";
		}
		cout << "   ";
		if (i > P1WLine)
			cout << "     ";
		else if (i == P1WLine) {
			int temp = Player1.PlayerWall % 5;
			if (temp == 0)
				cout << "WWWWW";
			else {
				for (int temp1 = 0; temp1 < temp; temp1++)
					cout << "W";
				for (int temp2 = 0; temp2 < 5 - temp; temp2++)
					cout << " ";
			}
		}
		else {
			cout << "WWWWW";
		}
		//打印空格
		for (int j = 0; j < Line; j++) {
			cout << " ";
		}
		//打印空格
		if (i > P2WLine)
			cout << "     ";
		else if (i == P2WLine) {
			int temp = Player2.PlayerWall % 5;
			if (temp == 0)
				cout << "WWWWW";
			else {
				for (int temp2 = 0; temp2 < 5 - temp; temp2++)
					cout << " ";
				for (int temp1 = 0; temp1 < temp; temp1++)
					cout << "W";
			}
		}
		else {
			cout << "WWWWW";
		}
		cout << "   ";
		if (i > P2CLine)
			cout << "     ";
		else if (i == P2CLine) {
			int temp = Player2.PlayerCastle % 5;
			if (temp == 0)
				cout << "TTTTT";
			else {
				for (int temp2 = 0; temp2 < 5 - temp; temp2++)
					cout << " ";
				for (int temp1 = 0; temp1 < temp; temp1++)
					cout << "T";
			}
		}
		else {
			cout << "TTTTT";
		}

		cout << endl;
	}
	cout << "   " << Player1.PlayerCastle  << "      " << Player1.PlayerWall;
	//打印空格
	for (int j = 0; j < Line; j++) {
		cout << " ";
	}
	cout << "     " << Player2.PlayerWall <<"      " << Player2.PlayerCastle << endl;

}
//展示手牌（已完成）
void Show_Player_Hand(Player ThePlayer) {
	for (int i = 0; i < ThePlayer.Hand.size(); i++)
		cout << i + 1 << ":" << "<" << ThePlayer.Hand[i].CardName << ">" << ThePlayer.Hand[i].Function << endl;
}
//展示所有的部分(对以上函数的封装)（已完成）
void Show_All(Player Player1, Player Player2, int RoundsNum, MagicCard LastCard) {
	Show_Player_Situation(Player1, Player2, RoundsNum);
	Show_The_last_Card(LastCard, RoundsNum);
	Show_Player_Blood(Player1, Player2);
	if (RoundsNum % 2 == 0) {
		cout << playername2 << "的手牌：" << endl;
		Show_Player_Hand(Player2);
	}
	else {
		cout << playername1 << "的手牌：" << endl;
		Show_Player_Hand(Player1);
	}
}
//刷屏后重新打印所有（已完成）
void Refresh(Player Player1, Player Player2, int RoundsNum, MagicCard LastCard) {
	system("Cls");
	Show_All(Player1, Player2, RoundsNum, LastCard);

}
//攻击操作（已完成）
void Attack(Player &ThePlayer, int AttackPoints) {
	if (ThePlayer.PlayerWall >= AttackPoints)
		ThePlayer.PlayerWall -= AttackPoints;
	else {
		int temp = AttackPoints - ThePlayer.PlayerWall;
		ThePlayer.PlayerCastle -= temp;
		ThePlayer.PlayerWall = 0;
	}
}
//卡牌特殊技能操作函数较长，稍后再写。(已完成)
void SocialCard(Player &SPlayer, Player &OPlayer, MagicCard TheCard)
{
	switch (TheCard.SpecialNumber)
	{
	case 0:break;
	case 1: SPlayer.BrickOutput += 1; break;
	case 2:
		if (SPlayer.BrickOutput < OPlayer.BrickOutput)
		{
			SPlayer.BrickOutput += 2;
		}
		else SPlayer.BrickOutput += 1; break;

	case 3: SPlayer.BrickOutput += 1; break;
	case 4:
		if (SPlayer.BrickOutput < OPlayer.BrickOutput)
		{
			SPlayer.BrickOutput = OPlayer.BrickOutput;
		}break;
	case 5:
		SPlayer.BrickOutput += 1;
		OPlayer.BrickOutput += 1;
		break;
	case 6:
		if (SPlayer.PlayerWall == 0)
		{
			SPlayer.PlayerWall += 6;
		}
		else SPlayer.PlayerWall += 3; break;
	case 7:
		SPlayer.PlayerWall -= 5;
		OPlayer.PlayerWall -= 5; break;
	case 8:
		SPlayer.JewelOutput += 1; break;
	case 9:
		SPlayer.BrickOutput += 1;
		OPlayer.BrickOutput += 1;
		break;
	case 10:
		OPlayer.BrickOutput -= 1; break;
	case 11:
		SPlayer.BrickOutput += 2; break;
	case 12:
		SPlayer.BrickOutput -= 1; break;
	case 13:
		SPlayer.MonsterOutput += 1; break;
	case 14:
		SPlayer.JewelOutput += 1; break;
	case 15:
		break;
	case 16:
		//这张牌不能换只能使用；
	case 17:
		SPlayer.JewelOutput += 1; break;
	case 18:
		SPlayer.JewelOutput += 2; break;
	case 19:
		SPlayer.JewelOutput += 1; break;
	case 20:
		int max;
		max = SPlayer.JewelOutput > OPlayer.JewelOutput ? SPlayer.JewelOutput : OPlayer.JewelOutput;
		SPlayer.JewelOutput = max;
		OPlayer.JewelOutput = max;
		break;
	case 21:
		SPlayer.JewelOutput += 1; break;
	case 22:
		SPlayer.JewelOutput -= 1; break;
	case 23:
		SPlayer.JewelOutput -= 1; break;
	case 24:
		SPlayer.MonsterOutput += 1; break;
	case 25:
		SPlayer.MonsterOutput += 1; break;
	case 26:
		break;
	case 27:
		if (SPlayer.PlayerWall >= 3)
			SPlayer.PlayerWall -= 3;
		else {
			int temp = 3 - SPlayer.PlayerWall;
			SPlayer.PlayerCastle -= temp;
			SPlayer.PlayerWall = 0;
		}break;
	case 28:
		SPlayer.MonsterOutput += 2; break;
	case 29:
		SPlayer.MonsterOutput += 1; break;
		OPlayer.MonsterOutput += 1; break;
	case 30:
		if (OPlayer.PlayerWall == 0)
			OPlayer.PlayerCastle -= 10;
		else
		{
			int temp = 6 - OPlayer.PlayerWall;
			OPlayer.PlayerCastle -= temp;
			OPlayer.PlayerWall = 0;
		}break;
	case 31:
		if (OPlayer.PlayerWall == 0)
			OPlayer.PlayerCastle -= 7;
		else
		{
			int temp = 10 - OPlayer.PlayerWall;
			OPlayer.PlayerCastle -= temp;
			OPlayer.PlayerWall = 0;
		}break;
	case 32:
		if (SPlayer.JewelOutput > OPlayer.JewelOutput)
		{
			if (OPlayer.PlayerWall > 12)
			{
				OPlayer.PlayerWall -= 12;
			}
			else
			{
				int temp = 12 - OPlayer.PlayerWall;
				OPlayer.PlayerCastle -= temp;
				OPlayer.PlayerWall = 0;
			}
		}
		else
		{
			if (OPlayer.PlayerWall > 8)
			{
				OPlayer.PlayerWall -= 8;
			}
			else
			{
				int temp = 8 - OPlayer.PlayerWall;
				OPlayer.PlayerCastle -= temp;
				OPlayer.PlayerWall = 0;
			}
		}break;
	case 33:
		if (SPlayer.PlayerWall < OPlayer.PlayerWall)
		{
			if (OPlayer.PlayerWall > 6)
			{
				OPlayer.PlayerWall -= 6;
			}
			else
			{
				int temp = 6 - OPlayer.PlayerWall;
				OPlayer.PlayerCastle -= temp;
				OPlayer.PlayerWall = 0;
			}
		}
		else
		{
			OPlayer.PlayerCastle -= 5;
		}break;
	case 34:
		OPlayer.BrickOutput -= 1; break;
	case 35:
		SPlayer.BrickOutput -= 1; break;
	case 36:
		SPlayer.BrickOutput -= 1; break;

	}

}
//普通操作（已完成）
void Ui_Controls(Player &SPlayer, Player &OPlayer, MagicCard TheCard) {
	SPlayer.PlayerBrick += TheCard.M_Brick;
	SPlayer.PlayerCastle += TheCard.M_Castle;
	SPlayer.PlayerWall += TheCard.M_Wall;
	SPlayer.PlayerMonster += TheCard.M_Monster;
	SPlayer.PlayerJewel += TheCard.M_Jewel;
	OPlayer.PlayerBrick += TheCard.O_Brick;
	OPlayer.PlayerCastle += TheCard.O_Castle;
	OPlayer.PlayerWall += TheCard.O_Wall;
	OPlayer.PlayerMonster += TheCard.O_Monster;
	OPlayer.PlayerJewel += TheCard.O_Jewel;
}
//弃牌操作  完成  
void Drsert_Car(Player &SPlayer, Player &OPlayer, MagicCard TheCard,int  *flag)
{
	if (SPlayer.PlayerMonster + TheCard.M_Monster < 0 || SPlayer.PlayerBrick + TheCard.M_Brick < 0 || SPlayer.PlayerJewel + TheCard.M_Jewel < 0)
	{
		*flag = 1;
	}
	else
	{
		*flag = 0;
	}
}
//输入玩家姓名(完成)
void Inputname()
{
	system("Cls");
	int i;
	for (i = 0; i <= 4; i++)
	{
		cout << endl;

	}
	cout << "请输入玩家1的姓名：" << endl;
	cin.ignore();
	cin.getline(playername1, 100);
	cout << "请输入玩家2的姓名：" << endl;
	cin.getline(playername2, 100);
}
//配置游戏参数，并创建txt文档进行查看
void configure()
{
	int n, m, j;
	system("Cls");
	int i;
	for (i = 0; i <= 5; i++)
	{
		cout << endl;
	}
	cout << "                                        游戏参数如下：" << endl;
	cout << "             " << " 1:Original_Wall    " << Original_Wall << "             " << "2:Original_Castle " << Original_Castle << endl;
	cout << "             " << " 3:Original_Monster " << Original_Monster << "             " << "4:Original_Brick  " << Original_Brick << endl;
	cout << "             " << " 5:Original_Jewel   " << Original_Jewel << "             " << "6:OriginalOutput  " << OriginalOutput << endl;
	cout << "             " << " 7:Original_CardNum " << Original_CardNum << "             " << "8:RoundsSum       " << RoundsSum << endl;
	cout << "             " << " 9:winrecouce       " << winrecouce << "             " << "10:wincastle      " << wincastle << endl;
	cout << "          先输入修改次数 ,再输入数字进行相对应的改变（例如输入1，1则对Original_wall进行修改），输入0则视为退出" << endl;
	cin >> j;
	while (j--)
	{
		cin >> n;
		if (n == 1)
		{
			cout << "                              Original_Wall:      ";
			cin >> m;
			Original_Wall = m;
		}
		if (n == 2)
		{
			cout << "                              Original_Castle:    ";
			cin >> m;
			Original_Castle = m;
		}
		if (n == 3)
		{
			cout << "                              Original_Monster:   ";
			cin >> m;
			Original_Monster = m;
		}
		if (n == 4)
		{
			cout << "                              Original_Brick:     ";
			cin >> m;
			Original_Brick = m;
		}if (n == 5)
		{
			cout << "                              Original_Jewel:     ";
			cin >> m;
			Original_Jewel = m;
		}
		if (n == 6)
		{
			cout << "                              OriginalOutput:     ";
			cin >> m;
			OriginalOutput = m;
		}
		if (n == 7)
		{
			cout << "                              Original_CardNum:    ";
			cin >> m;
			Original_CardNum = m;
		}
		if (n == 8)
		{
			cout << "                                RoundsSum:         ";
			cin >> m;
			RoundsSum = m;
		}
		if (n == 9)
		{
			cout << "                                winrecouce:         ";
			cin >> m;
			winrecouce = m;
		}
		if (n == 10)
		{
			cout << "                                wincastle：         ";
			cin >> m;
			wincastle = m;
		}
	}


	ofstream outfile("..\\txt\\configure.txt", ios::app);

	if (outfile) {
		outfile << "                                 游戏参数如下：" << endl;
		outfile << "             " << " 1:Original_Wall    = " << Original_Wall << "             " << "2:Original_Castle = " << Original_Castle << endl;
		outfile << "             " << " 3:Original_Monster = " << Original_Monster << "             " << "4:Original_Brick  = " << Original_Brick << endl;
		outfile << "             " << " 5:Original_Jewel   = " << Original_Jewel << "             " << "6:OriginalOutput  = " << OriginalOutput << endl;
		outfile << "             " << " 7:Original_CardNum = " << Original_CardNum << "              " << "8:RoundsSum       = " << RoundsSum << endl;
		outfile << "             " << " 9:winrecouce       = " << winrecouce << "             " << "10:wincastle      = " << wincastle << endl;
		outfile.close();
	}
}
//开始界面 完成
void Startgame()
{
	int i;
	for (i = 0; i <= 4; i++)
	{
		cout << endl;
	}
	cout << "                            ***********************" << endl;
	Sleep(500);
	cout << "                             **                 **" << endl;
	Sleep(500);
	cout << "                              **               **" << endl;
	Sleep(500);
	cout << "                               **             **" << endl;
	Sleep(500);
	cout << "                                 **          **" << endl;
	Sleep(500);
	cout << "                                  **        **" << endl;
	Sleep(500);
	cout << "                                    **    **" << endl;
	Sleep(500);
	cout << "                                      **** " << endl;
	Sleep(500);
	cout << "                                    **    **" << endl;
	Sleep(500);
	cout << "                                  **        **" << endl;
	Sleep(500);
	cout << "                                 **          **" << endl;
	Sleep(500);
	cout << "                               **             **" << endl;
	Sleep(500);
	cout << "                              **               **" << endl;
	Sleep(500);
	cout << "                             **                 **" << endl;
	Sleep(500);
	cout << "                            ***********************" << endl;
	Sleep(500);
	cout << "                                ~欢迎来到魔幻牌~                   " << endl;
	Sleep(500);
	cout << "                           请输入1或2进入相对应的模式  " << endl;
	Sleep(500);
	cout << "                                 1:进入玩家对战                " << endl;
	Sleep(500);
	cout << "                                 2:人机对战" << endl;
	Sleep(500);
	cout << "                                 3:测试牌的功能 " << endl;
	Sleep(500);
	cout << "                                 您选择的模式是：";
	Sleep(500);
	cin >> n;
	if (n == 1)
	{
		Inputname();
		configure();

	}
	//人机对战
	if (n == 2)
	{
		Inputname();
		configure();
	}
	if (n == 3)
	{
		configure();
	}


}
//出牌操作(对以上函数的封装)(完成了一小部分)
MagicCard SendCard1(Player &SPlayer, Player &OPlayer, int RoundsNum) {
	int choice;
	//Choose(SPlayer, RoundsNum);
	if (RoundsNum % 2 == 0)
		cout << playername2 << "出牌：请选择您的卡片（输入编号即可），若出牌阶段所出牌不符合出牌条件则视为弃牌" << endl;
	else
		cout << playername1 << "出牌：请选择您的卡片（输入编号即可），若出牌阶段所出牌不符合出牌条件则视为弃牌" << endl;
	for (;;) {
		cin >> choice;
		choice--;
		if (choice<0 || choice>SPlayer.Hand.size())
			cout << "输入超限，请重新输入" << endl;
		else
			break;
	}
	int flag=1;
    Drsert_Car(SPlayer, OPlayer, SPlayer.Hand[choice],&flag);//判断牌是否能出

	if (flag == 0) 
	{
		Ui_Controls(SPlayer, OPlayer, SPlayer.Hand[choice]);
		SocialCard(SPlayer, OPlayer, SPlayer.Hand[choice]);
		if (SPlayer.Hand[choice].Attack != 0)
			Attack(OPlayer, SPlayer.Hand[choice].Attack);
	}



    MagicCard Goal = SPlayer.Hand[choice];
	SPlayer.Hand.erase(SPlayer.Hand.begin() + choice);//发出的牌移除手牌;
	return Goal;
}
//出牌操作 人机
MagicCard SendCard2(Player &SPlayer, Player &OPlayer, int RoundsNum)
{
	if (RoundsNum % 2 == 0)
	{
		cout << "电脑出牌回合" << endl;
	}
	else
		cout << "玩家：" << "出牌：请选择您的卡片（输入编号即可），若出牌阶段所出牌不符合出牌条件则视为弃牌" << endl;
	int i;
	int choice = 1;
	for (i = 0; i < SPlayer.Hand.size(); i++)
		{
			
			int flag = 1;
			Drsert_Car(SPlayer, OPlayer, SPlayer.Hand[i], &flag);//判断牌是否能出
			if (flag == 0)
			{
				//if ();
				choice = i;
				Ui_Controls(SPlayer, OPlayer, SPlayer.Hand[choice]);
				SocialCard(SPlayer, OPlayer, SPlayer.Hand[choice]);
				if (SPlayer.Hand[choice].Attack != 0)
					Attack(OPlayer, SPlayer.Hand[choice].Attack);
				break;
			}
			if (i == SPlayer.Hand.size())
			{
				choice = 1;
				break;

			}
		}
	cout << "电脑已选择的牌是" << choice+1;
	Sleep(900);
	MagicCard Goal = SPlayer.Hand[choice];
	SPlayer.Hand.erase(SPlayer.Hand.begin() + choice);//发出的牌移除手牌;
	return Goal;

}
//判断胜负操作  布尔类型的函数（在出牌操作后进行）
bool Is_Win(Player Player1, Player Player2, int R , int winrecouce, int wincastle) {
	if (R == 60) {
		cout << "双方已各下30回合，两者皆胜" << endl;
		return 1;
	}
	else if (Player1.PlayerBrick >= winrecouce || Player1.PlayerMonster >= winrecouce || Player1.PlayerJewel >= winrecouce || Player1.PlayerCastle >= wincastle || Player2.PlayerCastle <= 0) {
		cout << "					Player1 wins!游戏结束！" << endl;
		return 1;
	}
	else if (Player2.PlayerBrick >= winrecouce || Player2.PlayerMonster >= winrecouce || Player2.PlayerJewel >= winrecouce || Player2.PlayerCastle >= wincastle || Player1.PlayerCastle <= 0) {
		cout << "					Player2 wins!游戏结束！" << endl;
		return 1;
	}
	else
		return 0;
}
//资源判断
void Resource_Chick(Player &ThePlayer)
{
	if (ThePlayer.BrickOutput < 0)
	{
		ThePlayer.BrickOutput = 0;
	}
	if (ThePlayer.MonsterOutput < 0)
	{
		ThePlayer.MonsterOutput = 0;
	}
	if (ThePlayer.JewelOutput < 0)
	{
		ThePlayer.JewelOutput = 0;
	}
	if (ThePlayer.PlayerWall < 0)
	{
		ThePlayer.PlayerWall = 0;
	}
}
//资源更新（已完成）
void Resource_Update(Player &ThePlayer) {
	system("Cls");
	int i;
	for (i = 0; i <= 4; i++)
	{
		cout << endl;
	}
	cout << "                新的回合开始，玩家资源更新中……" << endl;//可以让其输出在屏幕中央

	ThePlayer.PlayerBrick += ThePlayer.BrickOutput;
	ThePlayer.PlayerMonster += ThePlayer.MonsterOutput;
	ThePlayer.PlayerJewel += ThePlayer.JewelOutput;
	Sleep(700);
	system("Cls");
}
int main() {
	Startgame();
	system("Cls");
	int RoundsNum = 1;//用来记录回合数的变量
	MagicCard LastCard;//用来记录上回合的出牌
	srand((unsigned)time(NULL));
	Player p1, p2;
	Initialization(p1, p2);
	MagicCard EmptyCard = { "无",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"无" };//初始化一张空牌
	Show_All(p1, p2, RoundsNum, EmptyCard);//展示桌面
	Sleep(1000);
	Resource_Update(p1);//玩家一资源更新
	Resource_Chick(p1);
	Show_All(p1, p2, RoundsNum, EmptyCard);//展示桌面
	for (;;) {
		bool flag = 1;// 重新打牌效果的标志
		while (flag) {
			LastCard = SendCard1(p1, p2, RoundsNum);//p1打牌
			flag = LastCard.Is_PlayAgain;
			Refresh(p1, p2, RoundsNum, LastCard);//展示发牌后的桌面
			if (Is_Win(p1, p2, RoundsNum, winrecouce, wincastle))//如果p1赢了，游戏结束
				goto T1;
		}
		flag = 1;
		Deal_Card(p1.Hand);//如果p1没赢，给p1发牌
		RoundsNum++;//回合数加一
		Resource_Update(p2);//玩家二资源更新
		Resource_Chick(p2);
		Refresh(p1, p2, RoundsNum, LastCard);//到了p2的回合
		while (flag) {
			if (n == 1)
			{
				LastCard = SendCard1(p2, p1, RoundsNum);//p2打牌
			}
			if (n == 2||n==3)
			{
				LastCard = SendCard2(p2, p1, RoundsNum);//人机打牌
			}
			flag = LastCard.Is_PlayAgain;
			Refresh(p1, p2, RoundsNum, LastCard);//展示打牌后的桌面
			if (Is_Win(p1, p2, RoundsNum, winrecouce, wincastle))//如果p2赢了，游戏结束
				goto T1;
		}
		Deal_Card(p2.Hand);//如果p2没赢，给p2发牌
		RoundsNum++;//回合数加一
		Resource_Update(p1);//玩家一资源更新
		Refresh(p1, p2, RoundsNum, LastCard);//开始新的一轮
	}
T1:		Startgame();
}