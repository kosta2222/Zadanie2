#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include <synchapi.h>
#include <fstream>
using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "We must have 1 parameters." << endl;
        cout << "Usage: <program> <name log file>" << endl;
        cout << "Server is on port 25001" << endl;
        return EXIT_FAILURE;
    }
    cout << "SR START" << '\n';
    WSAData WSData;
    WSAStartup(0x101, &WSData);
    struct sockaddr_in local;
    SOCKET s;
    int s1;
    int rc;
    char buf [1024];
    ofstream fout(argv[1]); // имя файла куда записывать
    local.sin_family = AF_INET;
    local.sin_port = htons(25001);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) perror("Could not create socket.");
    rc = bind(s, (struct sockaddr *) &local, sizeof (local));
    if (rc < 0) {
        perror("Could not bind address");
        closesocket(s);
    }
    int server_running = 1;
    while (server_running) {
        rc = listen(s, 5);
        if (rc) {
            perror("Cannot listen port.");
            closesocket(s);
        }
        s1 = accept(s, NULL, NULL);
        if (s1 < 0) {
            perror("Cannot accept connection.");
            closesocket(s);
        }
        rc = recv(s1, buf, 1024, 0);
        buf[rc]='\0';
        if (rc <= 0) {
            perror("Cannot read data.");
            closesocket(s);
        }
        fout << buf;
        cout << buf << '\n';
        Sleep(1000);
    }
    fout.close();
    closesocket(s);
    WSACleanup();
    system("PAUSE");
    return EXIT_SUCCESS;
}