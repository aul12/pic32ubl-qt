#include "stdafx.h"

#include "comport.h"
#include "usbhid.h"
#include "ethernet.h"
#include "hex.h"
#include "bootloader.h"
#include "pic32ubldlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BOOL CEthernet::OpenUdpPort(USHORT udpSocket, ULONG ip)
{

    //---------------------------------------------
    // Create a socket for sending data
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(SendSocket == INVALID_SOCKET)
    {
//        MessageBox(NULL, "Socket creation failed!", "Socket Error", MB_ICONERROR|MB_OK);
        QMessageBox::warning(NULL, "Socket Error", "Socket creation failed!", QMessageBox::Ok);
        return false;
    }

    int	recvTmo=10;	// receive tmo, ms
    if (setsockopt(SendSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTmo, sizeof(int)) == SOCKET_ERROR)
    {
//        MessageBox(NULL, "Failed to set the receive time out!",NULL, MB_ICONERROR|MB_OK);
        QMessageBox::warning(NULL, "Socket Error", "Failed to set the receive time out!", QMessageBox::Ok);
        return false;
    }

    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver and the specified port number.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_addr.s_addr = ip;//inet_addr(ip);
    RecvAddr.sin_port = htons(udpSocket);

    return true;


}


void CEthernet::SendUdpPort(BYTE *buffer, INT bufflen)
{

    sendto(SendSocket, (CHAR*)buffer, bufflen, 0, (struct sockaddr *) &RecvAddr, sizeof(RecvAddr));
}


USHORT CEthernet::ReadUdpPort(BYTE* buffer, INT MaxLen)
{
    INT sRes;

    sRes = recvfrom( SendSocket, (CHAR*)buffer, MaxLen, 0, 0, 0);
    if(sRes == SOCKET_ERROR)
    {
        //MessageBox(NULL, "Receive Socket Error!", "Socket Error", MB_OK|MB_ICONERROR);
        return 0;
    }
    return MaxLen;

}

void CEthernet::CloseUdpPort(void)
{
    if(SendSocket != INVALID_SOCKET)
    {
        close(SendSocket);
    }
}


BOOL CEthernet::GetSocketOpenStatus(void)
{
    if(SendSocket != INVALID_SOCKET)
    {
        return true;
    }
    else
    {
        return false;
    }
}
