/* Author: Marcus Ross
 *         MR867434
 */

/* lots of error messages, just in case someone would want to localise this program to another language */
#define ERR_DIR_OPEN "Failed to open directory "
#define ERR_DIR_NAV "Failed to navigate to directory "
#define ERR_FILE "Failed to access file "
#define ERR_ARGS "Error: Incorrent number of arguments. Usage is: p5a path optionalFlag\n"
#define ERR_ARGS_X "Error: Incorrent number of arguments. Usage is: p5b archiveName optionalflag\n"
#define ERR_ARGS_P "Error: Incorrent number of arguments. Usage is: p5b prefix archiveName\n"
#define ERR_ARGS_C "Error: Incorrent number of arguments. Usage is: p5b archiveName file1 file2 ...\n"
#define ERR_FLAG_B "Error: Arguments must contain flag -x, -p, or -c.\n"
#define ERR_ARGS_FEW "Error: Too few arguments.\n"
#define ERR_FLAG_A "Third argument must be \"-s\" if given.\n"
#define ERR_ARCH "Failed to read archive  "
#define ERR_CREAT "Failed to create file "