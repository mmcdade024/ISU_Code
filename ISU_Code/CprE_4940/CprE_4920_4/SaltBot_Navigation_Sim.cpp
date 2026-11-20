#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <optional>
#include <iostream>
#include "Navigation.h"

// Bring in the new battery variables from Navigation.cpp
extern std::vector<float> batteryLevels;
extern float MAX_BATTERY_CAPACITY;

int CELL_SIZE = 30; 
float resolution = 0.0f; 
namespace fs = std::filesystem;

void loadGrid(std::string grid_path, int& out_w, int& out_h, int& out_startX, int& out_startY) {
    if (!fs::exists(grid_path)) {
        std::cout << "CRITICAL ERROR: Grid file does not exist at " << grid_path << std::endl;
        exit(1);
    }

    std::ifstream file(grid_path);
    std::vector<std::string> lines;
    std::string line;

    if (file.is_open()) {
        bool firstLine = true; 
        while (std::getline(file, line)) {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            
            if (!line.empty()) {
                if (firstLine && line.find("RESOLUTION:") == 0) {
                    std::string resStr = line.substr(11);
                    resolution = std::stof(resStr);
                    firstLine = false;
                    continue;
                }
                firstLine = false;
                lines.push_back(line);
            }
        }
        file.close();
    }

    if (lines.empty()) {
        std::cout << "CRITICAL ERROR: Grid file is empty!" << std::endl;
        exit(1);
    }

    if (createGrid(lines) == -1) {
        std::cout << "CRITICAL ERROR: Failed to create grid. Map may be invalid." << std::endl;
        exit(1);
    }

    out_w = GridWidth;
    out_h = GridLength;
    out_startX = (Start == -1) ? 0 : Start % GridWidth;
    out_startY = (Start == -1) ? 0 : Start / GridWidth;
}

int main() {
    int gridSelect = 0;
    std::vector<std::string> gridFiles;
    std::string path = "C:\\School\\ISU\\CPRE\\CprE_4920_01\\Grids\\";
    std::string screenshotPath = "C:\\School\\ISU\\CPRE\\CprE_4920_01\\Screenshots\\";

    if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            gridFiles.push_back(entry.path().filename().string());
        }
    } else {
        std::cout << "ERROR: Grids directory not found at " << path << std::endl;
        return 1;
    }
    
    if (gridFiles.empty()) {
        std::cout << "ERROR: No .txt files found!" << std::endl;
        return 1;
    }

    std::cout << "\n This is the list of grids. Select one:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    for (int i = 0; i < gridFiles.size(); i++) {
        std::cout << i + 1 << ".) " << gridFiles[i] << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;

    while (true) {
        std::cout << "Enter which grid you would like to traverse: ";
        std::cin >> gridSelect;
        if (gridSelect <= gridFiles.size() && gridSelect > 0) break;
        std::cout << "Error: Invalid selection.\n";
    }

    int gridW = 0, gridH = 0;
    int startX = 0, startY = 0;
    std::string selectedFileName = gridFiles[gridSelect - 1];
    std::string selectedFile = path + selectedFileName; 
    
    loadGrid(selectedFile, gridW, gridH, startX, startY);

    std::string baseName = selectedFileName.substr(0, selectedFileName.find_last_of("."));
    sf::Texture bgTexture;
    bool hasBg = false;
    
    if (bgTexture.loadFromFile(screenshotPath + baseName + ".png")) hasBg = true;
    else if (bgTexture.loadFromFile(screenshotPath + baseName + ".jpg")) hasBg = true;
    
    float TargetViewHeight = 800.0f;
    float TargetViewWidth = 1400.0f; 
    
    CELL_SIZE = std::min((int)(TargetViewHeight / gridH), (int)(TargetViewWidth / (gridW * 2)));
    if (CELL_SIZE < 1) CELL_SIZE = 1; 

    float TOP_PADDING = 50.0f; 
    float BOTTOM_PADDING = 20.0f;  
    float LEFT_PANEL_W = 140.0f; 
    float RIGHT_PANEL_W = 380.0f; 
    float GAP_BETWEEN_MAPS = 10.0f;

    float GRID_AREA_W = gridW * CELL_SIZE;
    float GRID_AREA_H = gridH * CELL_SIZE;
    
    float GRID_OFFSET_X_LEFT = LEFT_PANEL_W; 
    float GRID_OFFSET_X_RIGHT = LEFT_PANEL_W + GRID_AREA_W + GAP_BETWEEN_MAPS; 

    unsigned int WINDOW_W = LEFT_PANEL_W + (GRID_AREA_W * 2) + GAP_BETWEEN_MAPS + RIGHT_PANEL_W;
    unsigned int WINDOW_H = std::max(600.0f, TOP_PADDING + GRID_AREA_H + BOTTOM_PADDING); 

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_W, WINDOW_H)), "SaltBot Ground Control Station");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "CRITICAL ERROR: Windows Arial font missing!\n";
        exit(1); 
    }

    std::optional<sf::Sprite> bgSprite;
    if (hasBg) {
        bgSprite.emplace(bgTexture); 
        float scaleX = (float)GRID_AREA_W / bgTexture.getSize().x;
        float scaleY = (float)GRID_AREA_H / bgTexture.getSize().y;
        bgSprite->setScale(sf::Vector2f(scaleX, scaleY));
        bgSprite->setPosition(sf::Vector2f(GRID_OFFSET_X_LEFT, TOP_PADDING)); 
    }

    float drawSize = (float)CELL_SIZE; 
    float robotSize = (CELL_SIZE > 2) ? (CELL_SIZE / 2.0f - 1.0f) : 1.0f;

    sf::RectangleShape rect(sf::Vector2f(drawSize, drawSize)); 
    sf::CircleShape robot(robotSize);
    robot.setFillColor(sf::Color::White);
    robot.setOutlineThickness(1);
    robot.setOutlineColor(sf::Color::Black);

    bool showPaths = true;
    bool showSalted = true;
    bool showUpcoming = true;
    bool showReturn = true;
    bool showTransit = true; 
    bool showDrivenLine = true; // NEW TOGGLE

    bool hKeyPressed = false;
    bool sKeyPressed = false;
    bool nKeyPressed = false;
    bool bKeyPressed = false;
    bool tKeyPressed = false; 
    bool dKeyPressed = false; // NEW TOGGLE KEY

    int totalPathSize = 0; 
    int pathIndex = 0;

    enum State { WAITING_FOR_START, MOVING };
    State currentState = WAITING_FOR_START; 

    // FLATTENED VISUAL ARRAY
    std::vector<bool> visualSalted(GridWidth * GridLength, false);
    int lastVisualIndex = 0;

    auto getVisualSprayBounds = [&](int cx, int cy, int dir, int& minX, int& maxX, int& minY, int& maxY) {
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

    // --- LEFT PANEL: TRIPLE GAUGES ---
    float barMaxHeight = WINDOW_H - TOP_PADDING - BOTTOM_PADDING - 60.0f;
    
    sf::Text jobLabel(font, "JOB", 14);
    jobLabel.setPosition(sf::Vector2f(10.0f, TOP_PADDING + 10.0f));
    jobLabel.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape progressBG(sf::Vector2f(20.0f, barMaxHeight));
    progressBG.setPosition(sf::Vector2f(15.0f, TOP_PADDING + 30.0f));
    progressBG.setFillColor(sf::Color(50, 50, 50));
    progressBG.setOutlineThickness(2);
    progressBG.setOutlineColor(sf::Color(200, 200, 200));

    sf::RectangleShape progressFill(sf::Vector2f(20.0f, 0.0f));
    progressFill.setFillColor(sf::Color(0, 255, 0));

    sf::Text tankLabel(font, "SALT", 14);
    tankLabel.setPosition(sf::Vector2f(50.0f, TOP_PADDING + 10.0f));
    tankLabel.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape saltBG(sf::Vector2f(20.0f, barMaxHeight));
    saltBG.setPosition(sf::Vector2f(55.0f, TOP_PADDING + 30.0f));
    saltBG.setFillColor(sf::Color(50, 50, 50));
    saltBG.setOutlineThickness(2);
    saltBG.setOutlineColor(sf::Color(200, 200, 200));

    sf::RectangleShape saltFill(sf::Vector2f(20.0f, 0.0f));
    saltFill.setFillColor(sf::Color(0, 191, 255)); 

    sf::Text battLabel(font, "BATT", 14);
    battLabel.setPosition(sf::Vector2f(90.0f, TOP_PADDING + 10.0f));
    battLabel.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape batteryBG(sf::Vector2f(20.0f, barMaxHeight));
    batteryBG.setPosition(sf::Vector2f(95.0f, TOP_PADDING + 30.0f));
    batteryBG.setFillColor(sf::Color(50, 50, 50));
    batteryBG.setOutlineThickness(2);
    batteryBG.setOutlineColor(sf::Color(200, 200, 200));

    sf::RectangleShape batteryFill(sf::Vector2f(20.0f, 0.0f));
    batteryFill.setFillColor(sf::Color::Yellow);

    auto createText = [&](std::string str, float yPos, int size=18) {
        sf::Text text(font, str, size);
        text.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 40.0f, yPos));
        text.setFillColor(sf::Color::White);
        return text;
    };

    auto createSwatch = [&](sf::Color color, float yPos) {
        sf::RectangleShape swatch(sf::Vector2f(25.0f, 25.0f));
        swatch.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 5.0f, yPos));
        swatch.setFillColor(color);
        return swatch;
    };

    sf::Text legendTitle(font, "SYSTEM STATUS", 24);
    legendTitle.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 5.0f, TOP_PADDING));
    legendTitle.setStyle(sf::Text::Underlined | sf::Text::Bold);

    sf::Text saltDataText(font, "Salt Tank: 8.00 / 8.00 lbs", 18);
    saltDataText.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 5.0f, TOP_PADDING + 80.0f));
    saltDataText.setFillColor(sf::Color(0, 191, 255));
    saltDataText.setStyle(sf::Text::Bold);

    sf::Text batteryDataText(font, "Battery: 10.00 / 10.00 U", 18);
    batteryDataText.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 5.0f, TOP_PADDING + 110.0f));
    batteryDataText.setFillColor(sf::Color::Yellow);
    batteryDataText.setStyle(sf::Text::Bold);

    sf::Text toggleText(font, "TOGGLE CONTROLS:\n[H] Show/Hide All Lines\n[N] Toggle Upcoming\n[S] Toggle Salted\n[B] Toggle Base Return\n[T] Toggle Transit\n[D] Toggle Driven Track", 16);
    toggleText.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 5.0f, WINDOW_H - BOTTOM_PADDING - 130.0f));
    toggleText.setFillColor(sf::Color(200, 200, 200));

    sf::Text headerLeft(font, "SATELLITE GROUND TRUTH", 14);
    headerLeft.setPosition(sf::Vector2f(GRID_OFFSET_X_LEFT, TOP_PADDING - 20.0f));
    headerLeft.setFillColor(sf::Color(150, 150, 150));

    sf::Text headerRight(font, "ALGORITHM ABSTRACT VIEW", 14);
    headerRight.setPosition(sf::Vector2f(GRID_OFFSET_X_RIGHT, TOP_PADDING - 20.0f));
    headerRight.setFillColor(sf::Color(150, 150, 150));

    sf::RectangleShape baseStationIcon(sf::Vector2f(CELL_SIZE * 1.5f, CELL_SIZE * 1.5f));
    baseStationIcon.setFillColor(sf::Color(50, 205, 50)); 
    baseStationIcon.setOutlineThickness(2);
    baseStationIcon.setOutlineColor(sf::Color::White);
    baseStationIcon.setOrigin(sf::Vector2f((CELL_SIZE * 1.5f)/2, (CELL_SIZE * 1.5f)/2));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (currentState == WAITING_FOR_START) {
                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseBtn->button == sf::Mouse::Button::Left) {
                        float mx = mouseBtn->position.x;
                        float my = mouseBtn->position.y;
                        
                        int gridX = -1, gridY = -1;
                        
                        if (mx >= GRID_OFFSET_X_LEFT && mx < GRID_OFFSET_X_LEFT + GRID_AREA_W &&
                            my >= TOP_PADDING && my < TOP_PADDING + GRID_AREA_H) {
                            gridX = (mx - GRID_OFFSET_X_LEFT) / CELL_SIZE;
                            gridY = (my - TOP_PADDING) / CELL_SIZE;
                        }
                        else if (mx >= GRID_OFFSET_X_RIGHT && mx < GRID_OFFSET_X_RIGHT + GRID_AREA_W &&
                                 my >= TOP_PADDING && my < TOP_PADDING + GRID_AREA_H) {
                            gridX = (mx - GRID_OFFSET_X_RIGHT) / CELL_SIZE;
                            gridY = (my - TOP_PADDING) / CELL_SIZE;
                        }

                        if (gridX >= 0 && gridY >= 0 && gridX < GridWidth && gridY < GridLength) {
                            bool validClick = false;
                            
                            // 1D MATH UPDATE
                            if (!grid[(gridY * GridWidth) + gridX].wall) {
                                validClick = true;
                            } else {
                                for (int r = 1; r <= 5 && !validClick; r++) {
                                    for (int dx = -r; dx <= r && !validClick; dx++) {
                                        for (int dy = -r; dy <= r && !validClick; dy++) {
                                            int nx = gridX + dx;
                                            int ny = gridY + dy;
                                            if (nx >= 0 && ny >= 0 && nx < GridWidth && ny < GridLength && !grid[(ny * GridWidth) + nx].wall) {
                                                gridX = nx;
                                                gridY = ny;
                                                validClick = true;
                                            }
                                        }
                                    }
                                }
                            }

                            if (validClick) {
                                std::cout << "Setting start...";
                                setStart(gridX, gridY);
                                spreadX = 2;
                                spreadY = 3;
                                findPath();

                                std::cout << "\n=== MISSION PLAN WAYPOINTS ===\n";
                                for (size_t i = 0; i < fullMissionPlan.size(); i++) {
                                    std::cout << "Step " << i << ": " << fullMissionPlan[i].action 
                                              << " | Grid: (" << fullMissionPlan[i].x << ", " << fullMissionPlan[i].y << ") "
                                              // Force standard decimal format with exactly 6 places for GPS
                                              << std::fixed << std::setprecision(6) 
                                              << "| GPS: [" << fullMissionPlan[i].lat << ", " << fullMissionPlan[i].lon << "]\n";
                                }
                                std::cout << "Total Waypoints: " << fullMissionPlan.size() << "\n==============================\n";

                                totalPathSize = aStarPath.size();
                                pathIndex = 0; 
                                
                                // 1D MATH UPDATE
                                visualSalted.assign(GridWidth * GridLength, false);
                                lastVisualIndex = 0;
                                
                                if (!aStarPath.empty() && pathState[0] == 0) {
                                    int minX, maxX, minY, maxY;
                                    getVisualSprayBounds(aStarPath[0]->x, aStarPath[0]->y, 0, minX, maxX, minY, maxY);
                                    for (int sx = minX; sx <= maxX; sx++) {
                                        for (int sy = minY; sy <= maxY; sy++) {
                                            // 1D MATH UPDATE
                                            if (sx >= 0 && sx < GridWidth && sy >= 0 && sy < GridLength && !grid[(sy * GridWidth) + sx].wall) {
                                                visualSalted[(sy * GridWidth) + sx] = true;
                                            }
                                        }
                                    }
                                }
                                currentState = MOVING; 
                            }
                        }
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
                if (!hKeyPressed) { showPaths = !showPaths; hKeyPressed = true; }
            } else hKeyPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                if (!sKeyPressed) { showSalted = !showSalted; sKeyPressed = true; }
            } else sKeyPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
                if (!nKeyPressed) { showUpcoming = !showUpcoming; nKeyPressed = true; }
            } else nKeyPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
                if (!bKeyPressed) { showReturn = !showReturn; bKeyPressed = true; }
            } else bKeyPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
                if (!tKeyPressed) { showTransit = !showTransit; tKeyPressed = true; }
            } else tKeyPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                if (!dKeyPressed) { showDrivenLine = !showDrivenLine; dKeyPressed = true; }
            } else dKeyPressed = false;
        }

        int speedMultiplier = 4; // Fast forward so you can watch it paint!

        if (currentState == MOVING && pathIndex < totalPathSize - 1) {
            pathIndex = std::min(totalPathSize - 1, pathIndex + speedMultiplier);
            
            while (lastVisualIndex < pathIndex) {
                lastVisualIndex++;
                int i = lastVisualIndex;
                if (pathState[i] == 0) { 
                    int cx = aStarPath[i]->x;
                    int cy = aStarPath[i]->y;
                    int px = aStarPath[i-1]->x;
                    int py = aStarPath[i-1]->y;
                    int dx = cx - px;
                    int dy = cy - py;

                    if (dx > 0) dx = 1; else if (dx < 0) dx = -1;
                    if (dy > 0) dy = 1; else if (dy < 0) dy = -1;

                    int dir = 0; 
                    if (dx == 0 && dy == -1) dir = 0;
                    else if (dx == 1 && dy == -1) dir = 1;
                    else if (dx == 1 && dy == 0) dir = 2;
                    else if (dx == 1 && dy == 1) dir = 3;
                    else if (dx == 0 && dy == 1) dir = 4;
                    else if (dx == -1 && dy == 1) dir = 5;
                    else if (dx == -1 && dy == 0) dir = 6;
                    else if (dx == -1 && dy == -1) dir = 7;

                    int minX, maxX, minY, maxY;
                    getVisualSprayBounds(cx, cy, dir, minX, maxX, minY, maxY);

                    for (int sx = minX; sx <= maxX; sx++) {
                        for (int sy = minY; sy <= maxY; sy++) {
                            // 1D MATH UPDATE
                            if (sx >= 0 && sx < GridWidth && sy >= 0 && sy < GridLength && !grid[(sy * GridWidth) + sx].wall) {
                                visualSalted[(sy * GridWidth) + sx] = true; 
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(20, 20, 20)); 

        window.draw(headerLeft);
        window.draw(headerRight);

        if (hasBg && bgSprite.has_value()) {
            window.draw(*bgSprite);
            for (int x = 0; x < GridWidth; x++) {
                for (int y = 0; y < GridLength; y++) {
                    rect.setPosition(sf::Vector2f(x * CELL_SIZE + GRID_OFFSET_X_LEFT, y * CELL_SIZE + TOP_PADDING));
                    rect.setFillColor(sf::Color::Transparent);
                    rect.setOutlineThickness(1);
                    rect.setOutlineColor(sf::Color(255, 255, 255, 30)); 
                    window.draw(rect);
                }
            }
        }

        for (int x = 0; x < GridWidth; x++) {
            for (int y = 0; y < GridLength; y++) {
                rect.setPosition(sf::Vector2f(x * CELL_SIZE + GRID_OFFSET_X_RIGHT, y * CELL_SIZE + TOP_PADDING));
                // 1D MATH UPDATE
                if (grid[(y * GridWidth) + x].wall) rect.setFillColor(sf::Color(34, 139, 34)); 
                else rect.setFillColor(sf::Color(105, 105, 105)); 
                rect.setOutlineThickness(1);
                rect.setOutlineColor(sf::Color(255, 255, 255, 30)); 
                window.draw(rect);
            }
        }

        rect.setOutlineThickness(0); 

        // 1. Draw the massive Blue Spray regions first
        if (showPaths && showSalted && currentState == MOVING) {
            sf::Color saltColor = sf::Color(0, 191, 255, 180); 
            rect.setFillColor(saltColor);
            for (int x = 0; x < GridWidth; x++) {
                for (int y = 0; y < GridLength; y++) {
                    // 1D MATH UPDATE
                    if (visualSalted[(y * GridWidth) + x]) {
                        rect.setPosition(sf::Vector2f(x * CELL_SIZE + GRID_OFFSET_X_LEFT, y * CELL_SIZE + TOP_PADDING));
                        window.draw(rect);
                        rect.setPosition(sf::Vector2f(x * CELL_SIZE + GRID_OFFSET_X_RIGHT, y * CELL_SIZE + TOP_PADDING));
                        window.draw(rect);
                    }
                }
            }
        }

        // 2. We skip drawing the individual blocks for the driven tracks 
        if (showPaths && currentState == MOVING) {
            for (int i = pathIndex; i < totalPathSize; i++) { // Only draw Upcoming blocks here
                Cell* p = aStarPath[i];
                if (pathState[i] == 0 && showUpcoming) {
                    rect.setPosition(sf::Vector2f(p->x * CELL_SIZE + GRID_OFFSET_X_LEFT, p->y * CELL_SIZE + TOP_PADDING));
                    rect.setFillColor(sf::Color(255, 255, 0, 180)); 
                    window.draw(rect);

                    rect.setPosition(sf::Vector2f(p->x * CELL_SIZE + GRID_OFFSET_X_RIGHT, p->y * CELL_SIZE + TOP_PADDING));
                    rect.setFillColor(sf::Color(255, 255, 0, 180)); 
                    window.draw(rect);
                }
            }
        }

        // 3. The Master Tracking Lines
        if (showPaths && showDrivenLine && currentState == MOVING) {
            sf::VertexArray masterLineLeft(sf::PrimitiveType::LineStrip);
            sf::VertexArray masterLineRight(sf::PrimitiveType::LineStrip);

            for (int i = 0; i <= pathIndex && i < totalPathSize; i++) {
                Cell* p = aStarPath[i];
                float pxL = p->x * CELL_SIZE + GRID_OFFSET_X_LEFT + (CELL_SIZE / 2.0f);
                float py = p->y * CELL_SIZE + TOP_PADDING + (CELL_SIZE / 2.0f);
                float pxR = p->x * CELL_SIZE + GRID_OFFSET_X_RIGHT + (CELL_SIZE / 2.0f);

                sf::Color lineColor;
                if (pathState[i] == 1) { // Return To Base
                    lineColor = showReturn ? sf::Color(255, 0, 0, 255) : sf::Color::Transparent;
                } else if (pathState[i] == 2) { // Transit
                    lineColor = showTransit ? sf::Color(255, 0, 255, 255) : sf::Color::Transparent;
                } else { // Active Salting
                    lineColor = sf::Color(255, 140, 0, 255); // Bright Orange!
                }

                sf::Vertex vLeft;
                vLeft.position = sf::Vector2f(pxL, py);
                vLeft.color = lineColor;
                masterLineLeft.append(vLeft);

                sf::Vertex vRight;
                vRight.position = sf::Vector2f(pxR, py);
                vRight.color = lineColor;
                masterLineRight.append(vRight);
            }
            window.draw(masterLineLeft);
            window.draw(masterLineRight);
        }

        if (!aStarPath.empty()) {
            float bsX_Left = aStarPath[0]->x * CELL_SIZE + GRID_OFFSET_X_LEFT + (CELL_SIZE/2.0f);
            float bsX_Right = aStarPath[0]->x * CELL_SIZE + GRID_OFFSET_X_RIGHT + (CELL_SIZE/2.0f);
            float bsY = aStarPath[0]->y * CELL_SIZE + TOP_PADDING + (CELL_SIZE/2.0f);
            
            baseStationIcon.setPosition(sf::Vector2f(bsX_Left, bsY));
            window.draw(baseStationIcon);
            baseStationIcon.setPosition(sf::Vector2f(bsX_Right, bsY));
            window.draw(baseStationIcon);
        }

        if (currentState == MOVING && !aStarPath.empty()) {
            float rx_Left = aStarPath[pathIndex]->x * CELL_SIZE + GRID_OFFSET_X_LEFT + 0.5f;
            float rx_Right = aStarPath[pathIndex]->x * CELL_SIZE + GRID_OFFSET_X_RIGHT + 0.5f;
            float ry = aStarPath[pathIndex]->y * CELL_SIZE + TOP_PADDING + 0.5f;
            
            robot.setPosition(sf::Vector2f(rx_Left, ry));
            window.draw(robot);
            robot.setPosition(sf::Vector2f(rx_Right, ry));
            window.draw(robot);
            
            float progressPct = (float)pathIndex / (float)totalPathSize;
            float currentHeight = barMaxHeight * progressPct;
            progressFill.setSize(sf::Vector2f(20.0f, currentHeight));
            progressFill.setPosition(sf::Vector2f(15.0f, TOP_PADDING + 30.0f + (barMaxHeight - currentHeight)));

            float currentSalt = saltLevels[pathIndex];
            float saltPct = currentSalt / MAX_SALT_LBS;
            float saltHeight = barMaxHeight * saltPct;
            saltFill.setSize(sf::Vector2f(20.0f, saltHeight));
            saltFill.setPosition(sf::Vector2f(55.0f, TOP_PADDING + 30.0f + (barMaxHeight - saltHeight)));

            char saltBuf[64];
            snprintf(saltBuf, sizeof(saltBuf), "Salt Tank: %.2f / %.2f lbs", currentSalt, MAX_SALT_LBS);
            saltDataText.setString(saltBuf);
            if (currentSalt <= (MAX_SALT_LBS * 0.15f)) saltDataText.setFillColor(sf::Color::Red);
            else saltDataText.setFillColor(sf::Color(0, 191, 255));

            float currentBatt = batteryLevels[pathIndex];
            float battPct = currentBatt / MAX_BATTERY_CAPACITY;
            float battHeight = barMaxHeight * battPct;
            batteryFill.setSize(sf::Vector2f(20.0f, battHeight));
            batteryFill.setPosition(sf::Vector2f(95.0f, TOP_PADDING + 30.0f + (barMaxHeight - battHeight)));

            char battBuf[64];
            snprintf(battBuf, sizeof(battBuf), "Battery: %.2f / %.2f U", currentBatt, MAX_BATTERY_CAPACITY);
            batteryDataText.setString(battBuf);
            if (currentBatt <= (MAX_BATTERY_CAPACITY * 0.20f)) batteryDataText.setFillColor(sf::Color::Red);
            else batteryDataText.setFillColor(sf::Color::Yellow);
        }

        sf::RectangleShape verticalDivider(sf::Vector2f(2.0f, GRID_AREA_H));
        verticalDivider.setPosition(sf::Vector2f(GRID_OFFSET_X_RIGHT - (GAP_BETWEEN_MAPS/2.0f), TOP_PADDING));
        verticalDivider.setFillColor(sf::Color::Red);
        window.draw(verticalDivider);
        
        window.draw(jobLabel);
        window.draw(progressBG);
        window.draw(progressFill);
        
        window.draw(tankLabel);
        window.draw(saltBG);
        window.draw(saltFill);

        window.draw(battLabel);
        window.draw(batteryBG);
        window.draw(batteryFill);

        window.draw(legendTitle);
        window.draw(saltDataText);
        window.draw(batteryDataText);

        window.draw(createText("Map Scale:", TOP_PADDING + 150.0f, 20));
        char scaleBuffer[64];
        if (resolution > 0) {
            snprintf(scaleBuffer, sizeof(scaleBuffer), "%.2f ft x %.2f ft per cell", resolution, resolution);
        } else {
            snprintf(scaleBuffer, sizeof(scaleBuffer), "Scale data missing from txt!");
        }
        sf::Text scaleText(font, scaleBuffer, 16);
        scaleText.setPosition(sf::Vector2f(WINDOW_W - RIGHT_PANEL_W + 40.0f, TOP_PADDING + 175.0f));
        scaleText.setFillColor(sf::Color(200, 200, 200));
        window.draw(scaleText);

        float swatchStart = TOP_PADDING + 220.0f;
        window.draw(createSwatch(sf::Color(255, 255, 0, 180), swatchStart));
        window.draw(createText("[N] Upcoming Path", swatchStart));

        window.draw(createSwatch(sf::Color(255, 140, 0, 255), swatchStart + 50.0f));
        window.draw(createText("[D] Driven Tracking Line", swatchStart + 50.0f));
        
        window.draw(createSwatch(sf::Color(0, 191, 255, 180), swatchStart + 100.0f));
        window.draw(createText("[S] Salted Concrete", swatchStart + 100.0f));
        
        window.draw(createSwatch(sf::Color(255, 0, 0, 180), swatchStart + 150.0f));
        window.draw(createText("[B] Returning to Base", swatchStart + 150.0f));

        window.draw(createSwatch(sf::Color(255, 0, 255, 180), swatchStart + 200.0f));
        window.draw(createText("Transit (No Salt)", swatchStart + 200.0f));

        window.draw(createSwatch(sf::Color(50, 205, 50), swatchStart + 250.0f));
        window.draw(createText("Base Station", swatchStart + 250.0f));

        window.draw(toggleText);

        window.display();
    }

    return 0;
}