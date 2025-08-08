#include <memory>
#include <queue>
#include "raylib.h"
#include "BFS.h"
#include <iostream>

BFS::BFS(int _gridWidth, int _gridHeight, int _gridSize) : gridWidth(_gridWidth), gridHeight(_gridHeight), gridSize(_gridSize)
{
    // Initialize the node grid
    nodeVector = std::make_unique<std::vector<std::vector<Node>>>();
    // Initialize the node grid
    Node::CreateNodeGrid(nodeVector.get(), gridWidth, gridHeight);
}

BFS::~BFS()
{
}

void BFS::OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2])
{
    // Convert start and end positions to Node objects
    
    // Call the BFS algorithm starting from the startNode
    Node& startNode = nodeVector->at(_nodeIndexStart[0]).at(_nodeIndexStart[1]); // Example: starting node
    startNode.visited = true; // Mark the start node as visited
    startNode.isPath = true; // Mark the start node as part of the path
    startNode.isObstacle = false; // Ensure the start node is not an obstacle   

    // End Node
    Node& endNode = nodeVector->at(_nodeIndexEnd[0]).at(_nodeIndexEnd[1]); // Example: ending node
    //endNode.visited = true; // Mark the end node as visited
    endNode.isPath = true; // Mark the start node as part of the path
    endNode.isObstacle = false; // Ensure the start node is not an obstacle 
    endNode.visited = false; // Ensure the end node is not visited

    BreadthFirstSearch(startNode, endNode);
}

void BFS::OnUpdate() const
{
}

void BFS::OnRender() const
{
    for(int x = 0; x < gridWidth; x++)
    {
        for(int y = 0; y < gridHeight; y++)
        {
            // Render each node in the grid
            // draw obstacles and visited nodes
            Node& currentNode = nodeVector->at(x).at(y);
            Color color = WHITE; // Default color for nodes

            // tile is obstacle: Set tile color to black
            if (currentNode.isObstacle == true) {
                color = LIGHTGRAY;
            } 

            // Tile is Visited: Set tile color to light grey
            if(currentNode.visited && !currentNode.isObstacle) {
                color = BLUE;
            }

            // Tile is Path: Set tile color to green
            if(currentNode.isPath == true) {
                color = GREEN; // Color for path nodes
            }

            // Draw the tile
            DrawRectangle(
                static_cast<int>(currentNode.position.x * gridSize), // Scale for visibility
                static_cast<int>(currentNode.position.y * gridSize),
                gridSize, gridSize, color); // Draw the node rectangle
        }
    }
}

void BFS::OnEnd()
{
}

int BFS::GetGridWidth() const
{
    return gridWidth;
}

int BFS::GetGridHeight() const
{
    return gridHeight;
}

void BFS::SetGridSize(int width, int height)
{
    gridWidth = width;
    gridHeight = height;
}

void BFS::BreadthFirstSearch(Node& _startNode, Node& _endNode)
{
    ResetGrid(gridWidth, gridHeight, gridSize);

    // Implementation of the BFS algorithm
    // This function will be called to perform the BFS search starting from the given node
    // used to scan
    Node& currentNode = _startNode;
    // This is a first in the first out structure FIFO
    std::queue<Node*> queue;

    // Empty the queue before starting the search
    queue.empty();

    // visit the first node
    currentNode.visited = true;

    // insert the node r into the queue
    queue.push(&currentNode); // Add the start node to the queue


    // while the queue is not empty loop
    while(!queue.empty())
    {
        // add a slight delay for visualization purposes
        //WaitTime(0.1f); // Uncomment if you want to add a delay for visualization
        //WaitTime(0.01f);
        Node* n = queue.front(); // Get the front node from the queue
        queue.pop(); // Remove the front node from the queue
        // Check if we reached the end node
        if (n->position.x == _endNode.position.x && n->position.y == _endNode.position.y) {
            // start retracing the path
            Node* pathNode = n; // Start from the end node
            while (pathNode != nullptr) {
                pathNode->isPath = true; // Mark the node as part of the path
                pathNode = pathNode->parent; // Move to the parent node
            }
            return; // Exit if the end node is reached
        }

        // for(each unmarked Node s adjacent to n){ 
        // how to get adjacent nodes?
        // get the node to the right and left

        // Check if the adjacent nodes are within bounds
        if (n->position.x + 1 < gridWidth && n->position.y < gridHeight) {
            Node& rightNode = nodeVector->at(n->position.x + 1).at(n->position.y);
            if (!rightNode.visited && !rightNode.isObstacle) {
                rightNode.visited = true; // Mark the node as visited
                rightNode.parent = n; // Set the parent node
                queue.push(&rightNode); // Add the adjacent node to the queue
            }
        }

        // check if the adjacent nodes are within bounds
        if (n->position.x < gridWidth && n->position.y + 1 < gridHeight) {
            Node& downNode = nodeVector->at(n->position.x).at(n->position.y + 1);
            if (!downNode.visited && !downNode.isObstacle) {
                downNode.visited = true; // Mark the node as visited
                downNode.parent = n; // Set the parent node
                queue.push(&downNode); // Add the adjacent node to the queue
            }
        }

        if (n->position.x - 1 >= 0 && n->position.y < gridHeight) {
            Node& leftNode = nodeVector->at(n->position.x - 1).at(n->position.y);
            if (!leftNode.visited && !leftNode.isObstacle) {
                leftNode.visited = true; // Mark the node as visited
                leftNode.parent = n; // Set the parent node
                queue.push(&leftNode); // Add the adjacent node to the queue
            }
        }
        if (n->position.x < gridWidth && n->position.y - 1 >= 0) {
            Node& upNode = nodeVector->at(n->position.x).at(n->position.y - 1);
            if (!upNode.visited && !upNode.isObstacle) {
                upNode.visited = true; // Mark the node as visited
                upNode.parent = n; // Set the parent node
                queue.push(&upNode); // Add the adjacent node to the queue
            }
        }
    }   // end while
} // end BredthFirstSearch


void BFS::CreateRandomObstacles(int obstacleCount)
{
    
    // Create random obstacles in the BFS grid
    if (!nodeVector) {
        TraceLog(LOG_WARNING, "nodeVector is null");
        return; // Check if the node vector is initialized
    }

    // randomly select nodes to be obstacles
    if (obstacleCount <= 0 || obstacleCount > gridWidth * gridHeight) {
        TraceLog(LOG_WARNING, "Invalid obstacle count: %d", obstacleCount);
        return; // Invalid obstacle count
    }
    
    // for loop to create obstacles
    for (int i = 0; i < obstacleCount; ++i) {
        int x = GetRandomValue(0, gridWidth - 1);
        int y = GetRandomValue(0, gridHeight - 1);
        nodeVector.get()->at(x).at(y).isObstacle = true; // Mark the node as an obstacle
    }
}

// Reset the grid to its initial state
void BFS::ResetGrid(int _gridWidth, int _gridHeight, int _gridSize) {
    for (int x = 0; x < _gridWidth; ++x) {
        for (int y = 0; y < _gridHeight; ++y) {
            Node& node = nodeVector->at(x).at(y);
            node.gCost = FLT_MAX; // Your constructor already does this, but it's good practice
            node.parent = nullptr;
            node.visited = false;
            node.isPath = false;
        }
    }
}

Vector2 BFS::GetNextWaypoint()
{
    // If the path has fewer than 2 nodes, there's no "next" step.
    if (finalPath.size() < 2) {
        // Return the final destination or the agent's current location.
        if (!finalPath.empty()) {
            return finalPath.back()->position; // Return the goal
        }
        return {-1, -1}; // Or some other invalid position
    }

    // The next waypoint is the second node in the path.
    // m_finalPath[0] is the start node.
    // m_finalPath[1] is the first step to take.
    return finalPath[1]->position;
}

void BFS::AdvanceToNextWaypoint()
{
}
