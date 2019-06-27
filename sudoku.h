#ifndef SUDOKU_H
#define SUDOKU_H
#define SDKSIZ 81
#define DLX_R 9*SDKSIZ
#define DLX_C 4*SDKSIZ

class Sudoku {
  public:
    void gen();
    void print_sdk();
    void put();
    void change_num(int n1, int n2);
    void change_row(int n1, int n2);
    void change_col(int n1, int n2);
    void rotate(int t);
    void flip(int opt);
    void dlx();
  private:
    struct node {
        node *up;
        node *down;
        node *left;
        node *right;
        node *root;     //the head of each col
        int row;
        int nodesum;    //the sum of each col
        int questionnum;
    };
    node *nodes,*row,*col,*head;
    int rownum,colnum,nodecount;
    int *result,resultcount;
    void setup();
    bool dance(int k);
    void addnode(int r,int c);
    void print_sol();

    int map[SDKSIZ],solvemap[SDKSIZ];
    int zeronum;
    int solvenum;
};

#endif // SUDOKU_H
