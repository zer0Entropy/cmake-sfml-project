#include "../include/state.hpp"

void GameState::CenterEntity(EntityID ownerID) {
    TransformComponent& transform{transformCmps[ownerID]};
    BoundingBoxComponent& boundingBox{boundingBoxCmps[ownerID]};
    if(transform.enabled && boundingBox.enabled) {
        transform.sfmlTransformable.setOrigin({
            boundingBox.sfmlBoundingBox.size.x / 2.f,
            boundingBox.sfmlBoundingBox.size.y / 2.f
        });
    }
}