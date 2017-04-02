all:	 clean ipk-client clean-obj

ipk-client: ipk-client.cpp
	g++ -std=c++0x -c ipk-client.cpp
	g++ -std=c++0x ipk-client.o -o ipk-client

clean-obj:
	rm -rf *.o

clean:
	rm -rf ipk-client