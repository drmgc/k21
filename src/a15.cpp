#include <iostream>
#include <set>
#include <queue>

#include "a15.hpp"

using namespace std;

namespace k21_ppl {
namespace a15 {


index_t::index_t(const size_t  kp) : k(kp) {}
index_t::~index_t() {}

std::shared_ptr< std::vector<dist_t> >  index_t::query(const vertex_t  s, const vertex_t  t) const {
	std::multiset<dist_t>  distances;
	
	const dist_label_t&  Ls = m_labels.at(s).first;
	
	for (vertex_t  v = 1; v <= m_max_vertex; v++) {
		if (v == s || v == t)
			continue;

		const auto& Iv = m_labels.at(v); 
		const dist_label_t&  Lv = Iv.first;
		const loop_label_t&  Cv = Iv.second;

		for (dist_t  dsv : Ls[v]) {
			for (dist_t  dvs : Lv[t]) {
				for (dist_t  dvv : Cv) {
					distances.insert(dsv + dvv + dvs);
				}
			}
		}
	}

	auto  result = std::make_shared< std::vector<dist_t> >(k, infdst);

	auto  it = distances.begin();
	for (size_t  i = 0; i < k && it != distances.end(); i++, it++) {
		(*result)[i] = *it;
	}
	
	return result;
}


void  index_t::compute_loops(const graph_t&  g, const vertex_t  vk) {
	loop_label_t&  Cv = m_labels.at(vk).second;

	vector<size_t>  visits (g.max_vertex + 1, 0);

	queue< std::pair<vertex_t, dist_t> >  Q;
	Q.push({ vk, 0 });

	while (!Q.empty()) {
		vertex_t  u = Q.front().first;
		dist_t  d = Q.front().second;
		Q.pop();

		if (visits[u] > k) {
			continue;
		}

		if (u == vk) {
			Cv[visits[vk]] = d;
		}
		visits[u]++;

		for (const edge_t&  e : g.edges) {
			vertex_t  w = e.second;
			if (e.first == u && w >= vk) {
				Q.push({ w, d + 1 });
			}
		}
	}
}



void  index_t::pruned_bfs(const graph_t&  g, const vertex_t  vk) {
	dist_label_t&  Lv = m_labels.at(vk).first;

	std::queue< std::pair<vertex_t, dist_t>>  Q;
	Q.push({ vk, 0 });

	while (!Q.empty()) {
		vertex_t  u = Q.front().first;
		dist_t  d = Q.front().second;
		Q.pop();

		auto  query_result = query(vk, u);
		if (d < *std::max(query_result->begin(), query_result->end())) {
			Lv[u].push_back(d);
			// m_labels.at(u).first[vk].push_back(d);

			for (const edge_t&  e : g.edges) {
				vertex_t  w = e.second;
				if (e.first == u && w > vk) {
					Q.push({ w, d + 1 });
				}
			}
		}
	}
}



std::shared_ptr<index_t>  index_t::preprocess_graph(const graph_t&  g, const size_t  k) {
	std::shared_ptr<index_t>  ip (new index_t(k));

	ip->m_max_vertex = g.max_vertex;
	ip->m_labels.resize(ip->m_max_vertex + 1);
	for (label_t&  l : ip->m_labels) {
		l.first.resize(ip->m_max_vertex + 1);
		l.second.resize(k, infdst);
	}

	for (vertex_t  i = 1; i <= ip->m_max_vertex; i++) {
		ip->compute_loops(g, i);
	}
	for (vertex_t  i = 1; i <= ip->m_max_vertex; i++) {
		ip->pruned_bfs(g, i);
	}

	return ip;
}



std::ostream&  operator<<(std::ostream&  o, const index_t&  idx) {\
	o << "k21ppl::a15::index_t@" << (const index_t*)&idx
		<< " {\n\tmax_vertex = " << idx.m_max_vertex
		<< "\n\tk = " << idx.k
		<< "\n\tloop labels ={\n";
	for (vertex_t  i = 1; i <= idx.m_max_vertex; i++) {
		o << i << ":\t";
		for (dist_t  d : idx.m_labels.at(i).second) {
			cout << " " << format_dist(d) << ",";
		}
		cout << "\b\n";
	}
	cout << "\t}"
		<< "\n\tdist labels ={";
	for (vertex_t  i = 1; i <= idx.m_max_vertex; i++) {
		auto&  Li = idx.m_labels.at(i).first;
		o << '\n' << i << ":\t";
		for (vertex_t  j = 1; j <= idx.m_max_vertex; j++) {
			o << "\n\t" << j << ":";
			for (dist_t  d : Li.at(j)) {
				cout << " " << format_dist(d) << ",";
			}
			cout << "\b";
		}
	}
	cout << "\n\t}";
	cout << "\n}";
	return o;
}

}
}
