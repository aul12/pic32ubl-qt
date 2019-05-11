#ifndef USBHID_H
#define USBHID_H

#include "hidapi.h"

class CUsbHid
{
private:

    hid_device *hidDevice;

public:

    BOOL OpenUSBDevice(UINT vid, UINT pid);
    BOOL WriteUSBDevice(CHAR *buffer, INT bufflen);
    USHORT ReadUSBDevice(CHAR* buffer, INT bufflen);
    BOOL GetPortOpenStatus(VOID);
    VOID ClosePort();

    // Constructor
    CUsbHid(void)
    {
        hidDevice = NULL;
    }

    // Destructor
    ~CUsbHid(void)
    {
        ClosePort();
    }
};


#endif // USBHID_H
