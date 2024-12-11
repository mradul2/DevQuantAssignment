#include "container.hpp"
#include "network.hpp"
#include "event_handler.hpp"

void EventHandler::event_job()
{
    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto response = network.getSlot();
        uint64_t slot = response.first;
        auto end = std::chrono::high_resolution_clock::now();

        double latency = std::chrono::duration<double, std::milli>(end - start).count();

        Item result{
            slot,
            latency,
            response.second};

        container.add_result(result);

        // Prune results outside the time window
        auto cutoff = result.slot - time_window;
        container.prune(cutoff);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during INVOKE: " << e.what() << std::endl;
    }
}

void EventHandler::handle_event(EventType event)
{
    switch (event)
    {
    case EventType::INVOKE:
    {
        std::thread worker([this]()
                           { this->event_job(); });
        worker.detach();
        break;
    }

    case EventType::NOTHING:
    {
        break;
    }

    case EventType::ERROR:
    {
        std::cerr << "ERROR event received: Something went wrong!" << std::endl;
        break;
    }
    }
}