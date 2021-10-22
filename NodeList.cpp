#include "NodeList.h"
#include <iostream>

// Constructor
NodeList::NodeList() :
    length(0)
{
    nodes = new Node*[length];
}

// Copy constructor
NodeList::NodeList(NodeList& other) :
length(other.getLength()) 
{
    nodes = new Node*[length];
    for (int i = 0; i < length; i++) {
        nodes[i] = new Node(*other.getNode(i));
    }
}

// Destructor
NodeList::~NodeList() {
    for(int i = 0; i < length; i++) {
        delete nodes[i];
    }
    delete [] nodes;
}

int NodeList::getLength(){
    return length;
}

/* 
 * Adjusted for milestone 4.
 * We resize our nodes array before adding a deep copy
 * of the element taken as parameter, newNode.
 */
void NodeList::addElement(Node* newNode){
    Node** temp = new Node*[length];
    // Stores the objects within nodes in temp array.
    for(int i = 0; i < length; i++) {
        temp[i] = nodes[i];
        nodes[i] = nullptr;
    }
    delete [] nodes;

    // Resize our nodes array.
    nodes = new Node*[length + 1];

    // Re-allocate the objects to our nodes array
    for(int i = 0; i < length; i++) {
        nodes[i] = temp[i];
        temp[i] = nullptr;
    }
    nodes[length] = new Node(*newNode);
    delete [] temp;
    length++;
}

Node* NodeList::getNode(int i){
    return nodes[i];
}

/*
 * Checks whether our list contains an object with the same
 * row and column values. Returns a bool value.
 */ 
bool NodeList::contains(int row, int col) {
    bool contains = false;
    for(int i = 0; i < length; i++) {
        if(col == nodes[i]->getCol() && row == nodes[i]->getRow()) {
            contains = true;
        }
    }
    return contains;    
}