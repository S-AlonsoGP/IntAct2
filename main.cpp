#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

// helper types
typedef std::vector<std::vector<int>> mat ;
typedef std::vector<std::pair<int, int>> coords ;
typedef std::pair<double, double> Point;
typedef std::vector<Point> Polygon;

// MST helper function
int min_key(std::vector<int>& key, std::vector<bool>& mst_set) {
    int mn = INT_MAX, min_index = -1 ;
    for (unsigned int i = 0 ; i < key.size() ; i++) {
        if (!mst_set[i] && key[i] < mn) {
            mn = key[i] ;
            min_index = i ;
        }
    }
    return min_index;
}

// Minimum Spanning Tree (MST) using Prim's Algorithm (Adjacency matrix)
/*int mst(unsigned int n, mat& adj_matrix) {
    std::vector<int> key(n, INT_MAX);
    int res = 0 ;
    // MST is a vector that tracks vertex inclusion
    std::vector<bool> mst_set(n, false);
    
    key[0] = 0 ;

    for (unsigned int c = 0 ; c < n; c++) {
        int u = min_key(key, mst_set) ;
        mst_set[u] = true ;
        res += key[u] ;
        
        for (int v = 0; v < n ; v++) {
            if (adj_matrix[u][v] != 0 &&
            !mst_set[v] &&
            adj_matrix[u][v] < key[v]) {
                key[v] = adj_matrix[u][v] ;
            }
        }
    }

    return res;
}

*/
// Devuelve el costo total y llena 'edges' con los arcos del MST
int mst(unsigned int n, mat& adj_matrix,
        std::vector<std::pair<int,int>>& edges) {
    std::vector<int> key(n, INT_MAX);
    std::vector<int> parent(n, -1);          // NUEVO
    std::vector<bool> mst_set(n, false);
    int res = 0;

    key[0] = 0;

    for (unsigned int c = 0; c < n; c++) {
        int u = min_key(key, mst_set);
        mst_set[u] = true;
        res += key[u];

        if (parent[u] != -1) {               // NUEVO: guardar el arco
            edges.push_back({parent[u], u});
        }

        for (unsigned int v = 0; v < n; v++) {
            if (adj_matrix[u][v] != 0 &&
                !mst_set[v] &&
                adj_matrix[u][v] < key[v]) {
                key[v] = adj_matrix[u][v];
                parent[v] = u;               // NUEVO
            }
        }
    }
    return res;
}

/*
// TSP using Top-Down Dynamic Programming with Bitmasking
int total_cost(int mask, int curr, mat& cost, mat& dp) {
    int n = cost.size() ;

    // base case
    if (mask == (1 << n) - 1) {
        return cost[curr][0];
    }

    if (dp[curr][mask] != -1) {
        return dp[curr][mask];
    }

    int ans = INT_MAX;

    for (int i = 0 ; i < n ; i++) {
        if ((mask & (1 << i)) == 0) {
            // unvisited
            ans = std::min(ans, cost[curr][i] + total_cost(mask | (1 << i), i, cost, dp));
        }
    }

    return dp[curr][mask] = ans ;
}

int tsp(mat& cost) {
    int n = cost.size() ;
    mat dp(n, std::vector<int>(1 << n, -1)) ;

    int mask = 1, curr = 0;

    return total_cost(mask, curr, cost, dp);
}
*/
// TSP con DP + bitmask, guardando la decisión en nxt
int total_cost(int mask, int curr, mat& cost, mat& dp, mat& nxt) {
    int n = cost.size();

    if (mask == (1 << n) - 1) {
        return cost[curr][0];
    }
    if (dp[curr][mask] != -1) {
        return dp[curr][mask];
    }

    int ans = INT_MAX;
    for (int i = 0; i < n; i++) {
        if ((mask & (1 << i)) == 0) {
            int cand = cost[curr][i] + total_cost(mask | (1 << i), i, cost, dp, nxt);
            if (cand < ans) {
                ans = cand;
                nxt[curr][mask] = i;         // NUEVO: mejor siguiente ciudad
            }
        }
    }
    return dp[curr][mask] = ans;
}

// Devuelve el costo y llena 'route' con el orden de visita
int tsp(mat& cost, std::vector<int>& route) {
    int n = cost.size();
    mat dp(n, std::vector<int>(1 << n, -1));
    mat nxt(n, std::vector<int>(1 << n, -1));

    int best = total_cost(1, 0, cost, dp, nxt);

    // Reconstrucción del camino
    int curr = 0, mask = 1;
    route.push_back(0);
    while (mask != (1 << n) - 1) {
        int i = nxt[curr][mask];
        route.push_back(i);
        mask |= (1 << i);
        curr = i;
    }
    route.push_back(0);                      // regresar al origen
    return best;
}

// Max Flow using Ford-Fulkerson Algorithm (Edmonds-Karp Implementation)
bool bfs(mat &rGraph, int src, int sink, std::vector<int> &parent) {
    int V = rGraph.size();

    std::vector<bool> vis(V, false);
    std::queue<int> q;

    q.push(src);
    vis[src] = true;
    parent[src] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; v++) {
            if (!vis[v] && rGraph[u][v] > 0) {
                vis[v] = true;
                parent[v] = u;

                if (v == sink) {
                    return true;
                }

                q.push(v);
            }
        }
    }

    return false;
}

// Returns the maximum flow from source (0) to sink (V - 1)
int max_flow(int V, mat& flow_matrix) {
    int src = 0;
    int sink = V - 1;

    std::vector<int> parent(V);
    int maxFlow = 0;

    while (bfs(flow_matrix, src, sink, parent)) {
        int pathFlow = INT_MAX;

        for (int v = sink; v != src; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, flow_matrix[u][v]);
        }

        for (int v = sink; v != src; v = parent[v]) {
            int u = parent[v];
            flow_matrix[u][v] -= pathFlow;
            flow_matrix[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}


// Determines which side of the perpendicular bisector
// a point belongs to.
// A value <= 0 means the point is closer to A.
double evaluate(Point p, Point a, Point b) {
    return 2 * (b.first - a.first) * p.first
         + 2 * (b.second - a.second) * p.second
         - (b.first * b.first + b.second * b.second
         - a.first * a.first - a.second * a.second);
}

// Clips a polygon, keeping only the area closer to A than B.
Polygon clip_polygon(Polygon& polygon, Point a, Point b) {
    Polygon result;

    const double EPS = 1e-9;

    if (polygon.empty()) return result;

    Point prev = polygon.back();
    double prev_val = evaluate(prev, a, b);
    bool prev_inside = prev_val <= EPS;

    for (Point curr : polygon) {
        double curr_val = evaluate(curr, a, b);
        bool curr_inside = curr_val <= EPS;

        // Segment crosses the bisector
        if (prev_inside != curr_inside) {
            double t = prev_val / (prev_val - curr_val);

            Point intersection = {
                prev.first + t * (curr.first - prev.first),
                prev.second + t * (curr.second - prev.second)
            };

            result.push_back(intersection);
        }

        if (curr_inside) {
            result.push_back(curr);
        }

        prev = curr;
        prev_val = curr_val;
        prev_inside = curr_inside;
    }

    return result;
}

// Constructs the Voronoi polygons for all exchanges.
std::vector<Polygon> voronoi(coords& sites) {
    int n = sites.size();

    std::vector<Point> points;

    for (auto [x, y] : sites) {
        points.push_back({(double)x, (double)y});
    }

    // Find coordinate boundaries
    double minX = points[0].first;
    double maxX = points[0].first;
    double minY = points[0].second;
    double maxY = points[0].second;

    for (Point p : points) {
        minX = std::min(minX, p.first);
        maxX = std::max(maxX, p.first);
        minY = std::min(minY, p.second);
        maxY = std::max(maxY, p.second);
    }

    // Create an artificial bounding rectangle
    double margin = std::max({
        maxX - minX,
        maxY - minY,
        1.0
    }) * 10.0;

    minX -= margin;
    maxX += margin;
    minY -= margin;
    maxY += margin;

    std::vector<Polygon> polygons;

    for (int i = 0; i < n; i++) {

        // Initial polygon
        Polygon poly = {
            {minX, minY},
            {maxX, minY},
            {maxX, maxY},
            {minX, maxY}
        };

        // Compare against every other exchange
        for (int j = 0; j < n; j++) {
            if (i == j) continue;

            poly = clip_polygon(poly, points[i], points[j]);

            if (poly.empty()) break;
        }

        polygons.push_back(poly);
    }

    return polygons;
}


// main function
int main (int argc, char **argv) {
    // read from stdin
    std::ios::sync_with_stdio(false) ;
    std::cin.tie(nullptr) ;

    int n ;
    std::cin >> n;

    mat ma(n, std::vector<int>(n)), mb = ma;
    for (auto& row : ma) for (int& v : row) std::cin >> v ;
    for (auto& row : mb) for (int& v : row) std::cin >> v;

    coords pts(n) ;
    char c;
    for (auto& [x, y] : pts) std::cin >> c >> x >> c >> y >> c ; // '(' x ',' y ')'
    
    // 1. Cableado con fibra óptica (MST)
    std::vector<std::pair<int,int>> edges;
    mst(n, ma, edges);

    std::cout << "1. Cableado:" << std::endl;
    for (auto& [a, b] : edges) {
        std::cout << "(" << char('A' + a) << "," << char('A' + b) << ")" << std::endl;
    }
    std::cout << std::endl;

    // 2. Ruta del repartidor (TSP)
    std::vector<int> route;
    tsp(ma, route);

    std::cout << "2. Ruta:" << std::endl;
    for (size_t k = 0; k < route.size(); k++) {
        std::cout << char('A' + route[k]);
        if (k + 1 < route.size()) std::cout << " -> ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;

    std::cout << "Maximum flow from 1 to " << n << " in mb: " << max_flow(n, mb) << std::endl;
    
    
    // 4. Voronoi diagram
    std::vector<Polygon> polygons = voronoi(pts);

    std::cout << "\n4. Poligonos de Voronoi:\n";

    for (int i = 0; i < polygons.size(); i++) {

        std::cout << "Exchange " << char('A' + i) << ": ";

        for (Point p : polygons[i]) {
            std::cout << "(" << p.first << ", "
                      << p.second << ") ";
        }

        std::cout << std::endl;
    }


    return 0 ;
}

