#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>

#include "evpp/inner_pre.h"

namespace evpp {
namespace httpc {
struct URLParser {
public:
    std::string schema;
    std::string host;
    int port;
    std::string path;
    std::string query;

    URLParser(const std::string& url);

private:
    int parse(const std::string& url);
};
} // httpc
} // evpp