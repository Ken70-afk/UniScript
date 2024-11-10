/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Diyon Johnson / Abhinav Ranjith
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=------------------------------------------------="
# ECHO "|        COMPILERS - ALGONQUIN COLLEGE (F24)     |"
# ECHO "=------------------------------------------------="
# ECHO "                                        	      "
# ECHO " +++++++=++*+++++++++++++++=*++++++++++++++++++++ "
# ECHO " ++++++++++++++++++++++++++++++++++++++=+++++++++ "
# ECHO " ++++++++++++++++++++++++***+*+*+=**+++++++++++++ "
# ECHO " +++++*+++++.        -++++=+=.        -+++*+++++= "
# ECHO " ++++++++++  @@@@@@@  ++++++  @@@@@@@# -++++**+++ "
# ECHO " +++++++++* @@     @@ ++++++ @@     @@ .=**====+* "
# ECHO " +++++++++* -@ = @ @@ ++++++ =@  @@  @  **=++**+= "
# ECHO " *++++++++* -@ @@@ @@ ++++++ :@ @@@ %@ .+==++++++ "
# ECHO " +++++++++* -@     @@ ++=+== :@     @@  +++++++*+ "
# ECHO " *+++++++++ -@ @@% @@ +*+**+ :@ @@  %@  +++++++*+ "
# ECHO " +++++++*=+ -@     @@ ===+=+ :@  .@@#@  +++***+++ "
# ECHO " ++++++++++ :@ ##% @@ *++++* .@ @@  @@  +++++++++ "
# ECHO " +===++*+++ -@     @@  .++-  @@     @@ .++++**+*+ "
# ECHO " +*++*++=++ =@  @@  @@@     @@  @@@ @@ :+++++++=+ "
# ECHO " ++*+=+**++  @@ @@    @@@@@@    @@  @@ +++++****+ "
# ECHO " ++++*===+==  @@   %@@   @   @@    @@  .*++++==++ "
# ECHO " ++++*+***++-  @@@      @@       @@  @@  ++++*+++ "
# ECHO " ++++++*=+++==   @@@@   @@   @@@@*  @@ @ =+++++++ "
# ECHO " *===*+*+=+++==:    %@@@@@@@@@:     @@@  ++++++++ "
# ECHO " **++++==**+++++=:              -+*-    =++++++*+ "
# ECHO " +++++++++++*+=++++++==---==+++*++++++++++++++*+= "
# ECHO " **=++++*++*+*+++*+++++++++++++++++++++++++++=+*+ "
# ECHO " *+++*+++++++*+  U N I S C R I P T ++++++++++++++ "
# ECHO " *+*+++++==++++*++=++++++++++++++++++++++++++=+++ "
# ECHO " *+*+++++**++*+****++++++++++++++++++++++++++*+++ "
# ECHO "                                                  "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

BufferPointer readerCreate(uni_int size, uni_int increment, uni_char mode) {
	BufferPointer readerPointer;
	uni_int count = 0;
	/* TO_DO: Defensive programming */
	if (size < 0)
		return UNI_INVALID;
	if (!size)
		size = READER_DEFAULT_SIZE;
	if (increment < 0)
		return UNI_INVALID;
	if (!increment)
		increment = READER_DEFAULT_INCREMENT;
	if (!mode)
		mode = MODE_FIXED;
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return UNI_INVALID;
	readerPointer->content = (uni_string)malloc(size);

	/* TO_DO: Defensive programming */
	if (!readerPointer->content) {  // Handle allocation failure
		free(readerPointer);  // Avoid memory leaks
		return NULL;
	}

	/* TO_DO: Initialize the histogram */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;  // Clear the histogram (no characters processed)
	}
	readerPointer->mode = mode;
	readerPointer->size = size;
	readerPointer->increment = increment;
	
	/* TO_DO: Initialize errors */
	readerPointer ->numReaderErrors = 0;
	/* TO_DO: Initialize flags */
	readerPointer->flags[FLAG_EMP] = UNI_TRUE;   // New buffer is empty
	readerPointer->flags[FLAG_FUL] = UNI_FALSE;  // Buffer is not full initially
	readerPointer->flags[FLAG_END] = UNI_FALSE;  // No data read yet
	readerPointer->flags[FLAG_REL] = UNI_FALSE;  // No memory reallocations yet
	/* TO_DO: Default checksum */
	readerPointer->checksum = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer readerPointer, uni_char ch) {
	uni_string tempReader = UNI_INVALID;  // Temporary variable for reallocation
	uni_int newSize = 0;  // New size to be calculated based on the mode

	/* TO_DO: Defensive programming */
	if (!readerPointer || ch < 0 || ch > 127) {
		return UNI_INVALID;  // Return invalid if pointer is null or char is out of range
	}

	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->positions.wrte * (uni_int)sizeof(uni_char) < readerPointer->size) {
		/* Buffer is NOT full, add the character */
		readerPointer->content[readerPointer->positions.wrte++] = ch;
	}
	else {
		/* Buffer is full, handle according to mode */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			/* Fixed size mode - no reallocation allowed */
			readerPointer->flags[FLAG_FUL] = UNI_TRUE; // Mark as full
			readerPointer->content[readerPointer->positions.wrte] = READER_TERMINATOR;
			return readerPointer;

		case MODE_ADDIT:
			/* Additive mode - increase size by the increment */
			newSize = readerPointer->size + readerPointer->increment;
			break;

		case MODE_MULTI:
			/* Multiplicative mode - increase size by multiplying with the increment */
			newSize = readerPointer->size * readerPointer->increment;
			break;

		case MODE_TOTAL:
			/* Total mode ('t') - allocate the maximum size once */
			newSize = READER_MAX_SIZE;
			break;

		default:
			return UNI_INVALID;  // Invalid mode
		}

		/* TO_DO: Defensive programming - check if new size is valid */
		if (newSize <= 0 || newSize > READER_MAX_SIZE) {
			return UNI_INVALID;
		}

		/* Reallocate buffer */
		tempReader = (uni_string)realloc(readerPointer->content, newSize * sizeof(uni_char));
		if (!tempReader) {
			return UNI_INVALID;  // Reallocation failed
		}

		/* Update buffer and check if memory was relocated */
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags[FLAG_REL] = (readerPointer->content != tempReader);  // Set memory relocated flag

		/* Add character after reallocation */
		readerPointer->content[readerPointer->positions.wrte++] = ch;
	}

	/* Update full flag */
	readerPointer->flags[FLAG_FUL] = (readerPointer->positions.wrte == readerPointer->size);

	/* Update histogram */
	readerPointer->histogram[ch]++;

	return readerPointer;
}




/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return UNI_FALSE;
	/* TO_DO: Adjust the write, mark and read to zero */
	readerPointer->positions.wrte = 0;
	readerPointer->positions.read = 0;
	readerPointer->positions.mark = 0;
	/* TO_DO: Adjust flags */
	readerPointer->flags[FLAG_EMP] = UNI_TRUE;   // Buffer is now empty
	readerPointer->flags[FLAG_FUL]= UNI_FALSE;   // Buffer is not full
	readerPointer->flags[FLAG_END] = UNI_FALSE;   // Buffer hasn't been fully read
	readerPointer->flags[FLAG_REL] = UNI_FALSE;  // Memory hasn't moved
	return UNI_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/*uni_boln readerFree(BufferPointer const readerPointer) {
    /* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE;  // Return false if the pointer is NULL
	}

	/* TO_DO: Free pointers */
	if (readerPointer->content) {
		free(readerPointer->content);  // Free the buffer memory
		readerPointer->content = NULL; // Set pointer to NULL to avoid dangling pointer
	}

	free(readerPointer);  // Free the reader structure itself

	return UNI_TRUE;  // Return true if operation is successful

}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE;  // Return false if the pointer is NULL
	}

	/* TO_DO: Check if the buffer is marked as full */
	return readerPointer->flags[FLAG_FUL];  // Return true if the buffer is full
}

/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE;  // Return false if the pointer is NULL
	}

	/* TO_DO: Check flag if buffer is EMP */
	return readerPointer->flags[FLAG_EMP]
		;  // Return true if the buffer is empty
}


/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerSetMark(BufferPointer const readerPointer, uni_int mark) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE;  // Invalid readerPointer
	}

	/* TO_DO: Check boundary conditions */
	if (mark < 0 || mark > readerPointer->positions.wrte) {
		return UNI_FALSE;  // Invalid mark position
	}

	/* TO_DO: Adjust mark */
	readerPointer->positions.mark = mark;

	return UNI_TRUE;  // Mark successfully set
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerPrint(BufferPointer const readerPointer) {
	// TO_DO: Defensive programming (including invalid chars)
	if (!readerPointer) {
		return 0; // Return 0 if the pointer is invalid
	}

	uni_int cont = 0; // Counter for the number of characters printed
	uni_char c; // Variable to hold the character

	// Loop until we have printed all characters or reached the end of the reader
	while (cont < readerPointer->positions.wrte) {
		c = readerGetChar(readerPointer); // Get the next character

		// Check for invalid characters or the END flag
		if (readerPointer->flags[FLAG_FUL]) {
			break; // Exit the loop if the end of content is reached
		}

		// Print the character and increment the counter
		printf("%c", c);
		cont++; // Increment the counter
	}

	return cont; // Return the total number of characters printed
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	uni_int size = 0; // Counter for the number of characters read
	uni_char c; // Variable to hold each character

	/* TO_DO: Defensive programming */
	if (!readerPointer || !fileDescriptor) {
		return 0; // Return 0 if the readerPointer or fileDescriptor is NULL
	}

	while (!feof(fileDescriptor)) {
		c = (uni_char)fgetc(fileDescriptor); // Read one character from the file

		// Check if we have reached the end of the file or encountered an error
		if (feof(fileDescriptor)) {
			break; // Exit the loop if end of file is reached
		}
		else if (ferror(fileDescriptor)) {
			return size; // Return the size if there is an error reading the file
		}

		// Attempt to add the character to the reader
		if (readerAddChar(readerPointer, c) == UNI_INVALID) {
			ungetc(c, fileDescriptor); // Push back the character if it cannot be added
			break; // Exit the loop since we can't add the character
		}
		size++; // Increment the size counter
	}

	/* TO_DO: Defensive programming */
	return size; // Return the total number of characters read and added to the buffer
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE; // Return false if the readerPointer is NULL
	}

	/* TO_DO: Recover positions: read and mark must be zero */
	readerPointer->positions.read = 0; // Reset read position to 0
	readerPointer->positions.mark = 0; // Reset mark position to 0

	/* TO_DO: Update flags */
	readerPointer->flags[FLAG_REL] = UNI_FALSE; // Reset the moved flag
	readerPointer->flags[FLAG_FUL] = UNI_FALSE;  // Reset the full flag

	return UNI_TRUE; // Return true to indicate success
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE; // Return false if readerPointer is NULL
	}

	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->positions.read > 0) {
		readerPointer->positions.read--;  // Decrement the read position
		return UNI_TRUE;  // Successfully retracted
	}
	else {
		// If the read position is already at 0, it can't be decremented further
		return UNI_FALSE;
	}
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_boln readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return UNI_FALSE;  // Return false if readerPointer is NULL
	}

	/* TO_DO: Restore positions (read to mark) */
	if (readerPointer->positions.mark >= 0 && readerPointer->positions.mark <= readerPointer->positions.wrte) {
		// Set the read position to the mark position
		readerPointer->positions.read = readerPointer->positions.mark;
		return UNI_TRUE;  // Success
	}
	else {
		return UNI_FALSE;  // Mark position is invalid, return failure
	}
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		return READER_TERMINATOR;  // Return the end-of-string character if the pointer is NULL
	}

	/* TO_DO: Check condition to read/wrte */
	if (readerPointer->positions.read >= readerPointer->positions.wrte) {
		// Set the isRead flag when the read position reaches the write position
		readerPointer->flags[FLAG_END] = UNI_TRUE;
		return READER_TERMINATOR;  // Return end-of-string character when at the end of the reader
	}
	else {
		// Reset the isRead flag since there are more characters to read
		readerPointer->flags[FLAG_END] = UNI_FALSE;
	}

	// Return the current character and increment the read position
	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_string readerGetContent(BufferPointer const readerPointer, uni_int pos) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		return NULL;  // Return NULL if readerPointer or content is invalid
	}

	/* TO_DO: Check boundary conditions */
	if (pos < 0 || pos >= readerPointer->positions.wrte) {
		return NULL;  // Return NULL if the pos value is out of bounds
	}

	// Return a pointer to the character at the specified position in the content
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return -1;  // Return -1 if readerPointer is NULL to indicate an invalid position
	}

	/* TO_DO: Return read */
	return readerPointer->positions.read;
}

/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return error if readerPointer is NULL
	}

	/* TO_DO: Return wrte */
	return readerPointer->positions.wrte;  // Return the current write position
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return an error if readerPointer is NULL
	}

	/* TO_DO: Return mark */
	return readerPointer->positions.mark;  // Return the mark position in the buffer
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return an error if readerPointer is NULL
	}

	/* TO_DO: Return size */
	return readerPointer->size;  // Return the current size of the buffer
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetInc(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return an error if readerPointer is NULL
	}

	/* TO_DO: Return increment */
	return readerPointer->increment;  // Return the current buffer increment
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_char readerGetMode(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return an error if readerPointer is NULL
	}

	/* TO_DO: Return mode */
	return readerPointer->mode;  // Return the current mode (fixed / additive / multiplicative)
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_null readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID || readerPointer->content == UNI_INVALID) {
		return;  // Exit the function if the readerPointer or its content is NULL
	}

	/* TO_DO: Updates the histogram */
	printf("Reader statistics:\n");

	uni_int first = 1; // To help with formatting commas between entries

	for (uni_int i = 0; i < NCHAR; ++i) {
		if (readerPointer->histogram[i] > 0) {  // Only print characters that appear at least once
			if (!first) {
				printf(", "); // Add a comma before each item except the first one
			}
			first = 0; // First entry printed, next ones will have a comma
			printf("B[%c]=%d", i, readerPointer->histogram[i]);
		}
	}

	printf("\n"); // End the line after printing the histogram
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
uni_int readerGetNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID) {
		return UNI_ERROR;  // Return an error if readerPointer is NULL
	}

	/* TO_DO: Returns the number of errors */
	return readerPointer->numReaderErrors;  // Return the number of errors encountered
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

uni_null readerCalcChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNI_INVALID || readerPointer->content == UNI_INVALID) {
		return;  // Exit if the readerPointer or content is invalid
	}

	uni_int checksum = 0;  // Initialize the checksum

	/* TO_DO: Calculate checksum */
	for (uni_int i = 0; i < readerPointer->positions.wrte; ++i) {
		checksum += (uni_byte)readerPointer->content[i];  // Sum the value of each character
	}

	// Store the checksum as an 8-bit value (use bitwise AND with 0xFF to mask only the lower 8 bits)
	readerPointer->checksum = (uni_byte)(checksum & 0xFF);
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

uni_boln readerPrintFlags(BufferPointer const readerPointer) {
	// Defensive programming: Check if readerPointer is NULL
	if (readerPointer == NULL) {
		fprintf(stderr, "Error: Invalid pointer.\n");
		return UNI_FALSE;  // Return False to indicate the function did not execute successfully
	}

	// Check if the buffer is empty
	if (readerPointer->positions.wrte == 0) {
		readerPointer->flags[FLAG_EMP]= UNI_TRUE;  // Set isEmpty to True if no characters have been written
	}
	else {
		readerPointer->flags[FLAG_EMP] = UNI_FALSE;  // Set isEmpty to False if there are written characters
	}

	// Check if the buffer is full
	if (readerPointer->positions.wrte == readerPointer->size) {
		readerPointer->flags[FLAG_FUL] = UNI_TRUE;  // Buffer is full when write position equals size
	}
	else {
		readerPointer->flags[FLAG_FUL] = UNI_FALSE;  // Buffer is not full otherwise
	}

	// Check if the buffer has been read from
	if (readerPointer->positions.read > 0 && readerPointer->positions.read < readerPointer->positions.wrte) {
		readerPointer->flags[FLAG_END] = UNI_TRUE;  // Set isRead to True if some data has been read
	}
	else {
		readerPointer->flags[FLAG_END] = UNI_FALSE;  // Set isRead to False if nothing has been read
	}

	// Check if the buffer was moved
	if (readerPointer->positions.read != readerPointer->positions.mark) {
		readerPointer->flags[FLAG_REL] = UNI_TRUE;  // Set  to True if the read pointer has moved past the mark
	}
	else {
		readerPointer->flags[FLAG_REL] = UNI_FALSE;  // Set isMoved to False if read and mark positions are equal
	}

	// Print the flag statuses
	printf("Buffer Flags:\n");
	printf("isEmpty: %s\n", readerPointer->flags[FLAG_EMP] ? "True" : "False");
	printf("isFull: %s\n", readerPointer->flags[FLAG_FUL] ? "True" : "False");
	printf("isRead: %s\n", readerPointer->flags[FLAG_END] ? "True" : "False");
	printf("isMoved: %s\n", readerPointer->flags[FLAG_REL] ? "True" : "False");
	uni_boln flagValue = (readerPointer->flags[FLAG_EMP] << 0) |
		(readerPointer->flags[FLAG_FUL] << 1) |
		(readerPointer->flags[FLAG_END] << 2) |
		(readerPointer->flags[FLAG_REL] << 3);

	printf("The value of the flag field is: %02X\n", flagValue);
	return UNI_TRUE;  // Return True to indicate the function executed successfully
}

