#pragma once
#include "raymath.h"
#include "Node.h"

class Pathfinding {
public:
    virtual ~Pathfinding() = default;

    // Pure virtual methods for pathfinding algorithms
    virtual void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2], std::vector<std::vector<Node*>>& _nodeVector) = 0;
    virtual void OnUpdate() const = 0;
    virtual void OnRender(std::vector<std::vector<Node*>>& _nodeVector) const = 0;
    virtual void OnEnd() = 0;
    virtual Vector2 GetNextWaypoint() = 0; // Method to get the next waypoint in the path
    virtual void AdvanceToNextWaypoint() = 0; // Method to advance to the next waypoint in the path

    // Declaration of common methods for pathfinding algorithms
    void ResetGrid(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector);
    void CreateRandomObstacles(std::vector<std::vector<Node*>>& _nodeVector, int _gridWidth, int _gridHeight, int _obstacleCount);
};