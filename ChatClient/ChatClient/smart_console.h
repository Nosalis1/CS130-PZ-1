#pragma once

void console_clear() {
	printf("\033[2J"); // Clear console buffer
	printf("\033[H"); // Reset cursor position
}

// Function to print separator lines
void console_separator() {
	for (int i = 0; i < 50; i++)printf("-");
	printf("\n");
}

// Function to move cursor position
void console_move_cursor(int x, int y) {
	printf("\033[%d;%dH", y, x);
}

// Function to center label in console
void console_print_centered(const char* text) {
	int length = strlen(text);
	int padding = (50 - length) / 2;
	for (size_t i = 0; i < padding; i++)
		printf(" ");
	printf("%s\n", text);
}

// Definitions of console colors
#define CONSOLE_COLOR_RED 31
#define CONSOLE_COLOR_GREEN 32
#define CONSOLE_COLOR_YELLOW 33
#define CONSOLE_COLOR_BLUE 34
#define CONSOLE_COLOR_MAGENTA 35
#define CONSOLE_COLOR_CYAN 36
#define CONSOLE_COLOR_WHITE 37

// Function to print colored text
void console_print_colored(char* text, int color) {
	printf("\033[%dm%s\033[0m", color, text);
}

// Print console design
void print_console() {
	console_clear();

	console_move_cursor(0, 0);
	console_print_centered("Smart Console");
	console_separator();

	for (int i = 0; i < MAX_MESSAGES_DISPLAYED; i++) {
		if (i >= messages_count) {
			printf("\n");
			continue;
		}
		console_print_colored(messages[i].time, CONSOLE_COLOR_GREEN);
		printf(" # ");
		console_print_colored(messages[i].username, CONSOLE_COLOR_BLUE);
		printf(": ");
		printf("%s", messages[i].data);
	}

	console_separator();
	if (logger_debuged == TRUE) read_logger_file();
	console_separator();
	console_print_colored("Input: ", CONSOLE_COLOR_YELLOW);
	printf("\n");
}