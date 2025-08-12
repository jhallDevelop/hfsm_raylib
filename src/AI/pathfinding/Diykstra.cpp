#include "raylib.h"
#include "Diykstra.h"
#include <iostream>

// forward declaration of the function to update the neighbour cost
Diykstra::Diykstra(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector) : gridWidth(_gridWidth), gridHeight(_gridHeight), gridSize(_gridSize)
{
    // Initialize the node grid
    Node::CreateNodeGrid(_nodeVector, gridWidth, gridHeight);
}

Diykstra::~Diykstra()
{
}

void Diykstra::OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2], std::vector<std::vector<Node*>>& _nodeVector)
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

    DijkstraSearch(startNode, endNode, _nodeVector);

}

void Diykstra::OnUpdate() const
{
}

void Diykstra::OnRender(std::vector<std::vector<Node*>>& _nodeVector) const
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
                color = LIGHTGRAY;
            } 

            if(currentNode.traversalCost > 1.0f) {
                // If the node has a traversal cost greater than 1, set it to a different color
                color = BROWN; // Color for hard-to-traverse nodes
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

void Diykstra::OnEnd()
{
}


void Diykstra::DijkstraSearch(Node &_startNode, Node &_endNode, std::vector<std::vector<Node*>>& _nodeVector)
{
    // Reset the grid to its initial state
    ResetGrid(gridWidth, gridHeight, gridSize, _nodeVector);
    
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

        // find the node with the lowest gCost
        int lowestGCostNodeIndex = 0;
        for(int i = 0; i < openSet.size(); i++) {
            if(openSet[i]->gCost < openSet[lowestGCostNodeIndex]->gCost) {
                lowestGCostNodeIndex = i; // Update the index of the node with the lowest gCost
            }
        }
        Node* lowestGCostNode = openSet[lowestGCostNodeIndex]; // Get the node with
        // the lowest gCost from the openSet
        // Remove the node from the openSet
        openSet.erase(openSet.begin() + lowestGCostNodeIndex); // Remove the node

        // mark the node as visited
        lowestGCostNode->visited = true; // Mark the node as visited


        //openSet.pop(); // Remove the front node from the queue
        // Check if we reached the end node
        if (lowestGCostNode->position.x == _endNode.position.x && lowestGCostNode->position.y == _endNode.position.y) {
            // start retracing the path
            Node* pathNode = lowestGCostNode; // Start from the end node
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
        if (lowestGCostNode->position.x + 1 < gridWidth && lowestGCostNode->position.y < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestGCostNode->position.x + 1).at(lowestGCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the right node
                UpdateNeighbourCost(*lowestGCostNode, neighbour, openSet); // Update the neighbour cost
            }
        }

        // Down Neighbor
        // check if the adjacent nodes are within bounds
        if (lowestGCostNode->position.x < gridWidth && lowestGCostNode->position.y + 1 < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestGCostNode->position.x).at(lowestGCostNode->position.y + 1);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the down node
                UpdateNeighbourCost(*lowestGCostNode, neighbour, openSet); // Update the neighbour cost
            }
        }

        // Left Neighbor
        if (lowestGCostNode->position.x - 1 >= 0 && lowestGCostNode->position.y < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestGCostNode->position.x - 1).at(lowestGCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the left node
                UpdateNeighbourCost(*lowestGCostNode, neighbour, openSet); // Update the neighbour cost
            }
        }

        // Up Neighbor
        if (lowestGCostNode->position.x < gridWidth && lowestGCostNode->position.y - 1 >= 0) {
            Node& neighbour = *_nodeVector.at(lowestGCostNode->position.x).at(lowestGCostNode->position.y - 1);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the up node
                UpdateNeighbourCost(*lowestGCostNode, neighbour, openSet); // Update the neighbour cost
            }
        }
    }   // end while
}

void Diykstra::UpdateNeighbourCost(Node& _lowestGCostNode, Node& _neighbour,std::vector<Node*>& _openSet){
    float newGCost = _lowestGCostNode.gCost + _neighbour.traversalCost; // Assuming a cost of 1 for moving to an adjacent node
    if (newGCost < _neighbour.gCost) {
        _neighbour.gCost = newGCost; // Update the gCost
        _neighbour.parent = &_lowestGCostNode; // Set the parent node

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


Vector2 Diykstra::GetNextWaypoint()
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

void Diykstra::AdvanceToNextWaypoint()
{
}
