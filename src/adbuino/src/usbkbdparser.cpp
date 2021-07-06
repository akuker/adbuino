#include "usbinterface.h"

uint8_t KbdRptParser::GetLastKey()
{
    return m_last_key_pressed;
}
uint8_t KbdRptParser::GetLastUpOrDown()
{
    return m_last_key_up_or_down;
}
void KbdRptParser::ClearLastKey()
{
    m_last_key_pressed = NoKey;
}



uint16_t KbdRptParser::GetAdbRegister0(){
    uint16_t kbdreg0 = 0;
    // kbdreg0 |= (usb_keycode_to_adb_code(m_previous_key_pressed) << 8);
    // kbdreg0 |= (KeyReleasedFlag << 8);
    if(GetLastUpOrDown() == KeyUp){
        kbdreg0 |= KeyReleasedFlag;
    }
    kbdreg0 |= usb_keycode_to_adb_code(GetLastKey());
    
    m_previous_key_pressed = GetLastKey();

    return kbdreg0;
}


void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
    MODIFIERKEYS mod;
    *((uint8_t *)&mod) = m;
    Serial.print((mod.bmLeftCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI == 1) ? "G" : " ");

    Serial.print(" >");
    PrintHex<uint8_t>(key, 0x80);
    Serial.print("< ");

    Serial.print((mod.bmRightCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    Serial.print("DN ");
    PrintKey(mod, key);
    uint8_t c = OemToAscii(mod, key);
    m_last_key_pressed = key;
    m_last_key_up_or_down = KeyDown;

    if (c)
        OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

    MODIFIERKEYS beforeMod;
    *((uint8_t *)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t *)&afterMod) = after;

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
    {
        Serial.println("LeftCtrl changed");
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift)
    {
        Serial.println("LeftShift changed");
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt)
    {
        Serial.println("LeftAlt changed");
    }
    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI)
    {
        Serial.println("LeftGUI changed");
    }

    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl)
    {
        Serial.println("RightCtrl changed");
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift)
    {
        Serial.println("RightShift changed");
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt)
    {
        Serial.println("RightAlt changed");
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI)
    {
        Serial.println("RightGUI changed");
    }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
    Serial.print("UP ");
    PrintKey(mod, key);
    m_last_key_pressed = key;
    m_last_key_up_or_down = KeyUp;
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
    Serial.print("ASCII: ");
    Serial.println((char)key);
};
