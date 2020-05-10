/* ECE 368:
 * Programming Assingment 2
 *
 * Author: Nicholas Gildenhuys
 * Due Date: February 24, 2020 | 23:59
 *
 * Huffman File Compression Program
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include "combined_header.h"

/* main:
 * 
 * Expected Arguments:
 *  - argv[1]: the name of the input file to be compressed; ex: original/gophers 
 *  - argv[2]: name of output file containing the frequencies of occurences of the characters of the input file; ex: gophers.count
 *  - argv[3]: the output file to store the topology information of the constructed Huffman coding trees using pre-order traversal; ex: gophers.tree 
 *  - argv[4]: an output file to store the Huffman code of every character; ex: gophers.code 
 *  - argv[5]: the output file to store the compressed file; ex: gophers.hbt
 *
 * Conditions for Exit Failure:
 *  - Number of arguments if incorrect
 *  - input file does not exists
 *  - program runs out of memory
 *  - a file can not be opened for output
 *  - a file can not be opened successfully
 *  
 * Other Notes:
 *  - The compression program should always produce a compressed file even if the compressed version is larger than the original
 *  - Run with linux command "time" to make sure the runtime is not too long
 *
 * Main structure:
 *  Read file into data struct (haven't figured out what that's gonna be yet)
 *  Output frequencies
 *  Construct tree
 *  Output topology of tree
 *  Output each huffman code for each character into .code file by traversing the tree
 *  Use tree to output Huffman Compressed file by using tree or tgenerated key that was stored in .code file
 *
 */

int main(int argc, char ** argv)
{

   if(argc != 6)
   {
      //("\nIncorrect number of arguments\n");
      return(EXIT_FAILURE);
   }
   long countArr[256] = { 0 }; // stores the number of occurences of each character in the file
   long codeArray[256] = { 0 }; // store the code of each character
   int topArray[(256 * 3) - 1] = { 0 }; // stores the topology of the tree
   int stack[256] = { 0 }; // stack for temporarily storing the binary values when decoding tree
   int stackTop = 0;

   // read the array and frequency of each letter into an array of counts
   long numRead = file_to_array(argv[1], countArr);
   if(numRead == -1) // to check if the funciton was sucessfull
   {
      //("\n file to array failed\n");
      return(EXIT_FAILURE);
   }
   //fprintf(stderr, "\nNum read: %ld", numRead);
   // write the frequencies of each ASCII character to the file
   int chk = write_freq_file(argv[2], countArr);
   if(chk == -1)
   {
      //("\nwrite freq file failed\n");
      return(EXIT_FAILURE);
   }
   // DO NOT MODIFY ABOVE OR FUNCTIONS
   // IT ALL WORKS UP TO THIS POINT
   //fprintf(stderr, "\nHeaded to make the tree");
   treeNode * treeHead = create_tree(countArr);
   //fprintf(stderr, "\nPreorder Print of Tree: ");
   //print_tree(treeHead);  
   //fprintf(stderr, "\ntree prpoerly created\n");
   // creating the code array, and writing it to the file
   FILE * codefp = fopen(argv[4], "w");
   if(codefp == NULL)
   {
      return(EXIT_FAILURE);
   }
   //fprintf(stderr, "\nHeaded to encode");
   // write encoding table to file this is the txt doc not topology 
   create_encode_table(codefp, treeHead, codeArray, stack, stackTop);
   fclose(codefp);
// good up to here
   //fprintf(stderr, "\nCreated Code table");
   // Writing the topology of the tree to the .tree file
  
   stackTop = 0;
   int topTop = 0;
   FILE * topfp = fopen(argv[3], "w");
   if(topfp == NULL)
   {
      return(EXIT_FAILURE);
   }
   read_tree_topology(topfp, treeHead, stack, topArray, &stackTop, &topTop); 
   /*
   fprintf(stderr, "\nPrinting Top Array");
   for(int i = 0; i < topTop; i++)
   {
      fprintf(stderr, "%c|%d, ", topArray[i], topArray[i]);
   }
*/
   fclose(topfp);
// Good up to this point now 
   //write_tree_top(argv[3], topArray, uniqSize);
   // count topology size
   // count file size based on codes and occurances

   encode(argv[3], argv[1], argv[5], codeArray, countArr, topArray, treeHead, topTop);
   free_tree(treeHead);
   //free(huffTree);

     
   return(EXIT_SUCCESS);
}
