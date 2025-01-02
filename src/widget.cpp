#include "widget.hpp"

namespace UI
{
    Widget::Widget(const Rect &area, Widget * const parent):
        area_(area),
        parent_(parent),
        child_(nullptr),
        nextSibling_(nullptr),
        faceColor_(DefaultFaceColor),
    hidden_(false)
    {
        if (parent_) {

            Widget * firstChild = parent_->child_;
            if (firstChild) {
                nextSibling_ = firstChild;
            }
            parent->child_ = this;
        }
    }

    Widget::Widget(const Rect &area, Theme const& theme, Widget * const parent):
        Widget(area, parent)
    {
        setTheme(theme);
    }

    Widget::~Widget()
    {
        if(parent_) {
            Widget * previousChild = nullptr;
            Widget * child = parent_->child_;

            while(child && child != this) {
                previousChild = child;
                child = child ->nextSibling_;
            }

            if(previousChild && child) {
                previousChild ->nextSibling_ = nextSibling_;
            } else if( child ) {
                parent_->child_ = nextSibling_;
            } // else the child was not found which would be an inconsistency error and should never happen
        }
    }

    void Widget::redraw(TFT_eSprite &drawBuffer, const Rect &clientArea) const
    {
    if (hidden_) return;

        Rect const newClientArea = clientArea.intersect(area_);

        if(newClientArea.isValid())
        {
            draw(drawBuffer, newClientArea);

            Widget const * child = child_;
            while(child) {
                child->redraw(drawBuffer,newClientArea);
                child = child->nextSibling_;
            }
        }
    }

    void Widget::draw(TFT_eSprite &drawBuffer, const Rect &clientArea) const
    {
        Rect area = clientArea.toScreen();
        drawBuffer.fillRect(area.x, area.y, area.width, area.height, faceColor_);
    }

    void Widget::onNotify(Widget*, NotificationCode)
    {
        // Default action is to do nothing
    }

    void Widget::onKeyboardEvent(int32_t, UsbKeyboard::EventData const *)
    {
        // Default action is to do nothing
    }
}
