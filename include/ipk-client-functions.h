//
// Created by Dominik on 02.04.2017.
//

#ifndef IPK2_IPK_CLIENT_FUNCTIONS_H
#define IPK2_IPK_CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <limits.h>
#ifdef _WIN32
	#include <winsock2.h>
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

/*
 * Function returns current time, it is only used for logging
 *
 */
string getCurrDate();

/*
 * Function logs everything to console (stdout or stderr), where precisely does it print depends on bool std, which switches between stdout and stderr
 * 			normal logging is depending on whether global variable for logging is active or not
 *
 * @param bool logging specifies whether to log to console or not
 * @param bool date specifies whether we will print date before every log message
 * @param string msg msg to be printed
 * @param bool std value switching between stdout and stderr, if std is file, we print to stderr, otherwise we are printing to stdout
 */
void logConsole(bool logging, bool date, string msg, bool std);

/*
 * Function prints a error message on stderr and exits program
 *
 * @param const char *message message to be printed to stderr
 * @param bool date value switching between showing date and not showing date in log
 */
void throwException(const char *message, bool date);

/*
 * Function prints help message after argument --help is passed to program
 *
 */
void printHelp();

/*
 * Function returns substring from the given string, by delimiter
 *
 * @param string String which is searched
 * @param string delimiter which is used to split the String
 * @param bool way - true is substring to the right, false is to the left
 */
string returnSubstring(string String, string delimiter, bool way);

/*
 * Function returns command that is in message, if no command was specified, return ""
 *
 * @param string msg Message resolved from server
 * @return string command
 */
string getCmd(string msg);

/*
 * Function returns code of operation which is going to be done. There are 3 operation codes = 0 - error, 1 - bye operation, 2 - solve operation.
 * Type of operation depends on the command in message
 *
 * @param string cmd Command that is in the message sent from server
 * @return int code
 */
int getOperation(string cmd);

/*
 * parseBye function parses the bye message sent by server, if the format is correct, function returns true, otherwise returns false
 *
 * @param string message bye message to be parsed
 */
bool parseBye(string message);

/*
 * Function parses the message from solve operation into string array
 *
 * @param string message This message is going to be parsed into array
 * @return string *arr Array of operations and operator
 */
string *parseMessage(string message);

/*
 * Function converts string to long long int
 *
 * @param string operand number in string type
 * @return long long int number
 */
long long int convertStringToNumber(string operand);

/*
 * This function checks for message validity, it should be only called before parseMessage function!
 *
 * @params string message message that is going to be validated
 * @return true if message is ok, false if otherwise
 */
bool checkMessageValidity(string message);

/*
 * checkOperand function takes an operand, converts it to long long int and checks for validity, it must be called before conversion to number!
 *
 * @param string operand operand that is being checked
 * @return bool true if operand is ok, false if something wrong with the operand
 */
bool checkOperand(string operand);

/*
 * checkOperator function checks if the operator given in message from server is in range [+,-,*,/]
 *
 * @param string op operator that is being checked
 * @return true if operator is in range, false otherwise
 */
bool checkOperator(string op);

/*
 * Function currently checks only if the second number is not 0 (just for division)
 *
 * @param array of strings that were given from server and parsed into array
 * @return false if division is actual operation and 0 is a divisor, otherwise true is returned
 */
bool checkMathValidity(string *arr);

/*
 * Function takes all functions checking operands and operator and combines them in a single return value
 *
 * @param string *arr array containing parsed message
 * @return true if all operators and operand are correct, otherwise false is returned
 */
bool checkAll(string *arr);

/*
 * Function takes all operators and operands and solves the equation, after that returns the result
 *
 * @param array of operator and operands
 * @return long long int result of equation
 */
double getResult(string *arr);

/*
 * generateHello creates a request message for server with HELLO and xskala11 md5 hash
 *
 * @return request message in format HELLO hash\n
 */
string generateHello(string login);

/*
 * Function generates response message for the server
 *
 * @param long long int result number for result
 * @param bool error if this param is true, error result is returned
 * @return string response for the server in format RESULT NUM\n or RESULT ERROR\n if error flag is true
 */
string generateResult(double result, bool error);
#endif //IPK2_IPK_CLIENT_FUNCTIONS_H
