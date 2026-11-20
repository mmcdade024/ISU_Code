#include "Navigation.h"
#include <algorithm>
#include <iostream>
#include <string>

// Global Variables
Cell* currentcell;

int GridWidth, GridLength;
std::vector<Cell> grid;
std::vector<Cell*> searchHistory;
std::vector<Cell*> aStarPath;
std::vector<int> pathState;
int Start = -1, Goal = -1;
int currentPathType = 0; 

std::vector<float> saltLevels;
std::vector<float> batteryLevels; 
float MAX_SALT_LBS = 8.0f;
float MAX_BATTERY_CAPACITY = 10.0f; 
float currentSalt = 8.0f;
float currentBattery = 10.0f;
float volPerCell = 0.002f; 
extern float resolution; 

std::vector<Waypoint> fullMissionPlan;

double ORIGIN_LAT = 42.0266; 
double ORIGIN_LON = -93.6465;
double GRID_HEADING = 45.0; 

const double FEET_PER_DEG_LAT = 364000.0;
const double PI = 3.14159265358979323846;

int spreadX = 2; 
int spreadY = 3; 
int robotX = 2;
int robotY = 2;


void A_Star(int sX, int sY, int gX, int gY){
    if (grid.empty()) return;
    
    int Sindex = (sY * GridWidth) + sX;
    int Gindex = (gY * GridWidth) + gX;

    grid[Sindex].start = true;
    grid[Gindex].goal = true;

    std::vector<Cell*> openSet;
    Cell* startCell = &grid[Sindex];
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
                currentBattery -= volPerCell;
                if (currentBattery < 0) currentBattery = 0.0f;
                
                saltLevels.push_back(currentSalt);
                batteryLevels.push_back(currentBattery); 
            }
            return;
        }

        openSet[bestIndex] = openSet.back();
        openSet.pop_back();
        currentcell->closed = true;

        int dx[] = {1, -1, 0, 0, 1, -1, -1, 1};
        int dy[] = {0, 0, 1, -1, -1, -1, 1, 1};

        for(int i = 0; i < 8; i ++){
            int nx = currentcell->x + dx[i];
            int ny = currentcell->y + dy[i];

            if(nx >= 0 && nx < GridWidth && ny >= 0 && ny < GridLength){
                int nIndex = (ny * GridWidth) + nx;
                Cell* neighbor = &grid[nIndex];

                if(neighbor->available()){
                    double moveCost = (i < 4) ? 1.0 : 1.4142;
                    auto it = std::find(openSet.begin(), openSet.end(), neighbor);

                    if(it == openSet.end()){
                        neighbor->parent = currentcell;
                        neighbor->gScore = currentcell->gScore + 1.0;
                        neighbor->getf(gX, gY);
                        openSet.push_back(neighbor);
                    }
                    else if (currentcell->gScore + moveCost < neighbor->gScore) {
                        neighbor->parent = currentcell;
                        neighbor->gScore = currentcell->gScore + moveCost;
                        neighbor->getf(gX, gY);
                    }
                }
            }
        }
    }
}

int createGrid(const std::vector<std::string>& mapLines) {
    if (mapLines.empty()) return -1;
    GridLength = mapLines.size();
    GridWidth = mapLines[0].size(); 
    grid.resize(GridWidth * GridLength);

    for (int y = 0; y < GridLength; y++) {
        for (int x = 0; x < GridWidth; x++) {
            int index = (y * GridWidth) + x;
            grid[index].x = x;
            grid[index].y = y;
            grid[index].wall = (mapLines[y][x] == '#');
        }
    }
    return 0;
}

void resetGridForNextRun() {
    for (auto& cell : grid){
        cell.gScore = 0;
        cell.hScore = 0;
        cell.fScore = 0;
        cell.closed = false;
        cell.parent = nullptr;
        cell.start = false;
        cell.goal = false;
    }
}

bool setStart(int x, int y) {
    if (x < 0 || x >= GridWidth || y < 0 || y >= GridLength) return false;
    int index = (y * GridWidth) + x;

    if (grid[index].wall) return false;

    if (Start != -1){
        grid[Start].start = false;
        grid[Start].gScore = 0;
    }

    Start = index;
    grid[index].start = true;
    grid[index].gScore = 0;
    return true;
}

void applyGPS(Waypoint& wp) {
    if (ORIGIN_LAT == 0.0) {
        wp.lat = 0.0;
        wp.lon = 0.0;
        return;
    }

    int baseX = Start % GridWidth;
    int baseY = Start / GridWidth;
    double dx_cells = wp.x - baseX;
    double dy_cells = baseY - wp.y; 

    double dx_feet = dx_cells * resolution;
    double dy_feet = dy_cells * resolution;
    double rad = GRID_HEADING * (PI / 180.0);
    double easting_feet  = (dx_feet * cos(rad)) + (dy_feet * sin(rad));
    double northing_feet = (-dx_feet * sin(rad)) + (dy_feet * cos(rad));
    double delta_lat = northing_feet / FEET_PER_DEG_LAT;
    double feet_per_deg_lon = FEET_PER_DEG_LAT * cos(ORIGIN_LAT * (PI / 180.0));
    double delta_lon = easting_feet / feet_per_deg_lon;

    wp.lat = ORIGIN_LAT + delta_lat;
    wp.lon = ORIGIN_LON + delta_lon;
}

void findPath() {
    if (grid.empty() || Start == -1) return;

    aStarPath.clear();
    pathState.clear();
    saltLevels.clear();
    batteryLevels.clear();
    fullMissionPlan.clear();

    int baseX = Start % GridWidth;
    int baseY = Start / GridWidth;

    currentSalt = MAX_SALT_LBS;
    currentBattery = MAX_BATTERY_CAPACITY;
    float safeRes = (resolution > 0) ? resolution : 2.5f;
    float lbsPerCell = (safeRes * safeRes) * 0.005f; 
    if (lbsPerCell < 0.001f) lbsPerCell = 0.001f;
    float safeBatteryThreshold = volPerCell * (GridWidth + GridLength) * 0.5f; 

    std::vector<bool> reachable(GridWidth * GridLength, false);
    std::vector<int> queue;
    
    int baseIndex = (baseY * GridWidth) + baseX;
    reachable[baseIndex] = true;
    queue.push_back(baseIndex);
    
    int qIdx = 0;
    int dx8[] = {0, 1, 0, -1, 1, -1, -1, 1}; 
    int dy8[] = {-1, 0, 1, 0, 1, 1, -1, -1};

    while (qIdx < queue.size()) {
        int currentIndex = queue[qIdx];
        int cx = currentIndex % GridWidth; 
        int cy = currentIndex / GridWidth; 
        qIdx++;

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx8[i];
            int ny = cy + dy8[i];
            
            if (nx >= 0 && nx < GridWidth && ny >= 0 && ny < GridLength) {
                int nIndex = (ny * GridWidth) + nx;
                if (!grid[nIndex].wall && !reachable[nIndex]) {
                    reachable[nIndex] = true;
                    queue.push_back(nIndex); 
                }
            }
        }
    }

    int wallPadding = std::max(robotX / 2, robotY / 2); 
    if (wallPadding < 1) wallPadding = 1; 

    std::vector<bool> paddedReachable = reachable;
    
    for (int y = 0; y < GridLength; y++) {
        for (int x = 0; x < GridWidth; x++) {
            int Index = (y * GridWidth) + x;
            if (reachable[Index]) {
                bool nearWall = false;
                for (int padY = -wallPadding; padY <= wallPadding; padY++) {
                    for (int padX = -wallPadding; padX <= wallPadding; padX++) {
                        int nx = x + padX;
                        int ny = y + padY;
                        int nIndex = (ny * GridWidth) + nx;
                        if (nx < 0 || ny < 0 || nx >= GridWidth || ny >= GridLength || grid[nIndex].wall) {
                            nearWall = true;
                            break;
                        }
                    }
                    if (nearWall) break;
                }
                if (nearWall) paddedReachable[Index] = false;
            }
        }
    }

    int unsaltedPaddedCount = 0;
    for (int y = 0; y < GridLength; y++) {
        for (int x = 0; x < GridWidth; x++) {
            int Index = (y * GridWidth) + x;
            if (paddedReachable[Index]) unsaltedPaddedCount++;
        }
    }

    auto hasLineOfSight = [&](int x0, int y0, int x1, int y1) {
        if (x0 == x1 && y0 == y1) return true; 

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        int safety = 0;
        while (safety++ < 10000) {
            if (x0 < 0 || x0 >= GridWidth || y0 < 0 || y0 >= GridLength) return false;
            
            int idx = (y0 * GridWidth) + x0;
            if (!reachable[idx]) return false; 

            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
        return true;
    };

    auto compressAndSavePath = [&](int startIndex, std::string action) {
        int aSize = (int)aStarPath.size(); 
        if (startIndex >= aSize || aSize - startIndex < 2) {
            if (startIndex < aSize) {
                Waypoint wp = {action, aStarPath.back()->x, aStarPath.back()->y, 0.0, 0.0, currentSalt, currentBattery};
                applyGPS(wp);
                fullMissionPlan.push_back(wp);
            }
            return;
        }
        
        std::vector<Cell*> rawSegment;
        for (int i = startIndex; i < aSize; i++) {
            rawSegment.push_back(aStarPath[i]);
        }
        
        std::vector<int> cornerIndices;
        cornerIndices.push_back(0); 
        
        int current_idx = 0;
        int rawSize = (int)rawSegment.size(); 
        
        int loopSafety = 0;
        while (current_idx < rawSize - 1 && loopSafety++ < 10000) {
            int furthest_safe = current_idx + 1;
            for (int i = current_idx + 2; i < rawSize; i++) {
                if (hasLineOfSight(rawSegment[current_idx]->x, rawSegment[current_idx]->y, rawSegment[i]->x, rawSegment[i]->y)) {
                    furthest_safe = i;
                } else {
                    break; 
                }
            }
            cornerIndices.push_back(furthest_safe);
            current_idx = furthest_safe;
        }

        for (size_t i = 1; i < cornerIndices.size(); i++) {
            Cell* c = rawSegment[cornerIndices[i]];
            Waypoint wp = {action, c->x, c->y, 0.0, 0.0, currentSalt, currentBattery};
            applyGPS(wp);
            fullMissionPlan.push_back(wp);
        }

        std::vector<Cell*> smoothedSegment;
        int cornerCount = (int)cornerIndices.size(); 
        
        for (int i = 0; i < cornerCount - 1; i++) {
            int idxA = cornerIndices[i];
            int idxB = cornerIndices[i+1];
            
            int x0 = rawSegment[idxA]->x;
            int y0 = rawSegment[idxA]->y;
            int x1 = rawSegment[idxB]->x;
            int y1 = rawSegment[idxB]->y;
            
            int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
            int err = dx + dy, e2;

            int lineSafety = 0;
            while (lineSafety++ < 10000) {
                if (x0 >= 0 && x0 < GridWidth && y0 >= 0 && y0 < GridLength) {
                    smoothedSegment.push_back(&grid[(y0 * GridWidth) + x0]);
                }
                if (x0 == x1 && y0 == y1) break;
                e2 = 2 * err;
                if (e2 >= dy) { err += dy; x0 += sx; }
                if (e2 <= dx) { err += dx; y0 += sy; }
            }

            if (i < cornerCount - 2 && !smoothedSegment.empty()) {
                smoothedSegment.pop_back();
            }
        }

        float startBatt = (startIndex > 0) ? batteryLevels[startIndex - 1] : currentBattery;
        
        aStarPath.erase(aStarPath.begin() + startIndex, aStarPath.end());
        pathState.erase(pathState.begin() + startIndex, pathState.end());
        saltLevels.erase(saltLevels.begin() + startIndex, saltLevels.end());
        batteryLevels.erase(batteryLevels.begin() + startIndex, batteryLevels.end());

        currentBattery = startBatt; 

        for (Cell* c : smoothedSegment) {
            aStarPath.push_back(c);
            pathState.push_back(currentPathType);
            currentBattery -= volPerCell;
            if (currentBattery < 0) currentBattery = 0.0f;
            saltLevels.push_back(currentSalt);
            batteryLevels.push_back(currentBattery);
        }
    };

    std::vector<int> salted(GridWidth * GridLength, 0);
    std::vector<bool> driven(GridWidth * GridLength, false);
    int currentPassID = 1; 

    auto getSprayBounds = [&](int cx, int cy, int dir, int& minX, int& maxX, int& minY, int& maxY) {
        int b = 1; 
        if (dir == 0) { minX = cx - spreadX; maxX = cx + spreadX; minY = cy - spreadY; maxY = cy + b; }
        else if (dir == 1) { minX = cx - spreadX - b; maxX = cx + spreadY; minY = cy - spreadY; maxY = cy + spreadX + b; }
        else if (dir == 2) { minX = cx - b; maxX = cx + spreadY; minY = cy - spreadX; maxY = cy + spreadX; }
        else if (dir == 3) { minX = cx - spreadX - b; maxX = cx + spreadY; minY = cy - spreadX - b; maxY = cy + spreadY; }
        else if (dir == 4) { minX = cx - spreadX; maxX = cx + spreadX; minY = cy - b; maxY = cy + spreadY; }
        else if (dir == 5) { minX = cx - spreadY; maxX = cx + spreadX + b; minY = cy - spreadX - b; maxY = cy + spreadY; }
        else if (dir == 6) { minX = cx - spreadY; maxX = cx + b; minY = cy - spreadX; maxY = cy + spreadX; }
        else if (dir == 7) { minX = cx - spreadY; maxX = cx + spreadX + b; minY = cy - spreadY; maxY = cy + spreadX + b; }
        else if (dir == 8) { 
            int capSize = std::max(spreadX, spreadY);
            minX = cx - capSize; maxX = cx + capSize; 
            minY = cy - capSize; maxY = cy + capSize; 
        }
    };

    auto markSalted = [&](int cx, int cy, int dir) {
        int saltDropped = 0;
        int minX, maxX, minY, maxY;
        getSprayBounds(cx, cy, dir, minX, maxX, minY, maxY);

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                if (x >= 0 && x < GridWidth && y >= 0 && y < GridLength) {
                    int Index = (y * GridWidth) + x;
                    if (reachable[Index] && salted[Index] == 0) { 
                        salted[Index] = currentPassID; 
                        saltDropped++;
                        if (paddedReachable[Index]) {
                            unsaltedPaddedCount--; 
                        }
                    }
                }
            }
        }
        return saltDropped;
    };

    int currentX = baseX;
    int currentY = baseY;
    int cIndex = (currentY * GridWidth) + currentX;
    driven[cIndex] = true; 
    
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1}; 
    int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

    markSalted(currentX, currentY, 0);

    currentPathType = 0; 
    aStarPath.push_back(&grid[cIndex]);
    pathState.push_back(currentPathType);
    saltLevels.push_back(currentSalt);
    batteryLevels.push_back(currentBattery);
    
    while (unsaltedPaddedCount > 0) {
        
        float maxPotentialUse = ((spreadX * 2 + 1) * (spreadY + 1)) * lbsPerCell;
        
        if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) {
            currentPathType = 1; 
            resetGridForNextRun();
            
            int prevSize = aStarPath.size();
            A_Star(currentX, currentY, baseX, baseY);
            compressAndSavePath(prevSize, "RTB");
            
            currentSalt = MAX_SALT_LBS;
            currentBattery = MAX_BATTERY_CAPACITY; 
            
            resetGridForNextRun();
            currentPathType = 2; 
            
            prevSize = aStarPath.size();
            A_Star(baseX, baseY, currentX, currentY);
            compressAndSavePath(prevSize, "TRANSIT");

            currentPathType = 0; 
            
            if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) {
                std::cout << "\nCRITICAL ERROR: The work zone is too far away. The robot burned through its entire battery just driving there! Terminating." << std::endl;
                break; 
            }
        }

        int maxLineScore = -1;
        int bestDir = -1;

        for (int dir = 0; dir < 8; dir++) {
            int tx = currentX;
            int ty = currentY;
            int score = 0; 
            int unsaltedInLine = 0;

            while (true) { 
                tx += dx[dir];
                ty += dy[dir];

                if (tx < 0 || tx >= GridWidth || ty < 0 || ty >= GridLength) break;
                
                int tIdx = (ty * GridWidth) + tx;

                if (!paddedReachable[tIdx] || driven[tIdx]) break;
                if (salted[tIdx] > 0 && salted[tIdx] != currentPassID) break;
                
                score++;
                if (salted[tIdx] == 0) unsaltedInLine++;
            }

            if (unsaltedInLine > 0 && score > maxLineScore) {
                maxLineScore = score;
                bestDir = dir;
            }
        }

        if (bestDir != -1 && maxLineScore > 0) {
            
            while (true) {
                if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) {
                    break; 
                }

                int nx = currentX + dx[bestDir];
                int ny = currentY + dy[bestDir];

                if (nx < 0 || nx >= GridWidth || ny < 0 || ny >= GridLength) break;
                
                int nIdx = (ny * GridWidth) + nx;

                if (!paddedReachable[nIdx] || driven[nIdx]) break;
                if (salted[nIdx] > 0 && salted[nIdx] != currentPassID) break;

                currentX = nx;
                currentY = ny;
                
                driven[(currentY * GridWidth) + currentX] = true; 

                int newlySalted = markSalted(currentX, currentY, bestDir);
                
                currentSalt -= (newlySalted * lbsPerCell);
                if (currentSalt < 0) currentSalt = 0.0f;
                currentBattery -= volPerCell;
                if (currentBattery < 0) currentBattery = 0.0f;

                aStarPath.push_back(&grid[(currentY * GridWidth) + currentX]);
                pathState.push_back(currentPathType);
                saltLevels.push_back(currentSalt);
                batteryLevels.push_back(currentBattery);
            }

            int capSalted = markSalted(currentX, currentY, 8);
            currentSalt -= (capSalted * lbsPerCell);
            if (currentSalt < 0) currentSalt = 0.0f;

            Waypoint wp = {"SALT_LINE_TO", currentX, currentY, 0.0, 0.0, currentSalt, currentBattery};
            applyGPS(wp);
            fullMissionPlan.push_back(wp);

        } else {
            int bestLosDist = -1;
            int bestLosX = -1;
            int bestLosY = -1;

            for (int y = 0; y < GridLength; y++) {
                for (int x = 0; x < GridWidth; x++) {
                    int idx = (y * GridWidth) + x;
                    if (paddedReachable[idx] && salted[idx] == 0) {
                        if (hasLineOfSight(currentX, currentY, x, y)) {
                            int dist = (x - currentX)*(x - currentX) + (y - currentY)*(y - currentY);
                            if (dist > bestLosDist && dist > 0) {
                                bestLosDist = dist;
                                bestLosX = x;
                                bestLosY = y;
                            }
                        }
                    }
                }
            }

            if (bestLosX != -1) {
                int x0 = currentX, y0 = currentY;
                int x1 = bestLosX, y1 = bestLosY;
                int los_dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
                int los_dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
                int err = los_dx + los_dy, e2;

                while (true) {
                    if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) break;

                    if (x0 >= 0 && x0 < GridWidth && y0 >= 0 && y0 < GridLength) {
                        currentX = x0;
                        currentY = y0;
                        driven[(currentY * GridWidth) + currentX] = true; 

                        int newlySalted = markSalted(currentX, currentY, 8); 
                        
                        currentSalt -= (newlySalted * lbsPerCell);
                        if (currentSalt < 0) currentSalt = 0.0f;
                        currentBattery -= volPerCell;
                        if (currentBattery < 0) currentBattery = 0.0f;

                        aStarPath.push_back(&grid[(currentY * GridWidth) + currentX]);
                        pathState.push_back(currentPathType); 
                        saltLevels.push_back(currentSalt);
                        batteryLevels.push_back(currentBattery);
                    }

                    if (x0 == x1 && y0 == y1) break;
                    e2 = 2 * err;
                    if (e2 >= los_dy) { err += los_dy; x0 += sx; }
                    if (e2 <= los_dx) { err += los_dx; y0 += sy; }
                }

                Waypoint wp = {"SALT_LINE_TO", currentX, currentY, 0.0, 0.0, currentSalt, currentBattery};
                applyGPS(wp);
                fullMissionPlan.push_back(wp);

            } else {
                // A* Fallback
                int bestDist = 999999;
                int bestX = -1, bestY = -1;

                for (int y = 0; y < GridLength; y++) {
                    for (int x = 0; x < GridWidth; x++) {
                        int idx = (y * GridWidth) + x;
                        
                        if (paddedReachable[idx] && salted[idx] == 0) {
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
                    currentPathType = 2; 
                    A_Star(currentX, currentY, bestX, bestY);
                    compressAndSavePath(prevSize, "TRANSIT");

                    if (aStarPath.size() > prevSize) {
                        currentX = bestX;
                        currentY = bestY;
                        int bIndex = (currentY * GridWidth) + currentX;
                        driven[bIndex] = true; 
                        currentPathType = 0; 
                        currentPassID++; 
                        markSalted(currentX, currentY, 0); 
                    } else {
                        salted[(bestY * GridWidth) + bestX] = currentPassID;
                        unsaltedPaddedCount--;
                        currentPathType = 0; 
                    }
                } else {
                    break;
                }
            }
        }
    }

    currentPathType = 1; 
    resetGridForNextRun();
    int prevSize = aStarPath.size();
    A_Star(currentX, currentY, baseX, baseY);
    compressAndSavePath(prevSize, "RTB");
}