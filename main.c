#include "raylib.h"
#include <time.h>
#include <stdio.h>
#include <stdio.h>

#define ARRAY_SIZE 300
#define FONT_SIZE 20
#define LINE_HEIGHT (FONT_SIZE + 2)

const int screenWidth = 1920;
const int screenHeight = 1080;
const int panelWidth = 400;

int array[ARRAY_SIZE];
int currentAlgorithm = 0; // 0 for BubbleSort, 1 for InsertionSort
double speed = 1.0;
bool isSorting = false;
clock_t startTime;
double elapsedTime = 0;
int currentHighlightLine = -1;  // For highlighting the current line being executed

void InitializeArray() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = (int)((float)i / ARRAY_SIZE * screenHeight);
    }
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int j = GetRandomValue(0, ARRAY_SIZE - 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void BubbleSortStep() {
    static int i = 0, j = 0;

    currentHighlightLine = 1;
    if (i < ARRAY_SIZE - 1) {
        currentHighlightLine = 2;
        if (j < ARRAY_SIZE - i - 1) {
            currentHighlightLine = 3;
            if (array[j] > array[j+1]) {
                currentHighlightLine = 4;
                swap(&array[j], &array[j+1]);
            }
            j++;
        } else {
            j = 0;
            i++;
        }
    } else {
        isSorting = false;
        elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;
        i = 0; j = 0;
        currentHighlightLine = -1;
    }
}

void InsertionSortStep() {
    static int i = 1, j = 1;

    currentHighlightLine = 1;
    if (i < ARRAY_SIZE) {
        int key = array[i];
        currentHighlightLine = 2;
        if (j > 0 && array[j-1] > key) {
            currentHighlightLine = 4;
            array[j] = array[j-1];
            j--;
        } else {
            currentHighlightLine = 7;
            array[j] = key;
            i++;
            j = i;
        }
    } else {
        isSorting = false;
        elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;
        i = 1; j = 1;
        currentHighlightLine = -1;
    }
}

void DrawArray() {
    float barWidth = (float)(screenWidth - panelWidth) / ARRAY_SIZE;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        Color barColor = (Color){i * 255 / ARRAY_SIZE, 255 - i * 255 / ARRAY_SIZE, 50, 255};
        DrawRectangle(i * barWidth, screenHeight - array[i], barWidth, array[i], barColor);
    }
}

void DrawCodePanel() {
    char *sortingAlgorithms[2][10] = {
        {
            "BubbleSort:",
            "for i = 0 to n-1:",
            "  for j = 0 to n-i-1:",
            "    if arr[j] > arr[j+1]:",
            "      swap(arr[j], arr[j+1])",
            NULL
        },
        {
            "InsertionSort:",
            "for i = 1 to n:",
            "  key = arr[i]",
            "  j = i - 1",
            "  while j >= 0 and arr[j] > key:",
            "    arr[j + 1] = arr[j]",
            "    j = j - 1",
            "  arr[j + 1] = key",
            NULL
        }
    };

    int x = screenWidth - panelWidth + 10;
    int y = 50;

    DrawRectangle(screenWidth - panelWidth, 0, panelWidth, screenHeight, (Color){0x1A, 0x1A, 0x1A, 0xFF});

    for (int i = 0; sortingAlgorithms[currentAlgorithm][i] != NULL; i++) {
        if (i == currentHighlightLine) {
            DrawRectangle(screenWidth - panelWidth, y - 2, panelWidth, LINE_HEIGHT, (Color){50, 50, 150, 100});
        }
        DrawText(sortingAlgorithms[currentAlgorithm][i], x, y, FONT_SIZE, WHITE);
        y += LINE_HEIGHT;
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Sorting Algorithm Visualizer");
    InitializeArray();
    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE) && !isSorting) {
            isSorting = true;
            startTime = clock();
        }

        if (isSorting) {
            if (currentAlgorithm == 0) {
                for (int k = 0; k < speed * 10; k++) BubbleSortStep();
            } else {
                for (int k = 0; k < speed * 10; k++) InsertionSortStep();
            }
        }

        if (IsKeyPressed(KEY_N)) {
            currentAlgorithm = (currentAlgorithm + 1) % 2;
        }

        if (IsKeyPressed(KEY_R)) {
            InitializeArray();
        }

        int wheelMove = GetMouseWheelMove();
        if (wheelMove > 0) {
            speed += 0.1;
        } else if (wheelMove < 0) {
            speed -= 0.1;
        }

        BeginDrawing();
        ClearBackground((Color){0x0B, 0x0B, 0x0B, 0xFF});
        DrawArray();
        DrawCodePanel();

        char timeStr[50];
        sprintf(timeStr, "Elapsed Time: %.3f seconds", elapsedTime);
        DrawText(timeStr, 10, 10, FONT_SIZE, WHITE);

        char speedStr[50];
        sprintf(speedStr, "Speed: x%.1f", speed);
        DrawText(speedStr, screenWidth - panelWidth + 10, 10, FONT_SIZE, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
