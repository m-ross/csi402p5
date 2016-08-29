/* Author: Marcus Ross
 *         MR867434
 */

DIR* dirOpenChecked(char *);
void listFile(char *);
void listFileSort(char *);
treeNode* createNode(struct dirent *, struct stat *);
void addNode(treeNode *, treeNode *);
void treeFree(treeNode *);
void printTree(treeNode *, int);
void archiveFiles(int, char *, char **);
void printFiles(char *, char *);
void extractFiles(char *);