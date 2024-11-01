#include "aStarAlgorithm.h"
#include <vector>
using namespace std;

struct intersection {
    int x;
    int y;
    vector<char> involved;
};

vector<pair<int, intersection>> findIntersections(vector<vector<pair<int, int>>> allPaths) {
    // Find the longest path
    int longestPath = 0;
    for (int i = 0; i < allPaths.size(); i++) {
        if (allPaths[i].size() > longestPath) {
            longestPath = allPaths[i].size();
        }
    }

    // List of intersections at their respective times and locations
    vector<pair<int, intersection>> filteredIntersections;

    // Iterate across the columns
    for (int i = 0; i < longestPath; i++) {
        // Iterate across the columns (the current time interval)
        // We need to check for all possible collisions within this time frame and ensure no duplicates

        // Create a map to store the occurrences of each intersection
        map<pair<int, int>, intersection> intersectionMap;

        // Iterate across the rows
        for (int j = 0; j < allPaths.size(); j++) {
            // Add a check to see if the row is even long enough for this column
            if (i >= allPaths[j].size()) {
                continue;
            }

            int x = allPaths[j][i].first;
            int y = allPaths[j][i].second;
            pair<int, int> currentIntersection = make_pair(x, y);
            if (intersectionMap.find(currentIntersection) != intersectionMap.end()) {
                intersectionMap[currentIntersection].involved.push_back('A' + j);
            } else {
                char involved = 'A' + j;
                intersectionMap[currentIntersection] = {x, y, {involved}};
            }
        }

        // Now we need to filter out the intersections that only have one involved path
        for (auto it = intersectionMap.begin(); it != intersectionMap.end(); it++) {
            if (it->second.involved.size() > 1) {
                pair<int, intersection> pushIntersection = make_pair(i, it->second);
                filteredIntersections.push_back(pushIntersection);
            }
        }
    }

    return filteredIntersections;
}

// Driver program to test above function
int main()
{
    /* Description of the Grid-
     1--> The cell is not blocked
     0--> The cell is blocked    */
    int grid[ROW][COL]
            = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };

    map<pair<int, int>, vector<int>> reservationTable;  // This will be used across all aStarSearch calls
    map<Edge, vector<int>> edgeReservationTable;  // This will be used across all aStarSearch calls for edges

    // Make the comparison reservation table
    // Make the comparison edge reservation table
    map<pair<int, int>, vector<int>> comparisonReservationTable;
    map<Edge, vector<int>> comparisonEdgeReservationTable;

    // Source is the left-most bottom-most corner
    Pair src1 = make_pair(0, 0);
    Pair dest1 = make_pair(8, 9);   // 5, 0
    vector<pair<int, int>> path1 = aStarSearch(grid, src1, dest1, reservationTable, edgeReservationTable, 0);

    // Prune the reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison reservation table
    for (int i = 0; i < path1.size(); i++) {
        comparisonReservationTable[path1[i]].push_back(i+1);
        // Sort the vector
        sort(comparisonReservationTable[path1[i]].begin(), comparisonReservationTable[path1[i]].end());
    }

    // Clear the original reservation table and copy over the comparison reservation table
    reservationTable.clear();
    for (auto it = comparisonReservationTable.begin(); it != comparisonReservationTable.end(); it++) {
        reservationTable[it->first] = it->second;
    }

    // Prune the edge reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison edge reservation table
    int edgeTime = 2;
    for (int i = 0; i < path1.size() - 1; i++) {
        pair<int, int> startNode = path1[i];
        pair<int, int> destNode = path1[i + 1];
        comparisonEdgeReservationTable[make_pair(startNode, destNode)].push_back(edgeTime);
        edgeTime++;
        // Sort the vector
        sort(comparisonEdgeReservationTable[make_pair(startNode, destNode)].begin(), comparisonEdgeReservationTable[make_pair(startNode, destNode)].end());
    }

    // Clear the original edge reservation table and copy over the comparison edge reservation table
    edgeReservationTable.clear();
    for (auto it = comparisonEdgeReservationTable.begin(); it != comparisonEdgeReservationTable.end(); it++) {
        edgeReservationTable[it->first] = it->second;
    }

    // 2nd source and destination
    Pair src2 = make_pair(8, 0);
    Pair dest2 = make_pair(0, 9);
    vector<pair<int, int>> path2 = aStarSearch(grid, src2, dest2, reservationTable, edgeReservationTable, 0);   // Added for more commentary

    // Prune the reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison reservation table
    for (int i = 0; i < path2.size(); i++) {
        comparisonReservationTable[path2[i]].push_back(i+1);
        // Sort the vector
        sort(comparisonReservationTable[path2[i]].begin(), comparisonReservationTable[path2[i]].end());
    }

    // Clear the original reservation table and copy over the comparison reservation table
    reservationTable.clear();
    for (auto it = comparisonReservationTable.begin(); it != comparisonReservationTable.end(); it++) {
        reservationTable[it->first] = it->second;
    }

    // Prune the edge reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison edge reservation table
    edgeTime = 2;
    for (int i = 0; i < path2.size() - 1; i++) {
        pair<int, int> startNode = path2[i];
        pair<int, int> destNode = path2[i + 1];
        comparisonEdgeReservationTable[make_pair(startNode, destNode)].push_back(edgeTime);
        edgeTime++;
        // Sort the vector
        sort(comparisonEdgeReservationTable[make_pair(startNode, destNode)].begin(), comparisonEdgeReservationTable[make_pair(startNode, destNode)].end());
    }

    // Clear the original edge reservation table and copy over the comparison edge reservation table
    edgeReservationTable.clear();
    for (auto it = comparisonEdgeReservationTable.begin(); it != comparisonEdgeReservationTable.end(); it++) {
        edgeReservationTable[it->first] = it->second;
    }

    // 3rd source and destination
    Pair src3 = make_pair(4, 0);
    Pair dest3 = make_pair(4, 9);
    vector<pair<int, int>> path3 = aStarSearch(grid, src3, dest3, reservationTable, edgeReservationTable, 0);
    // Prune the reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison reservation table
    for (int i = 0; i < path3.size(); i++) {
        comparisonReservationTable[path3[i]].push_back(i+1);
        // Sort the vector
        sort(comparisonReservationTable[path3[i]].begin(), comparisonReservationTable[path3[i]].end());
    }

    // Clear the original reservation table and copy over the comparison reservation table
    reservationTable.clear();
    for (auto it = comparisonReservationTable.begin(); it != comparisonReservationTable.end(); it++) {
        reservationTable[it->first] = it->second;
    }

    // Prune the edge reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison edge reservation table
    edgeTime = 2;
    for (int i = 0; i < path3.size() - 1; i++) {
        pair<int, int> startNode = path3[i];
        pair<int, int> destNode = path3[i + 1];
        comparisonEdgeReservationTable[make_pair(startNode, destNode)].push_back(edgeTime);
        edgeTime++;
        // Sort the vector
        sort(comparisonEdgeReservationTable[make_pair(startNode, destNode)].begin(), comparisonEdgeReservationTable[make_pair(startNode, destNode)].end());
    }

    // Clear the original edge reservation table and copy over the comparison edge reservation table
    edgeReservationTable.clear();
    for (auto it = comparisonEdgeReservationTable.begin(); it != comparisonEdgeReservationTable.end(); it++) {
        edgeReservationTable[it->first] = it->second;
    }

    // 4th source and destination (opposite diagonal)
    Pair src4 = make_pair(0, 4);
    Pair dest4 = make_pair(8, 4);
    vector<pair<int, int>> path4 = aStarSearch(grid, src4, dest4, reservationTable, edgeReservationTable, 0);
    // Prune the reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison reservation table
    for (int i = 0; i < path4.size(); i++) {
        comparisonReservationTable[path4[i]].push_back(i+1);
        // Sort the vector
        sort(comparisonReservationTable[path4[i]].begin(), comparisonReservationTable[path4[i]].end());
    }

    // Clear the original reservation table and copy over the comparison reservation table
    reservationTable.clear();
    for (auto it = comparisonReservationTable.begin(); it != comparisonReservationTable.end(); it++) {
        reservationTable[it->first] = it->second;
    }

    // Prune the edge reservation table of all the reservations that aren't in the path

    // Iterate through the path and add to the comparison edge reservation table
    edgeTime = 2;
    for (int i = 0; i < path4.size() - 1; i++) {
        pair<int, int> startNode = path4[i];
        pair<int, int> destNode = path4[i + 1];
        comparisonEdgeReservationTable[make_pair(startNode, destNode)].push_back(edgeTime);
        edgeTime++;
        // Sort the vector
        sort(comparisonEdgeReservationTable[make_pair(startNode, destNode)].begin(), comparisonEdgeReservationTable[make_pair(startNode, destNode)].end());
    }

    // Clear the original edge reservation table and copy over the comparison edge reservation table
    edgeReservationTable.clear();
    for (auto it = comparisonEdgeReservationTable.begin(); it != comparisonEdgeReservationTable.end(); it++) {
        edgeReservationTable[it->first] = it->second;
    }

    // Print the paths
    cout << "Path from the source to the destination-1: ";
    for (int i = 0; i < path1.size(); i++) {
        cout << "(" << path1[i].first << ", " << path1[i].second << ") ";
    }
    cout << endl;

    cout << "Path from the source to the destination-2: ";
    for (int i = 0; i < path2.size(); i++) {
        cout << "(" << path2[i].first << ", " << path2[i].second << ") ";
    }
    cout << endl;

    cout << "Path from the source to the destination-3: ";
    for (int i = 0; i < path3.size(); i++) {
        cout << "(" << path3[i].first << ", " << path3[i].second << ") ";
    }
    cout << endl;

    cout << "Path from the source to the destination-4: ";
    for (int i = 0; i < path4.size(); i++) {
        cout << "(" << path4[i].first << ", " << path4[i].second << ") ";
    }
    cout << endl;

    // cout << "Welcome to the Collision Detection System!" << endl;
    // cout << "For testing purposes the grid is a 9x10 grid." << endl;
    //
    // cout << "Enter the number of paths you would like to simulate: ";
    // int numPaths;
    // cin >> numPaths;
    //
    // // Display the grid
    // cout << "---------------------------------" << endl;
    // cout << "Here is the grid: " << endl;
    // for (int j = 0; j < ROW; j++) {
    //     for (int k = 0; k < COL; k++) {
    //         cout << (grid[j][k] == 1 ? '_' : '#') << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "---------------------------------" << endl;
    //
    // // Store the computed paths
    // vector<vector<pair<int, int>>> paths;
    //
    // for (int i = 0; i < numPaths; i++) {
    //     cout << "\nEnter the source and destination for path " << 'A' + i << " (x1 y1 x2 y2) Separated by space: " << endl;
    //     cout << "Note: choose unique points for all paths" << endl;
    //     int x1, y1, x2, y2;
    //     cin >> x1 >> y1 >> x2 >> y2;
    //     Pair src = make_pair(x1, y1);
    //     Pair dest = make_pair(x2, y2);
    //     vector<pair<int, int>> path = aStarSearch(grid, src, dest, reservationTable, edgeReservationTable, 0);
    // }
    //
    // cout << "\nLoading paths...\n" << endl;
    //
    // Find the largest path
    // int largestPath = 0;
    // for (int i = 0; i < 4; i++) {
    //     if (paths[i].size() > largestPath) {
    //         largestPath = paths[i].size();
    //     }
    // }
    //
    // // Print the paths
    // cout << endl;
    // for (int i = 0; i < 4; i++) {
    //     cout << "Path " << static_cast<char>('A' + i) << ": ";
    //     for (int j = 0; j < paths[i].size(); j++) {
    //         cout << "(" << paths[i][j].first << ", " << paths[i][j].second << ") ";
    //     }
    //     cout << endl;
    // }


    // Using these vectors, we can find where the intersections are.
    // Create a new vector to store these paths
    vector<vector<pair<int, int>>> allPaths;

    allPaths.push_back(path1);
    allPaths.push_back(path2);
    allPaths.push_back(path3);
    allPaths.push_back(path4);

    // Find the intersection
    vector<pair<int, intersection>> intersections = findIntersections(allPaths);

    cout << endl;

    // Print the intersections
    for (int i = 0; i < intersections.size(); i++) {
        cout << "Intersection at time : " << intersections[i].first << ", at grid location: (" << intersections[i].second.x << ", " << intersections[i].second.y << ") involving paths: ";
        for (int j = 0; j < intersections[i].second.involved.size(); j++) {
            cout << intersections[i].second.involved[j] << " ";
        }
        cout << endl;
    }

    return (0);
}

// Things to do:
// Add max flow algorithm to fix traffic jams
// Make more modular
// Add more test cases
// Add the randomized sequential path executions with analytics
// Add the ability to add more paths
// Add a web interface