all: server client

server:
	g++ server.cpp -o server

client:
	g++ client.cpp -o client

clean:
	rm -rf server client

# lo means loopback, use tcpdump -D to show available interface
monitor:
	sudo tcpdump -XX -vv -i lo port 5000

dump:
	rm -rf dump.pcap
	sudo tcpdump -w dump.pcap -i lo port 5000

port:
	netstat -nltp
