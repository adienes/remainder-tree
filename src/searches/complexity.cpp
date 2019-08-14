#include <vector>
#include <functional>
#include <chrono>
#include <fstream>

using std::vector;

vector <vector<long>> complexity_graph(long d, vector <std::function< vector<bool> (long N)>> search_funcs) {
	long s = search_funcs.size();

	vector<vector<long>> graph(1);
	graph.reserve(d);
	graph[0] = vector<long> (s+1, 0); //Corresponds to origin---make s+1 larger if there are other data collected.

	long B = 1;
	while (d >= 0) {

		vector<long> datum;
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

		B <<= 1;
		d -= 1;
	}

	return graph;
}



void write_complexity_graph (const std::vector<std::vector<long>>& cg, std::string file = "results.csv") {
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