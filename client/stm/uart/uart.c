/**
 * @file 	uart.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Implementation of UART communication functions.
 *
 * @copyright Copyright (c) 2024
 */

#include "uart.h"

/**
 * @brief Initializes UART communication on ttyO1.
 *
 * This function opens the specified UART device file (/dev/ttyO1) and configures the communication settings.
 *
 * @param fd Pointer to store the file descriptor for the opened UART device.
 * @param opt Pointer to a termios structure for configuring UART communication settings.
 */
void init_uart1(int *fd, void *opt)
{
    struct termios *options = (struct termios *)(opt);

    *fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (*fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyO1 - ");
        // TODO: handle to error
    }

    tcgetattr(*fd, options);
    cfsetispeed(options, B115200);
    cfsetospeed(options, B115200);
    options->c_cflag |= (CLOCAL | CREAD);
    options->c_cflag &= ~PARENB;
    options->c_cflag &= ~CSTOPB;
    options->c_cflag &= ~CSIZE;
    options->c_cflag |= CS8;
    options->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options->c_iflag &= ~(IXON | IXOFF | IXANY);
    options->c_oflag &= ~OPOST;
    // options->c_cc[VMIN] = 0;
    // options->c_cc[VTIME] = 10;
    tcsetattr(*fd, TCSANOW, options);
}

/**
 * @brief Initializes UART communication on ttyO4.
 *
 * This function opens the specified UART device file (/dev/ttyO4) and configures the communication settings.
 *
 * @param fd Pointer to store the file descriptor for the opened UART device.
 * @param opt Pointer to a termios structure for configuring UART communication settings.
 */
void init_uart4(int *fd, void *opt)
{
    struct termios *options = (struct termios *)(opt);

    *fd = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);
    if (*fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyO4 - ");
        // TODO: handle to error
    }

    tcgetattr(*fd, options);
    cfsetispeed(options, B115200);
    cfsetospeed(options, B115200);
    options->c_cflag |= (CLOCAL | CREAD);
    options->c_cflag &= ~PARENB;
    options->c_cflag &= ~CSTOPB;
    options->c_cflag &= ~CSIZE;
    options->c_cflag |= CS8;
    options->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options->c_iflag &= ~(IXON | IXOFF | IXANY);
    options->c_oflag &= ~OPOST;
    // options->c_cc[VMIN] = 0;
    // options->c_cc[VTIME] = 10;
    tcsetattr(*fd, TCSANOW, options);
}