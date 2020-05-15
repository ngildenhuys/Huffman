/*
 * header file for huffman_file_IO.c
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <limits.h>
// binary search tree declariations
typedef struct _treeNode
{
   unsigned char val; //for heads this values should be -1
   long count;
   int h; // the heght of a node
   struct _treeNode * left; //for leafs (the ends of the tree) should be NULL
   struct _treeNode * right; //for leafs this shoudl be NULL
   struct _treeNode * next; // the next item of the linked list 
} treeNode;

// binary tree declarations
treeNode * create_node(unsigned char val, long count, int height);
treeNode * create_tree(long * countArr);
treeNode * insert_tree_node(treeNode * huffTree, treeNode * addNode);
treeNode * pop_and_combine(treeNode * listHead);
void free_tree(treeNode * node);
void print_tree(treeNode * node);

// file io declariations
long file_to_array(char * filename, long * countArr);
int write_freq_file(char * filename, long * countArr);
int write_top_file(char * filename, treeNode * treeHead);
int write_code_file(char * filename, unsigned char * codeArray, long * countArray);
int read_tree_topology(FILE * fp, treeNode * node, int * stack, int * topArray, int * top, int * topTop);
//int write_tree_top(char * filename, unsigned char * topArray, unsigned char uniqSize);

// coding declariations
void create_encode_table(FILE * fp, treeNode * node, long * codeArray, int * stack, int top);
void encode(char * treeFilename, char * inFilename, char * outFilename, long * codeArray, long * countArr, int * topArray, treeNode * treeHead, int topTop);
void print_list(treeNode * node);
long write_tree_topology(FILE * treeFp, FILE * outFp, long * totCount, int * topArray, int topTop);
