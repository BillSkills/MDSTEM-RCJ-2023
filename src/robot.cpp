/*
robot.cpp
April 09, 2023
MDSTEM-RCJ-2023
William Zheng
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Add coordinates
void add_coords(vector<vector<int>> &coords, int x, int y) { coords.push_back({x, y}); }

// Add tile
void add_tiles(vector<vector<int>> &coords, vector<vector<int>> &adj, vector<int> sensors,
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

      } else {
        adj[tile[0]].push_back(tile[1]);
        vector<int> new_tile = {};
        adj.push_back(new_tile);
      }
      tile[1]++;
    }
  }
}

// Read sensors
void read_sensors(vector<int> &sensors, int &n, bool init) {
  if (init == true) {
    cout << "\nEnter sensor data (4):" << endl;
    cin >> sensors[0] >> sensors[1] >> sensors[2] >> sensors[3];
  } else {
    cout << "\nEnter sensor data (3):" << endl;
    cin >> sensors[0] >> sensors[1] >> sensors[2];
  }

  for (int i = 0; i < sensors.size(); i++) {
    if (sensors[i] == 0) {
      n++;
    }
  }
}

// Print coordinates list
void print_coords(vector<vector<int>> coords) {
  cout << "\nCoordinates List" << endl;
  for (int i = 0; i < coords.size(); i++) {
    cout << i << ": (" << coords[i][0] << ", " << coords[i][1] << ')' << endl;
  }
}

// Print adjacency list
void print_adj(vector<vector<int>> adj, int n) {
  cout << "\nAdjacency List" << endl;
  for (int i = 0; i < n; i++) {
    cout << i << " <=> ";
    for (int &x : adj[i]) {
      cout << x << " ";
    }
    cout << endl;
  }
}

// Print tile and position coords
void print_pos(vector<int> tile, vector<int> pos) {
  cout << "\nCurrent Position\nTile: " << tile[0] << "\nCoords: (" << pos[0] << ',' << pos[1]
       << ")\n";
}

// Print tile colors
void print_colors(vector<int> colors) {
  cout << "\nColors" << endl;
  for (int i = 0; i < colors.size(); i++) {
    cout << i << ": " << colors[i] << endl;
  }
}

// Move function
void move(vector<vector<int>> coords, vector<int> &tile, vector<int> &pos) {
  char move;
  cout << "\nEnter move (L/R, F/B): ";
  cin >> move;

  pos[2] = pos[0];
  pos[3] = pos[1];

  switch (move) {
  case 'L':
    pos[0]--;
    pos[4] = 0;
    break;
  case 'R':
    pos[0]--;
    pos[4] = 2;
    break;
  case 'F':
    pos[1]++;
    pos[4] = 1;
    break;
  case 'B':
    pos[1]--;
    pos[4] = 3;
    break;
  }
  vector<vector<int>> newpos = {{pos[0], pos[1]}};
  for (int i = 0; i < coords.size(); i++) {
    if (coords[i] == newpos[0]) {
      tile[0] = i;
    }
  }
}

// Pause screen
void pause_screen() {
  cout << endl;
  system("pause");
  system("cls");
}

// Depth first search
void dfs(vector<vector<int>> &adj, vector<int> &colors, int v) {
  colors[v] = 1;
  cout << "Tile " << v << endl;
  for (int u : adj[v]) {
    if (colors[u] == 0)
      dfs(adj, colors, u);
  }
  colors[v] = 2;
}

void color_tiles(vector<vector<int>> adj, vector<int> &colors, vector<int> sensors,
                 vector<int> tile, bool init) {
  if (find(sensors.begin(), sensors.end(), 0) != sensors.end()) {
    colors[tile[0]] = 1;
  } else {
    colors[tile[0]] = 2;
  }

  for (int i = 0; i < adj[tile[0]].size(); i++) {
    if (init == true) {
      colors[adj[tile[0]][i]] = 0;
    } else {
      colors.push_back(0);
    }
  }
}

// Main
int main() {
  int n = 1;
  int m = 0;
  vector<int> tile = {0, 1};
  vector<int> pos = {0, 0, 0, 0, 0};
  vector<int> sensors(4);
  bool init = true;

  // Init sequence
  vector<vector<int>> coords;
  add_coords(coords, pos[0], pos[1]);

  read_sensors(sensors, n, init);
  vector<vector<int>> adj(n);
  vector<int> colors(n);

  add_tiles(coords, adj, sensors, tile, n, init, pos[0], pos[1]);
  color_tiles(adj, colors, sensors, tile, init);

  init = false;
  sensors.pop_back();
  print_colors(colors);
  // print_pos(tile, pos);
  // print_adj(adj, n);
  // print_coords(coords);
  pause_screen();

  do {
    move(coords, tile, pos);
    if (adj[tile[0] - 1].size() < 2) {
      colors[tile[0] - 1] = 2;
    }
    read_sensors(sensors, n, init);
    add_tiles(coords, adj, sensors, tile, n, init, pos[0], pos[1]);
    color_tiles(adj, colors, sensors, tile, init);
    print_colors(colors);
    if (adj[tile[0]].size() < 1) {
      while (adj[tile[0]].size() < 1) {
        tile[0]--;
      }
      pos[2] = pos[0];
      pos[3] = pos[1];
      pos[0] = coords[tile[0]][0];
      pos[1] = coords[tile[0]][1];

      cout << "\nReturning to last unexplored tile" << endl;
    }
    print_pos(tile, pos);
    pause_screen();
  } while (find(colors.begin(), colors.end(), 0) != colors.end());
  print_colors(colors);
  pause_screen();
}

/*
TO DO LIST

Loop while there are still unexplored nodes

Tile alignment and color check

[CHECK] Wall detection

[CHECK] Add node to maze graph

Victim detection

Rescue kit deployment

Movement to explore next node
*/