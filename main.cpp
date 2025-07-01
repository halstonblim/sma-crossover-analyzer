#include "CsvReader.h"
#include "SmaCalculator.h"
#include "CrossDetector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
using namespace std;

void write_csv(const string& filename, unordered_map<int, vector<double>>& sma_map, vector<string>& dates, vector<double>& prices, vector<int>& periods) {
	ofstream out(filename);

	// Print header
	out << "Date,Price";
	for (int p : periods) {
		out << ",SMA" << p;
	}
	out << "\n";

	// Print rows
	for (size_t i = 0; i < dates.size(); ++i) {
		out << dates[i] << "," << prices[i];
		for (int p : periods) {
			double val = sma_map[p][i];
			if (std::isnan(val)) out << ","; // empty cell for NaN
			else out << "," << val;
		}
		out << "\n";
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <csv_file> [period1 period2 ...]\n";
		return 1;
	}
	string inputFilename = argv[1];

	// Parse periods from command line or use defaults
	vector<int> periods;
	if (argc > 2) {
		for (int i = 2; i < argc; ++i) {
			int p = stoi(argv[i]);
			if (p <= 0) throw invalid_argument("Period must be positive: " + to_string(p));
			periods.push_back(p);
		}
	} else periods = {50, 200};

	try {
		auto csvData = CsvModule::readCsv(inputFilename);

		// Extract the price series (first numeric column per row)
		vector<double> prices(csvData.values.size(), numeric_limits<double>::quiet_NaN());
		for (size_t i = 0; i < csvData.values.size(); ++i) {
			if (!csvData.values[i].empty()) prices[i] = csvData.values[i][0];
		}

		// Compute SMAs
		unordered_map<int, vector<double>> sma_map = SmaModule::computeSMA(prices, periods);

		// Determine output filename by replacing input file's extension with ".out"
		string outputFilename = inputFilename.substr(0, inputFilename.find_last_of('.')) + "_out.csv";
		write_csv(outputFilename, sma_map, csvData.dates, prices, periods);

		// Detect cross overs
		CrossDetector::CrossResult crossResult = CrossDetector::detectCross(csvData.dates, periods[0], periods[1], sma_map);

		// Write cross overs to output file
		string crossoverFilename = inputFilename.substr(0, inputFilename.find_last_of('.')) + "_crossovers.csv";
		ofstream out(crossoverFilename);
		out << "date,cross_type\n";
		for (const string& date : crossResult.golden) out << date << ",golden\n";
		for (const string& date : crossResult.death) out << date << ",death\n";
	}
	catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}