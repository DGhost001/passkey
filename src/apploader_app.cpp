#include "apploader_app.hpp"
#include "config.hpp"

#include <functional>
#include <algorithm>

AppLoaderApplication::AppLoaderApplication(std::initializer_list<AbstractAppItem*> appItems) :
    UI::Application(),
    doUnload_(false)
{
}

AppLoaderApplication::~AppLoaderApplication()
{
}

void AppLoaderApplication::update()
{
    if (doUnload_)
    {
        rotate();
        load(); // Loads front and unloads anything else that potentially existed already
        doUnload_ = false;
    }

    if (currentApp_)
    {
        currentApp_->update();
    }
    else
    {
        UI::Application::update(); // Call the update from the base class
    }
}


void AppLoaderApplication::load(unsigned index)
{
    currentApp_ = apps_.at(index)->create(this);
}


void AppLoaderApplication::unload()
{
    currentApp_.reset();
}


void AppLoaderApplication::rotate()
{
    std::rotate(apps_.begin(), apps_.begin() + 1, apps_.end());
}


void AppLoaderApplication::onKeyboardEvent(int32_t eventId, UsbKeyboard::EventData const * event)
{
    if (currentApp_)
    {
        currentApp_->onKeyboardEvent(eventId, event);
    }
}


void AppLoaderApplication::onNotify(Widget* requestOrigin, UI::NotificationCode code)
{
    switch (code)
    {
        case UI::NotificationCode::DESTROY_ME:
            doUnload_ = true;
            break;
    }
}

