#include "SmaCalculator.h"
#include <deque>
#include <limits>
#include <stdexcept>

unordered_map<int, vector<double>> SmaModule::computeSMA(const vector<double>& prices, const vector<int>& periods) {
    unordered_map<int, vector<double>> result;

    for (int p : periods) {
        result[p] = vector<double>(prices.size(), numeric_limits<double>::quiet_NaN());
    }

    // Define states for each period
    struct Window {
        deque<double> buf;
        double sum = 0.0;
    };
    unordered_map<int, Window> windows;
    for (int p : periods) windows[p] = Window();

    // Single pass over prices
    for (size_t i = 0; i < prices.size(); ++i) {
        double price = prices[i];
        for (int p : periods) {
            Window& w = windows[p];
            w.buf.push_back(price);
            w.sum += price;
            if (w.buf.size() > p) {
                w.sum -= w.buf.front();
                w.buf.pop_front();
            }
            if (w.buf.size() == p) {
                result[p][i] = w.sum / p;
            }
        }
    }
    return result;
}

vector<double> SmaModule::computeSMA(const vector<double>& prices, int period) {
    vector<double> result(prices.size(), numeric_limits<double>::quiet_NaN());

    deque<double> buf;
    double sum = 0.0;

    for (size_t i = 0; i < prices.size(); ++i) {
        double price = prices[i];
        buf.push_back(price);
        sum += price;
        if (buf.size() > period) {
            sum -= buf.front();
            buf.pop_front();
        }
        if (buf.size() == period) {
            result[i] = sum / period;
        }
    }
    return result;
}