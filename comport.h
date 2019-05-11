#ifndef COMPORT_H
#define COMPORT_H

#include <SerialPort.h>
#include <SerialStream.h>
#include <SerialStreamBuf.h>

using namespace LibSerial;
class CComPort
{
private:
    SerialPort *serialPort;

public:
// Constructor
    CComPort(void)
    {
        serialPort = NULL;
    }


// Destructor
    ~CComPort(void)
    {
        CloseComPort();
    }


    void OpenComPort(QString&, unsigned int);
    void CloseComPort(void);
    void SendComPort(char*, int );
    bool GetComPortOpenStatus(void);
    unsigned short ReadComPort(char*, int);


};


#endif // COMPORT_H
