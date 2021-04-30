//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GAME_HPP
#define MARTRIXGOC_GAME_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"


const int BOARD_SIZE = 19;

class Game
{
public:
    int player = BLACK_PLAYER;
    int boardSize = BOARD_SIZE;
    vector_2d(Point*) allBoardPoints;
    std::vector<Step*> steps = std::vector<Step*>();
    vector_2d(int) board = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    long long boardZobristHash = 0;
    std::vector<vector_2d(int)> historyBoard = std::vector<vector_2d(int)>();
    std::unordered_set<long long> historyZobristHash = std::unordered_set<long long>();
    vector_2d(int) newBoard = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    long long newBoardZobristHash = 0;
    Step* nextStep = nullptr;
    GoBlock* targetBlock = nullptr;
    std::vector<GoBlock*> removingBlock = std::vector<GoBlock*>();
    std::map<Point*, GoBlock*> pointBlockMap = std::map<Point*, GoBlock*>();

    explicit Game(vector_2d(Point*) &points);

    void initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap);


    bool moveAnalyze(Step* Step);

    void move(bool handCapFlag);

    void getPickUpBlock(Point* targetPoint);

    void loadFromBoardFile(const std::string &fileName, int gapPlayer);

    void loadFromBoard(const std::string &boardCode, int gapPlayer);

    void boardStrEncode(char* boardStr);

    friend void operator<<(std::ostream &out, const Game &o)
    {
        std::cout << " ";
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << " " << char('a' + i);
        }
        std::cout << std::endl;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            std::cout << char('a' + i) << " ";
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                switch (o.board[i][j])
                {
                    case 0:
                        std::cout << ". ";
                        break;
                    case BLACK_PLAYER:
                        std::cout << "x ";
                        break;
                    case WHITE_PLAYER:
                        std::cout << "o ";
                        break;
                }
            }
            std::cout << '\n';
        }
    }
};

#endif //MARTRIXGOC_GAME_HPP
