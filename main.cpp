#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm> 
#include <climits>

// helper types
typedef std::vector<std::vector<int>> mat ;
typedef std::vector<std::pair<int, int>> coords ;

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
int mst(unsigned int n, mat& adj_matrix) { 
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
	
	std::cout << "Minimum spanning tree over ma: " << mst(n, ma) << std::endl;

	std::cout << std::endl;

	std::cout << "Total cost of visiting all points in ma: " << tsp(ma) << std::endl;

	std::cout << std::endl;

	std::cout << "Maximum flow from 1 to " << n << " in mb: " << max_flow(n, mb) << std::endl;

	return 0 ;
}

