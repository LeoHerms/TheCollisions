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

    // Source is the left-most bottom-most corner
    Pair src1 = make_pair(4, 0);
    Pair dest1 = make_pair(4, 1);   // 5, 0
    vector<pair<int, int>> path1 = aStarSearch(grid, src1, dest1, reservationTable, edgeReservationTable, 0);

    // 2nd source and destination
    Pair src2 = make_pair(4, 1);
    Pair dest2 = make_pair(4, 0);
    vector<pair<int, int>> path2 = aStarSearch(grid, src2, dest2, reservationTable, edgeReservationTable, 0);   // Added for more commentary

    // 3rd source and destination
    // Pair src3 = make_pair(3, 3);
    // Pair dest3 = make_pair(5, 3);
    // vector<pair<int, int>> path3 = aStarSearch(grid, src3, dest3, reservationTable, edgeReservationTable, 0);

    // 4th source and destination (opposite diagonal)
    // Pair src4 = make_pair(2, 2);
    // Pair dest4 = make_pair(5, 2);
    // vector<pair<int, int>> path4 = aStarSearch(grid, src4, dest4, reservationTable, edgeReservationTable, 0);

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

    // cout << "Path from the source to the destination-3: ";
    // for (int i = 0; i < path3.size(); i++) {
    //     cout << "(" << path3[i].first << ", " << path3[i].second << ") ";
    // }
    // cout << endl;

    // cout << "Path from the source to the destination-4: ";
    // for (int i = 0; i < path4.size(); i++) {
    //     cout << "(" << path4[i].first << ", " << path4[i].second << ") ";
    // }
    // cout << endl;

    // Using these vectors, we can find where the intersections are.
    // Create a new vector to store these paths
    vector<vector<pair<int, int>>> allPaths;

    allPaths.push_back(path1);
    allPaths.push_back(path2);
    // allPaths.push_back(path3);
    // allPaths.push_back(path4);

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