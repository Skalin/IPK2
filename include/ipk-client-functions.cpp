//
// Created by Dominik on 02.04.2017.
//

#include <iomanip>
#include "ipk-client-functions.h"
#include "md5/md5.cpp"


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

void logConsole(bool logging, bool date, string msg, bool std) {
	if (std) {
		if (date) {
			cerr << getCurrDate()+": "+msg << endl;
		} else {
			cerr << msg << endl;
		}
	} else {
		if (logging) {
			if (date) {
				cout << getCurrDate()+": "+msg;
			} else {
				cout << msg;
			}
		}
	}
}

void throwException(const char *message, bool date) {
	logConsole(true, date, message, true);
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
	cout << "  --login=LOGIN\t\tAllows to change the login of student. Not required argument. Default is \'xskala11\'" << endl;
	cout << "  --logging=[T/F]\t\tChooses between logging to console all operations or only the BYE message. Not required argument. Default is \'false\'" << endl;
	cout << "  --date=[T/F]\t\tChooses between showing date in logs or not. Not required argument. Default is \'false\'" << endl;
	exit(0);
}

void checkArguments(int argc, char *argv[], bool *log, bool *date, string *login) {

	if (argc < 2 && argc > 5) {
		throwException("Error: Wrong amount of arguments. Use --help.", *date);
	} else if (argc == 3) {
		string arg = argv[2];
		if (arg.find("--logging=") != string::npos) {
			if (arg.substr(10) == "true") {
				*log = true;
			} else if (arg.substr(10) == "false") {
				*log = false;
			} else {
				throwException("Error: Wrong logging argument usage. Use --help.", *date);
			}
		} else if (arg.find("--login=") != string::npos) {
			*login = arg.substr(8);
		} else if (arg.find("--date=") != string::npos) {
			if (arg.substr(7) == "true") {
				*date = true;
			} else if (arg.substr(7) == "false") {
				*date = false;
			} else {
				throwException("Error: Wrong date argument usage. Use --help.",*date);
			}
		} else {
			throwException("Error: Wrong amount of arguments. Use --help.", *date);
		}
	} else if (argc == 4) {
		string arg = argv[2];
		string arg1 = argv[3];
		if (arg.find("--logging=") != string::npos || arg1.find("--logging=") != string::npos) {
			if (arg.find("--logging=") != string::npos && arg1.find("--logging=") != string::npos) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			} else {
				if (arg.find("--logging=") != string::npos) {
					if (arg.substr(10) == "true") {
						*log = true;
					} else if (arg.substr(10) == "false") {
						*log = false;
					} else {
						throwException("Error: Wrong logging argument usage. Use --help.", *date);
					}
				} else if (arg1.find("--logging=") != string::npos) {
					if (arg1.substr(10) == "true") {
						*log = true;
					} else if (arg1.substr(10) == "false") {
						*log = false;
					} else {
						throwException("Error: Wrong logging argument usage. Use --help.", *date);
					}
				} else {
					throwException("Error: Wrong amount of arguments. Use --help.", *date);
				}
			}
		}
		if (arg.find("--login=") != string::npos || arg1.find("--login=") != string::npos) {
			if (arg.find("--login=") != string::npos && arg1.find("--login") != string::npos) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			}
			if (arg.find("--login=") != string::npos)
				*login = arg.substr(8);
			if (arg1.find("--login=") != string::npos)
				*login = arg1.substr(8);
		}
		if (arg.find("--date=") != string::npos || arg1.find("--date=") != string::npos) {
			if (arg.find("--date=") != string::npos && arg1.find("--date=") != string::npos) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			} else {
				if (arg.find("--date=") != string::npos) {
					if (arg.substr(7) == "true") {
						*date = true;
					} else if (arg.substr(7) == "false") {
						*date = false;
					} else {
						throwException("Error: Wrong date argument usage. Use --help.", *date);
					}
				} else if (arg1.find("--date=") != string::npos) {
					if (arg1.substr(7) == "true") {
						*date = true;
					} else if (arg1.substr(7) == "false") {
						*date = false;
					} else {
						throwException("Error: Wrong date argument usage. Use --help.", *date);
					}
				} else {
					throwException("Error: Wrong amount of arguments. Use --help.", *date);
				}
			}

		}
	} else if (argc == 5) {
		string arg = argv[2];
		string arg1 = argv[3];
		string arg2 = argv[4];

		if (arg.find("--logging=") != string::npos  || arg1.find("--logging=") != string::npos  || arg2.find("--logging=") != string::npos ) {
			if ((arg.find("--logging=") != string::npos && arg1.find("--logging=") != string::npos) || (arg.find("--logging=") != string::npos && arg2.find("--logging=") != string::npos) || (arg1.find("--logging=") != string::npos && arg2.find("--logging=") != string::npos)) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			} else {
				if (arg.find("--logging=") != string::npos) {
					if (arg.substr(10) == "true") {
						*log = true;
					} else if (arg.substr(10) == "false") {
						*log = false;
					} else {
						throwException("Error: Wrong logging argument usage. Use --help.", *date);
					}
				} else if (arg1.find("--logging=") != string::npos) {
					if (arg1.substr(10) == "true") {
						*log = true;
					} else if (arg1.substr(10) == "false") {
						*log = false;
					} else {
						throwException("Error: Wrong logging argument usage. Use --help.", *date);
					}
				} else if (arg2.find("--logging=") != string::npos) {
					if (arg2.substr(10) == "true") {
						*log = true;
					} else if (arg2.substr(10) == "false") {
						*log = false;
					} else {
						throwException("Error: Wrong logging argument usage. Use --help.", *date);
					}
				} else {
					throwException("Error: Wrong amount of arguments. Use --help.", *date);
				}
			}
		}

		if (arg.find("--login=") != string::npos || arg1.find("--login=") != string::npos || arg2.find("--login=") != string::npos) {
			if ((arg.find("--login=") != string::npos && arg1.find("--login") != string::npos) || (arg.find("--login=") != string::npos && arg2.find("--login") != string::npos) || (arg1.find("--login=") != string::npos && arg2.find("--login") != string::npos)) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			} else {
				if (arg.find("--login=") != string::npos) {
					*login = arg.substr(8);
				} else if (arg1.find("--login=") != string::npos) {
					*login = arg1.substr(8);
				} else if (arg2.find("--login=") != string::npos) {
					*login = arg2.substr(8);
				}
			}
		}

		if (arg.find("--date=") != string::npos || arg1.find("--date=") != string::npos || arg2.find("--date=") != string::npos) {
			if ((arg.find("--date=") != string::npos && arg1.find("--date=") != string::npos) || (arg.find("--date=") != string::npos && arg2.find("--date=") != string::npos) || (arg1.find("--date=") != string::npos && arg2.find("--date=") != string::npos)) {
				throwException("Error: Wrong amount of arguments. Use --help.", *date);
			} else {
				if (arg.find("--date=") != string::npos) {
					if (arg.substr(7) == "true") {
						*date = true;
					} else if (arg.substr(7) == "false") {
						*date = false;
					} else {
						throwException("Error: Wrong date argument usage. Use --help.", *date);
					}
				} else if (arg1.find("--date=") != string::npos) {
					if (arg1.substr(7) == "true") {
						*date = true;
					} else if (arg1.substr(7) == "false") {
						*date = false;
					} else {
						throwException("Error: Wrong date argument usage. Use --help.", *date);
					}
				} else if (arg2.find("--date=") != string::npos) {
					if (arg2.substr(7) == "true") {
						*date = true;
					} else if (arg2.substr(7) == "false") {
						*date = false;
					} else {
						throwException("Error: Wrong date argument usage. Use --help.", *date);
					}
				} else {
					throwException("Error: Wrong amount of arguments. Use --help.", *date);
				}
			}
		}
	} else {
		if (!strcmp(argv[1],"--help"))
			printHelp();
	}
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

bool checkArr(string operand) {

	char *str = new char[operand.length()];
	strcpy(str, operand.c_str());

	for (unsigned int i = 0; i < strlen(str); i++) {
		if (i == 0) {
			if ((str[i] != '+' && str[i] != '-') && (str[i] < 48 && str[i] > 57)) {
				return false;
			}
		} else {
			if (str[i] < 48 || str[i] > 57) {
				return false;
			}
		}
	}

	memset(&str, '\0', sizeof str);
	return true;

}

bool checkOperand(string operand) {

	if (!checkArr(operand)) {
		return false;
	}

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

bool checkResultOverflow(string *arr) {
	long long int operand1 = convertStringToNumber(arr[0].c_str());
	long long int operand2 = convertStringToNumber(arr[2].c_str());

	if (arr[1] == "+") {
		if ((operand1 > LONG_LONG_MAX/2-1 && operand2 > LONG_LONG_MAX-operand1) || (operand2 > LONG_LONG_MAX/2-1 && operand1 > LONG_LONG_MAX - operand2)) {
			return false;
		}
	} else if (arr[1] == "*") {
/*
		//overflow
		if ((operand1 > LONG_LONG_MAX/2 -1 && operand2 >= 2) || (operand1 >= 2 && operand2 > LONG_LONG_MAX/2-1)) {
			return false;
		}

		// overflow
		if ((operand1 < LONG_LONG_MIN/2-1 && operand2 <= -2) || (operand1 <= -2 && operand2 < LONG_LONG_MIN/2-1)) {
			return false;
		}

		// underflow
		if ((operand1 < LONG_LONG_MIN/2-1 && operand2 >= 2) || (operand1 >= 2 && operand2 > LONG_LONG_MIN/2-1)) {
			return false;
		}

		if ((operand1 > LONG_LONG_MAX/2-1 && operand2 <= -2) || (operand1 <= -2 && operand2 > LONG_LONG_MAX/2-1)) {
			return false;
		}
		*/
	}



	return true;

}

bool checkAll(string *arr) {
	return (checkOperand(arr[0]) && checkOperand(arr[2]) && checkOperator(arr[1]) && checkMathValidity(arr) && checkResultOverflow(arr));
}

double getResult(string *arr) {
	long long int operand1 = convertStringToNumber(arr[0].c_str());
	long long int operand2 = convertStringToNumber(arr[2].c_str());
	double result = 0.0;

	if (arr[1] == "+") {
		result = operand1 + operand2;
	} else if (arr[1] == "-") {
		result = operand1 - operand2;
	} else if (arr[1] == "*") {
		result = operand1 * operand2;
	} else if (arr[1] == "/") {
		result = operand1 / (double) operand2;
	}



	return result;
}

string generateHello(string login) {
	return "HELLO "+md5(login)+"\n";
}

string generateResult(double result, bool error) {
	ostringstream rst;
	rst << fixed;
	rst << setprecision(10);
	rst << result;
	string substring = rst.str();

	return (error ? "RESULT ERROR\n" : "RESULT "+substring.substr(0,substring.find(".")+3)+"\n");
}
#endif //IPK2_IPK_CLIENT_H
