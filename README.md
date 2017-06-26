# IPK Project 2
##### Dominik Skála, xskala11
##### Fakulta Informačních technologií, Vysoké učení technické v Brně
##### Results: 20 / 20 points

##### Client for solving mathematical operations

The client is receiving math operations from server and returning results back to server

All operations are only integer type operations, FP operations are currently not supported.
Client can solve any operation from range **+,-,*,/**.
It can solve them with **numbers** that can be saved on **64 bits**.

The program itself works on a simple base. Client sends request message to the server specified in the first required argument, which **runs** on port **55555**, containing **HELLO md5hash(LOGIN)\\n**, which is resolved by server.
After that, server is responding with mathematical equations, which must be in format **SOLVE [num1] [op] [num2]\\n**. The spaces between operators and operand are \fB\fIREQUIRED\fR!
If the operation is successfully resolved, client returns the result in the message with format: **RESULT [result]\\n**, if any error occurs, client is responding with **RESULT ERROR\\n**. The result is a **decimal number** in format **Xn.XX**, where **X** stands for any number from range **0 - 9** and **Xn** stands for any amount of numbers from range **0 - 9**.
This can take any number of iterations. Only the server can stop the process.
.TP
If the server wants to stop, it has to send message in format **BYE [hash]\n**, the hash is then printed to the console. The hash might not be the same as the one that the client sent. It also might be in the unknown format, which means, that the hash sent from client could not be resolved.



#### Compilation using makefile:

| make cmd | operation |
| :-------------------------- | --------:|
| **make** | compilation of the client **ipk-client**, before the compilation, all older versions are deleted |
| **make all** | alias for **make** |
| **make ipk-client** | compilation of the client runnable as **ipk-client** |
| **make clean-obj** | all binary files and object files will be deleted for clean remake |
| **make clean** | all versions of program will be deleted |

## Client **ipk-client**
##### $ **ipk-client IP** -- runs the client which is requesting the math operations from the server on the IP address given in the argument
##### $ **ipk-client --help** -- show the help for this program
##### $ **ipk-client --logging=BOOL** -- enables logging
##### $ **ipk-client --date=BOOL** -- enables showing date in logs
##### $ **ipk-client --login=XLOGIN99** -- runs the program for the user with this login, which is then hashed using md5, if no login is given default is used (default is xskala11)

## Bugs
No other operations than +,-,*,/ are working but the system is ready to be upgraded to solve other operations for example modulo, etc.
FP operation are not supported but could be using a simple reimplementation

##### Contact
* Dominik Skála <xskala11@stud.fit.vutbr.cz>
