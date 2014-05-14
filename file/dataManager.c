#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "abstractNetwork.h"
#include "structure.h"

/* 
==========================================================================================================

											Private function 
											
==========================================================================================================
*/
int addProject(char *data,PROJECT_T *pProject)
	{
	int status = 1;
	char *pToken = NULL;

	pToken = strtok(data,"|");
	if(pToken == NULL)
		return 0;
	strcpy(pProject->projectName,pToken);

	pToken = strtok(NULL,"|");
	if(pToken == NULL)
		return 0;
	strcpy(pProject->startDate,pToken);

	pToken = strtok(NULL,"|");
	if(pToken == NULL)
		return 0;
	strcpy(pProject->endDate,pToken);

	pToken = strtok(NULL,"\0");
	if(pToken == NULL)
		return 0;
	sscanf(pToken,"%d",&pProject->exist);

	clearGraph();
	return status;
	}

void commandProcess(char *command,char *data,int showLog)
	{
	char *pToken;
	VERTEX_T addVertex;
	VERTEX_T *pTask=NULL;
	char requireTask[256];
	char taskName[256];
	int status = 0;

	memset(&addVertex,0,sizeof(VERTEX_T));

    if (strcasecmp(command,"VERTEX") == 0)
      	{
		pToken = strtok(data,"|");
		strcpy(addVertex.key,pToken);

		pToken = strtok(NULL,"|");
		strcpy(addVertex.task,pToken);

		pToken = strtok(NULL,"|");
		strcpy(addVertex.information,pToken);

		pToken = strtok(NULL,"|");
		sscanf(pToken,"%d",&addVertex.duration);

		pToken = strtok(NULL,"|");
		strcpy(addVertex.owner,pToken);

		pToken = strtok(NULL,"\0");
		sscanf(pToken,"%d",&addVertex.bDone);

		pTask = (VERTEX_T*) calloc(1,sizeof(VERTEX_T));
		memcpy(pTask,&addVertex,sizeof(VERTEX_T));

	    addTaskToList(pTask);	
	    if(showLog)
	    	{
	    	printf("ADDTASK |%s| DONE\n",addVertex.key);
	    	}
    	}

    else if (strcasecmp(command,"EDGE") == 0)
       	{
	   	pToken = strtok(data,"|");
		strcpy(requireTask,pToken);

		pToken = strtok(NULL,"|");
		strcpy(taskName,pToken);

        status=addEdge(requireTask,taskName);
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
       	fprintf(stderr,"Incorrect command found in file - ignoring\n");
       	fprintf(stderr,"  |%s|\n",command);
       	}
	}
	
/* 
==========================================================================================================

											Public function 
											
==========================================================================================================
*/

/* writeData
 * write all of project's data to the file.
 * Returns : 1 for success, 0 for failure.
 */
void writeData(char *outputFile,PROJECT_T *pProject)
	{
	VERTEX_T *vertextHead = (VERTEX_T *) getVListHead();
	int status = 1;
	char lineBuffer[64];
	FILE* pOut = NULL;
	pOut = fopen(outputFile,"w");
	VERTEX_T *pTask = NULL;
	VERTEX_T *pTmpTask = NULL;
	ADJACENT_T *pEdge = NULL;
	if (pOut == NULL)
		{
        fprintf(stderr,">>> Can't Opened file %s\n", outputFile);	
		}
	else
		{
		fprintf(pOut,"PROJECT|%s|%s|%s|%d\n",pProject->projectName,pProject->startDate,pProject->endDate,pProject->exist);
		pTask = vertextHead;
		while(pTask!=NULL)
			{
			fprintf(pOut,"VERTEX|%s|%s|%s|%d|%s|%d\n"
				,pTask->key,pTask->task,pTask->information,pTask->duration,pTask->owner,pTask->bDone);
			pTask = pTask->next;
			}
		pTask = vertextHead;
		while(pTask!=NULL)
			{
			pEdge = (ADJACENT_T *) pTask->adjacentHead;
			while(pEdge!=NULL)
				{
				pTmpTask = (VERTEX_T*) pEdge->pVertex;
				fprintf(pOut,"EDGE|%s|%s|%d",pTask->key,pTmpTask->key,pEdge->weight);
				pEdge = pEdge->next;
				}

			pTask = pTask->next;
			}
		}
	fclose(pOut);
	printf(">>> Created Backup file %s done\n",outputFile);
	}

/* readData
 * read the data from files to store in the programe
 * Returns : 1 for success, 0 for failure.
 */
int readData(char *inputFile,PROJECT_T *pProject,int showLog)
	{
	int status = 1;

	char lineBuffer[256];
	char *command=NULL;
	char *data=NULL;
	char *pToken=NULL;
	FILE *pIn = NULL;

	pIn = fopen(inputFile,"r");

	memset(lineBuffer,0,sizeof(lineBuffer));

	if (pIn == NULL)
		{
		status = 0;
        fprintf(stderr,">>> Can't Opened file %s\n", inputFile);
		}
	else
		{
		if (showLog)
			{
           	fprintf(stderr,">>> Opened file %s complete\n", inputFile);
			}

       	if (fgets(lineBuffer,sizeof(lineBuffer),pIn) != NULL)
        	{
        	if(lineBuffer[strlen(lineBuffer)-1]=='\n')
        		lineBuffer[strlen(lineBuffer)-1]='\0';
    		pToken = strtok(lineBuffer,"|");
    		command = pToken;
           	if (command != NULL && strcasecmp("PROJECT",command) == 0)
               	{
    			pToken = strtok(NULL,"\0");
    			data = pToken;
               	if(addProject(data,pProject)==0)
               		{
               		status = 0;
           			fprintf(stderr,">>> Error can't created project\n");
               		}
               	else
               		{
			       	while(fgets(lineBuffer,sizeof(lineBuffer),pIn)!=NULL)
			       		{
	    				pToken = strtok(lineBuffer,"|");
    					command = pToken;
			       		pToken = strtok(NULL,"\0");
    					data = pToken;
			       		commandProcess(lineBuffer,data,showLog);
			       		}
               		}
               	}
            else
            	{
            	status = 0;
           		fprintf(stderr,">>> Error can't created project\n");
            	}
            }
       	else
           	{
           	status = 0;
           	fprintf(stderr,">>> Error can't created project\n");
           	}  
       	}
	fclose(pIn);
	return status;
	}