#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 3
#define MAX_STATES 10000
#define EXEC_TIMES 100

typedef struct
{
    int board[N][N];
    int g; // cost from start to current state
    int h; // heuristic cost to goal
    int f; // g + h
    struct State *parent;
} State;

typedef struct
{
    State *states[MAX_STATES];
    int size;
} PriorityQueue;

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

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int heuristic(State *state)
{
    int misplaced = 0;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (state->board[i][j] != 0 && state->board[i][j] != goal[i][j])
            {
                misplaced++;
            }
        }
    }
    return misplaced;
}

void initialize_state(State *state, int board[N][N], int g, State *parent)
{
    memcpy(state->board, board, sizeof(state->board));
    state->g = g;
    state->h = heuristic(state);
    state->f = state->g + state->h;
    state->parent = parent;
}

void push(PriorityQueue *pq, State *state)
{
    pq->states[pq->size++] = state;
}

State *pop(PriorityQueue *pq)
{
    int minIndex = 0;
    for (int i = 1; i < pq->size; ++i)
    {
        if (pq->states[i]->f < pq->states[minIndex]->f)
        {
            minIndex = i;
        }
    }
    State *minState = pq->states[minIndex];
    pq->states[minIndex] = pq->states[--pq->size];
    return minState;
}

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

int is_same_state(State *a, State *b)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (a->board[i][j] != b->board[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

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

void get_neighbors(State *state, State neighbors[], int *num_neighbors)
{
    int row, col;
    *num_neighbors = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (state->board[i][j] == 0)
            {
                row = i;
                col = j;
            }
        }
    }

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; ++i)
    {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N)
        {
            State neighbor;
            memcpy(&neighbor, state, sizeof(State));
            swap(&neighbor.board[row][col], &neighbor.board[newRow][newCol]);
            neighbor.g = state->g + 1;
            neighbor.h = heuristic(&neighbor);
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.parent = state;
            neighbors[(*num_neighbors)++] = neighbor;
        }
    }
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

    fp = fopen("exec_time.csv", "w");
    fprintf(fp, "exec_times,time\n");

    for (int exe = 0; exe < EXEC_TIMES; exe++)
    {
        int initial_board[N][N] = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
        PriorityQueue openList;
        openList.size = 0;

        int flag = 0;

        shuffle_initial_board(initial_board);
        // shuffle_goal(goal);

        printf("----- %d -----\n", exe);

        start_clock = clock();

        State *initial_state = (State *)malloc(sizeof(State));
        initialize_state(initial_state, initial_board, 0, NULL);
        push(&openList, initial_state);

        while (openList.size > 0)
        {
            // printf("%d ", openList.size);
            State *current = pop(&openList);

            if (openList.size > MAX_STATES - 1000)
            {
                flag = 1;
                fprintf(fp, "%d,failed\n", exe);
                break;
            }

            if (is_goal(current))
            {
                print_solution(current);
                break;
            }

            State neighbors[4];
            int num_neighbors;
            get_neighbors(current, neighbors, &num_neighbors);

            for (int i = 0; i < num_neighbors; ++i)
            {
                int found_in_open = 0;
                for (int j = 0; j < openList.size; ++j)
                {
                    if (is_same_state(openList.states[j], &neighbors[i]) && openList.states[j]->g <= neighbors[i].g)
                    {
                        found_in_open = 1;
                        break;
                    }
                }
                if (!found_in_open)
                {
                    State *new_state = (State *)malloc(sizeof(State));
                    memcpy(new_state, &neighbors[i], sizeof(State));
                    push(&openList, new_state);
                }
            }
        }

        end_clock = clock();
        time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
        if (flag == 1)
        {
            continue;
        }
        printf("time = %lf\n", time);
        fprintf(fp, "%d,%lf\n", exe, time);
        sleep(1);
    }

    fclose(fp);
    return 0;
}
