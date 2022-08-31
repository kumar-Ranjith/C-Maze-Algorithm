#include <ics46/factory/DynamicFactory.hpp>
#include "myMazeSolver.hpp"
#include "Maze.hpp"
#include "MazeSolution.hpp"
#include <random>
#include <vector>
using namespace std;

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, myMazeSolver, "MyMazeSolver (Required)");

bool canMove(const Maze& maze, int w, int h, Direction d, vector<vector<int>>& arr)
{
    if (not maze.wallExists(w, h, d))
    {
        switch(d) {
            case Direction::up:
                if (h == 0 || arr[w][h-1] != 0) {
                    return false;
                }
                break;
            case Direction::down:
                if (h == maze.getHeight()-1 || arr[w][h+1] != 0) {
                    return false;
                }
                break;
            case Direction::left:
                if(w==0 || arr[w-1][h] != 0) {
                    return false;
                }
                break;
            case Direction::right:
                if(w == maze.getWidth()-1 || arr[w+1][h] != 0){
                    return false;
                }
                break;
        }
    }
    else 
        return false;

    return true;
}

bool checkCantMove(const Maze& maze, int w, int h, vector<vector<int>>& arr) {
    if (canMove(maze,w,h,Direction::up,arr)) 
        return false; 
    else if (canMove(maze,w,h,Direction::down,arr))
        return false;
    else if (canMove(maze,w,h,Direction::left,arr))
        return false;
    else if (canMove(maze,w,h,Direction::right,arr))
        return false;
    return true;
    }

Direction randDirection(const Maze& maze, int w, int h, MazeSolution& thisSolution, vector<vector<int>>& arr) {
    random_device dev;
    default_random_engine engine{dev()};
    uniform_int_distribution<int> dist{0,3};
    vector<Direction> directionChoices = {Direction::up,Direction::down,Direction::right,Direction:: left};
    int random_index = dist(engine);
    Direction whichwall = directionChoices[random_index];
    if (canMove(maze,w,h,whichwall,arr)) {
        return whichwall;
    }
    else {
        return randDirection(maze,w,h,thisSolution,arr);
    }
    return whichwall;
}

void moveSolution(const Maze& maze, int w, int h, MazeSolution& thisSolution,vector<vector<int>>& arr)
{
    if (not thisSolution.isComplete()) {
    if (checkCantMove(maze, w, h, arr))
    {
        arr[w][h] = -1;
        if (thisSolution.getMovements().back() == Direction::up)
            h++;
        else if (thisSolution.getMovements().back() == Direction::down)
            h--;
        else if (thisSolution.getMovements().back() == Direction::right)
            w--;
        else if (thisSolution.getMovements().back() == Direction::left)
            w++;  
        thisSolution.backUp();
        return moveSolution(maze, w, h, thisSolution, arr);
    }
    Direction thisDirection = randDirection(maze, w, h, thisSolution, arr);
    arr[w][h] = 1;
    switch(thisDirection) {
        case Direction::down:
            thisSolution.extend(Direction::down);
            h++;
            break;
        case Direction::right:
            thisSolution.extend(Direction::right);
            w++;
            break;
        case Direction::left:
            thisSolution.extend(Direction::left);
            w--;
            break;
        case Direction::up:
            thisSolution.extend(Direction::up);
            h--;
            break;
    }
    moveSolution(maze, w, h, thisSolution, arr);
    }
    else {
        return;
    }
}

void myMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
    mazeSolution.restart();
    vector<vector<int>> arr(maze.getWidth(), vector<int>(maze.getHeight()));    
    moveSolution(maze, 0, 0, mazeSolution, arr);
}