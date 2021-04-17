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
    /*
    std::vector<aaa> b;
    b.push_back(test);
    b.emplace_back(test);
    std::cout << b[0].a1 << " " << b[1].a1 << std::endl;
    test.a1 = 2;
    b.push_back(test);
    std::cout << b[0].a1 << " " << b[1].a1 << " " << b[2].a1 << std::endl;
    std::cout << &b[0] << " " << &b[1] << " " << &b[2] << " " << &test;
     */
    vector_2d(Point) points(BOARD_SIZE);
    Point::pointsInit(points);
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << points[i][j] << ":" << std::endl;
            for (auto point : Point::getAround(points[i][j], points)) {
                std::cout << *point << " ";
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}