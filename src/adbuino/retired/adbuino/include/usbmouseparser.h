#include <hidboot.h>

class MouseRptParser : public MouseReportParser
{
public:
    bool MouseChanged();
    int32_t GetDeltaX();
    int32_t GetDeltaY();
    void ResetMouseMovement();
    uint16_t GetAdbRegister0();
    bool MouseButtonIsPressed();

protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    uint32_t EightBitToSevenBitSigned(int8_t value);


private:
    int8_t m_movedx = 0;
    int8_t m_movedy = 0;
    bool m_mouse_button_is_pressed = false;
    bool m_mouse_button_changed = false;
};
