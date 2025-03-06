#include "../../include/component/component.hpp"

void InitComponent(Component& component, EntityID ownerID, Component::Type type) {
    component.ownerID = ownerID;
    component.type = type;
    component.enabled = true;
}