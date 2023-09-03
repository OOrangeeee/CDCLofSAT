/*最后编辑：*/
/*晋晨曦 2023.9.4 00:01*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#define WIN32_LEAN_AND_MEAN
#include"others.h"
#include <windows.h>

void SetColor(int ForgC)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void Start()
{
	SetColor(3);
	cout << "\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~" << "\n";
	cout << "||" << "												     " << "||" << "\n";
	cout << "||";
	SetColor(11); // 设置颜色为淡蓝色
	cout << "					晋晨曦的SAT问题解决系统					     ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												     " << "||" << "\n";
	cout << "||";
	SetColor(14); // 设置颜色为黄色
	cout << "1. 解决SAT问题   2. 解决普通数独问题   3. 解决蜂窝数独问题   4. 检查res文件是否正确   0. 退出系统  ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												     " << "||" << "\n";
	cout << "\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~" << "\n";
	SetColor(7);
	cout << "请输入：_\b";
}

void CoutWrods(int choice)
{
	switch (choice)
	{
	case 0:
		system("cls");
		SetColor(4);
		cout << "\n\n---------------------------------------------\n\n" << "	程序结束，现在退出，拜拜~~~" << "\n\n---------------------------------------------\n\n";
		SetColor(7);
		break;
	default:
		system("cls");
		cout << "输入错误，请重新输入!";
	}
}
