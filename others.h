#pragma once
#include<bits/stdc++.h>
using namespace std;

const int ROW{ 9 };
const int COL{ 9 };

void Start();
void CoutWrods(int choice);
void ShowTheRes(char filePath[]);
string createBackup(char originalFile[]);
string createSudokuToFile();
void createSudoku(int a[][COL]);
void createStartinggrid(const int a[][COL], int b[][COL], int numDigits);
void print(const int a[][COL], string shuduPath);
string ToCnf(int a[][COL], int holes, string shuduPath);
void randomFirstRow(int a0[], int n);
int Digit(int a[][COL], int i, int j);
int index(int i, int j);
void showShuDu(char fileName[]);