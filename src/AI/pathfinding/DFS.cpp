#include <memory>
#include <queue>
#include "raylib.h"
#include "DFS.h"
#include <iostream>

DFS::DFS(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector) : gridWidth(_gridWidth), gridHeight(_gridHeight), gridSize(_gridSize)
{
     // Initialize the node grid
     Node::CreateNodeGrid(_nodeVector, gridWidth, gridHeight);
}

DFS::~DFS()
{
}

void DFS::OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2], std::vector<std::vector<Node*>>& _nodeVector)
{
   

    // Convert start and end positions to Node objects
    // Call the DFS algorithm starting from the startNode
    Node& startNode = *_nodeVector.at(_nodeIndexStart[0]).at(_nodeIndexStart[1]); // Example: starting node
    startNode.visited = true; // Mark the start node as visited
    startNode.isPath = true; // Mark the start node as part of the path
    startNode.isObstacle = false; // Ensure the start node is not an obstacle   

    // End Node
    Node& endNode = *_nodeVector.at(_nodeIndexEnd[0]).at(_nodeIndexEnd[1]); // Example: ending node
    //endNode.visited = true; // Mark the end node as visited
    endNode.isPath = true; // Mark the start node as part of the path
    endNode.isObstacle = false; // Ensure the start node is not an obstacle 
    endNode.visited = false; // Ensure the end node is not visited

    DepthFirstSearch(startNode, endNode, _nodeVector);
}

void DFS::OnUpdate() const
{
}

void DFS::OnRender(std::vector<std::vector<Node*>>& _nodeVector) const
{
    for(int x = 0; x < gridWidth; x++)
    {
        for(int y = 0; y < gridHeight; y++)
        {
            // Render each node in the grid
            // draw obstacles and visited nodes
            Node& currentNode = *_nodeVector.at(x).at(y);
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

void DFS::OnEnd()
{
}

int DFS::GetGridWidth() const
{
    return gridWidth;
}

int DFS::GetGridHeight() const
{
    return gridHeight;
}

void DFS::SetGridSize(int width, int height)
{
    gridWidth = width;
    gridHeight = height;
}

void DFS::DepthFirstSearch(Node& _startNode, Node& _endNode, std::vector<std::vector<Node*>>& _nodeVector)
{
    ResetGrid(gridWidth, gridHeight, gridSize, _nodeVector);
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
            //std::cout << "Reached the end node at: (" << n->position.x << ", " << n->position.y << ")\n";
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
            Node& rightNode = *_nodeVector.at(n->position.x + 1).at(n->position.y);
            if (!rightNode.visited && !rightNode.isObstacle) {
                rightNode.visited = true; // Mark the node as visited
                rightNode.parent = n; // Set the parent node
                stack.push(&rightNode); // Add the adjacent node to the queue
            }
        }

        // check if the adjacent nodes are within bounds
        if (n->position.x < gridWidth && n->position.y + 1 < gridHeight) {
            Node& downNode = *_nodeVector.at(n->position.x).at(n->position.y + 1);
            if (!downNode.visited && !downNode.isObstacle) {
                downNode.visited = true; // Mark the node as visited
                downNode.parent = n; // Set the parent node
                stack.push(&downNode); // Add the adjacent node to the queue
            }
        }

        if (n->position.x - 1 >= 0 && n->position.y < gridHeight) {
            Node& leftNode = *_nodeVector.at(n->position.x - 1).at(n->position.y);
            if (!leftNode.visited && !leftNode.isObstacle) {
                leftNode.visited = true; // Mark the node as visited
                leftNode.parent = n; // Set the parent node
                stack.push(&leftNode); // Add the adjacent node to the queue
            }
        }
        if (n->position.x < gridWidth && n->position.y - 1 >= 0) {
            Node& upNode = *_nodeVector.at(n->position.x).at(n->position.y - 1);
            if (!upNode.visited && !upNode.isObstacle) {
                upNode.visited = true; // Mark the node as visited
                upNode.parent = n; // Set the parent node
                stack.push(&upNode); // Add the adjacent node to the queue
            }
        }

        
    }   // end while
} // end DepthFirstSearch






Vector2 DFS::GetNextWaypoint()
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

void DFS::AdvanceToNextWaypoint()
{
}
