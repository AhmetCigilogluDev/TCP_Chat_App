📌 Project Overview
This project is a multi-client chat application developed in C using TCP sockets. The system allows multiple clients to connect to a server and exchange messages in real-time. The server manages incoming connections and relays messages between clients.

This project was created as part of a Computer Networks course, focusing on socket programming, concurrency, and real-time communication.

🛠️ Technologies Used
Programming Language: C
Networking: TCP/IP (Berkeley Sockets)
Concurrency: pthread (POSIX Threads)
I/O Handling: select() for multiple client management
📂 Features
✔ Multi-Client Support – Multiple clients can connect and chat simultaneously.
✔ Server-Client Architecture – Centralized server manages all communication.
✔ Non-Blocking I/O – Uses select() for handling multiple clients efficiently.
✔ Threaded Server – Uses pthread for concurrent client handling.
✔ Basic Command Handling – Clients can send and receive messages in real-time.

🚀 Installation & Setup
Prerequisites
Before running this project, ensure you have:

A Linux-based system (or WSL for Windows)
GCC compiler installed (sudo apt install gcc)
