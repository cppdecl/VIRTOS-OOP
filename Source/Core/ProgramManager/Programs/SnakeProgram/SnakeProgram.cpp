#include "SnakeProgram.h"
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include "Utils/Clock.hh"
#include "Utils/Console.hh"
#include "Utils/Random.hh"

SnakeProgram::SnakeProgram()
{
    srand(static_cast<unsigned>(time(0)));
}

SnakeProgram::~SnakeProgram()
{
}

void SnakeProgram::Run(std::vector<std::string> args)
{
    UserAccount* user = System::GetInstance().GetUserAccountControl()->GetCurrentUser();
    if (!user)
        return;

    m_Difficulty = SetDifficulty();

    Thread::Sleep(2000);

    GameInit();

    TimePoint lastRender = Clock::Now();
    TimePoint lastUpdate = Clock::Now();
    while (!m_IsGameOver)
    {
        if (Clock::DiffMilli(lastRender) > 150)
        {
            lastRender = Clock::Now();
            GameRender(user->GetUsername());
        }

        if (Clock::DiffMilli(lastUpdate) > m_Difficulty)
        {    
            lastUpdate = Clock::Now();
            UserInput();
            UpdateGame();
        }
    }

    Thread::Sleep(500);

    system("cls");

    std::cout << "\nGame Over! Final Score: " << m_PlayerScore << std::endl;
    std::cout << "Press Escape to exit or Enter to restart.\n";

    while (true)
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == 27)
                break;
            else if (key == 13)
            {
                Run(args);
                break;
            }
        }
    }
}

void SnakeProgram::GameInit()
{
    m_IsGameOver = false;
    m_SDir = STOP;
    m_X = m_Width / 2;
    m_Y = m_Height / 2;
    m_FruitCordX = rand() % m_Width;
    m_FruitCordY = rand() % m_Height;
    m_PlayerScore = 0;
    m_SnakeTailLen = 0;
}

void SnakeProgram::GameRender(const std::string& playerName)
{
    system("cls");

    for (int i = 0; i < m_Width + 2; i++)
        std::cout << "-";
    std::cout << std::endl;

    for (int i = 0; i < m_Height; i++)
    {
        for (int j = 0; j <= m_Width; j++)
        {
            if (j == 0 || j == m_Width)
                std::cout << "|"; 
            else if (i == m_Y && j == m_X)
                std::cout << "O";
            else if (i == m_FruitCordY && j == m_FruitCordX)
                std::cout << "#"; 
            else
            {
                bool isTail = false;
                for (int k = 0; k < m_SnakeTailLen; k++)
                {
                    if (m_SnakeTailX[k] == j && m_SnakeTailY[k] == i)
                    {
                        std::cout << "o";
                        isTail = true;
                        break;
                    }
                }
                if (!isTail)
                    std::cout << " ";
            }
        }
        std::cout << std::endl;
    }


    for (int i = 0; i < m_Width + 2; i++)
        std::cout << "-";
    std::cout << std::endl;

    std::cout << playerName << "'s Score: " << m_PlayerScore << std::endl;

    COORD pos = {0, 0};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void SnakeProgram::UpdateGame()
{
    int prevX = m_SnakeTailX[0];
    int prevY = m_SnakeTailY[0];
    int prev2X, prev2Y;

    m_SnakeTailX[0] = m_X;
    m_SnakeTailY[0] = m_Y;

    for (int i = 1; i < m_SnakeTailLen; i++)
    {
        prev2X = m_SnakeTailX[i];
        prev2Y = m_SnakeTailY[i];
        m_SnakeTailX[i] = prevX;
        m_SnakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    int lastX = m_X;
    int lastY = m_Y;

    switch (m_SDir)
    {
    case LEFT:
        m_X -= Random::Gen(1, 2);
        break;
    case RIGHT:
        m_X += Random::Gen(1, 2);
        break;
    case UP:
        m_Y -= 1;
        break;
    case DOWN:
        m_Y += 1;
        break;
    }

    if (m_X >= m_Width || m_X < 0 || m_Y >= m_Height || m_Y < 0)
        m_IsGameOver = true;

    for (int i = 0; i < m_SnakeTailLen; i++)
    {
        if (m_SnakeTailX[i] == m_X && m_SnakeTailY[i] == m_Y)
            m_IsGameOver = true;
    }

    if ((m_X == m_FruitCordX && m_Y == m_FruitCordY) || ((m_SDir == LEFT && m_X == m_FruitCordX + 1 && m_Y == m_FruitCordY) || (m_SDir == RIGHT && m_X == m_FruitCordX - 1 && m_Y == m_FruitCordY)))
    {
        m_PlayerScore += 10;
        m_FruitCordX = rand() % m_Width;
        m_FruitCordY = rand() % m_Height;
        m_SnakeTailLen++;
    }
}

void SnakeProgram::UserInput()
{
    if (_kbhit())
    {
        char key = _getch();
        switch (key)
        {
        case 'a':
            m_SDir = LEFT;
            break;
        case 'd':
            m_SDir = RIGHT;
            break;
        case 'w':
            m_SDir = UP;
            break;
        case 's':
            m_SDir = DOWN;
            break;
        case 27: // Escape key
            m_IsGameOver = true;
            break;
        }
    }
}

int SnakeProgram::SetDifficulty()
{
    std::cout << "Set Difficulty\n1: Easy\n2: Medium\n3: Hard\nChoose: ";
    char choice;
    std::cin >> choice;
    switch (choice)
    {
    case '1':
        return 150;
    case '2':
        return 100;
    case '3':
        return 50;
    default:
        std::cout << "Invalid choice. Setting to Medium (default).\n";
        return 100;    
    }
}
