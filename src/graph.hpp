#pragma once

#define K21_PPL_GRAPH_HPP 1

#include <cstdint>
#include <utility>
#include <memory>
#include <vector>
#include <istream>
#include <string>

namespace k21_ppl {

typedef uint16_t  vertex_t;
constexpr vertex_t  nullvrt = 0;

typedef size_t  dist_t;
constexpr dist_t  infdst = 2000000000;

typedef std::pair<vertex_t, vertex_t>  edge_t;

struct graph_t {
	std::vector<edge_t>  edges;
	vertex_t  max_vertex = nullvrt;
	
	static std::shared_ptr<graph_t>  create_from_edges(const std::vector<edge_t>&  edges, bool is_directed = false);

	/**
	 * Считывает из поток граф в формате
	 * 	C D
	 *  F1 T1
	 *  F2 T2
	 *  ...
	 *  Fn Tn
	 * где:
	 * 	C -- количество рёбер
	 *  D (0/1) -- является ли граф направленным
	 *  Fn Tn -- рёбра (Fn; Tn)
	 */
	static std::shared_ptr<graph_t>  create_from_stream(std::istream&  in);
private:
	graph_t() {}
};


inline std::string  format_dist(const dist_t  d) {
	if (d == infdst) {
		return "~";
	} else if (d >= infdst) {
		return "~+";
	} else {
		return std::to_string(d);
	}
}

}
