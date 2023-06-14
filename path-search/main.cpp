#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#define int int64_t

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("tune=native")

/////   STRUCTURES WITH ALGORITHMS  /////

struct Edge {
    int v;
    int u;
    int w;

    Edge(const int& v, const int& u, const int& w) {
        this->v = v;
        this->u = u;
        this->w = w;
    }
};

struct Graph {
    const int inf = 1e18;
    int vertices_number;
    int edges_number; // Новое поле для хранения числа ребер
    std::vector<Edge> edges;
    std::vector<std::vector<std::pair<int, int>>> adj_list;
    std::vector<std::vector<int>> adj_list_ford;
    std::vector<std::vector<int>> adj_matrix;

    Graph() {}

    Graph(const int& n) {
        vertices_number = n;
        edges_number = 0; // Инициализация числа ребер
        for (auto i = 0; i < vertices_number; ++i) {
            adj_list.emplace_back();
            adj_list_ford.emplace_back();
        }
        adj_matrix.resize(vertices_number, std::vector<int>(vertices_number, inf));
    }

    void add(const int& v, const int& u, const int& w) {
        if (v >= vertices_number || u >= vertices_number) {
            return;
        }
        edges.emplace_back(v, u, w);
        edges.emplace_back(u, v, w);
        adj_list[v].emplace_back(u, w);
        adj_list[u].emplace_back(v, w);
        adj_list_ford[v].emplace_back(u);
        adj_list_ford[u].emplace_back(v);
        adj_matrix[v][u] = w;
        adj_matrix[u][v] = w;
        ++edges_number; // Увеличение числа ребер
    }

    void dijkstra(const int& root, std::vector<int>& dist) {
        dist[root] = 0;
        std::set<std::pair<int, int>> set;
        set.emplace(dist[root], root);
        while (!set.empty()) {
            int v = set.begin()->second;
            set.erase(set.begin());
            for (auto j = 0; j < adj_list[v].size(); ++j) {
                int u = adj_list[v][j].first;
                int w = adj_list[v][j].second;
                if (dist[v] + w < dist[u]) {
                    set.erase({ dist[u], u });
                    dist[u] = dist[v] + w;
                    set.emplace(dist[u], u);
                }
            }
        }
    }

    void dfs(const int& root, std::vector<int>& dist) {
        dist[root] = -inf;
        for (auto const& u : adj_list_ford[root]) {
            if (dist[u] != -inf) {
                dfs(u, dist);
            }
        }
    }

    void bellmanFord(const int& root, std::vector<int>& dist) {
        int v = root;
        dist[v] = 0;
        std::vector<int> parent(vertices_number, -1);
        int negative_cycle_flag;
        int m = static_cast<int>(edges.size());
        for (auto i = 0; i < vertices_number - 1; ++i) {
            negative_cycle_flag = 0;
            for (auto j = 0; j < m; ++j) {
                v = edges[j].v;
                int u = edges[j].u;
                int w = edges[j].w;
                if (dist[v] < inf) {
                    if (dist[u] > dist[v] + w) {
                        dist[u] = std::max(-inf, dist[v] + w);
                        parent[u] = v;
                    }
                }
            }
        }
        for (auto j = 0; j < m; ++j) {
            v = edges[j].v;
            int u = edges[j].u;
            int w = edges[j].w;
            if (dist[v] < inf && dist[u] > dist[v] + w) {
                dist[u] = -inf;
                negative_cycle_flag = 1;
                dfs(u, dist);
            }
        }
        if (negative_cycle_flag == 1) {
            for (auto i = 0; i < vertices_number; ++i) {
                if (dist[i] == -inf) {
                    dist[i] = inf;
                }
            }
        }
    }

    void floydWarshall(std::vector<std::vector<int>>& dist) {
        int n = vertices_number;
        for (auto i = 0; i < n; ++i) {
            for (auto j = 0; j < n; ++j) {
                if (i == j) {
                    dist[i][j] = 0;
                }
            }
        }
        for (auto i = 0; i < edges.size(); ++i) {
            int v = edges[i].v;
            int u = edges[i].u;
            int w = edges[i].w;
            dist[v][u] = std::min(dist[v][u], w);
        }
        for (auto k = 0; k < n; ++k) {
            for (auto i = 0; i < n; ++i) {
                for (auto j = 0; j < n; ++j) {
                    if (dist[i][k] < inf && dist[k][j] < inf) {
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
    }
};

/////   GRAPHS GENERATORS  /////

// Генерация полного графа
Graph generateCompleteGraph(const int& n) {
    Graph completeGraph(n);
    for (auto i = 0; i < n; ++i) {
        for (auto j = i + 1; j < n; ++j) {
            int w = rand() % 100 + 1; // случайный вес ребра от 1 до 100
            completeGraph.add(i, j, w);
        }
    }
    return completeGraph;
}

// Генерация связного графа с заданным коэффициентом плотности
Graph generateConnectedGraph(const int& n) {
    const double density = 0.5;
    Graph connectedGraph(n);
    for (auto i = 0; i < n - 1; ++i) {
        int w = rand() % 100 + 1; // случайный вес ребра от 1 до 100
        connectedGraph.add(i, i + 1, w);
    }
    int m = static_cast<int>(density * (n * (n - 1)) / 2 - (n - 1));
    for (auto i = 0; i < m; ++i) {
        int v = rand() % n;
        int u = rand() % n;
        if (v != u) {
            int w = rand() % 100 + 1; // случайный вес ребра от 1 до 100
            connectedGraph.add(v, u, w);
        }
    }
    return connectedGraph;
}

// Функция для генерации разреженного графа (дерева)
Graph generateSparseGraph(const int& n) {
    Graph graph(n);

    for (int i = 1; i < n; ++i) {
        int weight = rand() % 100 + 1; // случайный вес ребра от 1 до 100
        graph.add(i, i - 1, weight);
    }

    return graph;
}

/////   SAVE DATA TO FILE  /////

void saveLogToFile(const std::string& fileName, const std::string& graphType, int vertexCount, int edgeCount, const std::string& algorithm, const std::chrono::nanoseconds& duration) {
    std::ofstream logFile(fileName);
    if (logFile.is_open()) {
        logFile << "Graph Type: " << graphType << "\n";
        logFile << "Vertex Count: " << vertexCount << "\n";
        logFile << "Edge Count: " << edgeCount << "\n";
        logFile << "Algorithm: " << algorithm << "\n";
        logFile << "Duration: " << duration.count() << " nanoseconds\n";
        logFile.close();
    }
}

/////   GENERATE GRAPHS AND LAUNCH ALGORITHMS USING TIME MEASUREMENT  /////

using Func = std::function<Graph(int)>;

void generateData(const std::string& graph_type, const Func& func) {

    std::cout << "generate graph " << graph_type << "\n";

    // Генерируем полные графы
    for (int n = 10; n <= 1010; n += 50) {
        Graph graph = func(n);

        std::cout << "generated graph with v_n = " << n << "\n";

        std::vector<int> distDijkstra(n, graph.inf);
        auto start = std::chrono::high_resolution_clock::now();
        graph.dijkstra(0, distDijkstra);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::string logFileName = "logs/complete_graph_" + std::to_string(n) + "_Dijkstra.txt";
        saveLogToFile(logFileName, graph_type, n, graph.edges_number, "Dijkstra", duration);

        std::cout << "finish dijkstra\n";

        std::vector<int> distBellmanFord(n, graph.inf);
        start = std::chrono::high_resolution_clock::now();
        graph.bellmanFord(0, distBellmanFord);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        logFileName = "logs/complete_graph_" + std::to_string(n) + "_BellmanFord.txt";
        saveLogToFile(logFileName, graph_type, n, graph.edges_number, "Bellman-Ford", duration);

        std::cout << "finish bellman-ford\n";

        auto distFloydWarshall = graph.adj_matrix;
        start = std::chrono::high_resolution_clock::now();
        graph.floydWarshall(distFloydWarshall);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        logFileName = "logs/complete_graph_" + std::to_string(n) + "_FloydWarshall.txt";
        saveLogToFile(logFileName, graph_type, n, graph.edges_number, "Floyd-Warshall", duration);

        std::cout << "finish floyd-warshall\n";
    }

    // Аналогичные циклы для связных графов и разреженных графов
}

/////   MAIN: LAUNCH GENERATION AND FILE OUTPUT  /////

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // Создаем директорию для сохранения лог-файлов
    std::filesystem::create_directory("logs");

    const Func funcs[3] = {
            generateCompleteGraph,
            generateConnectedGraph,
            generateSparseGraph
    };

    for (auto i = 0; i < 3; ++i) {
        auto type = (i == 0) ? "complete" : ((i == 1) ? "connected" : "sparse");
        generateData(type, funcs[i]);
    }

    return 0;
}
