#pragma once

class Pathfinding {
public:
    virtual ~Pathfinding() = default;

    // Pure virtual methods for pathfinding algorithms
    virtual void OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2]) = 0;
    virtual void OnUpdate() const = 0;
    virtual void OnRender() const = 0;
    virtual void OnEnd() = 0;
};