#include <stdio.h>
#include <string.h>

#define N 3 // 盤面１辺の目の数
typedef struct node *link;

struct node
{
    int state[3][3]; // 盤面の状態を表す２次元配列
    int space;       // 空白位置のインデックス

    link next[4]; // 子ノードのポインタを格納する配列
};

char board[N][N]; // 盤面

link NEW(int *state, int space, link *next)
{
    link x = malloc(sizeof *x);
    x->space = space;
}

void init()
{
}

int main(int argc, char const *argv[])
{
    char initial[N][N] = {{3, 2, 1}, {4, 5, 6}, {7, 8, 0}};
    char goal[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    // memcpy(board, initial, N);

    init();

    return 0;
}
