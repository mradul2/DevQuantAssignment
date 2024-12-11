# HFT DevQuant Assignment

## Architectural details 

Code is organised in the following manner: 

### Network

Seperate class to create curl requests to Solana API and provide user interfaces to implement functions such as `getBlockHeight` and `getSlot` 

Scalability - to implement more Solana API functions, one just need to add the corresponing method with the appropriate payload. 

Use libcurl and JSON library for making and parsing the requests. 

### Container 

Container class for storing the respones in sorted order or slots (temporal marker for solana), it uses a `std::multiset` to store the custom entity type in sorted order of its `slot` attribute. 

`Container` object also stores the latency and payload for the curl request. It exposes following methods to the user: 

1. `add_result` : Add the entity to the container (update the required statistics)
2. `prune` : Remove entities from the container which are not valid for the current time window of `[X-T, T]` (and update the statistics)
3. `get_latest_result` : Returns the oldest payload and the corresponding slot from the container (sorted in order of slots)
4. `get_standard_deviation` : Returns the std deviation of current time window of `[X-T, X]` using the statistics stored within the container. 

Statistics stored
- Number of items 
- Sum of latencies 
- Sum of square of latencies

### Event Handler 

`EventHandler` class implements the `IEventHandler` class by defining the method `handle_event` which according to the type of `EventType` spawns a `std::thread` and detaches it from the main thread. 

The thread calls `network.getSlot()` which makes the curl request then it add the result into the container, prune the old results and measures the latency of the curl request. 

Data sharing inside the container is protected using `std::lock_guard<std::mutex>` to ensure atomicity. 

### Simulation

All the 3 tasks are simulated in `main.cpp` in which instances of `Network`, `Container` and `EventHandler` are created and a random sequence of `EventType` are simulted using them. 

## Instructions 

Compile 

```
apt install curl
make all
```

Run the simulations 

```
./program
```


## Libraries used

1. [nlohmann/json](https://github.com/nlohmann/json) - To parse the JSON responses 
2. [libcurl](https://curl.se/libcurl/) - To make curl requests to Solana server

Single header is included for the JSON library and libcurl is linked during compilation