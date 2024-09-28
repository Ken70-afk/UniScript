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
	readerPointer->flags.isEmpty = UNI_TRUE;  // New buffer is empty
	readerPointer->flags.isFull = UNI_FALSE;  // Buffer is not full initially
	readerPointer->flags.isRead = UNI_FALSE;  // No data read yet
	readerPointer->flags.isMoved = UNI_FALSE; // No memory reallocations yet
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
		/* TO_DO: This buffer is NOT full */
		readerPointer->content[readerPointer->positions.wrte++] = ch;
	}
	else {
		/* TO_DO: Reset Full flag */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			/* TO_DO: Update the last position with Terminator */
			readerPointer->flags.isFull = UNI_TRUE; // Mark as full
			readerPointer->content[readerPointer->positions.wrte] = READER_TERMINATOR;
			return readerPointer;

		case MODE_ADDIT:
			/* TO_DO: Update size for Additive mode */
			newSize = readerPointer->size + readerPointer->increment;
			break;

		case MODE_MULTI:
			/* TO_DO: Update size for Multiplicative mode */
			newSize = readerPointer->size * readerPointer->increment;
			break;

		default:
			return UNI_INVALID;  // Invalid mode
		}

		/* TO_DO: Defensive programming */
		if (newSize <= 0 || newSize > READER_MAX_SIZE) {
			return UNI_INVALID;
		}

		/* TO_DO: Reallocate */
		tempReader = (uni_string)realloc(readerPointer->content, newSize * sizeof(uni_char));
		if (!tempReader) {
			return UNI_INVALID;  // Reallocation failed
		}

		/* Update the buffer with the new size and memory block */
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isMoved = (readerPointer->content != tempReader);  // Set flag if memory address changed

		/* Add the character after successful reallocation */
		readerPointer->content[readerPointer->positions.wrte++] = ch;
	}

	/* TO_DO: Update the flags */
	readerPointer->flags.isFull = (readerPointer->positions.wrte == readerPointer->size);

	/* TO_DO: Update histogram */
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
	readerPointer->flags.isEmpty = UNI_TRUE;   // Buffer is now empty
	readerPointer->flags.isFull = UNI_FALSE;   // Buffer is not full
	readerPointer->flags.isRead = UNI_FALSE;   // Buffer hasn't been fully read
	readerPointer->flags.isMoved = UNI_FALSE;  // Memory hasn't moved
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
	return readerPointer->flags.isFull;  // Return true if the buffer is full
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
	return readerPointer->flags.isEmpty;  // Return true if the buffer is empty
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
	/* TO_DO: Adjust mark */
	return UNI_TRUE;
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
	uni_int cont = 0;
	uni_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	while (cont < readerPointer->positions.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
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
	uni_int size = 0;
	uni_char c;
	/* TO_DO: Defensive programming */
	while (!feof(fileDescriptor)) {
		c = (uni_char)fgetc(fileDescriptor);
		readerPointer = readerAddChar(readerPointer, c);
		size++;
	}
	/* TO_DO: Defensive programming */
	return size;
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
	/* TO_DO: Recover positions: read and mark must be zero */
	/* TO_DO: Update flags */
	return UNI_TRUE;
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
	/* TO_DO: Retract (return 1 pos read) */
	return UNI_TRUE;
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
	/* TO_DO: Restore positions (read to mark) */
	return UNI_TRUE;
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
	/* TO_DO: Check condition to read/wrte */
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
	/* TO_DO: Return read */
	return 0;
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
	/* TO_DO: Return wrte */
	return 0;
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
	/* TO_DO: Return mark */
	return 0;
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
	/* TO_DO: Return size */
	return 0;
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
	/* TO_DO: Return increment */
	return 0;
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
	/* TO_DO: Return mode */
	return '\0';
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
	/* TO_DO: Updates the histogram */
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
	/* TO_DO: Returns the number of errors */
	return 0;
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
	/* TO_DO: Calculate checksum */
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

uni_boln readerPrintFlags(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Shows flags */
	return UNI_TRUE;
}
