#include "event_handler.hpp"

int main()
{
    // Task 1
    Network network("https://api.devnet.solana.com");
    network.getBlockHeight();

    // Task 2 and 3
    Container container;
    uint64_t time_window(5); // 5-unit window
    EventHandler handler(container, network, time_window);

    // Simulate events
    std::vector<EventType> events = {
        EventType::NOTHING, EventType::NOTHING, EventType::ERROR,
        EventType::INVOKE, EventType::NOTHING, EventType::INVOKE,
        EventType::ERROR, EventType::INVOKE, EventType::NOTHING,
        EventType::INVOKE, EventType::INVOKE, EventType::INVOKE,
        EventType::INVOKE, EventType::INVOKE, EventType::INVOKE,
        EventType::INVOKE, EventType::INVOKE, EventType::INVOKE,
        EventType::INVOKE, EventType::INVOKE, EventType::INVOKE,
        EventType::INVOKE, EventType::INVOKE, EventType::INVOKE,
        EventType::NOTHING, EventType::INVOKE, EventType::INVOKE};

    for (size_t i = 0; i < events.size(); ++i)
    {
        std::cout << "Processing event " << i + 1 << "/" << events.size() << "..." << std::endl;

        handler.handle_event(events[i]);

        // Simulate a delay between events
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "\n=== Querying the container ===" << std::endl;

        auto result = container.get_latest_result();
        if (result)
        {
            std::cout << "Slot: " << result->slot << std::endl;
            std::cout << "Latency: " << result->latency_ms << " ms" << std::endl;
            double std_dev = container.get_standard_deviation();
            std::cout << "Standard Deviation of Latencies: " << std_dev << " ms" << std::endl;
        }
        else
        {
            std::cout << "No results in the container.\n";
        }
    }

    return 0;
}