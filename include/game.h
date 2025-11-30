#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_NAME 40
#define MAX_SCORES 100

typedef struct {
    char name[MAX_NAME];
    int attempts;
} Score;

void print_welcome();
void print_hint(int guess, int secret);
int read_guess();
void save_score(const char *name, int attempts);
void load_and_show_leaderboard();
void show_guess_history(int *history, int size);
void play_game();

#endif
