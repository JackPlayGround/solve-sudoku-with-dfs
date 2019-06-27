#include<iostream>
#include"sudoku.h"
using namespace std;
int main() {
    int a,b,c,i;
    Sudoku s;
    s.put();
    while(1) {
        cin>>a;
        if(a==0) {
            break;
        }
        switch(a) {
        case 1:
            cin >> b >> c;
            s.change_num(b,c);
            break;
        case 2:
            cin >> b >> c;
            s.change_row(b,c);
            break;
        case 3:
            cin >> b >> c;
            s.change_col(b,c);
            break;
        case 4:
            cin >> b;
            s.rotate(b);
            break;
        case 5:
            cin >> b;
            s.flip(b);
            break;
        }
    }
    s.print_sdk();
    return 0;
}
