/**
  ******************************************************************************
  * @file    uart_communication.h
  * @brief   Header file for transporting and receiving data
  * 		 using the UART peripheral.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#ifndef INC_UART_COMMUNICATION_H_
#define INC_UART_COMMUNICATION_H_

/* Handler types ------------------------------------------------------------*/
extern UART_HandleTypeDef 		huart3;			/**< External UART handler for user communication */
extern UART_HandleTypeDef 		huart4;			/**< External UART handler for general purpose */

/* Define types ------------------------------------------------------------ */
#define USER_UART 				&huart3			/**< Macro for debugging and clarify when the program starts */
#define UART_4 					&huart4			/**< Macro for receiving and transmitting data from the BBB controller */
#define BUFFER_SIZE_TO_SEND    	  10	 		/**< Size of the buffer used for data transmission via UART_4 */
#define SMALL_DELAY				 1000			/**< Represents the amount of time there is a transmission delay in milliseconds, this value can be changed */

#endif /* INC_UART_COMMUNICATION_H_ */
