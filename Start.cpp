#include"others.h"
void Start()
{
    cout << "�����ص�SAT������ϵͳ" << endl;
    cout << "1. ���SAT����   2. �����ͨ��������   3.��������������⡣   0. �˳�ϵͳ" << endl;
    cout << "��ѡ��:_\b";
}
void CoutWrods(int choice)
{
    switch (choice)
    {
    case 1:
        system("cls");
        cout << "�������ļ�·����ע�⣺�����ļ���ַʱ���ߵ�˫������Ҫ������\n____________________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
        break;
    case 0:
        system("cls");
        cout << "\n\n---------------------------------------------\n\n" << "��������������˳����ݰ�~~~" << "\n\n---------------------------------------------\n\n";
        break;
    default:
        system("cls");
        cout << "�����������������!";
    }
}