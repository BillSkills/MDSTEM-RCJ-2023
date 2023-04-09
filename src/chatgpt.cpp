#include <iostream>
#include <vector>
using namespace std;

int main() {
  int n;
  cout << "Enter # of vertices: ";
  cin >> n;

  // Create a 2D vector to represent the graph
  vector<vector<int>> graph(n);

  // Add edges to the graph
  int m;
  cout << "Enter # of edges: ";
  cin >> m;
  cout << "Enter edges (u v):\n";
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  // Print the initial graph
  cout << "Graph:\n";
  for (int i = 0; i < n; i++) {
    cout << i << ": ";
    for (int j = 0; j < graph[i].size(); j++) {
      cout << graph[i][j] << " ";
    }
    cout << "\n";
  }

  // Allow the user to add new vertices to the graph
  char choice;
  do {
    cout << "Do you want to add a new vertex? (y/n): ";
    cin >> choice;
    if (choice == 'y') {
      vector<int> new_vertex;
      int l;
      cout << "Enter # of edges: ";
      cin >> l;
      cout << "Enter edges (u v):\n";
      for (int j = 0; j < l; j++) {
        int u, v;
        cin >> u >> v;
        new_vertex.push_back(v);
      }
      graph.push_back(new_vertex);
      n++;
    }
  } while (choice == 'y');

  // Print the updated graph
  cout << "Graph:\n";
  for (int i = 0; i < n; i++) {
    cout << i << ": ";
    for (int j = 0; j < graph[i].size(); j++) {
      cout << graph[i][j] << " ";
    }
    cout << "\n";
  }

  system("pause");
}
