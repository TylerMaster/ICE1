// Header file containing link-list definitions & implementation methods
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure Definition
struct player{ // items on a shelf in a supermarket
   int playerId;
   char performanceGrade;
   char playerName[20];
   struct player *nextPtr;
};

// Structure renaming
typedef struct player Player;
typedef struct player *PlayerPtr;

/*Function Prototypes*/
PlayerPtr makePlayer(int value, char *grade, char name[20]);
PlayerPtr addPlayer(PlayerPtr sPtr, int value, char *grade, char name[20]);
void printList(PlayerPtr sPtr);
PlayerPtr removePlayer(PlayerPtr sPtr, int value);
void viewPlayer(PlayerPtr sPtr, int value);
void menu ();

PlayerPtr makePlayer(int value, char *grade, char name[20])
{
  PlayerPtr newPtr = malloc(sizeof(Player));
  if(newPtr == NULL)
  {
     puts("Memory Issues... Item Not Created");
  }
  else{
     newPtr->playerId = value;
     newPtr->performanceGrade = *grade;
     strcpy (newPtr->playerName,name);
     newPtr->nextPtr = NULL;
  }
  return newPtr;
}

PlayerPtr addPlayer(PlayerPtr sPtr, int value, char *grade, char name[20])
{
    PlayerPtr previousPtr, currentPtr, newPtr;
    previousPtr = NULL;
    currentPtr = sPtr;

   if (currentPtr != NULL){
    printf("%d", currentPtr->playerId);
}
    newPtr = makePlayer(value, grade, name);

    // Check if the player ID already exists in the list
    while (currentPtr != NULL && value >= currentPtr->playerId)
    {
        // If the player ID already exists, print an error message and return
        if (value == currentPtr->playerId)
        {
            printf("Player ID %d already exists in the list. Please enter a unique ID.\n", value);
            free(newPtr); // Free the memory allocated for the new player
            return sPtr;  // Return the original list without adding the player
        }
        previousPtr = currentPtr;
        currentPtr = currentPtr->nextPtr;
    }

    // Insert the new player into the list
    if (previousPtr == NULL)
    {
        newPtr->nextPtr = sPtr;
        sPtr = newPtr;
    }
    else
    {
        previousPtr->nextPtr = newPtr;
        newPtr->nextPtr = currentPtr;
    }
    return sPtr;
}

void printList(PlayerPtr sPtr)
{
  PlayerPtr tempPtr = sPtr;
  if(tempPtr == NULL)
  {
     puts("List is Empty... Nothing to Print");
     return;
  }
  else{
     puts("The Items in the List are");
     while(tempPtr != NULL)
     {
       printf("%d %c %s\n", tempPtr->playerId,tempPtr->performanceGrade, tempPtr->playerName );
       tempPtr = tempPtr->nextPtr;
     }

  }
}

void menu ()
{
  printf ("\t1: Insert Player into Ordered List\n");
  printf ("\t2: Remove Player from List\n");
  printf ("\t3: View Player from List\n");
  printf ("\t4: Printing the List\n");
  printf ("\t5: Exit\n");
  printf ("\tEnter Choice: ");
}

PlayerPtr removePlayer(PlayerPtr sPtr, int value)
{
  PlayerPtr previousPtr, currentPtr, tempPtr;
  previousPtr = NULL;
  currentPtr = sPtr;
  // revisit here
  if (sPtr == NULL) {
     puts("List is Empty... Nothing to Delete");
     return sPtr;
  }
  while (currentPtr != NULL && value != currentPtr->playerId){
     previousPtr = currentPtr;
     currentPtr = currentPtr->nextPtr;
  } // exit traversal
  if(currentPtr == NULL){ // check if node is not in the list
     printf("%d is [not] found in the list\n", value);
  }
  else if(previousPtr == NULL){ // node found at the front list
    tempPtr = sPtr;
    printf("Node %d %c %s being deleted\n", tempPtr->playerId,tempPtr->performanceGrade, tempPtr->playerName); // inform user
    sPtr = sPtr->nextPtr;
    free(tempPtr);
  }
  else{ // node found elsewhere in the lis
    tempPtr = currentPtr;
    printf("Node %d %c %s being deleted\n", tempPtr->playerId,tempPtr->performanceGrade, tempPtr->playerName); // inform user
    previousPtr->nextPtr = currentPtr->nextPtr;
    free(tempPtr);
  }
  return sPtr;
}
void viewPlayer(PlayerPtr sPtr, int value)
{
  PlayerPtr currentPtr = sPtr;
  int position = 0;
  if (sPtr == NULL) {
     puts("List is Empty... Nothing to View");
     return;
  }
  while (currentPtr != NULL && value != currentPtr->playerId){
     currentPtr = currentPtr->nextPtr;
     position++;
  } // exit traversal
  if(currentPtr == NULL){ // check if node is not in the list
     printf("%d is [not] found in the list\n", value);
  }
  else {
     printf("Node %d %c %s found at position %d\n", currentPtr->playerId,currentPtr->performanceGrade, currentPtr->playerName, position+1); // inform user
  }
}