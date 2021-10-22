#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "Types.h"
#include "Node.h"
#include "NodeList.h"
#include "PathSolver.h"
#include <queue>

// Read an environment from standard input.
Env readEnvStdin(int* rowPtr, int* colPtr, Node** startPtr, Node** goalPtr);
// Print our environment.
void printEnvStdout(Env env, NodeList* solution, int envRow, int envCol);
// Dynamically allocate memory for our environment.
Env make_env(const int rows, const int cols);
// Delete memory for our dynamically allocated environment.
void delete_env(Env env, int rows, int cols);

/*
 * Beyond implementing basic functionality, one of the most notable aspects
 * of my program is that the start and goal positions are assigned to nodes 
 * within the main.readEnvStdin method not within PathSolver, this approach 
 * was taken as it fit within the scope of the specification and allowed
 * fewer iterations of the env, though this may need to be reconsidered if
 * the scope of the program grew for the sake of coupling. 
 * 
 * Limitations: Input files MUST NOT have an extra newline character, 
 * PathSolver.giveInputs MUST be called before any other PathSolver method.
 */
int main(int argc, char** argv){
    int envRow = 0,
    envCol = 0;
    Node* start = nullptr;
    Node* goal = nullptr;

    Env env = readEnvStdin(&envRow, &envCol, &start, &goal);
    PathSolver* pathSolver = new PathSolver();
    pathSolver->giveInputs(envRow, envCol, start, goal);
    pathSolver->forwardSearch(env);

    NodeList* exploredPositions = pathSolver->getNodesExplored();
    NodeList* solution = pathSolver->getPath(env);
    printEnvStdout(env, solution, envRow, envCol);

    delete_env(env, envRow, envCol);
    delete exploredPositions;
    delete solution;
    delete start;
    delete goal;
    delete pathSolver;
}

Env readEnvStdin(int* rowPtr, int* colPtr, Node** startPtr, Node** goalPtr) {
    char c = 'a';
    int envCol = 0, envRow = 0;
    bool firstLine = true;
    std::queue<char> firstChars;
    // Read input to determine env dimensions, push each character to a queue
    while (!std::cin.eof()) {
        std::cin.get(c);
        if(c == '\n') {
            envRow++;
            firstLine = false;
        } else {
            firstChars.push(c);
        }
        if(firstLine) {
            envCol++;
        }
    }  
    
    envRow++;
    
    Env env = make_env(envRow, envCol);
    // Assign each character in queue to appropriate position within env
    for (int row = 0; row < envRow; row++) {
        for (int col = 0; col < envCol; col++) {
            c = firstChars.front();
            firstChars.pop();
            env[row][col] = c;
            // We assign our start and goal positions to nodes
            if(c == SYMBOL_START) {
                *startPtr = new Node(row, col, 0);
            } else if (c == SYMBOL_GOAL) {
                *goalPtr = new Node(row, col, 0);
            }
        }
    }
    *rowPtr = envRow;
    *colPtr = envCol;
    return env;
}

void printEnvStdout(Env env, NodeList* solution, int envRow, int envCol) {
    Node* pos = nullptr;
    Node* nextPos = nullptr;

    const int LEFT = -1, RIGHT = 1, UP = -1, DOWN = 1;

    //We update our environment with our directional characters
    //Note that we don't want to update our start and end nodes
    for (int i = 1; i < solution->getLength() - 1; i++) {
        pos = solution->getNode(i);
        nextPos = solution->getNode(i + 1);

        int colDiff = nextPos->getCol() - pos->getCol(),
        rowDiff = nextPos->getRow() - pos->getRow();
        
        if(colDiff == RIGHT) {
            env[pos->getRow()][pos->getCol()] = '>';
        } else if (colDiff == LEFT) {
            env[pos->getRow()][pos->getCol()] = '<';
        } else if (rowDiff == DOWN) {
            env[pos->getRow()][pos->getCol()] = 'v';
        } else if (rowDiff == UP) {
            env[pos->getRow()][pos->getCol()] = '^';
        }
    }
    //We print our environment
    for (int row = 0; row < envRow; row++) {
        for (int col = 0; col < envCol; col++) {
                std::cout << env[row][col];
        }
        std::cout << std::endl;
    }
}

Env make_env(const int rows, const int cols) {
    Env env = nullptr;
    if (rows >= 0 && cols >= 0) {
        env = new char*[rows];
        for (int i = 0; i != rows; ++i) {
            env[i] = new char[cols];
        }
    }
    return env;
}

void delete_env(Env env, int rows, int cols) {
   if (rows >= 0 && cols >= 0) {
      for (int i = 0; i != rows; ++i) {
         delete [] env[i];
      }
      delete [] env;
   }
   return;
}