#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <unistd.h>
#ifdef _WIN32
#include <Winsock2.h>
	#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <sstream>
#include <iostream>
#include <time.h>
#include <openssl/evp.h>

using namespace std;


/*
 * Function prints a error message on cerr and exits program
 *
 * @param const char *message message to be printed to cerr
 */
void throwException(const char *message) {
	cerr << (message) << endl;
	exit(EXIT_FAILURE);
}

/*
 * Function returns substring from the given string, by delimiter
 *
 * @param string String which is searched
 * @param string delimiter which is used to split the String
 * @param bool way - true is substr to the right, false is to the left
 */
string returnSubstring(string String, string delimiter, bool way){

	string subString = "";
	if (String.find(delimiter) != string::npos) {
		if (way) {
			subString = String.substr(String.find(delimiter) + delimiter.length());
		} else {
			subString = String.substr(0, String.find(delimiter));
		}
	}

	return subString;
}

string getCmd(string msg) {
	return returnSubstring(returnSubstring(msg, "\n", false), " ", false);
}

int getOp(string cmd) {

	int code = 0;
	if (cmd == "BYE") {
		code = 1;
	} else if (cmd == "SOLVE") {
		code = 2;
	}
	return code;
}

int parseBye(string message) {

	string hash;
	if ((hash = returnSubstring(returnSubstring(message, "\n", false), " ", true)) == "") {
		return 1;
	} else {
		cout << hash << endl;
		return 0;
	}
}

int checkMessageValidity(string message) {


}

string createHello() {
	string msg;
	// todo hashing, current state is temporary
	string hash = "f92f0527f211c422c36e9970bc2be3be";

	return msg = "HELLO "+hash;
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		throwException("Error: Wrong amount of arguments.");
	}

	// port was defined in the task assignment
	unsigned short port = 55555;

	struct sockaddr_in serveraddr;

	serveraddr.sin_port = htons(port);
	// get IP info
	struct addrinfo hint, *res = NULL;

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	int ret;
	memset(&hint, '\0', sizeof hint);
	if (ret = getaddrinfo(argv[1], NULL, &hint, &res)) {
		throwException("Error: Invalid IP address.");
	}

	if (res->ai_family == AF_INET || res->ai_family == AF_INET6) {
		serveraddr.sin_family = res->ai_family;
	} else {
		throwException("Error: Unknown format of IP address.");
	}

	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	freeaddrinfo(res);

	int client_socket;
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throwException("Error: Could not open client socket.\n");
	}

	if ((connect(client_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		throwException("Error: Couldn't connect to server.\n");
	}

	string message = createHello();

	send(client_socket, message.c_str(), message.size(), 0);

	char response[1024];
	int rcv;
	for (;;) {
		if ((rcv = recv(client_socket, response, 1024, 0)) > 0) {
			string msg(response);
			int op;
			if ((op = (getOp(cmd = getCmd(msg)))) == 0) {
				// 0 means wrong format, new iteration of cycle
			} else if (op == 1) {
				// bye
				if (parseBye(msg)) {
					// if bye msg was is not ok, we will for new cycle of the cycle
					continue;
				} else {
					// if bye message format is ok, break to close socket
					break;
				}
			} else if (op == 2) {
				// solve
				if (checkMessageValidity(message)) {
					continue;
				}


			} else {
				// we did receive something that shouldn't be received, program will now try to read another message from server (probably wrong memory access, or corrupted memory block)
				continue;
			}
		} else {
			if (rcv == 0) {
				break;
			}
			throwException("Error: Couldn't read data from server correctly.");
		}
	}

	if (close(client_socket) != 0) {
		throwException("Error: Could not close socket.\n");
	}


	return 0;
}
