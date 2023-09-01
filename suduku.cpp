#include "others.h"
#define CORRECT 0
#define WRONG -1
static int T = 0;

int index(int i, int j)
{
	return 8 * i + j + 1;
}

int Digit(int a[][COL], int i, int j)
{
	if (i < ROW && j < COL)
	{
		int x, y, k;
		int check[COL + 1] = { CORRECT };//�����ų��Ѿ�ʹ�ù��ĵ�����
		for (x = 0; x < i; x++)
			check[a[x][j]] = WRONG;//����ʹ�õ�������ΪWRONG
		for (x = 0; x < j; x++)
			check[a[i][x]] = WRONG;//��ʹ�ù���������ΪWRONG
		for (x = i / 3 * 3; x <= i; x++)
		{
			if (x == i)
				for (y = j / 3 * 3; y < j; y++)
					check[a[x][y]] = WRONG;
			else
				for (y = j / 3 * 3; y < j / 3 * 3 + 3; y++)
					check[a[x][y]] = WRONG;
		}
		int flag = 0;
		for (k = 1; k <= COL && flag == 0; k++)
		{
			if (check[k] == CORRECT)
			{
				flag = 1;
				a[i][j] = k;
				if (j == COL - 1 && i != ROW - 1)
				{
					if (Digit(a, i + 1, 0) == CORRECT) return CORRECT;
					else flag = 0;
				}
				else if (j != COL - 1)
				{
					if (Digit(a, i, j + 1) == CORRECT) return CORRECT;
					else flag = 0;
				}
			}
		}
		if (flag == 0)
		{
			a[i][j] = 0;
			return WRONG;
		}
	}
	return CORRECT;
}
/// <summary>
/// ������������ĵ�һ��
/// </summary>
/// <param name="a0"></param>
/// <param name="n"></param>
void randomFirstRow(int a0[], int n)
{
	int i{ 0 };
	int j{ 0 };
	srand((unsigned)time(nullptr));
	for (i = 0; i < n; i++)
	{
		a0[i] = rand() % 9 + 1;
		j = 0;
		while (j < i)
		{
			if (a0[i] == a0[j])
			{
				a0[i] = rand() % 9 + 1;
				j = 0;
			}
			else j++;
		}
	}
}

void createSudoku(int a[][COL])//��������
{
	randomFirstRow(a[0], COL);//������ɵ�һ��
	Digit(a, 1, 0);//�ݹ����ɺ�i��
}

void createStartinggrid(const int a[][COL], int b[][COL], int numDigits)
{
	int i, j, k;
	srand((unsigned)time(nullptr));
	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			b[i][j] = a[i][j];
	int** c = new int* [numDigits];
	for (int i = 0; i < numDigits; i++)
	{
		c[i] = new int[2];
	}
	int m, flag = 0;
	for (i = 0; i < numDigits; i++)
	{
		j = rand() % 9;
		k = rand() % 9;

		flag = 0;
		for (m = 0; m < i; m++)
			if (j == c[m][0] && k == c[m][1])
				flag = 1;

		if (flag == 0)
		{
			b[j][k] = 0;
			c[i][0] = j;
			c[i][1] = k;
		}
		else
			i--;
	}
	for (int i = 0; i < numDigits; i++)
		delete[] c[i];
	delete[] c;
}

void print(const int a[][COL],string shuduPath)
{
	int i, j;
	shuduPath.erase(shuduPath.size() - 3);
	shuduPath += "txt";
	ofstream fos(shuduPath);
	if (!fos.is_open())
	{
		cout << "�޷����ļ�" << endl;
		exit;
	}
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			cout << a[i][j] << " ";
			fos << a[i][j] << " ";
		}
		cout << endl;
		fos << endl;
	}
}

string ToCnf(int a[][COL], int holes, string shuduPath)
{
	//shuduPath = "E:\\test\\shusuShengCheng\\ShuDu.cnf";
	ofstream in(shuduPath);//���������ļ�
	if (!in.is_open())
	{
		cout << "�޷����ļ�\n";
		exit;
	}
	in << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - holes << endl;
	//���Ӿ�
	for (int x = 0; x < ROW; ++x) {
		for (int y = 0; y < COL; ++y)
			if (a[x][y] != 0)
				in << (x + 1) * 100 + (y + 1) * 10 + a[x][y] << " " << 0 << endl;
	}
	//����
	for (int x = 1; x <= 9; ++x) {
		for (int y = 1; y <= 9; ++y) {
			for (int z = 1; z <= 9; ++z)
				in << x * 100 + y * 10 + z << " ";
			in << 0;
			in << endl;
		}
	}
	//��
	for (int y = 1; y <= 9; ++y) {
		for (int z = 1; z <= 9; ++z)
			for (int x = 1; x <= 8; ++x)
				for (int i = x + 1; i <= 9; ++i)
					in << 0 - (x * 100 + y * 10 + z) << " "
					<< 0 - (i * 100 + y * 10 + z) << " " << 0 << endl;
	}
	//��
	for (int x = 1; x <= 9; ++x) {
		for (int z = 1; z <= 9; ++z)
			for (int y = 1; y <= 8; ++y)
				for (int i = y + 1; i <= 9; ++i)
					in << 0 - (x * 100 + y * 10 + z) << " "
					<< 0 - (x * 100 + i * 10 + z) << " " << 0 << endl;
	}
	//3*3
	for (int z = 1; z <= 9; ++z) {
		for (int i = 0; i <= 2; ++i)
			for (int j = 0; j <= 2; ++j)
				for (int x = 1; x <= 3; ++x)
					for (int y = 1; y <= 3; ++y)
						for (int k = y + 1; k <= 3; ++k)
							in << 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z) << " "
							<< 0 - ((3 * i + x) * 100 + (3 * j + k) * 10 + z) << " " << 0 << endl;
	}
	for (int z = 1; z <= 9; z++) {
		for (int i = 0; i <= 2; i++)
			for (int j = 0; j <= 2; j++)
				for (int x = 1; x <= 3; x++)
					for (int y = 1; y <= 3; y++)
						for (int k = x + 1; k <= 3; k++)
							for (int l = 1; l <= 3; l++)
								in << 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z) << ' '
								<< 0 - ((3 * i + k) * 100 + (3 * j + l) * 10 + z) << ' ' << 0 << endl;
	}
	in.close();
	return shuduPath;
}

string createSudokuToFile()
{
	int sudoku[ROW][COL] = { 0 };
	int starting_grid[ROW][COL] = { 0 };
	int holes{ 0 };//�ڶ�����
	cout << "���������ڼ���������������������������Զ�Խ���޽�Ŀ���Խ�󣬵���SAT�������Ŀ���ҲԽ��\n�������Ҫ����63���������45�����ڣ���__\b\b";
	cin >> holes;
	string shuduPath;
	cout << "�������Ӧ�������ļ���ַ:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	cin >> shuduPath;
	string shuDuName = "ShuDu.cnf";
	shuduPath = shuduPath + "\\" + shuDuName;
	createSudoku(sudoku);//������������
	createStartinggrid(sudoku, starting_grid, holes);//���ɳ���
	print(starting_grid,shuduPath);//�������
	//ת��Ϊcnf�ļ�
	string filename = ToCnf(starting_grid, holes, shuduPath);
	return filename;
}

//void SudokuShow(conse* result, int VARNUM)
//{
//	int res[9][9] = { 0 };
//	string shuduPath = "E:\\test\\shusuShengCheng\\ShuDuSolution.txt";
//	ofstream fos(shuduPath);//���������ļ�
//	if (!fos.is_open())
//	{
//		cout << "�޷����ļ�\n";
//		exit;
//	}
//	for (int i = 0; i < VARNUM; ++i)
//	{
//		if (result[i].value == TRUE)
//		{
//			int x = (int)(abs(result[i].num) / 100) - 1;
//			int y = (int)((abs(result[i].num) - (x + 1) * 100) / 10) - 1;
//			res[x][y] = abs(result[i].num) - (x + 1) * 100 - (y + 1) * 10;
//		}
//	}
//	//���result����
//	for (int i = 0; i < 9; ++i)
//	{
//		for (int j = 0; j < 9; ++j)
//		{
//			cout << res[i][j] << " ";
//			fos << res[i][j] << " ";
//		}
//		cout << endl;
//		fos << endl;
//	}
//}