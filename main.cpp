/*最后编辑：*/
/*晋晨曦 2023.9.3 3:46*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include"DPLLsolver.h"
#include"others.h"
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
			//变量定义

			Solver s;//生成类
			char fileName[500];//文件名
			char resultName[500];//结果文件名
			int length{ 0 };//文件名长度
			long long int time{ 0 };//时间
			clock_t start{ 0 };//开始时间
			clock_t end{ 0 };//结束时间
			int res{ 0 };//返回值，有解1，无解-1
			int literalNum{ -1 };//文字数量
			char* filenameP = nullptr;//文件名处理指针
			char* filenamePP = nullptr;//文件名处理指针

			//界面输出
			CoutWrods(choice);

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
			literalNum = s.read(fileName);//读取文件
			if (literalNum == -1)
			{
				res = -2;
				s.write(0, literalNum, fileName);
			}
			else
			{
				start = clock();
				res = s.solve();
				end = clock();
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				s.write(time, literalNum, fileName);
			}
			switch (res)
			{
			case 1:
				cout << "\n文件有解，答案已经保存到 " << resultName << " 文件下，用时 " << time << " ms\n";
				break;
			case 0:
			case -1:
				cout << "\n文件无解\n";
				break;
			case -2:
				cout << "\n文件无法打开\n";
				break;
			default:
				break;
			}
			ShowTheRes(resultName);
			createBackup(resultName);
			getchar();
			getchar();
		}
		else if (choice == 2)//普通数独
		{
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
			int literalNum{ -1 };//文字数量
			char resultName[500];//结果路径

			//核心代码
			literalNum = s.read(fileName);//读取文件
			if (literalNum == -1)
			{
				res = -2;
				s.write(0, literalNum, fileName);
			}
			else
			{
				start = clock();
				res = s.solve();
				end = clock();
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
				s.write(time, literalNum, fileName);
			}
			/*for (i = 0; i < strlen(fileName); i++)
				resultName[i] = fileName[i];
			resultName[i] = '\0';
			length = strlen(resultName);
			resultName[length - 1] = 's';
			resultName[length - 2] = 'e';
			resultName[length - 3] = 'r';*/
			showShuDu(fileName);
		}
		else if (choice == 3)//蜂窝数独
		{

			int hanidoku[ROW][COL];//数独盘
			int res{ 0 };//返回值，有解1，无解-1  
			char fileName[500];
			string linShiShuduPath;//数独文件临时储存  
			string hanidokuPath;//数独初盘文件

			//随机在数独盘中生成几个数，根据这几个数解出数独终盘

			cout << "请输入你想生成蜂窝数独的文件夹地址（不是文件地址！）:____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
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
				int literalNum{ -1 };//文字数量   
				long long int time{ 0 };//时间 
				clock_t start{ 0 };//开始时间 
				clock_t end{ 0 };//结束时间 

				//核心代码
				literalNum = s.read(fileName);//读取文件 
				if (literalNum == -1)
				{
					res = -2;
					s.write(0, literalNum, fileName);
				}
				else
				{
					start = clock();
					res = s.solve();
					end = clock();
					time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
					s.write(time, literalNum, fileName);
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
			int literalNum{ -1 };//文字数量    
			long long int time{ 0 };//时间  
			clock_t start{ 0 };//开始时间  
			clock_t end{ 0 };//结束时间  

			//核心代码
			literalNum = s.read(fileName);//读取文件  
			if (literalNum == -1) 
			{
				res = -2; 
				s.write(0, literalNum, fileName); 
			}
			else
			{
				start = clock(); 
				res = s.solve(); 
				end = clock();
				time = (double)(end - start) / CLOCKS_PER_SEC * 1000.0; 
				s.write(time, literalNum, fileName); 
			}
			KeepAnsInTxt(fileName);

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
	return 0;
}