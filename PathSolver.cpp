#include "PathSolver.h"
#include <iostream>
#include <stack>

// Constructor
PathSolver::PathSolver(){
    nodesExplored = new NodeList();
    envRow = ENV_DIM;
    envCol = ENV_DIM;
}

// Destructor
PathSolver::~PathSolver(){ 
    delete nodesExplored;
    delete goal;
    delete start;  
}

/*
 * Executes our forward search algorithm on an environment, env.
 * For each position, pos, found via findPos we add any viable 
 * next positions to our open list via addPos. We only exit our loop
 * when either our position == goal or we discover our goal is
 * unreachable.
 */
void PathSolver::forwardSearch(Env env){
    NodeList* open = new NodeList();
    bool searching = true;        
    open->addElement(start);

    while(searching) {    
        Node* pos = findPos(open, nodesExplored, goal);
        
        // Checks incase the goal is unreachable
        if (pos == nullptr) {
            searching = false;           
        } else {
            int col = pos -> getCol(), row = pos -> getRow(),
            up = row - 1, down = row + 1, left = col - 1, right = col + 1,
            dist = pos->getDistanceTraveled(); 

            addPos(open, dist, up, col, env);
            addPos(open, dist, down, col, env);
            addPos(open, dist, row, left, env);
            addPos(open, dist, row, right, env);

            nodesExplored->addElement(pos);

            // We cease our loop when our position == goal;
            searching = !(col == goal->getCol() && row == goal->getRow());
        }
         
    }
    delete open; 
}

NodeList* PathSolver::getNodesExplored(){
    return new NodeList(*nodesExplored);
}

/*
 * Executes the backtracking algorithm and pushes each node to a stack.
 * Our stack is then popped one element at a time to a NodeList, 'path'.
 * This is done so that our path is returned from START to FINISH.
 */
NodeList* PathSolver::getPath(Env env){
    NodeList* path = new NodeList();
    const int GOAL_POS = nodesExplored->getLength() - 1;
    Node* pos = nodesExplored->getNode(GOAL_POS);
    Node* nextPos = nullptr;
    std::stack<Node*> pathStack;
    
    pathStack.push(pos);

    for(int i = GOAL_POS - 1; i >= 0; i--) {
        nextPos = nodesExplored->getNode(i);

        if(nextStep(pos, nextPos)) {
            pos = nextPos;
            pathStack.push(pos);
        }
    }
    
    while(!pathStack.empty()) {
            nextPos = pathStack.top();
            pathStack.pop();
            path->addElement(nextPos);
    }

    NodeList* copy = new NodeList(*path);
    delete path;
    return copy;
}

/* 
 * Returns a pointer to the node within our open list that,
 * a) Has not yet been explored AKA is not in our closed list.
 * b) Has the least estimated distance to the goal
 */
Node* PathSolver::findPos(NodeList* open, NodeList* closed, Node* goal) {
    Node* leastDist = nullptr;

    for(int i = 0; i < open->getLength(); i++) {
        Node* node = open->getNode(i);
        bool unexplored = !closed->contains(node->getRow(), node->getCol());

        if(unexplored && leastDist == nullptr) {
            leastDist = node; 

        } else if (unexplored) {
            leastDist = leastDist->compareDist(node, goal);
        }
    }
    return leastDist;
}
/*
 * Checks if a (row, column) position exists within our environment,
 * if it is a traversable symbol (either empty or goal) and if it
 * is not already in our open list. If all conditions are satisfied
 * a node is created and added to our open list.
 */
void PathSolver::addPos(NodeList* open,int dist,int row,int col,Env env) { 
    bool bounds = (row >= 0 && col >= 0 && row < envRow && col < envCol);

    if (bounds) {
        bool isEmpty = (env[row][col] == SYMBOL_EMPTY),
        isGoal = (env[row][col] == SYMBOL_GOAL);

        if((isEmpty || isGoal) && !open->contains(row, col)) { 
            Node* toAdd = new Node(row, col, dist + 1);
            open->addElement(toAdd);       
            delete toAdd;
        }
    }
}
/*
 * Returns a bool value, true if a node, other, is the next step 
 * in our path, with relation to our current position, pos. 
 */
bool PathSolver::nextStep(Node* pos, Node* other) {
    int colDiff = pos->getCol() - other->getCol(),
    rowDiff = pos->getRow() - other->getRow(),
    otherDist = other->getDistanceTraveled(),
    dist = pos->getDistanceTraveled();

    // True if other is one position up, down, left or right of pos.
    bool neighbour = (abs(colDiff) + abs(rowDiff) == 1),
    nextStep = false;

    // Also checks if other is one distance travled less than pos.
    if(neighbour && otherDist < dist ) {
        nextStep = true;
    } 
    return nextStep;
}
/*
 * Passes the row and column values of our environment, as well as
 * our start and goal nodes to our PathSolver object. Must be called
 * prior to our forwardSearch.
 */
void PathSolver::giveInputs(int envRow, int envCol, Node* start, Node* goal) {
    this->start = new Node(*start);
    this->goal = new Node(*goal);
    this->envRow = envRow;
    this->envCol = envCol;
}

//-----------------------------
