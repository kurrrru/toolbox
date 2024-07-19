#include <vector>
#include <stack>

namespace toolbox
{

namespace graph
{

/**
* @brief Topological sort
* @tparam Vertex Type of vertex
* @param edge Adjacency list of graph
* @return Topological order if it exists, otherwise an empty vector
* @note [complexity] O(V + E)
* @note If you want to get the lexically smallest topological order, you should use priority_queue instead of stack.
*/
template <typename Vertex>
std::vector<Vertex> topological(const std::vector<std::vector<Vertex>> &edge)
{
	int N = edge.size();
	std::vector<int> indegree(N);
	for (int i = 0; i < N; i++)
		for (Vertex j : edge[i])
			indegree[j]++;
	std::stack<Vertex> st;
	for (int i = 0; i < N; i++)
		if (indegree[i] == 0)
			st.push(i);
	std::vector<Vertex> order;
	order.reserve(N);
	while (!st.empty())
	{
		Vertex v = st.top();
		st.pop();
		order.push_back(v);
		for (Vertex u : edge[v])
			if (--indegree[u] == 0)
				st.push(u);
	}
	if (order.size() != N)
		return std::vector<Vertex>();
	return order;
}

/**
* @brief Topological sort
* @param edge Adjacency list of graph
* @return Topological order if it exists, otherwise an empty vector
* @note [complexity] O(V + E)
* @note If you want to get the lexically smallest topological order, you should use priority_queue instead of stack.
*/
std::vector<long long> topological(const std::vector<std::vector<long long>> &edge)
{
	return topological<long long>(edge);
}

} // namespace graph

} // namespace toolbox
