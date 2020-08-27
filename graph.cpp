#include "graph.h"
#include "fast_output.h"

Graph::Graph(std::string vf, std::string ef, std::string af, uint32_t cn, uint32_t pn) { 
	vertexFilePath = vf;
	edgeFilePath = ef;
	adjFilePath = af;
	companyNumber = cn;
	personNumber = pn;
	graph.resize(cn + pn);

	FastOutput::init(vertexFilePath);
	std::cerr << "fuck";
	FastOutput::write_string("vertex_id:ID,type:LABEL");
	FastOutput::endline();

	uint32_t id = 0;
	while (id < companyNumber) {
		FastOutput::write_uint(id);
		FastOutput::write_char(',');
		FastOutput::write_char('c');
		FastOutput::endline();
		id++;
	}
	uint32_t total = companyNumber + personNumber;
	while (id < total) {
		FastOutput::write_uint(id);
		FastOutput::write_char(',');
		FastOutput::write_char('p');
		FastOutput::endline();
		id++;
	}
	FastOutput::close();

	FastOutput::init(edgeFilePath, "a+");
	FastOutput::write_string("from_id:START_ID,to_id:END_ID,type:TYPE");
	FastOutput::endline();
	FastOutput::close();
}

void Graph::construct_edge(std::vector<std::pair<uint32_t, uint32_t> >& fromVertexes,
	                       std::vector<std::pair<uint32_t, uint32_t> >& toVertexes, edgeType edge) {
	FastOutput::init(edgeFilePath, "a+");
	//std::ofstream outfile;
	//outfile.open(edgeFilePath , std::ofstream::app);
	auto fromVertexesIterBegin = fromVertexes.begin();
	uint32_t degree;
	uint32_t edges = 0;
	//char edge_t = edgeTypeMap[edge];
	for (const auto& toVertexesIter : toVertexes) {   // dsc
		degree = toVertexesIter.second;
		if (degree == 0) {
			continue;
		}
		while (fromVertexesIterBegin->second == 0)    // asc
			fromVertexesIterBegin++;
		auto fromVertexesIter = fromVertexesIterBegin;
		while (degree) {
			//outfile << toVertexesIter.first << '\t' << fromVertexesIter->first << '\t' << edge_t << std::endl;
			// FastOutput::write_uint(toVertexesIter.first);
			// FastOutput::write_sep();
			// FastOutput::write_uint(fromVertexesIter->first);
			// FastOutput::write_sep();
			// FastOutput::write_char(edge_t);
			// FastOutput::endline();

			FastOutput::write_uint(toVertexesIter.first);
			FastOutput::write_char(',');
			FastOutput::write_uint(fromVertexesIter->first);
			FastOutput::write_char(',');
			FastOutput::write_char('r');
			FastOutput::endline();

			FastOutput::write_uint(fromVertexesIter->first);
			FastOutput::write_char(',');
			FastOutput::write_uint(toVertexesIter.first);
			FastOutput::write_char(',');
			FastOutput::write_char('r');
			FastOutput::endline();

			graph[toVertexesIter.first].push_back(fromVertexesIter->first);
			graph[fromVertexesIter->first].push_back(toVertexesIter.first);

			edges++;
			fromVertexesIter->second--;
			degree--;
			fromVertexesIter++;
		}
	}
	std::cerr << edges << " edges written." << std::endl;
	//outfile.close();
	FastOutput::close();


}

void Graph::construct_bi_edge(std::vector<std::pair<uint32_t, uint32_t> >& vertexes, edgeType edge) {
	FastOutput::init(edgeFilePath, "a+");
	//std::ofstream outfile;
	//outfile.open(edgeFilePath , std::ofstream::app);
	uint32_t edges = 0;
	//char edge_t = edgeTypeMap[edge];
	for (auto curIter = vertexes.begin(); curIter != vertexes.end(); curIter++) {
		if (curIter->second == 0) {
			continue;
		}
		auto iter = curIter;
		iter++;
		uint32_t degree = curIter->second;
		while (degree) {
			if (iter->second) {
				// outfile << curIter->first << '\t' << iter->first << '\t' << edge_t << std::endl;
				// FastOutput::write_uint(curIter->first);
				// FastOutput::write_sep();
				// FastOutput::write_uint(iter->first);
				// FastOutput::write_sep();
				// FastOutput::write_char(edge_t);
				// FastOutput::endline();

				FastOutput::write_uint(curIter->first);
				FastOutput::write_char(',');
				FastOutput::write_uint(iter->first);
				FastOutput::write_char(',');
				FastOutput::write_char('r');
				FastOutput::endline();

				FastOutput::write_uint(iter->first);
				FastOutput::write_char(',');
				FastOutput::write_uint(curIter->first);
				FastOutput::write_char(',');
				FastOutput::write_char('r');
				FastOutput::endline();

				graph[curIter->first].push_back(iter->first);
				graph[iter->first].push_back(curIter->first);

				edges++;
				iter->second--;
				degree--;
			}
			iter++;
		}
	}
	std::cerr << edges << " edges written." << std::endl;
	//outfile.close();
	FastOutput::close();
}

void Graph::write_adj() {
	FastOutput::init(adjFilePath, "w");
	uint32_t id = 0;
	uint32_t edge = 0;
	for (const auto& adj_list : graph) {
		FastOutput::write_uint(id);
		id++;
		std::set<uint32_t> adj_set;
		for (const auto& neighbor : adj_list) {
			adj_set.insert(neighbor);
		}
		FastOutput::write_sep();
		FastOutput::write_uint(adj_set.size());
		for (const auto& neighbor : adj_set) {
			FastOutput::write_sep();
			FastOutput::write_uint(neighbor);
		}
		FastOutput::endline();
	}
	std::cerr << edge / 2 << " edges in adj file" << std::endl;
	FastOutput::close();
}