#ifndef CSV_READER_H
#define CSV_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <unordered_map>

enum vertexType {
	PERSON,
	COMPANY
};

enum edgeType {
	INVESTOR,
	STAFF,
	LEGAL
};


class CSVReader {
protected:
	uint32_t personNumber = 0, companyNumber = 0, ci = 0, cl = 0, cs = 0, pi = 0, pl = 0;

	std::vector<std::pair<vertexType, edgeType> > personCols, companyCols;

	std::vector<uint32_t>* personDegree = new std::vector<uint32_t>[5];
	std::vector<uint32_t>* companyDegree = new std::vector<uint32_t>[5];
	
	
	void parse_csv(std::string& file_path, std::vector<uint32_t>* degree, 
		           std::vector<std::pair<vertexType, edgeType> >& cols);
	uint8_t currentCompanyCol = 0;

public:
	std::string personFilePath, companyFilePath, vertexOutputFilePath, edgeOutputFilePath;
	std::vector<std::pair<uint32_t, uint32_t> > fromVertexes;
	std::vector<std::pair<uint32_t, uint32_t> > toVertexes;
	CSVReader() {};
	void init();
	void setPar(std::string parName, std::string parValue);
	uint32_t getCompanyNumber() { return companyNumber; };

	std::pair<vertexType, edgeType> prepare();
	void generate_distribution(std::vector<uint32_t>& degree, 
		                       std::vector<std::pair<uint32_t, uint32_t> >& target,
		                       uint32_t target_number, uint32_t edge_number);
};

#endif