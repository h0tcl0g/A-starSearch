#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 3
#define MAX_DEPTH 16
#define EXEC_TIMES 100

typedef struct State
{
    int board[N][N];
    int empty_row;
    int empty_col;
    struct State *parent;
} State;

int goal[N][N] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};

// １辺Nの正方行列を表示*
void disp_array(int *array)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", array[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// min_valからmax_valの範囲で整数の乱数を返す関数*
int get_rand(int min_val, int max_val)
{
    srand((unsigned int)time(NULL));
    return (int)((rand() % (max_val + 1 - min_val)) + min_val);
}

// 要素数sizeの配列arrayの要素をシャッフルする関数*
void shuffle(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        int r = get_rand(i, size - 1);
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
    }
}

// 2次元配列の要素をシャッフルする関数*
void shuffle_state(int *array_2d, int size)
{
    int size_1d = size * size;
    int array_1d[size_1d];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array_1d[i * size + j] = array_2d[i * size + j];
        }
    }

    shuffle(array_1d, size_1d);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array_2d[i * size + j] = array_1d[i * size + j];
        }
    }
}

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
int iterative_deepening_search(State *initial_state)
{
    for (int depth = 0; depth <= MAX_DEPTH; ++depth)
    {
        printf("depth = %d\n", depth);

        if (depth_limited_search(initial_state, depth))
        {
            printf("Solution found at depth %d\n", depth);
            return 0;
        }
    }
    printf("No solution found up to depth %d\n", MAX_DEPTH);
    return -1;
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

// 初期状態のシャッフル*
void shuffle_initial_board(int *initial_board)
{
    printf("initial board:\n");
    disp_array(initial_board);
    shuffle_state(initial_board, N);
    printf("Shuffled initial state:\n");
    disp_array(initial_board);
}

// ゴールのシャッフル*
void shuffle_goal(int *goal_board)
{
    printf("initial goal board:\n");
    disp_array(goal_board);
    shuffle_state(goal_board, N);
    printf("Shuffled goal state:\n");
    disp_array(goal_board);
}

int main()
{
    clock_t start_clock, end_clock;
    double time;
    FILE *fp;
    int isSearched;

    fp = fopen("exec_time.csv", "w");
    fprintf(fp, "exec_times,time\n");

    for (int i = 0; i < EXEC_TIMES; i++)
    {
        int initial_board[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
        State initial_state;

        shuffle_initial_board(initial_board);
        shuffle_goal(goal);

        printf("----- %d -----\n", i);

        start_clock = clock();

        initialize_state(&initial_state, initial_board);

        isSearched = iterative_deepening_search(&initial_state);

        end_clock = clock();

        time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
        printf("time = %lf\n", time);

        if (isSearched == 0)
        {
            fprintf(fp, "%d,%lf\n", i, time);
        }
        else
        {
            fprintf(fp, "%d,failed\n", i);
        }
        sleep(1); // 乱数のシードにtime()を用いているためある程度待たないとランダムな条件にならない
    }
    fclose(fp);
    return 0;
}
