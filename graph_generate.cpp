#include <chrono>
#include <regex>
#include "csv_reader.h"
#include "graph.h"
using namespace std;

int main(int argc, char* argv[]) {
	auto wall_clock = chrono::steady_clock::now();
	cerr << "running............." << endl;
	regex opexp("(.*)=(.*)");
	cmatch opmatch;
	CSVReader reader;
	for (int i = 1; i < argc; i++)
	{
		regex_match(argv[i], opmatch, opexp);
		if (opmatch.size() != 3)
		{
			std::cerr << "each cacheParam needs to be in form name=value" << endl;
			return 1;
		}
		reader.setPar(opmatch[1], opmatch[2]);
	}
	reader.init();
	Graph graph(reader.vertexOutputFilePath, reader.edgeOutputFilePath);
	long timeElapsed;
	for (int i = 0; i < 5; i++) {
		cout << "*********************************************************" << endl;
		auto edge = reader.prepare();
		timeElapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - wall_clock).count();
		cerr << "edge " << graph.vertexTypeMap[edge.first] << '/' << graph.edgeTypeMap[edge.second] << " done preparation at " << timeElapsed << " ms " << endl;
		if (edge.first == PERSON || edge.second == STAFF) {
			graph.construct_edge(reader.fromVertexes, reader.toVertexes, edge.second);
		}
		else {
			graph.construct_bi_edge(reader.fromVertexes, edge.second);
		}
		timeElapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - wall_clock).count();
		cerr << "edge " << graph.vertexTypeMap[edge.first] << '/' << graph.edgeTypeMap[edge.second] << " finished at " << timeElapsed << "ms" << endl;
	}
	cout << "*********************************************************" << endl;
	timeElapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - wall_clock).count();
	cout << "time duration : " << timeElapsed << "ms" << endl;
	return 0;
}


// 1.  all in first char initials: pl -> person/legal, et cetera
// 2.  encode ID for vertexes in integer:
//                            company-node: from 0 to company_number
//                            person-node:  from company_number to company_number + person_number
// 3.  output edge file line format: dstID '\t' srcID '\t' edgeType
// 4.  command line input the infomation about wanted graph:
//       ./graph_generate person_file=person.csv company_file=company.csv edge_output_file=graph.txt person_number=100229708 company_number=35902819 ci=3549620 cl=134609 cs=71318306 pi=57613205 pl=33581663