
#ifndef WP_UTIL
#define WP_UTIL

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

// Tokenizer shamelessly stolen from https://stackoverflow.com/a/2275192
auto tokenize(const std::string& p_pcstStr, char delim) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::stringstream mySstream(p_pcstStr);
    std::string temp;

    while (std::getline(mySstream, temp, delim)) {
        if (temp != "") {
            tokens.push_back(temp);
        }
    }

    return tokens;
}

#endif
