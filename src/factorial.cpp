#include "factorial.h"

std::vector<int> fact_data;

int fact(int n) {
    if (fact_data.size() <= n) {
        fact_data.emplace_back((n == 0) ? 1 : n * fact(n - 1));
    }
    return fact_data[n];
}
