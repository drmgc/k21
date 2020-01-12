/**
 * Реализация алгоритма алгоритма быстрого поиска длины кратчайшего пути между двумя произвольными точками графа
 */

#pragma once

#define K21_PPL_A13_HPP 1

#include <ostream>
#include <vector>
#include <queue>

#include "graph.hpp"

namespace k21_ppl {
namespace a13 {

class index_t {
	friend std::ostream&  operator<<(std::ostream&  o, const index_t&  idx);

	typedef std::vector<dist_t>  label_t;
	std::vector<label_t>  m_labels;
	vertex_t  m_max_vertex;

	index_t();

	void  pruned_bfs(const graph_t&  g, const vertex_t  vk);
public:
	~index_t();
	
	dist_t  query(const vertex_t  s, const vertex_t  t) const;
	static std::shared_ptr<index_t>  preprocess_graph(const graph_t&  g);
};


std::ostream&  operator<<(std::ostream&  o, const index_t&  idx);

}
}
