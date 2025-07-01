#include "CrossDetector.h"

CrossDetector::CrossResult CrossDetector::detectCross(const vector<string>& dates,const int shortPeriod,const int longPeriod,const unordered_map<int, vector<double>>& sma_values) {
    CrossResult result;

    for (size_t i = 1; i < dates.size(); ++i) {
        double shortSma = sma_values.at(shortPeriod)[i];
        double longSma = sma_values.at(longPeriod)[i];
        double shortSma_prev = sma_values.at(shortPeriod)[i-1];
        double longSma_prev = sma_values.at(longPeriod)[i-1];        

        if (shortSma > longSma && shortSma_prev < longSma_prev) {
            result.golden.push_back(dates[i]);
        }
        else if (shortSma < longSma && shortSma_prev > longSma_prev) {
            result.death.push_back(dates[i]);
        }    
    }
    return result;
}