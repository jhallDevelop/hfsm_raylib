#include "raylib.h"
#include "A_Star.h"
#include <iostream>
AStar::AStar(int _gridWidth, int _gridHeight, int _gridSize) : gridWidth(_gridWidth), gridHeight(_gridHeight), gridSize(_gridSize)
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
    // Reset the grid to its initial state
    ResetGrid(gridWidth, gridHeight, gridSize);
    
    // Implementation of the Dijkstra algorithm
    // This function will be called to perform the Dijkstra search starting from the given node
    // used to scan
    Node& currentNode = _startNode;
    
    // The openSet should be a NEW, SEPARATE vector that holds pointers to the nodes to be evaluated.
    std::vector<Node*> openSet;
    openSet.push_back(&currentNode);
    
    // Initialize the start node
    currentNode.isPath = true; // Mark the start node as part of the path
    currentNode.isObstacle = false; // Ensure the start node is not an obstacle
    // set the gCost of the start node to 0
    currentNode.gCost = 0.0f;

    

    
    // insert the node r into the queue
    //openSet.push(&currentNode); // Add the start node to the queue


    // while the queue is not empty loop
    while(!openSet.empty())
    {
        // add a slight delay for visualization purposes
        //WaitTime(0.1f); // Uncomment if you want to add a delay for visualization
        //WaitTime(0.01f);

        // find the node with the lowest gCost
        int lowestFCostNodeIndex = 0;
        for(int i = 0; i < openSet.size(); i++) {
            if(openSet[i]->fCost < openSet[lowestFCostNodeIndex]->fCost) {
                lowestFCostNodeIndex = i; // Update the index of the node with the lowest gCost
            }
        }
        Node* lowestFCostNode = openSet[lowestFCostNodeIndex]; // Get the node with
        // the lowest gCost from the openSet
        // Remove the node from the openSet
        openSet.erase(openSet.begin() + lowestFCostNodeIndex); // Remove the node

        // mark the node as visited
        lowestFCostNode->visited = true; // Mark the node as visited


        //openSet.pop(); // Remove the front node from the queue
        // Check if we reached the end node
        if (lowestFCostNode->position.x == _endNode.position.x && lowestFCostNode->position.y == _endNode.position.y) {
            std::cout << "Reached the end node at: (" << lowestFCostNode->position.x << ", " << lowestFCostNode->position.y << ")\n";
            // start retracing the path
            Node* pathNode = lowestFCostNode; // Start from the end node
            while (pathNode != nullptr) {
                pathNode->isPath = true; // Mark the node as part of the path
                pathNode = pathNode->parent; // Move to the parent node
            }
            return; // Exit if the end node is reached
        }

        // for(each unmarked Node s adjacent to n){ 
        // how to get adjacent nodes?
        // get the node to the right and left
        // also calculate the gCost for each adjacent node, and update the parent node if necessary

        // Right Neighbor
        // Check if the adjacent nodes are within bounds
        if (lowestFCostNode->position.x + 1 < gridWidth && lowestFCostNode->position.y < gridHeight) {
            Node& neighbour = nodeVector->at(lowestFCostNode->position.x + 1).at(lowestFCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the right node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Down Neighbor
        // check if the adjacent nodes are within bounds
        if (lowestFCostNode->position.x < gridWidth && lowestFCostNode->position.y + 1 < gridHeight) {
            Node& neighbour = nodeVector->at(lowestFCostNode->position.x).at(lowestFCostNode->position.y + 1);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the down node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Left Neighbor
        if (lowestFCostNode->position.x - 1 >= 0 && lowestFCostNode->position.y < gridHeight) {
            Node& neighbour = nodeVector->at(lowestFCostNode->position.x - 1).at(lowestFCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the left node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Up Neighbor
        if (lowestFCostNode->position.x < gridWidth && lowestFCostNode->position.y - 1 >= 0) {
            Node& neighbour = nodeVector->at(lowestFCostNode->position.x).at(lowestFCostNode->position.y - 1);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the up node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }
    }   // end while
}

void AStar::UpdateNeighbourCost(Node& _lowestGCostNode, Node& _neighbour, Node& _endNode, std::vector<Node*>& _openSet){
    float newGCost = _lowestGCostNode.gCost + 1.0f; // Assuming a cost of 1 for moving to an adjacent node
    
    if (newGCost < _neighbour.gCost) {
        _neighbour.parent = &_lowestGCostNode; // Set the parent node
        _neighbour.gCost = newGCost; // Update the gCost

        // Calculate the H cost, heuristic cost (hCost) using Manhattan distance
        _neighbour.hCost = fabs(_neighbour.position.x - _endNode.position.x) + fabs(_neighbour.position.y - _endNode.position.y);

        // Calculate the total cost (fCost)
        float newFCost = _neighbour.gCost + _neighbour.hCost; // Calculate the total cost
        _neighbour.fCost = newFCost; // Update the fCost

        // CHECK if neighbor is in the openSet. If not, add it.
        bool inOpenSet = false;
        for(Node* node : _openSet) {
            if (node == &_neighbour) {
                inOpenSet = true;
                break;
            }
        }
        if (!inOpenSet) {
            _openSet.push_back(&_neighbour);
        }
    }
}

// Reset the grid to its initial state
void AStar::ResetGrid(int _gridWidth, int _gridHeight, int _gridSize) {
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
