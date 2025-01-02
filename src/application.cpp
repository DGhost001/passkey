#include "application.hpp"
#include "ec1834.hpp"

namespace UI {
    Application::Application(Application * parent):
        Widget(getFullFrameRect(), parent),
        tft_(),
        backBuffer_(&tft_)
    {
        tft_.init();

        backBuffer_.createSprite(TFT_HEIGHT, TFT_WIDTH);
        backBuffer_.setAttribute(UTF8_SWITCH,0);
        backBuffer_.fillScreen(TFT_BLACK);
        backBuffer_.setTextColor(TFT_GREEN, TFT_BLACK);
        ec1834bin_select(backBuffer_);

        tft_.setAttribute(UTF8_SWITCH,0);
        tft_.setRotation(1);
        tft_.fillScreen(TFT_BLACK);
        tft_.setTextColor(TFT_GREEN, TFT_BLACK);
        ec1834bin_select(tft_);
    }

    void Application::update()
    {
        redraw(backBuffer_, getFullFrameRect());
        backBuffer_.pushSprite(0,0);
    }
}
