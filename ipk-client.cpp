#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <limits.h>
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

void printHelp() {
	cout << endl << "Developer: Dominik Skala (xskala11)" << endl;
	cout << "Task name: IPK2 - math operations solving client" << endl;
	cout << "Subject: IPK (2016/2017)" << endl << endl << endl;
	cout << "Math operations solving client can be only used in cooperation with server, that is sending clients their math operations, which all clients solve and send results back." << endl << endl << endl;
	cout << "Usage: ipk-client IP" << endl;
	cout << "Arguments:" << endl;
	cout << "  IP\t\t\tSpecifies the IP of the math server in format of IPv4, IPv6 or Hostname. This server should ALWAYS run on port 55555 (connection to this server is forced)" << endl;
	cout << "  --help\t\tCan be used only when no other arguments are passed. Prints this help message." << endl;
	exit(0);
}

string getCurrDate() {

	char mbstr[100];
	string dateTime;

	time_t t = time(NULL);
	if (strftime(mbstr, sizeof(mbstr), "%T", localtime(&t))) {
		dateTime = mbstr;
	} else {
		cout << ("Error: Date could not be resolved.") << endl;
	}

	return dateTime;
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
		return 0;
	} else {
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


long long int convertStringToInt(string operand) {
	return (strtoll(operand.c_str(), NULL, 10));
}

bool checkOperand(string operand) {

	char *pEnd;
	long long int op;
	if ((op = strtoll(operand.c_str(), &pEnd, 10)) == LONG_LONG_MIN) {
		return false;
	} else if (op == LONG_LONG_MAX) {
		return false;
	} else if (operand == pEnd) {
		return false;
	}

	return true;
}

long long int checkMathValidity(string *arr) {
	return convertStringToInt(arr[2]);
}

long long int getResult(string *arr) {
	long long int operand1 = convertStringToInt(arr[0].c_str());
	long long int operand2 = convertStringToInt(arr[2].c_str());
	long long int result = 0;
	
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

string generateResult(long long int result) {
	return "RESULT "+to_string(result)+"\n";
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		throwException("Error: Wrong amount of arguments.");
	}

	if (strcmp(argv[1], "--help") == 0) {
		printHelp();
	}

	// port was defined in the task assignment
	unsigned short port = 55555;

	// get IP info
	struct addrinfo hint, *res = NULL;

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	int ret;
	memset(&hint, '\0', sizeof hint);
	if ((ret = getaddrinfo(argv[1], NULL, &hint, &res)) != 0) {
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

	unsigned int client_socket;
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
	cout << getCurrDate()+": Sending request to the server on IP: "+argv[1] << endl;
	char request[1024];
	memset(&request, '\0', sizeof(request));
	int rcv;
	for (;;) {
		if ((rcv = recv(client_socket, request, 1024, 0)) > 0) {
			cout << getCurrDate()+": Server responded and receiving math operation." << endl;
			string msg(request);
			cout << getCurrDate()+": Solving: "+msg;
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
				string error = "RESULT ERROR\n";
				string *arr = parseMessage(msg);
				if ((!checkOperand(arr[0])) || !checkOperand(arr[2])) {
					cout << getCurrDate()+": Math operation result: "+error;
					send(client_socket, error.c_str(), 1024, 0);
				} else {
					if (!checkMathValidity(arr)) {
						cout << getCurrDate()+": Math operation result: "+error;
						send(client_socket, error.c_str(), 1024, 0);
					} else {
						long long int rst = getResult(arr);
						cout << getCurrDate()+": Math operation result: "+generateResult(rst);
						send(client_socket, generateResult(rst).c_str(), 1024, 0);
					}
				}

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
