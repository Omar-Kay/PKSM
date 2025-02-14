#pragma once

#include <memory>
#include <pu/Plutonium>
#include <string>
#include <vector>

#include "data/AccountManager.hpp"
#include "data/providers/interfaces/ISaveDataProvider.hpp"
#include "data/providers/interfaces/ITitleDataProvider.hpp"
#include "data/titles/Title.hpp"
#include "gui/screens/title-load-screen/sub-components/SaveList.hpp"
#include "gui/screens/title-load-screen/sub-components/game-list/GameList.hpp"
#include "gui/shared/components/FocusableButton.hpp"
#include "gui/shared/components/FocusableImage.hpp"
#include "gui/shared/components/UserIconButton.hpp"
#include "input/directional/DirectionalInputHandler.hpp"
#include "input/visual-feedback/FocusManager.hpp"

namespace pksm::layout {
class TitleLoadScreen : public pu::ui::Layout {
private:
    // Header sectionimage.png
    static constexpr u32 HEADER_TOP_MARGIN = 24;  // Space between screen top and header
    static constexpr u32 HEADER_HEIGHT = 80;  // Height of header text area
    static constexpr u32 HEADER_BOTTOM_MARGIN = 60;  // Space between header and game list
    static constexpr u32 HEADER_TOTAL_VERTICAL_SPACE = HEADER_TOP_MARGIN + HEADER_HEIGHT + HEADER_BOTTOM_MARGIN;

    // User icon section
    static constexpr u32 USER_ICON_SIZE = 72;
    static constexpr u32 USER_ICON_MARGIN = 28;

    // Game list section
    static constexpr u32 GAME_LIST_SIDE_MARGIN = 70;  // Side margin for game list component

    // Save list section (bottom)
    static constexpr u32 SAVE_LIST_TOP_MARGIN = 48;  // Space between game list and save list
    static constexpr u32 SAVE_LIST_WIDTH = 1240;
    static constexpr u32 SAVE_LIST_X = GAME_LIST_SIDE_MARGIN;
    static constexpr u32 SAVE_ITEM_HEIGHT = 48;
    static constexpr u32 SAVE_LIST_MAX_VISIBLE_ITEMS = 5;
    static constexpr u32 SAVE_LIST_HEIGHT = SAVE_ITEM_HEIGHT * SAVE_LIST_MAX_VISIBLE_ITEMS;
    static constexpr u32 SAVE_LIST_BOTTOM_MARGIN = 48;  // Space between save list and bottom
    static constexpr u32 SAVE_LIST_TOTAL_VERTICAL_SPACE = SAVE_LIST_TOP_MARGIN + SAVE_LIST_HEIGHT +
        SAVE_LIST_BOTTOM_MARGIN;

    // Buttons
    static constexpr u32 BUTTON_WIDTH = 508;
    static constexpr u32 BUTTON_HEIGHT = 111;
    static constexpr u32 BUTTON_SPACING = 20;

    // Input handling
    pksm::input::DirectionalInputHandler buttonHandler;
    pksm::input::DirectionalInputHandler saveListHandler;
    pksm::input::DirectionalInputHandler gameListHandler;
    pksm::input::DirectionalInputHandler userIconButtonHandler;

    // Focus management
    pksm::input::FocusManager::Ref titleLoadFocusManager;
    pksm::input::FocusManager::Ref gameListManager;

    // UI Elements
    pu::ui::elm::TextBlock::Ref headerText;
    pksm::ui::GameList::Ref gameList;
    pksm::ui::SaveList::Ref saveList;
    pksm::ui::FocusableButton::Ref loadButton;
    pksm::ui::FocusableButton::Ref wirelessButton;
    pksm::ui::UserIconButton::Ref userIconButton;

    // Data providers and managers
    ITitleDataProvider::Ref titleProvider;
    ISaveDataProvider::Ref saveProvider;
    data::AccountManager& accountManager;

    // Event handlers
    void OnSaveSelected();
    void OnLoadButtonClick();
    void OnWirelessButtonClick();
    void OnInput(u64 down, u64 up, u64 held);
    void OnGameTouchSelect();
    void OnSaveListTouchSelect();

    // Helper methods
    void LoadSaves();
    pksm::titles::Title::Ref GetSelectedTitle() const;
    pu::i32 GetBottomSectionY() const;  // Helper to calculate Y position for save list and buttons

    // Navigation helpers
    void MoveButtonSelectionUp();
    void MoveButtonSelectionDown();
    void TransitionToSaveList();
    void TransitionToButtons();
    void FocusGameSection();
    void FocusSaveList();
    void HandleButtonInteraction(pksm::ui::FocusableButton::Ref& buttonToFocus);

public:
    TitleLoadScreen(
        ITitleDataProvider::Ref titleProvider,
        ISaveDataProvider::Ref saveProvider,
        data::AccountManager& accountManager
    );
    PU_SMART_CTOR(TitleLoadScreen)
};
}  // namespace pksm::layout
