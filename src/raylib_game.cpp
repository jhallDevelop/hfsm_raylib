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
const char* GAME_TITLE = "HFSM Example"; // Game title
const char* GAME_DESCRIPTION = "W, A, S, D to Move"; // Game description
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
    std::unique_ptr<Pathfinding> pathfinding; // Pathfinding algorithm (if needed)
    // Font
    Font font;
    PathfindingType pathFindingType;
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

    // Initialise the pawns and AI state machine
    // get a reference to the player, enemy pawns and AI state machine
    Pawn* playerPawn = gameData->playerPawn.get();
    Pawn* enemyPawn = gameData->enemyPawn.get();
    AI_HSFM* enemyAIStateMachine = gameData->enemyAIStateMachine.get();

    // Initialize player and enemy pawns
    playerPawn->CreatePawn({ 400, 280 }, 2.0f, BLUE, CIRCLE, 20.0f, true);
    enemyPawn->CreatePawn({ 600, 280 }, 1.0f, RED, TRIANGLE, 20.0f, false);
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

    // Load global data (assets that must be available in all screens, i.e. font)
    gameData.get()->font = LoadFont(FONT_FILE_PATH);
    Font* font = &gameData.get()->font; // Get a pointer to the font

    // setup the pathfinding algorithm
    int const gridSize = 20; // Example grid width
    int const randomObstacleCount = 250; // Number of random obstacles to create


    // ==============  Pathfinding  ================
    gameData.get()->pathFindingType = PathfindingType::A_STAR_TYPE;
    switch (gameData.get()->pathFindingType)
    {
    case PathfindingType::BFS_TYPE:
        gameData->pathfinding = std::make_unique<BFS>(SCREEN_WIDTH/gridSize, SCREEN_HEIGHT/gridSize, gridSize);
        break;

    case PathfindingType::DFS_TYPE:
        gameData->pathfinding = std::make_unique<DFS>(SCREEN_WIDTH/gridSize, SCREEN_HEIGHT/gridSize, gridSize);
        break;

    case PathfindingType::DIYKSTRA_TYPE:
        gameData->pathfinding = std::make_unique<Diykstra>(SCREEN_WIDTH/gridSize, SCREEN_HEIGHT/gridSize, gridSize);
        break;

    case PathfindingType::A_STAR_TYPE:
        gameData->pathfinding = std::make_unique<AStar>(SCREEN_WIDTH/gridSize, SCREEN_HEIGHT/gridSize, gridSize);
        break;
    
    }
    
    std::cout << "Pathfinding algorithm initialized." << std::endl;
    Pathfinding* pathfinding = gameData->pathfinding.get();
    if (pathfinding == nullptr) {
        TraceLog(LOG_ERROR, "Failed to cast pathfinding to pathfinding");
        return -1; // Exit if casting fails
    }
    
    pathfinding->CreateRandomObstacles(randomObstacleCount); // Create random obstacles in the grid
    // pick a random start and end node for the pathfinding algorithm
    int startNodeIndex[2] = { GetRandomValue(0, SCREEN_WIDTH/gridSize - 1), GetRandomValue(0, SCREEN_HEIGHT/gridSize - 1) };
    int endNodeIndex[2] = { GetRandomValue(0, SCREEN_WIDTH/gridSize - 1), GetRandomValue(0, SCREEN_HEIGHT/gridSize - 1) };
    pathfinding->OnStart(startNodeIndex, endNodeIndex); // Initialize the pathfinding algorithm
    

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
    _gameData.pathfinding->OnRender(); // Render the pathfinding grid


    // TODO: Draw gameplay text
    Vector2 titlePos = { 20, 10 };
    Vector2 descriptionPos = { 20, 60 };
    Vector2 statePos = { SCREEN_WIDTH - 100, 60 };
    DrawTextEx(_gameData.font, GAME_TITLE, titlePos, _gameData.font.baseSize*3.0f, 4, MAROON);
    DrawTextEx(_gameData.font, GAME_DESCRIPTION, descriptionPos, _gameData.font.baseSize*1.0f, 4, BLACK);
    DrawTextEx(_gameData.font, _gameData.enemyAIStateMachine.get()->GetCurrentState()->GetStateName(), statePos, _gameData.font.baseSize*1.0f, 4, BLACK);
    
    

    // Render player pawn
    _gameData.playerPawn->Render();
    
    // render enemy pawn
    _gameData.enemyPawn->Render();

    
}


// Update AI state machine and pawns
void UpdateAI(GameData& _gameData)
{
    // Update AI state machine
    Pawn* playerPawn = _gameData.playerPawn.get();
    Pawn* enemyPawn = _gameData.enemyPawn.get();
    _gameData.enemyAIStateMachine->Update(*enemyPawn, *playerPawn);
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

    // --- LOGIC PHASE 2: AI (with Debug Drawing) ---
    // The AI calculates its force and can draw debug visuals.
    // This must happen before the physics update.
    gameData.get()->enemyAIStateMachine->Update(*gameData.get()->enemyPawn, *gameData.get()->playerPawn);

    // --- LOGIC PHASE 3: PHYSICS ---
    // Now that all forces for this frame are known (from Input and AI),
    // we apply them to update the pawn positions and velocities.
    // This must happen after AI and before the final render.
    UpdatePawns(*gameData.get());

    // --- RENDER PHASE ---
    // Draw the final, updated state of the world.
    Draw(*gameData.get());

    EndDrawing();   // Raylib function to finish drawing
}

void WrapPawnPosition(Pawn &_pawn)
{
    // Wrap pawn position to screen bounds
    Vector2 pos = _pawn.GetPosition();
    if (pos.x < 0) pos.x = SCREEN_WIDTH;
    else if (pos.x > SCREEN_WIDTH) pos.x = 0;

    if (pos.y < 0) pos.y = SCREEN_HEIGHT;
    else if (pos.y > SCREEN_HEIGHT) pos.y = 0;

    _pawn.SetPosition(pos); // Set the wrapped position back to the pawn
}



// Helper function to update player pawn based on input
void UpdateInput(Pawn& _pawn)
{
    // Update player pawn based on input
    Vector2 force = { 0.0f, 0.0f };
    
    if (IsKeyDown(KEY_D)) force.x += 1.0f;
    if (IsKeyDown(KEY_A))  force.x -= 1.0f;
    if (IsKeyDown(KEY_W))    force.y -= 1.0f;
    if (IsKeyDown(KEY_S))  force.y += 1.0f;

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
}
