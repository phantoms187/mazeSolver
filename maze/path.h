#ifndef PATH_H
#define PATH_H

#include<utility>
#include<list>

using namespace std;

using point = pair<int,int>;
using path = list<point>;


// arbirary constants to represent directions
const int UP    = 0;
const int LEFT  = 1;
const int DOWN  = 2;
const int RIGHT = 3;
const int FAIL  = 4;

inline int opposite(const int dir)
{
    switch(dir)
    {
        case UP:    return DOWN;
        case LEFT:  return RIGHT;
        case DOWN:  return UP;
        case RIGHT: return LEFT;
    }
    return FAIL;
}

inline point moveIn(const int dir)
{
    switch(dir)
    {
        case UP:    return make_pair(-1,0);
        case LEFT:  return make_pair(0,-1);
        case DOWN:  return make_pair(1,0);
        case RIGHT: return make_pair(0,1);
    }
    return make_pair(-1,-1);
}

inline int direction(const point& p1, const point& p2)
{
    if(p1.first  - p2.first  == 1)  return UP;
    if(p1.first  - p2.first  == -1) return DOWN;
    if(p1.second - p2.second == 1)  return LEFT;
    if(p1.second - p2.second == -1) return RIGHT;
    return FAIL;
}

inline point operator+(const point& l, const point& r) {
    return make_pair(l.first+r.first, l.second+r.second);
}

#endif // PATH_H
