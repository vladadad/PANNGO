# PANNGO
Pango like project - simulating a parking application device using 2 microcontrollers (Beaglebone Black and NucleoF746ZG) and a server (PC)

# PART 1
Design and implement a system consisting of an STM32 microcontroller and a BBG
(BeagleBone Green) that acts as a single unit. The STM32 should mimic the functionality
of a GPS module and send GPS coordinates in the form of integers to the BBG via the
UART protocol. Before sending the coordinates, the STM32 should send a start message
containing its unique ID, and after sending the coordinates, it should send an end
message also containing its unique ID. The BBG should then initiate the transmission of
this data to a server using the Ethernet protocol only with a press of a button. The
STM32 should not send the time elapsed from the start message to the end message.
Make sure the start and end messages are sent with the coordinates and the unique ID
of the STM32.

# PART 2

Create a TCP server on a PC that can support multiple clients (from the system
implemented in Part 1). The server should be able to receive the information (GPS
coordinates, unique IDs, start and end messages) sent from the clients and store it in
shared memory. The server should then calculate the time elapsed from the start to the
end message for each client separately. Once an end message is received, use the GPS
coordinates (city) to calculate the price based on the time elapsed, using a different file
that stores the pricing information for each city. Another process should be
implemented to update the data to a file every 10 seconds.
Note: Do not create additional unique IDs in the database, instead main start and end
messages in the same row in the database file.
Make sure the server can handle multiple clients at the same time and the data received
includes the time elapsed from the start to the end message, which should be calculated
by the server. Also, make sure that the pricing calculation is done based on the city and
the time elapsed from the start to the end message.
