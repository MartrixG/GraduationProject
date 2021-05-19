//
// Created by 11409 on 2021/4/22.
//

#include <string>
#include <winsock.h>
#include "Game.hpp"
#include "GoSGF.hpp"
#include "App.hpp"
#include "BoardEncode.hpp"
#include "CommandLinePlayer.hpp"
#include "SocketPlayer.hpp"
#include "RandomPlayer.hpp"
#include "MCTSPlayer.hpp"

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
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    Game game = Game(allBoardPoints, allAround, allDiagonal);

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
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
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
//    int line = 0, lines = 9348;
    if (chosenNumberOfLine == 0)
    {
        while (std::getline(srcFileStream, srcSgf))
        {
            gameInformationAnalyze(allBoardPoints, allAround, allDiagonal, srcSgf, featureFileStream, labelFileStream);
//            line++;
//            std::cout << std::left << std::setw(4) << line << "/ " << lines << '\n';
        }
    }
    else
    {
        while (chosenNumberOfLine != 0)
        {
            chosenNumberOfLine--;
            std::getline(srcFileStream, srcSgf);
        }
        gameInformationAnalyze(allBoardPoints, allAround, allDiagonal, srcSgf, featureFileStream, labelFileStream);
    }
    srcFileStream.close();
    featureFileStream.close();
}

void Application::gameInformationAnalyze(PointPtr* allBoardPoints, PArVecPtr* around, PDiVecPtr* diagonal,
                                         std::string &srcSgf, std::ofstream &featureFileStream, std::ofstream &labelFileStream)
{
    // init go game
    Game game = Game(allBoardPoints, around, diagonal);
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
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    Game game = Game(allBoardPoints, allAround, allDiagonal);
    // init player
    auto* blackPlayer = new CommandLinePlayer(BLACK_PLAYER);
    auto* whitePlayer = new CommandLinePlayer(WHITE_PLAYER);
    CommandLinePlayer* player = blackPlayer;
    std::cout << "game start.\n" << game;

    while(true)
    {
        player->getNextStep(game.nextStep);
        if(game.nextStep->pos == -1)
        {
            std::cout << "finish.\n";
            break;
        }
        if(game.moveAnalyze(game.nextStep))
        {
            game.move();
            std::cout << game;
            std::cout << '\n';
        }
        else
        {
            std::cout << "illegal position.\n";
            player = player == blackPlayer ? whitePlayer : blackPlayer;
        }
        player = player == blackPlayer ? whitePlayer : blackPlayer;
    }
}

bool initSocket(SOCKET &serverSocket, SOCKET &clientSocket, char* ipAddr, char* port)
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
    // listening
    listen(serverSocket, 5);
    // init client socket
    sockaddr_in clientAddr{};
    int clientAddrSize = sizeof(clientAddr);
    // catch connect
    clientSocket = accept(serverSocket, (SOCKADDR*) &clientAddr, &clientAddrSize);
    std::cout << "connect start. client addr:" << clientAddr.sin_port << '\n';
    return true;
}

void Application::uiSocket(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "arg number is invalid.";
        return;
    }
    // init server socket
    SOCKET serverSocket, clientSocket;
    if (!initSocket(serverSocket, clientSocket, argv[2], argv[3]))
    {
        std::cout << "init socket failed.\n";
        return;
    }
    else
    {
        std::cout << "init socket success.\n";
    }
    // init go game
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    Game game = Game(allBoardPoints, allAround, allDiagonal);
    Step nextStep(-1, -1, -1);

    char srcMessage[512];
    int bufSize = 512;
    int handCapNum = 0;
    memset(srcMessage, 0, sizeof(srcMessage));

    recv(clientSocket, srcMessage, bufSize, 0);
    SocketPlayer* uiPlayer;
    MCTSPlayer* mctsPlayer;
    if(srcMessage[0] == '1')
    {
        uiPlayer = new SocketPlayer(BLACK_PLAYER);
//        mctsPlayer = new MCTSPlayer(WHITE_PLAYER);
    }
    else
    {
        uiPlayer = new SocketPlayer(WHITE_PLAYER);
//        mctsPlayer = new MCTSPlayer(BLACK_PLAYER);
    }
    handCapNum = srcMessage[1] - '0';
    for(int i = 0; i < handCapNum - 1; i++)
    {
        //pass
    }
    if(mctsPlayer->playerColor == BLACK_PLAYER)
    {
        mctsPlayer->getFirstStep(&nextStep);
        game.moveAnalyze(&nextStep);
        game.move();
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, bufSize, 0);
    }
    while (true)
    {
        recv(clientSocket, srcMessage, bufSize, 0);
        if(srcMessage[0] == 'c')
        {
            break;
        }
        uiPlayer->updatePlayer(srcMessage);
//        gameCore(&game, uiPlayer);
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, bufSize, 0);

        mctsPlayer->updatePlayer(&game);
//        gameCore(&game, mctsPlayer);
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, bufSize, 0);
    }
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

void Application::randomPlayerTest(int argc, char** argv)
{
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    int num;
    char* _;
    num = strtol(argv[2], &_, 10);
    int b[2];
    b[0] = b[1] = 0;

    auto* blackPlayer = new RandomPlayer(BLACK_PLAYER);
    auto* whitePlayer = new RandomPlayer(WHITE_PLAYER);

    for(int i = 0; i < num; i++)
    {
        Game game = Game(allBoardPoints, allAround, allDiagonal);
        RandomPlayer* player = blackPlayer;
        while(true)
        {
            player->updatePlayer(&game);
            player->getNextStep(game.nextStep);
            if(game.nextStep->pos == -1)
            {
                break;
            }
            game.moveAnalyze(game.nextStep);
            game.move();
            player = player == blackPlayer ? whitePlayer : blackPlayer;
        }
        b[1 - ((double)game.getWinner() >= 2.5)]++;
    }
    std::cout << "B:" << b[0] << "  W:" << b[1] << '\n';
}

void Application::mctsPlayerTest(int argc, char** argv)
{
    logger.info("mcts player test begin.");
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    Game game = Game(allBoardPoints, allAround, allDiagonal);

    int threadNum;
    char* _;
    threadNum = strtol(argv[2], &_, 10);
    ThreadPool threadPool(threadNum);

    auto* blackPlayer = new MCTSPlayer(BLACK_PLAYER, &threadPool, 30000);
    auto* whitePlayer = new MCTSPlayer(WHITE_PLAYER, &threadPool, 30000);
    MCTSPlayer* player = whitePlayer;

    blackPlayer->getFirstStep(game.nextStep);
    game.moveAnalyze(game.nextStep);
    game.move();
    std::cout << *game.nextStep << '\n';
    std::cout << game;
    std::cout << '\n';

    while(true)
    {
        player->updatePlayer(&game);
        player->getNextStep(game.nextStep);
        if(game.nextStep->pos == -1)
        {
            break;
        }
        if(game.nextStep->pos == -2)
        {
            std::cout << player->playerColor << " Confess.\n";
            std::cout << BLACK_PLAYER + WHITE_PLAYER - player->playerColor << " win.\n";
            return;
        }
        game.moveAnalyze(game.nextStep);
        game.move();
        std::cout << *game.nextStep << '\n';
        logger.info(game.nextStep->toString());
        std::cout << game;
        std::cout << '\n';
        player->playerColor = BLACK_PLAYER + WHITE_PLAYER - player->playerColor;
    }
    int winColor = 2 - ((double)game.getWinner() >= 2.5);
    std::cout << winColor << " win.\n";
}
