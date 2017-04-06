all:	 clean ipk-client clean-obj

ipk-client: ipk-client.cpp
	g++ -std=c++0x -Wall -Werror -c ipk-client.cpp
	g++ -std=c++0x -Wall -Werror ipk-client.o -o ipk-client

clean-obj:
	rm -rf *.o

clean:
	rm -rf ipk-client
