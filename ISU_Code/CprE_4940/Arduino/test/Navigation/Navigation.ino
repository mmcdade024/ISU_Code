#include <vector>
#include <cmath>
#include <algorithm>

// --- MEMORY PROFILER FOR ARM CORTEX-M4 (Uno R4) ---
extern "C" char* sbrk(int incr);
int getFreeMemory() {
    char top;
    return &top - reinterpret_cast<char*>(sbrk(0));
}

// Stored in Flash memory to save RAM
const char* const mapLines[] = {
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    "######################################........#####",
    ".#####################################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..........############################........#####",
    "..............................................#####",
    "..............................................#####",
    "..............................................#####",
    "..............................................#####",
    "..............................................#####",
    "..............................................#####",
    "..............................................#####",
    "...........########################################",
    "...........########################################",
    "...........########################################",
    "...........########################################"
};

// --- GLOBAL VARIABLES ---
int Width = 0;
int Length = 0;

float MAX_SALT_LBS = 8.0f;
float MAX_BATTERY_CAPACITY = 10.0f;

float currentSalt = 8.0f;
float currentBattery = 10.0f;
float resolution = 2.0f; 
float lbsPerCell = 0.0f;
float volPerCell = 0.002f; 
float safeBatteryThreshold = 0.0f;

int spreadX = 2; 
int spreadY = 3; 
int robotX = 2;
int robotY = 2;

int baseX = 5;
int baseY = 45;

// GPS & Waypoint Formatting
double ORIGIN_LAT = 42.0266; 
double ORIGIN_LON = -93.6465;
double GRID_HEADING = 45.0; 
const double FEET_PER_DEG_LAT = 364000.0;

// High-Water Memory Tracker
int lowestFreeMem = 32768; 
void checkMem() {
    int currentMem = getFreeMemory();
    if (currentMem < lowestFreeMem) {
        lowestFreeMem = currentMem;
    }
}

// Flattened Grid for memory optimization
std::vector<bool> walls;

// Point stays lightweight for the A* Engine
struct Point {
  uint8_t x;
  uint8_t y;
};

// Waypoint holds the heavy GPS Data for the final output
struct Waypoint {
  const char* action;
  uint8_t x;
  uint8_t y;
  double lat;
  double lon;
};

// The global list to store the final commands
std::vector<Waypoint> missionPlan;

struct AStarNode {
    uint16_t idx;
    uint16_t g;
    uint16_t f;
};

// =========================================================================
// SETUP & LOOP
// =========================================================================
void setup() {
  Serial.begin(115200);
  delay(2000); 

  Serial.println(F("--- SALT BOT INITIALIZING ---"));
  
  Length = sizeof(mapLines) / sizeof(mapLines[0]);
  Width = strlen(mapLines[0]);

  Serial.print(F("Map Size: "));
  Serial.print(Width);
  Serial.print(F(" x "));
  Serial.println(Length);

  walls.assign(Width * Length, false);
  for (int y = 0; y < Length; y++) {
    for (int x = 0; x < Width; x++) {
      if (mapLines[y][x] == '#') {
        walls[y * Width + x] = true;
      }
    }
  }

  bool found = false;
  for (int y = 0; y < Length && !found; y++) {
    for (int x = 0; x < Width && !found; x++) {
      if (!walls[y * Width + x]) {
        baseX = x;
        baseY = y;
        found = true;
      }
    }
  }

  lbsPerCell = (resolution * resolution) * 0.005f;
  if (lbsPerCell < 0.001f) lbsPerCell = 0.001f;
  safeBatteryThreshold = volPerCell * (Width + Length) * 0.5f;

  Serial.println(F("-----------------------------------"));
  Serial.println(F("--- SYSTEM PERFORMANCE PROFILER ---"));
  Serial.println(F("Total Device SRAM: ~32768 Bytes (32 KB)"));
  Serial.print(F("Free SRAM Before Run: "));
  Serial.print(getFreeMemory());
  Serial.println(F(" Bytes"));
  Serial.println(F("Starting Path Generation..."));
  Serial.println(F("-----------------------------------"));

  unsigned long startTime = millis();
  
  findPath();

  unsigned long runTime = millis() - startTime;

  // --- PRINT ALL THE POINTS ---
  Serial.println(F("\n=== MISSION PLAN WAYPOINTS ==="));
  for (size_t i = 0; i < missionPlan.size(); i++) {
      Serial.print(F("Step "));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(missionPlan[i].action);
      Serial.print(F(" | Grid: ("));
      Serial.print(missionPlan[i].x);
      Serial.print(F(", "));
      Serial.print(missionPlan[i].y);
      Serial.print(F(") | GPS: ["));
      Serial.print(missionPlan[i].lat, 6);
      Serial.print(F(", "));
      Serial.print(missionPlan[i].lon, 6);
      Serial.println(F("]"));
  }
  Serial.print(F("Total Waypoints: "));
  Serial.println(missionPlan.size());
  Serial.println(F("==============================\n"));

  Serial.println(F("-----------------------------------"));
  Serial.println(F("JOB COMPLETE. WAITING AT BASE."));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("--- FINAL DIAGNOSTICS ---"));
  Serial.print(F("Algorithm Runtime: "));
  Serial.print(runTime);
  Serial.println(F(" ms"));
  
  Serial.print(F("Peak RAM Consumed: "));
  Serial.print(32768 - lowestFreeMem);
  Serial.println(F(" Bytes"));
  Serial.println(F("-----------------------------------"));
}

// =========================================================================
// A Star
// =========================================================================
std::vector<Point> A_Star(int sX, int sY, int gX, int gY) {
    std::vector<Point> path;
    if (sX == gX && sY == gY) return path;

    int numCells = Width * Length;

    int8_t* cameFrom = new int8_t[numCells]; 
    for(int i = 0; i < numCells; i++) cameFrom[i] = -1;
    
    checkMem(); 
    
    std::vector<bool> visited(numCells, false);
    
    std::vector<AStarNode> openSet;
    openSet.reserve(500); 
    
    int startIdx = sY * Width + sX;
    
    int dx_diff = abs(sX - gX);
    int dy_diff = abs(sY - gY);
    uint16_t start_h = (dx_diff > dy_diff) ? (14 * dy_diff + 10 * (dx_diff - dy_diff)) : (14 * dx_diff + 10 * (dy_diff - dx_diff));

    openSet.push_back({(uint16_t)startIdx, 0, start_h});
    visited[startIdx] = true;

    int dx[] = {1, -1, 0, 0, 1, -1, -1, 1};
    int dy[] = {0, 0, 1, -1, -1, -1, 1, 1};

    bool found = false;

    while (!openSet.empty()) {
        int bestIdx = 0;
        for (size_t i = 1; i < openSet.size(); i++) {
            if (openSet[i].f < openSet[bestIdx].f) bestIdx = i;
        }

        AStarNode current = openSet[bestIdx];
        int cx = current.idx % Width;
        int cy = current.idx / Width;

        if (cx == gX && cy == gY) {
            found = true;
            break;
        }

        openSet[bestIdx] = openSet.back();
        openSet.pop_back();

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < Width && ny >= 0 && ny < Length) {
                uint16_t nIdx = ny * Width + nx;

                if (walls[nIdx] || visited[nIdx]) continue;

                uint16_t moveCost = (i < 4) ? 10 : 14;
                uint16_t ten_g = current.g + moveCost;
                
                int hx = abs(nx - gX);
                int hy = abs(ny - gY);
                uint16_t h = (hx > hy) ? (14 * hy + 10 * (hx - hy)) : (14 * hx + 10 * (hy - hx));
                uint16_t ten_f = ten_g + h;

                visited[nIdx] = true; 
                cameFrom[nIdx] = i; 
                
                openSet.push_back({nIdx, ten_g, ten_f});
            }
        }
    }

    if (found) {
        int currX = gX;
        int currY = gY;
        
        while (currX != sX || currY != sY) {
            path.push_back({(uint8_t)currX, (uint8_t)currY});
            int dir = cameFrom[currY * Width + currX];
            
            if (dir == -1) break; 
            
            currX -= dx[dir];
            currY -= dy[dir];
        }
        std::reverse(path.begin(), path.end());
    }

    delete[] cameFrom; 
    return path;
}

// =========================================================================
// HELPER PRINTER & GPS CALCULATOR
// =========================================================================
// --- Saves to the Mission Plan Vector instead of printing instantly
void saveWaypoint(const char* action, int x, int y) {
  double lat = 0.0;
  double lon = 0.0;

  if (ORIGIN_LAT != 0.0) {
      double dx_cells = x - baseX;
      double dy_cells = baseY - y; 
      
      double dx_feet = dx_cells * resolution;
      double dy_feet = dy_cells * resolution;
      
      double rad = GRID_HEADING * (PI / 180.0);
      double easting_feet  = (dx_feet * cos(rad)) + (dy_feet * sin(rad));
      double northing_feet = (-dx_feet * sin(rad)) + (dy_feet * cos(rad));
      
      double delta_lat = northing_feet / FEET_PER_DEG_LAT;
      double feet_per_deg_lon = FEET_PER_DEG_LAT * cos(ORIGIN_LAT * (PI / 180.0));
      double delta_lon = easting_feet / feet_per_deg_lon;
      
      lat = ORIGIN_LAT + delta_lat;
      lon = ORIGIN_LON + delta_lon;
  }

  missionPlan.push_back({action, (uint8_t)x, (uint8_t)y, lat, lon});
}

// =========================================================================
// MAIN
// =========================================================================
void findPath() {

    int numCells = Width * Length;
    std::vector<bool> reachable(numCells, false);
    std::vector<Point> queue;
    
    reachable[baseY * Width + baseX] = true;
    queue.push_back({(uint8_t)baseX, (uint8_t)baseY});
    
    checkMem();

    int qIdx = 0;
    int dx8[] = {0, 1, 0, -1, 1, -1, -1, 1};
    int dy8[] = {-1, 0, 1, 0, 1, 1, -1, -1};

    while (qIdx < queue.size()) {
        int cx = queue[qIdx].x;
        int cy = queue[qIdx].y;
        qIdx++;

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx8[i];
            int ny = cy + dy8[i];
            if (nx >= 0 && nx < Width && ny >= 0 && ny < Length) {
                int nIdx = ny * Width + nx;
                if (!walls[nIdx] && !reachable[nIdx]) {
                    reachable[nIdx] = true;
                    queue.push_back({(uint8_t)nx, (uint8_t)ny});
                }
            }
        }
    }

    queue.clear();
    queue.shrink_to_fit(); 

    int wallPadding = std::max(robotX / 2, robotY / 2);
    if (wallPadding < 1) wallPadding = 1; 

    std::vector<bool> paddedReachable = reachable;
    int unsaltedPaddedCount = 0;

    for (int y = 0; y < Length; y++) {
        for (int x = 0; x < Width; x++) {
            int idx = y * Width + x;
            if (reachable[idx]) {
                bool nearWall = false;
                for (int padY = -wallPadding; padY <= wallPadding; padY++) {
                    for (int padX = -wallPadding; padX <= wallPadding; padX++) {
                        int nx = x + padX;
                        int ny = y + padY;
                        if (nx < 0 || ny < 0 || nx >= Width || ny >= Length || walls[ny * Width + nx]) {
                            nearWall = true;
                            break;
                        }
                    }
                    if (nearWall) break;
                }
                if (nearWall) {
                    paddedReachable[idx] = false;
                } else {
                    unsaltedPaddedCount++;
                }
            }
        }
    }

    // ==========================================
    auto hasLineOfSight = [&](int x0, int y0, int x1, int y1) {
        if (x0 == x1 && y0 == y1) return true;

        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        int safety = 0;
        while (safety++ < 10000) {
            if (x0 < 0 || x0 >= Width || y0 < 0 || y0 >= Length) return false;
            
            int idx = y0 * Width + x0;
            if (!reachable[idx]) return false; 

            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
        return true;
    };

    auto printCompressedPath = [&](const std::vector<Point>& pth, const char* action) {
        int pthSize = (int)pth.size(); 
        
        if (pthSize == 0) return;
        if (pthSize == 1) {
            saveWaypoint(action, pth[0].x, pth[0].y);
            return;
        }
        
        int current_idx = 0;
        int loopSafety = 0;
        
        while (current_idx < pthSize - 1 && loopSafety++ < 10000) {
            int furthest_safe = current_idx + 1;
            for (int i = current_idx + 2; i < pthSize; i++) {
                if (hasLineOfSight(pth[current_idx].x, pth[current_idx].y, pth[i].x, pth[i].y)) {
                    furthest_safe = i;
                } else {
                    break;
                }
            }
            saveWaypoint(action, pth[furthest_safe].x, pth[furthest_safe].y);
            current_idx = furthest_safe;
        }
    };
    // ==========================================


    std::vector<uint8_t> salted(numCells, 0);
    uint8_t currentPassID = 1; 
    std::vector<bool> driven(numCells, false); 

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
                if (x >= 0 && x < Width && y >= 0 && y < Length) {
                    int idx = y * Width + x;
                    if (reachable[idx] && salted[idx] == 0) { 
                        salted[idx] = currentPassID;
                        saltDropped++;
                        if (paddedReachable[idx]) {
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
    driven[currentY * Width + currentX] = true;
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

    markSalted(currentX, currentY, 0);
    saveWaypoint("START", currentX, currentY);

    while (unsaltedPaddedCount > 0) {
        
        float maxPotentialUse = ((spreadX * 2 + 1) * (spreadY + 1)) * lbsPerCell;
        if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) {
            
            std::vector<Point> returnPath = A_Star(currentX, currentY, baseX, baseY);
            printCompressedPath(returnPath, "RTB");
            
            currentSalt = MAX_SALT_LBS; 
            currentBattery = MAX_BATTERY_CAPACITY; 
            
            std::vector<Point> transitPath = A_Star(baseX, baseY, currentX, currentY);
            printCompressedPath(transitPath, "TRANSIT");

            if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) {
                 Serial.println(F("CRITICAL: Zone unreachable with current battery! Terminating."));
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

                if (tx < 0 || tx >= Width || ty < 0 || ty >= Length) break;
                int idx = ty * Width + tx;
                
                if (!paddedReachable[idx]) break;
                if (salted[idx] > 0 && salted[idx] != currentPassID) break;
                score++;
                if (salted[idx] == 0) unsaltedInLine++;
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

                if (nx < 0 || nx >= Width || ny < 0 || ny >= Length) break;
                int idx = ny * Width + nx;

                if (!paddedReachable[idx]) break;
                if (salted[idx] > 0 && salted[idx] != currentPassID) break;
                currentX = nx;
                currentY = ny;

                int newlySalted = markSalted(currentX, currentY, bestDir);
                
                currentSalt -= (newlySalted * lbsPerCell);
                if (currentSalt < 0) currentSalt = 0.0f;
                currentBattery -= volPerCell;
                if (currentBattery < 0) currentBattery = 0.0f;
            }
            
            int capSalted = markSalted(currentX, currentY, 8);
            currentSalt -= (capSalted * lbsPerCell);
            if (currentSalt < 0) currentSalt = 0.0f;
            
            saveWaypoint("SALT_LINE_TO", currentX, currentY);
        } else {
            // --- Line-Of-Sight Diagonal Sweep ---
            int bestLosDist = -1;
            int bestLosX = -1;
            int bestLosY = -1;

            for (int y = 0; y < Length; y++) {
                for (int x = 0; x < Width; x++) {
                    int idx = y * Width + x;
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
                int los_dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
                int los_dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
                int err = los_dx + los_dy, e2;

                while (true) {
                    if (currentSalt < maxPotentialUse || currentBattery < safeBatteryThreshold) break;

                    if (x0 >= 0 && x0 < Width && y0 >= 0 && y0 < Length) {
                        currentX = x0;
                        currentY = y0;
                        
                        int newlySalted = markSalted(currentX, currentY, 8); 
                        currentSalt -= (newlySalted * lbsPerCell);
                        if (currentSalt < 0) currentSalt = 0.0f;
                        currentBattery -= volPerCell;
                        if (currentBattery < 0) currentBattery = 0.0f;
                    }

                    if (x0 == x1 && y0 == y1) break;
                    e2 = 2 * err;
                    if (e2 >= los_dy) { err += los_dy; x0 += sx; }
                    if (e2 <= los_dx) { err += los_dx; y0 += sy; }
                }
                
                saveWaypoint("SALT_LINE_TO", currentX, currentY);

            } else {
                // A* Fallback
                int bestDist = 999999;
                int bestX = -1, bestY = -1;

                for (int y = 0; y < Length; y++) {
                    for (int x = 0; x < Width; x++) {
                        int idx = y * Width + x;
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
                    std::vector<Point> jumpPath = A_Star(currentX, currentY, bestX, bestY);
                    printCompressedPath(jumpPath, "TRANSIT");

                    if (jumpPath.size() > 0 || (currentX == bestX && currentY == bestY)) {
                        currentX = bestX;
                        currentY = bestY;
                        currentPassID++; 
                        markSalted(currentX, currentY, 0); 
                    } else {
                        salted[bestY * Width + bestX] = currentPassID;
                        unsaltedPaddedCount--;
                    }
                } else {
                    break;
                }
            }
        }
    }

    std::vector<Point> finalPath = A_Star(currentX, currentY, baseX, baseY);
    printCompressedPath(finalPath, "RTB");
}

void loop() {
  delay(1000);
}