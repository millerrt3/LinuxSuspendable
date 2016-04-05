#ifndef MODULE_TYPES_H
#define MODULE_TYPES_H

/**
 * @brief Represents possible return codes from functions within the module
 *
 * @details
 * 		Represents the possible return code values from the functions within the module.
 */
typedef enum
{

	OK                    =  0, /**< Function was successful */
	ERROR            = -1, /**< An unknown error occurred */
	INVALID_PID   = -2, /**< Invalid PID was provided or the PID is not currently active */
	INVALID_ARG = -3, /**< Invalid argument provided to the function */

} Status;

#endif
