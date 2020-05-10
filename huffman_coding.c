#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combined_header.h"
#include <limits.h>
/*creat_encode_table
 * This function will take the head of the binary tree created previously and create the reference table used to encode teh file
 *
 * Parameters:
 *  - treeHead: the head of the tree
 *
 * Return Value: the array of structures storing the code values for each letter/val
 */
void create_encode_table(FILE * fp, treeNode * node, long * codeArray, int * stack, int top)
{
   if(node == NULL)
   {
      return;
   }
   if(((node -> left) == NULL) && ((node -> right) == NULL))
   {
      fprintf(fp, "%c:", node -> val);
      //fprintf(stderr, "%c:", node -> val);
      codeArray[node -> val] = codeArray[node -> val] | 1;
      codeArray[node -> val] = codeArray[node -> val] << 1;
      for(int i = 0; i < top; ++i)
      {
         fprintf(fp, "%d", stack[i]);
         //fprintf(stderr, "%d", stack[i]);
         codeArray[node -> val] = codeArray[node -> val] | stack[i];
         codeArray[node -> val] = codeArray[node -> val] << 1;
      }
      codeArray[node -> val] = codeArray[node -> val] >> 1;
      fprintf(fp, "\n");
      //fprintf(stderr, "\n");
   }

   if(node -> left != NULL)
   {
      stack[top] = 0;
      create_encode_table(fp, node -> left, codeArray, stack, top + 1);
   }
   
   if(node -> right != NULL)
   {
      stack[top] = 1;
      create_encode_table(fp, node -> right, codeArray, stack, top + 1);
   }
   return;
}

long write_tree_topology(FILE * treeFp, FILE * outFp, long * totCount, int * topArray, int topTop)
{
   int readVal = 0;
   int count = 0;
   unsigned char buff = 0;
   long topCount = 0;
   //while((readVal = fgetc(treeFp)) != EOF) //&& readVal != '\n')
   //while(fread(&readVal, sizeof(char), 1, treeFp))
   for(int j = 0; j < topTop; j++)
   {
      readVal = topArray[j];
      //fprintf(stderr, "\nreadVal: c: %c | d: %d", readVal, readVal);
      if(readVal != -2 && readVal != -1)
      {
         for(int i = 0; i < 8; i++)
         {
            if(readVal & 1)
            {
               buff = buff >> 1;
               buff = buff | CHAR_MIN;
               count++;
            }
            else
            {
               buff = buff >> 1;
               buff = buff | 0;
               count++;
            }
            if(count == 8)
            {
               //fprintf(stderr, "\nFputting: %hhx", buff);
               fputc(buff, outFp);
               buff = 0;
               count = 0;
               topCount++;
               (*totCount)++;
            }
            readVal = readVal >> 1;
         }
      }
      else
      {
         if(readVal == -1)
         {
            buff = buff >> 1;
            buff = buff | CHAR_MIN;
            count++;
         }
         else
         {
            buff = buff >> 1;
            buff = buff | 0;
            count++;
         }
      } 
      if(count == 8)
      {
         //fprintf(stderr, "\nFputting: %hhx", buff);
         fputc(buff, outFp);
         buff = 0;
         count = 0;
         topCount++;
         (*totCount)++;
      }
      readVal = 0;
   }
   if(count != 0)
   {
      buff = buff >> (8 - count);
      fputc(buff, outFp);
      //fprintf(stderr, "\nfputing %hhx > ", buff);
      topCount++;
      (*totCount)++;
   }
   return(topCount);
}



/*
 * encode
 *  This fucntion will take the file name and reference code array and write the compressed file based on the reference array
 *  Will also have to store some header info in the compressed file
 *
 *  Additional info that could be added (can run file IO function in here or in main, don't know yet):
 *      - total number of characters in compressed file as a long
 *      - total number of characters storing the topology of the coding tree as a long
 *      - total number of charaters in the original uncompressed file
 * 
 * Parameters:
 *  - inFilename: the file to be compressed
 *  - outFilename: the file to write the compressed version
 *
 * Return Value: non
 */

void encode(char * treeFilename, char * inFilename, char * outFilename, long * codeArray, long * countArr, int * topArray, treeNode * treeHead, int topTop)
{
   FILE * inFp = fopen(inFilename, "r");
   FILE * outFp = fopen(outFilename, "w");
   FILE * treeFp = fopen(treeFilename, "r");
   int c = 0;
   unsigned char buff = 0;
   long temp = 0;
   int count = 0;
   long totCount = 0; 
   long origCount = 0;
   long topCount = 0;
   treeNode * node = treeHead;
   long code;
   if(treeHead == NULL)
   {
      totCount += 8 * 3;
      fseek(outFp, 0, SEEK_SET);
      fwrite(&totCount, sizeof(long), 1, outFp);
      fwrite(&topCount, sizeof(long), 1, outFp);
      fwrite(&origCount, sizeof(long), 1, outFp);
      fclose(treeFp);
      fclose(outFp);
      fclose(inFp);
      return;
   }

   fseek(outFp, sizeof(long) * 3, SEEK_SET); // sets cursor to 3 longs down

// Writes the topology of the tree to the file
   //fprintf(stderr, "\n in writing topology");
   topCount = write_tree_topology(treeFp, outFp, &totCount, topArray, topTop); 
   //fprintf(stderr, "\n in encoding");
// Loop does all the encoding of character by character   
   while((c = fgetc(inFp)) != EOF)
   {
      //fprintf(stderr, "\nOnto next char: %c", c); 
      code = codeArray[c];
      node = treeHead;
      //fprintf(stderr, " %lx, ", code);
      temp = 0;
      // bit shifting the code until the start of the huffman code
      while(temp != LONG_MIN)
      {
         code = code << 1;
         temp = code & LONG_MIN;
      }
      code = code << 1;
      //fprintf(stderr, ": %lx | ", code);
      while((node -> left != NULL) || (node -> right) != NULL)
      {
         if(!(code & LONG_MIN))
         {
            //fprintf(stderr, "goin left,%c > ", c);
            node = node -> left;
            buff = buff >> 1;
            buff = buff | 0;
            count++;
         }
         else
         {
            //fprintf(stderr, "goin right,%c > ", c);
            node = node -> right;
            buff = buff >> 1;
            buff = buff | CHAR_MIN;
            count++;
         }
         if(count == 8)
         {
            //fprintf(stderr, "\nfputing %hhx > ", buff);
            fputc(buff, outFp);
            buff = 0;
            count = 0;
            totCount++;
         }
         code = code << 1;
      }
   }
   if(count != 0)
   {
      buff = buff >> (8 - count);
      fputc(buff, outFp);
      //fprintf(stderr, "\nfputing %hhx > ", buff);
      totCount++;
   }

// finding the total number of original character in the file

   for(int i = 0; i < 256; i++)
   {
      if(countArr[i] > 0)
      {
         origCount += countArr[i];
      }
      
   }

   totCount += 8 * 3;
   fseek(outFp, 0, SEEK_SET);
   fwrite(&totCount, sizeof(long), 1, outFp);
   fwrite(&topCount, sizeof(long), 1, outFp);
   fwrite(&origCount, sizeof(long), 1, outFp);
   
   fclose(treeFp);
   fclose(outFp);
   fclose(inFp);
   return;
}

/*
 * decompress
 *  Does the reverse of the encode file, should take in the header info and create a decompressed file
 *
 * Parameters:
 *  - freqFilename: filename for the frequency of each ASCII value in the decompressed file
 *  - topFilename: the filename for the file containing the topology of the binary search tree used to create the compressed file
 *  - codeFilename: the filename for the file contianing the huffman codes for each character in the compressed file
 *  - compFilename: the filename of the compressed file to be decompressed
 *
 * Return Value: none
 * possibly change ths function to return directly decompress into the output file or change function type to create an array of nodes to then write to a file figue it out man
 *
 */
/*
void decompress(char * freqfileName, char * topFilename, char * codeFilename, char * compFilename)
{
    return;
}
*/
