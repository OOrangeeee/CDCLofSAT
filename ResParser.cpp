#include"others.h"
void ShowTheRes(char filePath[])
{
	int x{ 0 };
	cout << "\n���Ƿ���鿴������ݣ�\n1. YES   2. NO\n������:_\b";
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
			cout << "\n��ȡ�����ļ��򿪴�����������ȷ���ļ���ʽ�����Ƶ�ַ��˫������Ҫ����\n\n";
			cin >> fileP;
			fis.open(fileP);
		}
		string Line;
		while (getline(fis, Line))
			cout << Line << endl;
		cout << "--------------------------------------\n";
		cout << "�ļ�������\n";
	}
	else
	{
		cout << endl;
	}
}
string createBackup(char originalFile[])
{
	int x{ 0 };
	cout << "\n���Ƿ���Ҫ�����ļ�?\n1. YES   2.NO\n������:_\b";
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
			cout << "\n�ڱ����ļ�ʱ��Ŀ���ļ��򿪴�����������ȷ���ļ���ʽ�������ļ���ַʱ���ߵ�˫������Ҫ����\n\n";
			string originalFileX;
			cin >> originalFileX;
			originalFileX = originalFileX.substr(1, originalFileX.size() - 2);
			in.open(originalFileX);
		}
		ofstream out;
		out.open(backupFile);
		out << in.rdbuf();
		cout << "\n������ɣ��Ѿ����ݵ� " << backupFile << " ��\n";
	}
	else
	{
		cout << endl;
	}
	return backupFile;
}