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
		int check[COL + 1] = { CORRECT };//用于排除已经使用过的的数字
		for (x = 0; x < i; x++)
			check[a[x][j]] = WRONG;//列已使用的数字置为WRONG
		for (x = 0; x < j; x++)
			check[a[i][x]] = WRONG;//行使用过的数字置为WRONG
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
/// 随机生成数独的第一行
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

void createSudoku(int a[][COL])//生成数独
{
	randomFirstRow(a[0], COL);//随机生成第一行
	Digit(a, 1, 0);//递归生成后i行
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
		cout << "无法打开文件" << endl;
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
	ofstream in(shuduPath);//定义输入文件
	if (!in.is_open())
	{
		cout << "无法打开文件\n";
		exit;
	}
	in << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - holes << endl;
	//单子句
	for (int x = 0; x < ROW; ++x) {
		for (int y = 0; y < COL; ++y)
			if (a[x][y] != 0)
				in << (x + 1) * 100 + (y + 1) * 10 + a[x][y] << " " << 0 << endl;
	}
	//进入
	for (int x = 1; x <= 9; ++x) {
		for (int y = 1; y <= 9; ++y) {
			for (int z = 1; z <= 9; ++z)
				in << x * 100 + y * 10 + z << " ";
			in << 0;
			in << endl;
		}
	}
	//排
	for (int y = 1; y <= 9; ++y) {
		for (int z = 1; z <= 9; ++z)
			for (int x = 1; x <= 8; ++x)
				for (int i = x + 1; i <= 9; ++i)
					in << 0 - (x * 100 + y * 10 + z) << " "
					<< 0 - (i * 100 + y * 10 + z) << " " << 0 << endl;
	}
	//列
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
	int holes{ 0 };//挖洞个数
	cout << "输入你想挖几个洞（由于是随机生成数独所以洞越多无解的可能越大，导致SAT问题出错的可能也越大\n建议最大不要大于63个，最好在45个以内）：__\b\b";
	cin >> holes;
	string shuduPath;
	cout << "请输入对应的数独文件地址:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	cin >> shuduPath;
	string shuDuName = "ShuDu.cnf";
	shuduPath = shuduPath + "\\" + shuDuName;
	createSudoku(sudoku);//生成数独终盘
	createStartinggrid(sudoku, starting_grid, holes);//生成初盘
	print(starting_grid,shuduPath);//输出初盘
	//转化为cnf文件
	string filename = ToCnf(starting_grid, holes, shuduPath);
	return filename;
}

//void SudokuShow(conse* result, int VARNUM)
//{
//	int res[9][9] = { 0 };
//	string shuduPath = "E:\\test\\shusuShengCheng\\ShuDuSolution.txt";
//	ofstream fos(shuduPath);//定义输入文件
//	if (!fos.is_open())
//	{
//		cout << "无法打开文件\n";
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
//	//输出result数组
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