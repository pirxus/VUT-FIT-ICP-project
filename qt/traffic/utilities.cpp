#include "utilities.h"
#include <regex>

/**
 * @brief This function strips a string of the initial and trailing whitespace characters
 * @param str
 * @return string
 */
std::string strip_whitespace(std::string str) {
    str = std::regex_replace(str, std::regex("^\\s+"), std::string(""));
    str = std::regex_replace(str, std::regex("\\s+$"), std::string(""));
    return str;
}
