#include <ics46/factory/DynamicFactory.hpp>
#include "mazeGen.hpp"
#include "Maze.hpp"
#include <random>
#include <vector>
using namespace std;

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, mazeGen, "MazeGen (Required)");



//Boolean Returns True if the Direction is going to a Removeable Wall
//Checks the bounds of the Maze to return boolean value for each Direction
bool isWallRemovable(Maze& maze, int w, int h, Direction d,vector<vector<int>>& arr)
{
    switch(d) {
        //Arr is removable if it equals 0 at specific w and h
        case Direction::down:
            if (h == maze.getHeight()-1 || arr[w][h+1] != 0){
                return false;
            }
            break;
        case Direction::up:
            if (h == 0 || arr[w][h-1] != 0){
                return false;
            }
            break;
        case Direction::right:
             if (w == maze.getWidth()-1 || arr[w+1][h] != 0){
                return false;
            }
            break;
        case Direction::left:
            if (w == 0 || arr[w-1][h] != 0){
                return false;
            }
            break;
    }
    return true;
}


//Check if All Walls are Done
bool checkDone(Maze& maze, vector<vector<int>>& arr)
{
    for (int i = 0; i< arr.size();i++) {
        for(int j = 0; j < arr[i].size(); j++){
            if(arr[i][j] == 0)
                return false;
        }
    }
    return true;
}

//Check if All Walls at current W and H are able to be removed or if Container is empty
//Function to simplify and clean code
bool checkCantRemove(Maze& maze, int w, int h, vector<vector<int>>& arr) {
    if (isWallRemovable(maze,w,h,Direction::up,arr)) 
        return false; 
    else if (isWallRemovable(maze,w,h,Direction::down,arr))
        return false;
    else if (isWallRemovable(maze,w,h,Direction::left,arr))
        return false;
    else if (isWallRemovable(maze,w,h,Direction::right,arr))
        return false;
    return true;
    }

//Create Random Distribution
random_device dev;
default_random_engine engine{dev()};
uniform_int_distribution<int> dist{0,3};
vector<Direction> directionChoices = {Direction::up,Direction::down,Direction::right,Direction:: left};



//Generate a Recursive Complete Maze
void CreateNewMaze(Maze& maze, int w, int h, vector<Direction>& direction_container, vector<vector<int>>& arr) {
    //backtrack w&h once theres no more walls to Remove
    if(checkCantRemove(maze,w,h,arr) && not direction_container.empty()) {
        //Arr = -1 once its been removed and checked
        arr[w][h] = -1;
        if (direction_container.back() == Direction::up)
            h++;
        else if (direction_container.back() == Direction::down)
            h--;
        else if (direction_container.back() == Direction::right)
            w--;
        else if (direction_container.back() == Direction::left)
            w++;  
    direction_container.pop_back();
    //Recursively backtrack
    CreateNewMaze(maze,w,h,direction_container,arr); 
}

    //Remove Wall Randomly and Store Each Direction in container 
    int random_index = dist(engine);
    Direction whichwall = directionChoices[random_index];

    if (not isWallRemovable(maze,w,h,whichwall, arr)){
        if (not checkDone(maze,arr))
            CreateNewMaze(maze,w,h,direction_container,arr);
    }
    else {
        maze.removeWall(w,h,whichwall);
        //Arr = 1 once its been removed
        arr[w][h] = 1;
        direction_container.push_back(whichwall);
        if (whichwall == Direction::up)
            h--;
        else if (whichwall == Direction::down)
            h++;
        else if (whichwall == Direction::left)
            w--;
        else if (whichwall == Direction::right)
            w++;

    }
    //If not done continue recursive function
    if (not checkDone(maze,arr)) {
        CreateNewMaze(maze, w,h, direction_container, arr);
    }
    }

    void mazeGen::generateMaze(Maze&maze) {
        maze.addAllWalls();
        vector<vector<int>> arr(maze.getWidth(), vector<int>(maze.getHeight()));
        vector<Direction> d;
        CreateNewMaze(maze,0,0,d,arr);
    }





