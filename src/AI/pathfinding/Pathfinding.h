#pragma once
#include "raymath.h"
class Pathfinding {
public:
    virtual ~Pathfinding() = default;

    // Pure virtual methods for pathfinding algorithms
    virtual void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2]) = 0;
    virtual void OnUpdate() const = 0;
    virtual void OnRender() const = 0;
    virtual void OnEnd() = 0;
    virtual void CreateRandomObstacles(int obstacleCount) = 0;
    virtual void ResetGrid(int _gridWidth, int _gridHeight, int _gridSize) = 0;
    virtual Vector2 GetNextWaypoint() = 0; // Method to get the next waypoint in the path
    virtual void AdvanceToNextWaypoint() = 0; // Method to advance to the next waypoint in the path
};