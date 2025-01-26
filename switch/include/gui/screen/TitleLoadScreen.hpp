#pragma once

#include <pu/Plutonium>
#include "titles/Title.hpp"
#include "gui/FocusableButton.hpp"
#include "gui/FocusableImage.hpp"
#include "gui/FocusableMenu.hpp"
#include <vector>
#include <string>
#include <memory>

class TitleLoadScreen : public pu::ui::Layout {
private:
    // Layout constants
    static constexpr u32 SCREEN_WIDTH = 1280;
    static constexpr u32 SCREEN_HEIGHT = 720;
    static constexpr u32 HEADER_HEIGHT = 80;
    static constexpr u32 HEADER_BOTTOM_MARGIN = 20;
    static constexpr u32 GAME_SECTION_Y = 180;
    
    // Game card section (left side)
    static constexpr u32 GAME_CARD_SIZE = 350;
    static constexpr u32 GAME_CARD_X = 150;
    
    // Section divider
    static constexpr u32 SECTION_DIVIDER_PADDING = 100;
    static constexpr u32 SECTION_DIVIDER = GAME_CARD_X + GAME_CARD_SIZE + SECTION_DIVIDER_PADDING;
    
    // Installed games section (right side)
    static constexpr u32 INSTALLED_START_X = SECTION_DIVIDER + SECTION_DIVIDER_PADDING;
    static constexpr u32 INSTALLED_GAME_SIZE = 240;
    static constexpr u32 GAME_SPACING = 280;
    
    // Save list section (bottom)
    static constexpr u32 SAVE_LIST_TOP_MARGIN = 140;
    static constexpr u32 SAVE_LIST_WIDTH = 1100;
    static constexpr u32 SAVE_LIST_HEIGHT = 300;
    static constexpr u32 SAVE_LIST_X = (SCREEN_WIDTH - SAVE_LIST_WIDTH) / 2 + 105;
    static constexpr u32 SAVE_LIST_Y = GAME_SECTION_Y + GAME_CARD_SIZE + 250;
    
    // Buttons
    static constexpr u32 BUTTON_WIDTH = 400;
    static constexpr u32 BUTTON_HEIGHT = 117;
    static constexpr u32 BUTTON_SPACING = 20;
    static constexpr u32 SAVE_ITEM_HEIGHT = 50;

    // UI Elements
    pu::ui::elm::TextBlock::Ref headerText;
    pu::ui::elm::TextBlock::Ref cartridgeText;
    pu::ui::elm::TextBlock::Ref installedText;
    FocusableImage::Ref gameCardImage;
    std::vector<FocusableImage::Ref> installedGameImages;
    FocusableMenu::Ref saveList;
    FocusableButton::Ref loadButton;
    FocusableButton::Ref wirelessButton;

    // Mock data (will be replaced with real data later)
    titles::TitleRef mockCartridgeTitle;
    std::vector<titles::TitleRef> mockInstalledTitles;
    int selectedTitle;  // -2: none, -1: cartridge, >= 0: installed game index
    
    // Save list state
    int selectedSave = -1;
    int firstSave = -1;
    std::vector<std::string> availableSaves;

    // Event handlers
    void OnSaveSelected();
    void OnLoadButtonClick();
    void OnWirelessButtonClick();
    void OnSaveItemKey();
    void OnInput(u64 down, u64 up, u64 held);

    // Helper methods
    void LoadSaves();
    bool LoadSelectedSave();

public:
    TitleLoadScreen();
    PU_SMART_CTOR(TitleLoadScreen)
}; 