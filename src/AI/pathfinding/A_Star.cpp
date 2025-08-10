#include "raylib.h"
#include "A_Star.h"
#include <iostream>
AStar::AStar(int _gridWidth, int _gridHeight, int _gridSize, std::vector<std::vector<Node*>>& _nodeVector) : gridWidth(_gridWidth), gridHeight(_gridHeight), gridSize(_gridSize)
{
    // Initialize the node grid
    Node::CreateNodeGrid(_nodeVector, gridWidth, gridHeight);
}

AStar::~AStar()
{
}

void AStar::OnStart(int _nodeIndexStart[2], int _nodeIndexEnd[2], std::vector<std::vector<Node*>>& _nodeVector)
{
    // Reset the grid to its initial state
    ResetGrid(gridWidth, gridHeight, gridSize, _nodeVector);

    // Convert start and end positions to Node objects
    // Call the A* algorithm starting from the startNode
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

    AStarSearch(startNode, endNode, _nodeVector);
}

void AStar::OnUpdate() const
{
}

void AStar::OnRender(std::vector<std::vector<Node*>>& _nodeVector) const
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

void AStar::OnEnd()
{
}



void AStar::AStarSearch(Node &_startNode, Node &_endNode, std::vector<std::vector<Node*>>& _nodeVector)
{
    
    // Reset the grid to its initial state
    ResetGrid(gridWidth, gridHeight, gridSize, _nodeVector);

    _startNode.visited = true; // Mark the start node as visited
    _startNode.isPath = true; // Mark the start node as part of the path
    _startNode.isObstacle = false; // Ensure the start node is not an obstacle   

    // End Node
    //endNode.visited = true; // Mark the end node as visited
    _endNode.isPath = true; // Mark the start node as part of the path
    _endNode.isObstacle = false; // Ensure the start node is not an obstacle 
    _endNode.visited = false; // Ensure the end node is not visited
    
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
        if (lowestFCostNode == &_endNode) { // Simpler check: compare pointers
            finalPath.clear();
            Node* pathNode = lowestFCostNode;
            while (pathNode != nullptr) {
                pathNode->isPath = true;
                finalPath.push_back(pathNode);
                pathNode = pathNode->parent;
            }
            std::reverse(finalPath.begin(), finalPath.end());
            m_currentWaypointIndex = 0; // Reset the waypoint index for the new path
            return;
        }

        // for(each unmarked Node s adjacent to n){ 
        // how to get adjacent nodes?
        // get the node to the right and left
        // also calculate the gCost for each adjacent node, and update the parent node if necessary

        // Right Neighbor
        // Check if the adjacent nodes are within bounds
        if (lowestFCostNode->position.x + 1 < gridWidth && lowestFCostNode->position.y < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestFCostNode->position.x + 1).at(lowestFCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the right node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Down Neighbor
        // check if the adjacent nodes are within bounds
        if (lowestFCostNode->position.x < gridWidth && lowestFCostNode->position.y + 1 < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestFCostNode->position.x).at(lowestFCostNode->position.y + 1);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the down node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Left Neighbor
        if (lowestFCostNode->position.x - 1 >= 0 && lowestFCostNode->position.y < gridHeight) {
            Node& neighbour = *_nodeVector.at(lowestFCostNode->position.x - 1).at(lowestFCostNode->position.y);
            if (!neighbour.visited && !neighbour.isObstacle) {
                // Calculate the gCost for the left node
                UpdateNeighbourCost(*lowestFCostNode, neighbour, _endNode, openSet); // Update the neighbour cost
            }
        }

        // Up Neighbor
        if (lowestFCostNode->position.x < gridWidth && lowestFCostNode->position.y - 1 >= 0) {
            Node& neighbour = *_nodeVector.at(lowestFCostNode->position.x).at(lowestFCostNode->position.y - 1);
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




void AStar::StoreFinalPath(Node& _lowestCostNode, Node &_endNode)
{
    // Clear any previous path
    finalPath.clear();

    Node* pathNode = &_lowestCostNode; // Start from the end node
    while (pathNode != nullptr) {
        pathNode->isPath = true;
        finalPath.push_back(pathNode); // Add each node to our path vector
        pathNode = pathNode->parent;
    }
    // The path is now stored backwards (end -> start), so reverse it
    std::reverse(finalPath.begin(), finalPath.end());
}

// Advance to the next waypoint in the path
void AStar::AdvanceToNextWaypoint() {
    if (!finalPath.empty() && m_currentWaypointIndex < finalPath.size() - 1) {
        m_currentWaypointIndex++;
    }
}

// Get the next waypoint in the path
Vector2 AStar::GetNextWaypoint()
{
    if (finalPath.empty() || m_currentWaypointIndex >= finalPath.size()) {
        return {-1, -1}; // Return an invalid position if no path or path is complete
    }
    return finalPath[m_currentWaypointIndex]->position;
}
