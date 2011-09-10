#
# Makefile for UDP Echo program
#
# Type make to compile all the programs
#
all: UDPEchoServer UDPEchoClient

clean:
	rm -f UDPEchoServer UDPEchoClient

UDPEchoServer: UDPEchoServer.c
	cc -o $@ $^ -Wall -O2

UDPEchoClient: UDPEchoClient.c
	cc -o $@ $^ -Wall -O2
