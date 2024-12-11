#pragma once

#include <iostream>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

class Network
{
    std::string m_url;
    // Generic function to make a network request to the url with the payload
    std::string netRequest(const std::string &url, const std::string &payload);
    json getJsonResponse(std::string payload);

public:
    Network(std::string url) : m_url(url) {}
    void getBlockHeight();
    std::pair<uint64_t, json> getSlot();
};
