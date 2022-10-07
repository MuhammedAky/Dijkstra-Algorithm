#include <iostream>
#include <limits.h>
#include <vector>
#include <map>

#define V 7

template<typename T>
bool contains(std::vector<T> v, T x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

namespace dijkstra {
   typedef std::vector<std::pair<int, double>> Queue;
	typedef std::pair<int, double> QElement;
	double** dijkstra(double graph[V][V], int src);
}

int findMinOnQueue(dijkstra::Queue queue);
int findOnQueue(dijkstra::Queue queue, int element);
double calculateDistance(double weight[V], int prevNode[V], int element);
double calculateDistance(double** shortestPathsTree, int src, int goal);

void printArray(double graph[V][V]) {
    for (int r = 0; r < V; r++) {
        for (int c = 0; c < V; c++) {
            std::cout << graph[r][c] << ", ";
        }
        std::cout << std::endl;
    }
}

void printArray(double** graph) {
    for (int r = 0; r < V; r++) {
        for (int c = 0; c < V; c++) {
            std::cout << graph[r][c] << ", ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "Hello, Dijkstra!" << std::endl;

    char nodes[V] = {'A','B','C','D','E','F','G'};

    double graph[V][V] = {
	   // A, B, C, D, E, F, G
		{ 0, 5, 3, 0, 0, 0, 0 },
		{ 0, 0, 2, 0, 3, 0, 1 },
		{ 0, 0, 0, 7, 7, 0, 0 },
		{ 2, 0, 0, 0, 0, 6, 0 },
		{ 0, 0, 0, 2, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0 }
	};

    printArray(graph);
    std::cout << std::endl << std::endl;

    double** shortestPathsTree = dijkstra::dijkstra(graph, 0);

    printArray(shortestPathsTree);

    int start = 0;
    int goal = 3;

    std::cout << std::endl << nodes[start] << "->" << nodes[goal] << ": " << calculateDistance(shortestPathsTree, start, goal);

    return 0;
}

double** dijkstra::dijkstra(double graph[V][V], int src) {
    double weight[V] = { INT_MAX };
    int prevNode[V] = { 0 };

    weight[src] = 0;
    prevNode[src] = src;

    std::vector<int> visited;

    Queue queue;
	queue.push_back(std::pair<int, double>(src, 0));

    while (queue.size() != 0) {
        int smallest = findMinOnQueue(queue);

        visited.push_back(smallest);
        queue.erase(queue.begin() + findOnQueue(queue, smallest));

        for (int i = 0; i < V; i++) {
            auto loc = findOnQueue(queue, i);

            if (graph[smallest][i] == 0) {
                continue;
            }
            else if (contains<int>(visited, i)) {
                continue;
            }
            else if (loc != -1) {
                double currentDistance = calculateDistance(weight, prevNode, i);

                double newDistance = calculateDistance(weight, prevNode, smallest) + graph[smallest][i];

                if (newDistance < currentDistance) {
                    queue.erase(queue.begin() + loc);
                    queue.push_back(dijkstra::QElement(i, newDistance));
                    weight[i] = graph[smallest][i];
                    prevNode[i] = smallest;
                }
            }
            else {
                weight[i] = graph[smallest][i];
                prevNode[i] = smallest;

                queue.push_back(dijkstra::QElement(i, calculateDistance(weight, prevNode, i)));
            }
        }
    }

    double** shortestPathsTree = 0;
	shortestPathsTree = new double*[V];
	for (int r = 0; r < V; r++) {
		shortestPathsTree[r] = new double[V];

		for (int c = 0; c < V; c++) {
			shortestPathsTree[r][c] = 0;
		}
	}

    for (int i = 0; i < V; i ++) {
        shortestPathsTree[prevNode[i]][i] = weight[i];
    }

    return shortestPathsTree;
}

int findMinQueue(dijkstra::Queue queue) {
    double min = queue.at(0).second;
    int idx = queue.at(0).first;

    for (int i = 1, size = queue.size(); i < size; i++) {
        if (queue.at(i).second < min) {
            min = queue.at(i).second;
            idx = queue.at(i).first;
        }
    }

    return idx;
}

int findOnQueue(dijkstra::Queue queue, int element) {
    for (int i = 0, size = queue.size(); i < size; i++) {
        if (queue.at(i).first == element) {
            return i;
        }
    }

    return -1;
}

double calculateDistance(double weight[V], int prevNode[V], int element) {
    if (prevNode[element] == element) {
        return 0;
    }

    return weight[element] + calculateDistance(weight, prevNode, prevNode[element]);
}

double calculateDistance(double** shortestPathsTree, int src, int goal) {
    if (src == goal) {
        return 0;
    }

    for (int r = 0; r < V; r++) {
        if (shortestPathsTree[r][goal] != 0) {
            return shortestPathsTree[r][goal] + calculateDistance(shortestPathsTree, src, r);
        }
    }
}