#include"others.h"
void ShowTheRes(char filePath[])
{
	int x{ 0 };
	cout << "\n你是否想查看结果内容？\n1. YES   2. NO\n请输入:_\b";
	cin >> x;
	if (x)
	{
		system("cls");
		cout << "--------------------------------------\n";
		string fileP = filePath;
		ifstream fis;
		fis.open(fileP);
		while (!fis)
		{
			cout << "\n读取备份文件打开错误，请输入正确的文件格式，复制地址的双引号需要保留\n\n";
			cin >> fileP;
			fis.open(fileP);
		}
		string Line;
		while (getline(fis, Line))
			cout << Line << endl;
		cout << "--------------------------------------\n";
		cout << "文件输出完成\n";
	}
	else
	{
		cout << endl;
	}
}
string createBackup(char originalFile[])
{
	int x{ 0 };
	cout << "\n你是否需要备份文件?\n1. YES   2.NO\n请输入:_\b";
	cin >> x;
	string backupFile = originalFile;
	if (x == 1)
	{
		backupFile.erase(backupFile.size() - 4);
		string backUp = "BackUp.res";
		backupFile += backUp;
		ifstream in;
		in.open(originalFile);
		while (!in)
		{
			cout << "\n在备份文件时，目标文件打开错误，请输入正确的文件格式，复制文件地址时两边的双引号需要保留\n\n";
			string originalFileX;
			cin >> originalFileX;
			originalFileX = originalFileX.substr(1, originalFileX.size() - 2);
			in.open(originalFileX);
		}
		ofstream out;
		out.open(backupFile);
		out << in.rdbuf();
		cout << "\n备份完成，已经备份到 " << backupFile << " 下\n";
	}
	else
	{
		cout << endl;
	}
	return backupFile;
}