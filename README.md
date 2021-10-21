# Distributed-System
README

In this project, I implemented a simple file upload and download service based on message-

oriented, client-server communication and a computation service using remote procedure call

(RPC) based communication. The file server supports four basic operations: UPLOAD,

DOWNLOAD, DELETE, and RENAME. We assume that the file service is implemented using

a connection-oriented protocol, in which the client and server first establish a network

connection, negotiate the operation to be performed, and carry out the file transfer through the

same connection.

HOW TO RUN CODE

In all 4 program run the server file first and then client file in different terminal

window.

Single Client Single Server:

• **cd into location of assignment 1 (where client and server located)**

• **gcc server.c -o server**

• **./server**

• **Open another terminal**

• **gcc client.c -o client**

• **./client**

• **Program will run.**





Multiple Client Single Server:

• **cd into location of assignment 1 (where client and server located)**

• **gcc server.c -o server -pthread**

• **./server**

• **Open another terminal (you can open another terminal to check the program)**

• **gcc client.c -o client**

• **./client**

• **Program will run.**





Primitive RPC without Library Function:

\*\* remember that for matrix part in this code please follow print out command in code (check

screenshots)

• **cd into location of assignment 1 (where client and server located)**

• **gcc server.c -o server**

• **./server**

• **Open another terminal**

• **gcc client.c -o client**

• **./client**

• **Program will run.**









Primitive Asynchronous and Deferred RPC without Library Function:

• **cd into location of assignment 1 (where client and server located)**

• **gcc server.c -o server**

• **./server**

• **Open another terminal**

• **gcc client.c -o client**

• **./client**

• **Program will run.**

