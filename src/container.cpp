#include "container.hpp"

bool Item::operator<(const Item &other) const
{
    return slot < other.slot;
}

void Container::add_result(const Item &result)
{
    std::lock_guard<std::mutex> lock(mtx); 
    results.insert(result);
    add_to_stats(result.latency_ms);
}

std::optional<Item> Container::get_latest_result()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (results.empty())
        return std::nullopt;

    Item latest = *results.rbegin(); 
    return latest;
}

void Container::add_to_stats(double latency)
{
    count++;
    sum += latency;
    sum_sqr += latency * latency;
}

void Container::remove_from_stats(double latency)
{
    if (count <= 1)
    {
        count = 0;
        sum = 0;
        sum_sqr = 0;
    }
    else
    {
        sum -= latency;
        sum_sqr -= latency * latency;
        count--;
    }
}

// Prune results outside the time window [X-T, X]
void Container::prune(uint64_t cutoff)
{
    std::lock_guard<std::mutex> lock(mtx);
    while (!results.empty() && results.begin()->slot < cutoff)
    {
        remove_from_stats(results.begin()->latency_ms);
        results.erase(results.begin());
    }
}

// Get the current standard deviation
double Container::get_standard_deviation()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (count < 2)
        return 0.0;
    double res = sum_sqr / count - (sum / count) * (sum / count);
    return std::sqrt(res);
}