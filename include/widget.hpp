#pragma once

#include <TFT_eSPI.h>

#include <keyboard.hpp>
#include <rect.hpp>
#include <colors.hpp>
#include <themes.hpp>


namespace UI {
    // NotificationCodes for interaction between widgets
    enum class NotificationCode {
        DESTROY_ME // Request deconstrution at parent level
    };

    class Widget
    {
        public:
        explicit Widget(const Rect &area, Widget * const parent = nullptr);
        explicit Widget(const Rect &area, Theme const& theme, Widget * const parent = nullptr);
        virtual ~Widget();

        virtual void redraw(TFT_eSprite &drawBuffer, const Rect &clientArea) const;
        virtual void setFaceColor(Color color) { faceColor_ = color; }
        Color getFaceColor() const { return faceColor_; }

        virtual void setTheme(Theme const& theme) { faceColor_ = theme.colors.background; }

        void setHidden(bool hidden) { hidden_ = hidden; }
        bool isHidden() { return hidden_; }

        void moveTo(int x, int y) { area_.x = x; area_.y = y; }

        Widget const * getSibling() const {return nextSibling_;}

        void notifyParent(NotificationCode code)
        {
            if (parent_ != nullptr) parent_->onNotify(this, code);
        }

        virtual void onKeyboardEvent(int32_t eventId, UsbKeyboard::EventData const * event);

        protected:
        virtual void draw(TFT_eSprite &drawBuffer, const Rect &clientArea) const;
        virtual void onNotify(Widget* requestOrigin, NotificationCode code);

        Rect area_;
        Widget * parent_;
        Widget * child_;
        Widget * nextSibling_;
        Color faceColor_;
        bool hidden_;
    };
}
