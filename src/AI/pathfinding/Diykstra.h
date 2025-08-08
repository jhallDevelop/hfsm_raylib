#pragma once
#include "Pathfinding.h"
#include "Node.h"
class Diykstra : public Pathfinding {
public:
    Diykstra(int _gridWidth, int _gridHeight, int _gridSize);
    ~Diykstra() override;
    void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2]) override;
    void OnUpdate() const override;
    void OnRender() const override;
    void OnEnd() override;
    int GetGridWidth() const;
    int GetGridHeight() const;
    void SetGridSize(int width, int height);
    void CreateRandomObstacles(int obstacleCount) override;
    void DijkstraSearch(Node& _startNode, Node& _endNode);  
    void ResetGrid(int _gridWidth, int _gridHeight, int _gridSize) override;
    void UpdateNeighbourCost(Node& _lowestGCostNode, Node& _neighbour,std::vector<Node*>& _openSet);


private:
    std::unique_ptr<std::vector<std::vector<Node>>> nodeVector;
    int gridWidth;  // Example width
    int gridHeight; // Example height
    int gridSize; // Size of each node in the grid
    // Add any private members or methods needed for Dijkstra's implementation
};