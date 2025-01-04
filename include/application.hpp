#pragma once

#include <widget.hpp>

namespace UI {
class Application: public Widget
{
    public:
        Application(Application * parent = nullptr);

        static Rect getFullFrameRect() {return Rect(0,0,TFT_HEIGHT,TFT_WIDTH).toLogical(); }

        virtual void update();
};
}
