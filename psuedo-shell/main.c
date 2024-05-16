/* Seth Tal
 * 04.04.2020
 * Psuedo-Shell: In this project we were tasked with creating a Pseudo-shell environment that
 *               that executes on the command line.
*/

/* -------------------------------------Preprocessor Directives--------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "command.h"

#define __DEBUG 0



void interactive_mode(int argc, char** argv);
void file_mode(int argc, char** argv);
void clean_up(char** input);
void clear_buff(char** buf, int len);
void error_handler(char** token_arr, int len, int __exit_cmd);
void cmd_exec(char** cmd, int len, int __exit_cmd, int cmd_type);


/*  
 * This function catches usage errors when 
 * running the program.

*/
int usage(int argc, char** argv) {
    
    if (argc == 1) {
        return 0;
    } else {
        FILE* check_if_exists = fopen(argv[2], "r");
        char* flag = argv[1];
        // char* cf = "-f";
        if (strncmp("-f", flag, (size_t)2) == 0 && check_if_exists != NULL) {
            fclose(check_if_exists);
            return 1;
        } else if (strncmp("-f", flag, (size_t)2) != 0 && check_if_exists != NULL) {
            fprintf(stderr, "ERR: Invalid flag. ~ flag: %s\n", argv[1]);
            fprintf(stderr, "Usage: ./pseudo-shell\trun program in interactive mode.\n");
            fprintf(stderr, "\t-f <filename>\trun program in file mode.\n");
            fclose(check_if_exists);
            exit(EXIT_FAILURE);
        } else if (strncmp("-f", flag, (size_t)2) == 0 && check_if_exists == NULL) {
            fprintf(stderr, "ERR: invalid filename. ~ file: %s\n", argv[2]);
            fprintf(stderr, "Usage: ./pseudo-shell\trun program in interactive mode.\n");
            fprintf(stderr, "\t-f <filename>\trun program in file mode.\n");
            fclose(check_if_exists);
            exit(EXIT_FAILURE);
        } else if (strncmp("-f", flag, (size_t)2) != 0 && check_if_exists == NULL) {
            fprintf(stderr, "ERR: invalid flag and filename. ~ flag: %s, file: %s\n", argv[1], argv[2]);
            fprintf(stderr, "Usage: ./pseudo-shell\trun program in interactive mode.\n");
            fprintf(stderr, "\t-f <filename>\trun program in file mode.\n");
            fclose(check_if_exists);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char** argv) {
    setbuf(stdout, NULL);

    int mode = usage(argc, argv);


    if (mode) { // if mode == 1, then file mode
        file_mode(argc, argv);
    } else { // else, then interactive mode
        interactive_mode(argc, argv);
    }

    return EXIT_SUCCESS;
}


/* --------------------------------------------------------------------------------------------- */
/*                               Source for File and Interactive Modes                           */
/* --------------------------------------------------------------------------------------------- */
void interactive_mode(int argc, char** argv) {
    // printf("Running program in interactive mode...\n");
    /* Main Function Vars */
    int __exit_cmd = 0;      //loop exit condition
    unsigned long int size = 0;    
    signed long int num_lines = 0;   //
    char* input_buf = NULL;  // input_buff
    char* curr_token = NULL; // holds current token durring processing
    char** token_arr = NULL; // stores all tokens taken from stdin

    while (!__exit_cmd) {
        printf(">>> ");

        // grab input data from user
        num_lines = getline(&input_buf, &size, stdin);

        // removes the new line character
        clean_up(&input_buf);

        /* if the user entered <exit> then exit the loop */
        if (strcmp(input_buf, "exit") == 0) {
            __exit_cmd = 1;
            printf("\n");
            break;
        }

        // used for allocing the array of tokens to process
        int num_spaces = 0;

        for (int i = 0; i < strlen(input_buf); i++) {
            if (input_buf[i] == ' ')
                num_spaces++;
        }
        num_spaces++;

        token_arr = (char**)malloc(sizeof(char*) * num_spaces);
        for (int i = 0; i < num_spaces; i++) {
            token_arr[i] = NULL;
        }


        int i = 0; // counts num tokens

        /* Tokenize the input string */
        curr_token = strtok(input_buf, " ");
        
        /* Display each token and store in array */
        while (curr_token != NULL && __exit_cmd != 1) {
            token_arr[i] = curr_token;
            i++;
            curr_token = strtok(NULL, " ");
        }

        error_handler(token_arr, num_spaces, __exit_cmd);

        if (token_arr != NULL) {
            free(token_arr);
            token_arr = NULL;
        }
    }

    free(input_buf);
    free(token_arr);
    token_arr = NULL;
}

void file_mode(int argc, char** argv) {
  
    /* File Mode Vars */
    FILE* input = fopen(argv[2], "r");
    FILE* output = fopen("output.txt", "w");

    freopen("output.txt", "w", stdout);

    // tokening vars
    unsigned long int size = 0;
    signed long int getLineFlag = 0;
    int __call_count = 0;
    char* input_buf = NULL;
    char* curr_token = NULL;
    char** token_arr = NULL;

    getLineFlag = getline(&input_buf, &size, input);
    
    // clean_up(&input_buf);  // removes new line character

    while (getLineFlag >= 0) {

        // used for allocing the array of tokens to process
        int num_spaces = 0;

        for (int i = 0; i < strlen(input_buf); i++) {
            if (input_buf[i] == ' ')
                num_spaces++;
        }
        num_spaces++;

        token_arr = (char**)malloc(sizeof(char*) * num_spaces);
        for (int i = 0; i < num_spaces; i++) {
            token_arr[i] = NULL;
        }

        // /* Display each token and store in array */
        int i = 0;
        curr_token = strtok(input_buf, " ");
        while (curr_token != NULL) {
            token_arr[i] = curr_token;
            i++;
            curr_token = strtok(NULL, " ");
        }
        
        error_handler(token_arr, num_spaces, 0);
        
       
        if (token_arr != NULL) {
            free(token_arr);
            token_arr = NULL;
        }

        getLineFlag = getline(&input_buf, &size, input);
        clean_up(&input_buf);  // removes new line character
    }

    /* free allocated memory */
    fclose(input);
    fclose(output);
    free(input_buf);
    free(token_arr);
   
}


/* --------------------------------------------------------------------------------------------- */
/*                                        Helper Functions                                       */
/* --------------------------------------------------------------------------------------------- */

void error_handler(char** token_arr, int len, int __exit_cmd) {

    // removing new line chars
    char* pos;
    if ((pos = strrchr(token_arr[len - 1], '\n')) != NULL) {
        *pos = '\0';
    }

    // ERRORS:
    int __invalid_cmd = -1;

    // List of valid commands 
    char CMD_LIST[8][6] = {"ls\0", "pwd\0", "mkdir\0", "cd\0", "cp\0", "mv\0", "rm\0", "cat\0"};
    // command ID list
    int CMD_ID[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    // which command user entered
    int cmd_type = 0;

    if (__exit_cmd)
        return;

 
    /* 
    * Checks for control code at the end of command and also if there
    * is empty white space following a command
    */
    if (token_arr[len - 1] == NULL || strcmp(token_arr[len - 1], ";") == 0) {
        fprintf(stderr, "Error! Trailing whitespace, or control code where unecessary.\n");
        return;
    }

    // stores current command to be processed
    char** cmd = NULL;
    cmd = (char**)malloc(sizeof(char*) * len);
    for (int i = 0; i < len; i++)
        cmd[i] = NULL;

    int param_count = 0;

    int i = 0;
    int j = 0;
    while (i < len) {
        // mark where each command lies
        for (; i < len; i++) {
            // printf("here!\n");
            if (token_arr[i] != NULL && strcmp(token_arr[i], ";") != 0) {
                cmd[j] = token_arr[i];
                param_count++;
                j++;
            } else {
                break;
            }
        }

        for (j = 0; j < param_count; j++) {
            // the command
            if (j == 0) {
                // check if valid command
                for (int k = 0; k < 8; k++) {
                    if (strcmp(cmd[j], CMD_LIST[k]) == 0) {
                        cmd_type = CMD_ID[k];
                        __invalid_cmd = 0;
                        break;
                    } else {
                        __invalid_cmd = 1;
                    }
                }
                if (__invalid_cmd) {
          
                    fprintf(stderr, "Error! Unrecognized command: %s\n", cmd[j]);
                    return;
          
                }
            }

            // the params
            if (strcmp(cmd[j], cmd[0]) == 0 && param_count > 1 && j > 0) {
                
                // * Syntax error
                fprintf(stderr, "Error! Incorrect syntax. No control code found.\n");
                return;
                // exit(EXIT_FAILURE);
            }
            // printf("%d\n", param_count);
            switch(cmd_type)
            {
                case 1:
                    if (param_count > 1) {
                        fprintf(stderr, "Error! Unsupported or too many parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 2:
                        if (param_count > 1) {
                        fprintf(stderr, "Error! Unsupported or too many parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 3:
                    if (param_count > 2) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 2) {
                        fprintf(stderr, "Error! Command: %s must have 1 parameter.\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 4:
                    if (param_count > 2) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 2) {
                        fprintf(stderr, "Error! Command: %s must have 1 parameter.\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 5:
                    if (param_count > 3) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 3) {
                        fprintf(stderr, "Error! Command: %s must have 2 parameters\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 6:
                    if (param_count > 3) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 3) {
                        fprintf(stderr, "Error! Command: %s must have 2 parameters\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 7:
                    if (param_count > 2) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 2) {
                        fprintf(stderr, "Error! Command: %s must have 1 parameter.\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
                case 8:
                    if (param_count > 2) {
                        fprintf(stderr, "Error! Unsupported parameters for command: %s\n", CMD_LIST[cmd_type - 1]);
                        return;
                    } else if (param_count < 2) {
                        fprintf(stderr, "Error! Command: %s must have 1 parameter.\n", CMD_LIST[cmd_type - 1]);
                        return;
                    }
                    break;
            }
        }

        cmd_exec(cmd, len, __exit_cmd, cmd_type);

        param_count = 0;
        j = 0;
        clear_buff(cmd, len);
        i++;
    }

    free(cmd);
}

void cmd_exec(char** cmd, int len, int __exit_cmd, int cmd_type) {
    if (__exit_cmd)
        return;

    char *src, *dst, *dirName_mkdir, *dirName_cd, *file_name;

    switch(cmd_type)
    {
        case 1:
            
            listDir();    
            break;
        case 2:
        
            
            showCurrentDir();

        
            break;
        case 3:
         

            dirName_mkdir = cmd[1];
            makeDir(dirName_mkdir);

      
            break;
        case 4:
       

            dirName_cd = cmd[1];
            changeDir(dirName_cd);

          
            break;
        case 5:
     

            src = cmd[1];
            dst = cmd[2];
            copyFile(src, dst);

         
            break;
        case 6:
         

            src = cmd[1];
            dst = cmd[2];
            moveFile(src, dst);

         
            break;
        case 7:
        

            file_name = cmd[1];
            deleteFile(file_name);

            
            break;
        case 8:
       
            file_name = cmd[1];
            displayFile(file_name);
            break;
    }
}

void clean_up(char** input) {
    char* cleaned = *input;
    int   len = strlen(cleaned);
    cleaned[len - 1] = '\0';
    *input = cleaned;
}

void clear_buff(char** buf, int len) {
    for (int i = 0; i < len; i++) {
        buf[i] = NULL;
    }
}
