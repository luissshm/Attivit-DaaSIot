# Attivita-DaaSIot

This repository contains a simple peer-to-peer chat implemented using the DaasAPI communication library. It demonstrates how to send and receive messages between two nodes (Nodo A and Nodo B) over an IPv4 link, using the DAAS API.

Features
--------
- Two independent programs: Nodo A and Nodo B.
- Bidirectional real-time text messaging over loopback or network.
- Uses DaasAPI to:
  - Initialize DAAS cores.
  - Enable IPv4 drivers.
  - Map remote nodes and establish connections.
- Threaded design:
  - One thread for sending messages.
  - One thread for receiving messages.
- Safe console I/O with mutex locking.

How it works
------------
Each node:
1. Initializes its own DAAS core with a SID (System ID) and DIN (Device ID).
2. Enables a driver to listen on a specific IP and port.
3. Maps the remote node's DIN to its IP:port.
4. Starts two threads:
   - Receiver thread -> Calls node.pull() to retrieve incoming messages.
   - Sender thread   -> Reads user input and sends messages via node.push().

When you type "exit", the node stops both threads and terminates.

Diagram
-------------
       +----------------+                  +----------------+
       |                |   TCP/IPv4 Link  |                |
       |   Nodo A       |<---------------->|    Nodo B      |
       |                |                  |                |
       | DIN = 110      |                  | DIN = 200      |
       | Port = 2020    |                  | Port = 2024    |
       +----------------+                  +----------------+

Messages can be sent in both directions simultaneously.

Building
--------
Make sure you have the DaasAPI headers and library available.

To build both nodes:
```shell 
   make 
```

This will create two executables:\
    nodo_a\
    nodo_b

To clean up:
```shell
    make clean
```

Running locally (loopback)
--------------------------
Open two terminals.

Terminal 1 - Start Nodo A:
```shell
    ./nodo_a
```

Terminal 2 - Start Nodo B:
```shell
    ./nodo_b
```

Example session:
----------------
### Terminal 1 (Nodo A)
```shell
Type a message ('exit' to quit): Hello from A
[Sent] Hello from A
```
```shell
[Received] Hi A, this is B
Type a message ('exit' to quit): exit
```
### Terminal 2 (Nodo B)
```shell
[Received] Hello from A
Type a message ('exit' to quit): Hi A, this is B
[Sent] Hi A, this is B
```

Running over the network
------------------------
Edit runNodoA() and runNodoB() calls to use the correct IP addresses:

Example:
```cpp
    // A = 192.168.1.10, B = 192.168.1.20
    runNodoA("192.168.1.10", "192.168.1.20");
    runNodoB("192.168.1.20", "192.168.1.10");
```

Then rebuild and run each node on different machines.

Notes
-----
- localDin and remoteDin must be unique per node.
- The ports (2020 and 2024) are hardcoded; adjust if needed.

Known Issue
---
At program termination, you may encounter the following runtime error:
```shell
free(): invalid pointer
[1] <pid> IOT instruction ./nodo_a
```
This error is caused by improper memory deallocation during shutdown. This is all I know so far...
