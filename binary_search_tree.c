#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combined_header.h"

/* new_node
 * creates a new node with the left and right children as NULL, the value as the given value
 *
 */
treeNode * create_node(unsigned char val, long count, int height)
{
   treeNode * newNode = malloc(sizeof(treeNode));
   newNode -> left = NULL;
   newNode -> right = NULL;
   newNode -> next = NULL;
   newNode -> h = height;
   newNode -> count = count;
   newNode -> val = val;

   return(newNode);
}        

/* create_tree
 * creates a tree with size 0, capacitance the size of the count array and an empty array of tree nodes
 *
 */
treeNode * create_tree(long * countArr)
{
   treeNode * listHead = NULL;
   treeNode * buffNode = NULL;
   for(int i = 0; i < 265; i++)
   {
      if(countArr[i] != 0)
      {
         //fprintf(stderr, "\nInserting: '%c'", i);
         //fprintf(stderr, " :with a count of: '%ld'", countArr[i]);
         buffNode = create_node(i, countArr[i], 1);
         listHead = insert_tree_node(listHead, buffNode);
         //("\ndone inserting\n");
         //print_list(listHead);
      }
   }
   //fprintf(stderr, "\n\nFinshed inserting into list");
   //print_list(listHead);
    
   while(listHead != NULL && listHead -> next != NULL)
   {
      buffNode = listHead;
      listHead = listHead -> next -> next; 
      buffNode = pop_and_combine(buffNode);
      listHead = insert_tree_node(listHead, buffNode);
      //print_list(listHead);
   }

   return(listHead);
}

/* insert_tree_node
 * inserts a node into the correct place in the list of nodes
 */
treeNode * insert_tree_node(treeNode * listHead, treeNode * newNode)
{
   treeNode * currNode = listHead;
   if(listHead == NULL || (listHead -> count > newNode -> count))
   {
      newNode -> next = listHead;
      listHead = newNode;
   }
   else
   {
      while(currNode -> next != NULL && currNode -> next -> count < newNode -> count)
      {
         currNode = currNode -> next;
      }
      if(currNode -> next != NULL && currNode -> next -> count == newNode -> count)
      {
         //treeNode * prevNode = currNode;
         while(currNode -> next != NULL && ((currNode -> next -> val < newNode -> val) || (currNode -> next -> h < newNode -> h)) && currNode -> next -> count == newNode -> count)
         {
            //prevNode = currNode;
            currNode = currNode -> next;
         }
         newNode -> next = currNode -> next;
         currNode -> next = newNode;
         //newNode -> next = prevNode -> next;
         //prevNode -> next = newNode;
      }
      else
      {
         newNode -> next = currNode -> next;
         currNode -> next = newNode;
      }
   }
   return(listHead);
}

// THIS ALSO WORKS DO NOT TOUCH
void print_list(treeNode * node)
{
   fprintf(stderr, "\nThe Current linked list is:");
   while(node != NULL)
   {
      fprintf(stderr, "%c|%ld; ", node -> val, node -> count);
      node = node -> next;
   }
   return;
}

/* pop_and_combine
 * pops the top two items out of the array of nodes and makes a new node that has those two as it's children
 */

treeNode * pop_and_combine(treeNode * listHead)
{
   treeNode * first = listHead;
   treeNode * second = listHead -> next;
   
   listHead = second -> next;
   
   // some house keeping to keep me sain
   first -> next = NULL;
   second -> next = NULL;
   int height = first -> h + second -> h;
   long count = first -> count + second -> count;
   treeNode * newHead = create_node((((first -> val) > (second -> val)) ? (first -> val) : (second -> val)), count, height);
   
   newHead -> left = first;
   newHead -> right = second;
   
   return(newHead);
}

void print_tree(treeNode * node)
{
   if(node == NULL)
   {
      return;
   }

   if(node -> left == NULL && node -> right == NULL)
   {
      fprintf(stderr, "%c, ", node -> val);
   }
   print_tree(node -> left);
   print_tree(node -> right);

   return;
}

void free_tree(treeNode * node)
{
   if(node == NULL)
   {
      return;
   }

   free_tree(node -> left);
   free_tree(node -> right);

   free(node);
   return;
}

