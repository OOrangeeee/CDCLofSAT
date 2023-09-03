#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include <unordered_map>
#include"ImportantData.h"
using namespace std;

typedef int status;

struct DataNode
{
	int data = 0;
	DataNode* next{};
	DataNode* pre{};
};

struct HeadNode
{
	int Num = 0;
	DataNode* right{};
	HeadNode* down{};
	HeadNode* up{};
};

struct consequence
{
	int value = -1;//存真值 真时为true-1，假时为false-0
};

struct conse
{
	int num = 0;
	int value = -1;//存真值 真时为true-1，假时为false-0
};

struct times
{
	int data{ 0 };
	int fuHao{ -1 };//0-  1+
	int isHere{ 0 };//0没出现    1出现
};

struct howMany
{
	int ifTure{ 0 };
	int ifFalse{ 0 };
	int howM{ 0 };
};

string createBackup(string originalFile);
void ShowTheRes(string filePath);
void ShowDifference();
HeadNode* FindHeadOfOne(HeadNode*& LIST, int num);
void DeleteHeadOfOne(HeadNode*& LIST, howMany* howManyTimes, int VARNUM);
int choose(HeadNode* LIST);
void Start();
void DeleteList(HeadNode*& LIST);
HeadNode* CreateClause(string& fileP, int& VARNUM, howMany*& howManyTimes);
status DPLL(HeadNode*& LIST, consequence* result, int chooseWay);
HeadNode* IsSingleClause(HeadNode*);
status IsEmptyClause(HeadNode*& LIST);
HeadNode* ADDSingleClause(HeadNode*&, int);
HeadNode* Duplication(HeadNode*&);
HeadNode* DeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);
void XDeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);
void DeleteDataNode(int, HeadNode*&);
void show(struct consequence*, int);
int Digit(int a[][COL], int i, int j);
void randomFirstRow(int a0[], int n);
void createSudoku(int a[][COL]);
void createStartinggrid(const int a[][COL], int b[][COL], int numDigits);
void print(const int a[][COL]);
string ToCnf(int a[][COL], int holes);
string createSudokuToFile();
status SudoDPLL(HeadNode* LIST, conse* result, int VARNUM);
void SudokuShow(conse* result, int VARNUM);
