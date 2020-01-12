#include <iostream>
#include <fstream>

#include <cxxopts.hpp>

#include "graph.hpp"
#include "a13.hpp"
#include "a15.hpp"
using namespace std;
using namespace k21_ppl;

#define DEFAULT_K_FACTOR (10)


struct nullstream : ostream {
	nullstream() : ostream(0) { }
};

bool  verbose = false;

ostream&  vout(ostream&  out = cout) {
	static nullstream dummy;
	return verbose ? out : dummy;
}


bool  input_vertices(vertex_t&  s, vertex_t&  t) {
	cin >> s >> t;
	return s != nullvrt && t != nullvrt;
}


int  main(int  argc, char**  argv) {
	cxxopts::Options  options ("k21-ppl");

#define ARG_A13 "a13"
#define ARG_A15 "a15"
#define ARG_USE_STDIN "stdin"
#define ARG_GRAPH "G"
#define ARG_VERBOSE "verbose"
#define ARG_HELP "help"
#define ARG_K_FACTOR "k"
#define ARG_DUMP "d"

	options.add_options()
		(ARG_A13, "Use A13")
		(ARG_A15, "Use A15")
		(ARG_USE_STDIN, "Use stdin as input")
		(ARG_GRAPH, "Load graph from file", cxxopts::value<string>())
		("v," ARG_VERBOSE, "Verbose")
		("h," ARG_HELP, "Print help")
		(ARG_K_FACTOR, "k factor (Default " + to_string(DEFAULT_K_FACTOR) + ")", cxxopts::value<int>())
		(ARG_DUMP, "Do index dump")
	;

	auto  optres = options.parse(argc, argv);

	if (optres[ARG_HELP].as<bool>()) {
		cout << options.help() << endl;
		cout << "Enter vertices for query in format:\n\ts1 t1\n\ts2 t2\n\t...\n\tsn tn\n\t0 0\n";
		return EXIT_SUCCESS;
	}

	verbose = optres[ARG_VERBOSE].as<bool>();
	string  graph_path = optres[ARG_GRAPH].as<string>();
	bool  with_a13 = optres[ARG_A13].as<bool>();
	bool  with_a15 = optres[ARG_A15].as<bool>();
	bool  use_stdin = optres[ARG_USE_STDIN].as<bool>();
	bool  spec_graph = !graph_path.empty();
	bool  do_dump = optres[ARG_DUMP].as<bool>();

	if (use_stdin && spec_graph) {
		cerr << "Don't use " ARG_USE_STDIN " and " ARG_GRAPH " at once!\n";
		return EXIT_FAILURE;
	}

	if (with_a13 && with_a15) {
		cerr << "Don't use --" ARG_A13 " and --" ARG_A15 " at once!\n";
		return EXIT_FAILURE;
	}

	shared_ptr<graph_t>  g;

	if (use_stdin) {
		g = graph_t::create_from_stream(cin);
	} else if (spec_graph) {
		ifstream  f (graph_path);
		g = graph_t::create_from_stream(f);
		f.close();
		if (g == nullptr) {
			cerr << "Failed to load graph from \"" << graph_path << '"' << endl;
			return EXIT_FAILURE;
		}
	} else {
		cerr << "Specify graph file with " ARG_USE_STDIN " or use stdin with " ARG_GRAPH "\n";
		return EXIT_FAILURE;
	}

	if (with_a13) {
		auto  ip = a13::index_t::preprocess_graph(*g);
		if (do_dump) {
			cout << *ip << std::endl;
		}

		vertex_t  s, t;
		while (input_vertices(s, t)) {
			cout << "d(" << s <<  ";" << t << ") = " << format_dist(ip->query(s, t)) << std::endl;
		}
	} else if (with_a15) {
		size_t  k;
		try {
			k = optres[ARG_K_FACTOR].as<int>();
		} catch (std::domain_error&  e) {
			k = DEFAULT_K_FACTOR;
			vout() << "Attention: k factor is not set. Using default k = " << k << endl;
		}

		auto  ip = a15::index_t::preprocess_graph(*g, k);
		if (do_dump) {
			cout << *ip << std::endl;
		}
		
		vertex_t  s, t;
		while (input_vertices(s, t)) {
			auto  result = ip->query(s, t);
			cout << "Query(" << s <<  ";" << t << ") = {";

			for (dist_t  d : *result) {
				cout << " " << format_dist(d) << ",";
			}

			cout << "\b }\n";
		}
	} else {
		cerr << "Specify --" ARG_A13 " or -" ARG_A15 "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
