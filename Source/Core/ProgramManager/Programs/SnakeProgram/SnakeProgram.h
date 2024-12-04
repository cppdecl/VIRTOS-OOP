#ifndef SNAKE_PROGRAM_HH
#define SNAKE_PROGRAM_HH

#include "../../Program.h"
#include <windows.h>
#include <vector>
#include <string>

class SnakeProgram : public Program 
{
public:
    SnakeProgram();
    ~SnakeProgram();

    void Run(std::vector<std::string> args) override;

private:
    void GameInit();
    void GameRender(const std::string& playerName);
    void UpdateGame();
    void UserInput();
    int SetDifficulty();

private:
    static const int m_Width = 80;
    static const int m_Height = 20;

    int m_X, m_Y; // Snake head position
    int m_FruitCordX, m_FruitCordY; // Fruit position
    int m_PlayerScore;
    int m_SnakeTailX[100], m_SnakeTailY[100]; // Snake tail positions
    int m_SnakeTailLen; // Snake tail length

    enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    snakesDirection m_SDir;
    bool m_IsGameOver;
    int m_Difficulty;
};

#endif // SNAKE_PROGRAM_HH