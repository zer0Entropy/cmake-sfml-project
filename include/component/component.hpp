#pragma once

#include <array>

using EntityID = unsigned int;

struct Component {
    enum class Type {
        Text,
        Sprite,
        Transform,
        BoundingBox,
        MouseOver,
        LeftClick,
        RightClick,
        Widget,
        Creature,
        NumTypes
    };

    EntityID        ownerID;
    Type            type;
    bool            enabled;
};

void InitComponent(Component& component, EntityID ownerID, Component::Type type);

using ComponentList = std::array<Component*, (unsigned int)Component::Type::NumTypes>;

using Entity = std::pair<EntityID, ComponentList>;

constexpr unsigned int MaxNumEntities{1024};