#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "widget.h"
#include "core/timer/timer.h"

using namespace std;
using namespace ui;

class StatusBar {
public:
    StatusBar();
    void show();
    void hide();

private:
    unique_ptr<Widget> body;
    unique_ptr<TextBox> timeBox;
    unique_ptr<Image> batteryIcon;
    unique_ptr<Image> signalIcon;
    unique_ptr<Image> wifiIcon;
    unique_ptr<Image> logo;
    Timer timer;
    void updateTime();

};

#endif