/* Author: Marcus Ross
 *         MR867434
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "const.h"

void archiveFiles(int fileCountI, char *archiveName, char **fileName) {
	unsigned char fileCount = fileCountI; /* cast the int to a char, essentially */
	int fdArch; /* file descriptor for archive */
	int fd; /* file descriptor for files being archived */
	int i; /* for loop counter */
	int maxNameLen = 0;
	unsigned char nameLen; /* length of the filename */
	unsigned int fileSize[fileCount]; /* hold the size of every file b/c it's needed for both metadata and the payload */
	char errMsg[300];
	char *fileContents; /* buffer to hold the contents of each file */
	struct stat *stats; /* for holding each file's stats */
	
	fdArch = open(archiveName, O_CREAT | O_WRONLY | O_TRUNC, 0644); /* create the archive file */
	
	if (fdArch < 0) { /* if open() returns a neg error value */
		strcpy(errMsg, ERR_CREAT); /* construct error message */
		strcat(errMsg, archiveName);
		perror(errMsg);
		exit(-1);
	}
	
	write(fdArch, &fileCount, sizeof(fileCount)); /* print # of files to archive */
	
	for (i = 0; i < fileCount; i++) { /* for each file being archived */
		stats = (struct stat *) malloc(sizeof(struct stat));
		
		if(stat(fileName[i], stats) < 0) { /* if stat() returns a neg error value */
			strcpy(errMsg, ERR_FILE); /* construct error message */
			strcat(errMsg, fileName[i]);
			perror(errMsg);
			exit(-1);
		}
		
		nameLen = strlen(fileName[i]); /* get the length of the file name */
		fileSize[i] = stats->st_size; /* store the file size */
		
		write(fdArch, &nameLen, sizeof(nameLen)); /* print the length of the filename to archive */
		write(fdArch, fileName[i], nameLen); /* print the filename to archive */
		write(fdArch, &fileSize[i], sizeof(fileSize[i])); /* print the size of the file to archive */
		
		free(stats);
	}
	
	for (i = 0; i < fileCount; i++) {
		fd = open(fileName[i], O_RDONLY); /* open the file to be archived */
		
		if (fd < 0) { /* if open() returns a neg error value */
			strcpy(errMsg, ERR_FILE); /* construct error message */
			strcat(errMsg, fileName[i]);
			perror(errMsg);
			exit(-1);
		}
		
		fileContents = (char *) malloc(sizeof(char) * fileSize[i]); /* size of buffer = size of file */
		read(fd, fileContents, fileSize[i]); /* put the file in the buffer */
		write(fdArch, fileContents, fileSize[i]); /* put the buffer in the archive */
		
		free(fileContents);
		close(fd);
	}
	
	close(fdArch);
}

void printFiles(char *prefix, char *archiveName) {
	char *fileName; /* name of file in archive */
	char errMsg[300];
	int fdArch; /* file descriptor for archive */
	int i; /* for loop counter */
	unsigned int fileSize;
	unsigned char filesFound = 0; /* to know at the end whether any matches were found */
	unsigned char nameLen; /* length of the name of the archived file */
	unsigned char fileCount; /* # of files in archive */
	unsigned char prefixLen; /* the length of the prefix argument */
	unsigned char maxNameLen = 0; /* used for output formatting */
	
	prefixLen = strlen(prefix);
	
	fdArch = open(archiveName, O_RDONLY);
	
	if (fdArch < 0) { /* if open() returns a neg error value */
		strcpy(errMsg, ERR_ARCH); /* construct error message */
		strcat(errMsg, archiveName);
		perror(errMsg);
		exit(-1);
	}
	
	read(fdArch, &fileCount, sizeof(char)); /* read file count from archive */
	
	for (i = 0; i < fileCount; i++) { /* for each file in the archive */
		read(fdArch, &nameLen, sizeof(char)); /* read name length */
		
		if(maxNameLen < nameLen) { /* find the greatest name length for output formatting */
			maxNameLen = nameLen;
		}
		
		lseek(fdArch, sizeof(char) * nameLen + sizeof(fileSize), SEEK_CUR); /* skip the filename and file size */
	}
	
	lseek(fdArch, sizeof(char), SEEK_SET); /* seek back to the first file's metadata */
	
	for (i = 0; i < fileCount; i++) { /* for each file in the archive */
		read(fdArch, &nameLen, sizeof(char)); /* read the name length */
		fileName = (char *) malloc(sizeof(char) * (nameLen + 1)); /* malloc nameLen plus 1 for terminator */
		read(fdArch, fileName, sizeof(char) * nameLen); /* read filename */
		fileName[nameLen] = '\0'; /* terminate the string */
		read(fdArch, &fileSize, sizeof(fileSize)); /* read the file size */
		
		if (strncmp(fileName, prefix, prefixLen) == 0) { /* if fileName beings with prefix */
			filesFound++; /* remember that a matching file has been found */
			printf("%-*s %10u\n", maxNameLen, fileName, fileSize); /* print the file's info */
			free(fileName);
		}
	}
	
	if (!filesFound) { /* filesFound == 0 if no matches were found */
		printf("No files found in \"%s\" with names that start with \"%s\".\n", archiveName, prefix);
	}
	
	close(fdArch);
}

void extractFiles(char *archiveName) {
	char **fileName; /* names of files in archive */
	char errMsg[300];
	char *fileContents; /* buffer to hold the contents of each file */
	int fd; /* file descriptor for files being extracted */
	int fdArch; /* file descriptor for archive */
	int i; /* for loop counter */
	unsigned int *fileSize;
	unsigned char filesFound = 0; /* to know at the end whether any matches were found */
	unsigned char nameLen; /* length of the name of the archived file */
	unsigned char fileCount; /* # of files in archive */
	unsigned char maxNameLen = 0; /* used for output formatting */
	
	fdArch = open(archiveName, O_RDONLY);
	
	if (fdArch < 0) { /* if open() returns a neg error value */
		strcpy(errMsg, ERR_ARCH); /* construct error message */
		strcat(errMsg, archiveName);
		perror(errMsg);
		exit(-1);
	}
	
	read(fdArch, &fileCount, sizeof(fileCount)); /* read the file count */
	
	fileName = (char **) malloc(sizeof(char *) * fileCount); /* space for # of filenames equal to fileCount */
	fileSize = (unsigned int *) malloc(sizeof(unsigned int) * fileCount); /* space for # of file sizes equal to fileCount */
	
	for (i = 0; i < fileCount; i++) { /* for each file in archive */
		read(fdArch, &nameLen, sizeof(nameLen)); /* read the filename length */
		fileName[i] = (char *) malloc(sizeof(char) * (nameLen + 1));
		read(fdArch, fileName[i], sizeof(char) * nameLen); /* read the filename */
		fileName[nameLen] = '\0'; /* terminate the string */
		read(fdArch, &fileSize[i], sizeof(fileSize[i])); /* read the file size */
	}
	
	for (i = 0; i < fileCount; i++) { /* for each file in archive */
		fd = open(fileName[i], O_CREAT | O_WRONLY | O_TRUNC, 0644); /* create a new file with appropriate name */
		
		if (fd < 0) { /* if open() returns a neg value */
			strcpy(errMsg, ERR_CREAT); /* construct error message */
			strcat(errMsg, fileName[i]);
			perror(errMsg);
			exit(-1);
		}
		
		fileContents = (char *) malloc(sizeof(char) * fileSize[i]); /* size of buffer = size of file */
		read(fdArch, fileContents, fileSize[i]); /* put the file in the buffer */
		write(fd, fileContents, fileSize[i]); /* put the buffer in the archive */
		
/* 		free(fileName[i]); causes mysterious crash for some reason */
		free(fileContents);
		close(fd);
	}
	
/* 	free(fileSize); causes crash for some reason */
	free(fileName);
	close(fdArch);
}