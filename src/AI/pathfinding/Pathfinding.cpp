#include "raylib.h"
#include "Pathfinding.h"


// Reset the grid to its initial state
void Pathfinding::ResetGrid(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector) {
    for (int x = 0; x < _gridWidth; ++x) {
        for (int y = 0; y < _gridHeight; ++y) {
            if (_nodeVector.at(x).at(y) != nullptr) {
                Node& node = *_nodeVector.at(x).at(y);
                node.gCost = FLT_MAX;
                node.parent = nullptr;
                node.visited = false;
                node.isPath = false;
            }
        }
    }
}

void Pathfinding::CreateRandomObstacles(std::vector<std::vector<Node*>>& _nodeVector, int _gridWidth, int _gridHeight, int _obstacleCount)
{
    // randomly select nodes to be obstacles
    if (_obstacleCount <= 0 || _obstacleCount > _gridWidth * _gridHeight) {
        TraceLog(LOG_WARNING, "Invalid obstacle count: %d", _obstacleCount);
        return; // Invalid obstacle count
    }
    
    // for loop to create obstacles
    for (int i = 0; i < _obstacleCount; ++i) {
        int x = GetRandomValue(0, _gridWidth - 1);
        int y = GetRandomValue(0, _gridHeight - 1);
        _nodeVector.at(x).at(y)->isObstacle = true; // Mark the node as an obstacle
    }
}