#ifndef GRAPH_H
#define GRAPH_H
#include "csv_reader.h"

class Graph
{
private:
	std::string vertexFilePath, edgeFilePath;

public:
	Graph(std::string vf, std::string ef) : vertexFilePath(vf), edgeFilePath(ef) {};
	std::unordered_map<vertexType, char> vertexTypeMap = { {PERSON, 'p'}, {COMPANY, 'c'} };
	std::unordered_map<edgeType, char> edgeTypeMap = { {INVESTOR, 'i'}, {STAFF, 's'}, {LEGAL, 'l'} };
	void construct_edge(std::vector<std::pair<uint32_t, uint32_t> >& fromVertexes,
		std::vector<std::pair<uint32_t, uint32_t> >& toVertexes, edgeType edge);
	void construct_bi_edge(std::vector<std::pair<uint32_t, uint32_t> >& vertexes, edgeType edge);
};

#endif