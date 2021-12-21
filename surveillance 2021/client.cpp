#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <math.h>

#include "huffman.h"

using namespace std;

struct pixel
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char alpha;
};

int recv_wrap(SOCKET s, char * buf, int len, int flags)
{
    int to_recv = len;
    int recvd = 0;
    int cursor = 0;

    while(to_recv)
    {
        recvd = recv(s, buf + cursor, to_recv, flags);
        if(recvd == SOCKET_ERROR)
            return SOCKET_ERROR;

        cursor += recvd;
        to_recv -= recvd;
    }

    return len;
}

int save_image(unsigned char * data, int width, int height, string filename)
{
    ofstream img(filename);

    img<<"P2\n"<<width<<" "<<height<<"\n256\n";
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            img<<(unsigned int)data[width * i + j]<<" ";
        }
        img<<"\n";
    }

    img.close();
    return 1;
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Setup our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(32303);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = INADDR_ANY;

	if(bind(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) == SOCKET_ERROR)
	{
		cout<<"Unable to bind socket!\n";
		WSACleanup();
		return 0;
	}

	listen(Socket, 1);
    cout<<"Waiting for connection:\n";
	SOCKET TempSock = accept(Socket, NULL, NULL);
    cout<<"Client connected!\n\n";

    while(1)
    {
        int res;
        char mark[8];
        memset(mark, 0, 8);

        res = recv_wrap(TempSock, mark, 4, 0);
        if(res == SOCKET_ERROR)
        {
            cout<<"Socket error!\n";
            break;
        }

        if(strcmp(mark, "XMAS"))
        {
            cout<<"Unknown packet!\n";
            break;
        }

        cout<<"Receiving packet...\n";

        int id = 0;
        int timestamp = 0;

        res = recv_wrap(TempSock, (char *)&id, 4, 0);
        if(res == SOCKET_ERROR)
        {
            cout<<"Socket error!\n";
            break;
        }

        cout<<"Type: "<<id<<"\n";

        res = recv_wrap(TempSock, (char *)&timestamp, 4, 0);
        if(res == SOCKET_ERROR)
        {
            cout<<"Socket error!\n";
            break;
        }

        cout<<"Timestamp: "<<timestamp<<"\n";

        if(id == 1)
        {
            char key[8];
            memset(key, 0, 8);

            res = recv_wrap(TempSock, key, 4, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error!\n";
                break;
            }

            cout<<"Undecrypted key: "<<*(int *)(key)<<"\n";
            *(int *)(key) ^= ~(timestamp - 69420);
            cout<<"Decrypted squared key: "<<*(int *)(key)<<"\n";
            float key_sq = (float)(*(int *)(key));
            float key_rt = sqrt(key_sq);
            int key_i = (int) key_rt;

            cout<<"Pressed key: "<<key_i<<" ("<<(char)key_i<<")\n\n";
        }
        else if(id == 2)
        {
            int width = 0;
            int height = 0;

            res = recv_wrap(TempSock, (char *)&width, 4, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error!\n";
                break;
            }

            res = recv_wrap(TempSock, (char *)&height, 4, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error!\n";
                break;
            }
            cout<<"Width: "<<width<<", height: "<<height<<"\n";

            char * header = new char[266];
            char * buf = new char[width * height * 100];
            string packet = "";

            res = recv_wrap(TempSock, header, 10, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error - receiving padding!\n";
                break;
            }

            res = recv_wrap(TempSock, header, 266, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error - receiving huffman header!\n";
                break;
            }

            if(header[0] != 'H' || header[1] != 'C')
            {
                cout<<"Corrupted huffman header!\n";
                break;
            }

            uint64_t comp_size = *(uint64_t *)(header + 2);

            res = recv_wrap(TempSock, buf, comp_size, 0);
            if(res == SOCKET_ERROR)
            {
                cout<<"Socket error - receiving image packet!\n";
                break;
            }

            cout<<"Received image packet!\n";

            //decompress image packet
            huffmanCompressor * HFC = new huffmanCompressor();
            HFC->deserialize(header + 10);

            delete [] header;

            HFC->decompressFinal(buf, comp_size, packet);

            delete HFC;

            for(int i = 0; i < 24; ++i)
            {
                string filename = to_string(timestamp) + "_" + to_string(i) + ".pgm";
                save_image(packet.c_str() + i * width * height, width, height, filename);
            }

            delete [] buf;
            packet = "";
        }
        else
        {
            cout<<"Id not done yet!\n";
        }
    }
    return 0;
}
