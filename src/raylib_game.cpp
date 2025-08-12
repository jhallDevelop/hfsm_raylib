/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <memory>   // For smart pointers
#include <iostream>
#include "raylib.h"
#include "raymath.h" // For mathematical functions
#include "Pawn.h"
#include "AI/state_machine/AI_HFSM.h"
#include "AI/pathfinding/BFS.h"
#include "AI/pathfinding/DFS.h"
#include "AI/pathfinding/Diykstra.h"
#include "AI/pathfinding/A_Star.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif



//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 450;
int const GRID_SIZE = 20; // Example grid width
const char* GAME_TITLE = "HFSM Example"; // Game title
const char* GAME_DESCRIPTION = "W, A, S, D to Move, 1, 2, 3, 4 for AI Type"; // Game description
enum PathfindingType {
    BFS_TYPE,
    DFS_TYPE,
    DIYKSTRA_TYPE,
    A_STAR_TYPE
};

// Assets
const char* FONT_FILE_PATH = "resources/mecha.png"; // Font file path


// Game Data containing Pawns & AI State Machine
typedef struct GameData {
    std::unique_ptr<Pawn> playerPawn; // Player controlled pawn
    std::unique_ptr<Pawn> enemyPawn; // Example of an enemy pawn
    std::unique_ptr<AI_HSFM> enemyAIStateMachine; // AI state machine
    std::unique_ptr<Weapon> playerWeapon; // Player's weapon
    std::unique_ptr<Weapon> enemyWeapon; // Enemy's weapon (if needed)
    
    // Pathfinding
    std::shared_ptr<Pathfinding> pathfinding; // Pathfinding algorithm (if needed)
    std::shared_ptr<Pathfinding> aStarPathfinding;
    std::shared_ptr<Pathfinding> bfsPathfinding;
    std::shared_ptr<Pathfinding> dfsPathfinding;
    std::shared_ptr<Pathfinding> diykstraPathfinding;
    // Font
    Font font;
    std::shared_ptr<std::vector<std::vector<Node*>>> nodeVector; // Node grid for pathfinding
    PathfindingType pathFindingType;
    std::unique_ptr<Pawn> pathfindingTarget; // Target for pathfinding (if needed)
    int gridSize[2]; // Size of each node in the grid
    int gridWidth; // Width of the grid in nodes
    int gridHeight; // Height of the grid in nodes
} GameData;
std::unique_ptr<GameData> gameData;
//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
// Forward declarations
void Draw(const GameData& _gameData);      // Draw gameplay screen
void UpdateInput(Pawn& _pawn); // Update pawn input
void UpdateAI(GameData& _gameData);
void UpdatePawns(GameData& _gameData);
void GameUpdate(); // Update game logic
void WrapPawnPosition(Pawn& _pawn); // Wrap pawn position to screen bounds
void SwapToPathfindingAlgorithm(GameData& _gameData, PathfindingType _pathfindingType); // Swap pathfinding algorithm


//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);    // Set configuration flags (can be done before InitWindow()), required of MacOS and Linux to support high DPI
    
    // Initialization
    //---------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE); // Initialize window and OpenGL context
    
    // Create a space for the game data on the heap
    gameData = std::make_unique<GameData>();

    // Create the player pawn, enemy pawn, and AI state machine on the heap
    gameData->playerPawn = std::make_unique<Pawn>();
    gameData->enemyPawn= std::make_unique<Pawn>();
    gameData->enemyAIStateMachine = std::make_unique<AI_HSFM>();
    gameData->pathfindingTarget = std::make_unique<Pawn>(); // Create a target for pathfinding (if needed)
    gameData->nodeVector = std::make_shared<std::vector<std::vector<Node*>>>();

    // Initialise the pawns and AI state machine
    // get a reference to the player, enemy pawns and AI state machine
    Pawn* playerPawn = gameData->playerPawn.get();
    Pawn* enemyPawn = gameData->enemyPawn.get();
    AI_HSFM* enemyAIStateMachine = gameData->enemyAIStateMachine.get();

    // Initialize player and enemy pawns
    playerPawn->CreatePawn({ 400, 280 }, 2.0f, BLUE, CIRCLE, GRID_SIZE /2, true);
    enemyPawn->CreatePawn({ 600, 280 }, 1.0f, RED, TRIANGLE, GRID_SIZE/2, false);
    enemyAIStateMachine->InitializeStates(*enemyPawn); // Initialize AI states
    
    // Create a weapon for the player pawn
    // Player
    gameData->playerWeapon = std::make_unique<Weapon>();
    gameData->playerWeapon->CreateWeapon(5, 1.0f, 2.0f); // Create a weapon with 5 ammo, 1 second fire rate
    playerPawn->SetWeapon(gameData->playerWeapon.get()); // Assign the weapon to the player pawn

    // Enemy
    gameData->enemyWeapon = std::make_unique<Weapon>();
    gameData->enemyWeapon->CreateWeapon(5, 1.0f, 2.0f); // Create a weapon with 5 ammo, 1 second fire rate
    enemyPawn->SetWeapon(gameData->enemyWeapon.get()); // Assign the weapon to the
    enemyAIStateMachine->TransitionToState(*enemyPawn, e_AI_StateID::Pathfind); // Start in wander state

   // Load global data (assets that must be available in all screens, i.e. font)
    gameData.get()->font = LoadFont(FONT_FILE_PATH);
    Font* font = &gameData.get()->font; // Get a pointer to the font

    // setup the pathfinding algorithm
    int const randomObstacleCount = 250; // Number of random obstacles to create

    // ==============  Pathfinding  ================
    std::vector<std::vector<Node*>>& nodeVector = *gameData.get()->nodeVector.get();

    // Initialize the node grid
    gameData.get()->gridWidth = (int)SCREEN_WIDTH/GRID_SIZE;
    gameData.get()->gridHeight = (int)SCREEN_HEIGHT/GRID_SIZE;

    // Create the node grid
    Node::CreateNodeGrid(nodeVector, gameData.get()->gridWidth,  gameData.get()->gridHeight);

    // create all the pathfinding algorithms
    gameData.get()->bfsPathfinding = std::make_shared<BFS>(SCREEN_WIDTH/GRID_SIZE, SCREEN_HEIGHT/GRID_SIZE, GRID_SIZE, nodeVector);
    gameData.get()->dfsPathfinding = std::make_shared<DFS>(SCREEN_WIDTH/GRID_SIZE, SCREEN_HEIGHT/GRID_SIZE, GRID_SIZE, nodeVector);
    gameData.get()->diykstraPathfinding = std::make_shared<Diykstra>(SCREEN_WIDTH/GRID_SIZE, SCREEN_HEIGHT/GRID_SIZE, GRID_SIZE, nodeVector);
    gameData.get()->aStarPathfinding = std::make_shared<AStar>(SCREEN_WIDTH/GRID_SIZE, SCREEN_HEIGHT/GRID_SIZE, GRID_SIZE, nodeVector);
    
    // Set the initial pathfinding algorithm to BFS
    gameData.get()->pathFindingType = PathfindingType::BFS_TYPE;
    SwapToPathfindingAlgorithm(*gameData, gameData.get()->pathFindingType); // Swap to the A* pathfinding algorithm
    

    // pick a random start and end node for the pathfinding algorithm
    int startNodeIndex[2] = { (int)(playerPawn->GetPosition().x/GRID_SIZE), (int)(playerPawn->GetPosition().y/GRID_SIZE)};
    int endNodeIndex[2] = { (int)(enemyPawn->GetPosition().x/GRID_SIZE), (int)(enemyPawn->GetPosition().y/GRID_SIZE)};
    
    // Get a reference to the current pathfinding algorithm
    Pathfinding* pathfinding = gameData.get()->pathfinding.get(); // Get the current pathfinding algorithm
    pathfinding->OnStart(startNodeIndex, endNodeIndex, nodeVector); // Initialize the pathfinding algorithm

    pathfinding->CreateRandomObstacles(nodeVector, gameData.get()->gridWidth, gameData.get()->gridHeight, randomObstacleCount); // Create random obstacles in the grid
    
    

#if defined(PLATFORM_WEB)
    //emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
    emscripten_set_main_loop(GameUpdate, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        GameUpdate(); // Update game logic
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing

    // Unload global data loaded
    UnloadFont(*font); // Unload font
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



// ----------------------------------------------------------------------------------
// Local Functions Definition
// draw UI and gameplay elements
// ----------------------------------------------------------------------------------
void Draw(const GameData& _gameData){

    // render the pathfinding
    _gameData.pathfinding->OnRender(*gameData.get()->nodeVector.get()); // Render the pathfinding grid


    // TODO: Draw gameplay text
    Vector2 titlePos = { 20, 10 };
    Vector2 descriptionPos = { 20, 60 };
    Vector2 statePos = { SCREEN_WIDTH - 100, 60 };
    DrawTextEx(_gameData.font, GAME_TITLE, titlePos, _gameData.font.baseSize*3.0f, 4, MAROON);
    DrawTextEx(_gameData.font, GAME_DESCRIPTION, descriptionPos, _gameData.font.baseSize*1.0f, 4, BLACK);
    DrawTextEx(_gameData.font, _gameData.enemyAIStateMachine.get()->GetCurrentState()->GetStateName(), statePos, _gameData.font.baseSize*1.0f, 4, BLACK);
    
    // Draw the pathfinding type as text to the screen
    char pathfindingTypeCharBuffer[1024];
    switch (_gameData.pathFindingType)
    {
    case PathfindingType::BFS_TYPE:
        snprintf(pathfindingTypeCharBuffer, sizeof(pathfindingTypeCharBuffer), "Pathfinding: BFS");
        break;
    case PathfindingType::DFS_TYPE:
        snprintf(pathfindingTypeCharBuffer, sizeof(pathfindingTypeCharBuffer), "Pathfinding: DFS");
        break;
    case PathfindingType::DIYKSTRA_TYPE:
        snprintf(pathfindingTypeCharBuffer, sizeof(pathfindingTypeCharBuffer), "Pathfinding: Diykstra");
        break;
    case PathfindingType::A_STAR_TYPE:
        snprintf(pathfindingTypeCharBuffer, sizeof(pathfindingTypeCharBuffer), "Pathfinding: A*");
        break;
    default:
        snprintf(pathfindingTypeCharBuffer, sizeof(pathfindingTypeCharBuffer), "Pathfinding: Unknown");
        break;
    }
    DrawTextEx(_gameData.font, pathfindingTypeCharBuffer, { 20, SCREEN_HEIGHT - 30 }, _gameData.font.baseSize*1.0f, 4, BLUE);
    

    // Render player pawn
    _gameData.playerPawn->Render();
    
    // render enemy pawn
    _gameData.enemyPawn->Render();
}


// Update AI state machine and pawns
void UpdateAI(GameData& _gameData)
{
    // Get pointers to the necessary objects
    Pawn* playerPawn = _gameData.playerPawn.get();
    Pawn* enemyPawn = _gameData.enemyPawn.get();
    Pathfinding* pathfinding = _gameData.pathfinding.get();
    AI_HSFM* enemyAIStateMachine = _gameData.enemyAIStateMachine.get();

    // Safety checks
    if (!playerPawn || !enemyPawn || !pathfinding || !enemyAIStateMachine) {
        TraceLog(LOG_ERROR, "UpdateAI: A required game object is null!");
        return;
    }

    // --- 1. Calculate and CLAMP Path Indices ---
    // Get grid dimensions from the pathfinding object itself for safety.
    // (You will need to add GetGridWidth() and GetGridHeight() to your Pathfinding/AStar class)
    int gridW = SCREEN_WIDTH / GRID_SIZE;
    int gridH = SCREEN_HEIGHT / GRID_SIZE;

    // Calculate raw indices from world positions
    int startX = (int)(enemyPawn->GetPosition().x / GRID_SIZE);
    int startY = (int)(enemyPawn->GetPosition().y / GRID_SIZE);
    int endX   = (int)(playerPawn->GetPosition().x / GRID_SIZE);
    int endY   = (int)(playerPawn->GetPosition().y / GRID_SIZE);

    // Clamp the indices to ensure they are always within the valid grid range [0, width-1] and [0, height-1]
    startX = Clamp(startX, 0, gridW - 1);
    startY = Clamp(startY, 0, gridH - 1);
    endX   = Clamp(endX, 0, gridW - 1);
    endY   = Clamp(endY, 0, gridH - 1);

    int startNodeIndex[2] = { startX, startY };
    int endNodeIndex[2]   = { endX, endY };

    pathfinding->OnStart(startNodeIndex, endNodeIndex, *gameData.get()->nodeVector.get()); // Initialize the pathfinding algorithm with the clamped indices

    // --- 2. Get the Current Waypoint (in Grid Coordinates) ---
    Vector2 currentWaypointGrid = pathfinding->GetNextWaypoint();

    // --- 3. Check if the Path is Valid ---
    if (currentWaypointGrid.x == -1 && currentWaypointGrid.y == -1) {
        // No path exists or the path is complete. Seek the player directly as a fallback.
        enemyAIStateMachine->Update(*enemyPawn, *playerPawn);
        return;
    }

    // --- 4. Convert Waypoint to World Coordinates ---
    Vector2 currentWaypointWorld = {
        (currentWaypointGrid.x * GRID_SIZE) + (GRID_SIZE / 2.0f),
        (currentWaypointGrid.y * GRID_SIZE) + (GRID_SIZE / 2.0f)
    };

    // --- 5. Check for Arrival at the Current Waypoint ---
    float distanceToWaypoint = Vector2Distance(enemyPawn->GetPosition(), currentWaypointWorld);
    float stoppingRadius = GRID_SIZE * 0.75f; // A sensible radius, slightly more than half a grid cell

    if (distanceToWaypoint < stoppingRadius) {
        // We're close enough, so advance to the next waypoint in the path.
        pathfinding->AdvanceToNextWaypoint();
        
        // Get the position of the NEW current waypoint (in Grid Coordinates)
        currentWaypointGrid = pathfinding->GetNextWaypoint();
        
        // Check again in case we just finished the last waypoint.
        if (currentWaypointGrid.x == -1 && currentWaypointGrid.y == -1) {
            // Path is now complete, seek the player's actual final position.
            enemyAIStateMachine->Update(*enemyPawn, *playerPawn);
            return;
        }

        // CONVERT THE NEW WAYPOINT TO WORLD COORDINATES
        currentWaypointWorld = {
            (currentWaypointGrid.x * GRID_SIZE) + (GRID_SIZE / 2.0f),
            (currentWaypointGrid.y * GRID_SIZE) + (GRID_SIZE / 2.0f)
        };
    }

    // --- 6. Set AI Target and Update ---
    // The target is always the valid, up-to-date waypoint in WORLD coordinates.
    _gameData.pathfindingTarget->SetPosition(currentWaypointWorld);
    enemyAIStateMachine->Update(*enemyPawn, *_gameData.pathfindingTarget.get());
}

// Update pawns based on AI and player input
void UpdatePawns(GameData& _gameData)
{
    // Update player pawn
    _gameData.playerPawn->Update();
    _gameData.enemyPawn->Update();

    WrapPawnPosition(*_gameData.playerPawn); // Wrap player pawn position to screen bounds
    WrapPawnPosition(*_gameData.enemyPawn); // Wrap enemy pawn position to screen bounds
}

void GameUpdate()
{
    // --- LOGIC PHASE 1: INPUT ---
    // Calculate the player's desired force for this frame.
    UpdateInput(*gameData.get()->playerPawn); // Update player pawn input

    // --- DRAWING & LOGIC PHASE ---
    BeginDrawing(); // Raylib function to start drawing
    ClearBackground(RAYWHITE);  // Raylib function to clear the background

    // --- PHYSICS ---
    // Now that all forces for this frame are known (from Input and AI),
    // we apply them to update the pawn positions and velocities.
    // This must happen after AI and before the final render.
    UpdatePawns(*gameData.get());

    // update AI state machine
    UpdateAI(*gameData.get());

    // --- RENDER PHASE ---
    // Draw the final, updated state of the world.
    Draw(*gameData.get());

    EndDrawing();   // Raylib function to finish drawing
}

void WrapPawnPosition(Pawn &_pawn)
{
    // Wrap pawn position to screen bounds
    Vector2 pos = _pawn.GetPosition();

    // Wrap X position
    if (pos.x < 0) {
        pos.x = SCREEN_WIDTH;
    }
    else if (pos.x > SCREEN_WIDTH) {
        pos.x = 0;
    }

    // Wrap Y position
    if (pos.y < 0) {
        pos.y = SCREEN_HEIGHT;
    }
    else if (pos.y > SCREEN_HEIGHT) {
        pos.y = 0;
    }

    _pawn.SetPosition(pos); // Set the wrapped position back to the pawn
}



// Helper function to update player pawn based on input
void UpdateInput(Pawn& _pawn)
{
    // Update player pawn based on input
    Vector2 force = { 0.0f, 0.0f };
    
    if (IsKeyDown(KEY_D)) {
        force.x += 1.0f;
    }

    if (IsKeyDown(KEY_A))  {
        force.x -= 1.0f;
    }

    if (IsKeyDown(KEY_W)){
        force.y -= 1.0f;
    }

    if (IsKeyDown(KEY_S)){
        force.y += 1.0f;
    }

    // Normalize the force if moving diagonally to prevent faster speed
    if (Vector2LengthSqr(force) > 0) {
        force = Vector2Normalize(force);
    }

    // Scale the force by the pawn's speed to create a desired velocity
    // Note: This is a simplified steering force for direct control
    Vector2 desired = Vector2Scale(force, _pawn.GetSpeed());
    Vector2 steer = Vector2Subtract(desired, _pawn.GetVelocity());
    
    // Apply the steering force
    _pawn.ApplyForce(steer);

    // update input to choose AI Pathfinding algorithm
    if (IsKeyPressed(KEY_ONE)) {
        gameData.get()->pathFindingType = PathfindingType::BFS_TYPE;
        SwapToPathfindingAlgorithm(*gameData, gameData.get()->pathFindingType); //
        std::cout << "Switched to BFS Pathfinding Algorithm." << std::endl;
    }
    else if (IsKeyPressed(KEY_TWO)) {
        gameData.get()->pathFindingType = PathfindingType::DFS_TYPE;
        SwapToPathfindingAlgorithm(*gameData, gameData.get()->pathFindingType);
        std::cout << "Switched to DFS Pathfinding Algorithm." << std::endl;
    }
    else if (IsKeyPressed(KEY_THREE)) {
        gameData.get()->pathFindingType = PathfindingType::DIYKSTRA_TYPE;
        SwapToPathfindingAlgorithm(*gameData, gameData.get()->pathFindingType);
        std::cout << "Switched to Diykstra Pathfinding Algorithm." << std::endl;
    }
    else if (IsKeyPressed(KEY_FOUR)) {
        gameData.get()->pathFindingType = PathfindingType::A_STAR_TYPE;
        SwapToPathfindingAlgorithm(*gameData, gameData.get()->pathFindingType);
        std::cout << "Switched to A* Pathfinding Algorithm." << std::endl;
    }
}


void SwapToPathfindingAlgorithm(GameData& _gameData, PathfindingType _pathfindingType){
    switch (_pathfindingType)
    {
    case PathfindingType::BFS_TYPE:
        _gameData.pathfinding = gameData.get()->bfsPathfinding;
        break;

    case PathfindingType::DFS_TYPE:
        _gameData.pathfinding = gameData.get()->dfsPathfinding;
        break;

    case PathfindingType::DIYKSTRA_TYPE:
        _gameData.pathfinding = gameData.get()->diykstraPathfinding;
        break;

    case PathfindingType::A_STAR_TYPE:
        _gameData.pathfinding = gameData.get()->aStarPathfinding;
        break;
    }
}