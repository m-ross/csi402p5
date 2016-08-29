/* Author: Marcus Ross
 *         MR867434
 * Description: The arguments this program required are an absolute or relative path to a directory and following that, an optional flag of "-s". The program prints to stdout the name, size in bytes, and date modified of each file in the directory. The output is sorted by increasing order of date modified if the flag is specified.
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bst.h"
#include "proto.h"
#include "const.h"

int main(int argc, char** argv) {
	if (argc < 2 || argc > 3) { /* argc must be 2 or 3 */
		fprintf(stderr, ERR_ARGS);
		return -1;
	}
	
	if (argc == 2) { /* if argc == 2, no flags to examine */
		listFile(argv[1]); /* list the files in dir named by argv[1] */
		return 0;
	}
	
	if (strcmp(argv[2], "-s") != 0) { /* only reach here if argc == 3; throw error if argv[2] is not appropriate flag */
		fprintf(stderr, ERR_FLAG_A);
		return -1;
	}
	else {
		listFileSort(argv[1]); /* list the files in dir named by argv[1], sorted */
	}
	
	return 0;
}

/* This function's argument is a string containing an absolute or relative path for a directory. It opens the directory and returns the pointer. If it fails to open the directory, it displays an error and kills the program. */
DIR* dirOpenChecked(char* path) {
	char errMsg[300];
	DIR *dir;
	
	if (!(dir = opendir(path))) {
		strcat(errMsg, ERR_DIR_OPEN);
		strcat(errMsg, path);
		perror(errMsg);
		exit(-1);
	}
	
	return dir;
}

/* This function's argument is a string containing an absolute or relative path to a directory. It prints to stdout the name, size in bytes, and date modified of all files in the directory. */
void listFile(char *path) {
	DIR *dir;
	struct dirent *dirEntry;
	struct stat *stats;
	char *errMsg;
	struct tm *time;
	unsigned char maxNameLen = 0;
	
	dir = dirOpenChecked(path);
	
	if (chdir(path) < 0) {
		errMsg = (char *) malloc(sizeof(char) * 100);
		strcat(errMsg, ERR_DIR_NAV);
		strcat(errMsg, path);
		perror(errMsg);
		free(errMsg);
		closedir(dir);
		exit(-1);
	}
	
	while (dirEntry = readdir(dir)) { /* loop until dirEntry == NULL */
		if (dirEntry->d_name[0] != '.' || strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
			continue; /* ignore files that are . or .. or aren't hidden */
		}
		
		if (maxNameLen < strlen(dirEntry->d_name)) {
			maxNameLen = strlen(dirEntry->d_name);
		}
	}
	
	rewinddir(dir);
	
	while (dirEntry = readdir(dir)) { /* loop until dirEntry == NULL */
		if (dirEntry->d_name[0] != '.' || strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
			continue; /* ignore files that are . or .. or aren't hidden */
		}
		
		stats = (struct stat *) malloc(sizeof(struct stat));
		
		if(stat(dirEntry->d_name, stats) < 0) {
			errMsg = (char *) malloc(sizeof(char) * 100);
			strcat(errMsg, ERR_FILE);
			strcat(errMsg, dirEntry->d_name);
			perror(errMsg);
			free(errMsg);
			free(stats);
			continue;
		}
		
		time = localtime(&(stats->st_mtime));
		time->tm_year %= 100;
		
		printf("%-*s %10i %02i/%02i/%02i %2i:%02i\n", maxNameLen, dirEntry->d_name, stats->st_size, time->tm_mon + 1, time->tm_mday, time->tm_year, time->tm_hour, time->tm_min);
		
		free(stats);
	}
	
	closedir(dir);
}

/* This function's argument is a string containing an absolute or relative path to a directory. It prints to stdout the name, size in bytes, and date modified of all files in the directory, sorted by increasing order of date modified. */
void listFileSort(char *path) {
	DIR *dir;
	struct dirent *dirEntry;
	struct stat *stats;
	char *errMsg;
	struct tm *time;
	int maxNameLen = 0;
	treeNode *root = NULL, *newNode;
	
	dir = dirOpenChecked(path);
	
	if (chdir(path) < 0) {
		errMsg = (char *) malloc(sizeof(char) * 100);
		strcat(errMsg, ERR_DIR_NAV);
		strcat(errMsg, path);
		perror(errMsg);
		free(errMsg);
		closedir(dir);
		exit(-1);
	}
	
	while (dirEntry = readdir(dir)) { /* loop until dirEntry == NULL */
		if (dirEntry->d_name[0] != '.' || strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
			continue; /* ignore files that are . or .. or aren't hidden */
		}
		
		if (maxNameLen < strlen(dirEntry->d_name)) {
			maxNameLen = strlen(dirEntry->d_name);
		}
		
		stats = (struct stat *) malloc(sizeof(struct stat));
		
		if(stat(dirEntry->d_name, stats) < 0) {
			errMsg = (char *) malloc(sizeof(char) * 100);
			strcat(errMsg, ERR_FILE);
			strcat(errMsg, dirEntry->d_name);
			perror(errMsg);
			free(errMsg);
			free(stats);
			continue;
		}
		
		if (root) { /* if the root of the BST exists */
			newNode = createNode(dirEntry, stats);
			addNode(root, newNode);
		}
		else { /* if the root hasn't been created yet */
			root = createNode(dirEntry, stats);
		}
	}
	
	if (root) { /* don't print anything if root doesn't exist */
		printTree(root, maxNameLen);
	}
	
	closedir(dir);
	treeFree(root);
}