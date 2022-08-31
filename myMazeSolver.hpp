#ifndef MYMAZESOLVER_HPP
#define MYMAZESOLVER_HPP
#include "MazeSolver.hpp"
using namespace std;

class myMazeSolver : public MazeSolver
{
    public:
        void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
};


#endif