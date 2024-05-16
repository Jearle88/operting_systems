/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan, Monil
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/
	    if (buf == NULL || delim == NULL){// Check for NULL strings
        return 0;
    }
    	int count=0;
    	char* buf_copy = strdup(buf);
 

       while (*buf != '\0' && strchr(delim, *buf) != NULL) {
        buf++;
    }

   char* token = strtok(buf_copy, delim); // Get the first token

      while (token != NULL && token[0] == '\0') {
        token = strtok(NULL, delim);
    }

    while (token != NULL) {
        count++;
        token = strtok(NULL, delim); // Get the next token
    }

    // If the string ends with delimiter, reduce the count
    if (buf_copy[strlen(buf_copy) - 1] == delim[0])
        count--;
    free(buf_copy);
    return count;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/
	 char* token;
    char* saveptr;
    int i = 0;

	 command_line result;
	 
	 result.num_token=count_token(buf,delim);

	
	 result.command_list = (char**)malloc((result.num_token+ 1) * sizeof(char*));
	   	strtok_r(buf,"\n", &saveptr);
	   	  token = strtok_r(buf,delim, &saveptr);

    /*
     for (token = strtok_r(buf, delim, &saveptr); token != NULL; token = strtok_r(NULL, delim, &saveptr)) {
        // Allocate memory for each index of the array with the length of tokens
        result.command_list[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if (result.command_list[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        // Copy the token to the command_list array
        strcpy(result.command_list[i], token);
        i++;
    }
    */
     while (token != NULL) {
        // Allocate memory for each token and copy it
        result.command_list[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if (result.command_list[i] == NULL) {
            // Handle memory allocation failure
            exit(EXIT_FAILURE);
        }
        strcpy(result.command_list[i], token);
        token = strtok_r(NULL, delim, &saveptr);
        i++;
    }

     result.command_list[result.num_token] = NULL;

     return result;

}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/
	    for (int i = 0; i < command->num_token; i++) {
        free(command->command_list[i]);
    }

    // Free the command_list array itself
    free(command->command_list);
	
	//free(command);
}
