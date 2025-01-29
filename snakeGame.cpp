#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>
#include "CRUD.hpp"

bool gameOver;
const int width = 40;
const int height = 15;
const int maxLength = width * height; // Maximum possible length of the snake
int tailX[maxLength];
int tailY[maxLength];
int x, y, fruitX, fruitY, score, highScore, fruit;
int nTail, fruitEatenX, fruitEatenY;
int speed = 120;
std::string filePath;
bool fruitEaten;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;
CRUD crud;

void Setup()
{
    srand(time(0));
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    filePath = crud.getDocumentsPath();
    if (!crud.fileExists(filePath))
    {
        crud.createFile(filePath, score);
    }
}

void SetCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, coord);
}

void MaximizeConsoleWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);
}

void SetConsoleFontSize(int fontSize)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = fontSize;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Draw()
{
    SetCursorPosition(0, 0);
    std::cout << "USE WASD TO MOVE THE SNAKE" << std::endl;
    for (int i = 0; i < width + 2; i++)
        std::cout << "H";
    std::cout << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                std::cout << "H";
            if (i == y && j == x)
                std::cout << "X";
            else if (i == fruitY && j == fruitX)
            {
                if (score != 0 && score % 8 == 0)
                {
                    fruit = 2;
                    std::cout << fruit;
                }
                else
                {
                    fruit = 1;
                    std::cout << fruit;
                }
            }
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        std::cout << "x";
                        print = true;
                    }
                }
                if (!print)
                    std::cout << " ";
            }

            if (j == width - 1)
                std::cout << "H";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; i++)
        std::cout << "H";
    std::cout << std::endl;
    std::cout << "Score: " << score << std::endl;
    highScore = crud.readFileAndExtractIntegers(filePath);
    if (highScore <= score)
    {
        highScore = score;
    }

    std::cout << "HighScore: " << highScore << std::endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 'p': // Pause the game
            std::cout << "\nGame Paused. Press 'p' to resume...\n";
            while (true)
            {
                if (_kbhit() && _getch() == 'p')
                {
                    std::cout << "Resuming...\n";
                    break;
                }
            }
            break;
        }
    }
}

bool isFruitOnSnake(int fx, int fy)
{
    if (fx == x && fy == y) // Check if fruit is on the head
        return true;

    for (int i = 0; i < nTail; i++)
    {
        if (fx == tailX[i] && fy == tailY[i]) // Check if fruit is on the tail
            return true;
    }
    return false;
}

void spawnFruit()
{
    do
    {
        fruitX = rand() % width;
        fruitY = rand() % height;
    } while (isFruitOnSnake(fruitX, fruitY)); // Keep generating until it's not on the snake
}

void Logic(int walls)
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (walls == 1)
    {
        if (x >= width || x < 0 || y >= height || y < 0)
        {
            gameOver = true;
        }
    }
    else if (walls == 0)
    {
        if (x >= width)
            x = 0;
        else if (x < 0)
            x = width - 1;
        if (y >= height)
            y = 0;
        else if (y < 0)
            y = height - 1;
    }

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY)
    {
        if (fruit == 1)
            score += 1;
        else if (fruit == 2)
            score += 2;

        fruitEatenX = fruitX;
        fruitEatenY = fruitY;
        spawnFruit();
        fruitEaten = true;

        // Increase speed every time the snake eats 2 fruits, with a limit of 80 ms
        if (score % 2 == 0 && speed > 80)
            speed--;
    }

    std::cout << std::endl;

    if (fruitEaten && ((nTail > 0 && (tailX[nTail - 1] == fruitEatenX) && (tailY[nTail - 1] == fruitEatenY)) || nTail == 0))
    {
        nTail++;
        fruitEaten = false;
    }
}

void Wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void ContinueGame()
{
    crud.deleteFile(filePath);
    crud.createFile(filePath, highScore);
    std::cout << "Game is OVER" << std::endl;
    char choice;
    Wait(1000);
    do
    {
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> choice;

        if ((choice == 'N') || (choice == 'n'))
        {
            std::cout << "Thank you for playing" << std::endl;
            gameOver = true;
        }
        else if ((choice == 'Y') || (choice == 'y'))
        {
            std::cout << "The game is starting" << std::endl;
            Wait(2000);
            system("cls");
            Setup();
        }
        else
        {
            std::cout << "Invalid input. Please try again.\n"
                      << std::endl;
        }
    } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
}

int Walls()
{
    int walls;

    std::cout << "Play with NO WALLS. Press 0" << std::endl;
    std::cout << "Play with WALLS. Press 1" << std::endl;

    while (true)
    {
        std::cin >> walls;
        if (walls == 0)
        {
            std::cout << "Play with NO WALLS\n"
                      << std::endl;
            break;
        }
        else if (walls == 1)
        {
            std::cout << "Play with WALLS\n"
                      << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid input. Please try again.\n"
                      << std::endl;
        }
    }
    return walls;
}

int main()
{
    MaximizeConsoleWindow();
    SetConsoleFontSize(24);

    int walls = Walls();
    system("cls");
    Setup();
    while (true)
    {
        while (!gameOver)
        {
            Draw();
            Input();
            Logic(walls);
            Wait(speed);
        }
        ContinueGame();
        if (gameOver)
            break;
    }

    std::cin.get();
    std::cin.get();
}