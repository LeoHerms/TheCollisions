//
// Created by 19514 on 10/26/2024.
//

#ifndef ASTARALGORITHM_H
#define ASTARALGORITHM_H

#include <bits/stdc++.h>
using namespace std;

#define ROW 9
#define COL 10

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the necessary parameters
struct cell;

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col);

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[][COL], int row, int col);

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest);

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest);

// A Utility Function to trace the path from the source
// to destination
vector<pair<int, int>> tracePath(cell cellDetails[][COL], Pair dest);

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
vector<pair<int, int>> aStarSearch(int grid[][COL], Pair src, Pair dest, map<pair<int, int>, vector<int>> & reservationTable, int startTime);


#endif //ASTARALGORITHM_H
