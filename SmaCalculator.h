#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

namespace SmaModule {
    // prices: input price series
    // periods: list of window lengths (e.g. 10, 50)
    // return: map of period -> vector of SMAs (same length as prices; entries before full window are NaN)
    unordered_map<int, vector<double>> computeSMA(const vector<double>& prices, const vector<int>& periods);

    // Compute SMA for a single period
    vector<double> computeSMA(const vector<double>& prices, int period);
}