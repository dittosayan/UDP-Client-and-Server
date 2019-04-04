# UDP-Client-and-Server

The client application should be executed using the following command:
./client <ServerIP> <ServerPort> <P> <TTL> <NumPackets>
  

The server application should be executed using the following command:
./server <ServerPort>

Here NumPackets could be anything between 1 to 50 and for each packet, and
corresponding sequence number should be generated.

When server receives a datagram from client, it immediately decrements the TTL value
by one in the datagram and sends the same datagram (with the new TTL) back to client.

On reception of the packets at client back from the server, calculate the Round Trip Time
(RTT) delay for each packet and calculate the average RTT delay across the packets.
