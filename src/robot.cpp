/*
robot.cpp
April 09, 2023
MDSTEM-RCJ-2023
William Zheng
*/

#include <iostream>
#include <vector>

using namespace std;

// Add coordinates
void add_coords(vector<vector<int>> &coords, int x, int y) { coords.push_back({x, y}); }

// Add tile
void add_tile(vector<vector<int>> &coords, vector<vector<int>> &adj, vector<int> sensors,
              vector<int> &tile, int &n, bool init, int x, int y) {
  for (int i = 0; i < sensors.size(); i++) {
    if (sensors[i] == 0) {
      switch (i) {
      case 0:
        add_coords(coords, x - 1, y);
        break;
      case 1:
        add_coords(coords, x, y + 1);
        break;
      case 2:
        add_coords(coords, x + 1, y);
        break;
      case 3:
        add_coords(coords, x, y - 1);
        break;
      }
      if (init == true) {
        adj[tile[0]].push_back(tile[1]);
        adj[tile[1]].push_back(tile[0]);
        tile[1]++;
      } else {
        adj[tile[0]].push_back(tile[1]);
        vector<int> new_tile = {tile[0]};
        adj.push_back(new_tile);
        tile[1]++;
      }
    }
  }
}

// Read sensors
void read_sensors(vector<int> &sensors, int &n, bool init) {
  cout << "\nread_sensors" << endl;
  if (init == true) {
    cin >> sensors[0] >> sensors[1] >> sensors[2] >> sensors[3];
  } else {
    cin >> sensors[0] >> sensors[1] >> sensors[2];
  }

  for (int i = 0; i < sensors.size(); i++) {
    if (sensors[i] == 0) {
      n++;
    }
  }
}

// Print coordinates
void print_coords(vector<vector<int>> coords) {
  cout << "\nprint_coords" << endl;
  for (int i = 0; i < coords.size(); i++) {
    cout << i << ": (" << coords[i][0] << ", " << coords[i][1] << ')' << endl;
  }
}

// Print adjacency list
void print_adj(vector<vector<int>> adj, int n) {
  cout << "\nprint_adj" << endl;
  for (int i = 0; i < n; i++) {
    cout << i << ": ";
    for (int &x : adj[i]) {
      cout << x << " ";
    }
    cout << endl;
  }
}

// Print tile and position coords
void print_pos(vector<int> tile, vector<int> pos) {
  cout << "\nprint_pos\ntile: " << tile[0] << "\npos: (" << pos[0] << ',' << pos[1] << ")\n";
}

int main() {
  int n = 1;
  int m = 0;
  vector<int> tile = {0, 1};
  vector<int> pos = {0, 0, 0};
  vector<int> sensors(4);
  bool init = true;

  // Initialize coordinates
  vector<vector<int>> coords;
  add_coords(coords, 0, 0);

  // Initialize starting tile
  read_sensors(sensors, n, init);
  vector<vector<int>> adj(n);
  add_tile(coords, adj, sensors, tile, n, init, pos[0], pos[1]);

  init = false;
  sensors.pop_back();

  print_pos(tile, pos);
  print_coords(coords);
  print_adj(adj, n);

  // Non-init run
  read_sensors(sensors, n, init);
  add_tile(coords, adj, sensors, tile, n, init, pos[0], pos[1]);
  print_pos(tile, pos);
  print_coords(coords);
  print_adj(adj, n);

  system("pause");
}

/*
TO DO LIST

Loop while there are still unexplored nodes

Tile alignment and color check

Wall detection

Add node to maze graph

Victim detection

Rescue kit deployment

Movement to explore next node
*/