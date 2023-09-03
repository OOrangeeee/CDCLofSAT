/*���༭��*/
/*������ 2023.9.3 3:46*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include"others.h"
#include <filesystem>
const int CORRECT{ 520 };
const int WRONG{ 521 };

//������ϸ���ͼ���������

string CreatHanidokuToFile(int hanidoku[][COL], string filePath)
{
	//��������
	int starting_grid[ROW][COL] = { 0 };//����
	int holes{ 0 };//�ڶ�����
	cout << "���������ڼ�������������ò�Ҫ����50������__\b\b";
	cin >> holes;

	//�����ļ���
	string filename;//����·��
	filePath.erase(filePath.size() - 3);
	filename = filePath + "cnf";

	MakeHanidokuSure(starting_grid);//��ʼ������ 
	CreateHanidokuQus(hanidoku, starting_grid, holes);//������������
	KeepStartInTxt(starting_grid, filename);//��������ļ�
	filename = HanidokuToCnf(hanidoku, 61 - holes, filename);//���������ļ�

	return filename;
}

void MakeHanidokuSure(int hannidoku[][COL])
{
	for (int i{ 0 }; i < ROW; i++)
	{
		for (int j{ 0 }; j < COL; j++)
		{
			hannidoku[i][j] = -1;
		}
	}
}

string CreateHanidokuForDPLL(int hanidoku[ROW][COL], string shuduPath)
{
	string hanidokuName = "hanidoku.cnf";
	shuduPath = shuduPath + "\\" + hanidokuName;
	MakeHanidokuSure(hanidoku);//��ʼ������   
	//RandomThreeNumsInMidRowOfHanidoku(hanidoku[4]);//��������м������������
	RandomTwoNumsOfHanidoku(hanidoku);//�������3����
	string hanidokuFileName;
	hanidokuFileName = HanidokuToCnf(hanidoku, 61 - 2, shuduPath);//���������ļ�  
	return hanidokuFileName;
}

void RandomThreeNumsInMidRowOfHanidoku(int midRow[])
{
	// ����һ������1-9������
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	//ϴ��
	srand(static_cast<unsigned int>(time(0)));
	for (int i{ 8 }; i > 0; --i)
	{
		int j = rand() % (i + 1);
		swap(numbers[i], numbers[j]);
	}

	//�������̶�λ���������������
	midRow[3] = numbers[3];
	midRow[5] = numbers[6];
	midRow[8] = numbers[1];
}

void RandomTwoNumsOfHanidoku(int hanidoku[][COL])
{
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int ifSuc{ 0 };
	int ifCon{ 0 };
	int nums[6][3] = { 0 };

	for (int m{ 1 }; m <= 2; m++)
	{
		ifSuc = 0;
		while (1)
		{
			ifCon = 0;
			srand(static_cast<unsigned int>(time(0)));
			for (int i{ 8 }; i > 0; --i)
			{
				int j = rand() % (i + 1);
				swap(numbers[i], numbers[j]);
			}
			int x{ 0 };
			int y{ 0 };
			srand(static_cast<unsigned int>(time(0)));
			x = numbers[rand() % 9];
			srand(static_cast<unsigned int>(time(0)));
			y = numbers[rand() % 9 + 2];
			for (int q{ 1 }; q < m; q++)
				if (x == nums[q][1] && y == nums[q][2])
				{
					ifCon = 1;
					break;
				}
			if (ifCon)
				continue;
			if (x >= 0 && x <= 8 && y >= 0 && y <= 8 && y <= 8 - abs(x - 4) && x != 4)
				ifSuc = FindOtherNumsX(hanidoku, x, y);
			if (ifSuc)
			{
				nums[m][1] = x;
				nums[m][2] = y;
				break;
			}
		}
	}
}

int FindOtherNums(int hanidoku[][COL], int i, int j)
{
	int check[COL * ROW] = { 0 };
	int flag{ 0 };
	for (int i{ 0 }; i < COL * ROW; i++)
		check[i] = CORRECT;
	if (i >= 0 && i < ROW && j >= 0 && j < COL)
	{
		if (i < 4)
		{
			GetCheck(hanidoku, check, i, j);
			for (int x{ 1 }; x <= 9; x++)
			{
				if (check[x] == CORRECT)
				{
					flag = 1;
					hanidoku[i][j] = x;
					if (i == 0 && j == 4)
					{
						if (FindOtherNums(hanidoku, 5, 0) == CORRECT)
							return CORRECT;
						else
							flag = 0;
					}
					else if (j + 1 > i + 4)
					{
						if (FindOtherNums(hanidoku, i - 1, 0) == CORRECT)
							return CORRECT;
						else
							flag = 0;
					}
					else
					{
						if (FindOtherNums(hanidoku, i, j + 1) == CORRECT)
							return CORRECT;
						else
							flag = 0;
					}
				}
			}
		}
		else
		{
			GetCheck(hanidoku, check, i, j);
			for (int x{ 1 }; x <= 9; x++)
			{
				if (check[x] == CORRECT)
				{
					hanidoku[i][j] = x;
					if (j + 1 > 12 - i)
					{
						if (FindOtherNums(hanidoku, i + 1, 0) == CORRECT)
							return CORRECT;
						else
							flag = 0;
					}
					else
					{
						if (FindOtherNums(hanidoku, i, j + 1) == CORRECT)
							return CORRECT;
						else
							flag = 0;
					}
				}
			}
		}
	}
	if (flag == 0)
	{
		hanidoku[i][j] = -1;
		return WRONG;
	}
	return CORRECT;
}

int FindOtherNumsX(int hanidoku[][COL], int i, int j)
{
	int check[10] = { 0 };
	int flag{ 0 };
	for (int i{ 0 }; i < 10; i++)
		check[i] = CORRECT;
	if (i >= 0 && i < ROW && j >= 0 && j < COL)
	{
		GetCheck(hanidoku, check, i, j);
		int x{ 0 };
		while (1)
		{
			if (check[x] == CORRECT && x != 1 && x != 0)
			{
				flag = 1;
				hanidoku[i][j] = x;
				break;
			}
			if (x == 1)
			{
				srand(static_cast<unsigned int>(time(0)));
				x = rand() % 10;
				if (x == 0)
					x++;
			}
			x++;
			if (x > 9)
			{
				srand(static_cast<unsigned int>(time(0)));
				x = rand() % 10;
				if (x == 0)
					x++;
			}
		}
	}
	if (flag == 0)
	{
		hanidoku[i][j] = -1;
		return WRONG;
	}
	return CORRECT;
}

void GetCheck(int hanidoku[][COL], int check[], int i, int j)
{
	if (i < 4)
	{
		for (int x{ 0 }; x < 9; x++)//ͬһ�е���������
		{
			if (hanidoku[i][x] != -1)
				check[hanidoku[i][x]] = WRONG;
		}
		int line = i + 1;
		while (line != 5)
		{
			if (hanidoku[line][j] != -1)
				check[hanidoku[line][j]] = WRONG;
			if (hanidoku[line][j + (line - i)] != -1)
				check[hanidoku[line][j + (line - i)]] = WRONG;
			line++;
		}
	}
	else
	{
		for (int x{ 0 }; x < 9; x++)//ͬһ�е��������� 
		{
			if (hanidoku[i][x] != -1)
				check[hanidoku[i][x]] = WRONG;
		}
		int line = i - 1;
		while (line >= 0)
		{
			if (line <= 3)
			{
				if (j + line - 4 >= 0 && hanidoku[line][j - (4 - line)] != -1)
					check[hanidoku[line][j - (4 - line)]] = WRONG;
				if (j + i - 4 <= line + 4 && hanidoku[line][j + i - 4] != -1)
					check[hanidoku[line][j + i - 4]] = WRONG;
				line--;
			}
			else
			{
				if (hanidoku[line][j] != -1)
					check[hanidoku[line][j]] = WRONG;
				if (hanidoku[line][j + (i - line)] != -1)
					check[hanidoku[line][j + (i - line)]] = WRONG;
				line--;
			}
		}
	}
}

//void FindMaxAndMinInLine(int hanidoku[][COL], int i, int j, int& max, int& min)
//{
//	int iX{ i };
//	int jX{ j };
//	max = hanidoku[i][j];
//	min = hanidoku[i][j];
//	for (int a{ 0 }; a <= j; a++)
//	{
//		min = hanidoku[i][a] > min ? min : hanidoku[i][a];
//		max = hanidoku[i][a] > min ? hanidoku[i][a] : max;
//	}
//}
//
//void FindMaxAndMinInLeftUp(int hanidoku[][COL], int i, int j, int& max, int& min)
//{
//	int iX{ i };
//	int jX{ j };
//	max = hanidoku[i][j];
//	min = hanidoku[i][j];
//	if (i <= 4)
//	{
//
//	}
//	else
//	{
//
//	}
//}

string HanidokuToCnf(int a[][COL], int holes, string shuduPath)
{
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };
	int m{ 0 };
	ofstream in(shuduPath);//���������ļ�
	if (!in.is_open())
	{
		cout << "�޷����ļ�\n";
		exit(WRONG);
	}

	in << "p" << " " << "cnf" << " " << 999 << " " << 61 + 61 + 2196 + 6 + 60 + 540 + 18 + 36 + 810 + 30 + 18 + 1134 + 42 + 6 + 1512 + 27 + 972 - holes << endl;//61+61+2196+6+60+540+18+36+810+30+18+1134+42+6+1512+27+972=7535

	//���Ӿ�(61-holes)

	for (x = 0; x <= 8; x++)
		for (y = 0; y <= 8 - abs(x - 4); y++)
			if (a[x][y] != -1)
				in << (x + 1) * 100 + (y + 1) * 10 + a[x][y] << " " << 0 << endl;

	//��Լ��(61)

	for (x = 0; x <= 8; x++)
		for (y = 0; y <= 8 - abs(x - 4); y++)
		{
			for (z = 1; z <= 9; z++)
				in << (x + 1) * 100 + (y + 1) * 10 + z << " ";
			in << "0" << endl;
		}

	//��Լ��(2196)

	for (x = 0; x <= 8; x++)
		for (y = 0; y <= 8 - abs(x - 4); y++)
			for (z = 1; z <= 8; z++)
				for (m = z + 1; m <= 9; m++)
					in << 0 - ((x + 1) * 100 + (y + 1) * 10 + z) << " " << 0 - ((x + 1) * 100 + (y + 1) * 10 + m) << " " << 0 << endl;

	//������ĶԽ���Լ������

	//ע��������˵�����β����Ǵ�ͳ�����Σ���������Ϊ��������˵�������Σ��������Ŀ�����仰�Ļ����м�Ҫ����������ε������������ӳ��������ܸ��Ƕ���������ӣ�����������ȷ��

	//������ÿ���߶�Ҫ����5(6)

	for (y = 0; y <= 4; y++)
		in << 1 * 100 + (y + 1) * 10 + 5 << " ";
	in << "0\n";
	for (y = 0; y <= 4; y++)
		in << 9 * 100 + (y + 1) * 10 + 5 << " ";
	in << "0\n";
	for (x = 0; x <= 4; x++)
		in << (x + 1) * 100 + 1 * 10 + 5 << " ";
	in << "0\n";
	for (x = 4; x <= 8; x++)
		in << (x + 1) * 100 + (12 - x + 1) * 10 + 5 << " ";
	in << "0\n";
	for (x = 4; x <= 8; x++)
		in << (x + 1) * 100 + 1 * 10 + 5 << " ";
	in << "0\n";
	for (x = 0; x <= 4; x++)
		in << (x + 1) * 100 + (x + 4 + 1) * 10 + 5 << " ";
	in << "0\n";

	//�����ε������ߣ�ÿһ����ѡ�����(60)
	//�ر���лWolframAlpha��վ��Ҫ��Ȼ�Ҳ�֪�������ȡ��ʽ��Ϊ��ȡ��ʽ��Ҫ����ʲôʱ��

	int ch1[][2] = { {1,6},{2,6},{2,7},{3,6},{3,7},{3,8},{4,6},{4,7},{4,8},{4,9} };//10+2
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 4; y++)
				in << 1 * 100 + (y + 1) * 10 + ch1[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 4; y++)
				in << 9 * 100 + (y + 1) * 10 + ch1[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (x = 0; x <= 4; x++)
				in << (x + 1) * 100 + 1 * 10 + ch1[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (x = 4; x <= 8; x++)
				in << (x + 1) * 100 + (12 - x + 1) * 10 + ch1[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (x = 4; x <= 8; x++)
				in << (x + 1) * 100 + 1 * 10 + ch1[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			for (x = 0; x <= 4; x++)
				in << (x + 1) * 100 + (x + 4 + 1) * 10 + ch1[i][j] << " ";
		in << "0\n";
	}

	//�����ߵ�����������(540)

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 4; y++)
			for (z = y + 1; z <= 4; z++)
				in << 0 - (1 * 100 + (y + 1) * 10 + i) << " " << 0 - (1 * 100 + (z + 1) * 10 + i) << " 0\n";

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 4; y++)
			for (z = y + 1; z <= 4; z++)
				in << 0 - (9 * 100 + (y + 1) * 10 + i) << " " << 0 - (9 * 100 + (z + 1) * 10 + i) << " 0\n";

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 4; y++)
			for (z = y + 1; z <= 4; z++)
				in << 0 - ((y + 1) * 100 + 1 * 10 + i) << " " << 0 - ((z + 1) * 100 + 1 * 10 + i) << " 0\n";

	for (int i = 1; i <= 9; i++)
		for (y = 4; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (12 - y + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (12 - z + 1) * 10 + i) << " 0\n";

	for (int i = 1; i <= 9; i++)
		for (y = 4; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + 1 * 10 + i) << " " << 0 - ((z + 1) * 100 + 1 * 10 + i) << " 0\n";

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 4; y++)
			for (z = y + 1; z <= 4; z++)
				in << 0 - ((y + 1) * 100 + (y + 4 + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (z + 4 + 1) * 10 + i) << " 0\n";

	//�м���һȦ�������α���4 5 6(18)

	for (int i = 4; i <= 6; i++)
	{
		for (y = 0; y <= 5; y++)
			in << 2 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 4; i <= 6; i++)
	{
		for (y = 0; y <= 5; y++)
			in << 8 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 4; i <= 6; i++)
	{
		for (y = 0; y <= 5; y++)
			in << (y + 1) * 100 + (min(1, 5 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 4; i <= 6; i++)
	{
		for (y = 3; y <= 8; y++)
			in << (y + 1) * 100 + (min(7, 11 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 4; i <= 6; i++)
	{
		for (y = 3; y <= 8; y++)
			in << (y + 1) * 100 + (min(1, y - 3) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 4; i <= 6; i++)
	{
		for (y = 0; y <= 5; y++)
			in << (y + 1) * 100 + (min(7, y + 3) + 1) * 10 + i << " ";
		in << "0\n";
	}

	//С������ÿ���ߵ�ѡ��(78)

	int ch2[][2] = { {1,7},{2,7},{2,8},{3,7},{3,8},{3,9} };
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 5; y++)
				in << 2 * 100 + (y + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 5; y++)
				in << 8 * 100 + (y + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 5; y++)
				in << (y + 1) * 100 + (min(1, 5 - y) + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 3; y <= 8; y++)
				in << (y + 1) * 100 + (min(7, 11 - y) + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 3; y <= 8; y++)
				in << (y + 1) * 100 + (min(1, y - 3) + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
			for (y = 0; y <= 5; y++)
				in << (y + 1) * 100 + (min(7, y + 3) + 1) * 10 + ch2[i][j] << " ";
		in << "0\n";
	}

	//С�����ε���������(810)

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 5; y++)
			for (z = y + 1; z <= 5; z++)
				in << 0 - (2 * 100 + (y + 1) * 10 + i) << " " << 0 - (2 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 5; y++)
			for (z = y + 1; z <= 5; z++)
				in << 0 - (8 * 100 + (y + 1) * 10 + i) << " " << 0 - (8 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 5; y++)
			for (z = y + 1; z <= 5; z++)
				in << 0 - ((y + 1) * 100 + (min(1, 5 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(1, 5 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 3; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(7, 11 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(7, 11 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 3; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(1, y - 3) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(1, z - 3) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 5; y++)
			for (z = y + 1; z <= 5; z++)
				in << 0 - ((y + 1) * 100 + (min(7, y + 3) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(7, z + 3) + 1) * 10 + i) << " 0\n";

	//������СС�����Σ���α������3 4 5 6 7(30)

	for (int i = 3; i <= 7; i++)
	{
		for (int y = 0; y <= 6; y++)
			in << 3 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 3; i <= 7; i++)
	{
		for (int y = 0; y <= 6; y++)
			in << 7 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 3; i <= 7; i++)
	{
		for (int y = 0; y <= 6; y++)
			in << (y + 1) * 100 + (min(2, 6 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 3; i <= 7; i++)
	{
		for (int y = 2; y <= 8; y++)
			in << (y + 1) * 100 + (min(6, 10 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 3; i <= 7; i++)
	{
		for (int y = 2; y <= 8; y++)
			in << (y + 1) * 100 + (min(2, y - 2) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 3; i <= 7; i++)
	{
		for (int y = 0; y <= 6; y++)
			in << (y + 1) * 100 + (min(6, y + 2) + 1) * 10 + i << " ";
		in << "0\n";
	}

	//СС�����ε�ѡ��(18)

	int ch3[][2] = { {1,8},{2,8},{2,9} };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (y = 0; y <= 6; y++)
				in << 3 * 100 + (y + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int y = 0; y <= 6; y++)
				in << 7 * 100 + (y + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int y = 0; y <= 6; y++)
				in << (y + 1) * 100 + (min(2, 6 - y) + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int y = 2; y <= 8; y++)
				in << (y + 1) * 100 + (min(6, 10 - y) + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int y = 2; y <= 8; y++)
				in << (y + 1) * 100 + (min(2, y - 2) + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int y = 0; y <= 6; y++)
				in << (y + 1) * 100 + (min(6, y + 2) + 1) * 10 + ch3[i][j] << " ";
		}
		in << "0\n";
	}

	//СС�����ε���������(1134)

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 6; y++)
			for (z = y + 1; z <= 6; z++)
				in << 0 - (3 * 100 + (y + 1) * 10 + i) << " " << 0 - (3 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 6; y++)
			for (z = y + 1; z <= 6; z++)
				in << 0 - (7 * 100 + (y + 1) * 10 + i) << " " << 0 - (7 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 6; y++)
			for (z = y + 1; z <= 6; z++)
				in << 0 - ((y + 1) * 100 + (min(2, 6 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(2, 6 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 2; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(6, 10 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(6, 10 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 2; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(2, y - 2) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(2, z - 2) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 6; y++)
			for (z = y + 1; z <= 6; z++)
				in << 0 - ((y + 1) * 100 + (min(6, y + 2) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(6, z + 2) + 1) * 10 + i) << " 0\n";

	//ССС�����εı��2 3 4 5 6 7 8(42)

	for (int i = 2; i <= 8; i++)
	{
		for (int y = 0; y <= 7; y++)
			in << 4 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 2; i <= 8; i++)
	{
		for (int y = 0; y <= 7; y++)
			in << 6 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 2; i <= 8; i++)
	{
		for (int y = 0; y <= 7; y++)
			in << (y + 1) * 100 + (min(3, 7 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 2; i <= 8; i++)
	{
		for (int y = 1; y <= 8; y++)
			in << (y + 1) * 100 + (min(5, 9 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 2; i <= 8; i++)
	{
		for (int y = 1; y <= 8; y++)
			in << (y + 1) * 100 + (min(3, y - 1) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 2; i <= 8; i++)
	{
		for (int y = 0; y <= 7; y++)
			in << (y + 1) * 100 + (min(5, y + 1) + 1) * 10 + i << " ";
		in << "0\n";
	}

	//ССС�����ε�ѡ��(6)

	int ch4[] = { 1,9 };
	for (int i = 0; i < 2; i++)
	{
		for (y = 0; y <= 7; y++)
			in << 4 * 100 + (y + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";
	for (int i = 0; i < 2; i++)
	{
		for (y = 0; y <= 7; y++)
			in << 6 * 100 + (y + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";
	for (int i = 0; i < 2; i++)
	{
		for (y = 0; y <= 7; y++)
			in << (y + 1) * 100 + (min(3, 7 - y) + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";
	for (int i = 0; i < 2; i++)
	{
		for (y = 1; y <= 8; y++)
			in << (y + 1) * 100 + (min(5, 9 - y) + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";
	for (int i = 0; i < 2; i++)
	{
		for (y = 1; y <= 8; y++)
			in << (y + 1) * 100 + (min(3, y - 1) + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";
	for (int i = 0; i < 2; i++)
	{
		for (y = 0; y <= 7; y++)
			in << (y + 1) * 100 + (min(5, y + 1) + 1) * 10 + ch4[i] << " ";
	}
	in << "0\n";

	//ССС�����ε���������(1512)

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 7; y++)
			for (z = y + 1; z <= 7; z++)
				in << 0 - (4 * 100 + (y + 1) * 10 + i) << " " << 0 - (4 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 7; y++)
			for (z = y + 1; z <= 7; z++)
				in << 0 - (6 * 100 + (y + 1) * 10 + i) << " " << 0 - (6 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 7; y++)
			for (z = y + 1; z <= 7; z++)
				in << 0 - ((y + 1) * 100 + (min(3, 7 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(3, 7 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 1; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(5, 9 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(5, 9 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 1; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(3, y - 1) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(3, z - 1) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 7; y++)
			for (z = y + 1; z <= 7; z++)
				in << 0 - ((y + 1) * 100 + (min(5, y + 1) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(5, z + 1) + 1) * 10 + i) << " 0\n";

	//СССС�����εı���(27)

	for (int i = 1; i <= 9; i++)
	{
		for (int y = 0; y <= 8; y++)
			in << 5 * 100 + (y + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 1; i <= 9; i++)
	{
		for (int y = 0; y <= 8; y++)
			in << (y + 1) * 100 + (min(4, 8 - y) + 1) * 10 + i << " ";
		in << "0\n";
	}
	for (int i = 1; i <= 9; i++)
	{
		for (int y = 0; y <= 8; y++)
			in << (y + 1) * 100 + (min(4, y) + 1) * 10 + i << " ";
		in << "0\n";
	}

	//СССС������û��ѡ��(0)

	// СССС�����εĻ���(972)

	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - (5 * 100 + (y + 1) * 10 + i) << " " << 0 - (5 * 100 + (z + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(4, 8 - y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(4, 8 - z) + 1) * 10 + i) << " 0\n";
	for (int i = 1; i <= 9; i++)
		for (y = 0; y <= 8; y++)
			for (z = y + 1; z <= 8; z++)
				in << 0 - ((y + 1) * 100 + (min(4, y) + 1) * 10 + i) << " " << 0 - ((z + 1) * 100 + (min(4, z) + 1) * 10 + i) << " 0\n";

	in.close();
	return shuduPath;
}

void CreateHanidokuAns(char fileName[], int hanidoku[][COL])
{
	string resultName = fileName;
	int temp[1000];
	ifstream out(resultName);
	if (!out.is_open())
	{
		cout << "�ļ�������\n";
		return;
	}
	string x;
	getline(out, x);
	out.get();
	out.get();//������һ���ո�
	if (x[2] == '1')
	{
		int num{ 1 };
		int k{ 0 };
		bool ifFalse{ false };
		while (1)
		{
			char c = out.get();
			if (c == ' ')
			{
				if (ifFalse)
				{
					temp[num++] = 0 - k;
				}
				else
				{
					temp[num++] = k;
				}
				if (k == 999)
				{
					break;
				}
				k = 0;
				ifFalse = false;
			}
			else if (c >= '0' && c <= '9')
			{
				k = (c - '0') + k * 10;
			}
			else if (c == '-')
			{
				ifFalse = true;
			}
		}
		for (int x{ 0 }; x <= 8; x++)
		{
			for (int y{ 0 }; y <= 8 - abs(x - 4); y++)
			{
				for (int i{ 111 }; i <= 999; i++)
				{
					if (temp[i] > 0)
					{
						hanidoku[x][y] = temp[i] % 10;
						temp[i] = 0 - temp[i];
						break;
					}
				}
			}
		}
	}
	else
	{
		cout << "�����������ɴ���" << endl;
	}
}

void DeleteFiles(char FileNames[])
{
	string filename = FileNames;
	filesystem::path filePath(filename);
	filesystem::remove(filePath);
	filename.erase(filename.size() - 3);
	filename += "cnf";
	filesystem::path filePathX(filename);
	filesystem::remove(filePathX);
}

void CreateHanidokuQus(int hanidoku[][COL], int starting_grid[][COL], int holes)
{
	int i{ 0 };
	int j{ 0 };
	int k{ 0 };
	int** tips = new int* [holes];
	for (i = 0; i < holes; i++)
		tips[i] = new int[2];
	srand((unsigned)time(NULL));
	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			starting_grid[i][j] = hanidoku[i][j];
	int m, flag = 0;
	for (i = 0; i < holes; i++)
	{
		j = rand() % 9;
		k = rand() % 9;
		while (k<max(0, j - 4) || k>min(i + 4, 8))
			k = rand() % 9;
		flag = 0;
		for (m = 0; m < i; m++)
			if (j == tips[m][0] && k == tips[m][1])
				flag = 1;
		if (flag == 0)
		{
			starting_grid[j][k] = 0;
			tips[i][0] = j;
			tips[i][1] = k;
		}
		else
			i--;
	}
}

void KeepStartInTxt(int starting_grid[][COL], string fileName)
{
	fileName.erase(fileName.size() - 3);
	fileName = fileName + "txt";
	ofstream in(fileName);
	for (int i{ 0 }; i <= 8; i++)
	{
		for (int z{ 0 }; z < abs(i - 4); z++)
			in << " ";
		for (int j{ 0 }; j <= 8 - abs(i - 4); j++)
			in << starting_grid[i][j] << " ";
		in << endl;
	}
}

void KeepAnsInTxt(char fileName[])
{
	string fileNameStr = fileName;//res
	string ansFileNameStr = fileName;//txt
	ansFileNameStr.erase(ansFileNameStr.size() - 4);
	ansFileNameStr += "Solution.txt";
	ifstream out(fileName);
	ofstream in(ansFileNameStr);
	string firstLine;
	getline(out, firstLine);
	out.get();
	out.get();//������һ���ո�
	int temp[1000];
	if (firstLine[2] == '1')
	{
		int num{ 1 };
		int k{ 0 };
		bool ifFalse{ false };
		while (1)
		{
			char c = out.get();
			if (c == ' ')
			{
				if (ifFalse)
				{
					temp[num++] = 0 - k;
				}
				else
				{
					temp[num++] = k;
				}
				if (k == 999)
				{
					break;
				}
				k = 0;
				ifFalse = false;
			}
			else if (c >= '0' && c <= '9')
			{
				k = (c - '0') + k * 10;
			}
			else if (c == '-')
			{
				ifFalse = true;
			}
		}
		int m = 111;
		for (int i{ 0 }; i <= 8; i++)
		{
			for (int z{ 0 }; z < abs(i - 4); z++)
				in << " ";
			for (int j{ 0 }; j <= 8 - abs(i - 4); j++)
			{
				while (1)
				{
					if (temp[m] > 0)
					{
						in << temp[m] % 10 << " ";
						m++;
						break;
					}
					m++;
				}
			}
			in << endl;
		}
	}
	else
	{
		cout << "\n�����޽⣡(���ң���Ҳ��֪��ʲô���������㿴���ҵĴ��룬���Ӧ��֪�������ɵ��������һ�����н�ģ�����޽⣬�����Լ���ԭ��)\n";
	}
}