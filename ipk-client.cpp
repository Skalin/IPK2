#include "include/ipk-client-functions.cpp"

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

	struct sockaddr_in serverAddress;
	struct sockaddr_in6 serverAddress6;
	if (res->ai_family == AF_INET) {
		serverAddress = *(struct sockaddr_in *) res->ai_addr;
		serverAddress.sin_port = htons(port);
		ip = 4;
	} else if (res->ai_family == AF_INET6) {
		serverAddress6.sin6_family = (short)res->ai_family;
		inet_pton(AF_INET6, argv[1], &serverAddress6.sin6_addr.s6_addr);
		serverAddress6.sin6_port = htons(port);
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
		if ((connect(client_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) < 0) {
			throwException("Error: Couldn't connect to server.");
		}
	} else if (ip == 6){
		if ((client_socket = socket(serverAddress6.sin6_family, SOCK_STREAM, 0)) <= 0) {
			throwException("Error: Could not open client socket.");
		}
		if ((connect(client_socket, (struct sockaddr *) &serverAddress6, sizeof(serverAddress6))) < 0) {
			throwException("Error: Couldn't connect to server.");
		}
	} else {
		throwException("Error: Non recognizable IP.");
	}

	string message = generateHello();
	send(client_socket, message.c_str(), message.size(), 0);
	logConsole(": Sending request to the server on IP: "+(string) argv[1] + "\n", false);
	char request[1024];
	memset(&request, '\0', sizeof(request));
	int rcv;
	for (;;) {
		if ((rcv = recv(client_socket, request, 1024, 0)) > 0) {
			logConsole(": Server responded and receiving math operation.\n", false);
			string msg(request);
			memset(&request, '\0', sizeof(request));
			int op;
			if ((op = (getOperation(getCmd(msg)))) == 1) {
				// bye
				if (!parseBye(msg)) {
					// if bye msg is not ok, we will continue for new iteration of the cycle
					continue;
				} else {
					// if bye message format is ok, break to close socket
					logging = true;
					logConsole(": "+returnSubstring(returnSubstring(msg, "\n", false), " ", true) + "\n", false);
					logging = false;
					break;
				}
			} else if (op == 2) {
				logConsole(": Solving: "+msg, false);
				if (checkMessageValidity(msg)) {
					continue;
				}
				string *arr = parseMessage(msg);
				long long int rst = 0;
				if (!checkAll(arr)) {
					logConsole(": Math operation result: "+generateResult(rst, true), false);
					send(client_socket, generateResult(rst, true).c_str(), 1024, 0);
				} else {
						rst = getResult(arr);
						logConsole(": Math operation result: "+generateResult(rst, false), false);
						send(client_socket, generateResult(rst, false).c_str(), 1024, 0);
				}
			} else {
				// we did receive something that shouldn't be received, program will now try to read another message from server (probably wrong memory access, or corrupted memory block)
				continue;
			}
		} else if (rcv == 0) {
			logConsole("Server transmission end. No more equations will be coming.\n", false);
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