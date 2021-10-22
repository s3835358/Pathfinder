#include "Node.h"
#include <iostream>

// Constructor
Node::Node(int row, int col, int dist_traveled)
{
    this->row = row;
    this->col = col;
    this->dist_traveled = dist_traveled;
}

/*
 *  Copy constructor.
 *  Could be achieved by calling the constructor
 *  Node(other->getRow(), other->getCol(), other->getDistanceTraveled())
 *  but I believe this approach greatly increases code readability
 */
Node::Node(Node& other) :
    row(other.row), col(other.col), dist_traveled(other.dist_traveled)
{
}

// Destructor
Node::~Node(){
}

int Node::getRow(){
    return row;
}

int Node::getCol(){
    return col;
}

int Node::getDistanceTraveled(){
    return dist_traveled;
}

void Node::setDistanceTraveled(int dist_traveled)
{
    this->dist_traveled = dist_traveled;
}

int Node::getEstimatedDist2Goal(Node* goal){
    int manhattan = abs(col - goal->getCol()) + abs(row - goal->getRow());
    return dist_traveled + manhattan;
}

/*
 * Compares this node to another node, 'other', 
 * and returns a pointer to the node with the 
 * lowest estimated distance to goal.
 */
Node* Node::compareDist(Node* other, Node* goal) {
    Node* target = this;
    if(other->getEstimatedDist2Goal(goal) < getEstimatedDist2Goal(goal)) {
        target = other;
    }
    return target;
}    
//--------------------------------                             