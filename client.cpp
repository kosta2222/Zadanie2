#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include <synchapi.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
using namespace std;
// Используем библиотеку RapidJson
StringBuffer WriteJsonInfoAboutWindow(const char* key_as_window_name, const char* value_as_window_class) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key(key_as_window_name);
    writer.String(value_as_window_class);
    writer.EndObject();
    return  s;
}
using namespace std;
// адресс сервера - 1
// порт у сервера - 2
int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "We must have 2 parameters." << endl;
        cout << "Usage: <program>  <addres to server> <servers port>" << endl;
        cout << "Server is on port 25001" << endl;
        return EXIT_FAILURE;
    }
    cout << "CL START" << '\n';
    WSAData WSData;
    WSAStartup(0x101, &WSData);
    struct sockaddr_in peer;
    SOCKET s;
    int rc;
    char wnd_title[256];
    char wnd_class_name[256];
    char* json_str;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(atoi(argv[2]));
    peer.sin_addr.s_addr = inet_addr(argv[1]); // 127.0.0.1 - адресс сервера
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Could not create socket.");
        closesocket(s);
    }
    rc = connect(s, (struct sockaddr *) &peer, sizeof (peer));
    if (rc) {
        perror("Could not connect - Server is unavailable.Launch the Server.");
        return EXIT_FAILURE;
    }
    int client_running = 1;
    while (client_running) {
        HWND hwnd = GetForegroundWindow();
        GetWindowText(hwnd, wnd_title, sizeof (wnd_title));
        GetClassName(hwnd, wnd_class_name, sizeof (wnd_class_name));
        StringBuffer json_expr=WriteJsonInfoAboutWindow(wnd_title, wnd_class_name);
        rc = send(s,json_expr.GetString(), json_expr.GetSize(), 0);
        cout << "json " <<json_expr.GetString()<< endl;
        if (rc <= 0) {
            perror("Could not send data");
            perror("Maybe Server connection lost");
            closesocket(s);
            break;
        }
        Sleep(1000);
    }
    WSACleanup();
    return EXIT_SUCCESS;
}