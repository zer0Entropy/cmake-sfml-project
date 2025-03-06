#pragma once

#include "state.hpp"

struct MainMenuState: public GameState {
    Entity                          mainMenu;
    Entity                          newGameOption;
    Entity                          loadGameOption;
    Entity                          settingsOption;
    Entity                          quitGameOption;

    std::optional<unsigned int>     selectedIndex;
    std::optional<unsigned int>     pressedIndex;

    static constexpr std::string_view newGameContents{"New Game"};
    static constexpr std::string_view loadGameContents{"Load Game"};
    static constexpr std::string_view settingsContents{"Settings"};
    static constexpr std::string_view quitGameContents{"Quit Game"};

    static const sf::Vector2u newGameRelPosition;
    static const sf::Vector2u loadGameRelPosition;
    static const sf::Vector2u settingsRelPosition;
    static const sf::Vector2u quitGameRelPosition;

    static constexpr std::string_view backgroundID{"mainMenuBackground"};
    static constexpr std::string_view backgroundPath{"resource/brown_age_by_darkwood67.jpg"};

    static constexpr unsigned int fontSize{48};
    static constexpr std::string_view fontID{"mainMenuFont"};
    static constexpr std::string_view fontPath{"resource/TruenoBlack-mBYV.otf"};
    static constexpr float fontOutlineThickness{1.0f};
    static const sf::Color fontOutlineColor;
    static const sf::Color fontFillColor;

    static constexpr float fontHighlightThickness{4.0f};
    static const sf::Color fontHighlightColor;

    void DoAction(ActionID action, std::optional<EntityID> ownerID = std::nullopt);
    void HighlightSelectedOption(EntityID menuOptionID);
    void ResetHighlight();
};
