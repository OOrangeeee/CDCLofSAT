#include "GlobalOfOldProgram.h"
/// <summary>
/// 读取对应的文件中的值，并返回第一个子句的头节点
/// </summary>
/// <param name="VARNUM"></param>
/// <param name="filename"></param>
/// <returns></returns>
HeadNode* CreateClause(string& fileP, int& VARNUM, howMany*& howManyTimes)
{
	ifstream fis;
	cout << "请输入文件路径（注意：复制文件地址时两边的双引号需要保留）\n____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	string filePath;
	cin >> filePath;
	fileP = filePath.substr(1, filePath.size() - 2);
	fis.open(fileP);
	while (!fis)
	{
		cout << "\n文件打开错误，请输入正确的文件格式，复制文件地址时两边的双引号需要保留\n\n";
		cin >> filePath;
		fileP = filePath.substr(1, filePath.size() - 2);
		fis.open(fileP);
	}
	char ch;
	string buf;
	fis >> ch;
	while (ch != 'p')
	{
		getline(fis, buf);
		fis >> ch;
	}
	string cnf;
	int VarNum, ClauseNum;
	fis >> cnf >> VarNum >> ClauseNum;
	fis.get();

	HeadNode* HEAD = new HeadNode;
	HEAD->up = nullptr;
	HEAD->Num = 0;
	HEAD->right = nullptr;
	HEAD->down = nullptr;
	HeadNode* headPre = HEAD;
	HeadNode* headRear = HEAD;
	HeadNode* END = headRear;
	for (int i = 0; i < ClauseNum; i++)
	{
		int temp;
		fis >> temp;
		if (howManyTimes[abs(temp)].howM == 0)
		{
			if (temp > 0)
			{
				howManyTimes[abs(temp)].ifTure++;
				howManyTimes[abs(temp)].howM++;
			}
			else
			{
				howManyTimes[abs(temp)].ifFalse++;
				howManyTimes[abs(temp)].howM++;
			}
		}
		else if (howManyTimes[abs(temp)].howM == 1)
		{
			if (temp > 0 && howManyTimes[abs(temp)].ifTure == 0)
			{
				howManyTimes[abs(temp)].ifTure++;
				howManyTimes[abs(temp)].howM++;
			}
			else if (temp < 0 && howManyTimes[abs(temp)].ifFalse == 0)
			{
				howManyTimes[abs(temp)].ifFalse++;
				howManyTimes[abs(temp)].howM++;
			}
		}
		DataNode* front = new DataNode;
		front->data = temp;
		front->pre = nullptr;
		front->next = nullptr;
		headRear->right = front;
		headRear->Num++;
		fis >> temp;
		while (temp != 0)
		{
			if (howManyTimes[abs(temp)].howM == 0)
			{
				if (temp > 0)
				{
					howManyTimes[abs(temp)].ifTure++;
					howManyTimes[abs(temp)].howM++;
				}
				else
				{
					howManyTimes[abs(temp)].ifFalse++;
					howManyTimes[abs(temp)].howM++;
				}
			}
			else if (howManyTimes[abs(temp)].howM == 1)
			{
				if (temp > 0 && howManyTimes[abs(temp)].ifTure == 0)
				{
					howManyTimes[abs(temp)].ifTure++;
					howManyTimes[abs(temp)].howM++;
				}
				else if (temp < 0 && howManyTimes[abs(temp)].ifFalse == 0)
				{
					howManyTimes[abs(temp)].ifFalse++;
					howManyTimes[abs(temp)].howM++;
				}
			}
			DataNode* rear = new DataNode;
			front->next = rear;
			rear->pre = front;
			rear->data = temp;
			rear->next = nullptr;
			front = front->next;
			headRear->Num++;
			fis >> temp;
		}
		fis.get();//换行符
		HeadNode* tp = new HeadNode;
		tp->down = nullptr;
		tp->Num = 0;
		tp->right = nullptr;
		tp->up = headRear;
		headRear->down = tp;
		END = headRear;
		headRear = headRear->down;
	}
	END->down = nullptr;
	VARNUM = VarNum;
	return HEAD;
}