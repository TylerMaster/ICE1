#include "mylink-list-fixed.h"

int main ()
{
    // Activity 2: creating list pointer
    PlayerPtr startPtr;
    // Activity 3: initialising the start of the list
    startPtr = NULL;

    char value[10];
    char choice[10];
    int grade;
    char name[20];
    int convertedChoice;

    menu ();
    fgets(choice, sizeof(choice), stdin);
    convertedChoice = strtol(choice, NULL, 10);
    printf("Choice: %d", convertedChoice);
    while (convertedChoice != 5)
    {
        switch (convertedChoice) // Activity 4: Test your link list tasks
        {
            int convertedValue;
            int convertedValue1;
            int convertedValue2;
            case 1:
                printf ("\nEnter Value for Player for Insertion: \n");
                fgets(value, sizeof(value), stdin);
                convertedValue = strtol(value, NULL, 10);
                printf("Player Number Selected (Value Entered): %d", convertedValue);

                printf ("\nEnter Grade for Player for Insertion: \n");
                grade = getchar();
				getchar();
				char grade1 = (char)grade;

                printf("Performance Grade: %c", grade1);

                printf ("\nEnter Player Name for Player for Insertion: \n");
                fgets(name, sizeof(name), stdin);

                printf("Player Name: %s", name);
				
				

                startPtr = addPlayer (startPtr, convertedValue, &grade1, name);
                printList (startPtr);
                printf ("\n");

                // Clearing the input buffer
                while (getchar() != '\n');
                break;
            case 2:
                printf ("\nEnter Player Number for deletion : ");
                fgets(value, sizeof(value), stdin);
                convertedValue1 = strtol(value, NULL, 10);

                startPtr = removePlayer (startPtr, convertedValue1);
                printList (startPtr);
                printf ("\n");

                // Clearing the input buffer
                while (getchar() != '\n');
                break;
            case 3:
                printf ("\nEnter Player Number to View : ");
                fgets(value, sizeof(value), stdin);
                convertedValue2 = strtol(value, NULL, 10);

                viewPlayer (startPtr, convertedValue2);
                printf ("\n");

                // Clearing the input buffer
                while (getchar() != '\n');
                break;
            case 4:
                printList (startPtr);
                printf ("\n");
                break;
            default:
                printf ("Invalid Option... Please Try Again \n");
                break;
        }

        // Clearing the input buffer
        while (getchar() != '\n');

        menu ();
        fgets(choice, sizeof(choice), stdin);
        convertedChoice = strtol(choice, NULL, 10);
    }

    return 0;
} // end of main