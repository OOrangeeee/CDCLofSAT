/*最后编辑：*/
/*晋晨曦 2023.9.3 23:54*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include <unordered_map>
#include"ImportantData.h"
using namespace std;
typedef int status;

//邻接表数据节点
struct DataNode
{
	int data = 0;//数据
	DataNode* next{};//下一个
	DataNode* pre{};//上一个
};

//邻接表头节点
struct HeadNode
{
	int Num = 0;//文字数
	DataNode* right{};//第一个文字
	HeadNode* down{};//下一个子句
	HeadNode* up{};//上一个子句
};

//用于记录真假值
struct consequence
{
	int value = -1;//存真值 真时为true-1，假时为false-0
};

//用于记录每个文字的真假值
struct conse
{
	int num = 0;
	int value = -1;//存真值 真时为true-1，假时为false-0
};

//用于计算每个文字出现了几种形式
//struct times
//{
//	int data{ 0 };
//	int fuHao{ -1 };//0-  1+
//	int isHere{ 0 };//0没出现    1出现
//};

//用于计算每个文字出现了几种形式
struct howMany
{
	int ifTure{ 0 };
	int ifFalse{ 0 };
	int howM{ 0 };
};

/// <summary>
/// 找到有某个文字的子句
/// </summary>
/// <param name="LIST">邻接表</param>
/// <param name="num">寻找的文字</param>
/// <returns>对应的子句地址</returns>
HeadNode* FindHeadOfOne(HeadNode*& LIST, int num);

/// <summary>
/// 删除所有只出现一种形式的文字所在的子句
/// </summary>
/// <param name="LIST">邻接表</param>
/// <param name="howManyTimes">出现次数</param>
/// <param name="VARNUM">文字数量</param>
void DeleteHeadOfOne(HeadNode*& LIST, howMany* howManyTimes, int VARNUM);

/// <summary>
/// 选择出现次数最多的文字作为赋值变元
/// </summary>
/// <param name="LIST">邻接表</param>
/// <returns>该文字的值</returns>
int choose(HeadNode* LIST);

/// <summary>
/// 删除整个邻接表（回溯时要释放内存）
/// </summary>
/// <param name="LIST">邻接表</param>
void DeleteList(HeadNode*& LIST);

/// <summary>
/// 读取文件
/// </summary>
/// <param name="fileP">文件地址</param>
/// <param name="VARNUM">文字数目</param>
/// <param name="ClauseNum">子句数目</param>
/// <param name="howManyTimes">出现了多少次</param>
/// <returns>返回头节点的第一个</returns>
HeadNode* CreateClause(string& fileP, int& VARNUM, int& clauseNum, howMany*& howManyTimes);

/// <summary>
/// 读取文件的重载形式，用于无需计算出现多少次的读取情况
/// </summary>
/// <param name="fileP">文件地址</param>
/// <param name="VARNUM">文字数目</param>
/// <param name="ClauseNum">子句数目</param>
/// <returns>返回头节点的第一个</returns>
HeadNode* CreateClause(string fileP, int& VARNUM, int& clauseNum);

/// <summary>
/// DPLL
/// </summary>
/// <param name="LIST">邻接表</param>
/// <param name="result">结果数组</param>
/// <param name="chooseWay">选择实现方式</param>
/// <returns>返回是否有解</returns>
status DPLL(HeadNode*& LIST, consequence* result, int chooseWay);

/// <summary>
/// 找到单子句
/// </summary>
/// <param name="">头节点</param>
/// <returns>返回位置</returns>
HeadNode* IsSingleClause(HeadNode* Pfind);

/// <summary>
/// 判空
/// </summary>
/// <param name="LIST">头节点</param>
/// <returns>返回是否是空的</returns>
status IsEmptyClause(HeadNode*& LIST);

/// <summary>
/// 添加单子句
/// </summary>
/// <param name="">头节点</param>
/// <param name="">单子句中的文字</param>
/// <returns>返回头节点</returns>
HeadNode* ADDSingleClause(HeadNode*& LIST, int Var);

/// <summary>
/// 在进入下一次递归之前赋值邻接表
/// </summary>
/// <param name="">邻接表</param>
/// <returns>返回复制好的头节点</returns>
HeadNode* Duplication(HeadNode*& LIST);

/// <summary>
/// 删除目标子句
/// </summary>
/// <param name="delGoal">目标子句</param>
/// <param name="LIST">邻接表</param>
/// <returns>返回删除子句的下一个子句</returns>
HeadNode* DeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);

/// <summary>
/// 删除目标子句但是不返回下一个，用于删除第一个单子句
/// </summary>
/// <param name="delGoal">目标子句</param>
/// <param name="LIST">邻接表</param>
void XDeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);

/// <summary>
/// 删除有temp的子句或者删除-temp文字
/// </summary>
/// <param name="">要删除的值</param>
/// <param name="">邻接表</param>
void DeleteDataNode(int temp, HeadNode*& LIST);

/// <summary>
/// 判断res是否正确
/// </summary>
/// <param name="CNF">问题路径</param>
/// <param name="RES">答案路径</param>
/// <returns>返回是否正确</returns>
int CheckCNFandRES(string CNF, string RES);

/// <summary>
/// 输出CNF
/// </summary>
/// <param name="HEAD">邻接表头</param>
/// <param name="VARNUM">文字数目</param>
/// <param name="ClasuseNum">子句数目</param>
void ShowTheCnf(HeadNode* HEAD, const int VARNUM, const int ClasuseNum);
