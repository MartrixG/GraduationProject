#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<cstdio>
#include<map>

class Point
{
public:
    int x, y, boardSize;
    Point()
    {
        this->x = -1;
        this->y = -1;
        this->boardSize = -1;
    }

    Point(int x, int y, int boardSize)
    {
        this->x = x;
        this->y = y;
        this->boardSize = boardSize;
    }
}points[19][19];

std::vector<Point *> getAround(Point *nowPoint)
{
    std::vector<Point *> reAround;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        int newX = nowPoint->x + dx[i];
        int newY = nowPoint->y + dy[i];
        if(newX >= 0 && newX < nowPoint->boardSize && newY >= 0 && newY < nowPoint->boardSize)
        {
            reAround.push_back(&points[newX][newY]);
        }
    }
    return reAround;
}

class GoBlock
{
private:
    bool check(int x, int y)
    {
        return x >= 0 && x < this->beginPoint->boardSize && y >= 0 && y <= this->beginPoint->boardSize;
    }
public:
    std::set<Point*> pieces;
    std::set<Point*> qiPoint;
    Point* beginPoint;
    int color;
    int **board;
    int qi;

    GoBlock(Point* beginPoinnt, int **board)
    {
        this->board = board;
        this->beginPoint = beginPoint;
        this->color = board[beginPoint->x][beginPoint->y];
        this->qi = -1;
        this->findLinkedBlock();
    }

    void findLinkedBlock()
    {
        std::queue<Point*> Q;
        Q.push(this->beginPoint);
        this->pieces.insert(this->beginPoint);
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        while(!Q.empty())
        {
            Point* nowPoint = Q.front();
            for(int i = 0; i < 4; i++)
            {
                int newX = nowPoint->x + dx[i];
                int newY = nowPoint->y + dy[i];
                if(this->check(newX, newY) && this->board[newX][newY] == this->color)
                {
                    Point* newPoint = &points[newX][newY];
                    if(this->pieces.find(newPoint) == this->pieces.end())
                    {
                        Q.push(newPoint);
                        this->pieces.insert(newPoint);
                    }
                }
            }
        }
    }

    int getQi()
    {
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for(auto point : this->pieces)
        {
            auto around = getAround(point);
            for(auto aroundPoint : around)
            {
                int newX = aroundPoint->x;
                int newY = aroundPoint->y;
                if(this->board[newX][newY] == 0)
                {
                    Point* nowQi = &points[newX][newY];
                    if(this->qiPoint.find(nowQi) == this->qiPoint.end())
                    {
                        this->qiPoint.insert(nowQi);
                    }
                }
            }
        }
        this->qi = this->qiPoint.size();
        return this->qi;
    }
};

class Step
{
public:
    int player, x, y;

    Step(std::string srcString)
    {
        this->player = int(srcString[0]);
        char strX = srcString[3];
        char strY = srcString[2];
        if(strX >= 'a' && strX <= 'z' && strY >= 'a' && strY <= 'z')
        {
            this->x = x - 'a';
            this->y = y - 'a';
        }
    }

    Step(int player, Point* point)
    {
        this->player = player;
        this->x = point->x;
        this->y = point->y;
    }
};

class Game
{
public:
    int player;
    int boardSize = 19;
    int numOfStep = 0;
    // std::vector<Step*> steps;
};

std::ostream& operator<<(std::ostream& out, const Point& o)
{
    out << "x: " << o.x << " y: " << o.y;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Step& o)
{
    out << o.x << " " << o.y;
}

static void pointsInit()
{
    for(int i = 0; i < 19; i++)
    {
        for(int j = 0; j < 19; j++)
        {
            points[i][j] = *(new Point(i, j, 19));
        }
    }
}

int main()
{
    pointsInit();
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cout << points[i][j] << ":" << std::endl;
            for(auto point : getAround(&points[i][j]))
            {
                std::cout << *point << " ";
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}