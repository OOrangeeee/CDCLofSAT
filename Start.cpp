/*���༭��*/
/*������ 2023.9.3 3:46*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#include"others.h"
void Start()
{
    cout << "�����ص�SAT������ϵͳ" << endl;
    cout << "1. ���SAT����   2. �����ͨ��������   3.���������������   0. �˳�ϵͳ" << endl;
    cout << "��ѡ��:_\b";
}
void CoutWrods(int choice)
{
    switch (choice)
    { 
    case 0:
        system("cls");
        cout << "\n\n---------------------------------------------\n\n" << "��������������˳����ݰ�~~~" << "\n\n---------------------------------------------\n\n";
        break;
    default:
        system("cls");
        cout << "�����������������!";
    }
}