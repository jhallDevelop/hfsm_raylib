#include "Node.h"


void Node::CreateNodeGrid(std::vector<std::vector<Node>> *_nodeVector, int width, int height)
{
    if (!_nodeVector) {
    return;
   }
    
    _nodeVector->resize(width);
    for (int x = 0; x < width; x++) {
        (*_nodeVector)[x].resize(height);
        for (int y = 0; y < height; y++) {
            (*_nodeVector)[x][y] = Node(Vector2{static_cast<float>(x), static_cast<float>(y)});
        }
    }
}
