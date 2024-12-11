#pragma once

#include "json.hpp"
using nlohmann::json;

#include <queue>
#include <vector>
#include <chrono>
#include <mutex>
#include <thread>
#include <set>

struct Item
{
    uint64_t slot;     // Slot number
    double latency_ms; // Latency
    json payload;

    // Most recent first
    bool operator<(const Item &other) const;
};

class Container
{
private:
    std::multiset<Item> results; // Sorted results based on timestamp
    std::mutex mtx;

    size_t count = 0;  
    double sum = 0.0; 
    double sum_sqr = 0.0;

    void add_to_stats(double latency);
    void remove_from_stats(double latency);

public:
    void add_result(const Item &result);
    std::optional<Item> get_latest_result();

    // Prune results outside the time window [X-T, X]
    void prune(uint64_t cutoff);
    double get_standard_deviation();
};
