#------------------------------------
# Name: Dylan Markovic
# Class: CS372
# Assignment: Project 1
# Due Date: 2.11.2018
#------------------------------------

import sys
from socket import *
MESSAGE_LENGTH = 500
HANDLE_LENGTH = 10
#--------------------------
# checkArgs makes sure the client inputs the correct number of arguments
# 	to run the server program
# checkArgs also makes sure that the input is an integer that falls in the
#	range of valid, non-reserved port numbers
#--------------------------
def checkArgs():
	if len(sys.argv) != 2:
		print("invalid argument count for chatserve.py")
		print("Correct Input: python3 " + sys.argv[0] + " <port number>");
		sys.exit()
	if int(sys.argv[1]) < 1024 or int(sys.argv[1]) > 65535:
		print("Port Number does not fall into valid range")
		print("Acceptable Port Number Range is [1024, 65535]")
		sys.exit()
	
#-----------------------------
# getServerHandle is a simple function
#  that prompts the user to input their nameExchange
#  It will continue to ask user for a name until it is
#  between 1 and 10 characters long
#-----------------------------------	
def getServerHandle():
	serverHandle = ""
	while len(serverHandle) < 1 or len(serverHandle) > 10:
		serverHandle = input("Enter the handle for the server (must be 1 to 10 characters long):")
	return serverHandle
	
#--------------------------------------------
# startSocket creates a socket with the port number
#  included in the command line used to begin the program
#  after it is created, the socket listens and informs user
#  what port it is listening on
#-------------------------------------------------
def startSocket():
	serverPort = int(sys.argv[1])
	serverSocket = socket(AF_INET, SOCK_STREAM)
	serverSocket.bind(('',serverPort))
	serverSocket.listen(1)
	print("Server is listening on port: " + sys.argv[1])
	return serverSocket

#-----------------------------------------------------
# nameExchange is the first phase of the chatroom
# the server first receives the client's handle
# and it then sends the server's handle
# It requires the connectionSocket and the server's handle
# be passed as arguments.
#--------------------------------------------------
def nameExchange(connectionSocket, serverHandle):
	clientHandle = connectionSocket.recv(HANDLE_LENGTH).decode('UTF-8')
	print("Began chat with {}".format(clientHandle))
	connectionSocket.send(serverHandle.encode())
	return clientHandle
	
#-------------------------------------------------
# getClientMessage is responsible for receiving messages
#  from the client during the chat.  It receives, decodes the
#  message into a string, and then removes any trailing '\n'
# It requires the connection socket and client's handle to
#   be passed as arguments.  It returns the formatted message
#-----------------------------------------------------
def getClientMessage(client, clientHandle):
	message = client.recv(MESSAGE_LENGTH).decode('UTF-8').rstrip()
	return message
	
#--------------------------------------------------
# sendServerMessage is responsible for sending messages written on
# this side of the chat.  It appends the server's handle and '> '
# to the message to format it correctly.  It then checks if '\quit'
#  was the message, and sends it when this is not the case.
# It needs the connection socket and server's handle as arguments,
#  and returns the formatted message
#-------------------------------------------------------
def sendServerMessage(client, serverHandle):
	prefix = serverHandle + "> "
	message2 = input(prefix)
	deliverThis = prefix + message2
	if(message2 != "\quit"):
		client.send(deliverThis.encode())
	return deliverThis

#--------------------------------------------------------
# chat uses the sendServerMessage and getClientMessage functions
#  to create the entire chat experience.  It closes the connection
#  when the received message is blank or if the server side message is 
#  '\quit'.  It also prints the messages to the console and informs the
#  user when the connection is closed.
# It requires the connection socket, the client's handle, and the 
#  server's handle be passed in as arguments
#-----------------------------------------------------------
def chat(client, clientHandle, serverHandle):
	while True:
		clientMessage = getClientMessage(client, clientHandle)
		if (clientMessage == ""):
			print("Client has closed connection")
			print("Server is still listening on port " + sys.argv[1])
			break
		print("{}> {}".format(clientHandle, clientMessage));
		serverMessage = sendServerMessage(client, serverHandle)
		if (serverMessage == serverHandle + "> \quit"):
			print("Connection terminated");
			print("Server is still listening on port " + sys.argv[1])
			break
	client.close()
	


	
#-------------------------------------------------
# main drives the whole program.  It calls all necessary
#  functions to create the server side chatroom
#-----------------------------------------------
def main():
	checkArgs()  # check command line arguments
	serverHandle = getServerHandle() # get server's handle
	serverSocket = startSocket() #start the socket
	
	while True:
		connectionSocket, addr = serverSocket.accept()  #accept requested connections
		clientHandle = nameExchange(connectionSocket, serverHandle)  #swap handles
		chat(connectionSocket, clientHandle, serverHandle)  #start the chat
		

		
	
main()

#---------------------------------------
# Using Computer Networking, a Top Down Approach section 2.7 for reference
# https://wiki.python.org/moin/tcpcommunication
# Used to remove trailing newlines:
#https://stackoverflow.com/questions/275018/how-can-i-remove-chomp-a-trailing-newline-in-python