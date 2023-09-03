/*最后编辑：*/
/*晋晨曦 2023.9.3 3:46*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;

const int ROW{ 9 };
const int COL{ 9 };

//界面设定
void Start();
void CoutWrods(int choice);
void ShowTheRes(char filePath[]);

//DPLL
string createBackup(char originalFile[]);

//普通熟读
string createSudokuToFile();
void createSudoku(int a[][COL]);
void createStartinggrid(const int a[][COL], int b[][COL], int numDigits);
void print(const int a[][COL], string shuduPath);
string ToCnf(int a[][COL], int holes, string shuduPath);
void randomFirstRow(int a0[], int n);
int Digit(int a[][COL], int i, int j);
int index(int i, int j);
void showShuDu(char fileName[]);

//蜂窝数独

/// <summary>
/// 将数独挖洞后扔到文件里
/// </summary>
/// <param name="hanidoku">数独终盘</param>
/// <param name="filePath">数独文件路径</param>
/// <returns></returns>
string CreatHanidokuToFile(int hanidoku[][COL], string filePath);

/// <summary>
/// 初始化数度盘
/// </summary>
/// <param name="hannidoku">表示需要初始化的二维数组</param>
void MakeHanidokuSure(int hannidoku[][COL]);

/// <summary>
/// 初始化数独，并且根据一定规则生成数独中的几个数，再把数独写入文件，返回文件名
/// </summary>
/// <param name="hanidoku">数独盘</param>
/// <param name="shuduPath">数独文件夹路径</param>
/// <returns></returns>
string CreateHanidokuForDPLL(int hanidoku[ROW][COL], string shuduPath);

/// <summary>
/// 在三个固定的位置生成三个随机数
/// </summary>
/// <param name="midRow">表示数独的中间一行</param>
void RandomThreeNumsInMidRowOfHanidoku(int midRow[]);

/// <summary>
/// 在两个随机的位置生成随机数
/// </summary>
/// <param name="hanidoku">表示数独盘</param>
void RandomTwoNumsOfHanidoku(int hanidoku[][COL]);

/// <summary>
/// 根据已经生成的数字生成其他数字，适配于回溯生成数独
/// </summary>
/// <param name="hanidoku">表示数独盘</param>
/// <param name="i">表示当前横坐标</param>
/// <param name="j">表示当前纵坐标</param>
/// <returns></returns>
int FindOtherNums(int hanidoku[][COL], int i, int j);

/// <summary>
/// 专门适配根据随机数逆推数独的生成方式的生成数字函数
/// </summary>
/// <param name="hanidoku">数独盘</param>
/// <param name="i">当前横坐标</param>
/// <param name="j">当前纵坐标</param>
/// <returns>返回是否成功</returns>
int FindOtherNumsX(int hanidoku[][COL], int i, int j);

/// <summary>
/// 获得当前位置的互异列表
/// </summary>
/// <param name="hanidoku">数独盘</param>
/// <param name="check">检查数组</param>
/// <param name="i">当前横坐标</param>
/// <param name="j">当前纵坐标</param>
void GetCheck(int hanidoku[][COL], int check[], int i, int j);

/// <summary>
/// 写入数独文件到CNF
/// </summary>
/// <param name="a">数度盘</param>
/// <param name="holes">挖孔数</param>
/// <param name="shuduPath">数独文件路径</param>
/// <returns>返回结果路径</returns>
string HanidokuToCnf(int a[][COL], int holes, string shuduPath);

/// <summary>
/// 转化数独RES文件到数独盘中
/// </summary>
/// <param name="fileName">数独res文件</param>
/// <param name="hanidoku">数度盘</param>
void CreateHanidokuAns(char fileName[], int hanidoku[][COL]);

/// <summary>
/// 删除生成数独终盘的中间文件
/// </summary>
/// <param name="FileNames">文件路径</param>
void DeleteFiles(char FileNames[]);

/// <summary>
/// 创建数独初盘
/// </summary>
/// <param name="hanidoku">数独终盘</param>
/// <param name="starting_grid">数独初盘</param>
/// <param name="holes">挖洞数</param>
void CreateHanidokuQus(int hanidoku[][COL], int starting_grid[][COL], int holes);

/// <summary>
/// 将初盘保存到文件
/// </summary>
/// <param name="starting_grid"></param>
/// <param name="fileName"></param>
void KeepStartInTxt(int starting_grid[][COL], string fileName);

/// <summary>
/// 将结果写入文件
/// </summary>
/// <param name="fileName">res文件的地址</param>
void KeepAnsInTxt(char fileName[]);





