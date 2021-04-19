#include <iostream>
#include <vector>
#include "Point.hpp"
#include "Game.hpp"
#include "GoBlock.hpp"
/*
class bbb
{
public:
    int f1, f2;
    bbb(int f1, int f2)
    {
        this->f1 = f1;
        this->f2 = f2;
    }
    friend std::ostream& operator<<(std::ostream& out, const bbb& o) {
        out << "x: " << o.f1 << " y: " << o.f2;
        return out;
    }
};

class aaa
{
public:
    int a1;
    bbb* a2;
    aaa(int x, bbb* y)
    {
        this->a1 = x;
        this->a2 = y;
    }
};
bbb s = bbb(1, 2);
aaa test = aaa(1, &s);
*/
int main()
{
    //bbb b1(1, 2), b2(3, 4), b3(5, 6), b4(7, 8);
    /*
    vector_2d(Point) allBoardPoints(BOARD_SIZE);
    Point::pointsInit(allBoardPoints);
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << allBoardPoints[i][j] << ":" << std::endl;
            for (auto point : Point::getAround(allBoardPoints[i][j], allBoardPoints)) {
                std::cout << *point << " ";
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
     */
    Point::test();
    GoBlock::test();
}