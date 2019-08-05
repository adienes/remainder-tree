#include <vector>
#include <functional>
#include <chrono>
#include <fstream>

using std::vector;

vector <vector<int>> complexity_graph(int N, int d, vector <std::function< vector<bool> (int N)>> search_funcs) {
	int s = search_funcs.size();

	vector<vector<int>> graph(1);
	graph.reserve(d);
	graph[0] = vector<int> (s+1, 0); //Corresponds to origin---make s+1 larger if there are other data collected.
	
	int interval = N/d;
	int B = interval;
	while (B <= N) {

		vector<int> datum;
		datum.reserve(s+1);
		datum.push_back(B); //this is the x-value

		for (auto f : search_funcs) {
			auto start = std::chrono::high_resolution_clock::now();
			f(B);
			auto end = std::chrono::high_resolution_clock::now();

			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
			datum.push_back(elapsed);
		}

		graph.push_back(datum);

		B += interval;
	}

	return graph;
}



void write_complexity_graph (const std::vector<std::vector<int>>& cg, std::string file = "results.csv") {
	std::ofstream target;
	target.open("runtimes/data/"+file); //assuming this is run from main directory

	for (auto&& i : cg) {
		for (auto&& j : i) {
			target << j << ",";
		}

		target << "\n";
	}

	target.close();
}