#include "usbinterface.h"
extern bool global_debug;

void printProgStr(const char* str);
void print_hex(int v, int num_places);

const char STR_DX[] PROGMEM ="dx=";
const char STR_DY[] PROGMEM =" dy=";
const char STR_LFT_BUTTON[] PROGMEM ="Left Button ";
const char STR_MID_BUTTON[] PROGMEM ="Middle Button ";
const char STR_RHT_BUTTON[] PROGMEM ="Right Button ";
const char STR_DOWN[] PROGMEM ="Down\n";
const char STR_UP[] PROGMEM ="Up\n";
const char STR_SPACE[] PROGMEM = " ";
const char STR_CR[] PROGMEM = "\n";
const char STR_MOUSE_MOVE[] PROGMEM = "OnMouseMove()";


void TabletReportParser::Parse(USBHID *hid __attribute__((unused)), bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf) {
        ABSOLUTEMOUSEINFO *abs_mi = (ABSOLUTEMOUSEINFO*)buf;

        // printProgStr(STR_LFT_BUTTON);
        // print_hex(abs_mi->bmLeftButton,1);
        // printProgStr(STR_SPACE);
        // printProgStr(STR_MID_BUTTON);
        // print_hex(abs_mi->bmMiddleButton,1);
        // printProgStr(STR_SPACE);
        // printProgStr(STR_RHT_BUTTON);
        // print_hex(abs_mi->bmRightButton,1);
        // printProgStr(STR_SPACE);
        // print_hex(buf[0],2);
        // print_hex(buf[1],2);
        // print_hex(buf[2],2);

        // Optimization idea:
        //
        // 1: Don't pass the structure on every event. Buttons would not need it.
        // 2: Only pass x/y values in the movement routine.
        //
        // These two changes (with the ones I have made) will save extra flash.
        // The only "bad" thing is that it could break old code.
        //
        // Future thoughts:
        //
        // The extra space gained can be used for a generic mouse event that can be called
        // when there are _ANY_ changes. This one you _MAY_ want to pass everything, however the
        // sketch could already have noted these facts to support drag/drop scroll wheel stuff, etc.
        //

        // Why do we need to pass the structure for buttons?
        // The function call not enough of a hint for what is happening?
        if(prevState.mouseInfo.bmLeftButton != abs_mi->bmLeftButton ) {
                if(abs_mi->bmLeftButton) {
                        OnLeftButtonDown(abs_mi);
                } else {
                        OnLeftButtonUp(abs_mi);
                }
                // Future:
                // event = true;
        }

        if(prevState.mouseInfo.bmRightButton != abs_mi->bmRightButton) {
                if(abs_mi->bmRightButton) {
                        OnRightButtonDown(abs_mi);
                } else {
                        OnRightButtonUp(abs_mi);
                }
                // Future:
                // event = true;
        }

        if(prevState.mouseInfo.bmMiddleButton != abs_mi->bmMiddleButton) {
                if(abs_mi->bmMiddleButton) {
                        OnMiddleButtonDown(abs_mi);
                } else {
                        OnMiddleButtonUp(abs_mi);
                }
                // Future:
                // event = true;
        }

        //
        // Scroll wheel(s), are not part of the spec, but we could support it.
        // Logitech wireless keyboard and mouse combo reports scroll wheel in byte 4
        // We wouldn't even need to save this information.
        //if(len > 3) {
        //}
        //

        // Mice only report motion when they actually move!
        // Why not just pass the x/y values to simplify things??
        if((prevState.mouseInfo.dX != abs_mi->dX) ||
         (prevState.mouseInfo.dY != abs_mi->dY)){
            OnMouseMove(abs_mi);
                // Future:
                // event = true;
        }

        //
        // Future:
        // Provide a callback that operates on the gathered events from above.
        //
        // if(event) OnMouse();
        //

        // Only the first byte matters (buttons). We do NOT need to save position info.
        memcpy(prevState.bInfo, buf, sizeof(ABSOLUTEMOUSEINFO));
        // prevState.bInfo[0] = buf[0];
};












































bool TabletRptParser::MouseChanged()
{
    return (m_mouse_position_changed || m_mouse_button_changed);
}
int32_t TabletRptParser::GetX()
{
    return m_pos_x;
}
int32_t TabletRptParser::GetY()
{
    return m_pos_y;
}
void TabletRptParser::ResetMouseMovement()
{
    m_mouse_button_changed = false;
    m_mouse_position_changed = false;
}
bool TabletRptParser::MouseButtonIsPressed()
{
    return m_mouse_button_is_pressed;
}

uint32_t TabletRptParser::EightBitToSevenBitSigned(int8_t value)
{
    // Dividing the value by 2 has 2 benefits:
    //   - Easy way to convert from 8 bit to 7 bit number
    //   - It "softens" the mouse movement. Modern optical
    //     mice seem a little jumpy.
    return (uint32_t)((value/2) & 0x7F);
}

uint16_t TabletRptParser::GetAdbRegister0()
{
    uint16_t reg_value = 0;
    // Bit 15 = Button Status; 0=down
    if (!MouseButtonIsPressed())
    {
        reg_value |= (1 << 15);
    }
    // Bit 7 = Not used. Always 1
    reg_value |= (1 << 7);
    // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= (EightBitToSevenBitSigned(GetX()) << 8);

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= (EightBitToSevenBitSigned(GetY()) << 0);

    ResetMouseMovement();

    return reg_value;
}

void TabletRptParser::OnMouseMove(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_MOUSE_MOVE);
        printProgStr(STR_DX);
        Serial.print(mi->dX, DEC);
        printProgStr(STR_DY);
        Serial.println(mi->dY, DEC);
    }
    m_pos_x = mi->dX;
    m_pos_y = mi->dY;
};
void TabletRptParser::OnLeftButtonUp(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_LFT_BUTTON);
        printProgStr(STR_UP);
    }
    m_mouse_button_is_pressed = false;
    m_mouse_button_changed = true;
};
void TabletRptParser::OnLeftButtonDown(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_LFT_BUTTON);
        printProgStr(STR_DOWN);
    }
    m_mouse_button_is_pressed = true;
    m_mouse_button_changed = true;
};
void TabletRptParser::OnRightButtonUp(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_RHT_BUTTON);
        printProgStr(STR_UP);
    }
};
void TabletRptParser::OnRightButtonDown(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_RHT_BUTTON);
        printProgStr(STR_DOWN);
    }
};
void TabletRptParser::OnMiddleButtonUp(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_MID_BUTTON);
        printProgStr(STR_UP);
    }
};
void TabletRptParser::OnMiddleButtonDown(ABSOLUTEMOUSEINFO *mi)
{
    if (global_debug)
    {
        printProgStr(STR_MID_BUTTON);
        printProgStr(STR_DOWN);
    }
};
