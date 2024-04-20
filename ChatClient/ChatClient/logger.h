#pragma once

#define LOGGER_PATH "LogInfo.txt"

FILE* logger_file = NULL;

// Opening logger file with type ('w','r','a+')
BOOL open_logger_file(const char* type) {
	logger_file = fopen(LOGGER_PATH, type);
	return logger_file == NULL ? FALSE : TRUE; // Return open process type
}

// Closing logger file
void close_logger_file() {
	if (logger_file == NULL) return;
	fclose(logger_file);
}

// Logging message to file
void logger_log(const char* message) {
	if (logger_file == NULL) return;
	fprintf(logger_file, "%s\n", message);
	fflush(logger_file);
}

// Logging messages to file
void logger_loga(const char* message, const char* arg) {
	if (logger_file == NULL)return;
	fprintf(logger_file, "%s%s\n", message, arg);
	fflush(logger_file);
}

// Printing logger file content
void read_logger_file() {
	if (logger_file == NULL) return;

	rewind(logger_file); // Return pointer to 0 location

	char buffer[1024];
	// Read whole file and print in console
	while (fgets(buffer, sizeof(buffer), logger_file) != NULL) {
		printf("%s", buffer);
	}
}