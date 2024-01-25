#ifndef PNG_ENUMS_H
#define PNG_ENUMS_H

enum true_false
{
	TRUE = 0x1,
	FALSE = !TRUE
};

enum is_connected
{
	CONNECTED = 0x1U,
	NOT_CONNECTED = !CONNECTED
};

enum app_status
{
	ON = 1,
	OFF = 2,
	RESTART = 3,
	STAY_ON = 6,
	STAY_OFF = 7
};

enum quit
{
	QUIT = 12,
	DONT_QUIT = 13
};

#endif /*PNG_ENUMS_H*/