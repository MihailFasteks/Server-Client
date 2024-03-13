
//#include "Resource.h"
//#include <iostream>
//#include "WinSock2.h" //����� ��������� ����������, �����������
////��� Winsock 2 API.
//#include <ws2tcpip.h> //�������� ������� ��� ������ � ��������
// //����. inet_pton
//#pragma comment(lib, "Ws2_32.lib") //������� ���������� Windows Sockets
//
//#pragma comment(linker,"\"/manifestdependency:type='win32' \
//name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
//processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "Windows.h"
#include "tchar.h"
#include "resource.h"
using namespace std;

using namespace std;

HWND hEdit1, hEdit2, bConnect, bSend, hDialog;
SOCKET _socket; //���������� ������
sockaddr_in addr; //��������� ����� � ����
const int MAXSTRLEN = 255;
WSADATA wsaData;//��������� ��� �������� ����������
void Socket() {
   
    //� ������������� �������

    //������������� �������
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //������� ��������� �����, �������� TCP
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //��������� ������� IPv4
    addr.sin_family = AF_INET;
    /*����������� ����� � ����������
    ��������� �������� �������, ��������� ��������� � ���� sin_addr */
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    //��������� ����. 
    //������� htons ��������� �������������� ����� �
    //������� ������� ����, �.�. ����������� ������� 
    //�������� ������� �� �������� � ��������
    addr.sin_port = htons(20000);
}

void InitEdit(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    hDialog = hDlg;
    hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
    hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);

    bConnect = GetDlgItem(hDlg, IDC_BUTTON1);
    bSend = GetDlgItem(hDlg, IDC_BUTTON2);
    Socket();
}




LRESULT CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch (uMsg)
    {
    case WM_INITDIALOG:
        InitEdit(hDlg, uMsg, wParam, lParam);
        break;

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_BUTTON1) {
            if (connect(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                MessageBox(NULL, L"Error of client", L"ERROR", 0);
            }
            else {
                MessageBox(NULL, L"Client is done", L"SUCESSFUL", 0);
            }

        }
         else if (LOWORD(wParam) == IDC_BUTTON2) {
            char buf[MAXSTRLEN];

            int length = GetWindowTextLength(hEdit2);
            char* pBuffer = new char[length + 1];
            /*TCHAR buf[MAXSTRLEN];*/
            GetWindowTextA(hEdit2, pBuffer, length + 1);
          

           /* if (GetWindowText(hEdit2, pBuffer, length + 1) == 0) {
                MessageBox(NULL, L"Error getting text from edit control", L"ERROR", 0);
            }
            else {
                MessageBox(NULL, pBuffer, 0, 0);
            }*/
           
            send(_socket, pBuffer, strlen(pBuffer), 0);
            int i = recv(_socket, buf, MAXSTRLEN, 0);
            buf[i] = '\0';
            /*MessageBox(NULL, pBuffer, 0, 0);*/
            SetWindowTextA(hEdit1, buf);
            delete[] pBuffer;
        }
         else if (LOWORD(wParam) == IDC_BUTTON3) {
            closesocket(_socket);
            MessageBox(NULL, L"Connect destroyed!", 0, 0);
            //����������� �������
            WSACleanup();
            EndDialog(hDlg, 0);
        }
        break;

    }
    break;
    case WM_CLOSE:
    {
        closesocket(_socket);
        //����������� �������

        WSACleanup();
        EndDialog(hDlg, 0);
        return TRUE;
    }

    return FALSE;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    }
    return 0;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}