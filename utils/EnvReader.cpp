#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include "EnvReader.h"

EnvReader::EnvReader(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the .env file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::string key, value;
        std::istringstream keyValueStream(line);
        if (std::getline(keyValueStream, key, '=') && std::getline(keyValueStream, value)) {
            trim(key);
            trim(value);
            envData[key] = value;
        }
    }
}

std::string EnvReader::get(const std::string& key, const std::string& defaultValue) const {
    auto it = envData.find(key);
    if (it != envData.end()) {
        return it->second;
    }
    return defaultValue;
}

void EnvReader::trim(std::string& str) {
    const char* whitespace = " \t\n\r";
    str.erase(0, str.find_first_not_of(whitespace));
    str.erase(str.find_last_not_of(whitespace) + 1);

    if (str.size() >= 2 && str[0] == '"' && str[str.size() - 1] == '"') {
        str = str.substr(1, str.size() - 2);
    }
}
