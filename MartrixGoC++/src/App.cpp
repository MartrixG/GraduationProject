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
    int line = 0, lines = 9348;
    if (chosenNumberOfLine == 0)
    {
        while (std::getline(srcFileStream, srcSgf))
        {
            gameInformationAnalyze(allBoardPoints, allAround, allDiagonal, srcSgf, featureFileStream, labelFileStream);
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
        gameInformationAnalyze(allBoardPoints, allAround, allDiagonal, srcSgf, featureFileStream, labelFileStream);
    }
    srcFileStream.close();
    featureFileStream.close();
}

void Application::gameInformationAnalyze(PointPtr* allBoardPoints, PArVecPtr* around, PDiVecPtr* diagonal,
                                         std::string &srcSgf, std::ofstream &featureFileStream,
                                         std::ofstream &labelFileStream)
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
    sgf.getNextStep(game.nextStep);
    do
    {
        if (game.moveAnalyze(game.nextStep))
        {
            game.move();
            featureFileStream << boardEncode(&game) << '\n';
        } else
        {
            break;
        }
        sgf.getNextStep(game.nextStep);
        labelFileStream << *game.nextStep << '\n';
    } while (sgf.haveNextStep());
}

void Application::commandLine()
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

    while (true)
    {
        player->getNextStep(game.nextStep);
        if (game.nextStep->pos == -1)
        {
            std::cout << "finish.\n";
            break;
        }
        if (game.moveAnalyze(game.nextStep))
        {
            game.move();
            std::cout << game;
            std::cout << '\n';
        } else
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
    logger.info("Init socket success.");
    // listening
    logger.info("Server socket listening at: " + std::string(ipAddr) + ":" + std::to_string(portNumber));
    listen(serverSocket, 5);
    // init client socket
    sockaddr_in clientAddr{};
    int clientAddrSize = sizeof(clientAddr);
    // catch connect
    clientSocket = accept(serverSocket, (SOCKADDR*) &clientAddr, &clientAddrSize);
    logger.info("Accept client response. Client addr:" + std::to_string(clientAddr.sin_port));
    return true;
}

void Application::uiSocket(int argc, char** argv)
{
    //ui 127.0.0.0 23333 16 ai 10 human 0
    if(argc != 9)
    {
        logger.fatal("Arg number error.");
        return;
    }
    // init server socket
    SOCKET serverSocket, clientSocket;
    if (!initSocket(serverSocket, clientSocket, argv[2], argv[3]))
    {
        logger.fatal("Init socket failed.");
        return;
    }
    // init go game
    PointPtr allBoardPoints[BOARD_SIZE * BOARD_SIZE];
    PArVecPtr allAround[BOARD_SIZE * BOARD_SIZE];
    PDiVecPtr allDiagonal[BOARD_SIZE * BOARD_SIZE];
    Point::pointsInit(allBoardPoints, allAround, allDiagonal);
    Game game = Game(allBoardPoints, allAround, allDiagonal);
    logger.info("Game init finish.");
    // init thread pool
    int num;
    char* _;
    num = strtol(argv[4], &_, 10);
    ThreadPool threadPool(num);
    logger.info("Thread pool init. using " + std::to_string(num) + " threads.");
    // init player
    std::string blackPlayerType = argv[5];
    std::string whitePlayerType = argv[7];
    SocketPlayer* uiBlackPlayer = nullptr;
    SocketPlayer* uiWhitePlayer = nullptr;
    MCTSPlayer* mctsBlackPlayer = nullptr;
    MCTSPlayer* mctsWhitePlayer = nullptr;
    if (blackPlayerType == "ai")
    {
        num = strtol(argv[6], &_, 10);
        mctsBlackPlayer = new MCTSPlayer(BLACK_PLAYER, &threadPool, num * 1000);
    } else if (blackPlayerType == "human")
    {
        uiBlackPlayer = new SocketPlayer(BLACK_PLAYER);
    } else
    {
        logger.fatal("Error player type.");
        return;
    }
    if (whitePlayerType == "ai")
    {
        num = strtol(argv[8], &_, 10);
        mctsWhitePlayer = new MCTSPlayer(WHITE_PLAYER, &threadPool, num * 1000);
    } else if (whitePlayerType == "human")
    {
        uiWhitePlayer = new SocketPlayer(WHITE_PLAYER);
    } else
    {
        logger.fatal("Error player type.");
        return;
    }
    // init message & wait for start
    char srcMessage[512];
    int bufSize = 512;
    memset(srcMessage, 0, sizeof(srcMessage));
    recv(clientSocket, srcMessage, bufSize, 0);
    // init logger
    if (srcMessage[0] == startFlag)
    {
        logger.info("Game start.");
        if (blackPlayerType == "ai")
        {
            logger.info("Black player:ai.");
        } else
        {
            logger.info("Black player:human.");
        }
        if (whitePlayerType == "ai")
        {
            logger.info("White player:ai.");
        } else
        {
            logger.info("White player:human.");
        }
    } else
    {
        logger.fatal("Wrong start flag.");
        return;
    }
    // first step
    // s: game start
    // g: game end
    // e: null operator
    // b: board str
    // c: confess
    // n: ui next step
    if (blackPlayerType == "ai")
    {
        mctsBlackPlayer->getFirstStep(game.nextStep);
        game.moveAnalyze(game.nextStep);
        game.move();
        srcMessage[0] = boardStateStr;
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, bufSize, 0);
    } else
    {
        recv(clientSocket, srcMessage, bufSize, 0);
        uiBlackPlayer->updatePlayer(srcMessage);
        uiBlackPlayer->getNextStep(game.nextStep);
        game.moveAnalyze(game.nextStep);
        game.move();
        srcMessage[0] = boardStateStr;
        game.boardStrEncode(srcMessage);
        send(clientSocket, srcMessage, bufSize, 0);
    }
    // main game body
    MCTSPlayer* mctsPlayer;
    SocketPlayer* uiPlayer;
    int nowPlayerColor = WHITE_PLAYER;
    while (true)
    {
        if ((nowPlayerColor == WHITE_PLAYER && mctsWhitePlayer != nullptr) ||
            (nowPlayerColor == BLACK_PLAYER && mctsBlackPlayer != nullptr))
        {
            if (nowPlayerColor == WHITE_PLAYER)
            {
                mctsPlayer = mctsWhitePlayer;
            } else
            {
                mctsPlayer = mctsBlackPlayer;
            }
            mctsPlayer->updatePlayer(&game);
            mctsPlayer->getNextStep(game.nextStep);
            if (game.nextStep->pos == -2)
            {
                srcMessage[0] = confessFlag;
                srcMessage[1] = '\0';
                send(clientSocket, srcMessage, bufSize, 0);
                logger.info(nowPlayerColor == BLACK_PLAYER ? "Black" : "White" + std::string("ai confess."));
                break;
            }
            game.moveAnalyze(game.nextStep);
            logger.info(game.nextStep->toString() + " (ai)");
            game.move();
            srcMessage[0] = boardStateStr;
            game.boardStrEncode(srcMessage);
            send(clientSocket, srcMessage, bufSize, 0);
        } else
        {
            if (nowPlayerColor == WHITE_PLAYER)
            {
                uiPlayer = uiWhitePlayer;
            } else
            {
                uiPlayer = uiBlackPlayer;
            }
            recv(clientSocket, srcMessage, bufSize, 0);
            if (srcMessage[0] != uiNextStepFlag)
            {
                logger.fatal("Wrong player turn. Expect human player.");
                break;
            }
            uiPlayer->updatePlayer(srcMessage);
            uiPlayer->getNextStep(game.nextStep);
            bool analyzeFlag = game.moveAnalyze(game.nextStep);
            if (analyzeFlag)
            {
                logger.info(game.nextStep->toString() + " (human)");
                game.move();
            }
            srcMessage[0] = boardStateStr;
            game.boardStrEncode(srcMessage);
            send(clientSocket, srcMessage, bufSize, 0);
            if (!analyzeFlag)
            {
                logger.info("Human chose illegal position.");
                continue;
            }
        }
        nowPlayerColor = BLACK_PLAYER + WHITE_PLAYER - nowPlayerColor;
    }
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

void Application::randomPlayerTest(int argc, char** argv)
{
    if(argc != 3)
    {
        return;
    }
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

    for (int i = 0; i < num; i++)
    {
        Game game = Game(allBoardPoints, allAround, allDiagonal);
        RandomPlayer* player = blackPlayer;
        for(int j = 0; j < 120; j++)
        {
            player->updatePlayer(&game);
            player->getNextStep(game.nextStep);
            if (game.nextStep->pos == -1)
            {
                break;
            }
            game.moveAnalyze(game.nextStep);
            game.move();
            player = player == blackPlayer ? whitePlayer : blackPlayer;
        }
        b[1 - ((double) game.getWinner() >= 2)]++;
    }
    std::cout << "B:" << b[0] << "  W:" << b[1] << '\n';
}

void Application::mctsPlayerTest(int argc, char** argv)
{
    if(argc != 3)
    {
        logger.fatal("Arg number error.");
        return;
    }
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

    auto* blackPlayer = new MCTSPlayer(BLACK_PLAYER, &threadPool, 10000);
    auto* whitePlayer = new MCTSPlayer(WHITE_PLAYER, &threadPool, 10000);
    logger.info("Black player search time limit:" + std::to_string(blackPlayer->timeLimit));
    logger.info("White player search time limit:" + std::to_string(whitePlayer->timeLimit));
    MCTSPlayer* player = whitePlayer;

    blackPlayer->getFirstStep(game.nextStep);
    game.moveAnalyze(game.nextStep);
    logger.info(game.nextStep->toString());
    game.move();
    std::cout << *game.nextStep << '\n';
    std::cout << game;
    std::cout << '\n';

    while (true)
    {
        player->updatePlayer(&game);
        player->getNextStep(game.nextStep);
        if (game.nextStep->pos == -1)
        {
            break;
        }
        if (game.nextStep->pos == -2)
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
        player = player->playerColor == BLACK_PLAYER ? whitePlayer : blackPlayer;
//        player->playerColor = BLACK_PLAYER + WHITE_PLAYER - player->playerColor;
    }
    int winColor = 2 - ((double) game.getWinner() >= 2.5);
    std::cout << winColor << " win.\n";
}
