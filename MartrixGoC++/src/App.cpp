//
// Created by 11409 on 2021/4/22.
//

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <winsock.h>
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
        } else
        {
            break;
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
    // process feature file, label file
    std::string featureFileName = argv[3], labelFileName = argv[3];
    featureFileName += "/feature.txt";
    labelFileName += "/label.txt";
    std::ofstream featureFileStream(featureFileName, std::ios::out);
    std::ofstream labelFileStream(labelFileName, std::ios::out);
    std::string srcSgf;
    int chosenNumberOfLine;
    char* _;
    chosenNumberOfLine = strtol(argv[4], &_, 10);
    int line = 0, lines = 9348;
    if (chosenNumberOfLine == 0)
    {
        while (std::getline(srcFileStream, srcSgf))
        {
            gameInformationAnalyze(allBoardPoints, srcSgf, featureFileStream, labelFileStream);
            line++;
            std::cout << std::left << std::setw(4) << line << "/ " << lines << '\n';
        }
    } else
    {
        while (chosenNumberOfLine != 0)
        {
            chosenNumberOfLine--;
            std::getline(srcFileStream, srcSgf);
        }
        gameInformationAnalyze(allBoardPoints, srcSgf, featureFileStream, labelFileStream);
    }
    srcFileStream.close();
    featureFileStream.close();
}

void Application::gameInformationAnalyze(vector_2d(Point*) &allBoardPoints, std::string &srcSgf,
                                         std::ofstream &featureFileStream, std::ofstream &labelFileStream)
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
        } else
        {
            break;
        }
        sgf.getNextStep(step);
        labelFileStream << *step << '\n';
    } while (sgf.haveNextStep());
}

void Application::commandLine(int argc, char* argv[])
{
    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);
    game.loadFromBoardFile(argv[2], BLACK_PLAYER);

    int player = game.player == WHITE_PLAYER ? BLACK_PLAYER : WHITE_PLAYER;
    std::cout << "game start.\n" << game;
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

bool initSocket(SOCKET &serverSocket, char* ipAddr, char* port)
{
    WSAData wsaData{};
    int err;
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    { return false; }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr{};
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipAddr);
    int portNumber;
    char* _;
    portNumber = strtol(port, &_, 10);
    serverAddr.sin_port = htons(portNumber);
    err = bind(serverSocket, (SOCKADDR*) &serverAddr, sizeof(serverAddr));
    if (err != 0)
    { return false; }
    return true;
}

bool initMove(Game &game, const std::string& stepStr)
{
    int len = int(stepStr.length());
    int pos = 0;
    for(int i = 1; i < len; i++)
    {
        pos *= 10;
        pos += stepStr[i] - '0';
    }
    int x = pos / 19, y = pos % 19;
    Step* nextStep = new Step(game.player, game.allBoardPoints[x][y]);
    if(game.moveAnalyze(nextStep))
    {
        game.move();
        return true;
    }
    else
    {
        return false;
    }
}

void Application::uiSocket(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "arg number is invalid.";
        return;
    }
    // init server socket
    SOCKET serverSocket;
    if (!initSocket(serverSocket, argv[2], argv[3]))
    {
        std::cout << "init socket failed.\n";
        return;
    }
    else
    {
        std::cout << "init socket success.\n";
    }
    // listening
    listen(serverSocket, 5);

    // init client socket
    sockaddr_in clientAddr{};
    int clientAddrSize = sizeof(clientAddr);

    // catch connect
    SOCKET clientSocket = accept(serverSocket, (SOCKADDR*) &clientAddr, &clientAddrSize);
    std::cout << "connect start. client addr:" << clientAddr.sin_port << '\n';

    // init go game
    vector_2d(Point*) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    Game game = Game(allBoardPoints);

    char srcMessage[1024];
    std::string message;
    memset(srcMessage, 0, sizeof(srcMessage));

    while (true)
    {
        recv(clientSocket, srcMessage, 1024, 0);
        message.clear();
        message = srcMessage;
        switch (srcMessage[0])
        {
            case 'c':
                goto close;
            case 'b':
                //game.loadFromBoardStr(message, WHITE_PLAYER);
                break;
            case 's':
                initMove(game, message);
                break;
            default:
                break;
        }
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, 1024, 0);
    }
    close:
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}