/**
  ******************************************************************************
  * @file    client_data.h
  * @brief   Header file for the pango_client_data struct.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#ifndef INC_CLIENT_DATA_H_
#define INC_CLIENT_DATA_H_

/**
  * @brief Structure definition for storing client data with specific packing.
  */
#pragma pack(push,1)
struct pango_client_data
{
    uint8_t status;         		/**< A flag indicating if the application has started or ended. */
    uint8_t mac_address[6]; 		/**< MAC address storage. */
    uint8_t x_coor;          		/**< X-coordinate. */
    uint8_t y_coor;          		/**< Y-coordinate. */
};
#pragma pack(pop)

#endif /* INC_CLIENT_DATA_H_ */
