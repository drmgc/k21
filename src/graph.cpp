#include "graph.hpp"

namespace k21_ppl {

std::shared_ptr<graph_t>  graph_t::create_from_edges(const std::vector<edge_t>&  edges, bool is_directed) {
	std::shared_ptr<graph_t>  gp (new graph_t());
	
	for (const edge_t&  e : edges) {
		gp->max_vertex = std::max(gp->max_vertex, std::max(e.first, e.second));
	}
	
	if (is_directed) {
		gp->edges = edges;
	} else {
		gp->edges.resize(edges.size() * 2);
		for (size_t  i = 0; i < edges.size(); i++) {
			gp->edges[i * 2] = edges[i];
			gp->edges[i * 2 + 1].first = edges[i].second;
			gp->edges[i * 2 + 1].second = edges[i].first;
		}
	}
	
	return gp;
}


std::shared_ptr<graph_t>  graph_t::create_from_stream(std::istream&  in) {
	size_t  count;
	bool  is_directed;
	in >> count >> is_directed;
	
	std::vector<edge_t>  edges (count);
	for (edge_t&  e : edges) {
		in >> e.first >> e.second;
	}

	return create_from_edges(edges, is_directed);
}

}
