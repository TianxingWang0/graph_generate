#ifndef GRAPH_H
#define GRAPH_H
#include "csv_reader.h"

class Graph
{
private:
	uint32_t vertexNumber;
	std::string vertexFilePath, edgeFilePath, adjFilePath;
	std::vector<std::vector<uint32_t> > graph;

public:
	Graph(std::string vf, std::string ef, std::string af, uint32_t v) { 
		vertexFilePath = vf;
		edgeFilePath = ef;
		adjFilePath = af;
		vertexNumber = v;
		graph.resize(v); };
	std::unordered_map<vertexType, char> vertexTypeMap = { {PERSON, 'p'}, {COMPANY, 'c'} };
	std::unordered_map<edgeType, char> edgeTypeMap = { {INVESTOR, 'i'}, {STAFF, 's'}, {LEGAL, 'l'} };
	void construct_edge(std::vector<std::pair<uint32_t, uint32_t> >& fromVertexes,
		std::vector<std::pair<uint32_t, uint32_t> >& toVertexes, edgeType edge);
	void construct_bi_edge(std::vector<std::pair<uint32_t, uint32_t> >& vertexes, edgeType edge);
	void write_adj();
};

#endif