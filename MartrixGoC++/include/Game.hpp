//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GAME_HPP
#define MARTRIXGOC_GAME_HPP

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include "Step.hpp"
#include "GoBlock.hpp"
#include "Point.hpp"

class Game
{
public:
    // game base information
    int player = BLACK_PLAYER;
    // board points
    vector_2d(Point*) allBoardPoints;
    std::vector<Step*> steps = std::vector<Step*>();
    // board point information
    vector_2d(int) board = vector_2d(int)(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    long long boardZobristHash = 0;
    std::unordered_map<Point*, GoBlock*> pointBlockMap = std::unordered_map<Point*, GoBlock*>();
    // history information
    std::vector<vector_2d(int)> historyBoard = std::vector<vector_2d(int)>();
    std::unordered_set<long long> historyZobristHash = std::unordered_set<long long>();
    // tmp analyze information
    long long newBoardZobristHash = 0;
    Step* nextStep = nullptr;  GoBlock* targetBlock = nullptr;
    std::unordered_set<GoBlock*> opponentBlock = std::unordered_set<GoBlock*>();
    bool pickUpFlag = false;
    std::unordered_set<GoBlock*> mergedBlock = std::unordered_set<GoBlock*>();
    Point* around[4] = {nullptr, nullptr, nullptr, nullptr};
    size_t aroundSize = 0;

    explicit Game(vector_2d(Point*) &points);

    void initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap);

    bool moveAnalyze(Step* Step);

    void move();

    void getPickUpBlock(Point* targetPoint);

    void boardStrEncode(char* boardStr);

    void legalMove(int* legalMoves, int* qiAfterMove, size_t &len);

    void copy(Game& o);

    bool isEye(Point* pos);

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

    ~Game();
};

#endif //MARTRIXGOC_GAME_HPP
