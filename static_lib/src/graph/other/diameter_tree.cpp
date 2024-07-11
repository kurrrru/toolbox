#include <vector>
#include <queue>
#include <algorithm>
#include "../../../toolbox.h"

namespace toolbox
{

namespace graph
{

/**
* @brief Calculates the diameter of a tree.
* @tparam T The type of the cost of the edges.
* @param adjacent The adjacency list of the tree.
* @return The diameter of the tree.
* @note The time complexity is O(n), where n is the number of vertices in the tree.
*/
template <typename T>
T diameter_tree(const std::vector<std::vector<std::pair<long long, T> > > &adjacent)
{
	std::vector<T> dist(adjacent.size(), -1);
	std::queue<long long> que;
	que.push(0);
	dist[0] = 0;
	long long u = 0, v = 0;
	while (!que.empty())
	{
		u = que.front();
		que.pop();
		for (const auto [adj, cost] : adjacent[u])
		{
			if (dist[adj] != -1)
				continue;
			dist[adj] = dist[u] + cost;
			que.push(adj);
			if (dist[adj] > dist[v])
				v = adj;
		}
	}
	dist.assign(adjacent.size(), -1);
	que.push(v);
	dist[v] = 0;
	while (!que.empty())
	{
		u = que.front();
		que.pop();
		for (const auto [adj, cost] : adjacent[u])
		{
			if (dist[adj] != -1)
				continue;
			dist[adj] = dist[u] + cost;
			que.push(adj);
			if (dist[adj] > dist[v])
				v = adj;
		}
	}
	return dist[v];
}

/**
* @brief Calculates the diameter of a tree.
* @param adjacent The adjacency list of the tree.
* @return The diameter of the tree.
* @note The time complexity is O(n), where n is the number of vertices in the tree.
*/
long long diameter_tree(const std::vector<std::vector<std::pair<long long, long long> > > &adjacent)
{
	return diameter_tree<long long>(adjacent);
}

} // namespace graph

} // namespace toolbox
