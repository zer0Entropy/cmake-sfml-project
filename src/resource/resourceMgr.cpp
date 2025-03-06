#include "../../include/resource/resourceMgr.hpp"

bool ResourceMgr::DoesResourceExist(ResourceID id, Resource::Type type) const {
    bool exists{false};
    switch(type) {
        case Resource::Type::Font:
            if(fonts.find(id) != fonts.end()) {
                exists = true;
            }
            break;
        case Resource::Type::Texture:
            if(textures.find(id) != textures.end()) {
                exists = true;
            }
            break;
        case Resource::Type::Sound:
            if(sounds.find(id) != sounds.end()) {
                exists = true;
            }
            break;
        case Resource::Type::Music:
            if(musics.find(id) != musics.end()) {
                exists = true;
            }
            break;
        case Resource::Type::Document:
            if(documents.find(id) != documents.end()) {
                exists = true;
            }
            break;
    }
    return exists;
}

ResourceMgr::ErrorCode ResourceMgr::LoadResource(ResourceID id, Resource::Type type, std::string path) {
    ErrorCode outcome{ErrorCode::Success};
    Resource* resourcePtr{nullptr};
    switch(type) {
        case Resource::Type::Font: {
            fonts.insert(std::make_pair(id, std::make_unique<Font>()));
            Font& font{*fonts.at(id)};
            if(font.sfmlFont.openFromFile(path)) {
                resourcePtr = &font;
                resourcePtr->type = Resource::Type::Font;
            }
            else {
                outcome = ErrorCode::FileNotFound;
                fonts.erase(fonts.find(id));
            }
        } break;
        case Resource::Type::Texture: {
            textures.insert(std::make_pair(id, std::make_unique<Texture>()));
            Texture& texture{*textures.at(id)};
            if(texture.sfmlTexture.loadFromFile(path)) {
                resourcePtr = &texture;
                resourcePtr->type = Resource::Type::Texture;
            }
            else {
                outcome = ErrorCode::FileNotFound;
                textures.erase(textures.find(id));
            }
        } break;
        case Resource::Type::Sound: {
            sounds.insert(std::make_pair(id, std::make_unique<Sound>()));
            Sound& sound{*sounds.at(id)};
            if(sound.sfmlSoundBuffer.loadFromFile(path)) {
                resourcePtr = &sound;
                resourcePtr->type = Resource::Type::Sound;
            }
            else {
                outcome = ErrorCode::FileNotFound;
                sounds.erase(sounds.find(id));
            }
        } break;
        case Resource::Type::Music: {
            musics.insert(std::make_pair(id, std::make_unique<Music>()));
            Music& music{*musics.at(id)};
            if(music.sfmlMusic.openFromFile(path)) {
                resourcePtr = &music;
                resourcePtr->type = Resource::Type::Music;
            }
            else {
                outcome = ErrorCode::FileNotFound;
                musics.erase(musics.find(id));
            }
        } break;
        case Resource::Type::Document: {
            documents.insert(std::make_pair(id, std::make_unique<Document>()));
            Document& document{*documents.at(id)};
            if(document.loadFromFile(path)) {
                resourcePtr = &document;
                resourcePtr->type = Resource::Type::Document;
            }
            else {
                outcome = ErrorCode::FileNotFound;
                documents.erase(documents.find(id));
            }
        } break;
    }
    if(resourcePtr) {
        resourcePtr->id = id;
        resourcePtr->path = path;
        resourcePtr->refCount = 0;
    }
    return outcome;
}

Font* ResourceMgr::AcquireFontPtr(ResourceID id) const {
    Font* fontPtr{nullptr};
    auto fontIter{fonts.find(id)};
    if(fontIter != fonts.end()) {
        fontPtr = fontIter->second.get();
        fontPtr->refCount++;
    }
    return fontPtr;
}

Texture* ResourceMgr::AcquireTexturePtr(ResourceID id) const {
    Texture* texturePtr{nullptr};
    auto textureIter{textures.find(id)};
    if(textureIter != textures.end()) {
        texturePtr = textureIter->second.get();
        texturePtr->refCount++;
    }
    return texturePtr;
}

void ResourceMgr::ReleaseFontPtr(Font* font) {
    font->refCount--;
    if(font->refCount == 0) {
        fonts.erase(fonts.find(font->id));
    }
}

void ResourceMgr::ReleaseTexturePtr(Texture* texture) {
    texture->refCount--;
    if(texture->refCount == 0) {
        textures.erase(textures.find(texture->id));
    }
}