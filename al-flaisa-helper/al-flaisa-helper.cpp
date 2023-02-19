#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "pch.h" 

using json = nlohmann::json;

class FlightEmissions
{
private:
    std::string API_KEY;

public:
    FlightEmissions(std::string apiKey) : API_KEY(apiKey) {}

    double getEmissions(std::string origin, std::string destination)
    {
        CURL* curl;
        CURLcode res;
        std::string response;

        curl = curl_easy_init();
        if (curl)
        {
            std::string url = "https://maps.googleapis.com/maps/api/directions/json?origin=" + origin + "&destination=" + destination + "&key=" + API_KEY;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK)
            {
                json j = json::parse(response);
                int distance = j["routes"][0]["legs"][0]["distance"]["value"];
                double emissions = distance * 0.25 / 1000; // Assume that every 1 km of flight causes 0.25 kg of CO2 emissions

                return emissions;
            }
        }

        return -1;
    }

private:
    static size_t writeFunction(void* ptr, size_t size, size_t nmemb, void* userdata)
    {
        std::string* response = (std::string*)userdata;
        response->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }
};

extern "C" __declspec(dllexport) FlightEmissions * create(std::string apiKey)
{
    return new FlightEmissions(apiKey);
}

extern "C" __declspec(dllexport) double getEmissions(FlightEmissions * flightEmissions, std::string origin, std::string destination)
{
    return flightEmissions->getEmissions(origin, destination);
}
