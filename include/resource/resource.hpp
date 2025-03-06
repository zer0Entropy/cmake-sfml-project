#pragma once

#include <memory>
#include <string>
#include <unordered_map>

using ResourceID = std::string;

struct Resource {
    enum class Type {
        Font,
        Texture,
        Sound,
        Music,
        Document
    };

    ResourceID      id;
    Type            type;
    std::string     path;
    unsigned int    refCount;
};