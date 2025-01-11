#include <iostream>
#include <curl/curl.h>
#include "dmxreceiver.h"
#include <include/json.hpp>
#include <mainwindow.h>
#include <QTimer>


using json = nlohmann::json;

// Fonction de rappel pour écrire les données de réponse dans une chaîne
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

DMXReceiver::DMXReceiver(QObject *parent, WindowP *window) : QObject(parent), dmxData(512, 0), windowPInstance(window) {
    // Initialiser cURL globalement
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Erreur lors de l'initialisation de cURL" << std::endl;
    }

    // Initialiser et configurer le QTimer
    dmxTimer = new QTimer(this);
    connect(dmxTimer, &QTimer::timeout, this, &DMXReceiver::updateDMX);
    dmxTimer->start(200); // Déclencher updateDMX toutes les 200 ms
}

DMXReceiver::~DMXReceiver() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}
void DMXReceiver::updateDMX() {
    if (!curl) return;

    std::string url = "http://localhost:9090/get_dmx?u=7";
    std::string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Erreur lors de la récupération des données : " << curl_easy_strerror(res) << std::endl;
    } else {
        try {
            // Analyser la réponse JSON
            auto j = json::parse(response_string);
            if (j.contains("dmx")) {
                // Extraire les valeurs DMX et les stocker dans le tableau
                auto dmx_values = j["dmx"].get<std::vector<int>>();
                std::copy(dmx_values.begin(), dmx_values.end(), dmxData.begin());
                std::cout << "Données DMX mises à jour." << std::endl;
                windowPInstance->processDMXData();
            }
        } catch (const json::exception& e) {
            std::cerr << "Erreur lors de l'analyse du JSON: " << e.what() << std::endl;
        }
    }
}


