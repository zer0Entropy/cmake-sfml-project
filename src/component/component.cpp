#include "../../include/component/component.hpp"

void InitComponent(Component& component, EntityID ownerID, Component::Type type) {
    component.ownerID = ownerID;
    component.type = type;
    component.enabled = true;
}

Component* GetComponent(Entity& entity, Component::Type type) {
    for(auto cmp : entity.second) {
        if(cmp && cmp->type == type) {
            return cmp;
        }
    }
    return nullptr;
}