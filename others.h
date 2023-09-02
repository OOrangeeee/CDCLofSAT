/*最后编辑：*/
/*晋晨曦 2023.9.3 3:46*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#pragma once
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
string CreatHanidokuToFile();
void MakeHanidokuSure(int hannidoku[][COL]);
string CreateHanidoku(int hanidoku[ROW][COL]);
void RandomMidRowOfHanidoku(int midRow[]);
void RandomFiveNumsOfHanidoku(int hanidoku[][COL]);
int FindOtherNums(int hanidoku[][COL], int i, int j);
int FindOtherNumsX(int hanidoku[][COL], int i, int j);
void GetCheck(int hanidoku[][COL], int check[], int i, int j);
string HanidokuToCnf(int a[][COL], int holes, string shuduPath);