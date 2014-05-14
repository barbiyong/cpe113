/* 
==============================================================
projectSchedule.c

+ main function
+ showMenu
+ mainMenuOption

start the program, call menu and get a return value
show message that function is run success or fail.

program still need a readData and writeData to save the data
- program can not be save or load

Create by Thanat Lapthawan ID: 56070503413
                    DATE: 3/04/2014 
==============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "taskManager.h"
#include "dataManager.h"
#include "structure.h"

/*
-------------------------------------------------------------
showMenu function
    + this function will display menu to user

ARGUMENT : pProject that read from file
RETURN : option from 1 to 7

-------------------------------------------------------------
*/
int showMenu(PROJECT_T *pProject)
    {
    int option=-1;
    char input[16];
    while(option < 0)
        {
        printf("===============================================\n\n");

        if(pProject->exist == 0)
            printf("-- 1 -- :Create Project\n");
        if(pProject->exist == 1)
            {
            printf("-- 2 -- :Edit Project's Task\n");
            printf("-- 3 -- :Submit The Task\n");
            printf("-- 4 -- :Display Task Schedule\n");
            printf("-- 5 -- :Delete project\n");
            }
        printf("-- 6 -- :Manual\n");
        printf("-- 7 -- :Exit program\n");
        printf("\n===============================================\n\n");
        printf(">>> Enter option : ");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%d",&option);
        printf("\n");
        if((option<1) || (option>7))
            {
            printf("Input option is not valid!\n");
            option = -1;
            }
        if(((option>=2) && (option<=5)) && (pProject->exist != 1))
            {
            printf("Project doesn't exist!\n");
            option = -1;
            }
        if((option == 1)&&(pProject->exist == 1))
            {
            printf("Project already exist!\n");
            option=-1;
            }
        }
    return option;
    }

/*
-------------------------------------------------------------
mainMenuOption function
    +This function will take user option can call function
        from the option user had input
    +This function will print success or error message

ARGUMENT: pProject that read from file 
RETURN: option if user input 5 (exit) it will end loop 
        in main function
-------------------------------------------------------------
*/

int mainMenuOption(PROJECT_T *pProject)
    {
    int option=0;
    int status=1;
    
    option=showMenu(pProject);
    switch(option)
        {
        case 1:
            status = createProject(pProject);
            if(status==0)
                {
                printf("Fail to create Project!\n");
                }
            else if(status == -1)
                {
                printf("Project already exist!\n");
                }
            else 
                {
                printf("Create project successful!\n");
                }
            break;
        case 2:
            status = editOption(pProject);
            if(status == 0)
                {
                printf("Fail to Edit Project!\n");
                }
            else if(status == -1)
                {
                printf("Exit Edit Menu\n");
                }
            else
                {
                printf("Edit Task successful!\n");
                }
            break;
        case 3:
            status = taskSubmit();
            if(status == 0)
                {
                printf("Fail to Submit task!\n");
                }
            else if(status == -1)
                {
                printf("Task doesn't exist!\n");
                }
            else 
                {
                printf("Submit task successful!\n");
                }
            break;
        case 4:
            displayTask();
            break;
        case 5:
            deleteProject(pProject);
            break;

        case 6:
            programManual();
            break;
        case 7:
            printf("==              End Program                 ==\n");
            printf("==  Project Schedule program by Zodha Team  ==\n");
            break;

        }
    return option;
    }

/*
-------------------------------------------------------------

Note: readData and writeData is not done

-------------------------------------------------------------
*/

int main(int argc,char *argv[])
    {
    int option=0;
    char inputFile[256];
    memset(inputFile,0,sizeof(inputFile));

    PROJECT_T *pProject=NULL;
    pProject = (PROJECT_T *) calloc(1,sizeof(PROJECT_T));
    if(pProject == NULL)
        {
        printf("Error Can't start program correctly!\n");
        exit(0);
        }
    if(argc == 2)
        {
        strcpy(inputFile,argv[1]);
        readData(inputFile,(PROJECT_T *) pProject,1);    
        pProject->exist = 1;
        }
    if(pProject->exist != 1)
        {
        pProject->exist = 0;
        }


    while (option != 7)
        {
        option = mainMenuOption(pProject);
        }

    writeData("backupSchedule.txt",(PROJECT_T *) pProject);
    return 0;
    }


