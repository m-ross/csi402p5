/* Author: Marcus Ross
 *         MR867434
 * Description: The arguments this program requires are one flag from among "-c", "-x", and "-p". If "-c" is specified, the next argument must be the filename of an unformatted archive file to be created; following that, up to 255 filenames may be supplied which refer to files that will be added to the archive. If "-x" is specified, the only other argument must be the filename of an existing unformatted archive file to be read from; the program will extract each file contained in the archive file. If "-p" is specified, the next argument must be a string to search for in the filenames, and the next argument after that must be the filename of an existing unformatted archive file to be read from; the program will print to stdout the name and size of each file whose name starts with the specified string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "proto.h"
#include "const.h"

int main(int argc, char** argv) {
	if (argc < 3) { /* each of the flags require argc be 3 or more */
		fprintf(stderr, ERR_ARGS_FEW);
		exit(-1);
	}
	
	if (strcmp(argv[1], "-x") == 0) { /* if the flag is x */
		if (argc != 3) { /* check that argc is as expected */
			fprintf(stderr, ERR_ARGS_X);
			exit(-1);
		}
		else { /* if no error, run relevant function */
			extractFiles(argv[2]); /* need only the archive name */
		}
	}
	else {
		if (strcmp(argv[1], "-p") == 0) { /* if the flag is p */
			if (argc != 4) { /* check that argc is as expected */
				fprintf(stderr, ERR_ARGS_P);
				exit(-1);
			}
			else { /* if no error, run relevant function */
				printFiles(argv[2], argv[3]); /* argv[2] = prefix; argv[3] = archiveName */
			}
		}
		else {
			if (strcmp(argv[1], "-c") == 0) { /*if the flag is c */
				if (argc < 4) { /* check that argc is as expected */
					fprintf(stderr, ERR_ARGS_C);
					exit(-1);
				}
				else { /* if no error, run relevant function */
					archiveFiles(argc - 3, argv[2], &argv[3]); /* decrement argc by 3 to preserve only the count of filenames; argv[1] = archiveName; argv[2+] = fileNames */
				}
			}
			else { /* if the flag wasn't x or p or c */
				fprintf(stderr, ERR_FLAG_B);
				exit(-1);
			}
		}
	}
}