/*最后编辑：*/
/*晋晨曦 2023.9.6 14:39*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include"DPLLsolver.h"
#include"others.h"
#include"GlobalOfOldProgram.h"
int main(void)
{
	int choice{ 0 };//选择变量

	//界面输出
	Start();

	//输入选择
	cin >> choice;

	while (choice)
	{
		if (choice == 1)//SAT处理
		{
			system("cls");

			//变量定义
			char resultName[500];//结果文件名
			long long int time{ 0 };//时间
			clock_t start{ 0 };//开始时间
			clock_t end{ 0 };//结束时间

			int solveChoice{ 0 };
			SetColor(11);
			cout << "请选择解决方案:\n1. 解决方案1   2. 解决方案2   3. 解决方案3   4. 不同解决方案的区别   其他数字.返回主页面\n请选择:__\b\b";
			SetColor(7);
			cin >> solveChoice;
			if (solveChoice == 1)
			{
				//变量定义
				char fileName[500];//文件名
				int length{ 0 };//文件名长度
				Solver s;//生成类
				int res{ 0 };//返回值，有解1，无解-1
				int VARNUM{ -1 };//文字数量
				char* filenameP = nullptr;//文件名处理指针
				char* filenamePP = nullptr;//文件名处理指针

				//页面输出
				system("cls");

				SetColor(11);
				cout << "请输入文件路径（注意：复制文件地址时两边的双引号需要保留）\n____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
				SetColor(7);
				//文件名处理
				cin >> fileName;
				filenameP = fileName;
				filenamePP = fileName + 1;
				while (*filenamePP != '\"')
				{
					*filenameP = *filenamePP;
					filenameP++;
					filenamePP++;
				}
				*filenameP = '\0';
				strcpy(resultName, fileName);
				length = strlen(resultName);
				resultName[length - 1] = 's';
				resultName[length - 2] = 'e';
				resultName[length - 3] = 'r';

				//核心代码
				VARNUM = s.Read(fileName);//读取文件 
				SetColor(11);
				cout << "你是否想查看读取的cnf文件？\n1. YES   2. NO\n请输入:_\b";
				int ifShowCnf{ 0 };
				while (ifShowCnf != 1 && ifShowCnf != 2)
				{
					cin >> ifShowCnf;
					if (ifShowCnf != 1 && ifShowCnf != 2)
						cout << "输入错误，重新输入\n";
				}
				SetColor(7);
				if (ifShowCnf == 1)
					s.ShowCNF();
				if (VARNUM == -1)
				{
					res = -2;
					s.Print(0, VARNUM, fileName);
				}
				else
				{
					start = clock();
					res = s.GetAns();
					end = clock();
					time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
					s.Print(time, VARNUM, fileName);
				}
				switch (res)
				{
				case 1:
					SetColor(11);
					cout << "\n文件有解，答案已经保存到 " << resultName << " 文件下，用时 " << time << " ms\n";
					SetColor(7);
					break;
				case 0:
				case -1:
					SetColor(11);
					cout << "\n文件无解\n";
					SetColor(7);
					break;
				case -2:
					SetColor(11);
					cout << "\n文件无法打开\n";
					SetColor(7);
					break;
				default:
					break;
				}
				ShowTheRes(resultName);
				createBackup(resultName);
			}
			else if (solveChoice == 2)
			{
				//页面输出
				system("cls");

				//变量定义
				string filePath;
				howMany* howManyTimes;
				howManyTimes = new howMany[maxInt + 1];
				int VARNUM{ 0 };
				int clauseNum{ 0 };
				string fileP;
				int ifTrue{ 0 };

				//文件读取
				HeadNode* LIST = CreateClause(fileP, VARNUM, clauseNum, howManyTimes);

				//输出CNF
				SetColor(11);
				cout << "你是否想查看读取的cnf文件？\n1. YES   2. NO\n请输入:_\b";
				int ifShowCnf{ 0 };
				while (ifShowCnf != 1 && ifShowCnf != 2)
				{
					cin >> ifShowCnf;
					if (ifShowCnf != 1 && ifShowCnf != 2)
						cout << "输入错误，重新输入\n";
				}
				SetColor(7);
				if (ifShowCnf == 1)
					ShowTheCnf(LIST, VARNUM, clauseNum);

				//核心代码

				consequence* result = new consequence[VARNUM];
				start = clock();
				DeleteHeadOfOne(LIST, howManyTimes, VARNUM);
				ifTrue = DPLL(LIST, result, 1);
				end = clock();

				//写入文件
				string Outputfile = fileP;
				Outputfile.erase(Outputfile.size() - 3);
				Outputfile += "res";
				int i{ 0 };
				for (i = 0; i < Outputfile.size(); i++)
					resultName[i] = Outputfile[i];
				resultName[i] = 0;
				ofstream fos(Outputfile);
				int num{ 1 };
				fos << "s ";
				if (ifTrue)
				{
					fos << "1" << endl;
					fos << "v ";
					for (int i = 0; i < VARNUM; i++, num++)
					{
						if (result[i].value == TRUE)
							fos << i + 1 << "   ";
						else if (result[i].value == FALSE)
							fos << -(i + 1) << "   ";
						else
							fos << i + 1 << "   ";
						if (num % 10 == 0)
							fos << endl;
					}
					fos << endl;
				}
				else
				{
					fos << "0" << endl;
					fos << endl;
				}

				//写入时间
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				fos << "t " << time;
				fos.close();
				SetColor(11);
				cout << "\n文件有解，答案已经保存到 " << resultName << " 文件下，用时 " << time << " ms\n";
				SetColor(7);
				ShowTheRes(resultName);
				createBackup(resultName);
			}
			else if (solveChoice == 3)
			{
				//页面输出
				system("cls");

				//变量定义
				string filePath;
				howMany* howManyTimes;
				howManyTimes = new howMany[maxInt + 1];
				int VARNUM;
				int clauseNum{ 0 };
				string fileP;
				int ifTrue;

				//文件读取
				HeadNode* LIST = CreateClause(fileP, VARNUM, clauseNum, howManyTimes);

				//输出CNF
				SetColor(11);
				cout << "你是否想查看读取的cnf文件？\n1. YES   2. NO\n请输入:_\b";
				int ifShowCnf{ 0 };
				while (ifShowCnf != 1 && ifShowCnf != 2)
				{
					cin >> ifShowCnf;
					if (ifShowCnf != 1 && ifShowCnf != 2)
						cout << "输入错误，重新输入\n";
				}
				SetColor(7);
				if (ifShowCnf == 1)
					ShowTheCnf(LIST, VARNUM, clauseNum);

				//核心代码

				consequence* result = new consequence[VARNUM];
				start = clock();
				ifTrue = DPLL(LIST, result, 2);
				end = clock();

				//写入文件
				string Outputfile = fileP;
				Outputfile.erase(Outputfile.size() - 3);
				Outputfile += "res";
				int i{ 0 };
				for (i = 0; i < Outputfile.size(); i++)
					resultName[i] = Outputfile[i];
				resultName[i] = 0;
				ofstream fos(Outputfile);
				int num{ 1 };
				fos << "s ";
				if (ifTrue)
				{
					fos << "1" << endl;
					fos << "v ";
					for (int i = 0; i < VARNUM; i++, num++)
					{
						if (result[i].value == TRUE)
							fos << i + 1 << "   ";
						else if (result[i].value == FALSE)
							fos << -(i + 1) << "   ";
						else
							fos << i + 1 << "   ";
						if (num % 10 == 0)
							fos << endl;
					}
					fos << endl;
				}
				else
				{
					fos << "0" << endl;
					fos << endl;
				}

				//写入时间
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				fos << "t " << time;
				fos.close();
				SetColor(11);
				cout << "\n文件有解，答案已经保存到 " << resultName << " 文件下，用时 " << time << " ms\n";
				SetColor(7);
				ShowTheRes(resultName);
				createBackup(resultName);
			}
			else if (solveChoice == 4)
			{
				SetColor(11);
				cout << "三种解决方案的差别如下：\n"
					<< "第一种方案：\n第一种方案是理论上最快的方案，算法是基于 DPLL 框架进行改进的算法，加入了非时间顺序回溯、冲突驱动型子句学习、依靠活跃值启发式选择变元等操作。" << endl
					<< "第二种方案：\n第二种方案是基于最基础的DPLL算法改进的算法，它会把出现次数最多的变元优先选为赋值变元，并且会检测每种文字出现的形式，如果有只出现一种形式的文字。那么删除所有包含该文字的子句（一般没啥用）" << endl
					<< "第三种方案：\n第三种方案是最基础的DPLL采用了双向邻接表的形式加快速度，选取第一个变元作为赋值变元" << endl
					<< "你可能会奇怪，那既然第一种方案理论最快，为什么不只用第一种方案？这个问题很好，不过事实上对于SAT问题现在还不存在一种完全适配所有问题的解决方案，所以事实上对于有的问题，用方案三比方案一快很多，不过这种问题非常非常非常少。" << endl
					<< "所以，加入方案选择功能的原因很简单，就是希望通过整合各种方案来让使用者最快的获得答案。" << endl << endl;
				SetColor(7);
			}

			getchar();
			getchar();
		}
		else if (choice == 2)//普通数独
		{

			system("cls");

			//创建数独
			string filename;
			char fileName[500];
			int i{ 0 };//循环变量
			int k{ 0 };
			filename = createSudokuToFile();
			for (; i <= filename.size(); i++)
				fileName[i] = filename[k++];
			fileName[i] = '\0';

			//变量定义
			int board[8][8];
			Solver s;//生成类 
			int length{ 0 };//文件名长度
			long long int time{ 0 };//时间
			clock_t start{ 0 };//开始时间
			clock_t end{ 0 };//结束时间
			int res{ 0 };//返回值，有解1，无解-1
			int VARNUM{ -1 };//文字数量
			char resultName[500];//结果路径

			//核心代码
			VARNUM = s.Read(fileName);//读取文件
			if (VARNUM == -1)
			{
				res = -2;
				s.Print(0, VARNUM, fileName);
			}
			else
			{
				start = clock();
				res = s.GetAns();
				end = clock();
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				s.Print(time, VARNUM, fileName);
			}
			/*for (i = 0; i < strlen(fileName); i++)
				resultName[i] = fileName[i];
			resultName[i] = '\0';
			length = strlen(resultName);
			resultName[length - 1] = 's';
			resultName[length - 2] = 'e';
			resultName[length - 3] = 'r';*/
			SetColor(11);
			cout << "你是否打算输入数独答案？\n1. YES   2. NO\n请输入：_\b";
			SetColor(7);
			int ifDO{ 0 };
			cin >> ifDO;
			if (ifDO == 1)
			{
				int ans[ROW][COL];
				SetColor(11);
				cout << "请输入数独(请务必按照数独格式一行一行输入！输入完毕程序会判断答案是否正确！)：\n";
				SetColor(7);
				for (int i{ 0 }; i < 9; i++)
					for (int j{ 0 }; j < 9; j++)
						cin >> ans[i][j];
				string UserAns = UserAnsSudo(ans, filename);
				int isRight = CheckCNFandRES(filename, UserAns);
				SetColor(11);
				if (isRight == TRUE)
					cout << "\n\n----------------------\n\n" << "答案正确！^V^!" << "\n\n----------------------\n\n";
				else if (isRight == FALSE)
					cout << "\n\n----------------------\n\n" << "答案错误！T_T!" << "\n\n----------------------\n\n";
				SetColor(7);
			}
			showShuDu(fileName);
			getchar();
			getchar();
		}
		else if (choice == 3)//蜂窝数独
		{

			system("cls");

			//变量定义
			int hanidoku[ROW][COL];//数独盘
			int res{ 0 };//返回值，有解1，无解-1  
			char fileName[500];
			string linShiShuduPath;//数独文件临时储存  
			string hanidokuPath;//数独初盘文件

			//随机在数独盘中生成几个数，根据这几个数解出数独终盘
			SetColor(11);
			cout << "请输入你想生成蜂窝数独的文件夹地址（不是文件地址！）:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			SetColor(7);
			cin >> linShiShuduPath;
			do
			{
				//创建原始终盘文件
				string hanidokuFileName = CreateHanidokuForDPLL(hanidoku, linShiShuduPath);//根据一定规则随机生成数独中的几个数

				//处理文件名

				int i{ 0 };
				for (; i < hanidokuFileName.size(); i++)
					fileName[i] = hanidokuFileName[i];
				fileName[i] = '\0';

				//解出数独文件，生成终盘
				Solver s;//生成类   
				int VARNUM{ -1 };//文字数量   
				long long int time{ 0 };//时间 
				clock_t start{ 0 };//开始时间 
				clock_t end{ 0 };//结束时间 

				//核心代码
				VARNUM = s.Read(fileName);//读取文件 
				if (VARNUM == -1)
				{
					res = -2;
					s.Print(0, VARNUM, fileName);
				}
				else
				{
					start = clock();
					res = s.GetAns();
					end = clock();
					time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
					s.Print(time, VARNUM, fileName);
				}
			} while (res != 1);
			CreateHanidokuAns(fileName, hanidoku);
			DeleteFiles(fileName);
			hanidokuPath = CreatHanidokuToFile(hanidoku, fileName);
			int i{ 0 };
			for (; i < hanidokuPath.size(); i++)
				fileName[i] = hanidokuPath[i];
			fileName[i] = '\0';

			//核心代码
			Solver s;//生成类    
			int VARNUM{ -1 };//文字数量    
			long long int time{ 0 };//时间  
			clock_t start{ 0 };//开始时间  
			clock_t end{ 0 };//结束时间  

			VARNUM = s.Read(fileName);//读取文件  
			if (VARNUM == -1)
			{
				res = -2;
				s.Print(0, VARNUM, fileName);
			}
			else
			{
				start = clock();
				res = s.GetAns();
				end = clock();
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				s.Print(time, VARNUM, fileName);
			}
			SetColor(11);
			cout << "你是否打算输入数独答案？\n1. YES   2. NO\n请输入：_\b";
			SetColor(7);
			int ifDO{ 0 };
			cin >> ifDO;
			if (ifDO == 1)
			{
				int ans[ROW][COL];
				SetColor(11);
				cout << "请输入数独(请务必按照数独格式一行一行输入！输入完毕程序会判断答案是否正确！)\n例如：\n5 3 6 2 4\n4 2 5 7 6 3 \n3 6 9 4 8 5 7\n2 5 7 8 9 4 3 6 \n6 4 8 9 3 7 2 1 5\n";
				cout << "7 3 6 2 1 8 5 4 \n4 5 7 8 6 2 3 \n3 1 2 5 4 6\n5 6 4 3 7\n" << endl;
				cout << "现在请输入：\n";
				SetColor(7);
				for (int i{ 0 }; i < ROW; i++)
					for (int j{ 0 }; j <= 8 - abs(i - 4); j++)
						cin >> ans[i][j];
				string UserAns = UserAnsHanidoku(ans, hanidokuPath);
				int isRight = CheckCNFandRES(hanidokuPath, UserAns);
				SetColor(11);
				if (isRight == TRUE)
					cout << "\n\n----------------------\n\n" << "答案正确！^V^!" << "\n\n----------------------\n\n";
				else if (isRight == FALSE)
					cout << "\n\n----------------------\n\n" << "答案错误！T_T!" << "\n\n----------------------\n\n";
				SetColor(7);
			}
			KeepAnsInTxt(fileName);

			getchar();
			getchar();
		}
		else if (choice == 4)
		{
			system("cls");
			string cnfPath;
			SetColor(11);
			cout << "请输入.cnf文件的路径：____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			SetColor(7);
			cin >> cnfPath;
			cnfPath = cnfPath.substr(1, cnfPath.size() - 2);
			string resPath;
			SetColor(11);
			cout << "请输入.res文件的路径：____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			SetColor(7);
			cin >> resPath;
			resPath = resPath.substr(1, resPath.size() - 2);
			int isRight = CheckCNFandRES(cnfPath, resPath);
			SetColor(11);
			if (isRight == TRUE)
				cout << "\n\n----------------------\n\n" << "答案正确！^V^!" << "\n\n----------------------\n\n";
			else if (isRight == FALSE)
				cout << "\n\n----------------------\n\n" << "答案错误！T_T!" << "\n\n----------------------\n\n";
			SetColor(7);
			getchar();
			getchar();
		}
		else if (choice == 1024)
		{
			system("cls");
			SetColor(5);
			cout << " 10      101010   10      10  101010  101010  101010 " << endl;
			cout << " 10     10    10   10    10   10      10      10 " << endl;
			cout << " 10     10    10    10  10    101010  10        10 " << endl;
			cout << " 10     10    10     1010     10      10          10 " << endl;
			cout << " 101010  101010       10      101010  101010  101010 " << endl;
			SetColor(7);
			getchar();
			getchar();
		}
		else if (choice == 0)//退出程序
		{
			break;
		}
		else
		{
			CoutWrods(choice);
			getchar();
			getchar();
		}
		system("cls");
		Start();
		cin >> choice;
	}
	CoutWrods(0);
	getchar();
	getchar();
	return 0;
}
