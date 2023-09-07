/*���༭��*/
/*������ 2023.9.3 23:54*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include <unordered_map>
#include"ImportantData.h"
using namespace std;
typedef int status;

//�ڽӱ����ݽڵ�
struct DataNode
{
	int data = 0;//����
	DataNode* next{};//��һ��
	DataNode* pre{};//��һ��
};

//�ڽӱ�ͷ�ڵ�
struct HeadNode
{
	int Num = 0;//������
	DataNode* right{};//��һ������
	HeadNode* down{};//��һ���Ӿ�
	HeadNode* up{};//��һ���Ӿ�
};

//���ڼ�¼���ֵ
struct consequence
{
	int value = -1;//����ֵ ��ʱΪtrue-1����ʱΪfalse-0
};

//���ڼ�¼ÿ�����ֵ����ֵ
struct conse
{
	int num = 0;
	int value = -1;//����ֵ ��ʱΪtrue-1����ʱΪfalse-0
};

//���ڼ���ÿ�����ֳ����˼�����ʽ
//struct times
//{
//	int data{ 0 };
//	int fuHao{ -1 };//0-  1+
//	int isHere{ 0 };//0û����    1����
//};

//���ڼ���ÿ�����ֳ����˼�����ʽ
struct howMany
{
	int ifTure{ 0 };
	int ifFalse{ 0 };
	int howM{ 0 };
};

/// <summary>
/// �ҵ���ĳ�����ֵ��Ӿ�
/// </summary>
/// <param name="LIST">�ڽӱ�</param>
/// <param name="num">Ѱ�ҵ�����</param>
/// <returns>��Ӧ���Ӿ��ַ</returns>
HeadNode* FindHeadOfOne(HeadNode*& LIST, int num);

/// <summary>
/// ɾ������ֻ����һ����ʽ���������ڵ��Ӿ�
/// </summary>
/// <param name="LIST">�ڽӱ�</param>
/// <param name="howManyTimes">���ִ���</param>
/// <param name="VARNUM">��������</param>
void DeleteHeadOfOne(HeadNode*& LIST, howMany* howManyTimes, int VARNUM);

/// <summary>
/// ѡ����ִ�������������Ϊ��ֵ��Ԫ
/// </summary>
/// <param name="LIST">�ڽӱ�</param>
/// <returns>�����ֵ�ֵ</returns>
int choose(HeadNode* LIST);

/// <summary>
/// ɾ�������ڽӱ�����ʱҪ�ͷ��ڴ棩
/// </summary>
/// <param name="LIST">�ڽӱ�</param>
void DeleteList(HeadNode*& LIST);

/// <summary>
/// ��ȡ�ļ�
/// </summary>
/// <param name="fileP">�ļ���ַ</param>
/// <param name="VARNUM">������Ŀ</param>
/// <param name="ClauseNum">�Ӿ���Ŀ</param>
/// <param name="howManyTimes">�����˶��ٴ�</param>
/// <returns>����ͷ�ڵ�ĵ�һ��</returns>
HeadNode* CreateClause(string& fileP, int& VARNUM, int& clauseNum, howMany*& howManyTimes);

/// <summary>
/// ��ȡ�ļ���������ʽ���������������ֶ��ٴεĶ�ȡ���
/// </summary>
/// <param name="fileP">�ļ���ַ</param>
/// <param name="VARNUM">������Ŀ</param>
/// <param name="ClauseNum">�Ӿ���Ŀ</param>
/// <returns>����ͷ�ڵ�ĵ�һ��</returns>
HeadNode* CreateClause(string fileP, int& VARNUM, int& clauseNum);

/// <summary>
/// DPLL
/// </summary>
/// <param name="LIST">�ڽӱ�</param>
/// <param name="result">�������</param>
/// <param name="chooseWay">ѡ��ʵ�ַ�ʽ</param>
/// <returns>�����Ƿ��н�</returns>
status DPLL(HeadNode*& LIST, consequence* result, int chooseWay);

/// <summary>
/// �ҵ����Ӿ�
/// </summary>
/// <param name="">ͷ�ڵ�</param>
/// <returns>����λ��</returns>
HeadNode* IsSingleClause(HeadNode* Pfind);

/// <summary>
/// �п�
/// </summary>
/// <param name="LIST">ͷ�ڵ�</param>
/// <returns>�����Ƿ��ǿյ�</returns>
status IsEmptyClause(HeadNode*& LIST);

/// <summary>
/// ��ӵ��Ӿ�
/// </summary>
/// <param name="">ͷ�ڵ�</param>
/// <param name="">���Ӿ��е�����</param>
/// <returns>����ͷ�ڵ�</returns>
HeadNode* ADDSingleClause(HeadNode*& LIST, int Var);

/// <summary>
/// �ڽ�����һ�εݹ�֮ǰ��ֵ�ڽӱ�
/// </summary>
/// <param name="">�ڽӱ�</param>
/// <returns>���ظ��ƺõ�ͷ�ڵ�</returns>
HeadNode* Duplication(HeadNode*& LIST);

/// <summary>
/// ɾ��Ŀ���Ӿ�
/// </summary>
/// <param name="delGoal">Ŀ���Ӿ�</param>
/// <param name="LIST">�ڽӱ�</param>
/// <returns>����ɾ���Ӿ����һ���Ӿ�</returns>
HeadNode* DeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);

/// <summary>
/// ɾ��Ŀ���Ӿ䵫�ǲ�������һ��������ɾ����һ�����Ӿ�
/// </summary>
/// <param name="delGoal">Ŀ���Ӿ�</param>
/// <param name="LIST">�ڽӱ�</param>
void XDeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST);

/// <summary>
/// ɾ����temp���Ӿ����ɾ��-temp����
/// </summary>
/// <param name="">Ҫɾ����ֵ</param>
/// <param name="">�ڽӱ�</param>
void DeleteDataNode(int temp, HeadNode*& LIST);

/// <summary>
/// �ж�res�Ƿ���ȷ
/// </summary>
/// <param name="CNF">����·��</param>
/// <param name="RES">��·��</param>
/// <returns>�����Ƿ���ȷ</returns>
int CheckCNFandRES(string CNF, string RES);

/// <summary>
/// ���CNF
/// </summary>
/// <param name="HEAD">�ڽӱ�ͷ</param>
/// <param name="VARNUM">������Ŀ</param>
/// <param name="ClasuseNum">�Ӿ���Ŀ</param>
void ShowTheCnf(HeadNode* HEAD, const int VARNUM, const int ClasuseNum);
