#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 15

int playerPos;
int balloonX, balloonY;
int bulletX = -1, bulletY = -1;
int score;
int lives;

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void clear_screen_fast() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(h, pos);
}

void resetBalloon() {
    balloonX = rand() % WIDTH;
    balloonY = 0;
}

void draw() {
    clear_screen_fast();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            if (x == balloonX && y == balloonY)
                printf("O");                  // BALLOON

            else if (x == bulletX && y == bulletY)
                printf("|");                  // BULLET

            else if (y == HEIGHT - 1 && x == playerPos)
                printf("^");                  // PLAYER PIN

            else
                printf(" ");
        }
        printf("\n");
    }

    printf("\nScore: %d   Lives: %d\n", score, lives);
    printf("Controls: LEFT/RIGHT arrows = Move | SPACE = Shoot\n");
}

void input() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 0 || key == -32)
            key = _getch();

        if (key == 75 && playerPos > 0)
            playerPos--;                       // LEFT

        if (key == 77 && playerPos < WIDTH - 1)
            playerPos++;                       // RIGHT

        if (key == ' ' && bulletY == -1) {
            bulletX = playerPos;
            bulletY = HEIGHT - 2;              // Starts above player
        }
    }
}

void logic() {
    // Move bullet
    if (bulletY != -1)
        bulletY--;

    // Balloon hit by bullet
    if (bulletX == balloonX && bulletY == balloonY) {
        score++;
        bulletY = -1;
        resetBalloon();
    }

    // Balloon falling speed control
    static int counter = 0;
    counter++;
    if (counter >= 3) {
        balloonY++;
        counter = 0;
    }

    // Balloon reached bottom
    if (balloonY >= HEIGHT - 1) {
        lives--;
        resetBalloon();
    }

    // Bullet outside screen
    if (bulletY < 0)
        bulletY = -1;
}

int main() {
    srand(time(0));
    hideCursor();

    playerPos = WIDTH / 2;
    score = 0;
    lives = 3;

    resetBalloon();

    printf("🎈 Balloon Shooter Game 🎈\n");
    printf("Press any key to start...\n");
    _getch();

    while (lives > 0) {
        draw();
        input();
        logic();
        Sleep(100);
    }

    clear_screen_fast();
    printf("\n💀 GAME OVER 💀\nFinal Score: %d\n", score);

    return 0;
}
