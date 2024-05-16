#include <stdio.h>
#include <stdlib.h>
# include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <errno.h>
#include <unistd.h>
#include "command.h"




void listDir()
{
	/*for the ls command*/
	// first get directory, 
	//then itrarte through the memebrs in directory, 
	//printing them to stdout
	
	struct dirent *dirent;

    DIR *dir = opendir(".");
    
    if (dir != NULL) {
        while ((dirent=readdir(dir)) != NULL) {
            write(1, dirent->d_name, strlen(dirent->d_name));
            write(1, " ", strlen(" "));
        }
        write(1, "\n", strlen("\n"));
    } else {
        write(1, "Error! could not open current directory.\n", 41);
        return;
    }

    free(dir);

} 

void showCurrentDir(){
	/*for the pwd command*/
/* steps:
1. get the current directory/ path( check if we need path to)
2. print that without printf or anything like that */
	 long size;
    char* ptr = NULL;

    size = pathconf(".", _PC_PATH_MAX);
    char buf[(size_t) size];

    if (buf != NULL)
        ptr = getcwd(buf, (size_t)size);
    
    write(1, ptr, strlen(ptr));
    write(1, "\n", strlen("\n"));
} 

void makeDir(char *dirName){
/*for the mkdir command*/
	int flag = mkdir(dirName, 0777);

    if (flag == -1) {
        write(2, "Error! directory could not be created.\n", strlen("Error! directory could not be created.\n"));
        return;

	  

}
}
void changeDir(char *dirName) /*for the cd command*/
{

	/* steps:
	1. get the name of the directory
	2. if present open that directory
	if not return an error
	*/


if(dirName== NULL){
	EXIT_FAILURE;
		}
		else{
			chdir(dirName);
		}
	
		}

void copyFile(char *sourcePath, char *destinationPath)


{
    int src;
    int destination;
    size_t bytes_read;
    char buffer[4096];
    struct stat dest_stat;
    char* path = malloc(strlen(destinationPath) + strlen(sourcePath) + 2);
    src = open(sourcePath, O_RDONLY);
    if (src == -1) {
        write(1, "error cant copy", strlen("error cant copy"));
        free(path);
        return;
    }
    if (stat(destinationPath, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        strcpy(path, destinationPath);
        strcat(path, "/");
        strcat(path, basename(sourcePath));
        path[strlen(destinationPath)+strlen(sourcePath)+1]='\0';
        destination = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    } else {
        destination = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    if (destination == -1) {
        write(1, "error cant copy file", strlen("error cant copy file"));
        free(path);
        return;
    }
    while ((bytes_read = read(src, buffer, sizeof(buffer))) > 0) {
        write(destination, buffer, bytes_read);
    }
    free(path);
    close(src);
    close(destination);
	}

 





void moveFile(char *sourcePath, char *destinationPath) 
{
/*for the mv command
can check if destination path is valid,
 then rename the source path to it*/
	int flag = 0;
    // char* newDestPath[BUFSIZ];

    DIR* dirTest = opendir(destinationPath);

    if (dirTest != NULL) {
        int sizeOfNewPath = strlen(sourcePath) + strlen(destinationPath);
        char newDestPath[sizeOfNewPath];
        strcpy(newDestPath, destinationPath);
        strcat(newDestPath, "/");
        strcat(newDestPath, sourcePath);
        flag = rename(sourcePath, newDestPath);
    } else {
        flag = rename(sourcePath, destinationPath);
    }

    if (flag == -1) {
        write(2, "Error! an error occured while moving file.\n", strlen("Error! an error occured while moving file.\n"));
    }

    free(dirTest);
}

void deleteFile(char *filename) 
/*for the rm command*/
{
		int test=0;
		test= remove(filename);
		if(test==-1){
			 write(2, "an error occured while removing file.\n", strlen("an error occured while removing file.\n"));
    }
		
	remove(filename);

}

void displayFile(char *filename) 
/*for the cat command*/
{
	/*steps: 
	1. chekc if file exists in directory / given path
	2. oepn file
	3. read line by line, print output to console
	*/
	 char buf[4099];
    for (int i = 0; i < 4099; i++) {
        buf[i] = (unsigned long int)NULL;
    }

    int fd = open(filename, O_RDONLY);

    if (read(fd, buf, BUFSIZ) < 0) {
        write(2, "Error! an error occured when reading file\n", strlen("Error! an error occured when reading file\n"));
        return;
    }
    // strcat(buf, "\n");

    write(1, buf, strlen(buf));

    close(fd);

}