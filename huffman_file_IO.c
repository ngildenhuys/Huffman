#include <stdio.h>
#include <stdlib.h>
#include "combined_header.h"
#include <math.h>
/*
 * file_to_array
 *  This function should take the file anf parse it into an array of treeNodes that will later on be compiled into a full tree
 *
 * Parameters:
 *  - filename: the filename
 *  - countArr: static array of 256 items that keeps track of how many
 *              occurences of each ASCII character occure in the file
 *
 * Return Value: 1 for success -1 for failure
 */
long file_to_array(char * filename, long * countArr)
{
   FILE * fp = fopen(filename, "r");
   if (fp == NULL)
   {
      //("\nFirst fopen failed\n");
      return(-1);
   }
   long numRead = 0;
   int chr = 0;
   while(chr != EOF)
   {
      numRead++;
      chr = fgetc(fp);
      countArr[chr] += 1;
   }

   fclose(fp);
   return(numRead);
}


/*
 * write_freq_file
 *  Writes the number of occurences each ASCII value has in the original file
 *
 * Parameters:
 *  - filename: the filename to output to
 *  - treeArr: the array of tree nodes that stores thier occurences
 * 
 * Return Value: int for fail or success, fail is -1
 */
int write_freq_file(char * filename, long * countArr)
{
   FILE * fp = fopen(filename, "w");
   if(fp == NULL)
   {
      //("\nfopen for freq file failed\n");
      return(-1);
   }
   fwrite(countArr, sizeof(long), 256, fp); 
   //("\nWrote the count file\n");
   
   fclose(fp);
   return(1);
}

/*
 * write_top_file
 *  Writes the topology of the binary search tree to the an output file
 *
 * Parameters:
 *  - filename: the name of the file to output to 
 *  - treeHead: the head of the binary tree
 *
 * Return Value: 1 for succes -1 for fail
 */
/*
int write_top_file(char * filename, treeNode * treeHead)
{
   return(1);
}
*/

int read_tree_topology(FILE * fp, treeNode * node, int * stack, int * topArray, int * top, int * topTop)
{
   if(node == NULL)
   {
      return(0);
   }

   if(((node -> left) == NULL) && ((node -> right) == NULL))
   {
      for(int i = 0; i < *top; i++)
      {
         fprintf(fp, "%d", stack[i]);
      }
      topArray[*topTop] = -1;
      fprintf(fp, "%d", 1);
      ++(*topTop);
      topArray[*topTop] = node -> val;
      ++(*topTop);
      //if(node -> val == 0 || node -> val == 1)
      //   fprintf(stderr, "\nThere is a 1/0: %d", node -> val);
      fprintf(fp, "%c", node -> val);
      *top = 0;
   }

   if(node -> left)
   {
      topArray[*topTop] = -2; 
      stack[*top] = 0;
      ++(*top);
      ++(*topTop);
      read_tree_topology(fp, node -> left, stack, topArray, top, topTop);
   }

   if(node -> right)
   {
      //topArray[*top] = 1;
      //++(*top);
      read_tree_topology(fp, node -> right, stack, topArray, top, topTop);
   }
   return(1);
}

