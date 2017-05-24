# IPK Project 2
##### Dominik Skála, xskala11
##### Fakulta Informačních technologií, Vysoké učení technické v Brně

##### Client for solving mathematical operations

The client is receiving math operations from server and returning results back to server

All operations are only integer type operations, FP ops are currently not supported.
Client can solve any operation from range +,-,*,/.
It can solve them with numbers that can be saved on 64 bits.

The program itself works on a simple base. Client sends request message to the server specified in the first required argument, which runs on port 55555, containing HELLO md5hash(xskala11)\\n, which is resolved by server.
After that, server is responding with mathematical equations, which must be in format SOLVE [num1] [op] [num2]\\n. The spaces between operators and operand are \fB\fIREQUIRED\fR!
If the operation is successfully resolved, client returns the result in the message with format: RESULT [result]\\n, if any error occurs, client is responding with RESULT ERROR\\n. The result is a \fIdecimal number\fR in format \fB\fIXn.XX\fR, where \fB\fIX\fR stands for any number from range \fB0 - 9\fR and \fB\fIXn\fR stands for any amount of numbers from range \fB0 - 9\fR.
This can take any number of iterations. Only the server can stop the process.
.TP
If the server wants to stop, it has to send message in format BYE [hash]\n, the hash is then printed to the console. The hash might not be the same as the one that the client sent. It also might be in the unknown format, which means, that the hash sent from client could not be resolved.



#### Compilation using makefile:

| make cmd | operation |
| :-------------------------- | --------:|
| **make** | překlad klienta **ipk-client**, před překladem se smažou veškeré předchozí verze programu |
| **make all** | alias pro příkaz **make** |
| **make ipk-client** | překlad serveru, spustitelný jako **ipk-client** |
| **make clean-obj** | dojde ke smazání všech zastaralých objektů |
| **make clean** | dojde ke smazání veškerých zastaralých kopií |

## Klient **ipk-client**
##### $ **ipk-client IP**
##### $ **ipk-client --help**

| Argument | Popis |
|:-----:| :-----:|
| IP | IP adresa ve formátu IPv4, IPv6 nebo hostname |
| --help| nápověda programu pro lepší pochopení jeho fungování|

##### **PŘÍKLAD KOMUNIKACE**

##### $ ipk-client 127.0.0.1
* Klient provede komunikaci na server na ip adrese zadané prvním parametrem. Pokud na něj nedosáhne, upozorní na to klienta. Pokud se dokomunikuje, zašle na server zprávu ve tvaru: **HELLO md5hash(xskala11)\n**. Server se poté rozhodne zaslat buď příklad pro vyřešení nebo ukončí komunikaci příkazem: **BYE md5hash\n**.
* Řešení příkladů je zařízeno zprávami: **SOLVE x [+-*/] y\n**. Pokud je zpráva zaslána ve špatném formátu (obsahuje například tři operandy, dojde k chybě a klient vrací **RESULT ERROR\n**. Pokud je zpráva zpracována bez problému, navrátí se výsledek operace na server ve formátu: **RESULT z\n**.
* Ukončení komunikace je řešeno vždy zprávou: **BYE HASHMD5\n**. Pokud je zpráva ve špatném formátu, zpráva není zpracována a je načtena další.



##### **ZNÁMÉ CHYBY**
* Aktuálně neumí klient řešit Floating point operace
* Hash loginu je forcovaný pro xskala11, není generován

##### Contact
* Dominik Skála <xskala11@stud.fit.vutbr.cz>
