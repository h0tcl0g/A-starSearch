// 8パズルの状態をA=[1,2,3,4,5,6,7,8,0]のようなベクトルで表す。
// これはパズルの状態が[[1,2,3],[4,5,6],[7,8,*]]となっていることを意味する。

// 各ノードのメンバは
// a. パズルの状態を表すベクトルAのアドレス
// b. h(n)の値(A*の場合)
// c. 次ノードのアドレスs,t,u (左から優先的に使う。ない場合はnull)

//  - 1. 初期状態を入力
//  - 2. 初期状態を出力
//  - 3.

#include <stdio.h>
typedef struct node *link;
// typedef int *state;
static link head;

struct node
{
    int A[3][3];
    int h;

    link s;
    link t;
    link u;
};

link NEW(int h, int dh)
{
    link x = malloc(sizeof *x);
    x->A = A;
    x->h = h + dh; // dhはヒューリスティック関数hの増分
    x->s = NULL;
    x->t = NULL;
    x->u = NULL;

    return x;
}

void init()
{
    head = NEW(, , );
}

int main()
{
    init();
}