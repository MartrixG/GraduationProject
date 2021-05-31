//
// Created by 11409 on 2021/5/31.
//

#ifndef MARTRIXGOC_PYTHONBACKEND_HPP
#define MARTRIXGOC_PYTHONBACKEND_HPP

#include "TreeNode.hpp"
#include "winsock.h"

class PythonBackend
{
private:
    SOCKET serverSocket;
    std::string requestVector[64];
    char* sendMessageBuf;
    char* recvMessageBuf;
    int recvBufSize = 6 * BOARD_SIZE * BOARD_SIZE;
    std::vector<TreeNode*> commitVector;

public:
    explicit PythonBackend(SOCKET &server);

    void commit(TreeNode* node);

    void pushRequest();

    void clear();

    size_t size();

    ~PythonBackend();
};

#endif //MARTRIXGOC_PYTHONBACKEND_HPP
