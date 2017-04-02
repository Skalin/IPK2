//
// Created by Dominik on 02.04.2017.
//

#include "ipk-client-functions.h"

#ifndef IPK2_IPK_CLIENT_H
#define IPK2_IPK_CLIENT_H

string getCurrDate() {

	char multiByteString[100];
	string dateTime;

	time_t t = time(NULL);
	if (strftime(multiByteString, sizeof(multiByteString), "%T", localtime(&t))) {
		dateTime = multiByteString;
	} else {
		cout << ("Error: Date could not be resolved.") << endl;
	}

	return dateTime;
}

void logConsole(string msg, bool std) {
	if (std) {
		cerr << msg << endl;
	} else {
		if (logging) {
			cout << getCurrDate()+msg;
		}
	}
}

void throwException(const char *message) {
	logConsole(message, true);
	exit(EXIT_FAILURE);
}

void printHelp() {
	cout << endl << "Developer: Dominik Skála (xskala11)" << endl;
	cout << "Task name: IPK2 - math operations solving client" << endl;
	cout << "Subject: IPK (2016/2017)" << endl << endl << endl;
	cout << "Math operations solving client can be only used in cooperation with server, that is sending clients their math operations, which all clients solve and send results back." << endl << endl << endl;
	cout << "Usage: ipk-client IP" << endl;
	cout << "Arguments:" << endl;
	cout << "  IP\t\t\tSpecifies the IP of the math server in format of IPv4, IPv6 or Hostname. This server should ALWAYS run on port 55555 (connection to this server is forced)" << endl;
	cout << "  --help\t\tCan be used only when no other arguments are passed. Prints this help message." << endl;
	exit(0);
}

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

int getOperation(string cmd) {

	int code = 0;
	if (cmd == "BYE") {
		code = 1;
	} else if (cmd == "SOLVE") {
		code = 2;
	}
	return code;
}

bool parseBye(string message) {
	return !(returnSubstring(returnSubstring(message, "\n", false), " ", true) == "");
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

long long int convertStringToNumber(string operand) {
	return (strtoll(operand.c_str(), NULL, 10));
}

bool checkMessageValidity(string message) {

	string parsed = returnSubstring(returnSubstring(message, "\n", false), " ", true); // math op with two operands - operand operator operand
	parsed = returnSubstring(parsed, " ", true); // operator operand
	parsed = returnSubstring(parsed, " ", true); // operand

	return !(returnSubstring(parsed, " ", true) == "");
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

bool checkOperator(string op) {
	return !(op != "+" && op != "-" && op != "*" && op != "/");
}

bool checkMathValidity(string *arr) {
	return !(arr[1] == "/" && convertStringToNumber(arr[2]) == 0); // we are now just checking for zero
}

bool checkAll(string *arr) {
	return (checkOperand(arr[0]) && checkOperand(arr[2]) && checkOperator(arr[1]) && checkMathValidity(arr));
}

long long int getResult(string *arr) {
	long long int operand1 = convertStringToNumber(arr[0].c_str());
	long long int operand2 = convertStringToNumber(arr[2].c_str());
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

string generateResult(long long int result, bool error) {
	if (error)
		return "RESULT ERROR\n";
	return "RESULT "+std::to_string(result)+"\n";
}
#endif //IPK2_IPK_CLIENT_H