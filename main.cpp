#include <iostream>
#include <curl/curl.h>
#include "./lib/rapidjson/document.h"
#include "./lib/rapidjson/writer.h"
#include "./lib/rapidjson/stringbuffer.h"
#include "./utils/EnvReader.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string traducir(const std::string& texto, const std::string& langOrigen, const std::string& langDestino, const std::string& apiKey, CURL* curl) {
    // Datos
    std::string url = "https://api-free.deepl.com/v2/translate";
    std::string authKey = "DeepL-Auth-Key " + apiKey;
    std::string jsonData = R"({
        "text": [
            ")" + texto + R"("
        ],
        "source_lang": ")" + langOrigen + R"(",
        "target_lang": ")" + langDestino + R"("
    })";
    std::string responseString;

    // Headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_reset(curl);
    curl_easy_cleanup(curl);

    // Parsear JSON
    rapidjson::Document document;
    document.Parse(responseString.c_str());
    if (document.HasParseError()) {
        std::cerr << "Error al parsear JSON" << std::endl;
        return "";
    }

    std::string translatedText = document["translations"][0]["text"].GetString();
    return translatedText;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (!curl) {
        return 1;
    }

    EnvReader env(".env");
    std::string deeplApiKey = env.get("DEEPL_API_KEY", "");

    bool continuarMenuPrincipal = true;

    while (continuarMenuPrincipal) {
        std::cout << "1. Traducir texto" << std::endl;
        std::cout << "2. Salir" << std::endl;
        std::cout << "Seleccione una opción: ";
        int opcionMenuPrincipal;
        std::cin >> opcionMenuPrincipal;

        switch (opcionMenuPrincipal) {
            case 1: {
                std::string texto;
                std::cout << "Introduzca el texto a traducir: ";
                std::cin.ignore();
                std::getline(std::cin, texto);

                std::string textoTraducido = traducir(texto, "ES", "EN", deeplApiKey, curl);
                std::cout << "Texto traducido: " << textoTraducido << std::endl;
                break;
            }
            case 2: {
                continuarMenuPrincipal = false;
                break;
            }
            default: {
                std::cout << "Opción no válida" << std::endl;
                break;
            }
        }
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.example.com/data");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;

    return 0;
}
