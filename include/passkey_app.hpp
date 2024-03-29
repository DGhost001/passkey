#pragma once

#include "application.hpp"
#include "keyboard.hpp"
#include "statusbar.hpp"
#include "menu.hpp"
#include "themes.hpp"

class PassKeyApplication: public UI::Application
{
    public:
        PassKeyApplication(UI::Theme const& theme);
        virtual ~PassKeyApplication();

        void update() override;

        void updateStatusBar(UsbKeyboard::EventData const * const);

    private:
        enum class ApplicationState
        {
            SelectPassword,
            ResetLedsBeforePassword,
            TypePassword,
            ResetLedsAfterPassword
        };

        UsbKeyboard keyboard_;
        Statusbar statusBar_;
        UI::Menu typekeyMenu_;
        ApplicationState state_;
        ApplicationState previousState_;
        String selectedItem_;

        UI::AbstractMenuBar::MenuItems loadDirectoryContent();
        void onMenuEvent(UI::AbstractMenuBar& menuBar, UI::AbstractMenuBar::EventData const& eventData);
        UI::Rect getMenuArea() const;

        void handleSelectPassword();
        void handleResetLedsBeforePassword();
        void handleTypePassword();
        void handleResetLedsAfterPassword();
};
