#include "graph.h"
#include "fast_output.h"

void Graph::construct_edge(std::vector<std::pair<uint32_t, uint32_t> >& fromVertexes,
	                       std::vector<std::pair<uint32_t, uint32_t> >& toVertexes, edgeType edge) {
	FastOutput::init(edgeFilePath, "a+");
	//std::ofstream outfile;
	//outfile.open(edgeFilePath , std::ofstream::app);
	auto fromVertexesIterBegin = fromVertexes.begin();
	auto fromVertexesIter = fromVertexesIterBegin;
	uint32_t degree;
	uint32_t edges = 0;
	char edge_t = edgeTypeMap[edge];
	for (auto toVertexesIter : toVertexes) {
		degree = toVertexesIter.second;
		if (degree == 0) {
			continue;
		}
		while (fromVertexesIterBegin->second == 0)
			fromVertexesIterBegin++;
		fromVertexesIter = fromVertexesIterBegin;
		while (degree) {
			//outfile << toVertexesIter.first << '\t' << fromVertexesIter->first << '\t' << edge_t << std::endl;
			FastOutput::write_uint(toVertexesIter.first);
			FastOutput::write_sep();
			FastOutput::write_uint(fromVertexesIter->first);
			FastOutput::write_sep();
			FastOutput::write_char(edge_t);
			FastOutput::endline();
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
	char edge_t = edgeTypeMap[edge];
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
				FastOutput::write_uint(curIter->first);
				FastOutput::write_sep();
				FastOutput::write_uint(iter->first);
				FastOutput::write_sep();
				FastOutput::write_char(edge_t);
				FastOutput::endline();
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