#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;
pthread_t* threads = NULL;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

typedef struct {
    char* src;
    char* dst;
    int field;
} ThreadArgs;

void* update_field(void* _args) {
    ThreadArgs* args = (ThreadArgs*) _args;
    while (1) {
        args->dst[args->field] = is_alive(args->field / grid_width, args->field % grid_width, args->src);

        pause();

        char* tmp = args->src;
        args->src = args->dst;
        args->dst = tmp;
    }
}

void sigusr1_handler(int signo, siginfo_t* info, void* context) {}

void init_threads(char* src, char* dst) {

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = sigusr1_handler;
    sigaction(SIGUSR1, &action, NULL);

    threads = malloc(grid_height * grid_width * sizeof(pthread_t));
    for (int i=0; i < grid_height * grid_width; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->src = src;
        args->dst = dst;
        args->field = i;
        pthread_create(&threads[i], NULL, update_field, args);
    }
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_width * grid_height; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }
}