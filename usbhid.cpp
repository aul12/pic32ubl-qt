#include "stdafx.h"
#include "ethernet.h"
#include "comport.h"
#include "hex.h"
#include "usbhid.h"
#include "bootloader.h"
#include "pic32ubldlg.h"


#define USB_BUFFER_SIZE 64

/****************************************************************************
 *  Open USB device
 *
 * \param  vid: Vendor Id
 * \param  pid: Product Id
 * \param
 * \return
 *****************************************************************************/
BOOL CUsbHid::OpenUSBDevice(UINT vid, UINT pid)
{
    hidDevice = hid_open(vid, pid, NULL);

    if(!hidDevice)
    {
        QString error;
        error.sprintf("USB Device with VID= %04X and PID= %04X is not found", vid, pid);
        QMessageBox::warning(NULL, "Error opening USB Device", error, QMessageBox::Ok);
    }
    else
    {
      hid_set_nonblocking(hidDevice, true);
    }

    return (hidDevice != NULL);
}


/****************************************************************************
 *  Write USB device
 *
 * \param  *buffer: Pointer to the buffer
 * \param  bufflen: Buffer length
 * \param
 * \return
 *****************************************************************************/
BOOL CUsbHid::WriteUSBDevice(CHAR *buffer, INT bufflen)
{
    int BytesWritten;

    if(hidDevice == NULL || buffer == NULL || bufflen == 0)
    {
        return false;
    }

    BytesWritten = hid_write(hidDevice, (const unsigned char *)buffer, bufflen);

#ifdef QT_DEBUG
    if (BytesWritten == -1)
    {
      std::cout << "WriteUSBDevice BytesWritten == -1" << std::endl;
    }
    else if (BytesWritten > 0)
    {
      std::cout << "WriteUSBDevice BytesWritten == " << BytesWritten << "  bufflen: " << bufflen << std::endl;
    }
#endif

    return (BytesWritten != -1);
}


/****************************************************************************
 *  Read USB device
 *
 * \param  *buffer: Pointer to the buffer
 * \param  bufflen: Maximum size of the read buffer. (Not used, since USB buffer is always 64bytes)
 * \param
 * \return
 *****************************************************************************/
USHORT CUsbHid::ReadUSBDevice(CHAR* buffer, INT bufflen)
{

    if(hidDevice == NULL)
    {
        return 0;
    }

//    int bytesRead = hid_read_timeout(hidDevice, (unsigned char *)buffer, bufflen, 1000);
    int bytesRead = hid_read(hidDevice, (unsigned char *)buffer, bufflen);

    if (bytesRead == -1)
    {
        bytesRead = 0;
#ifdef QT_DEBUG
        std::cout << "ReadUSBDevice bytesRead == -1" << std::endl;
#endif
    }
#ifdef QT_DEBUG
    else if (bytesRead > 0)
    {
        std::cout << "ReadUSBDevice bytesRead == " << bytesRead << std::endl;
    }
#endif
    return bytesRead;
}

/****************************************************************************
 *  Check if USB port is already opened.
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
BOOL CUsbHid::GetPortOpenStatus(void)
{
    return (hidDevice != NULL);
}



/****************************************************************************
 *  Close port
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
VOID CUsbHid::ClosePort()
{
    if(hidDevice != NULL)
    {
        hid_close(hidDevice);
        // Initialize device found to false.
        hidDevice = NULL;
    }
}


/***************************End ********************************************/
