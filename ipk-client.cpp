#include "include/ipk-client-functions.cpp"

int main(int argc, char *argv[]) {
	// default values to run the program, they are rechecked and modified in checkArguments function
	bool logging = false;
	bool date = false;
	string login = "xskala11";

	//check for arguments validity
	checkArguments(argc, argv, &logging, &date, &login);

	// port was defined in the task assignment
	unsigned short port = 55555;

	// get IP info
	struct addrinfo hint, *res = NULL;

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	int ret;
	memset(&hint, '\0', sizeof hint);
	if ((ret = getaddrinfo(argv[1], NULL, &hint, &res)) != 0) {
		throwException("Error: Invalid IP address.", date);
	}

	struct sockaddr_in serverAddress;
	struct sockaddr_in6 serverAddress6;
	if (res->ai_family == AF_INET) {
		serverAddress = *(struct sockaddr_in *) res->ai_addr;
		serverAddress.sin_port = htons(port);
	} else if (res->ai_family == AF_INET6) {
		serverAddress6.sin6_family = (short)res->ai_family;
		inet_pton(AF_INET6, argv[1], &serverAddress6.sin6_addr.s6_addr);
		serverAddress6.sin6_port = htons(port);
	} else {
		throwException("Error: Unknown format of IP address.", date);
	}

	unsigned int client_socket;
	if (res->ai_family == AF_INET) {
		if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
			throwException("Error: Could not open client socket.", date);
		}
		if ((connect(client_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) < 0) {
			throwException("Error: Couldn't connect to server.", date);
		}
	} else if (res->ai_family == AF_INET6){
		if ((client_socket = socket(serverAddress6.sin6_family, SOCK_STREAM, 0)) <= 0) {
			throwException("Error: Could not open client socket.", date);
		}
		if ((connect(client_socket, (struct sockaddr *) &serverAddress6, sizeof(serverAddress6))) < 0) {
			throwException("Error: Couldn't connect to server.", date);
		}
	} else {
		throwException("Error: Non-recognizable IP.", date);
	}

	freeaddrinfo(res);

	string message = generateHello(login);
	int sent;

	if ((sent = send(client_socket, message.c_str(), message.size(), 0)) < 0) {
		throwException("Error: Could not send HELLO hash\\n", date);
	}
	logConsole(logging, date, "Sending request to the server on IP: "+(string) argv[1] + "\n", false);
	logConsole(logging, date, generateHello(login), false);
	char request[1024];
	memset(&request, '\0', sizeof(request));
	int rcv;
	for (;;) {
		if ((rcv = recv(client_socket, request, sizeof(request), 0)) > 0) {
			logConsole(logging, date, "Server responded and receiving request.\n", false);
			string msg(request);
			memset(&request, '\0', sizeof(request));
			int op;
			if ((op = (getOperation(getCmd(msg)))) == 1) {
				// bye
				if (!parseBye(msg)) {
					// if bye msg is not ok, we will continue for new iteration of the cycle
					continue;
				} else {
					// if bye message format is ok, we will print secret and then we will break to close the socke
					logConsole(true, date, returnSubstring(returnSubstring(msg, "\n", false), " ", true), false);
					break;
				}
			} else if (op == 2) {
				logConsole(logging, date, "Solving: "+msg, false);
				if (checkMessageValidity(msg)) {
					continue;
				}
				string *arr = parseMessage(msg);
				double rst = 0.0;
				// if all operands and operator are not correct, we will return ERROR otherwise, we will return the actual result of equation
				if (!checkAll(arr)) {
					logConsole(logging, date, "Math operation result: "+generateResult(rst, true), false);
					if ((sent = send(client_socket, generateResult(rst, true).c_str(), generateResult(rst, true).size(), 0)) < 0) {
						throwException("Error: Could not send math operation result.", date);
					}
				} else {
					rst = getResult(arr);
					logConsole(logging, date, "Math operation result: "+generateResult(rst, false), false);
					if ((sent = send(client_socket, generateResult(rst, false).c_str(), generateResult(rst, false).size(), 0)) < 0) {
						throwException("Error: Could not send math operation result.", date);
					}
				}
			} else {
				// we did receive something that shouldn't be received, program will now try to read another message from server (probably wrong memory access, or corrupted memory block)
				continue;
			}
		} else if (rcv == 0) {
			// server unexpectedly closed connection
			logConsole(logging, date, "Server transmission end. Probably the wrong result was sent to the server and server decided to end the connection.\n", false);
			break;
		} else {
			// probably connection lost
			throwException("Error: Couldn't read data from server correctly.", date);
		}
	}

	if (close(client_socket) != 0) {
		throwException("Error: Could not close socket.", date);
	}

	return 0;
}