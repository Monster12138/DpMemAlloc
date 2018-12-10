#include "os.h"

void menu()
{
    int chose = 0;
    while(1)
    {
        os s;
        s.init();
        s.create_empty();
        cout << "1.首次适应算法" << endl;
        cout << "2.循环首次适应算法" << endl;
        cout << "3.最佳适应算法" << endl;
        cout << "4.最坏适应算法" << endl;
        cout << "0.退出" << endl;

        cout << "请输入选项:>";
        cin >> chose;
        switch(chose){
        case 0:exit(0);
        case 1:s.FF();break;
        case 2:s.NF();break;
        case 3:s.BF();break;
        case 4:s.WF();break;
        default:continue;
        }
    }
}

int main()
{
    menu();
    return 0;
}
