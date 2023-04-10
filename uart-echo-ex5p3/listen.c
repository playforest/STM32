#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // Open the output file
    FILE *output = fopen(argv[1], "w");
    if (output == NULL) {
        perror("fopen");
        return 1;
    }

    // Open the serial port
    int fd = open("/dev/tty.usbmodem1103", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Get the current serial port settings
    struct termios options;
    tcgetattr(fd, &options);

    // Set the serial port settings
    options.c_cflag |= CLOCAL | CREAD;   // Enable the receiver and set local mode
    options.c_cflag &= ~CSIZE;           // Clear the data size bits
    options.c_cflag |= CS8;              // Set 8 data bits
    options.c_cflag &= ~PARENB;          // Disable parity
    options.c_cflag &= ~CSTOPB;          // Set one stop bit
    options.c_cflag &= ~CRTSCTS;         // Disable flow control

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    // Set raw input mode
    options.c_oflag &= ~OPOST;                            // Set raw output mode

    options.c_cc[VTIME] = 0;     // Set timeout in tenths of a second
    options.c_cc[VMIN] = 1;      // Set minimum number of characters to read

    // Apply the new serial port settings
    if (tcsetattr(fd, TCSANOW, &options) == -1) {
        perror("tcsetattr");
        return 1;
    } else {
        printf("Successfully connected to serial port: %s\n", argv[1]);
    }

    // Receive data from the serial port and write it to the output file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (fwrite(buffer, 1, bytes_read, output) != bytes_read) {
            perror("fwrite");
            return 1;
        }
    }

    // Close the output file and the serial port
    fclose(output);
    close(fd);

    return 0;
}