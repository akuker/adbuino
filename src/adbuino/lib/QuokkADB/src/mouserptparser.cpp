#include "hidinputclasses.h"
#include "tusb.h"
// @TODO add mutexes around mouse info
void MouseReportParser::Parse(const hid_mouse_report_t *report){
    static MOUSEINFO mouse_info;

    mouse_info.bmLeftButton = !!(report->buttons & MOUSE_BUTTON_LEFT);
    mouse_info.bmRightButton = !!(report->buttons & MOUSE_BUTTON_RIGHT);
    mouse_info.dX = report->x;
    mouse_info.dY = report->y;

    if(mouse_info.dX != 0 || mouse_info.dY != 0) {
        OnMouseMove(&mouse_info);
    }

    // change to mouse left button down
    if (!prevState.mouseInfo.bmLeftButton && mouse_info.bmLeftButton) {
        OnLeftButtonDown(&mouse_info);
    }

    // change to mouse left button up
    if (prevState.mouseInfo.bmLeftButton && !mouse_info.bmLeftButton) {
        OnLeftButtonUp(&mouse_info);
    }

    if (!prevState.mouseInfo.bmRightButton && mouse_info.bmRightButton) {
        OnRightButtonDown(&mouse_info);
    }

    if (prevState.mouseInfo.bmRightButton && !mouse_info.bmRightButton) {
        OnRightButtonUp(&mouse_info);
    }

    prevState.mouseInfo = mouse_info;
}