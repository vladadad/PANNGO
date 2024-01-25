/**
 * @file    client.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief   Implementation of functions related to the client application.
 */

#include "client.h"

/**
 * @brief Start-End function to control the application.
 *
 * This function continuously writes "Hi" to a file descriptor until the exit_app flag is set to 1.
 * It is intended for controlling the start and end of the application.
 *
 * Sends message via BBB UART_4, created for using the phisical button.
 * When the button is pressed the "Hi" message sent from UART_4 RX is received in UART_4 TX.
 * Thus starts/ends the communication with the server.
 * 
 * @param arg A pointer to the file descriptor (int).
 */
void *start_end_func(void *arg)
{
    int fd = *((int *)arg);
    /*  */
    while (exit_app != TRUE)
    {
        if (write(fd, "Hi", 2) == 0)
        {
            // Write operation completed
        }
    }
    pthread_exit(NULL);
}

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check func. .
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculateCRC8(uint8_t *data, uint32_t length)
{
    uint8_t crc = 0xFF;

    for (uint32_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief Check CRC-8 validity for a data buffer.
 *
 * This function checks the CRC-8 validity of the received data buffer.
 * If the CRC check fails, an error status is set.
 *
 * @param buff Data buffer to check.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the status variable.
 */
void CRC_8_check(uint8_t buff[10], uint8_t buff_size, uint8_t *status)
{
    if (buff[9] != calculateCRC8(buff, buff_size))
    {
        printf("The CRC-8 check has failed\nThere is data corruption in the received data from the STM\n");
        if (*status == ON)
        {
            *status = STAY_ON; 
        }
        else
        {
            *status = STAY_OFF;
        }
    }
}

/**
 * @brief Retrieve the status from a data buffer.
 *
 * This function extracts the status information from the given data buffer.
 *
 * @param buff Data buffer containing status information.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the variable to store the retrieved status.
 * @return None.
 */
void get_status(uint8_t buff[10], uint8_t buff_size, uint8_t *status)
{
    *status = buff[0];
}

/**
 * @brief Display the final parking information.
 *
 * This function displays the final parking information including duration and payment.
 *
 * @param payment An array containing payment information.
 * @param size The size of the payment array.
 * @return None.
 */
void final_price_for_costumer(double *payment, int size)
{
    int hour = (((int)payment[1] / 60) / 60) % 24;
    int minutes = ((int)payment[1] / 60) % 60;
    int seconds = (int)payment[1] % 60;
    double final_price = payment[0];

    printf("You parked for %d:%d:%d paid %.2f ILS\nHave a good one!\n\n\n", hour, minutes, seconds, final_price);
}

/**
 * @brief Convert received status to a boolean value.
 *
 * This function converts the received status to a boolean value.
 *
 * @param status The received status.
 * @return 1 if status is ON or OFF, 0 otherwise.
 */
uint8_t received_data_from_stm(uint8_t status)
{
    switch (status)
    {
    case ON:
    case OFF:
        return 1;
    case STAY_ON:
    case STAY_OFF:
    default:
        return 0;
    }
}

/**
 * @brief Update the status value by toggling between 0 and 1.
 *
 * This function updates the status value by toggling between 0 and 1.
 *
 * @param status Pointer to the status value.
 * @return None.
 */
void updating_status_value(uint8_t *status)
{
    /* Setting the status to 1(ON),2(OFF) or 3(RESTART).  */
    *status = (++(*status)) % 3;
    /* In case status is 0(Undefined) add one and become 1(ON).  */
    switch (*status)
    {
    case 0:
        ++(*status);
        break;
    default:
        break;
    }
}

/**
 * @brief Send data to the server and receive the client location name.
 *
 * This function sends data to the server and receives the client location name.
 *
 * @param client_socket Pointer to the client socket.
 * @param data_buff The data buffer to send.
 * @param data_buff_size The size of the data buffer.
 * @param location Pointer to store the received location name.
 * @param location_size The size of the location buffer.
 * @return 0 on success, -1 on error.
 */
int send_data_and_receive_location(int *client_socket, uint8_t *data_buff, uint8_t data_buff_size,
                                   uint8_t *location, uint8_t location_size)
{
    /* Sending the data buffer to the server. */
    if (send(*client_socket, data_buff, data_buff_size, 0) == -1)
    {
        perror("send");
        return -1;
    }

    /* Receiving the client location name from the server. */
    if (recv(*client_socket, location, location_size, 0) == -1)
    {
        perror("recv");
        return -1;
    }

    return 0;
}