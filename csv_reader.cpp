#include "csv_reader.h"
#include <algorithm>


void CSVReader::setPar(std::string parName, std::string parValue) {
	if (parName.compare("person_file") == 0)
	{
		personFilePath = parValue;
	}
	else if (parName.compare("company_file") == 0) {
		companyFilePath = parValue;
	}
	else if (parName.compare("vertex_output_file") == 0) {
		vertexOutputFilePath = parValue;
	}
	else if (parName.compare("edge_output_file") == 0) {
		edgeOutputFilePath = parValue;
	}
	else if (parName.compare("adj_output_file") == 0) {
		adjOutputFilePath = parValue;
	}
	else if (parName.compare("person_number") == 0) {
		const int n = stoull(parValue);
		personNumber = n;
	}
	else if (parName.compare("company_number") == 0) {
		const int n = stoull(parValue);
		companyNumber = n;
	}
	else if (parName.compare("ci") == 0) {
		const int n = stoull(parValue);
		ci = n * 2;
	}
	else if (parName.compare("cl") == 0) {
		const int n = stoull(parValue);
		cl = n * 2;
	}
	else if (parName.compare("cs") == 0) {
		const int n = stoull(parValue);
		cs = n;
	}
	else if (parName.compare("pi") == 0) {
		const int n = stoull(parValue);
		pi = n;
	}
	else if (parName.compare("pl") == 0) {
		const int n = stoull(parValue);
		pl = n;
	}
	else {
		std::cerr << "unrecognized parameter: " << parName << std::endl;
	}
}

void CSVReader::init() {
	parse_csv(personFilePath, personDegree, personCols);
	std::cerr << "read in " << personDegree[0].size() << " lines from person file." << std::endl;
	parse_csv(companyFilePath, companyDegree, companyCols);
	std::cerr << "read in " << companyDegree[0].size() << " lines from company file." << std::endl;
	std::cerr << "csv files done parse" << std::endl;
	// std::ofstream outfile;
	// outfile.open(vertexOutputFilePath);
	// uint32_t id = 0;
	// while (id < companyNumber) {
	// 	outfile << id << '\t' << 'c' << std::endl;
	// 	id++;
	// }
	// uint32_t total = companyNumber + personNumber;
	// while (id < total) {
	// 	outfile << id << '\t' << 'p' << std::endl;
	// 	id++;
	// }
	// outfile.close();
}

void CSVReader::parse_csv(std::string& file_path, std::vector<uint32_t>* degree,
	                      std::vector<std::pair<vertexType, edgeType> >& cols) {
	std::ifstream infile(file_path, std::ios::in);
	// Make sure the file is open
	if (!infile.is_open()) 
		throw std::runtime_error("Could not open file");
	std::string line, colname;
	std::stringstream sstream;
	int colIdx = 1;
	if (infile.good())
	{
		std::getline(infile, line);
		sstream.str(line);
		std::getline(sstream, colname, ',');
		while (std::getline(sstream, colname, ',')) {
			vertexType vt;
			edgeType et;
			if (colname.find("person") == std::string::npos) {
				vt = COMPANY;
				if (colname.find("investor") == std::string::npos)
					if (colname.find("staff") == std::string::npos)
						et = LEGAL;
					else
						et = STAFF;
				else
					et = INVESTOR;
			}
			else {
				vt = PERSON;
				if (colname.find("investor") == std::string::npos) {
					et = LEGAL;
				}
				else {
					et = INVESTOR;
				}
			}
			cols.push_back(std::make_pair(vt, et));
		}
	}
	while (std::getline(infile, line))
	{
		uint32_t i = 0;
		while (line[i] != ',') {
			i++;
		}
		for (colIdx = 0; colIdx < 5; colIdx++) {
			uint32_t val = 0;
			if (i == line.size()) {
				degree[colIdx].push_back(val);
				continue;
			}
			while (!isdigit(line[i])) {
				i++;
			}
			while (isdigit(line[i])) {
				val *= 10;
				val += (line[i++] - '0');
			}
			degree[colIdx].push_back(val);
		}
	}
	infile.close();
}

void CSVReader::generate_distribution(std::vector<uint32_t>& degree,
	                                  std::vector<std::pair<uint32_t, uint32_t> >& target,
	                                  uint32_t target_vertex_number, uint32_t target_edge_number) {
	std::sort(degree.begin(), degree.end());
	uint32_t vertex_number = degree.size();
	uint64_t edge_number = 0;
	uint32_t min_degree = degree[0], max_degree = degree[vertex_number - 1];
	std::cerr << "min_degree:" << min_degree << ' ' << ", max_degree:" << max_degree << std::endl;
	std::vector<uint32_t> distribute_map;
	uint32_t cumulate = 0;
	int gap;
	if (target_vertex_number == companyNumber && target_edge_number == pl)
		gap = 1;
	else
		gap = 5;
	auto it = degree.begin();
	for (auto val = min_degree; val < max_degree; val += gap) {
		while (it != degree.end() && *it <= val) {
			cumulate++;
			it++;
		}
		distribute_map.push_back(cumulate);
	}
	if (target_vertex_number == personNumber) { // ids from 0~companyNumber are COMPANY nodes
		for (uint32_t id = companyNumber; id < target_vertex_number + companyNumber; id++) {
			uint32_t _random = std::rand() % vertex_number;
			int i = 0;
			while (distribute_map[i] < _random) {
				i++;
			}
			int val = i * gap + std::rand() % gap - std::rand() % gap;
			val = val > 0 ? val : 0;
			target.push_back(std::make_pair(id, val));
			edge_number += val;
		}
	}
	else {
		for (uint32_t id = 0; id < target_vertex_number; id++) {
			uint32_t _random = std::rand() % vertex_number;
			int i = 0;
			while (distribute_map[i] < _random) {
				i++;
			}
			int val = i * gap + std::rand() % gap - std::rand() % gap;
			val = val > 0 ? val : 0;
			target.push_back(std::make_pair(id, val));
			edge_number += val;
		}
	}
	// next change the edge number to target edge number properly
	int ratio = target_edge_number / edge_number;
	if (ratio == 0) {  // edge_number > target_edge_number
		ratio = edge_number / target_edge_number;
		if (ratio > 1) {
			edge_number = 0;
			for (uint32_t i = 0; i < target_vertex_number; i++) {
				target[i].second /= ratio;
				edge_number += target[i].second;
			}
		}
	}
	else {			//  edge_number < target_edge_number, multiply
		if (ratio > 1) {
			edge_number = 0;
			for (uint32_t i = 0; i < target_vertex_number; i++) {
				target[i].second *= ratio;
				edge_number += target[i].second;
			}
		}
	}
	if (edge_number > target_edge_number) {
		while (edge_number > target_edge_number) {
			for (auto it = target.begin(); it != target.end(); it++) {
				if (edge_number == target_edge_number)
					break;
				if (it->second > 0) {
					edge_number--;
					it->second--;
				}
			}
		}
	}
	else if (edge_number < target_edge_number) {
		while (edge_number < target_edge_number) {
			for (auto it = target.begin(); it != target.end(); it++) {
				if (edge_number == target_edge_number)
					break;
				edge_number++;
				it->second++;
			}
		}
	}
	return;
}

bool cmp_asc(std::pair<uint32_t, uint32_t>& x, std::pair<uint32_t, uint32_t>& y) {
	return x.second < y.second;
}

bool cmp_dsc(std::pair<uint32_t, uint32_t>& x, std::pair<uint32_t, uint32_t>& y) {
	return x.second > y.second;
}

std::pair<vertexType, edgeType> CSVReader::prepare() {
	auto edge = companyCols[currentCompanyCol];
	int personCol = 0;
	fromVertexes.clear();
	toVertexes.clear();
	if (edge.first == PERSON || edge.second == STAFF) { // one-direction edge from PERSON to COMPANY
		while (personCols[personCol].first != edge.first || personCols[personCol].second != edge.second) {
			personCol++;
		}
		switch (edge.second)
		{
		case STAFF:
			generate_distribution(personDegree[personCol], fromVertexes, personNumber, cs);
			generate_distribution(companyDegree[currentCompanyCol], toVertexes, companyNumber, cs);
			break;
		case LEGAL:
			generate_distribution(personDegree[personCol], fromVertexes, personNumber, pl);
			generate_distribution(companyDegree[currentCompanyCol], toVertexes, companyNumber, pl);
			break;
		case INVESTOR:
			generate_distribution(personDegree[personCol], fromVertexes, personNumber, pi);
			generate_distribution(companyDegree[currentCompanyCol], toVertexes, companyNumber, pi);
			break;
		default:
			break;
		}
		std::sort(fromVertexes.begin(), fromVertexes.end(), cmp_asc);
		std::sort(toVertexes.begin(), toVertexes.end(), cmp_dsc);
	}
	else { // bi-direction edge between COMPANY and COMPANY
		if (edge.second == LEGAL) {
			generate_distribution(companyDegree[currentCompanyCol], fromVertexes, companyNumber, cl);
		}
		else {
			generate_distribution(companyDegree[currentCompanyCol], fromVertexes, companyNumber, ci);
		}
		std::sort(fromVertexes.begin(), fromVertexes.end(), cmp_dsc);
	}
	currentCompanyCol++;
	return edge;
}