/* =================================================

    developerManager.c

    manage the list of developer name
    create by:Zodha 
    DATE: 3/04/2014 

==================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structure.h"


DEVELOPER_T *devListHead = NULL;
DEVELOPER_T *devListTail = NULL;


/* validateDevloperName
 * This function will validate the developer name
 * no digits, no punctuations, and no have the same name.
 * Argument : name - string for developer's name
 * Returns : 1 for success, 0 for failure.
 */
int validateDeveloperName(char *name)
    {
    int bOk = 1;                    /* keep status of this function */
    int length = 0;             /* length of string */
    int i = 0;                      /* index */
    DEVELOPER_T * pDevCurrent = devListHead;

    if(name[strlen(name)-1] == '\n')
        name[strlen(name)-1] = '\0';

    length = strlen(name);

    for(i = 0;i < length; i++)
        {
        if( (isdigit(name[i])) || (ispunct(name[i])) )
            {
            bOk = 0;
            break;
            }
        }

    while(pDevCurrent!= NULL && bOk != 0)
        {
        if(strcasecmp(pDevCurrent->name,name) == 0)
            {
            bOk = 0;
            break;
            }
        pDevCurrent = pDevCurrent->pNext;
        }

    return bOk;
    }

/* addDeveloperList
 * This function will add developer to developer list.
 * Argument : developerName - name of developer
 * Returns : 1 for success, 0 for failure.
 */
int addDeveloperList(char *developerName)
        {
        int bOk = 1;                /* keep status of this function */
        DEVELOPER_T * pDevCurrent = NULL;       /* pointer to develop list */

        pDevCurrent = (DEVELOPER_T*) calloc(1,sizeof(DEVELOPER_T));     
        if (pDevCurrent == NULL)
            {
            bOk = 0;
            return bOk;    
            }

        strcpy(pDevCurrent->name,developerName);
        if (devListHead == NULL)
            {
            devListHead = pDevCurrent;
            devListTail = pDevCurrent;              
            }
        else
            {
            devListTail->pNext = pDevCurrent;
            devListTail = pDevCurrent;
            }

        return bOk;
        }
