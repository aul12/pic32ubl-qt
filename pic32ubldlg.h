#ifndef PIC32UBLDLG_H
#define PIC32UBLDLG_H

#include <QMainWindow>

#include "bootloader.h"

namespace Ui {
class CPIC32UBLDlg;
}

class CPIC32UBLDlg : public QMainWindow
{
    Q_OBJECT

protected:
    CBootLoader mBootLoader;
    bool EraseProgVer;
    bool ConnectionEstablished;

    UINT PortSelected;

public:
    explicit CPIC32UBLDlg(QWidget *parent = 0);
    ~CPIC32UBLDlg();

    void OnBnClickedCancel();
    void OnBnClickedButtonConnect();

    void ExtraInitialization(void);
    void StartSendReceiveTimers(void);
    void ProgressBarTask(void);
    void EnableAllButtons(bool enbl);
    void ProgramHexFile(void);
    void ButtonStatus(unsigned int oprn);

    char TxCommand;
    char TxData[255];

    void OnTimer(UINT nIDEvent);
    void OnBnClickedButtonErase();
    void OnBnClickedButtonLoadhex();
    void OnBnClickedButtonProgram();
    void OnBnClickedButtonDisconnect();
    void OnBnClickedButtonVerify();
    void OnBnClickedButtonEraseProgVerify();
    void OnBnClickedButtonRunapplication();
    void OnBnClickedButtonBootVer();

    void OnBnClickedRadioEnableCom();
    void OnBnClickedCheckComEnable();
    void OnBnClickedCheckUsbEnable();
    void OnBnClickedCheckEthEnable();
    BOOL OnDeviceChange(UINT wParam, DWORD lParam);
    void OnDevModeChange(LPTSTR lpDeviceName);

    void PrintKonsole(QString string);
    void ClearKonsole(void);

    unsigned long uLongEditBoxVID;
    unsigned long uLongEditBoxPID;
    DWORD ip_value;
    UINT valEditBoxSocket;

    
private slots:
    void on_ctrlButtonBootloaderVer_clicked();

    void on_ctrlButtonLoadHex_clicked();

    void on_ctrlButtonErase_clicked();

    void on_ctrlButtonProgram_clicked();

    void on_ctrlButtonVerify_clicked();

    void on_ctrlButtonRunApplication_clicked();

    void on_ctrlButtonEraseProgVerify_clicked();

    void on_ctrlButtonConnectDevice_clicked();

    void on_ctrlCheckBoxComEnable_clicked();

    void on_ctrlCheckBoxUSBEnable_clicked();

    void on_ctrlCheckBoxEthEnable_clicked();

    void OnTimer();

    void OnReceiveResponse(unsigned char, unsigned char *);
    void OnTransmitFailure(unsigned char);

private:
    Ui::CPIC32UBLDlg *ui;
    QTimer updateTimer;

};

#endif // PIC32UBLDLG_H
