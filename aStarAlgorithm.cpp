#include "aStarAlgorithm.h"
#include <bits/stdc++.h>
using namespace std;

#define ROW 9
#define COL 10

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// I need something to detect the transition conflicts
typedef pair<pair<int, int>, pair<int, int>> Edge;

// A structure to hold the necessary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
    // Add a wait time
    int waitTime;
};

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[][COL], int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt(
            (row - dest.first) * (row - dest.first)
            + (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source
// to destination
vector<pair<int, int>> tracePath(cell cellDetails[][COL], Pair dest)
{
    //printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;
    vector<pair<int, int>> path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        // Check if there is a wait time
        if (cellDetails[row][col].waitTime > 0) {
            for (int i = 0; i < cellDetails[row][col].waitTime; i++) {
                Path.push(make_pair(row, col));
            }
        }
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    // Check if there is a wait time
    if (cellDetails[row][col].waitTime > 0) {
        for (int i = 0; i < cellDetails[row][col].waitTime; i++) {
            Path.push(make_pair(row, col));
        }
    }

    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        path.push_back(p);
        Path.pop();
        //printf("-> (%d,%d) ", p.first, p.second);
    }

    return path;
}

bool isOccupiedAtThisTime(map<pair<int, int>, vector<int>> & reservationTable, int row, int col, int time) {
    if (reservationTable.find(make_pair(row, col)) != reservationTable.end()) {
        vector<int> times = reservationTable[make_pair(row, col)];
        for (int i = 0; i < times.size(); i++) {
            if (times[i] == time) {
                return true;
            }
        }
    }
    return false;
}

int occupationLength(map<pair<int, int>, vector<int>> & reservationTable, int row, int col, int time) {
    // From the start time, find how long that cell is continuously occupied for
    int length = 0;

    if (reservationTable.find(make_pair(row, col)) != reservationTable.end()) {
        vector<int> times = reservationTable[make_pair(row, col)];
        for (int i = 0; i < times.size(); i++) {
            if (times[i] >= time) {
                length++;
            }
        }
    }

    return length;
}

bool canIWaitHereForThisLong(map<pair<int, int>, vector<int>> & reservationTable, int row, int col, int time, int waitTime) {
    // Check if the cell is occupied for the next waitTime
    for (int i = 0; i < waitTime; i++) {
        if (isOccupiedAtThisTime(reservationTable, row, col, time + i)) {
            return false;
        }
    }
    return true;
}

bool isEdgeOccupiedAtThisTime(map<Edge, vector<int>> & edgeReservationTable, pair<int, int> startNode, pair<int, int> endNode, int time) {
    if (edgeReservationTable.find(make_pair(endNode, startNode)) != edgeReservationTable.end()) {
        vector<int> times = edgeReservationTable[make_pair(endNode, startNode)];
        for (int i = 0; i < times.size(); i++) {
            if (times[i] == time) {
                return true;
            }
        }
    }
    return false;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
vector<pair<int, int>> aStarSearch(int grid[][COL], Pair src, Pair dest, map<pair<int, int>, vector<int>> & reservationTable, map<Edge, vector<int>> & edgeReservationTable, int startTime)
{
    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return {};
    }

    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return {};
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.first, src.second) == false
        || isUnBlocked(grid, dest.first, dest.second)
           == false) {
        printf("Source or the destination is blocked\n");
        return {};
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return {};
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet. This closed
    // list is implemented as a boolean 2D array
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].waitTime = 0;
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].waitTime = 0;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair of
     pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        // Start making an edge reservation
        pair<int, int> startNode = make_pair(i, j);

        int arrivalTime = startTime + static_cast<int>(cellDetails[i][j].g) + 1; // Arrival time at successor cell
        if (!isOccupiedAtThisTime(reservationTable, i, j, arrivalTime)) {
            reservationTable[make_pair(i, j)].push_back(arrivalTime); // Mark cell occupied at new arrival time
        } else {
            // Get this node's parent and check how long to wait there for
            int wait_i = cellDetails[i][j].parent_i;
            int wait_j = cellDetails[i][j].parent_j;

            int waitTime = occupationLength(reservationTable, wait_i, wait_j, startTime + static_cast<int>(cellDetails[i][j].g));

            // Check if the wait time is below our threshold and if it is check if we can wait that time
            // Add this: && canIWaitHereForThisLong(reservationTable, i, j, startTime + static_cast<int>(cellDetails[i][j].g) + 2, waitTime)
            if (waitTime < 3) {
                // Add this to reservation table
                for (int k = 0; k < waitTime; k++) {
                    reservationTable[make_pair(wait_i, wait_j)].push_back(startTime + static_cast<int>(cellDetails[i][j].g) + 2 + k);
                    cellDetails[wait_i][wait_j].g += 1.0;
                    cellDetails[wait_i][wait_j].waitTime += 1;
                }
            } else {
                continue;
            }
        }

        /*
         Generating all the 4 successors of this cell

                   N
                   |
                   |
             W----Cell----E
                    |
                    |
                    S

         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West        (i, j-1)

        */

        // To store the 'g', 'h' and 'f' of the 4 successors
        double gNew, hNew, fNew;

        // THERE MIGHT BE A NICHE CASE WHERE DESTINATION NODE DOESN'T CHECK OCCUPANCY!!!
        // ADD EDGE RESERVATION FOR NON-DESTINATION NODES

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j) == true) {
            // Make the end node for the edge reservation
            pair<int, int> endNode = make_pair(i-1, j);

            // If the destination cell is the same as the current successor
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                vector<pair<int, int>> path = tracePath(cellDetails, dest);
                foundDest = true;

                // Check if the edge is occupied
                if (!isEdgeOccupiedAtThisTime(edgeReservationTable, startNode, endNode, startTime + static_cast<int>(cellDetails[i][j].g) + 2)) {
                    // Make the edge reservation
                    edgeReservationTable[make_pair(startNode, endNode)].push_back(startTime + static_cast<int>(cellDetails[i][j].g) + 2);   // The time we insert is the time when we arrive at the end node

                    int lastTime = startTime + static_cast<int>(cellDetails[i][j].g) + 2; // Arrival time at successor cell
                    reservationTable[make_pair(i-1, j)].push_back(lastTime); // Mark cell occupied at new arrival time

                    return path;
                }
            }
                // If the successor is already on the closed list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i - 1][j] == false && isUnBlocked(grid, i - 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                    }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j) == true) {
            // Make the end node for the edge reservation
            pair<int, int> endNode = make_pair(i+1, j);

            // If the destination cell is the same as the current successor
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                vector<pair<int, int>> path = tracePath(cellDetails, dest);
                foundDest = true;

                // Check if the edge is occupied
                if (!isEdgeOccupiedAtThisTime(edgeReservationTable, startNode, endNode, startTime + static_cast<int>(cellDetails[i][j].g) + 2)) {
                    // Make the edge reservation
                    edgeReservationTable[make_pair(startNode, endNode)].push_back(startTime + static_cast<int>(cellDetails[i][j].g) + 2);   // The time inserted is when we arrive at the end node

                    int lastTime = startTime + static_cast<int>(cellDetails[i][j].g) + 2; // Arrival time at successor cell
                    reservationTable[make_pair(i+1, j)].push_back(lastTime); // Mark cell occupied at new arrival time

                    return path;
                }
            }
                // If the successor is already on the closed list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i + 1][j] == false && isUnBlocked(grid, i + 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                    }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j + 1) == true) {
            // Make the end node for the edge reservation
            pair<int, int> endNode = make_pair(i, j+1);

            // If the destination cell is the same as the current successor
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                vector<pair<int, int>> path = tracePath(cellDetails, dest);
                foundDest = true;

                if (!isEdgeOccupiedAtThisTime(edgeReservationTable, startNode, endNode, startTime + static_cast<int>(cellDetails[i][j].g) + 2)) {
                    // Make the edge reservation
                    edgeReservationTable[make_pair(startNode, endNode)].push_back(startTime + static_cast<int>(cellDetails[i][j].g) + 2);   // The time we insert is the time when we arrive at the end node

                    int lastTime = startTime + static_cast<int>(cellDetails[i][j].g) + 2; // Arrival time at successor cell
                    reservationTable[make_pair(i, j+1)].push_back(lastTime); // Mark cell occupied at new arrival time

                    return path;
                }
            }

                // If the successor is already on the closed list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j + 1] == false && isUnBlocked(grid, i, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j - 1) == true) {
            // Make the end node for the edge reservation
            pair<int, int> endNode = make_pair(i, j-1);

            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                vector<pair<int, int>> path = tracePath(cellDetails, dest);
                foundDest = true;

                if (!isEdgeOccupiedAtThisTime(edgeReservationTable, startNode, endNode, startTime + static_cast<int>(cellDetails[i][j].g) + 2)) {
                    // Make the edge reservation
                    edgeReservationTable[make_pair(startNode, endNode)].push_back(startTime + static_cast<int>(cellDetails[i][j].g) + 2);   // The time we insert is the time when we arrive at the end node

                    int lastTime = startTime + static_cast<int>(cellDetails[i][j].g) + 2; // Arrival time at successor cell
                    reservationTable[make_pair(i, j-1)].push_back(lastTime); // Mark cell occupied at new arrival time

                    return path;
                }
            }

                // If the successor is already on the closed list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j - 1] == false && isUnBlocked(grid, i, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i, j - 1)));

                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                    }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return {};
}