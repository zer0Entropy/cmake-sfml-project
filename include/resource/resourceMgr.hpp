#pragma once

#include <memory>
#include <unordered_map>

#include "font.hpp"
#include "texture.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "document.hpp"

class ResourceMgr {
public:
    enum class ErrorCode {
        Success,
        FileNotFound,
        ResourceUnavailable,
        ResourceInUse
    };

    bool            DoesResourceExist(ResourceID id, Resource::Type type) const;
    ErrorCode       LoadResource(ResourceID id, Resource::Type type, std::string path);
    Font*           AcquireFontPtr(ResourceID id) const;
    Texture*        AcquireTexturePtr(ResourceID id) const;
    void            ReleaseFontPtr(Font* font);
    void            ReleaseTexturePtr(Texture* texture);
    
private:
    std::unordered_map<ResourceID, std::unique_ptr<Font>>       fonts;
    std::unordered_map<ResourceID, std::unique_ptr<Texture>>    textures;
    std::unordered_map<ResourceID, std::unique_ptr<Music>>      musics;
    std::unordered_map<ResourceID, std::unique_ptr<Sound>>      sounds;
    std::unordered_map<ResourceID, std::unique_ptr<Document>>   documents;
};