#pragma once

#define MAX_MESSAGES_DISPLAYED 10
#define BUFFER_SIZE 1024

// Message struct holding informations about message
struct Message {
	char data[BUFFER_SIZE]; // Actual message data
	char username[BUFFER_SIZE]; // Username of sender
	char time[BUFFER_SIZE]; // Timestamp of message sending
};
struct Message* messages; // Dynamicly allocated messages array
int messages_count = 0; // Count of current messages

// Function to clear messages from dynamic allocation
void messages_clear() {
	for (int i = 0; i < MAX_MESSAGES_DISPLAYED; i++) {
		messages[i].data[0] = '\0';
	}
	messages_count = 0;
}

// Adding new message to dynamicly allocated array of messages
void messages_add(char* data, char* username, char* time) {
	if (messages_count < MAX_MESSAGES_DISPLAYED) {
		// Copy message on next message index
		strcpy(messages[messages_count].data, data);
		strcpy(messages[messages_count].username, username);
		strcpy(messages[messages_count].time, time);
		messages_count++; // Increase counter
	}
	else {
		// Move all elements one place left
		for (int i = 1; i < MAX_MESSAGES_DISPLAYED; i++) {
			strcpy(messages[i - 1].data, messages[i].data);
			strcpy(messages[i - 1].username, messages[i].username);
			strcpy(messages[i - 1].time, messages[i].time);
		}
		// Copy message on the last position
		strcpy(messages[MAX_MESSAGES_DISPLAYED - 1].data, data);
		strcpy(messages[MAX_MESSAGES_DISPLAYED - 1].username, username);
		strcpy(messages[MAX_MESSAGES_DISPLAYED - 1].time, time);
	}
}