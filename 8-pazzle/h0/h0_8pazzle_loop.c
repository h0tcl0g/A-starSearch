#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 3
#define MAX_STATES 1000000

typedef struct
{
    int board[N][N];
    int g; // cost from start to current state
    int h; // heuristic cost to goal (always 0)
    int f; // g + h
    struct State *parent;
} State;

typedef struct
{
    State *states[MAX_STATES];
    int size;
} PriorityQueue;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void initialize_state(State *state, int board[N][N], int g, State *parent)
{
    memcpy(state->board, board, sizeof(state->board));
    state->g = g;
    state->h = 0; // heuristic is always 0
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
    int goal[N][N] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};
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
            neighbor.h = 0; // heuristic is always 0
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.parent = state;
            neighbors[(*num_neighbors)++] = neighbor;
        }
    }
}

int main()
{
    clock_t start_clock, end_clock;

    int initial_board[N][N] = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
    PriorityQueue openList;
    openList.size = 0;

    start_clock = clock();

    State *initial_state = (State *)malloc(sizeof(State));
    initialize_state(initial_state, initial_board, 0, NULL);
    push(&openList, initial_state);

    while (openList.size > 0)
    {
        State *current = pop(&openList);

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

    printf("time = %lf\n", (double)(end_clock - start_clock) / CLOCKS_PER_SEC);

    return 0;
}
