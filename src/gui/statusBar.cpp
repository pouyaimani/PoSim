#include "statusBar.h"
#include "gui.h"
#include "utility.h"
#include "asset.h"

#define STATUS_BAR_HEIGHT 42

using namespace ui;

StatusBar::StatusBar()
{
    body.reset(new Widget(LVGL::instance().getDisplay().raw()));
    body->setSize(DISPLAY_WIDTH, STATUS_BAR_HEIGHT);
    body->setBgOpa(LV_OPA_TRANSP).setBorderOpa(LV_OPA_TRANSP).setScrollEnable(false);

    timeBox.reset(new TextBox(body->raw()));
    timeBox->setFont(&lv_font_montserrat_20);
    timeBox->align(LV_ALIGN_LEFT_MID);

    batteryIcon.reset(new Image(body->raw()));
    batteryIcon->setSrc(ASSET_IMG_BATTERY);
    batteryIcon->align(LV_ALIGN_RIGHT_MID);

    signalIcon.reset(new Image(body->raw()));
    signalIcon->setSrc(ASSET_IMG_SIGNAL);
    signalIcon->align(LV_ALIGN_RIGHT_MID, -70);

    wifiIcon.reset(new Image(body->raw()));
    wifiIcon->setSrc(ASSET_IMG_WIFI);
    wifiIcon->align(LV_ALIGN_RIGHT_MID, -40);

    logo.reset(new Image(body->raw()));
    logo->setSrc(ASSET_IMG_POS_LOGO_SMALL);
    logo->align(LV_ALIGN_CENTER, 0, -2);

    hide();
    timer.start(1000, [this]() {
        this->updateTime();
    });
}

void StatusBar::show()
{
    body->show();
}

void StatusBar::hide()
{
    body->hide();
}

void StatusBar::updateTime()
{
    timeBox->setText(getTimeOfDayLocal().substr(0, 5));
}