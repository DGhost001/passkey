#include <Arduino.h>
#include <TFT_eSPI.h>

#include <aes/esp_aes.h>

#include <list>
#include <string_view>

#include "passkey_app.hpp"
#include "apploader_app.hpp"

#include "themes.hpp"
#include "macros.hpp"
#include "config.hpp"

static AppLoaderApplication* application;

// Available Apps
using PassKeyAppItem = AppItem<PassKeyApplication, UI::Theme const&>;
static PassKeyAppItem* passkeyApp;


static void propagateKeyboardEvent(
    void *event_handler_arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
  if (event_base == KEYBOARD_EVENT)
  {
      UsbKeyboard::EventData *event = reinterpret_cast<UsbKeyboard::EventData *>(event_data);
      application->onKeyboardEvent(event_id, event);
  }
}

static constexpr std::string_view themeDefault_ = "robotron";
#ifndef PASSKEY_THEME
    static constexpr std::string_view themeSet_ = themeDefault_;
#else
    static constexpr std::string_view themeSet_ = PASSKEY_STRINGIZE(PASSKEY_THEME);
#endif // PASSKEY_THEME

void setup()
{
    esp_event_loop_create_default();
    esp_event_handler_register(
        KEYBOARD_EVENT,
        UsbKeyboard::LedsUpdated,
        propagateKeyboardEvent,
        NULL);

    try
    {
        if constexpr (themeSet_ == "random")
        {
            auto it = UI::themes().begin();
            std::advance(it, random(UI::themes().size()));
            passkeyApp = new PassKeyAppItem("Passkey", it->second);
        }
        else
        {
            UI::Theme const& theme = UI::themes().at(String(themeSet_.data()));
            passkeyApp = new PassKeyAppItem("Passkey", theme);
        }
    }
    catch (std::out_of_range)
    {
        UI::Theme const& theme = UI::themes().at(String(themeDefault_.data()));
        passkeyApp = new PassKeyAppItem("Passkey", theme);
    }

    application = new AppLoaderApplication{
        passkeyApp
    };
    application->load();
}

void loop()
{
    application->update();
    delay(5);
}
