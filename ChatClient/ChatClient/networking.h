#pragma once

#define PORT 8080	// Server port
#define HOST "127.0.0.1"	// Server addr (localhost)

// Network buffer struct for holding received/sent buffers
struct NetworkBuffer {
	char data[BUFFER_SIZE]; // Send/Receive buffer data
	unsigned int bytes_received; // Count of bytes sent/received
};

// Typedefinition for unsigned long
typedef unsigned long status_t;

#define SUCCESS 0L // Constant for success
#define SERVER_ERROR 1L // Constant for server error
#define INVALID_ARGUMENT_ERROR 2L // Constant for invalid argument error
#define UNDEFINED_ERROR 3L // Constant for undefined error

// Definition for message reading 
status_t connection_read_message(SOCKET sock, struct NetworkBuffer* buffer);
// Definition for message sending
status_t connection_send_message(SOCKET sock, const char* message);

// Definition for receiving process
void process_message_receiving();
// Definition for sending process
void process_message_sending();

// Struct for holding connection hosts
struct Connection {
	WSADATA wsa; // WSADATA holding struct
	SOCKET sock; // SOCKET holding struct
	struct sockaddr_in serv_addr; // Addr holding struct
}connection; // Main connection

status_t connection_connect() {
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &connection.wsa) != 0)
		return SERVER_ERROR;

	// Create socket
	if ((connection.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return SERVER_ERROR;

	// Set server address
	connection.serv_addr.sin_family = AF_INET;
	connection.serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, HOST, &connection.serv_addr.sin_addr) <= 0)
		return SERVER_ERROR;

	// Connect
	if (connect(connection.sock, (struct sockaddr*)&connection.serv_addr, sizeof(connection.serv_addr)) < 0)
		return SERVER_ERROR;

	return SUCCESS;
}
// Cleaning up connection
void connection_cleanup() {
	closesocket(connection.sock);
	WSACleanup();
}

status_t connection_read_message(SOCKET sock, struct NetworkBuffer* buffer) {
	if (buffer == NULL)return INVALID_ARGUMENT_ERROR; // Throw invalid argument error

	// Receiving buffer from server
	buffer->bytes_received = recv(sock, buffer->data, BUFFER_SIZE, 0);
	// Setting end of string
	buffer->data[buffer->bytes_received] = '\0';

	// logger_log("Message received from server: ", buffer);

	// Return status
	return buffer->bytes_received == 0 ? SERVER_ERROR : (buffer->bytes_received > 0 ? SUCCESS : UNDEFINED_ERROR);
}
status_t connection_send_message(SOCKET sock, const char* message) {
	// Sent message buffer length
	int bytes_sent = send(sock, message, strlen(message), 0);
	
	// logger_log("Message sent to server: ", message);
	
	// Return status
	return bytes_sent == 0 ? SERVER_ERROR : (bytes_sent > 0 ? SUCCESS : UNDEFINED_ERROR);
}

// Connection status
BOOL connected = TRUE;

void process_message_receiving() {
	struct NetworkBuffer buffer; // Buffer storage
	struct Message message; // Message storage

	while (connected) {
		// Receive time message data
		if (connection_read_message(connection.sock, &buffer) == SERVER_ERROR) {
			connected = FALSE;
			break;
		}

		// Receive username message data
		strcpy(message.time, buffer.data);
		if (connection_read_message(connection.sock, &buffer) == SERVER_ERROR) {
			connected = FALSE;
			break;
		}

		// Receive data message data
		strcpy(message.username, buffer.data);
		if (connection_read_message(connection.sock, &buffer) == SERVER_ERROR) {
			connected = FALSE;
			break;
		}
		strcpy(message.data, buffer.data);
		// Add message to dynamicly allocated array
		messages_add(message.data, message.username, message.time);

		print_console(); // Print console
	}
}
void process_message_sending() {
	struct NetworkBuffer buffer; // Buffer storage

	while (connected) {
		fflush(stdout); // Flush input

		fgets(buffer.data, BUFFER_SIZE, stdin); // Get console input buffer
		if (buffer.data[0] == '#') {
			size_t len = strlen(buffer.data); // Get length of data buffer
			if (len > 0 && buffer.data[len - 1] == '\n') {
				buffer.data[len - 1] = '\0'; // Replace \n with \0
			}

			// TODO : Execute commands
			if (strcmp(buffer.data, "#clear") == 0) {
				messages_clear();
			}
			else if (strcmp(buffer.data, "#debug") == 0) {
				logger_debuged = logger_debuged == TRUE ? FALSE : TRUE;
			}
			else if (strcmp(buffer.data, "#exit") == 0) {
				connected = FALSE;
				break;
			}

			print_console();
		}
		else {
			// Send message to server
			if (connection_send_message(connection.sock, buffer.data) == SERVER_ERROR) {
				connected = FALSE; // Close connection if sending failed
				break;
			}
		}

		fflush(stdout); // Flush console input buffer
	}
}