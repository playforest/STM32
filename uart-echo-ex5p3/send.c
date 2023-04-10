#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <baudrate>\n", argv[0]);
        return 1;
    }

    printf("Opening file %s\n", argv[1]);
    fflush(stdout);

    // Open the input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("fopen");
        return 1;
    } else {
        printf("Opened file successfully\n");
        fflush(stdout);
    }

    // Open the serial port
    int fd = open("/dev/cu.usbmodem1103", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("open");
        return 1;
    } else {
        printf("/dev/cu.usbmodem1103 found. Connecting...\n");
        fflush(stdout);
        sleep(5);
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

    // Set the baud rate
    int baudrate = atoi(argv[2]);
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);

    // Apply the new serial port settings
    if (tcsetattr(fd, TCSANOW, &options) == -1) {
        perror("tcsetattr");
        return 1;
    } else {
        printf("Successfully connected to serial port: %s\n", argv[1]);
        printf("Baud rate: %d\n", baudrate);
        fflush(stdout);
    }

    // Send the contents of the input file to the serial port
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        if (write(fd, buffer, bytes_read) != bytes_read) {
            perror("write");
            return 1;
        }
    }

    // Close the input file and the serial port
    fclose(input);
    close(fd);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <termios.h>
// #include <errno.h>

// #define BUFFER_SIZE 1024

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <file>\n", argv[0]);
//         return 1;
//     }

//     printf("opening file\n");
//     fflush(stdout);

//     // Open the input file
//     FILE *input = fopen(argv[1], "r");
//     if (input == NULL) {
//         perror("fopen");
//         return 1;
//     } else {
//         printf("opened file successfully\n");
//         fflush(stdout);
//     }

//     printf("before open");
//     fflush(stdout);
//     // Open the serial port
//     int fd = open("/dev/cu.usbmodem1103", O_RDWR | O_NOCTTY);
//     printf("after open");
//     fflush(stdout);
//     if (fd == -1) {
//         printf("fd-1 branch");
//         fflush(stdout);
//         if (errno == ENOENT) {
//             printf("Serial port not found: /dev/tty.usbmodem21203\n");
//         } else {
//             perror("open");
//         }
//         return 1;
//     } else {
//         printf("/dev/tty.usbmodem21203 found. connecting.\n");
//         fflush(stdout);
//         sleep(5);
//     }
//         // Get the current serial port settings
//     struct termios options;
//     tcgetattr(fd, &options);


//     // Set the serial port settings
//     options.c_cflag |= CLOCAL | CREAD;   // Enable the receiver and set local mode
//     options.c_cflag &= ~CSIZE;           // Clear the data size bits
//     options.c_cflag |= CS8;              // Set 8 data bits
//     options.c_cflag &= ~PARENB;          // Disable parity
//     options.c_cflag &= ~CSTOPB;          // Set one stop bit
//     options.c_cflag &= ~CRTSCTS;         // Disable flow control
//     cfsetispeed(&options, B115200);  
//     options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    // Set raw input mode
//     options.c_oflag &= ~OPOST;                            // Set raw output mode

//     options.c_cc[VTIME] = 0;     // Set timeout in tenths of a second
//     options.c_cc[VMIN] = 1;      // Set minimum number of characters to read

//     printf("Serial port settings:\n");
//     printf("  Baud rate: %d\n", cfgetispeed(&options));
//     printf("  Data bits: %d\n", (options.c_cflag & CSIZE));
//     printf("  Parity: %s\n", (options.c_cflag & PARENB) ? "enabled" : "disabled");
//     printf("  Stop bits: %d\n", (options.c_cflag & CSTOPB) ? 2 : 1);
//     printf("  Flow control: %s\n", (options.c_cflag & CRTSCTS) ? "enabled" : "disabled");

//     // Apply the new serial port settings
//     if (tcsetattr(fd, TCSANOW, &options) == -1) {
//         perror("tcsetattr");
//         return 1;
//     } else {
//         printf("Successfully connected to serial port: %s\n", argv[1]);
//         fflush(stdout);
//     }

//     // Send the contents of the input file to the serial port
//     char buffer[BUFFER_SIZE];
//     size_t bytes_read;

//     while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
//         printf("Sending text: %s", buffer);
//         fflush(stdout);
//         if (write(fd, buffer, bytes_read) != bytes_read) {
//             perror("write");
//             return 1;
//         }
//     }

//     // Close the input file and the serial port
//     fclose(input);
//     close(fd);

//     return 0;
// }
