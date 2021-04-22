//
// Created by 11409 on 2021/4/22.
//

#include <string>
#include <iostream>
#include <fstream>
#include "Game.hpp"
#include "GoSGF.hpp"
#include "App.hpp"

void Application::loadSGF(char *fileName)
{
    // read sgf file
    std::fstream fileStick(fileName);
    std::string fileContext;
    std::getline(fileStick, fileContext);
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    GoSGF sgf(fileContext);

    if (sgf.HA != 0)
    {
        sgf.nowStep += sgf.HA;
        game.initHandCap(sgf.steps, sgf.HA);
    }
    while (sgf.haveNextStep())
    {
        Step *step = nullptr;
        sgf.getNextStep(step);
        std::cout << "No." << sgf.nowStep - sgf.HA << "(" << *step << ")\n";
        if (game.moveAnalyze(step))
        {
            game.move();
            std::cout << game;
        }
    }
}

void Application::gameInformationAnalyze()
{
    std::cout << "Not finished.\n";
}

void Application::commandLine()
{
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    int player = game.player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    std::cout << "game start.\n";
    while (true)
    {
        player = player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
        int x, y;
        std::cin >> x >> y;
        if (x == -1 && y == -1)
        {
            break;
        }
        x--;
        y--;
        Step *nextStep = new Step(player, allBoardPoints[x][y]);
        if (game.moveAnalyze(nextStep))
        {
            game.move();
            std::cout << *game.nextStep << std::endl << game;
        } else
        {
            std::cout << "illegal location\n";
            std::cout << game;
            player = player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
        }
    }
}