#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <vector>
#include <queue>

namespace toolbox
{

namespace graph
{

/**
* @brief Dijkstra's algorithm
* @tparam Vertex Type of vertex
* @tparam Cost Type of cost
* @tparam INF Cost of infinity
* @tparam Zero Cost of zero
* @param s Start vertex
* @param cost Adjacency list of graph
* @return Shortest distance from start vertex
* @note [constraint] each cost must be non-negative
* @note [complexity] O((V + E) log V)
* @note If you want to get the shortest path, you should record the previous vertex. (not implemented)
*/
template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> dijkstra(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost)
{
	int N = cost.size();
	std::vector<Cost> dist(N, INF);
    dist[s] = Zero;
	std::priority_queue<std::pair<Vertex, Cost>, std::vector<std::pair<Vertex, Cost>>, std::greater<std::pair<Vertex, Cost>>> que;
	que.push({0, s});
    while (!que.empty())
	{
		auto [s_dist, s] = que.top();
		que.pop();
		if (dist[s] < s_dist)
			continue;
		for (auto [t, t_cost] : cost[s])
		{
			if (dist[t] <= dist[s] + t_cost)
				continue;
			dist[t] = dist[s] + t_cost;
			que.push({dist[t], t});
		}
	}
	return dist;
}

std::vector<long long> dijkstra(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost)
{
	return dijkstra<long long, long long, 1ll << 60, 0ll>(s, cost);
}

} // namespace graph

} // namespace toolbox

#endif
