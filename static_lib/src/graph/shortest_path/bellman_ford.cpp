#include <vector>

namespace toolbox
{

namespace graph
{

/**
* @brief Bellman-Ford algorithm
* @tparam Vertex Type of vertex
* @tparam Cost Type of cost
* @tparam INF Cost of infinity
* @tparam Zero Cost of zero
* @param s Start vertex
* @param cost Adjacency list of graph
* @return Shortest distance from start vertex if there is no negative cycle, otherwise an empty vector
* @note [complexity] O(VE)
* @note If you want to detect negative cycle, you should check whether the return value is empty.
*/
template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> bellman_ford(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost)
{
	int N = cost.size();
	std::vector<Cost> dist(N, INF);
	dist[s] = Zero;
	for (int i = 0; i < N; i++)
	{
		bool update = false;
		for (int j = 0; j < N; j++)
		{
			for (auto [t, t_cost] : cost[j])
			{
				if (dist[j] == INF)
					continue;
				if (dist[t] > dist[j] + t_cost)
				{
					dist[t] = dist[j] + t_cost;
					update = true;
					if (i == N)
						return std::vector<Cost>();
				}
			}
		}
		if (!update)
			break;
	}
	return dist;
}

/**
* @brief Bellman-Ford algorithm
* @param s Start vertex
* @param cost Adjacency list of graph
* @return Shortest distance from start vertex if there is no negative cycle, otherwise an empty vector
* @note [complexity] O(VE)
* @note If you want to detect negative cycle, you should check whether the return value is empty.
*/
std::vector<long long> bellman_ford(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost)
{
	return bellman_ford<long long, long long, 1ll << 60, 0ll>(s, cost);
}

/**
* @brief Checks whether there is a negative cycle
* @tparam Vertex Type of vertex
* @tparam Cost Type of cost
* @tparam INF Cost of infinity
* @tparam Zero Cost of zero
* @param s Start vertex
* @param cost Adjacency list of graph
* @return true if there is a negative cycle, otherwise false
* @note [complexity] O(VE)
*/
template <typename Vertex, typename Cost, Cost INF, Cost Zero>
bool negative_cycle(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost)
{
	return bellman_ford(s, cost).empty();
}

/**
* @brief Checks whether there is a negative cycle
* @param s Start vertex
* @param cost Adjacency list of graph
* @return true if there is a negative cycle, otherwise false
* @note [complexity] O(VE)
*/
bool negative_cycle(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost)
{
	return negative_cycle<long long, long long, 1ll << 60, 0ll>(s, cost);
}

} // graph

} // toolbox
