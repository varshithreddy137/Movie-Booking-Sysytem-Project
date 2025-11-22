#pragma once
#include <ctime>
#include <cstdlib>
using namespace std;

inline int nextMovieId = 500;
inline int nextShowId = 1000;

inline int generateBookingId() 
{
    static int counter = 0;
    ++counter;
    srand((unsigned)time(nullptr));
    int r = rand() % 900 + 100;
    return (int)(time(nullptr) % 100000) + r + counter;
}
inline int generateShowId() 
{
    return nextShowId++;
}
inline int generateMovieId() 
{
    return nextMovieId++;
}

inline void setNextMovieId(int v) 
{
    if (v > nextMovieId) nextMovieId = v;
}
inline void setNextShowId(int v) 
{
    if (v > nextShowId) nextShowId = v;
}
