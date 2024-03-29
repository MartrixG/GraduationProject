//
// Created by 11409 on 2021/4/17.
//

#ifndef MARTRIXGOC_GAME_HPP
#define MARTRIXGOC_GAME_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include "Step.hpp"
#include "GoBlock.hpp"

class Game
{
public:
    using PointPtr = Point::PointPtr;
    using PArVecPtr = Point::PArVecPtr;
    using PDiVecPtr = Point::PDiVecPtr;
    using BlockPtr = GoBlock::BlockPtr;

    // game base information
    int player = BLACK_PLAYER;
    // board points
    PointPtr* allBoardPoints;
    PArVecPtr* allAround;
    PDiVecPtr* allDiagonal;
    // board point information
    int* board = nullptr;
    int64_t boardZobristHash = 0;
    BlockPtr* pointBlockMap = nullptr;
    // history information
    std::vector<std::vector<int>> historyBoard = std::vector<std::vector<int>>();
    std::vector<Step*> steps = std::vector<Step*>();
    std::unordered_set<int64_t> historyZobristHash = std::unordered_set<int64_t>();
    // tmp analyze information
    int64_t newBoardZobristHash = 0;
    Step* nextStep = nullptr;  BlockPtr targetBlock = nullptr; bool pickUpFlag = false;
    std::unordered_set<BlockPtr> opponentBlock = std::unordered_set<BlockPtr>();
    std::unordered_set<BlockPtr> mergedBlock = std::unordered_set<BlockPtr>();

    Game(PointPtr* points, PArVecPtr* around, PDiVecPtr* diagonal);

    void initHandCap(std::vector<Step*> &handCapSteps, int numOfHandCap);

    bool moveAnalyze(Step* Step);

    void move();

    void getPickUpBlock(int targetPoint);

    void boardStrEncode(char* boardStr) const;

    int getWinner() const;

    void legalMove(int* legalMoves, int* qiAfterMove, size_t &len);

    bool isEye(int pos, int posPlayer) const;

    void copy(Game* o) const;

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
                switch (o.board[i * BOARD_SIZE + j])
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
