/*���༭��*/
/*������ 2023.9.7 23:39*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#define WIN32_LEAN_AND_MEAN
#include"others.h"
#include <windows.h>

//��ϸ�÷��ͺ���������е�ע��

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
	cout << "\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~" << "\n";
	cout << "||";
	SetColor(11);
	cout << "					�����ص�SAT������ϵͳ					      ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												      " << "||" << "\n";
	cout << "|| ";
	SetColor(11);
	cout << "10101010        10       101010101010    10101010    10101010  1010101010   10101010  10        10";
	SetColor(3);
	cout << " ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "10             1010      101010101010  10        10  10        1010101010  10           10    10";
	SetColor(3);
	cout << "   ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "10            10  10         1010      10        10  10           1010     10             1010";
	SetColor(3);
	cout << "     ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "  1010       10101010        1010      10        10  10101010     1010     10              10";
	SetColor(3);
	cout << "      ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "      10    10      10       1010      10        10  10           1010     10             1010";
	SetColor(3);
	cout << "     ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "      10   10        10      1010      10        10  10      1010 1010     10           10    10";
	SetColor(3);
	cout << "   ||" << endl;
	cout << "|| ";
	SetColor(11);
	cout << "10101010  10          10     1010        10101010    10       101010        10101010  10        10";
	SetColor(3);
	cout << " ||" << endl;
	cout << "||" << "												      " << "||" << "\n";
	cout << "||" << "												      " << "||" << "\n";
	cout << "||";
	SetColor(14); // ������ɫΪ��ɫ
	cout << "1. ���SAT����   2. �����ͨ��������   3. ���������������   4. ���res�ļ��Ƿ���ȷ   0. �˳�ϵͳ   ";
	SetColor(3);
	cout << "||" << endl;
	cout << "||" << "												      " << "||" << "\n";
	cout << "\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~\~" << "\n";
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
