/*���༭��*/
/*������ 2023.9.4 00:01*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



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
	SetColor(11); // ������ɫΪ����ɫ
	cout << "					�����ص�SAT������ϵͳ					     ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												     " << "||" << "\n";
	cout << "||";
	SetColor(14); // ������ɫΪ��ɫ
	cout << "1. ���SAT����   2. �����ͨ��������   3. ���������������   4. ���res�ļ��Ƿ���ȷ   0. �˳�ϵͳ  ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												     " << "||" << "\n";
	cout << "\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~" << "\n";
	SetColor(7);
	cout << "�����룺_\b";
}

void CoutWrods(int choice)
{
	switch (choice)
	{
	case 0:
		system("cls");
		SetColor(4);
		cout << "\n\n---------------------------------------------\n\n" << "	��������������˳����ݰ�~~~" << "\n\n---------------------------------------------\n\n";
		SetColor(7);
		break;
	default:
		system("cls");
		cout << "�����������������!";
	}
}
