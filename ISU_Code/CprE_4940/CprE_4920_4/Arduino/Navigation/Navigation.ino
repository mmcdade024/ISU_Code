#include "Navigation.h"
#include <algorithm>
#include <iostream>

Cell* currentcell;
int Width, Length;
std::vector<std::vector<Cell>> grid;
std::vector<Cell*> searchHistory;
std::vector<Cell*> aStarPath;
std::vector<int> pathState;

std::vector<float> saltLevels;
float MAX_SALT_LBS = 8.0f;
float currentSalt = 8.0f;
extern float resolution; 

std::vector<int> Start, Goal;
int currentPathType = 0; 

int spreadX = 2; // Lateral Spread (e.g., 2 cells left, 2 cells right)
int spreadY = 3; // Forward Spread (e.g., 3 cells ahead of the robot)

int robotX = 2;
int robotY = 2;

void A_Star(int sX, int sY, int gX, int gY){
    if (grid.empty()) return;

    Start = {sX, sY};
    Goal = {gX, gY};
    grid[sX][sY].start = true;
    grid[gX][gY].goal = true;

    std::vector<Cell*> openSet;
    Cell* startCell = &grid[Start[0]][Start[1]];
    openSet.push_back(startCell);

    while (!openSet.empty()){
        int bestIndex = 0;
        for(int i = 1; i < openSet.size(); i ++){
            if (openSet[i]->fScore < openSet[bestIndex]->fScore) {
                bestIndex = i;
            }
        }
        Cell* currentcell = openSet[bestIndex];
        searchHistory.push_back(currentcell);

        if (currentcell->goal){
            std::vector<Cell*> segment;
            while(currentcell != nullptr){
                segment.push_back(currentcell);
                currentcell = currentcell->parent;
            }
            std::reverse(segment.begin(), segment.end());

            if (!aStarPath.empty() && !segment.empty() && aStarPath.back() == segment.front()) {
                segment.erase(segment.begin()); 
            }

            aStarPath.insert(aStarPath.end(), segment.begin(), segment.end());
            pathState.insert(pathState.end(), segment.size(), currentPathType);
            
            for(size_t i = 0; i < segment.size(); i++) {
                saltLevels.push_back(currentSalt);
            }
            return;
        }

        openSet.erase(openSet.begin() + bestIndex);
        currentcell->closed = true;

        int dx[] = {1, -1, 0, 0, 1, -1, -1, 1};
        int dy[] = {0, 0, 1, -1, -1, -1, 1, 1};

        for(int i = 0; i < 8; i ++){
            int nx = currentcell->x + dx[i];
            int ny = currentcell->y + dy[i];

            if(nx >= 0 && nx < Width && ny >= 0 && ny < Length){
                Cell* neighbor = &grid[nx][ny];
                if(neighbor->available()){
                    double moveCost = (i < 4) ? 1.0 : 1.4142;
                    auto it = std::find(openSet.begin(), openSet.end(), neighbor);

                    if(it == openSet.end()){
                        neighbor->parent = currentcell;
                        neighbor->gScore = currentcell->gScore + 1.0;
                        neighbor->getf(Goal[0], Goal[1]); 
                        openSet.push_back(neighbor);
                    }
                    else if (currentcell->gScore + moveCost < neighbor->gScore) {
                        neighbor->parent = currentcell;
                        neighbor->gScore = currentcell->gScore + moveCost;
                        neighbor->getf(Goal[0], Goal[1]);
                    }
                }
            }
        }
    }
}

int createGrid(const std::vector<std::string>& mapLines) {
    if (mapLines.empty()) return -1;
    Length = mapLines.size(); 
    Width = mapLines[0].size(); 
    grid.resize(Width, std::vector<Cell>(Length));

    for (int x = 0; x < Width; x++) {
        for (int y = 0; y < Length; y++) {
            grid[x][y].x = x;
            grid[x][y].y = y;
            grid[x][y].wall = (mapLines[y][x] == '#');
        }
    }
    return 0;
}

void resetGridForNextRun() {
    for (int x = 0; x < Width; x++) {
        for (int y = 0; y < Length; y++) {
            grid[x][y].gScore = 0;
            grid[x][y].hScore = 0;
            grid[x][y].fScore = 0;
            grid[x][y].closed = false;
            grid[x][y].parent = nullptr;
            grid[x][y].start = false;
            grid[x][y].goal = false;
        }
    }
}

bool setStart(int x, int y) {
    if (x < 0 || x >= Width || y < 0 || y >= Length) return false; 
    if (grid[x][y].wall) return false;
    if (!Start.empty()) {
        grid[Start[0]][Start[1]].start = false;
        grid[Start[0]][Start[1]].gScore = 0;
    }
    Start = {x, y};
    grid[x][y].start = true;
    grid[x][y].gScore = 0;
    return true;
}

// --- HYBRID GREEDY SPIRAL ALGORITHM WITH DIRECTIONAL SPRAY ---
void findPath() {
    if (grid.empty() || Start.empty()) return;

    aStarPath.clear();
    pathState.clear();
    saltLevels.clear();

    int baseX = Start[0];
    int baseY = Start[1];

    currentSalt = MAX_SALT_LBS;
    float safeRes = (resolution > 0) ? resolution : 2.5f;
    
    // The amount of salt consumed by ONE single cell of concrete
    float lbsPerCell = (safeRes * safeRes) * 0.005f; 
    if (lbsPerCell < 0.001f) lbsPerCell = 0.001f;

    std::vector<std::vector<bool>> reachable(Width, std::vector<bool>(Length, false));
    std::vector<std::pair<int, int>> queue;
    
    reachable[baseX][baseY] = true;
    queue.push_back({baseX, baseY});
    
    int qIdx = 0;
    int dx8[] = {0, 1, 0, -1, 1, -1, -1, 1}; 
    int dy8[] = {-1, 0, 1, 0, 1, 1, -1, -1};

    while (qIdx < queue.size()) {
        int cx = queue[qIdx].first;
        int cy = queue[qIdx].second;
        qIdx++;

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx8[i];
            int ny = cy + dy8[i];
            if (nx >= 0 && nx < Width && ny >= 0 && ny < Length) {
                if (!grid[nx][ny].wall && !reachable[nx][ny]) {
                    reachable[nx][ny] = true;
                    queue.push_back({nx, ny});
                }
            }
        }
    }

    int wallPadding = 1; 
    int totalWalkable = 0;
    std::vector<std::vector<bool>> paddedReachable = reachable;
    
    for (int x = 0; x < Width; x++) {
        for (int y = 0; y < Length; y++) {
            if (reachable[x][y]) {
                bool nearWall = false;
                for (int padY = -wallPadding; padY <= wallPadding; padY++) {
                    for (int padX = -wallPadding; padX <= wallPadding; padX++) {
                        int nx = x + padX;
                        int ny = y + padY;
                        if (nx < 0 || ny < 0 || nx >= Width || ny >= Length || grid[nx][ny].wall) {
                            nearWall = true;
                            break;
                        }
                    }
                    if (nearWall) break;
                }
                if (nearWall) paddedReachable[x][y] = false;
                else totalWalkable++;
            }
        }
    }

    std::vector<std::vector<bool>> salted(Width, std::vector<bool>(Length, false));
    int saltedCount = 0;

    // Helper Function: Calculates the rectangular spray bounds based on robot's direction
    auto getSprayBounds = [&](int cx, int cy, int dir, int& minX, int& maxX, int& minY, int& maxY) {
        if (dir == 0) { // Facing North
            minX = cx - spreadX; maxX = cx + spreadX;
            minY = cy - spreadY; maxY = cy;
        } else if (dir == 1) { // Facing East
            minX = cx; maxX = cx + spreadY;
            minY = cy - spreadX; maxY = cy + spreadX;
        } else if (dir == 2) { // Facing South
            minX = cx - spreadX; maxX = cx + spreadX;
            minY = cy; maxY = cy + spreadY;
        } else if (dir == 3) { // Facing West
            minX = cx - spreadY; maxX = cx;
            minY = cy - spreadX; maxY = cy + spreadX;
        }
    };

    // Helper Function: Drops salt dynamically in the facing direction
    auto markSalted = [&](int cx, int cy, int dir) {
        int newlySalted = 0;
        int minX, maxX, minY, maxY;
        getSprayBounds(cx, cy, dir, minX, maxX, minY, maxY);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                if (x >= 0 && x < Width && y >= 0 && y < Length) {
                    if (paddedReachable[x][y] && !salted[x][y]) {
                        salted[x][y] = true;
                        newlySalted++;
                    }
                }
            }
        }
        return newlySalted; // Returns exactly how many NEW cells got salted
    };

    // Helper Function: Scans ahead to evaluate if turning is necessary
    auto countNewSalt = [&](int cx, int cy, int dir) {
        int newSalt = 0;
        int minX, maxX, minY, maxY;
        getSprayBounds(cx, cy, dir, minX, maxX, minY, maxY);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                if (x >= 0 && x < Width && y >= 0 && y < Length) {
                    if (paddedReachable[x][y] && !salted[x][y]) {
                        newSalt++;
                    }
                }
            }
        }
        return newSalt;
    };

    int currentX = baseX;
    int currentY = baseY;
    
    int dx[] = {0, 1, 0, -1}; // N, E, S, W
    int dy[] = {-1, 0, 1, 0};
    int preferredDir = 1; 

    // Base Station deployment drop
    saltedCount += markSalted(currentX, currentY, preferredDir);

    currentPathType = 0; 
    aStarPath.push_back(&grid[currentX][currentY]);
    pathState.push_back(currentPathType);
    saltLevels.push_back(currentSalt);

    int dirOffsets[] = {0, 1, 3, 2}; 

    while (saltedCount < totalWalkable) {
        
        // --- SMART REFILL TRIGGER ---
        // If we don't have enough salt to cover a full standard blast, head home to refill.
        float maxPotentialUse = ((spreadX * 2 + 1) * (spreadY + 1)) * lbsPerCell;
        if (currentSalt < maxPotentialUse) {
            currentPathType = 1; 
            resetGridForNextRun();
            A_Star(currentX, currentY, baseX, baseY);
            
            currentSalt = MAX_SALT_LBS;
            
            resetGridForNextRun();
            A_Star(baseX, baseY, currentX, currentY);
            currentPathType = 0; 
        }

        bool moved = false;

        for (int i = 0; i < 4; i++) {
            int dir = (preferredDir + dirOffsets[i]) % 4;
            int nx = currentX + dx[dir];
            int ny = currentY + dy[dir];

            if (nx >= 0 && nx < Width && ny >= 0 && ny < Length) {
                if (paddedReachable[nx][ny]) {
                    // Check if spraying in this direction actually hits dry concrete
                    if (countNewSalt(nx, ny, dir) > 0) {
                        currentX = nx;
                        currentY = ny;

                        // Drop the salt in front of the robot
                        int newlySalted = markSalted(currentX, currentY, dir);
                        saltedCount += newlySalted;
                        
                        // NO OVERLAPPING WASTE: Only consume salt from the tank for the fresh hit!
                        currentSalt -= (newlySalted * lbsPerCell);
                        if (currentSalt < 0) currentSalt = 0.0f;

                        aStarPath.push_back(&grid[currentX][currentY]);
                        pathState.push_back(currentPathType);
                        saltLevels.push_back(currentSalt);

                        preferredDir = dir; 
                        moved = true;
                        break;
                    }
                }
            }
        }

        if (!moved) {
            int bestDist = 999999;
            int bestX = -1, bestY = -1;

            for (int x = 0; x < Width; x++) {
                for (int y = 0; y < Length; y++) {
                    if (paddedReachable[x][y] && !salted[x][y]) {
                        int dist = (x - currentX)*(x - currentX) + (y - currentY)*(y - currentY);
                        if (dist < bestDist) {
                            bestDist = dist;
                            bestX = x;
                            bestY = y;
                        }
                    }
                }
            }

            if (bestX != -1) {
                resetGridForNextRun();
                int prevSize = aStarPath.size();
                currentPathType = 0; 
                A_Star(currentX, currentY, bestX, bestY);

                if (aStarPath.size() > prevSize) {
                    currentX = bestX;
                    currentY = bestY;
                    saltedCount += markSalted(currentX, currentY, preferredDir);
                } else {
                    std::vector<std::pair<int, int>> islandQueue;
                    islandQueue.push_back({bestX, bestY});
                    salted[bestX][bestY] = true;
                    saltedCount++;

                    int idx = 0;
                    while(idx < islandQueue.size()) {
                        int ix = islandQueue[idx].first;
                        int iy = islandQueue[idx].second;
                        idx++;

                        for(int d = 0; d < 4; d++) {
                            int nx = ix + dx[d];
                            int ny = iy + dy[d];
                            if(nx >= 0 && nx < Width && ny >= 0 && ny < Length) {
                                if(paddedReachable[nx][ny] && !salted[nx][ny]) {
                                    salted[nx][ny] = true;
                                    saltedCount++;
                                    islandQueue.push_back({nx, ny});
                                }
                            }
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    currentPathType = 1; 
    resetGridForNextRun();
    A_Star(currentX, currentY, baseX, baseY);
}