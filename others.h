/*���༭��*/
/*������ 2023.9.3 3:46*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;

const int ROW{ 9 };
const int COL{ 9 };

//�����趨
void Start();
void CoutWrods(int choice);
void ShowTheRes(char filePath[]);

//DPLL
string createBackup(char originalFile[]);

//��ͨ���
string createSudokuToFile();
void createSudoku(int a[][COL]);
void createStartinggrid(const int a[][COL], int b[][COL], int numDigits);
void print(const int a[][COL], string shuduPath);
string ToCnf(int a[][COL], int holes, string shuduPath);
void randomFirstRow(int a0[], int n);
int Digit(int a[][COL], int i, int j);
int index(int i, int j);
void showShuDu(char fileName[]);

//��������

/// <summary>
/// �������ڶ����ӵ��ļ���
/// </summary>
/// <param name="hanidoku">��������</param>
/// <param name="filePath">�����ļ�·��</param>
/// <returns></returns>
string CreatHanidokuToFile(int hanidoku[][COL], string filePath);

/// <summary>
/// ��ʼ��������
/// </summary>
/// <param name="hannidoku">��ʾ��Ҫ��ʼ���Ķ�ά����</param>
void MakeHanidokuSure(int hannidoku[][COL]);

/// <summary>
/// ��ʼ�����������Ҹ���һ���������������еļ��������ٰ�����д���ļ��������ļ���
/// </summary>
/// <param name="hanidoku">������</param>
/// <param name="shuduPath">�����ļ���·��</param>
/// <returns></returns>
string CreateHanidokuForDPLL(int hanidoku[ROW][COL], string shuduPath);

/// <summary>
/// �������̶���λ���������������
/// </summary>
/// <param name="midRow">��ʾ�������м�һ��</param>
void RandomThreeNumsInMidRowOfHanidoku(int midRow[]);

/// <summary>
/// �����������λ�����������
/// </summary>
/// <param name="hanidoku">��ʾ������</param>
void RandomTwoNumsOfHanidoku(int hanidoku[][COL]);

/// <summary>
/// �����Ѿ����ɵ����������������֣������ڻ�����������
/// </summary>
/// <param name="hanidoku">��ʾ������</param>
/// <param name="i">��ʾ��ǰ������</param>
/// <param name="j">��ʾ��ǰ������</param>
/// <returns></returns>
int FindOtherNums(int hanidoku[][COL], int i, int j);

/// <summary>
/// ר���������������������������ɷ�ʽ���������ֺ���
/// </summary>
/// <param name="hanidoku">������</param>
/// <param name="i">��ǰ������</param>
/// <param name="j">��ǰ������</param>
/// <returns>�����Ƿ�ɹ�</returns>
int FindOtherNumsX(int hanidoku[][COL], int i, int j);

/// <summary>
/// ��õ�ǰλ�õĻ����б�
/// </summary>
/// <param name="hanidoku">������</param>
/// <param name="check">�������</param>
/// <param name="i">��ǰ������</param>
/// <param name="j">��ǰ������</param>
void GetCheck(int hanidoku[][COL], int check[], int i, int j);

/// <summary>
/// д�������ļ���CNF
/// </summary>
/// <param name="a">������</param>
/// <param name="holes">�ڿ���</param>
/// <param name="shuduPath">�����ļ�·��</param>
/// <returns>���ؽ��·��</returns>
string HanidokuToCnf(int a[][COL], int holes, string shuduPath);

/// <summary>
/// ת������RES�ļ�����������
/// </summary>
/// <param name="fileName">����res�ļ�</param>
/// <param name="hanidoku">������</param>
void CreateHanidokuAns(char fileName[], int hanidoku[][COL]);

/// <summary>
/// ɾ�������������̵��м��ļ�
/// </summary>
/// <param name="FileNames">�ļ�·��</param>
void DeleteFiles(char FileNames[]);

/// <summary>
/// ������������
/// </summary>
/// <param name="hanidoku">��������</param>
/// <param name="starting_grid">��������</param>
/// <param name="holes">�ڶ���</param>
void CreateHanidokuQus(int hanidoku[][COL], int starting_grid[][COL], int holes);

/// <summary>
/// �����̱��浽�ļ�
/// </summary>
/// <param name="starting_grid"></param>
/// <param name="fileName"></param>
void KeepStartInTxt(int starting_grid[][COL], string fileName);

/// <summary>
/// �����д���ļ�
/// </summary>
/// <param name="fileName">res�ļ��ĵ�ַ</param>
void KeepAnsInTxt(char fileName[]);





