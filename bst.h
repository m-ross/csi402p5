/* Author: Marcus Ross
 *         MR867434
 */

#include <dirent.h>
#include <sys/stat.h>

/* This struct corresponds to each node of a binary search tree, including pointers to the nodes immediately under it. Satellite data includes a struct dirent and struct stat to contain the name, size, and date of each file in the tree. */
typedef struct treeNode {
	struct dirent *dirEntry;
	struct stat *stats;
	struct treeNode *left, *right;
} treeNode;