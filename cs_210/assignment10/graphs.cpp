#include <iostream>
#include <vector>
using namespace std;

class Graph {
public:
    int numVertices;          // how many vertices exist right now
    vector<int> adjList[20];  

    Graph() {
        numVertices = 0;
    }

    // add a vertex
    void addVertex() {
        numVertices = numVertices + 1;
    }

    // add an edge (undirected)
    void addEdge(int a, int b) {
        adjList[a].push_back(b);
        adjList[b].push_back(a);
    }

    // print graph
    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            cout << i << " -> ";
            for (int j = 0; j < adjList[i].size(); j++) {
                cout << adjList[i][j] << " ";
            }
            cout << endl;
        }
    }

    // breadth-first search
    void bfs(int start) {
        bool visited[20];
        for (int i = 0; i < 20; i++) {
            visited[i] = false;
        }

        int queue[20];
        int front = 0;
        int back = 0;

        visited[start] = true;
        queue[back] = start;
        back = back + 1;

        while (front < back) {
            int current = queue[front];
            front = front + 1;

            cout << current << " ";

            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i];
                if (visited[neighbor] == false) {
                    visited[neighbor] = true;
                    queue[back] = neighbor;
                    back = back + 1;
                }
            }
        }
        cout << endl;
    }

    void dfsHelper(int current, bool visited[]) {
        visited[current] = true;
        cout << current << " ";

        for (int i = 0; i < adjList[current].size(); i++) {
            int neighbor = adjList[current][i];
            if (visited[neighbor] == false) {
                dfsHelper(neighbor, visited);
            }
        }
    }

    // depth first search
    void dfs(int start) {
        bool visited[20];
        for (int i = 0; i < 20; i++) {
            visited[i] = false;
        }
        dfsHelper(start, visited);
        cout << endl;
    }

    vector<int> shortestPath(int start, int target) {
        bool visited[20];
        int parent[20];
        for (int i = 0; i < 20; i++) {
            visited[i] = false;
            parent[i] = -1;
        }

        int queue[20];
        int front = 0;
        int back = 0;

        visited[start] = true;
        queue[back] = start;
        back = back + 1;

        while (front < back) {
            int current = queue[front];
            front = front + 1;

            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i];
                if (visited[neighbor] == false) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    queue[back] = neighbor;
                    back = back + 1;
                }
            }
        }

        vector<int> path;

        if (visited[target] == false) {
            return path; 
        }

        int walker = target;
        while (walker != -1) {
            path.push_back(walker);
            walker = parent[walker];
        }

        vector<int> flipped;
        for (int i = path.size() - 1; i >= 0; i--) {
            flipped.push_back(path[i]);
        }
        return flipped;
    }
};

int main() {
    Graph g;

    for (int i = 0; i < 8; i++) {
        g.addVertex();
    }

    // 10 edges
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(3, 6);

    cout << "Adjacency list:" << endl;
    g.printGraph();
    cout << endl;

    cout << "BFS order starting at 0: ";
    g.bfs(0);
    cout << "DFS order starting at 0: ";
    g.dfs(0);
    cout << endl;

    vector<int> p1 = g.shortestPath(0, 6);
    cout << "Shortest path from 0 to 6: ";
    if (p1.size() == 0) {
        cout << "No path" << endl;
    } else {
        for (int i = 0; i < p1.size(); i++) {
            cout << p1[i] << " ";
        }
        cout << endl;
    }

    vector<int> p2 = g.shortestPath(0, 7);
    cout << "Shortest path from 0 to 7: ";
    if (p2.size() == 0) {
        cout << "No path (7 is unreachable)" << endl;
    } else {
        for (int i = 0; i < p2.size(); i++) {
            cout << p2[i] << " ";
        }
        cout << endl;
    }

    return 0;
}