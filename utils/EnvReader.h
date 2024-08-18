#ifndef ENV_READER_H
#define ENV_READER_H

#include <iostream>
#include <unordered_map>

class EnvReader {
    public:
        EnvReader(const std::string& filename);
        std::string get(const std::string& key, const std::string& defaultValue = "") const;
    private:
        std::unordered_map<std::string, std::string> envData;
        static void trim(std::string& str);
};

#endif