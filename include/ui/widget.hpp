#pragma once

#include <vector>

struct WidgetNode {

    enum class Type {
        Menu,
        MenuOption
    };

    WidgetNode*                 parent;
    std::vector<WidgetNode*>    children;
    Type                        type;
    
    std::optional<unsigned int> GetIndex() const {
        if(parent) {
            int childCount{0};
            for(auto child : parent->children) {
                if(child == this) {
                    return childCount;
                }
                ++childCount;
            }
        }
        return std::nullopt;
    }
};