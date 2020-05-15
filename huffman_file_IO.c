#include "combined_header.h"
/*
 * file_to_array
 *  This function should take the file and parse it into an array of treeNodes that will later on be compiled into a full tree
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
      fprintf(stderr, "\nFailed to open the file to be compressed\n");
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
      fprintf(stderr, "\nFailed to open the frequency write file\n");
      return(-1);
   }
   fwrite(countArr, sizeof(long), 256, fp); 
   //("\nWrote the count file\n");
   fclose(fp);
   return(1);
}

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

treeNode * create_tree_from_file(char * filename)
{
   FILE * fp = fopen(filename, "r");
   if(fp == NULL)
   {
      fprintf(stderr, "\nFailed to open compressed file\n");
      return(NULL);
   }
   long sizeOfTop;
   int chk = fseek(fp, 0, sizeof(long)); // seek to where the # of charachers that store the topology info is
   chk = fread(&sizeOfTop, sizeof(long), 1, fp);

   

}

void tree_from_top(FILE * fp, long topSize, treeNode ** root, char readB, int bitPos, long numRead)
{
   if(numRead >= topSize)
   {
      return;
   }
   int curBit = findCurBit(fp, &readB, &bitPos); //returns the current bit and updates the read bit if the previous bit position was farther than 8
   if(curBit == 0)
   {
      // create Head
      if((*root) -> left != NULL)
      {
         (*root) -> left = // newly created head;
         tree_from_top(fp, topSize, &((*root) -> left), readB, bitPos, numRead);
      }
      else
      {
         (*root) -> right = // newly created head;
         tree_from_top(fp, topSize, &((*root) -> right), readB, bitPos, numRead);
      }
   }
   while(curBit == 1)
   {
      char readChar = findChar(fp, &readB, &bitPos);
      // create leaf node
      if((*root) -> left != NULL)
      {
         (*root) -> left = //new leafNode;
      }
      else
      {
         (*root) -> right = // new leafNode;
      }
      curBit = findeCurBit(fp, &readB, &bitPos);
   }






