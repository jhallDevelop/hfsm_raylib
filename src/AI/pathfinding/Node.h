#pragma once
#include <vector>
#include "raymath.h"

#define FLT_MAX 3.402823466e+38F // Maximum float value

class Node {
public:
    Node(Vector2 pos) : position(pos), visited(false), gCost(FLT_MAX), isObstacle(false), isPath(false), parent(nullptr) {}
    Node() : position({0,0}), visited(false), gCost(FLT_MAX), isObstacle(false),  isPath(false), parent(nullptr) {}
    ~Node() = default;
    
    Vector2 position; // Position of the node in the grid
    bool visited; // Flag to check if the node has been visited
    float gCost;    // Needed for Dijkstra and A* algorithms. Default set to maximum float value
    bool isObstacle = false; // Flag to check if the node is an obstacle
    bool isPath = false; // Flag to check if the node is part of the path
    Node* parent; // Pointer to the parent node in the path

    static void CreateNodeGrid(std::vector<std::vector<Node>>* _nodeVector, int width, int height);
};

