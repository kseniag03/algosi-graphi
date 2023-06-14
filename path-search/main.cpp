#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#define int int64_t

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("tune=native")

struct Edge {
    int v;
    int u;
    int w;

    Edge(const int &v, const int &u, const int &w) {
        this->v = v;
        this->u = u;
        this->w = w;
    }
};

struct Graph {
    const int inf = 1e18;
    int vertices_number;
    std::vector<Edge> edges;
    std::vector<std::vector<std::pair<int, int>>> adj_list;
    std::vector<std::vector<int>> adj_list_ford;
    std::vector<std::vector<int>> adj_matrix;

    Graph(const int &n) {
        vertices_number = n;
        for (auto i = 0; i < vertices_number; ++i) {
            adj_list.emplace_back();
            adj_list_ford.emplace_back();
        }
        adj_matrix.resize(vertices_number, std::vector<int>(vertices_number, inf));
    }

    void add(const int &v, const int &u, const int &w) {
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
    }

    void dijkstra(const int &root, std::vector<int> &dist) {
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
                    set.erase({dist[u], u});
                    dist[u] = dist[v] + w;
                    set.emplace(dist[u], u);
                }
            }
        }
    }

    void dfs(const int &root, std::vector<int> &dist) {
        dist[root] = -inf;
        for (auto const &u : adj_list_ford[root]) {
            if (dist[u] != -inf) {
                dfs(u, dist);
            }
        }
    }

    void bellmanFord(const int &root, std::vector<int> &dist) {
        int v = root;
        dist[v] = 0;
        std::vector<int> parent(vertices_number, -1);
        int negative_cycle_flag;
        int m = static_cast<int>(edges.size());
        for (auto i = 0; i < vertices_number; ++i) {
            negative_cycle_flag = -1;
            for (auto j = 0; j < m; ++j) {
                if (dist[edges[j].v] < inf) {
                    if (dist[edges[j].u] > dist[edges[j].v] + edges[j].w) {
                        dist[edges[j].u] = std::max(-inf, dist[edges[j].v] + edges[j].w);
                        parent[edges[j].u] = edges[j].v;
                        negative_cycle_flag = edges[j].u;
                    }
                }
            }
        }
        if (negative_cycle_flag == -1) {
            // no negative cycle from v
        } else {
            int vertex = negative_cycle_flag;
            for (auto i = 0; i < vertices_number; ++i) {
                vertex = parent[vertex];
            }
            std::vector<int> path;
            for (auto cur = vertex;; cur = parent[cur]) {
                path.emplace_back(cur);
                if (cur == vertex && path.size() > 1) {
                    break;
                }
            }
            // negative cycle found
            for (auto const &i : path) {
                dfs(i, dist);
            }
        }
    }

    std::vector<std::vector<int>> floydWarshell() const {
        auto dist = adj_matrix;
        for (auto k = 0; k < vertices_number; ++k) {
            for (auto i = 0; i < vertices_number; ++i) {
                for (auto j = 0; j < vertices_number; ++j) {
                    if (dist[i][k] < inf && dist[k][j] < inf) {
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
        return dist;
    }
};

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    auto graph = Graph(n);
    for (auto i = 0; i < m; ++i) {
        int a, b, c;
        std::cin >> a >> b >> c;
        graph.add(a, b, c);
    }

    std::vector<int> distDijkstra(n, graph.inf);
    graph.dijkstra(0, distDijkstra);
    for (auto i = 1; i < n; ++i) {
        std::cout << distDijkstra[i] << "\n";
    }

    std::vector<int> distBellmanFord(n, graph.inf);
    graph.bellmanFord(0, distBellmanFord);
    for (auto i = 1; i < n; ++i) {
        if (distBellmanFord[i] == -graph.inf) {
            std::cout << "-inf\n";
            continue;
        }
        std::cout << distBellmanFord[i] << "\n";
    }

    auto distFloydWarshell = graph.floydWarshell();
    for (auto i = 0; i < n; ++i) {
        for (auto j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            auto d = (distFloydWarshell[i][j] != graph.inf) ? distFloydWarshell[i][j] : -1;
            std::cout << i << " " << j << " " << d << "\n";
        }
    }

    return 0;
}
