#include "a13.hpp"

namespace k21_ppl {
namespace a13 {

index_t::index_t() {}
index_t::~index_t() {}


dist_t  index_t::query(const vertex_t  s, const vertex_t  t) const {
	dist_t  d = infdst;
	
	const label_t&  Ls = m_labels.at(s);
	
	for (vertex_t  v = 1; v <= m_max_vertex; v++) {
		if (v == s || v == t)
			continue;
		const label_t&  Lv = m_labels.at(v);
		d = std::min(d, (Ls[v] + Lv[t]));
	}
	
	return d;
}


void  index_t::pruned_bfs(const graph_t&  g, const vertex_t  vk) {
	std::queue<vertex_t>  Q;
	Q.push(vk);
	std::vector<dist_t>  P (m_labels.size(), infdst);
	P[vk] = 0;
	while (!Q.empty()) {
		vertex_t  u = Q.front();
		Q.pop();
		if (query(vk, u) <= P[u]) {
			continue;
		}
		m_labels.at(vk)[u] = P[u];
		for (const edge_t&  e : g.edges) {
			vertex_t  w = e.second;
			if (e.first == u && P[w] >= infdst) {
				P[w] = P[u] + 1;
				Q.push(w);
			}
		}
	}
}


std::shared_ptr<index_t>  index_t::preprocess_graph(const graph_t&  g) {
	std::shared_ptr<index_t>  ip (new index_t());
	ip->m_max_vertex = g.max_vertex;
	ip->m_labels.resize(ip->m_max_vertex + 1);
	for (label_t&  l : ip->m_labels) {
		l.resize(ip->m_max_vertex + 1, infdst);
	}
	for (vertex_t  v = 1; v <= g.max_vertex; v++) {
		ip->pruned_bfs(g, v);
	}

	return ip;
}



std::ostream&  operator<<(std::ostream&  o, const index_t&  idx) {\
	o << "k21ppl::a13::index_t@" << (const index_t*)&idx
		<< " {\n\tmax_vertex = " << idx.m_max_vertex
		<< "\n\tlabels ={\n";
	for (vertex_t  v = 1; v <= idx.m_max_vertex; v++) {
		o << '\t' << v;
	}
	o << std::endl;
	for (vertex_t  i = 1; i <= idx.m_max_vertex; i++) {
		o << i << ":\t";
		for (vertex_t  j = 1; j <= idx.m_max_vertex; j++) {
			dist_t  d = idx.m_labels[i][j];
			o << format_dist(d) << '\t';
		}
		o << std::endl;
	}
	o << "\t}\n}";
	return o;
}

}
}
