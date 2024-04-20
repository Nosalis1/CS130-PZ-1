#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

BOOL logger_debuged = FALSE;
void logger_log(const char* message);
void read_logger_file();

#include "message_system.h"
#include "smart_console.h"
#include "networking.h"
#include "logger.h"

#define OFFLINE
#undef OFFLINE

int main() {
	// Open file for logging events
	if (open_logger_file("a+") == FALSE) {
		printf("Could't open logger file!\n");
		return 1;
	}

	logger_log("File is opened for writing!");
#ifndef OFFLINE
	char username[BUFFER_SIZE];

	// Entering username for chat
	printf("Enter username: ");
	fgets(username, BUFFER_SIZE, stdin);
	username[strcspn(username, "\n")] = '\0'; // Clearing \n from end of name

	logger_loga("Username entered: ", username);

	// Dynamicly allocating messages
	messages = malloc(sizeof(struct Message) * MAX_MESSAGES_DISPLAYED);

	logger_log("Message memory allocated!");

	// Connecting to the server
	if (connection_connect() != SUCCESS) {
		connection_cleanup();
		logger_log("Failed to connect to server!");
		free(messages);
		close_logger_file();
		return 0;
	}
	logger_log("Connected to server!");

	// Sending username message to server
	connection_send_message(connection.sock, username);

	logger_log("Sent username message to server!");

	// Printing console design
	print_console();

	// Creating HANDLE(thread) for listening messages from server
	HANDLE receive_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)process_message_receiving, NULL, 0, NULL);
	// Handling message I/O system on main thread
	process_message_sending();

	logger_log("Cleaning up!");

	// Cleaning connection
	connection_cleanup();

	logger_log("Cleaning message memory!");

	// Cleaning dynamicly allocated memory
	free(messages);
#endif // !OFFLINE
	// Closing logging file
	close_logger_file();
	return 0;
}