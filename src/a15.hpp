#pragma once

#define K21_PPL_A15_HPP 1

#include <ostream>
#include <vector>
#include <queue>

#include "graph.hpp"

namespace k21_ppl {
namespace a15 {

class index_t {
	friend std::ostream&  operator<<(std::ostream&  o, const index_t&  idx);

	typedef std::vector< std::vector<dist_t> >  dist_label_t;
	typedef std::vector< dist_t >  loop_label_t;
	typedef std::pair<dist_label_t, loop_label_t>  label_t;
	std::vector<label_t>  m_labels;

	vertex_t  m_max_vertex;

	index_t(const size_t  k);

	void  compute_loops(const graph_t&  g, const vertex_t  vk);
	void  pruned_bfs(const graph_t&  g, const vertex_t  vk);
public:
	const size_t  k;

	~index_t();

	std::shared_ptr< std::vector<dist_t> >  query(const vertex_t  s, const vertex_t  t) const;
	static std::shared_ptr<index_t>  preprocess_graph(const graph_t&  g, const size_t  k);
};


std::ostream&  operator<<(std::ostream&  o, const index_t&  idx);

}
}
