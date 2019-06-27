#include"sudoku.h"
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<iomanip>
using namespace std;

/*generate a sdk*/
void Sudoku::gen() {
    int i,j;
    int sdk[SDKSIZ]= {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        4, 5, 6, 7, 8, 9, 1, 2, 3,
        7, 8, 9, 1, 2, 3, 4, 5, 6,
        2, 3, 1, 5, 6, 4, 8, 9, 7,
        5, 6, 4, 8, 9, 7, 2, 3, 1,
        8, 9, 7, 2, 3, 1, 5, 6, 4,
        3, 1, 2, 6, 4, 5, 9, 7, 8,
        6, 4, 5, 9, 7, 8, 3, 1, 2,
        9, 7, 8, 3, 1, 2, 6, 4, 5
    };
    for(i=0; i<SDKSIZ; ++i) {
        map[i]=sdk[i];
    }
    srand(time(NULL));
    int t=rand()%10+55;
    for(i=0; i<t; ++i) {
        map[rand()%SDKSIZ]=0;
    }
}

/*print out sdk*/
void Sudoku::print_sdk() {
    int i,j,tmp=0;
    for(i=0; i<9; ++i) {
        cout << map[tmp++];
        for(j=1; j<9; ++j) {
            cout << " ";
            cout << map[tmp++];
        }
        cout << endl;
    }
}

/*enter number into sdk*/
void Sudoku::put() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    zeronum=0;
    for(int i=0; i<SDKSIZ; ++i) {
        cin >> map[i];
        if(map[i]==0) {
            ++zeronum;
        }
    }
}

/*change locations of two different nums*/
void Sudoku::change_num(int n1, int n2) {
    for(int i=0; i<SDKSIZ; ++i) {
        if(map[i]==n1) {
            map[i]=n2;
        } else if(map[i]==n2) {
            map[i]=n1;
        }
    }
}

/*change two rows*/
void Sudoku::change_row(int n1, int n2) {
    int tmp, s1=n1*27,s2=n2*27;
    for(int i=0; i<27; ++i) {
        tmp=map[s1];
        map[s1++]=map[s2];
        map[s2++]=tmp;
    }
}

/*change two columns*/
void Sudoku::change_col(int n1, int n2) {
    int tmp, s1=n1*3, s2=n2*3, i, j;
    for(i=0; i<9; ++i) {
        s1= n1*3+i*9;
        s2= n2*3+i*9;
        for(j=0; j<3; ++j) {
            tmp=map[s1];
            map[s1++]=map[s2];
            map[s2++]=tmp;
        }
    }
}

/*rotate clockwise*/
void Sudoku::rotate(int t) {
    int tmp[SDKSIZ];
    int i,j,k,l;
    for(i=0; i<t%4; ++i) {
        for(j=0,l=8; j<SDKSIZ; j+=9,--l) {
            for(k=0; k<9; ++k) {
                tmp[k*9+l]=map[j+k];
            }
        }
        for(j=0; j<SDKSIZ; ++j) {
            map[j]=tmp[j];
        }
    }
}

/*updown flip leftright flip*/
void Sudoku::flip(int opt) {
    int tmp,i,j,k;
    if(opt==0) {
        for(i=0,j=72; i<36; i+=9,j-=9) {
            for(k=0; k<9; ++k) {
                tmp=map[i+k];
                map[i+k]=map[j+k];
                map[j+k]=tmp;
            }
        }
    } else if(opt==1) {
        for(i=4; i<SDKSIZ; i+=9) {
            for(j=1; j<5; ++j) {
                tmp=map[i+j];
                map[i+j]=map[i-j];
                map[i-j]=tmp;
            }
        }
    }
}

/*********** d l x ***************/

void Sudoku::print_sol() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int i;
    for(i=0; i<SDKSIZ; i++) {
        cout<<solvemap[i];
        cout<<(((i+1)%9==0)?'\n':' ');
    }
}

void Sudoku::setup() {
    nodes=new node[DLX_R*DLX_C];//create DLX board
    row=new node[DLX_R];
    col=new node[DLX_C+1];
    result=new int[DLX_R];
    int i;
    rownum=DLX_R;
    colnum=DLX_C;

    col[0].up=col[0].down=col;
    col[0].left=col+colnum;
    col[0].right=col+1;
    col[0].nodesum=0;
    col[0].questionnum=0;
    for(i=1; i<colnum; i++) {
        col[i].up=col[i].down=col+i;
        col[i].left=col+(i-1);
        col[i].right=col+(i+1);
        col[i].nodesum=0;
        col[i].questionnum=0;
    }
    col[i].up=col[i].down=col+i;
    col[i].left=col+(i-1);
    col[i].right=col;
    col[i].nodesum=0;
    col[i].questionnum=0;

    head=col+colnum;
    for(i=0; i<rownum; i++) {
        row[i].up=row[i].down=row[i].left=row[i].right=row[i].root=row+i;
    }
    nodecount=0;
}
void Sudoku::addnode(int r,int c) {
    //cout<<r<<" "<<c<<endl;
    nodes[nodecount].up=col[c].up;
    nodes[nodecount].down=col+c;
    nodes[nodecount].left=row[r].left;
    nodes[nodecount].right=row+r;
    nodes[nodecount].row=r;
    nodes[nodecount].root=col+c;
    col[c].up=col[c].up->down=row[r].left=row[r].left->right=nodes+nodecount++;
    ++col[c].nodesum;
}

bool Sudoku::dance(int k) {
    if(head->right==head) {
        int i,j;
        for(i=0; i<SDKSIZ; i++) { //store first solution to check have any other solution
            j=result[i];
            solvemap[j/9]=1+j%9;
        }
        solvenum++;
        if(solvenum>1) {
            return true;
        } else {
            //cout<<k<<endl;
            resultcount=k;
            return false;
        }
    }
    node *min,*p,*q,*r,*s,*t;
    for(min=head->right,p=min->right; p!=head; p=p->right) {
        if(min->nodesum>p->nodesum)
            min=p;
    }

    //cover(min);//start cover from min col
    min->left->right=min->right;
    min->right->left=min->left;
    for(p=min->down; p!=min; p=p->down) {
        for(q=p->right; q!=p; q=q->right) {
            q->up->down=q->down;
            q->down->up=q->up;
            q->root->nodesum--;
        }
    }
    //
    for(p=min->down; p!=min; p=p->down) {
        result[k]=p->row;
        for(q=p->right; q!=p; q=q->right) {
            //cover q->root
            t=q->root;
            t->left->right=t->right;
            t->right->left=t->left;
            for(r=t->down; r!=t; r=r->down) {
                for(s=r->right; s!=r; s=s->right) {
                    s->up->down=s->down;
                    s->down->up=s->up;
                    s->root->nodesum--;
                }
            }
            //
        }
        if(dance(k+1)) {
            return true;
        }
        for(q=p->left; q!=p; q=q->left) {
            //uncover q->root
            t=q->root;
            for(r=t->up; r!=t; r=r->up) {
                for(s=r->left; s!=r; s=s->left) {
                    s->up->down=s;
                    s->down->up=s;
                    s->root->nodesum++;//the number of this col increases
                }
            }
            t->left->right=t;
            t->right->left=t;
            //
        }
    }
    //uncover(min);
    for(p=min->up; p!=min; p=p->up) {
        for(q=p->left; q!=p; q=q->left) {
            q->up->down=q;
            q->down->up=q;
            q->root->nodesum++;//the number of this col increases
        }
    }
    min->left->right=min;
    min->right->left=min;
    return false;
}

void Sudoku::dlx() {
    int i,j;
    setup();
    int index,num;
    for(i=0; i<DLX_R; i++) {
        index=i/9;
        num=i%9;
        if(map[index]-1==num) { //add question first
            //if map[index] has been inserted number then add 4 corresponds in DLX board.
            addnode(i,index);
            addnode(i,81+(index/9)*9+num);
            addnode(i,162+(index%9)*9+num);
            addnode(i,243+((index/27)*3+(index%9)/3)*9+num);
            col[i].questionnum++;//record the number of question in each col
        }
    }
    for(i=0; i<DLX_R; i++) {
        index=i/9;
        num=i%9;
        //cout<<col[i].questionnum<<endl;
        if(map[index]==0&&col[i].questionnum==0) {
            addnode(i,index);
            addnode(i,81+(index/9)*9+num);
            addnode(i,162+(index%9)*9+num);
            addnode(i,243+((index/27)*3+(index%9)/3)*9+num);
        }
    }
    solvenum=0;
    dance(0);
    switch(solvenum) {
    case 0:
        cout<<0;
        break;
    case 1:
        cout<<1<<endl;
        print_sol();
        break;
    case 2:
        cout<<2;
        break;
    }
}

