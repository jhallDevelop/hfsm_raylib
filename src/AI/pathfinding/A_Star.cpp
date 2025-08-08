#include "raylib.h"
#include "A_Star.h"
#include <iostream>
AStar::AStar(int _gridWidth, int _gridHeight, int _gridSize)
{
    // Initialize the node grid
    nodeVector = std::make_unique<std::vector<std::vector<Node>>>();
    // Initialize the node grid
    Node::CreateNodeGrid(nodeVector.get(), gridWidth, gridHeight);
}

AStar::~AStar()
{
}

void AStar::OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2])
{
    // Convert start and end positions to Node objects
    std::cout << "Starting DFS with start node: (" << _nodeIndexStart[0] << ", " << _nodeIndexStart[1] << ") and end node: (" << _nodeIndexEnd[0] << ", " << _nodeIndexEnd[1] << ")\n";
    // Call the DFS algorithm starting from the startNode
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

    AStarSearch(startNode, endNode);
}

void AStar::OnUpdate() const
{
}

void AStar::OnRender() const
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
                color = BLACK;
            } 

            // Tile is Visited: Set tile color to light grey
            if(currentNode.visited && !currentNode.isObstacle) {
                color = LIGHTGRAY;
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

void AStar::OnEnd()
{
}

void AStar::CreateRandomObstacles(int obstacleCount)
{
    // Create random obstacles in the DFS grid
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

void AStar::AStarSearch(Node &_startNode, Node &_endNode)
{
    // Implementation of the DFS algorithm
    // This function will be called to perform the DFS search starting from the given node
    // used to scan
    Node& currentNode = _startNode;
    // This is a Last in the last out structure LIFO
    // This is one of the main differences between DFS and BFS
    // BFS uses a queue, while DFS uses a stack
    std::stack<Node*> stack;

    // Empty the stack before starting the search
    stack.empty();

    // visit the first node
    currentNode.visited = true;

    // insert the node r into the queue
    stack.push(&currentNode); // Add the start node to the queue


    // while the queue is not empty loop
    while(!stack.empty())
    {
        // add a slight delay for visualization purposes
        // This is one of the main differences between DFS and BFS
        // BFS uses a queue, while DFS uses a stack
        Node* n = stack.top(); // Get the front node from the queue
        stack.pop(); // Remove the front node from the queue
        // Check if we reached the end node
        if (n->position.x == _endNode.position.x && n->position.y == _endNode.position.y) {
            std::cout << "Reached the end node at: (" << n->position.x << ", " << n->position.y << ")\n";
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
                stack.push(&rightNode); // Add the adjacent node to the queue
            }
        }

        // check if the adjacent nodes are within bounds
        if (n->position.x < gridWidth && n->position.y + 1 < gridHeight) {
            Node& downNode = nodeVector->at(n->position.x).at(n->position.y + 1);
            if (!downNode.visited && !downNode.isObstacle) {
                downNode.visited = true; // Mark the node as visited
                downNode.parent = n; // Set the parent node
                stack.push(&downNode); // Add the adjacent node to the queue
            }
        }

        if (n->position.x - 1 >= 0 && n->position.y < gridHeight) {
            Node& leftNode = nodeVector->at(n->position.x - 1).at(n->position.y);
            if (!leftNode.visited && !leftNode.isObstacle) {
                leftNode.visited = true; // Mark the node as visited
                leftNode.parent = n; // Set the parent node
                stack.push(&leftNode); // Add the adjacent node to the queue
            }
        }
        if (n->position.x < gridWidth && n->position.y - 1 >= 0) {
            Node& upNode = nodeVector->at(n->position.x).at(n->position.y - 1);
            if (!upNode.visited && !upNode.isObstacle) {
                upNode.visited = true; // Mark the node as visited
                upNode.parent = n; // Set the parent node
                stack.push(&upNode); // Add the adjacent node to the queue
            }
        }

        
    }   // end while
}
