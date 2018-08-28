*** READ ME for Dylan Markovic's CS372 Project 1 Assignment ***

To run project:

step 1:
to run chatserve.py:
	enter "python3 chatserve.py <port_number>"
	enter the username for the server when prompted by program
	
step 2:
to compile chatclient.c:
	enter "gcc chatclient.c -o client"
	
step 3:
to run client:
	enter "client <hostname> <port_number>"
	enter the username for the client when prompted
	
step 4:
If connection is successful, the chat can now begin.  
Both applications will inform user that they have connected.
The client side starts the conversation

step 5: to close the connection and kill the client app, either party
can enter "\quit".  to terminate the server from running, a SIGINT is required.


Testing:
testing these to programs were done using the flip servers.
Predominantly, I used flip3 for chatserve.py and flip2 for chatclient.c.
  
The program was successful on any combination of the 3 flip servers (ie server on flip2
and client on flip1)
	

	

References (also included in each program file):
python:
# Using Computer Networking, a Top Down Approach section 2.7 for reference
# https://wiki.python.org/moin/tcpcommunication
# Used to remove trailing newlines:
#https://stackoverflow.com/questions/275018/how-can-i-remove-chomp-a-trailing-newline-in-python

C:
/***********************
References used in created the program:
Beej's Guide to Network Programming
reference for removing trailing \n's\nhttps://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
************************/