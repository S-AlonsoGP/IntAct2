#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <climits>

// helper types
typedef std::vector<std::vector<int>> mat ;
typedef std::vector<std::pair<int, int>> coords ;

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
	
	std::cout << "Minimum spanning tree over ma: \n" << mst(n, ma) << std::endl;

	return 0 ;
}

