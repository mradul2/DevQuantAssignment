#include "network.hpp"

std::size_t callback(
    const char *in,
    std::size_t size,
    std::size_t num,
    std::string *out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

std::string Network::netRequest(const std::string &url, const std::string &payload)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURL *curl = curl_easy_init();
    std::unique_ptr<std::string> httpData(new std::string());

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        // Add the Content-Type header
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return *httpData.get();
}

json Network::getJsonResponse(std::string payload)
{
    std::string response = netRequest(m_url, payload);
    json jsonResponse = json::parse(response);
    return jsonResponse;
}

void Network::getBlockHeight()
{
    std::string payload = R"({
        "jsonrpc": "2.0",
        "id": 1,
        "method": "getBlockHeight"
    })";

    auto response = getJsonResponse(payload);

    std::cout << "Block Height: " << response["result"] << std::endl;
}

std::pair<uint64_t, json> Network::getSlot()
{
    std::string payload = R"({
            "jsonrpc": "2.0",
            "id": 1,
            "method": "getSlot"
        })";

    auto response = getJsonResponse(payload);

    return std::make_pair(response["result"], response);
}