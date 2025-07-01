#pragma once
#include <string>
#include <vector>
using namespace std;

namespace CsvModule {
	
	struct CsvData {
		vector<string> dates;
		vector<vector<double>> values;
	};

	CsvData readCsv(const string& filename);
}