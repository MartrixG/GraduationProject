//
// Created by 11409 on 2021/4/22.
//

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Game.hpp"
#include "GoSGF.hpp"
#include "App.hpp"
#include "BoardEncode.hpp"

void Application::loadSGF(int argc, char* argv[])
{
    // read sgf file
    if (argc < 3)
    {
        std::cout << "need file name";
        return;
    }
    std::fstream fileStick(argv[2]);
    std::string fileContext;
    std::getline(fileStick, fileContext);
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    GoSGF sgf(fileContext);

    if (sgf.HA != 0)
    {
        sgf.nowStep += sgf.HA - 1;
        game.initHandCap(sgf.steps, sgf.HA - 1);
    }
    while (sgf.haveNextStep())
    {
        Step* step = nullptr;
        sgf.getNextStep(step);
        std::cout << "No." << sgf.nowStep - sgf.HA << "(" << *step << ")\n";
        if (game.moveAnalyze(step))
        {
            game.move();
            std::cout << game;
        }
    }
}

void Application::makeData(int argc, char* argv[])
{
    // arg analyze
    if (argc < 5)
    {
        std::cout << "need file name or file line";
        return;
    }
    // init points
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    // read sgf file
    std::ifstream srcFileStream(argv[2], std::ios::in);
    std::string featureFileName = argv[3];
    featureFileName += "/feature.txt";
    std::ofstream featureFileStream(featureFileName, std::ios::out);
    std::string srcSgf;
    int chosenNumberOfLine;
    char* _;
    chosenNumberOfLine = strtol(argv[4], &_, 10);
    int line = 0, lines = 482;
    if (chosenNumberOfLine == 0)
    {
        while (std::getline(srcFileStream, srcSgf))
        {
            gameInformationAnalyze(allBoardPoints, srcSgf, featureFileStream);
            line++;
            std::cout << std::left << std::setw(3) << line << "/ " << lines << '\n';
        }
    }
    else
    {
        while (chosenNumberOfLine != 0)
        {
            chosenNumberOfLine--;
            std::getline(srcFileStream, srcSgf);
        }
        gameInformationAnalyze(allBoardPoints, srcSgf, featureFileStream);
    }
    srcFileStream.close();
    featureFileStream.close();
}

void Application::gameInformationAnalyze(vector_2d(Point*) &allBoardPoints, std::string &srcSgf, std::ofstream &featureFileStream)
{
    // init go game
    Game game = Game(allBoardPoints);
    GoSGF sgf(srcSgf);
    // handCap
    if (sgf.HA != 0)
    {
        sgf.nowStep += sgf.HA - 1;
        game.initHandCap(sgf.steps, sgf.HA - 1);
    }
    // step by step
    Step* step = nullptr;
    sgf.getNextStep(step);
    do
    {
        if (game.moveAnalyze(step))
        {
            game.move();
            boardEncode(game, featureFileStream);
        }
        sgf.getNextStep(step);
        featureFileStream << *step << '\n';
    } while (sgf.haveNextStep());
}

void Application::commandLine(int argc, char* argv[])
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
        Step* nextStep = new Step(player, allBoardPoints[x][y]);
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