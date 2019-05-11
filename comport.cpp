#include "stdafx.h"
#include "comport.h"
#include "ethernet.h"
#include "usbhid.h"
#include "hex.h"
#include "bootloader.h"
#include "pic32ubldlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





/****************************************************************************
 * Opens com port
 *
 * \param comPort  Com port to be opened.
 * \param baud     Baud rate
 * \param
 * \return         Opens the com port
 *****************************************************************************/
void CComPort::OpenComPort(QString &comPort, unsigned int baud)
{
    CloseComPort();

    serialPort = new SerialPort(comPort.toStdString());

    SerialPort::BaudRate baudRate = SerialPort::BAUD_DEFAULT;
    switch (baud)
    {
      case (50):
        baudRate = SerialPort::BAUD_50;
        break;
      case (75):
        baudRate = SerialPort::BAUD_75;
        break;
      case (110):
        baudRate = SerialPort::BAUD_110;
        break;
      case (134):
        baudRate = SerialPort::BAUD_134;
        break;
      case (150):
        baudRate = SerialPort::BAUD_150;
        break;
      case (200):
        baudRate = SerialPort::BAUD_200;
        break;
      case (300):
        baudRate = SerialPort::BAUD_300;
        break;
      case (600):
        baudRate = SerialPort::BAUD_600;
        break;
      case (1200):
        baudRate = SerialPort::BAUD_1200;
        break;
      case (1800):
        baudRate = SerialPort::BAUD_1800;
        break;
      case (2400):
        baudRate = SerialPort::BAUD_2400;
        break;
      case (4800):
        baudRate = SerialPort::BAUD_4800;
        break;
      case (9600):
        baudRate = SerialPort::BAUD_9600;
        break;
      case (19200):
        baudRate = SerialPort::BAUD_19200;
        break;
      case (38400):
        baudRate = SerialPort::BAUD_38400;
        break;
      case (57600):
        baudRate = SerialPort::BAUD_57600;
        break;
      case (115200):
        baudRate = SerialPort::BAUD_115200;
        break;
      case (230400):
        baudRate = SerialPort::BAUD_230400;
        break;
    }

    try
    {
      serialPort->Open(baudRate, SerialPort::CHAR_SIZE_8, SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1, SerialPort::FLOW_CONTROL_NONE);
      if (!serialPort->IsOpen())
      {
        QString message = "Couldn't open ";
        message.append(comPort);
        QMessageBox::warning(NULL, "Socket Error", message, QMessageBox::Ok);
        CloseComPort();
      }
    }
    catch (SerialPort::OpenFailed of)
    {
      serialPort = NULL;
    }
}


/****************************************************************************
 * Close com port
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void CComPort::CloseComPort(void)
{
    if (serialPort != NULL)
    {
      serialPort->Close();
      delete serialPort;
      serialPort = NULL;
    }
}


/****************************************************************************
 * Send com port
 *
 * \param  buffer: Data buffer
 * \param  bufflen: Buffer Length
 * \param
 * \return
 *****************************************************************************/
void CComPort::SendComPort(char *buffer, int bufflen)
{
  if (serialPort != NULL)
  {
    std::string data(buffer, bufflen);
    serialPort->Write(data);
  }
}

/****************************************************************************
 * Reads com port
 *
 * \param  buffer: Data buffer
 * \param  MaxLen: Max possible length
 * \param
 * \return  Number of bytes read.
 *****************************************************************************/
unsigned short CComPort::ReadComPort(char* buffer, int MaxLen)
{
  if (serialPort != NULL)
  {
    SerialPort::DataBuffer dataBuffer;
    serialPort->Read(dataBuffer, MaxLen, 10);

    std::copy(dataBuffer.begin(), dataBuffer.end(), buffer);

    return dataBuffer.size();
  }
  return 0;
}

/****************************************************************************
 * Gets com port status
 *
 * \param
 * \param
 * \param
 * \return  true if com port is up and running
 *****************************************************************************/
bool CComPort::GetComPortOpenStatus(void)
{
  if (serialPort != NULL)
  {
    return serialPort->IsOpen();
  }
  return false;
}

/************End of file******************************************************/

