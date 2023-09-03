/*最后编辑：*/
/*晋晨曦 2023.9.3 3:46*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#include"others.h"
void Start()
{
    cout << "晋晨曦的SAT问题解决系统" << endl;
    cout << "1. 解决SAT问题   2. 解决普通数独问题   3.解决蜂窝数独问题   0. 退出系统" << endl;
    cout << "请选择:_\b";
}
void CoutWrods(int choice)
{
    switch (choice)
    { 
    case 0:
        system("cls");
        cout << "\n\n---------------------------------------------\n\n" << "程序结束，现在退出，拜拜~~~" << "\n\n---------------------------------------------\n\n";
        break;
    default:
        system("cls");
        cout << "输入错误，请重新输入!";
    }
}