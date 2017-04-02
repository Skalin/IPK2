# IPK Projekt 2
##### Dominik Skála, xskala11
##### Fakulta Informačních technologií, Vysoké učení technické v Brně

##### Klient pro výpočet matematických operací



#### Překlad programu je zajištěn pomocí makefile:

| make příkazy | Operace |
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
| - -help| nápověda programu pro lepší pochopení jeho fungování|

##### **PŘÍKLAD KOMUNIKACE**

##### $ ipk-client 127.0.0.1
* Klient provede komunikaci na server na ip adrese zadané prvním parametrem. Pokud na něj nedosáhne, upozorní na to klienta. Pokud se dokomunikuje, zašle na server zprávu ve tvaru: **HELLO md5hash(xskala11)\n**. Server se poté rozhodne zaslat buď příklad pro vyřešení nebo ukončí komunikaci příkazem: **BYE md5hash\n**.
* Řešení příkladů je zařízeno zprávami: **SOLVE x [+-*/] y\n**. Pokud je zpráva zaslána ve špatném formátu (obsahuje například tři operandy, dojde k chybě a klient vrací **RESULT ERROR\n**. Pokud je zpráva zpracována bez problému, navrátí se výsledek operace na server ve formátu: **RESULT z\n**.
* Ukončení komunikace je řešeno vždy zprávou: **BYE HASHMD5\n**. Pokud je zpráva ve špatném formátu, zpráva není zpracována a je načtena další.



##### **ZNÁMÉ CHYBY**
* Aktuálně neumí klient řešit Floating point operace
* Hash loginu je forcovaný pro xskala11, není generován
