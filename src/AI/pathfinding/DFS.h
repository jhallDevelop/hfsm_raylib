#pragma once
#include "Pathfinding.h"
#include "Node.h"
class DFS : public Pathfinding {
public:
    DFS(int _gridWidth, int _gridHeight, int _gridSize);
    ~DFS() override;

    void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2]) override;
    void OnUpdate() const override;
    void OnRender() const override;
    void OnEnd() override;
    int GetGridWidth() const;
    int GetGridHeight() const;
    void SetGridSize(int width, int height);
    void CreateRandomObstacles(int obstacleCount) override;
    void DepthFirstSearch(Node& _startNode, Node& _endNode);
    void ResetGrid(int _gridWidth, int _gridHeight, int _gridSize) override;  
    Vector2 GetNextWaypoint() override;
    void AdvanceToNextWaypoint() override; // Method to advance to the next waypoint in the path
    
private:
    std::unique_ptr<std::vector<std::vector<Node>>> nodeVector;
    std::vector<Node*> finalPath; // ADD THIS: To store the calculated path
    int gridWidth = 10;  // Example width
    int gridHeight = 10; // Example height
    int gridSize = 20; // Size of each node in the grid
    // Add any private members or methods needed for BFS implementation
};  