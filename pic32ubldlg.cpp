#include "stdafx.h"
#include "comport.h"
#include "ethernet.h"
#include "usbhid.h"
#include "hex.h"
#include "bootloader.h"
#include "pic32ubldlg.h"
#include "ui_pic32ubldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BOOTLOADER_ROUTINE_TIMER 1
#define BOOTLOADER_GUI_UPDATE_TIMER 2
#define SWAP(x)   ( ((x<<24)&(0xFF000000)) | ((x<<8)&(0x00FF0000))  | ((x>>24)&(0x000000FF)) | ((x>>8)&(0x0000FF00)) )

#define SAVE 1
#define RESTORE 2

#define SaveButtonStatus() ButtonStatus(SAVE)
#define RestoreButtonStatus() ButtonStatus(RESTORE)

Q_DECLARE_METATYPE(unsigned char)
Q_DECLARE_METATYPE(unsigned char *)

CPIC32UBLDlg::CPIC32UBLDlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CPIC32UBLDlg)
{
    ui->setupUi(this);

    qRegisterMetaType<unsigned char>("unsigned char");
    qRegisterMetaType<unsigned char *>("unsigned char *");

    ExtraInitialization();
}

CPIC32UBLDlg::~CPIC32UBLDlg()
{
    delete ui;
}

void CPIC32UBLDlg::on_ctrlButtonBootloaderVer_clicked()
{
    OnBnClickedButtonBootVer();
}

void CPIC32UBLDlg::on_ctrlButtonLoadHex_clicked()
{
    OnBnClickedButtonLoadhex();
}

void CPIC32UBLDlg::on_ctrlButtonErase_clicked()
{
    OnBnClickedButtonErase();
}

void CPIC32UBLDlg::on_ctrlButtonProgram_clicked()
{
    OnBnClickedButtonProgram();
}

void CPIC32UBLDlg::on_ctrlButtonVerify_clicked()
{
    OnBnClickedButtonVerify();
}

void CPIC32UBLDlg::on_ctrlButtonRunApplication_clicked()
{
    OnBnClickedButtonRunapplication();
}

void CPIC32UBLDlg::on_ctrlButtonEraseProgVerify_clicked()
{
    OnBnClickedButtonEraseProgVerify();
}

void CPIC32UBLDlg::on_ctrlButtonConnectDevice_clicked()
{
    OnBnClickedButtonConnect();
}

void CPIC32UBLDlg::on_ctrlCheckBoxComEnable_clicked()
{
    OnBnClickedCheckComEnable();
}

void CPIC32UBLDlg::on_ctrlCheckBoxUSBEnable_clicked()
{
    OnBnClickedCheckUsbEnable();
}

void CPIC32UBLDlg::on_ctrlCheckBoxEthEnable_clicked()
{
    OnBnClickedCheckEthEnable();
}

/****************************************************************************
 * Invoked when connect button is clicked
 *
 *
 *****************************************************************************/

void CPIC32UBLDlg::OnBnClickedButtonConnect()
{
    QString comport;
    QString baudString;
    INT baud;
    QString string;
    QString vidpidString;
    UINT vid;
    UINT pid;
    ULONG ip;
    USHORT skt;
//    UpdateData(true);

    if(ConnectionEstablished)
    {
        // Already connected. Disconnect now.
        ConnectionEstablished = false;

        // Shut down the rx-tx thread, if already opened.
            mBootLoader.ShutdownThread();

        if(mBootLoader.GetPortOpenStatus(PortSelected))
        {
            // com port already opened. close com port
            mBootLoader.ClosePort(PortSelected);
        }



        // Print console.
        PrintKonsole("Device disconnected");

        EnableAllButtons(false);


        ui->ctrlButtonConnectDevice->setText("Connect");
        ui->ctrlButtonConnectDevice->setEnabled(true);
    }
    else
    {
        // Establish new connection.
        comport = ui->ctrlComboBoxComPort->currentText();
        baudString = ui->ctrlComboBoxBaudRate->currentText();
        sscanf(baudString.toStdString().c_str(), "%d", &baud);
        vidpidString = ui->ctrlComboBoxVIDPID->currentText();
        sscanf(vidpidString.toStdString().c_str(), "%4X:%4X", &vid, &pid);
        // Read ip address;
        ip = SWAP(ip_value);
        skt = (USHORT)valEditBoxSocket;
//        UpdateData(false);

        if(mBootLoader.GetPortOpenStatus(PortSelected))
        {
            // com port already opened. close com port
            mBootLoader.ClosePort(PortSelected);
        }
        // Open Communication port freshly.
        mBootLoader.OpenPort(PortSelected, comport, baud, vid, pid, skt, ip);


        if(mBootLoader.GetPortOpenStatus(PortSelected))
        {	// COM port opened.
            // Shut down the rx-tx thread, if already opened.
            mBootLoader.ShutdownThread();
            // Create a new thread to do Rx Tx in the back ground.
            mBootLoader.CreateRxTxThread(this); // Create Rx Tx thread.
            // Trigger Read boot info command
            mBootLoader.SendCommand(READ_BOOT_INFO, 30, 200);

            // Start timer to handle GUI updates.
            updateTimer.setInterval(300);
            updateTimer.start();
            connect(&updateTimer,SIGNAL(timeout()),SLOT(OnTimer()));

            // Print a message to user/
            PrintKonsole("Please reset device and invoke bootloader");
            // Save button status.
            SaveButtonStatus();
            // Disable all buttons to avoid further operations
            EnableAllButtons(false);
        }


    }
}

/****************************************************************************
 * Invoked just after dialog is created. Do some extra initialization
 * like enabling and disabling buttons and so on.
 *
 *****************************************************************************/
void CPIC32UBLDlg::ExtraInitialization()
{
    char com[9][15] = { "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", "/dev/ttyS4", "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3"};
    char baud[5][10] = {"9600", "19200", "38400", "57600", "115200"};

    EraseProgVer = false;

    // Fill Com Port Combo Box with "COM1, COM2 ........, COM9"
    for(int i = 0; i < 9; i++)
    {
        ui->ctrlComboBoxComPort->addItem(QString(com[i]));
    }

    // By default set "COM1" in com port combo box
    ui->ctrlComboBoxComPort->setCurrentIndex(0);

    // Fill Baud Rate Combo Box with "9600, 19200, 38400, 57600, 115200"
    for(int i = 0; i < 5; i++)
    {
        ui->ctrlComboBoxBaudRate->addItem(QString(baud[i]));
    }
    // By default set "115200" in baud rate combo box
    ui->ctrlComboBoxBaudRate->setCurrentIndex(4);


    // Mask all buttons, except for "Connect Device"
    EnableAllButtons(false);
    ui->ctrlButtonConnectDevice->setEnabled(true);

    // Mark this flag as false.
    ConnectionEstablished = false;

    // Select communication "Com Port" by default.
    ui->ctrlCheckBoxComEnable->setChecked(true);
    PortSelected = COM;
    // Diasble serial port selection and baud rate selection.
    ui->ctrlComboBoxBaudRate->setEnabled(true);
    ui->ctrlComboBoxComPort->setEnabled(true);
    // Enable USB VID and PID edit boxes.
    ui->ctrlComboBoxVIDPID->setEnabled(false);
    // Enable IP Address and Socket
    ui->ctrlEditBoxIPaddress->setEnabled(false);
    ui->ctrlEditBoxSocketaddress->setEnabled(false);

    // Set default IP address to 192.168.1.11
    ip_value = inet_addr("192.168.1.11");
    ip_value = SWAP(ip_value);
    valEditBoxSocket = 6234;

    ui->ctrlProgressBar->setRange(0, 100);
    ui->ctrlProgressBar->setValue(0);

    struct hid_device_info *deviceInfosLinkedList = hid_enumerate(0, 0);
    struct hid_device_info *current = deviceInfosLinkedList;
    std::set<QString> addedStrings;
    if (deviceInfosLinkedList)
    {
      while (current)
      {
          QString vidpid;
          vidpid.sprintf("%04X:%04X", current->vendor_id, current->product_id);
          if (addedStrings.count(vidpid) == 0)
          {
            ui->ctrlComboBoxVIDPID->addItem(vidpid);
            addedStrings.insert(vidpid);
          }
          current = current->next;
      }
      hid_free_enumeration(deviceInfosLinkedList);
    }
//    UpdateData(false);

}

/****************************************************************************
 * Just do some mundane things here, like getting progress bar status,
    disabling buttons if connection cuts.
   This function is called everytime the timer elapses.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnTimer()
{
    int Lower;
    int Upper;

    // Update progress bar.
    mBootLoader.GetProgress(&Lower, &Upper);
    ui->ctrlProgressBar->setRange(0, Upper);
    ui->ctrlProgressBar->setValue(Lower);
}

/****************************************************************************
 * Invoked when button erase is clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonErase()
{
    SaveButtonStatus();
    // Disable all buttons, to avoid further operation.
    EnableAllButtons(false);
    mBootLoader.SendCommand(ERASE_FLASH, 3, 5000); //5s retry delay, becuse erase takes considerable time.
}


/****************************************************************************
 * Enables all button, if the parameter passed is true.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::EnableAllButtons(bool enbl)
{
    // Mask all buttons, except for "Connect Device"
    ui->ctrlButtonProgram->setEnabled(enbl);
    ui->ctrlButtonLoadHex->setEnabled(enbl);
    ui->ctrlButtonErase->setEnabled(enbl);
    ui->ctrlButtonVerify->setEnabled(enbl);
    ui->ctrlButtonRunApplication->setEnabled(enbl);
//	ui->ctrlButtonConnectDevice->setEnabled(enbl);
    ui->ctrlButtonEraseProgVerify->setEnabled(enbl);
    ui->ctrlButtonBootloaderVer->setEnabled(enbl);

}


/****************************************************************************
 * Invoked when load hex button is clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonLoadhex()
{
    // Save button status.
    SaveButtonStatus();
    if(mBootLoader.LoadHexFile())
    {
        PrintKonsole("Hex file loaded successfully");
        // Enable Program button
        ui->ctrlButtonProgram->setEnabled(true);
        ui->ctrlButtonEraseProgVerify->setEnabled(true);
    }
    else
    {
        PrintKonsole("Hex file loading failed");
    }
}


/****************************************************************************
 * This function is invoked when there is a response from the device.
 Process the response based on the command.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnReceiveResponse(unsigned char cmd, unsigned char *RxDataPtrAdrs)
{
    char MajorVer;// = RxData[3];
    char MinorVer ;//= RxData[4];
    char *RxData;
    QString string;
    unsigned short crc;

    RxData = (char *)RxDataPtrAdrs;
    MajorVer = RxData[0];
    MinorVer = RxData[1];

    switch(cmd)
    {
    case READ_BOOT_INFO:
        if(ConnectionEstablished == false)
        {
            // New connection.
            ClearKonsole();
            string = "Device connected";
            PrintKonsole(string);
        }
        string.sprintf("Bootloader Firmware Version: %d.%d", MajorVer, MinorVer);
        PrintKonsole(string);
        // Enable only load hex, Disconnect and erase buttons for next operation.
        ui->ctrlButtonLoadHex->setEnabled(true);
        ui->ctrlButtonErase->setEnabled(true);
        // Change the connect button to disconnect.
        ConnectionEstablished = true;
        ui->ctrlButtonConnectDevice->setText("Disconnect");
        // Disable baud rate and com port combo boxes.
        ui->ctrlComboBoxBaudRate->setEnabled(false);
        ui->ctrlComboBoxComPort->setEnabled(false);
        // Disable USB VID and PID boxes.
        ui->ctrlComboBoxVIDPID->setEnabled(false);

        // Also enable bootloader version info.
        ui->ctrlButtonBootloaderVer->setEnabled(true);
        break;

    case ERASE_FLASH:
        PrintKonsole("Flash Erased");
        if(EraseProgVer)// Operation Erase->Program->Verify
        {
            // Erase completed. Next operation is programming.
            mBootLoader.SendCommand(PROGRAM_FLASH, 3, 500); // 500ms delay
        }
        // Restore button status to allow further operations.
        RestoreButtonStatus();
        break;

    case PROGRAM_FLASH:
        PrintKonsole("Programming completed");
        // Restore button status to allow further operations.
        RestoreButtonStatus();
        ui->ctrlButtonVerify->setEnabled(true);
        ui->ctrlButtonRunApplication->setEnabled(true);

        if(EraseProgVer)// Operation Erase->Program->Verify
        {
            // Programming completed. Next operation is verification.
            mBootLoader.SendCommand(READ_CRC, 3, 5000);// 5 second delay
        }
        break;

    case READ_CRC:
        crc = ((RxData[1] << 8) & 0xFF00) | (RxData[0] & 0x00FF);

        if(crc == mBootLoader.CalculateFlashCRC())
        {
            PrintKonsole("Verification successfull");
        }
        else
        {
            PrintKonsole("Verification failed");
        }
        // Reset erase->program-verify operation.
        EraseProgVer = false;
        // Restore button status to allow further operations.
        RestoreButtonStatus();
        ui->ctrlButtonVerify->setEnabled(true);
        ui->ctrlButtonRunApplication->setEnabled(true);
        break;
    }

    if(!ConnectionEstablished)
    {
        // Disable all buttons, if disconnected.
        EnableAllButtons(false);
    }
}


/****************************************************************************
 *  This function is invoked when there is no resonse from the device, even after
    retries.
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnTransmitFailure(unsigned char cmd)
{
    EraseProgVer = false;
    switch(cmd)
    {
    case READ_BOOT_INFO:
    case ERASE_FLASH:
    case PROGRAM_FLASH:
    case READ_CRC:
        PrintKonsole("No Response from the device. Operation failed");
        RestoreButtonStatus();
        break;
    }

    if(!ConnectionEstablished)
    {
        // Disable all buttons, if disconnected.
        EnableAllButtons(false);
    }
}


/****************************************************************************
 * Invoked when the button program is clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonProgram()
{
    // Save button status.
    SaveButtonStatus();
    // Disable all buttons to avoid further operations
    EnableAllButtons(false);
    mBootLoader.SendCommand(PROGRAM_FLASH, 3, 5000); // 500ms delay

}


/****************************************************************************
 * This function is invoked when button verify is clicked
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonVerify()
{
    // Save button status.
    SaveButtonStatus();
    // Disable all buttons to avoid further operations
    EnableAllButtons(false);
    mBootLoader.SendCommand(READ_CRC, 3, 5000);
}


/****************************************************************************
 * This function is invoked when button Erase-Program-Verify is clicked
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonEraseProgVerify()
{
    // TODO: Add your control notification handler code here
    // Save button status.
    SaveButtonStatus();
    // Disable all buttons to avoid further operations
    EnableAllButtons(false);

    EraseProgVer = true;
    // Start with erase. Rest is automatically handled by state machine.
    mBootLoader.SendCommand(ERASE_FLASH, 3, 5000); // 5s delay

}


/****************************************************************************
 * This function is invoked when button run application is clicked
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonRunapplication()
{
    mBootLoader.SendCommand(JMP_TO_APP, 1, 10); // 10ms delay
    PrintKonsole("\nCommand issued to run application");
}

/****************************************************************************
 * This function is invoked when button Read Version is clicked
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedButtonBootVer()
{
    // TODO: Add your control notification handler code here
    // Save button status.
    SaveButtonStatus();
    // Disable all buttons to avoid further operations
    EnableAllButtons(false);
    mBootLoader.SendCommand(READ_BOOT_INFO, 50, 200);
}


/****************************************************************************
 * Print Konsole
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::PrintKonsole(QString string)
{
//    UpdateData(false);
    // Set the console window vertical scroll to the end.
    ui->ctrlEditConsole->setPlainText(ui->ctrlEditConsole->toPlainText().append(string).append("\n"));
}

/****************************************************************************
 * Clears konsole
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::ClearKonsole(void)
{
//    UpdateData(false);
    // Set the console window vertical scroll to the end.
    ui->ctrlEditConsole->setPlainText("");
}

/****************************************************************************
 * Saves the current status of the button and restores the button
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::ButtonStatus(unsigned int oprn)
{
    static unsigned int status;


    if(oprn == SAVE)
    {
        (ui->ctrlButtonProgram->isEnabled())? status |= 0x01: status &= ~0x01;
        (ui->ctrlButtonLoadHex->isEnabled())? status |= 0x02: status &= ~0x02;
        (ui->ctrlButtonErase->isEnabled())? status |= 0x04: status &= ~0x04;
        (ui->ctrlButtonVerify->isEnabled())? status |= 0x08: status &= ~0x08;
        (ui->ctrlButtonRunApplication->isEnabled())? status |= 0x10: status &= ~0x10;
        (ui->ctrlButtonConnectDevice->isEnabled())? status |= 0x20: status &= ~0x20;
        (ui->ctrlButtonEraseProgVerify->isEnabled())? status |= 0x40: status &= ~0x40;
        (ui->ctrlButtonBootloaderVer->isEnabled())? status |= 0x80: status &= ~0x80;
    }
    else
    {
        // Restore
        (status & 0x01)? ui->ctrlButtonProgram->setEnabled(true): ui->ctrlButtonProgram->setEnabled(false);
        (status & 0x02)? ui->ctrlButtonLoadHex->setEnabled(true): ui->ctrlButtonLoadHex->setEnabled(false);
        (status & 0x04)? ui->ctrlButtonErase->setEnabled(true): ui->ctrlButtonErase->setEnabled(false);
        (status & 0x08)? ui->ctrlButtonVerify->setEnabled(true): ui->ctrlButtonVerify->setEnabled(false);
        (status & 0x10)? ui->ctrlButtonRunApplication->setEnabled(true): ui->ctrlButtonRunApplication->setEnabled(false);
        (status & 0x20)? ui->ctrlButtonConnectDevice->setEnabled(true): ui->ctrlButtonConnectDevice->setEnabled(false);
        (status & 0x40)? ui->ctrlButtonEraseProgVerify->setEnabled(true): ui->ctrlButtonEraseProgVerify->setEnabled(false);
        (status & 0x80)? ui->ctrlButtonBootloaderVer->setEnabled(true): ui->ctrlButtonBootloaderVer->setEnabled(false);
    }


}


/****************************************************************************
 * COM check box button clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedCheckComEnable()
{
    // TODO: Add your control notification handler code here
//    UpdateData(true);
    ui->ctrlCheckBoxComEnable->setChecked(true);
    ui->ctrlCheckBoxUSBEnable->setChecked(false);
    ui->ctrlCheckBoxEthEnable->setChecked(false);
    PortSelected = COM;

    // Enable serial port selection and baud rate selection.
    ui->ctrlComboBoxBaudRate->setEnabled(true);
    ui->ctrlComboBoxComPort->setEnabled(true);
    // Disable USB VID and PID edit boxes.
    ui->ctrlComboBoxVIDPID->setEnabled(false);
    // Disable IP Address and Socket
    ui->ctrlEditBoxIPaddress->setEnabled(false);
    ui->ctrlEditBoxSocketaddress->setEnabled(false);


//    UpdateData(false);
}


/****************************************************************************
 * USB check box button clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedCheckUsbEnable()
{
    // TODO: Add your control notification handler code here
//    UpdateData(true);
    ui->ctrlCheckBoxUSBEnable->setChecked(true);
    ui->ctrlCheckBoxComEnable->setChecked(false);
    ui->ctrlCheckBoxEthEnable->setChecked(false);
    PortSelected = USB;

    // Diasble serial port selection and baud rate selection.
    ui->ctrlComboBoxBaudRate->setEnabled(false);
    ui->ctrlComboBoxComPort->setEnabled(false);
    // Enable USB VID and PID edit boxes.
    ui->ctrlComboBoxVIDPID->setEnabled(true);
    // Enable IP Address and Socket
    ui->ctrlEditBoxIPaddress->setEnabled(false);
    ui->ctrlEditBoxSocketaddress->setEnabled(false);


//    UpdateData(false);
}


/****************************************************************************
 * Ethernet check box button clicked.
 *
 *
 *****************************************************************************/
void CPIC32UBLDlg::OnBnClickedCheckEthEnable()
{
    // TODO: Add your control notification handler code here
//    UpdateData(true);
    ui->ctrlCheckBoxEthEnable->setChecked(true);
    ui->ctrlCheckBoxComEnable->setChecked(false);
    ui->ctrlCheckBoxUSBEnable->setChecked(false);
    PortSelected = ETH;

    // Diasble serial port selection and baud rate selection.
    ui->ctrlComboBoxBaudRate->setEnabled(false);
    ui->ctrlComboBoxComPort->setEnabled(false);
    // Enable USB VID and PID edit boxes.
    ui->ctrlComboBoxVIDPID->setEnabled(false);
    // Enable IP Address and Socket
    ui->ctrlEditBoxIPaddress->setEnabled(true);
    ui->ctrlEditBoxSocketaddress->setEnabled(true);

//    UpdateData(false);
}


/********************************End of file**********************************/

