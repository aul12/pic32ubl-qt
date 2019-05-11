#include "pic32ubldlg.h"
#include "hidapi.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget d;

    CPIC32UBLDlg w;
    w.setGeometry((d.screen()->width() - w.frameGeometry().width()) / 2,(d.screen()->height() - w.frameGeometry().height()) / 2, w.frameGeometry().width(), w.frameGeometry().height());
    w.show();

    if (hid_init())
    {
        w.PrintKonsole("Problem initialising HID");
    }
    
    return a.exec();
}
