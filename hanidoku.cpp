/*���༭��*/
/*������ 2023.9.3 3:46*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#define _CRT_SECURE_NO_WARNINGS
#include"others.h"
const int CORRECT{ 520 };
const int WRONG{ 521 };

string CreatHanidokuToFile()
{
	//��������
	int hanidoku[ROW][COL] = { 0 };//����
	int starting_grid[ROW][COL] = { 0 };//����
	int holes{ 0 };//�ڶ�����
	string shuduPath;//����·��
	string shuDuName = "Hanidoku.cnf";//��������
	string filename;//����·��
	int ifSuc{ 0 };

	MakeHanidokuSure(hanidoku);//��ʼ������ 
	MakeHanidokuSure(starting_grid);//��ʼ������ 
	CreateHanidoku(hanidoku);//������������ 
	//cout << "���������ڼ���������������������������Զ�Խ���޽�Ŀ���Խ�󣬵���SAT�������Ŀ���ҲԽ��\n�������Ҫ����63���������45�����ڣ���__\b\b";
	//cin >> holes;
	//cout << "�������Ӧ�������ļ���ַ:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	//cin >> shuduPath;
	//shuduPath = shuduPath + "\\" + shuDuName;
	//createStartinggrid(sudoku, starting_grid, holes);//���ɳ���
	//print(starting_grid, shuduPath);//�������
	////ת��Ϊcnf�ļ�
	//filename = ToCnf(starting_grid, holes, shuduPath);
	/*for (int i{ 0 }; i < ROW; i++)
	{
		for (int j{ 0 }; j < COL; j++)
			cout << hanidoku[i][j] << " ";
		cout << endl;
	}
	getchar();
	getchar();*/
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

string CreateHanidoku(int hanidoku[ROW][COL])
{
	MakeHanidokuSure(hanidoku);//��ʼ������   
	RandomMidRowOfHanidoku(hanidoku[4]);//��������м���
	RandomFiveNumsOfHanidoku(hanidoku);//������������
	string hanidokuFileName;
	string shuduPath = "E:\\test\\shusuShengCheng\\fengwo\\hanidoku.cnf";//�����ļ���ʱ����  
	hanidokuFileName = HanidokuToCnf(hanidoku, 61 - 6, shuduPath);//���������ļ�  
	return hanidokuFileName;
}

void RandomMidRowOfHanidoku(int midRow[])
{
	// ����һ������1-9������
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	srand(static_cast<unsigned int>(time(0)));
	for (int i{ 8 }; i > 0; --i)
	{
		int j = rand() % (i + 1);
		swap(numbers[i], numbers[j]);
	}
	/*for (int i{ 0 }; i < 9; i++)
		midRow[i] = numbers[i];*/
	midRow[3] = numbers[3];
	midRow[5] = numbers[6];
	midRow[8] = numbers[1];
}

void RandomFiveNumsOfHanidoku(int hanidoku[][COL])
{
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int ifSuc{ 0 };
	int ifCon{ 0 };
	int nums[6][3] = { 0 };

	for (int m{ 1 }; m <= 3; m++)
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
