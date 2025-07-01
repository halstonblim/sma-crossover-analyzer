#include "CsvReader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
using namespace std;

CsvModule::CsvData CsvModule::readCsv(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) throw runtime_error("Could not open file: " + filename);

	CsvModule::CsvData data;
	string line;
	int line_no = 0;

	while(getline(file, line)) {
		++line_no;
		if (line_no <= 3) continue;

		istringstream line_ss(line);
		string cell;

		// Extract the date
		if (!getline(line_ss, cell, ',')) continue;
		data.dates.push_back(cell);

		// Extract the price
		vector<double> row;
		for (int i = 0; i < 6; i ++) {
			if (getline(line_ss, cell, ',')) {
				try {
					row.push_back(stod(cell));
				}
				catch (...) {
					row.push_back(numeric_limits<double>::quiet_NaN());
				}				
			}
		}
		if (row.size() == 5) data.values.push_back(std::move(row));
		else data.dates.pop_back();
	}

	return data;
}