/* Author: Marcus Ross
 *         MR867434
 */

#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bst.h"

/* This function allocates memory for a struct treeNode, allocates memory for the treeNode's string, then returns a pointer to the treeNode. */
treeNode* createNode(struct dirent *dirEntry, struct stat *stats) {
	treeNode *node = (treeNode *) malloc(sizeof(treeNode));
	node->dirEntry = dirEntry;
	node->stats = stats;
	node->left = node->right = NULL;
	return node;
}

/* This function's arguments are a struct treeNode representing the root of a BST and another struct treeNode that is to be added to the BST. It adds the node to the tree by increasing order of date modified. */
void addNode(treeNode *root, treeNode *newNode) {
	if (newNode->stats->st_mtime < root->stats->st_mtime) { /* if the new file has an earlier date than the root */
		if (root->left) { /* if left child exists */
			addNode(root->left, newNode); /* add to left subtree */
		}
		else { /* if left child doesn't exist */
			root->left = newNode; /* add to left */
		}
	}
	else {
		if (newNode->stats->st_mtime > root->stats->st_mtime) { /* if the new file has a later date than the root */
			if (root->right) { /* if right child exists */
				addNode(root->right, newNode); /* add to right subtree */
			}
			else { /* if right child doesn't exist */
				root->right = newNode; /* add to right */
			}
		}
		else { /* if the times are equal, sort alphabetically */
			if (strcmp(newNode->dirEntry->d_name, root->dirEntry->d_name) < 0) { /* if new filename precedes root filename */
				if (root->left) { /* if left child exists */
					addNode(root->left, newNode); /* add to left subtree */
				}
				else { /* if left child doesn't exist */
					root->left = newNode; /* add to left */
				}
			}
			else { /* if new filename succeeds root filename */
				if (root->right) { /* if right child exists */
					addNode(root->right, newNode); /* add to right subtree */
				}
				else { /* if right child doesn't exist */
					root->right = newNode; /* add to right */
				}
			}
		}
	}
}

/* This function's argument is a pointer to a struct treeNode. It performs free() on the treeNode and all of its children. */
void treeFree(treeNode *node) {	
	if (node->left) { /* free left subtree if it exists */
		treeFree(node->left);
	}
	
	if (node->right) {  /* free right subtree if it exists */
		treeFree(node->right);
	}
	
	free(node); /* free this node */
}

/* This function's arguments are a pointer to a struct treeNode and an int with the maximum name length of all the nodes in the BST represented by the treeNode. It prints to stdout metadata on the file in each node in the tree. */
void printTree(treeNode *root, int maxNameLen) {
	struct tm *time;
	
	if (root->left) {
		printTree(root->left, maxNameLen);
	}
	
	time = localtime(&(root->stats->st_mtime));
	time->tm_year %= 100;
	
	printf("%-*s %10i %02i/%02i/%02i %2i:%02i\n", maxNameLen, root->dirEntry->d_name, root->stats->st_size, time->tm_mon + 1, time->tm_mday, time->tm_year, time->tm_hour, time->tm_min);
	
	if (root->right) {
		printTree(root->right, maxNameLen);
	}
}