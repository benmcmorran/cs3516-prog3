cs3516-prog3
============

Program 3, Sending Photos to a Concurrent Photo Server, for WPI CS 3516 B01.

Simulates the Application,Network, Datalink and Physical layers. 
Application layer is in client.c. Proceeds to send a request to the datalink and then physical layer to 
establish a TCP connection with the server. The physical layer is simulated using a TCP connection. An 
initial packet is sent to establish this connection.It parses through the photo jpg files to send 200 
bytes of data to the network layer.The network layer appends on  packet type, length, and end of photo 
fields to the beginning of the packet and then sends it to the datalink layer. The datalink encapsulates it 
with sequence number, end of packet, length, and error detection fields. It sends it to the physical
layer which sends it to the server physical layer which sends it back up the server layers. 

The server sends a data frame ACK at the datalink layer and then a network ack at the network layer. It 
stores the payload of the packet into the assigned file based on client id. This program properly
implements timers using the select function. It waits after sending a packet on the client side and if no ACK 
is received it resends the packet. It simulates errors at the datalink layer. 