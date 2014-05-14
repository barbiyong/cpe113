/* 
==============================================================
taskManager.c

------ Private function -----
+addTaskName
+addTaskInformation
+addTaskDuration
+addTaskOwner
+addTaskRequire
+addTask
------ Public function ------
+createProject
+editOption
+taskSubmit
+displayTask
+writeData
+readData

Create by   Thanat Lapthawan ID: 56070503413
            Chalermpon Thongmotai ID: 56070503403
                DATE: 3/04/2014 
==============================================================
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "taskManager.h"
#include "developerManager.h"
#include "validateFunctions.h"
#include "abstractNetwork.h"
#include "structure.h"

/* 
==========================================================================================================

                                            Private function 
                                            
==========================================================================================================
*/

/*
-------------------------------------------------------------
addTaskName function
    +This function will ask for the task name and check
        it if the task name is valid or not
    +This function will print error message if not valid

ARGUMENT: taskName - to take the input from user and return as a second value 
RETURN: status - return 1 if success or 0 if fail
-------------------------------------------------------------
*/

int addTaskName(char *taskName)
    {
    char input[64];
    int status=1;

    printf(">>> Enter task name(Enter 'DONE' if finish) : ");
    fgets(input,sizeof(input),stdin);
    status=validateTaskName(input);
    if(status == 1)
        {
        sscanf(input,"%s",taskName);
        printf("Add name successful!\n");
        }   
    else
        {
        printf("Task Name is not valid!\n");
        }
    return status;
    }

/*
-------------------------------------------------------------
addTaskInformation function
    +This function will ask for the information of the task 

ARGUMENT: information - to take the input from user as second return value
RETURN: NONE
-------------------------------------------------------------
*/

void addTaskInformation(char *information)
    {
    char input[256];

    memset(input,0,sizeof(input));
    printf(">>> Enter task information : ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",information);
    printf("Add information successful!\n");

    }

/*
-------------------------------------------------------------
addTaskDuration function
    +This function will ask for the duration of a task as a day

ARGUMENT: duration - to take input from user and return as a second value
RETURN: status - if success return 1 and return 0 if fail 
-------------------------------------------------------------
*/

int addTaskDuration(int *duration)
    {
    int status=1;
    char input[64];
    memset(input,0,sizeof(input));
    printf(">>> Enter The Duration of the task : ");
    fgets(input,sizeof(input),stdin);
    status=validateDuration(input);
    if(status==1)
        {
        sscanf(input,"%d",duration);
        printf("Add duration success!\n");
        }
    else
        {
        printf("Fail to add duration!\n");
        }
    return status;
    }


/*
-------------------------------------------------------------
addTaskOwner function
    +This function will ask for owner of the task and check 
            if valid or not
    +If success validate, this function will send owner name to
        addDeveloperList function in validateFunction file

ARGUMENT: owner - take the input from user and return as second return value
RETURN: status - return 1 if success, return 0 if fail
-------------------------------------------------------------
*/
int addTaskOwner(char *owner)
    {
    char input[64];
    int status=1;
    memset(input,0,sizeof(input));
    printf(">>> Enter Task Owner : ");
    fgets(input,sizeof(input),stdin);
    status=validateDeveloperName(input);
    if(status==1)
        {
        sscanf(input,"%s",owner);
        status=addDeveloperList(owner);
        printf("Assign task to owner success!\n");
        }
    else
        {
        printf("Fail to add owner!\n");
        }
    return status;
    }


/*
-------------------------------------------------------------
addTaskRequire function
    +This function will repeat asking for require task in 
        order to start this task

ARGUMENT: taskName - name of task that will assign the requirement
RETURN:NONE
-------------------------------------------------------------
*/
void addTaskRequire(char *taskName)
    {
    char input[64];
    int status=1;
    char requireTask[64];
    VERTEX_T *pRequire = NULL;

    memset(input,0,sizeof(input));
    while(strcmp(requireTask,"DONE")!=0)
        {
        printf("Enter require task('DONE' if no require or finish):");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%s",requireTask);
        if(strcmp(requireTask,"DONE")!=0)
            {
            status=findRequireTask(requireTask);
            if(status==1)
                {
                status=addEdge(taskName,requireTask);
                if(status == -1)
                    {
                    printf("Already have this requirement!\n");
                    }
                else if (status == 0)
                    {
                    printf("Error Allocate Adjacent!\n");
                    }
                else
                    {
                    printf("Add requirement success!\n");
                    }    
                }
            else
                {
                printf("Task name is not valid!\n");
                }
            }
        else
            {
            printf("Done adding require task\n");
            }
        }
    }

/*
-------------------------------------------------------------
addTask function
    +This function will call other function to create a task
        then calculate the end date of the project

ARGUMENT:pPrpject - struct that keep the project endDate
RETURN:status - return 1 if success, return 0 if fail
-------------------------------------------------------------
*/

int addTask(PROJECT_T *pProject)
    {
    char input[64];
    char taskName[64];
    char information[256];
    char owner[64];
    int duration=0;
    int status=1;
    VERTEX_T *pTask;
    printf("\n---------- Add Task ----------\n");
    status=addTaskName(taskName);
    if(strcmp(taskName,"DONE")==0)
        {
        printf("End Adding Task\n");
        return 2;
        }
    if(status == 0)
        {
        printf("Fail to initialize task name\n");
        return 0;
        }
    addTaskInformation(information);
    status=addTaskDuration(&duration);
    if(status == 0)
        {
        printf("Fail to initialize task duration\n");
        return 0;
        }
    status=addTaskOwner(owner);
    if(status == 0)
        {
        printf("Fail to initialize task owner\n");
        }
    pTask=(VERTEX_T*)calloc(1,sizeof(VERTEX_T));
    strcpy(pTask->key,taskName);
    strcpy(pTask->task,taskName);
    strcpy(pTask->information,information);
    pTask->duration=duration;
    strcpy(pTask->owner,owner);
    addTaskToList(pTask);
    addTaskRequire(taskName);

    calculateEndDate(pProject->startDate,duration,pProject->endDate,0);
    printf("\n------------------------------\n");
    return 1;
    }

/*
-------------------------------------------------------------
EditMenu function
    +This function will display the menu for edit the task

ARGUMENT:NONE
RETURN: option - Return 1 to 6 
-------------------------------------------------------------
*/

int EditMenu()
    {
    char input[32];
    int option = -1;
    while(option < 0)
        {
        printf("-- 1 --:Edit task name\n");
        printf("-- 2 --:Edit task information\n");
        printf("-- 3 --:Edit task duration\n");
        printf("-- 4 --:Edit task owner\n");
        printf("-- 5 --:Add task\n");
        printf("-- 6 --:Delete task\n");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%d",&option);
        if((option<1)||(option>6))
            {
            printf("Input option is not valid\n");
            option = -1;
            }
        }
    return option;
    }
/*
-------------------------------------------------------------


ARGUMENTซ
RETURN: 
-------------------------------------------------------------
*/
int checkConnect()
    {
    VERTEX_T * pCheck = NULL;
    do
        {
        pCheck=(VERTEX_T*)checkNetworkConnect();
        if(pCheck != NULL)
            {
            printf(" This Task is unconnect or independence %s\n", pCheck ->task);
            printf("Do you want to ");
            }
        }while(pCheck != NULL);
    }


/* 
==========================================================================================================

                                            Public function 

==========================================================================================================
*/


/*
-------------------------------------------------------------
createProject function
    +This function will create the project and repeat asking 
        for the task of this project
    +If project already exist function will not create other 
        project

ARGUMENT: pPrpject - struct that keep data of the project
RETURN: status - return 1 if success, 0 if fail , -1 if project
            already exist
-------------------------------------------------------------
*/
int createProject(void *project)
    {
    PROJECT_T * pProject = (PROJECT_T*) project;
    char input[256];
    char projectName[64];
    char startProjectDate[32];
    char endDate[32];
    int dateStatus=0;
    int status=1;/* 1 if success 0 if fail*/

    if(pProject->exist == 0)
        {
        printf(">>> Enter Project name : ");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%s",projectName);
        while(dateStatus==0)
            {
            printf(">>> Enter Start Project's date : ");
            fgets(input,sizeof(input),stdin);
            dateStatus=validateStartProjectDate(input,&pProject);
            strcpy(endDate,startProjectDate);
            }
        strcpy(pProject->projectName,projectName);
        strcpy(pProject->endDate,endDate);
        clearGraph();
        while(status != 2)
            {
            status = addTask(pProject);
            }
        pProject->exist = 1;
        }
    else
        {
        status= -1;
        }
    return status;
    }


/*
-------------------------------------------------------------
editOption function
    +This function will edit the task that user want

ARGUMENT: pProject - struct that store data of the project
RETURN: status- return 1 if success,0 if fail
-------------------------------------------------------------
*/
int editOption(void *project)
    {
    PROJECT_T * pProject = (PROJECT_T*)project;
    char input[256];
    char taskName[64];
    int option;
    int status=0;
    VERTEX_T *pTask = NULL;

    option=EditMenu();
    if(option != 5)
        {
        memset(input,0,sizeof(input));
        printf(">>> Enter task name : ");
        fgets(input,sizeof(input),stdin);
        status=validateTaskName(input);
        if(status==1)
            {
            sscanf(input,"%s",taskName);
            if(strcasecmp(taskName,"EXIT")==0)
                {
                return -1;
                }
            else if(strcasecmp(taskName,"DISPLAY")==0)
                {
                findTaskToDisplay();
                }
            pTask = findTask(taskName);
            if(pTask == NULL)
                {
                return 0;
                }
            }
        }
    switch(option)
    {
    case 1:
        printf("== Edit Task Name ==\n");
        memset(input,0,sizeof(input));
        printf(">>> Change Task Name to : ");
        fgets(input,sizeof(input),stdin);
        status = validateTaskName(input);
        if(status ==1)
            {
            sscanf(input,"%s",pTask->task);
            }
        break;
    case 2:
        printf("== Edit task information ==\n");
        memset(input,0,sizeof(input));
        printf(">>> Change Task information to : ");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%s",pTask->information);
        break;
    case 3:
        printf("== Edit Task Duration ==\n");
        memset(input,0,sizeof(input));
        printf(">>> Change Task Duration to : ");
        fgets(input,sizeof(input),stdin);
        status=validateDuration(input);
        if(status == 1)
            {
            sscanf(input,"%d",&pTask->duration);
            calculateEndDate(pProject->startDate,pTask->duration,pProject->endDate,0);
            }
        break;
    case 4:
        printf("== Edit Task Owner ==\n");
        memset(input,0,sizeof(input));
        printf(">>> Change Task Owner : ");
        fgets(input,sizeof(input),stdin);
        status=validateDeveloperName(input);
        if(status == 1)
            {
            sscanf(input,"%s",pTask->owner);
            status=addDeveloperList(pTask->owner);
            if(status == 0)
                printf("Allocate Developer list fail\n");
            }
        break;
    case 5:
        printf("== Add Task ==\n");
        status=0;
        while(status==0)
            {
            memset(input,0,sizeof(input));
            printf(">>> Enter Task name : ");
            fgets(input,sizeof(input),stdin);
            status=addTask(pProject);
            if(status == 0)
                {
                printf("Fail to add task\n");
                }
            }
        break;
    case 6:
        printf("== Delete Task ==\n");
        char* returnTask=removeTask(taskName);
        if(returnTask != NULL)
            {
            printf("Delete task: -- %s -- success\n", returnTask);
            }
        else
            {
            printf("Error while delete\n");
            }
        break;
    }
    
    return status;
    }


/*
-------------------------------------------------------------
taskSubmit function
    +This function will ask for task name that user want to 
            submit and send it to findTaskSubmit in linkedListNetwork file

ARGUMENT: NONE
RETURN:status - return 1 if success, 0 if fail
-------------------------------------------------------------
*/
int taskSubmit()
    {
    char input[64];
    int status=1;

    printf(">>> Submit task : ");
    fgets(input,sizeof(input),stdin);
    status=findTaskSubmit(input);
    return status;
    }

/*
-------------------------------------------------------------
displayTask function
    +This function will display the current task and the 
            other task that is uncomplete

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/
void displayTask(void* pProject)
    {
    findTaskToDisplay(pProject);
    }
/*
-------------------------------------------------------------
programManual function
    +This function will display the manual of the program 
        from the manual text file

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/

void programManual()
    {
    FILE *pIn = NULL;
    char input[256];
    char text[256];
    pIn=fopen("manual.txt","r");
    if(pIn == NULL)
        {
        printf("Error can not open the file from text file\n");
        }
    while(fgets(input,sizeof(input),pIn)!=NULL)
        {
        sscanf(input,"%s",text);
        printf("%s\n",text);
        memset(input,0,sizeof(input));
        }
    fclose(pIn);
    }
/*
-------------------------------------------------------------
programManual function
    +This function will display the manual of the program 
        from the manual text file

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/
void deleteProject(void *project)
    {
    PROJECT_T * pProject = (PROJECT_T*)project;
    char input[16];
    int option=0;
    do
        {
        printf("Are you sure to delete (Yes - 1 / No - 2):\n");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%d",&option);
        }   
    while(option < 1 && option > 2);

    if(option == 1)
        {
        clearGraph();
        printf("22\n");
        free(pProject);
        printf("23\n");
        }
    else 
        {
        printf("Cancel deleting the project\n");
        }
    }
