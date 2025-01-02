#include "passkey_app.hpp"
#include "sdcard.hpp"
#include "config.hpp"

#include <functional>

PassKeyApplication::PassKeyApplication(UI::Theme const& theme, Application * parent): UI::Application(parent),
    keyboard_(false),
    statusBar_(0, theme, this),
    typekeyMenu_(
        [this]( UI::AbstractMenuBar& menuBar, UI::AbstractMenuBar::EventData const& eventData)
        {
            onMenuEvent(menuBar, eventData);
        }, 
        this, 
        UI::VerticalMenu, 
        loadDirectoryContent(), 
        getMenuArea(),
        theme, 
        0),
        state_(ApplicationState::SelectPassword),
        previousState_(ApplicationState::SelectPassword),
        selectedItem_("")
{
    typekeyMenu_.makeActive();
}

PassKeyApplication::~PassKeyApplication()
{
}

void PassKeyApplication::update()
{
    UI::Application::update(); //Call the update from the base class

    ApplicationState tmp = state_;

    switch(state_)
    {
        case  ApplicationState::SelectPassword: handleSelectPassword(); break;
        case  ApplicationState::ResetLedsBeforePassword: handleResetLedsBeforePassword(); break;
        case  ApplicationState::TypePassword: handleTypePassword(); break;
        case  ApplicationState::ResetLedsAfterPassword: handleResetLedsAfterPassword(); break;
    }

    previousState_ = state_;

    keyboard_.tick(); // Tick the Keyboard here
}

void PassKeyApplication::onKeyboardEvent(int32_t eventId, UsbKeyboard::EventData const * const event)
{
    if (eventId == UsbKeyboard::LedsUpdated && event)
    {
        statusBar_.setCapsLockStatus(event->self->isCapsLockSet());
        statusBar_.setNumLockStatus(event->self->isNumLockSet());
        statusBar_.setScrollLockStatus(event->self->isScrollLockSet());
    }
}

UI::AbstractMenuBar::MenuItems PassKeyApplication::loadDirectoryContent()
{
  UI::AbstractMenuBar::MenuItems result;
  std::shared_ptr<SDCard> sdCard = SDCard::load();
  std::shared_ptr<DIR> directory = sdCard->openDir("");

  dirent *dir;
  while((dir = readdir(directory.get())))
  {
      if(DT_REG == dir->d_type)
      {
            if (dir->d_name != configFileName())
        {
                result.emplace_back(String(dir->d_name));
        }
      }
  }

  if(result.empty()) {
    result.emplace_back(String("No Files found"));
  }

  return result;
}

void PassKeyApplication::onMenuEvent(UI::AbstractMenuBar &menuBar, UI::AbstractMenuBar::EventData const &eventData)
{
        if(state_ == ApplicationState::SelectPassword) 
        {
            selectedItem_ = menuBar.selectedItem();
            state_ = ApplicationState::ResetLedsBeforePassword;
        }
}

UI::Rect PassKeyApplication::getMenuArea() const
{
    UI::Rect screen = getFullFrameRect();
    
    return UI::Rect(0,1,screen.width, screen.height -1);
}

void PassKeyApplication::handleSelectPassword()
{
    typekeyMenu_.enable();
}

void PassKeyApplication::handleResetLedsBeforePassword()
{
    if(previousState_ == state_)
    {
        typekeyMenu_.disable();
        if(keyboard_.ledsNeedReset())
        {
            keyboard_.restoreOriginalLedState();
        }

        state_ = ApplicationState::TypePassword;
    } 
}

void PassKeyApplication::handleTypePassword()
{
    if(previousState_ == state_)
    {
        std::shared_ptr<SDCard> sdCard = SDCard::load();
        KeyStrokeFile file(sdCard->open(selectedItem_, SDCard::OpenMode::FILE_READONLY));
        keyboard_.sendKeyStrokes(file);

        state_ = ApplicationState::ResetLedsAfterPassword;
    }
}

void PassKeyApplication::handleResetLedsAfterPassword()
{
    if(previousState_ == state_)
    {
        if(keyboard_.ledsNeedReset())
        {
            keyboard_.restoreOriginalLedState();
        }

        state_ = ApplicationState::SelectPassword;
    }
}
