#include "hidcomposite.h"



// Override HIDComposite to be able to select which interface we want to hook into
class FlexibleUsbHidDevice : public HIDComposite
{
public:
    FlexibleUsbHidDevice(USB *p) : HIDComposite(p) {};

    HIDReportParser* KeyboardParser = nullptr;
    HIDReportParser* MouseParser = nullptr;
    HIDReportParser* TabletParser = nullptr;

protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);

    void MyParseMouseData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    void MyParseKeyboardData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
};