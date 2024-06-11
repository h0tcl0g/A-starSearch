#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 3
#define MAX_DEPTH 50

typedef struct State
{
    int board[N][N];
    int empty_row;
    int empty_col;
    struct State *parent;
} State;

int goal[N][N] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};

// 目標状態かどうかをチェックする関数
int is_goal(State *state)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (state->board[i][j] != goal[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

// 状態をコピーする関数
void copy_state(State *dst, State *src)
{
    memcpy(dst->board, src->board, sizeof(src->board));
    dst->empty_row = src->empty_row;
    dst->empty_col = src->empty_col;
    dst->parent = src->parent;
}

// 隣接する状態を生成する関数
void get_neighbors(State *state, State neighbors[], int *num_neighbors)
{
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    *num_neighbors = 0;
    for (int i = 0; i < 4; ++i)
    {
        int newRow = state->empty_row + directions[i][0];
        int newCol = state->empty_col + directions[i][1];
        if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N)
        {
            State neighbor;
            copy_state(&neighbor, state);
            neighbor.board[state->empty_row][state->empty_col] = neighbor.board[newRow][newCol];
            neighbor.board[newRow][newCol] = 0;
            neighbor.empty_row = newRow;
            neighbor.empty_col = newCol;
            neighbor.parent = state;
            neighbors[(*num_neighbors)++] = neighbor;
        }
    }
}

// 深さ制限付き探索
int depth_limited_search(State *state, int depth)
{
    if (is_goal(state))
    {
        return 1;
    }
    if (depth == 0)
    {
        return 0;
    }

    State neighbors[4];
    int num_neighbors;
    get_neighbors(state, neighbors, &num_neighbors);

    for (int i = 0; i < num_neighbors; ++i)
    {
        if (depth_limited_search(&neighbors[i], depth - 1))
        {
            return 1;
        }
    }
    return 0;
}

// 反復深化探索
void iterative_deepening_search(State *initial_state)
{
    for (int depth = 0; depth <= MAX_DEPTH; ++depth)
    {
        if (depth_limited_search(initial_state, depth))
        {
            printf("Solution found at depth %d\n", depth);
            return;
        }
    }
    printf("No solution found up to depth %d\n", MAX_DEPTH);
}

// 初期状態を設定する関数
void initialize_state(State *state, int board[N][N])
{
    memcpy(state->board, board, sizeof(state->board));
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (state->board[i][j] == 0)
            {
                state->empty_row = i;
                state->empty_col = j;
                break;
            }
        }
    }
    state->parent = NULL;
}

// 解を表示する関数
void print_solution(State *state)
{
    if (state->parent != NULL)
    {
        print_solution(state->parent);
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%d ", state->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    clock_t start_clock, end_clock;

    int initial_board[N][N] = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
    State initial_state;

    start_clock = clock();

    initialize_state(&initial_state, initial_board);

    iterative_deepening_search(&initial_state);

    print_solution(&initial_state);

    end_clock = clock();

    printf("time = %lf\n", (double)(end_clock - start_clock) / CLOCKS_PER_SEC);

    return 0;
}
