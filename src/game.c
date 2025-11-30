#include "game.h"

void print_welcome() {
    printf("\n=== Random Number Guessing Game ===\n");
    printf("I have chosen a number between 1 and 100.\n");
    printf("Try to guess it in as few attempts as possible!\n\n");
}

void print_hint(int guess, int secret) {
    if (guess > secret)
        printf("Too high! Try a smaller number.\n");
    else if (guess < secret)
        printf("Too low! Try a larger number.\n");
}

int read_guess() {
    int guess;
    printf("Enter your guess: ");
    int ret = scanf("%d", &guess);
    
    if (ret != 1 || guess < 1 || guess > 100) {
        printf("Invalid input! Please enter a number between 1 and 100.\n");
        while (getchar() != '\n');   // clear input buffer
        return -1;                   // invalid
    }
    return guess;
}

void save_score(const char *name, int attempts) {
    FILE *fp = fopen("highscores.txt", "a");
    if (fp) {
        fprintf(fp, "%s %d\n", name, attempts);
        fclose(fp);
    }
}

void load_and_show_leaderboard() {
    Score scores[MAX_SCORES];
    int count = 0;
    
    FILE *fp = fopen("highscores.txt", "r");
    if (!fp) {
        printf("No high scores yet. Be the first!\n\n");
        return;
    }
    
    while (count < MAX_SCORES && fscanf(fp, "%49s %d", scores[count].name, &scores[count].attempts) == 2)
        count++;
    fclose(fp);
    
    // Simple bubble sort by attempts (ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j].attempts > scores[j + 1].attempts) {
                Score temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
    
    printf("============ LEADERBOARD ============\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%d. %s → %d attempt(s)\n", i + 1, scores[i].name, scores[i].attempts);
    }
    if (count == 0) printf("No scores recorded yet.\n");
    printf("====================================\n\n");
}

void show_guess_history(int *history, int size) {
    printf("\nYour guessing history: ");
    for (int i = 0; i < size; i++)
        printf("%d ", history[i]);
    printf("\n");
}

void play_game() {
    srand(time(NULL));
    int secret = rand() % 100 + 1;
    int *history = NULL;
    int capacity = 0, attempts = 0;
    int guess;
    
    print_welcome();
    
    while (1) {
        guess = read_guess();
        if (guess == -1) continue;           // invalid input, don't count
        
        attempts++;
        
        // Dynamic array to store history
        if (attempts > capacity) {
            capacity = capacity == 0 ? 8 : capacity * 2;
            history = (int *)realloc(history, capacity * sizeof(int));
        }
        history[attempts - 1] = guess;
        
        if (guess == secret) {
            printf("\nCongratulations! You guessed it in %d attempt(s)!\n", attempts);
            show_guess_history(history, attempts);
            break;
        } else {
            print_hint(guess, secret);
        }
    }
    
    // Get player name safely
    char name[MAX_NAME];
    printf("\nEnter your name for the leaderboard: ");
    scanf("%39s", name);          // prevent buffer overflow
    
    save_score(name, attempts);
    free(history);
}
