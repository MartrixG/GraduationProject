//
// Created by 11409 on 2021/5/31.
//

#include "PythonBackend.hpp"
#include "BoardEncode.hpp"

PythonBackend::PythonBackend(SOCKET &server)
{
    this->serverSocket = server;
    this->sendMessageBuf = new char[2000];
    this->recvMessageBuf = new char[2200];
}

void PythonBackend::commit(TreeNode* node)
{
    requestVector[this->commitVector.size()] = boardEncode(node->game);
    requestVector[this->commitVector.size()].push_back((node->game->player == BLACK_PLAYER ? '1' : '2'));
    this->commitVector.push_back(node);
    if(this->commitVector.size() == 64)
    {
        this->pushRequest();
    }
}

void PythonBackend::pushRequest()
{
    if(this->commitVector.empty())
    {
        return;
    }
    if(int(this->commitVector.size()) & 1)
    {
        TreeNode* tmpNode = this->commitVector.back();
        requestVector[this->commitVector.size()] = boardEncode(tmpNode->game);
        requestVector[this->commitVector.size()].push_back((tmpNode->game->player == BLACK_PLAYER ? '1' : '2'));
        this->commitVector.push_back(tmpNode);
    }
    int size = int(this->commitVector.size());
    this->sendMessageBuf[0] = (char)size;
    send(this->serverSocket, this->sendMessageBuf, 1, 0);
    for(size_t i = 0; i < this->commitVector.size(); i++)
    {
        strcpy(this->sendMessageBuf, this->requestVector[i].c_str());
        send(this->serverSocket, this->sendMessageBuf, (int)this->requestVector[i].length(), 0);
    }

    for(auto & node : this->commitVector)
    {
        recv(this->serverSocket, this->recvMessageBuf, this->recvBufSize, 0);
        node->updateChildQ(this->recvMessageBuf);
    }
    this->commitVector.clear();
}

PythonBackend::~PythonBackend()
{
    delete[] this->sendMessageBuf;
    delete[] this->recvMessageBuf;
}

void PythonBackend::clear()
{
    this->commitVector.clear();
}

size_t PythonBackend::size()
{
    return this->commitVector.size();
}
