/*最后编辑：*/
/*晋晨曦 2023.9.4 01:15*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#include "others.h"
const int CORRECT{ 520 };
const int WRONG{ 521 };
static int T = 0;

int index(int i, int j)
{
	return 8 * i + j + 1;
}

int Digit(int a[][COL], int i, int j)//1 0
{
	if (i < ROW && j < COL)
	{
		int x, y, k;
		int check[COL + 1];
		for (int a{ 0 }; a < COL + 1; a++)
			check[a] = CORRECT;
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
		for (k = 1; k <= 9 && flag == 0; k++)
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
		c[i] = new int[2];
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

void print(const int a[][COL], string shuduPath)
{
	int i, j;
	shuduPath.erase(shuduPath.size() - 3);
	shuduPath += "txt";
	ofstream fos(shuduPath);
	if (!fos.is_open())
	{
		SetColor(11);
		cout << "无法打开文件" << endl;
		SetColor(7);
		exit(WRONG);
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
		SetColor(11);
		cout << "无法打开文件\n";
		SetColor(7);
		exit(WRONG);
	}
	in << "p" << " " << "cnf" << " " << 999 << " " << 8829 + 81 - holes << endl;
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
	SetColor(11);
	cout << "输入你想挖几个洞（建议最大不要大于63个，最好在45个以内）：__\b\b";
	SetColor(7);
	cin >> holes;
	string shuduPath;
	SetColor(11);
	cout << "请输入对应的数独文件地址:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	SetColor(7);
	cin >> shuduPath;
	string shuDuName = "ShuDu.cnf";
	shuduPath = shuduPath + "\\" + shuDuName;
	createSudoku(sudoku);//生成数独终盘
	createStartinggrid(sudoku, starting_grid, holes);//生成初盘
	print(starting_grid, shuduPath);//输出初盘
	//转化为cnf文件
	string filename = ToCnf(starting_grid, holes, shuduPath);
	return filename;
}

void showShuDu(char fileName[])
{
	int ifShow{ 0 };
	SetColor(11);
	cout << "你是否要查看数独答案？\n1. YES   2. NO\n请输入:_\b";
	SetColor(7);
	cin >> ifShow;
	string resultName = fileName;
	int temp[1000];
	ifstream out(resultName);
	if (!out.is_open())
	{
		SetColor(11);
		cout << "文件名错误\n";
		SetColor(7);
		return;
	}
	string x;
	getline(out, x);
	out.get();
	out.get();//消除第一个空格
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
		string solutionName = fileName;
		solutionName.erase(solutionName.size() - 4);
		solutionName += "SoluTion.txt";
		ofstream in(solutionName);
		if (!in.is_open())
		{
			SetColor(11);
			cout << "文件写入失败\n";
			SetColor(7);
			return;
		}
		int count{ 0 };//计数器
		for (int i{ 111 }; i <= 999; i++)
		{
			if (temp[i] > 0)
			{
				if (ifShow == 1)
					cout << temp[i] % 10 << " ";
				in << temp[i] % 10 << " ";
				count++;
			}
			if (count == 9)
			{
				if (ifShow == 1)
					cout << endl;
				in << endl;
				count = 0;
			}
		}
	}
	else
	{
		SetColor(11);
		cout << "该数独无解！（可能是我的生成数独部分有误T_T）" << endl;
		SetColor(7);
	}
}
