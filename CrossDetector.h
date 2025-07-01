#pragma once
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

namespace CrossDetector {
    struct CrossResult {
        vector<string> golden; //dates of golden crosses
        vector<string> death; //dates of death crosses
    };

    // Detects cross overs
    // dates: dates of the price series
    // shortPeriod: shorter period for SMA
    // longPeriod: longer period for SMA
    // sma_values: map of period -> vector of SMAs (same length as prices; entries before full window are NaN)
    // return: struct containing dates of golden and death crosses
    CrossResult detectCross(const vector<string>& dates,const int shortPeriod,const int longPeriod,const unordered_map<int, vector<double>>& sma_values);
}