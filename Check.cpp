/*最后编辑：*/
/*晋晨曦 2023.9.3 23:57*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#include"GlobalOfOldProgram.h"
#include"others.h"

int CheckCNFandRES(string CNFpath, string RESpath)
{
	ifstream CNFout(CNFpath);
	ifstream RESout(RESpath);
	if (!CNFout || !RESout)
	{
		SetColor(11);
		cout << "\n\n文件打开失败\n";
		SetColor(7);
		return ERROR;
	}
	string ifHaveAns;
	getline(RESout, ifHaveAns);
	if (ifHaveAns[2] == '0')
	{
		SetColor(11);
		cout << "\n\n文件无解\n";
		SetColor(7);
		return ERROR;
	}
	RESout.get();
	RESout.get();
	int VARNUM{ 0 };
	int clauseNum{ 0 };
	HeadNode* list = CreateClause(CNFpath, VARNUM, clauseNum);
	int* RES = new int[VARNUM + 1];
	int num{ 1 };
	int k{ 0 };
	bool ifFalse{ false };
	while (1)
	{
		char c = RESout.get();
		if (c == ' ')
		{
			if (ifFalse)
				RES[num++] = 0 - k;
			else
				RES[num++] = k;
			if (k == VARNUM)
				break;
			k = 0;
			ifFalse = false;
		}
		else if (c >= '0' && c <= '9')
			k = (c - '0') + k * 10;
		else if (c == '-')
			ifFalse = true;
	}
	HeadNode* lP = list;
	int isRight{ 0 };
	for (int i{ 1 }; i <= clauseNum && lP; i++, lP = lP->down)
	{
		isRight = 0;
		DataNode* dP = lP->right;
		while (dP)
		{
			if (dP->data > 0)
			{
				if (RES[dP->data] > 0)
				{
					isRight = 1;
					break;
				}
			}
			else
			{
				if (RES[0 - dP->data] < 0)
				{
					isRight = 1;
					break;
				}
			}
			dP = dP->next;
		}
		if (!isRight)
			return FALSE;
	}
	return TRUE;
}

string UserAnsSudo(int ans[][COL], string filename)
{
	filename.erase(filename.size() - 9);
	filename += "User.res";
	ofstream in(filename);
	int temp[999] = { 0 };
	for (int i{ 0 }; i < ROW; i++)
		for (int j{ 0 }; j < COL; j++)
			temp[ans[i][j] + (i + 1) * 100 + (j + 1) * 10] = 1;
	in << "s 1" << endl;
	in << "v ";
	for (int i{ 1 }; i <= 999; i++)
	{
		if (temp[i])
			in << i << " ";
		else
			in << 0 - i << " ";
	}
	return filename;
}

string UserAnsHanidoku(int ans[][COL], string filename)
{
	filename.erase(filename.size() - 12);
	filename += "HanidokuUser.res";
	ofstream in(filename);
	int temp[999] = { 0 };
	for (int i{ 0 }; i < ROW; i++)
		for (int j{ 0 }; j <= 8 - abs(i - 4); j++)
			temp[ans[i][j] + (i + 1) * 100 + (j + 1) * 10] = 1;
	in << "s 1" << endl;
	in << "v ";
	for (int i{ 1 }; i <= 999; i++)
	{
		if (temp[i])
			in << i << " ";
		else
			in << 0 - i << " ";
	}
	return filename;
}
