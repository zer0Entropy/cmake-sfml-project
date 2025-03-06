#pragma once

#include <vector>
#include "resource.hpp"

struct Document: public Resource {
    std::vector<std::string>        contents;
    bool loadFromFile(std::string_view path) {
        bool success{false};
        return success;
    }
};