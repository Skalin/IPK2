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

using namespace std;

// login const to be hashed
const string login = "xskala11";


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

	cout << code << endl;
	return code;
}

int parseBye(string message) {

	string hash;
	if ((hash = returnSubstring(returnSubstring(message, "\n", false), " ", true)) == "") {
		return 0;
	} else {
		cout << hash << endl;
		return 1;
	}
}

int checkMessageValidity(string message) {
	
	string parsed = returnSubstring(returnSubstring(message, "\n", false), " ", true); // math op with two operands - operand operator operand
	parsed = returnSubstring(parsed, " ", true); // operator operand
	parsed = returnSubstring(parsed, " ", true); // operand
	return returnSubstring(parsed, " ", true) == "" ? 0 : 1;
	
	
	// todo osetreni chybovych stavu
}

string *parseMessage(string message) {
	
	string parsed = returnSubstring(returnSubstring(message, "\n", false), " ", true); // math op with two operands - operand operator operand
	
	string *arr = new string[3];
	
	arr[0] = returnSubstring(parsed, " ", false); // operand no.1
	parsed = returnSubstring(parsed, " ", true); // operator operand
	arr[1] = returnSubstring(parsed, " ", false); // operator
	parsed = returnSubstring(parsed, " ", true); // operand
	arr[2] = parsed; // operand
	
	return arr;	
}

int convertStringToInt() {

	int number = 0;

	return number;
}

int getResult(string *arr) {
	int operand1 = atoi(arr[0].c_str());
	int operand2 = atoi(arr[2].c_str());
	int result = 0;
	
	if (arr[1] == "+") {
		result = operand1 + operand2;
	} else if (arr[1] == "-") {
		result = operand1 - operand2;
	} else if (arr[1] == "*") {
		result = operand1 * operand2;
	} else if (arr[1] == "/") {
		result = operand1 / operand2;
	}
	return result;
}

string generateHello() {
	string msg;
	// todo hashing, current state is temporary
	string hash = "f92f0527f211c422c36e9970bc2be3be";

	return msg = "HELLO "+hash+"\n";
}

char *generateResult(long long int result) {

	string strResponse = "RESULT "+to_string(result)+"\n";
	char *response;
	strcpy(response, strResponse.c_str());
	return response;
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
				throwException("Error: Wrong amount of arguments.");
	}

	// port was defined in the task assignment
	unsigned short port = 55555;

	// get IP info
	struct addrinfo hint, *res = NULL;

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	int ret;
	memset(&hint, '\0', sizeof hint);
	if (ret = getaddrinfo(argv[1], NULL, &hint, &res)) {
		throwException("Error: Invalid IP address.");
	}

	int ip = 0;

	struct sockaddr_in serveraddr;
	struct sockaddr_in6 serveraddr6;
	if (res->ai_family == AF_INET) {
		serveraddr = *(struct sockaddr_in *) res->ai_addr;
		serveraddr.sin_port = htons(port);
		ip = 4;
	} else if (res->ai_family == AF_INET6) {
		serveraddr6.sin6_family = (short)res->ai_family;
		inet_pton(AF_INET6, argv[1], &serveraddr6.sin6_addr.s6_addr);
		serveraddr6.sin6_port = htons(port);
		ip = 6;
	} else {
		throwException("Error: Unknown format of IP address.");
	}

	freeaddrinfo(res);

	int client_socket;
	if (ip == 4) {
		if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
			throwException("Error: Could not open client socket.");
		}

		if ((connect(client_socket, (struct sockaddr *) &serveraddr, sizeof(serveraddr))) < 0) {
			throwException("Error: Couldn't connect to server.");
		}
	} else if (ip == 6){
		if ((client_socket = socket(serveraddr6.sin6_family, SOCK_STREAM, 0)) <= 0) {
			throwException("Error: Could not open client socket.");
		}

		if ((connect(client_socket, (struct sockaddr *) &serveraddr6, sizeof(serveraddr6))) < 0) {
			throwException("Error: Couldn't connect to server.");
		}
	} else {
		throwException("Error: Non recognizable IP.");
	}

	string message = generateHello();
	send(client_socket, message.c_str(), message.size(), 0);
	cout << message << endl;

	cout << "something client_socket send is happening" << endl;
	char request[1024];
	memset(&request, '\0', sizeof(request));
	int rcv;
	for (;;) {
		if ((rcv = recv(client_socket, request, 1024, 0)) > 0) {
			string msg(request);
			cout << "received message" << endl;
			cout << msg;
			memset(&request, '\0', sizeof(request));
			int op;
			if ((op = (getOp(getCmd(msg)))) == 1) {
				// bye
				if (!parseBye(msg)) {
					// if bye msg is not ok, we will continue for new iteration of the cycle
					continue;
				} else {
					// if bye message format is ok, break to close socket
					break;
				}
			} else if (op == 2) {
				// solve
				if (checkMessageValidity(msg)) {
					continue;
				}

				string *arr = parseMessage(msg);

				long long int rst = getResult(arr);
				// conv int to string
				char *response = generateResult(rst);
				printf("%s", response);
				if (strchr(response, '\n')) {
					cout << "\\n je pritomno" << endl;
				}
				send(client_socket, response, 20, 0);
				memset(&response, '\0', sizeof(response));
			} else {
				// we did receive something that shouldn't be received, program will now try to read another message from server (probably wrong memory access, or corrupted memory block)
				continue;
			}
		} else if (rcv == 0) {
			cout << "Info: Server transmission end.\n";
			break;
		} else {
			throwException("Error: Couldn't read data from server correctly.");
		}
	}

	if (close(client_socket) != 0) {
		throwException("Error: Could not close socket.");
	}

	return 0;
}
