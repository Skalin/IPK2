all:	ipk-client.cpp clean
	g++ -std=c++0x ipk-client.cpp -o ipk-client

ipk-client: ipk-client.cpp
	g++ -std=c++0x ipk-client.cpp -o ipk-client

clean:
	rm -rf ipk-client