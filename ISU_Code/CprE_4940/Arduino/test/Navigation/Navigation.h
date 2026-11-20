#ifndef NAV_H
#define NAV_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

// Define the blueprint for a Cell here so both files can use it
struct Cell {
    int x, y;
    double gScore = 0, hScore = 0, fScore = 0;
    bool start = false, goal = false;
    bool wall = false, closed = false;
    Cell* parent = nullptr;

    Cell() {}
    Cell(int nx, int ny) { x = nx; y = ny; }

    double getf(int goalX, int goalY) {
        if (wall) return -1.0;
        hScore = std::sqrt(std::pow(x - goalX, 2) + std::pow(y - goalY, 2));
        fScore = gScore + hScore;
        return fScore;
    }
    bool available() { return !wall && !closed; }
};

struct Waypoint {
    std::string action;
    int x;
    int y;
    double lat;
    double lon;
    float saltRemaining;
    float battRemaining;
};
extern std::vector<Waypoint> fullMissionPlan;

// Share these global variables across both files
extern int GridWidth, GridLength;
extern std::vector<float> saltLevels;
extern std::vector<Cell> grid;
extern std::vector<Cell*> searchHistory;
extern std::vector<Cell*> aStarPath;
extern int Start, Goal;
extern float MAX_SALT_LBS;
extern int spreadX;
extern int spreadY;

extern std::vector<int> pathState; 
extern int currentPathType;

// Function declarations
void A_Star(int sX, int sY, int gX, int gY);
int createGrid(const std::vector<std::string>& mapLines);
bool setStart(int x, int y);
void findPath();
void resetGridForNextRun();
bool setSafeStart(int targetX, int targetY);
bool setMainIslandStart();

#endif // NAV_H