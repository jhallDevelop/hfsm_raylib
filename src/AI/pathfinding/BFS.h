#pragma once
#include "Pathfinding.h"
#include "Node.h"
class BFS : public Pathfinding {
public:
    BFS(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector);
    ~BFS() override;

    void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2], std::vector<std::vector<Node*>>& _nodeVector) override;
    void OnUpdate() const override;
    void OnRender(std::vector<std::vector<Node*>>& _nodeVector) const override;
    void OnEnd() override;
    int GetGridWidth() const;
    int GetGridHeight() const;
    void SetGridSize(int width, int height);
    void BreadthFirstSearch(Node& _startNode, Node& _endNode, std::vector<std::vector<Node*>>& _nodeVector);
    Vector2 GetNextWaypoint() override;
    void AdvanceToNextWaypoint() override; // Method to advance to the next waypoint in the path
    
private:
    std::vector<Node*> finalPath; // ADD THIS: To store the calculated path
    int gridWidth = 10;  // Example width
    int gridHeight = 10; // Example height
    int gridSize = 20; // Size of each node in the grid
    // Add any private members or methods needed for BFS implementation
};  