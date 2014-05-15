/*
 *  linkedListNetwork.c
 *
 *  Implements an abstractNetwork using adjacency lists (linked lists).
 *  This is a structure with two levels of list. There is a master
 *  list of vertices, linked in series. Each vertex points to a subsidiary
 *  linked list with references to all the other vertices to which it
 *  is connected.
 *
 *  Each vertex has an integer weight and a pointer to a parent vertex 
 *  which can be used for route finding and spanning tree algorithms
 *
 *  Key values are strings and are copied when vertices are inserted into
 *  the graph. Every vertex has a void* pointer to ancillary data which
 *  is simply stored. 
 *
 *  Created by Sally Goldin, 1 February 2012 for CPE 113
 *  Modified 18 March 2013 to improve naming.

 *  Modified by Zodha 3/04/57
 *    -Remove Undirected graph
 *    -Function initGraph don,t need argument
 *    -Modified Naming of some function 
 *    -Move colorAll function from private function to public function
 *    -Remove printShortest 
 *    -Add public fucntion 
 *        + validateTaskName
 *        + addTaskToList
 *        + findTaskSubmit
 *        + findTaskToDisplay
 *    -Add private function
 *        + compareTask
 *        + tranverseTask  
 *    -Modified printVertexInfo function not to show pVertex->data
 *    -Delete any pVertex->data variable
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"
#include "taskManager.h"
#include "abstractQueue.h"
#include "minPriorityQueue.h"
#include "structure.h"

#define WHITE 0
#define GRAY  1
#define BLACK 2

#define COMPLETE 2
#define IN_PROGRESS 1
#define INCOMPLETE 0

VERTEX_T *vListHead = NULL;
VERTEX_T *vListTail = NULL;

char* colorName[] = {"WHITE", "GRAY", "BLACK"};

/* 
==========================================================================================================

                                                Private function 
                      
==========================================================================================================
*/
/* Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 * Arguments
 *       key    -  Key we are looking for
 *       pPred  -  used to return the predecessor if any
 * Returns pointer to the vertex structure if one is found       
 */
VERTEX_T * findVertexByKey(char* key, VERTEX_T** pPred) 
    {
    VERTEX_T * pFoundVtx = NULL;
    VERTEX_T * pCurVertex = vListHead;
    *pPred = NULL;
    /* while there are vertices left and we haven't found
     * the one we want.
     */
    while ((pCurVertex != NULL) && (pFoundVtx == NULL))
        {
        if (strcmp(pCurVertex->key,key) == 0)
            {
            pFoundVtx = pCurVertex;
            }
        else
            {
            *pPred = pCurVertex;
            pCurVertex = pCurVertex->next;
            }
        }
    return pFoundVtx;
    }

/* Free the adjacencyList for a vertex 
 * Argument
 *   pVertex    - vertex whose edges we want to delete 
 */
void freeAdjacencyList(VERTEX_T *pVertex)
    {
    ADJACENT_T * pCurRef = pVertex->adjacentHead;
    while (pCurRef != NULL)
        {
        ADJACENT_T * pDelRef = pCurRef;
        pCurRef = pCurRef->next;
        free(pDelRef);
        }
    pVertex->adjacentHead = NULL;
    pVertex->adjacentTail = NULL;
    }

/* Check if there is already an edge between
 * two vertices. We do not want to add a duplicate.
 * Arguments
 *   pFrom        -  Start point of edge
 *   pTo          -  End point of edge
 * Return 1 if an edge already exists, 0 if it does
 * not.
 */
int edgeExists(VERTEX_T* pFrom, VERTEX_T* pTo)
    {
    int bEdgeExists = 0;
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
        {
        if (pCurRef->pVertex == pTo)
            {
            bEdgeExists = 1;  /* the 'To' vertex is already in the
                             * 'From' vertex's adjacency list */ 
            }
        else
            {
            pCurRef = pCurRef->next;
            }
        } 
    return bEdgeExists;
    }

/* Component of removeVertex. Removes all references
 * to this vertex as the end point of edges in other
 * vertices' adjacency lists.
 */
void removeReferences(VERTEX_T * pTarget)
    {
    VERTEX_T * pCurrentVtx = vListHead;
    while (pCurrentVtx != NULL)
        {
        if (pCurrentVtx != pTarget)
            {
            /* skip the target vertex */
            ADJACENT_T* pAdjacent = pCurrentVtx->adjacentHead;
            ADJACENT_T* pPrevAdjacent = NULL;
            while (pAdjacent != NULL)
                {
                if (pAdjacent->pVertex == pTarget)  /* if this edge involves the target*/
                    {
                    if (pPrevAdjacent != NULL)
                        {
                        pPrevAdjacent->next = pAdjacent->next;
                        }
                    else
                        {
                        pCurrentVtx->adjacentHead = pAdjacent->next;   
                        }
                    if (pAdjacent == pCurrentVtx->adjacentTail)
                        {
                        pCurrentVtx->adjacentTail = NULL;
                        }
                    free(pAdjacent);
                    pAdjacent = NULL;
                    break;    /* can only show up once in the adjacency list*/
                    }
                else
                    {
                    pPrevAdjacent = pAdjacent;
                    pAdjacent = pAdjacent->next;
                    }  
                }
            }
        pCurrentVtx = pCurrentVtx->next;      
        } 
    }

/* Count adjacent vertices to a vertex.
 * Argument
 *    pVertex   -   Vertex whose adjacent nodes we want to count
 * Returns integer value for count (could be zero)
 */
int countAdjacent(VERTEX_T * pVertex)
    {
    int count = 0;
    ADJACENT_T * pAdjacent = pVertex->adjacentHead;
    while (pAdjacent != NULL)
        {
        count += 1;
        pAdjacent = pAdjacent->next;
        }
    return count;
    }

/* Initialize the dValue and parent for all
 * vertices. dValue should be very big, parent
 * will be set to NULL. Also add to the minPriority queue.
 */
void initAll()
    {
    VERTEX_T* pVertex = vListHead;
    while (pVertex != NULL)
        {
        pVertex->dValue = 999999999;
        pVertex->parent = NULL;
        enqueueMin(pVertex);
        pVertex = pVertex->next;
        }
    }


/* Execute a breadth first search from a vertex,
 * calling the function (*vFunction) on each vertex
 * as we visit it and color it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 */
void traverseBreadthFirst(VERTEX_T* pVertex, void (*vFunction)(VERTEX_T*))
    {
    VERTEX_T * pCurrentVertex = NULL;
    VERTEX_T * pAdjVertex = NULL;    
    queueClear();
    colorAll(WHITE);
    pVertex->color = GRAY;
    enqueue(pVertex);
    while (queueSize() > 0)
        {
        pCurrentVertex = (VERTEX_T*) dequeue();
        if (pCurrentVertex->color != BLACK)
            {
            (*vFunction)(pCurrentVertex);
            pCurrentVertex->color = BLACK;
            ADJACENT_T* pAdjacent = pCurrentVertex->adjacentHead;
            while (pAdjacent != NULL)
                {
                pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
                if (pAdjVertex ->color != BLACK)
                    {
                    pAdjVertex->color = GRAY;
                    enqueue(pAdjVertex);
                    }
                pAdjacent = pAdjacent->next;
                }
            }
        } /* end while queue has data */
    }


/* Execute a breadth first search from a single vertex,
 * calling the function (*vFunction) on the lowest level
 * vertex we visit, and coloring it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 */
void traverseDepthFirst(VERTEX_T* pVertex, void (*vFunction)(VERTEX_T*))
    {
    VERTEX_T * pAdjVertex = NULL;    
    ADJACENT_T* pAdjacent = pVertex->adjacentHead;
    while (pAdjacent != NULL)
        {
        pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
        if (pAdjVertex->color == WHITE)
            {
            pAdjVertex->color = GRAY;
            traverseDepthFirst(pAdjVertex,vFunction);
            }
        pAdjacent = pAdjacent->next;  
        } /* end while queue has data */
    /* when we return from the bottom, call the 
     * function and color this node black.
     */
    (*vFunction)(pVertex);
    pVertex->color = BLACK;
    }


/* Function to print the information about a vertex
 * Argument  
 *   pVertex   -   vertex we want to print
 */
void printVertexInfo(VERTEX_T* pVertex)
    {
    printf("== Vertex key |%s| \n",pVertex->key);
    }

/* compareTask
 * This function will compare the task name.
 * Arguments: pVertex - pointer to the vertex of task
 *            taskName - task name from user
 *            pResult - keep result, 1 for the same
 */
void compareTask(void* pVertex,char *taskName,int* result)
    {
    if (strcasecmp(taskName,((VERTEX_T*)pVertex)->task) == 0)
        *result = 1;
    }

/* Execute a depth first search from a single vertex
 * on the lowest level
 * vertex we visit, and coloring it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 *    taskName  -  task name from user
 *    result    - keep result from comparing function
 *
 * Modified by Phasathorn Suwansri (Lukkid) ID: 3424
 *    - add comparison function
 *    - add more arguments
 */
void traverseTask(void* pVertex,char* taskName,int* result)
    {
    VERTEX_T * pAdjacent = NULL;    
    ADJACENT_T* pRef = ((VERTEX_T*)pVertex)->adjacentHead;
    while (pRef != NULL)
        {
        pAdjacent = (VERTEX_T*) pRef->pVertex;
        if (pAdjacent->color == WHITE)
            {
            pAdjacent->color = GRAY;
            traverseTask(pVertex,taskName,result);
            }
        pRef = pRef->next;  
        }
    compareTask(pVertex,taskName,result);
    ((VERTEX_T*)pVertex)->color = BLACK;
    }


void emptyFunction(VERTEX_T* pVertex)
{
}
/* Return information as to whether two vertices are
 * connected by a path.
 * Arguments
 *    
 * Returns 1 if the two vertices are connected, 0 if they
 * are not. Returns -1 if either vertex does not exist.
 */
int isReachable(VERTEX_T* startTask, VERTEX_T* endTask)
{
   int retval = 1;
   VERTEX_T * pDummy = NULL;
   if ((startTask == NULL) || (endTask == NULL))
      {
      retval = -1;
      }
   else
      {
      traverseBreadthFirst(startTask,&emptyFunction);
      /* if after a breadth first traversal, we didn't reach the 
       * the end vertex, it is not reachable.
       */
      if (endTask->color != BLACK)
          retval = 0;
      }
   return retval;
}

int calculateDone()
    {
    int done = 0;
    int all = 0;
    VERTEX_T *pTmp = NULL;
    pTmp = vListHead;
    while (pTmp != NULL)
        {
        if(pTmp->bDone == COMPLETE)
            done++;

        all++;
        pTmp = pTmp->next;
        }
    return done*(100/all);
    }
    
/* 
==========================================================================================================

                                          Public function 
                      
==========================================================================================================
*/

int checkNetworkConnect(char *task)
{
    VERTEX_T *pCurrent = vListHead;
    VERTEX_T *pTail = vListTail;
    ADJACENT_T *pRef = NULL;
    char input[16];
    int isReach = 0;
    int option =0;
    int status =0;
    if(pCurrent == NULL)
        {
        printf("Network is empty!\n");
        return 2;
        }
    while(pCurrent != NULL)
        { 
        if(pCurrent != pTail)
            {
            isReach=isReachable(pCurrent,pTail);
            if(isReach == 0)
                {
                printf("task: %s is not connect to last task\n",pCurrent->task);
                if(pCurrent->adjacentHead==NULL)
                    {
                    printf("task: %s don't connection to others\n",pCurrent->task);
                    printf("Do you want to add reqire task by yourself or automatically add\n");
                    memset(input,0,sizeof(input));
                    while(option < 1 || option > 2)
                        {
                        printf("\n-- 1 -- Add by yourself\n");
                        printf("-- 2 -- Add automatically\n");
                        fgets(input,sizeof(input),stdin);
                        sscanf(input,"%d",&option);
                        }
                    if(option==2)
                        {
                        status=addEdge(pCurrent->task,pTail->task);
                        if(status == 1)
                            {
                            printf("add require successful\n");
                            pTail->bDone = INCOMPLETE;
                            }
                        else if(status == 0)
                            {
                            printf("Error allocation\n");
                            return status;
                            }
                        else
                            {
                            printf("Unsuccess find task\n");
                            return status;
                            }
                        }
                    else
                        {
                        task=(char*)pCurrent->task;
                        return option;
                        }
                    }
                }
            pCurrent = pCurrent->next;
            }
        else
            {
            printf("Check to last task\n");
            pCurrent = pCurrent->next;
            status = 2;
            }
        }
    return status;    
    }

void setStatus(int countRequire,char* taskName)
{
    VERTEX_T *pTask = NULL;
    VERTEX_T *pPred = NULL;
    pTask=findVertexByKey(taskName,&pPred);
    if(pTask != NULL)
        {
        if(countRequire == 0)
            {
            printf("%s is in progress\n",pTask->task );
            pTask->bDone = IN_PROGRESS;
            }
        else
            {
            printf("%s is in complete\n",pTask->task );
            pTask->bDone = INCOMPLETE;
            }
        }
    else
        {
        printf("Task doesn't set\n");
        }
}

/* validateTaskName
 * This function will validate the task's name by
 * check the same name.
 * Argument : input - string for task's name
 * Returns : 1 for success, 0 for failure.
 */

int validateTaskName(char input[])
    {
    int bOk = 1;          /* keep status of this function */
    int result = 0;       /* keep result of same name */
    char taskName[64];    /* keep task name */
    VERTEX_T *pTask = vListHead;

    if(pTask != NULL)
        {
        input[strlen(input)-1] = '\0';
        sscanf(input,"%s",taskName);

        colorAll(WHITE);
        traverseTask(pTask,taskName,&result);
    if (result == 1)
        bOk = 0;
        }
    return bOk;
    }

/*
-------------------------------------------------------------
ARGUMENT:
RETURN:
-------------------------------------------------------------
*/

int findRequireTask(char input[])
    {
    int bOk = 1;          /* keep status of this function */
    VERTEX_T *pRequire = NULL;
    VERTEX_T *pPred = NULL;
    pRequire=findVertexByKey(input,&pPred);
    if(pRequire == NULL)
        {
        printf("Can not find this task, make sure you had create this task\n");
        bOk=0;
        }
    return bOk;
    }
/* Just empty function
 */
void notPrintVertexInfo(VERTEX_T* pVertex)
  {
  }
/*
-------------------------------------------------------------
ARGUMENT:
RETURN:
-------------------------------------------------------------
*/

void addTaskToList(void *pTask)
    {
    VERTEX_T *pTail=vListTail;
    if(vListHead == NULL)
        {
        vListHead = (VERTEX_T*)pTask;
        }
    else
        {
        pTail->next = (VERTEX_T*)pTask;
        }
    vListTail = (VERTEX_T*)pTask;
    }

/* findTaskSubmit
 * This function will validate and submit task which is
 * in progress status.
 * Argument : input - task that user want to submit.
 * Returns : 1 for success , 0 for failure,
 * and -1 for task does not exist.
 * Created by Chalermpon Thongmotai PAO
 * 
 */
int findTaskSubmit(char *input)
    {
    VERTEX_T *pTask = NULL;
    VERTEX_T *pPred = NULL;
    VERTEX_T *vTmp = NULL;
    VERTEX_T *pCheck = NULL;

    ADJACENT_T *tmpAdj = NULL;

    int status = 1;

    if(input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
    pTask=findVertexByKey(input,&pPred);
    if(pTask != NULL)
        {
        vTmp = vListHead;

        while(vTmp != NULL)
            {
            tmpAdj = vTmp->adjacentHead;
            while(tmpAdj != NULL)
                {
                pCheck = (VERTEX_T *) tmpAdj->pVertex;
                if(pCheck == pTask &&  pCheck->bDone == INCOMPLETE)
                    {
                    printf("Task |%s| have to be done before you can submit this task !\n",pCheck->key);
                    status = 0;
                    }
                tmpAdj = tmpAdj->next;
                }

            vTmp = vTmp->next;
            }
        }
    else
        status = -1;
        
    if(status == 1)
        {
        pTask->bDone = COMPLETE;
        }
    return status;
    }

/* displayTask
 * This function will display all the task which is
 * in progress status (current task)
 *
 * Created by Phasathorn Suwansri (Lukkid)
 *            ID : 56070503424
 */
void findTaskToDisplay(void* project)
    {
    PROJECT_T* pProject = (PROJECT_T*) project;
    VERTEX_T * pCurrentTask = vListHead;    /* pointer to the first task of the list */
    
    char status[3][64] = {"|incomplete|","in_progress","complete"};    

    printf("========================================\n");
    printf("== Start : %s\n",pProject->startDate);
    printf("== End   : %s\n",pProject->endDate);
    printf("== Now   : %d%% Completed..\n",calculateDone());
    printf("========================================\n");
    printf("============== Task Status =============\n");

    pCurrentTask = vListHead;
    while (pCurrentTask!= NULL)
        {
        if (pCurrentTask->bDone == COMPLETE)
            {
            printf(" - %s [%s]\t\t %s\n",pCurrentTask->task,pCurrentTask->owner,status[pCurrentTask->bDone]);
            printf("   [information] : %s\n",pCurrentTask->information);
            }
        pCurrentTask= pCurrentTask->next;
        }

    pCurrentTask = vListHead;
    while (pCurrentTask!= NULL)
        {
        if (pCurrentTask->bDone == IN_PROGRESS)
            {
            printf(" - %s [%s]\t\t %s\n",pCurrentTask->task,pCurrentTask->owner,status[pCurrentTask->bDone]);
            printf("   [information] : %s\n",pCurrentTask->information);            } 
          pCurrentTask= pCurrentTask->next;
        }

    pCurrentTask = vListHead;
    while(pCurrentTask!=NULL)
        {
        if(pCurrentTask->bDone == INCOMPLETE)
            {
            printf(" - %s [%s]\t\t %s\n",pCurrentTask->task,pCurrentTask->owner,status[pCurrentTask->bDone]);
            printf("   [information] : %s\n",pCurrentTask->information);
            }
        pCurrentTask = pCurrentTask->next;
        }
    }


/* Color all vertices to the passed color.
 * Argument
 *    A color constant
 */
void colorAll(int color)
    {
    VERTEX_T* pVertex = vListHead;
    while (pVertex != NULL)
        {
        pVertex->color = color;
        pVertex = pVertex->next;
        }
    }


/* Initialize or reintialize the graph.
 * Argument 
  * Returns 1 unless there is a memory allocation error,
 * in which case it returns zero.
 */
int initGraph()
    { 
    /* for a linked list graph, we call
     * clearGraph and then initialize bGraphDirected
     */
    clearGraph();
    return 1;  /* this implementation of initGraph can never fail */ 
    }


void clearGraph()
    {
    VERTEX_T * pCurVertex = vListHead;

    while (pCurVertex != NULL)
        {
        freeAdjacencyList(pCurVertex);
        VERTEX_T * pDelVtx = pCurVertex;
        pCurVertex = pCurVertex->next;
        free(pDelVtx);
        }
    vListHead = NULL;  
    vListTail = NULL;
    }

/* Add a vertex into the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex
 *     pData -   Additional information that can
 *               be associated with the vertex.
 * Returns 1 unless there is an error, in which case
 * it returns a 0. An error could mean a memory allocation 
 * error or running out of space, depending on how the 
 * graph is implemented. Returns -1 if the caller tries
 * to add a vertex with a key that matches a vertex
 * already in the graph.
 */
int addVertex(char* key, void* pData)
    {
    int bOk = 1;
    VERTEX_T * pPred;
    VERTEX_T * pFound = findVertexByKey(key, &pPred);
    if (pFound != NULL)  /* key is already in the graph */
        {
        bOk = -1;
        }
    else
        {
        VERTEX_T * pNewVtx = (VERTEX_T *) calloc(1,sizeof(VERTEX_T));
        char * pKeyval = strdup(key);
        if ((pNewVtx == NULL) || (pKeyval == NULL))
            {
            bOk = 0;  /* allocation error */
            }
        else
            {
            if (vListHead == NULL)  /* first vertex */
                {
                vListHead = pNewVtx;
                }
            else
                {
                vListTail->next = pNewVtx; 
                }
            vListTail = pNewVtx;
            }
        }
    return bOk;
    }

/* Remove a vertex from the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex to remove
 * Returns a pointer to the data stored at that
 * vertex, or NULL if the vertex could not be 
 * found.
 */
void* removeTask(char* key)
    {
    void * pData = NULL; /* data to return */
    VERTEX_T * pPredVtx = NULL;
    VERTEX_T * pRemoveVtx = findVertexByKey(key,&pPredVtx);
    if (pRemoveVtx != NULL)
        {
        removeReferences(pRemoveVtx);
        freeAdjacencyList(pRemoveVtx);
        if (pPredVtx != NULL)
            {
            pPredVtx->next = pRemoveVtx->next;
            }
        else /* if there is no predecessor that means this was the head */
            {
            vListHead = pRemoveVtx->next;
            }   
        if (pRemoveVtx == vListTail)
            vListTail = pPredVtx;
        free(pRemoveVtx->key);
        pData = pRemoveVtx->task;
        free(pRemoveVtx);
        } 
    return pData;
    }


/* Add an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 *    weight - weight for this edge
 * Returns 1 if successful, 0 if failed due to
 * memory allocation error, or if either vertex
 * is not found. Returns -1 if an edge already
 * exists in this direction.
 */
int addEdge(char* key1, char* key2)
    {
    int bOk = 1;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFromVtx = findVertexByKey(key1,&pDummy);
    VERTEX_T * pToVtx = findVertexByKey(key2,&pDummy);
    if ((pFromVtx == NULL) || (pToVtx == NULL))
        {
        printf("Can't find require\n");
        bOk = 0;
        }
    else if (edgeExists(pFromVtx,pToVtx))
        {
        bOk = -1;       
        }
    else
        {
        ADJACENT_T * pNewRef = (ADJACENT_T*) calloc(1,sizeof(ADJACENT_T));
        if (pNewRef == NULL)
            {
            printf("allocation fail\n");
            bOk = 0;
            }
        else
            {
            pNewRef->pVertex = pToVtx;
            pNewRef->weight = pFromVtx->duration; 
            if (pFromVtx->adjacentTail != NULL)
                {
                pFromVtx->adjacentTail->next = pNewRef;
                }
            else
                {
                pFromVtx->adjacentHead = pNewRef;
                }
            pFromVtx->adjacentTail = pNewRef;
            } 
        } 
    return bOk;
    }


/* Remove an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns 1 if successful, 0 if failed 
 * because either vertex is not found or there
 * is no edge between these items.
 */
int removeEdge(char* key1, char* key2)
    {
    int bOk = 1;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFromVtx = findVertexByKey(key1,&pDummy);
    VERTEX_T * pToVtx = findVertexByKey(key2,&pDummy);
    if ((pFromVtx == NULL) || (pToVtx == NULL))
        {
        bOk = 0;
        }
    else if (!edgeExists(pFromVtx,pToVtx))
        {
        bOk = 0;
        }
    else
        {
        ADJACENT_T* pAdjacent = pFromVtx->adjacentHead;
        ADJACENT_T* pPrevAdjacent = NULL;
        while (pAdjacent != NULL)
            {  
            if (pAdjacent->pVertex == pToVtx)  /* if this edge involves the target*/
                {
                if (pPrevAdjacent != NULL)
                    {
                    pPrevAdjacent->next = pAdjacent->next;
                    }
                else
                    {
                    pFromVtx->adjacentHead = pAdjacent->next;   
                    }
                if (pAdjacent == pFromVtx->adjacentTail)
                    {
                    pFromVtx->adjacentTail = NULL;
                    }
                free(pAdjacent);
                break;    /* can only show up once in the adjacency list*/
                }
            else
                {
                pPrevAdjacent = pAdjacent;
                pAdjacent = pAdjacent->next;
                }  
            }
        }
    return bOk;
    }

/* Find a Task and return its data
 * Arguments
 *    key  -  Key for the vertex to find
 * Returns the data for the vertex or NULL
 * if not found.
 */
void* findTask(char* key)
    {
    void* pData = NULL;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFoundVtx = findVertexByKey(key,&pDummy);
    if (pFoundVtx == NULL)
        {
        return NULL;
        }
    return pFoundVtx; 
    }

/* Find the edge between two vertices (if any) and return
 * its weight
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns weight if successful and edge exists.
 * Returns -1 if an edge is not found
 */
int findEdge(char* key1, char* key2)
    {
    int weight = -1;
    int bEdgeExists = 0;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFrom = findVertexByKey(key1,&pDummy);   
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
        {
        VERTEX_T * pFrom = (VERTEX_T*) pCurRef->pVertex;
        if (strcmp(pFrom->key,key2) == 0)
            {
            weight = pCurRef->weight;  
            bEdgeExists = 1;
            }
        else
            {
            pCurRef = pCurRef->next;
            }
        } 
    return weight;
    }



/* Return an array of copies of the keys for all nodes
 * adjacent to a node. The array and its
 * contents should be freed by the caller when it 
 * is no longer needed.
 * Arguments
 *    key   -  Key for the node whose adjacents we want
 *    pCount - Return number of elements in the array
 * Returns array of char* which are the keys of adjacent
 * nodes. Returns number of adjacent vertices in pCount.
 * If pCount holds -1, the vertex does not exist.
 */
char** getAdjacentVertices(char* key, int* pCount)
    {
    char** keyArray = NULL;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFoundVtx = findVertexByKey(key,&pDummy);
    if (pFoundVtx != NULL)
        {
        *pCount = countAdjacent(pFoundVtx);
        if (*pCount > 0)
            {
            int i = 0;
            keyArray = (char**) calloc(*pCount, sizeof(char*));
            if (keyArray != NULL)
                {
                ADJACENT_T * pAdjacent = pFoundVtx->adjacentHead;
                while (pAdjacent != NULL)
                    {
                    VERTEX_T* pVertex = (VERTEX_T*) pAdjacent->pVertex;
                    keyArray[i] = strdup(pVertex->key);
                    pAdjacent = pAdjacent->next;
                    i += 1;
                    }
                }
            }  
       } 
    else
        {
        *pCount = -1;
        }
    return keyArray;
    }


/* Comparison function to send to the minPriorityQueue
 * Arguments
 *   pV1     First vertex (will be cast to VERTEX_T *)
 *   pV2     Second vertex (will be cast to VERTEX_T *)
 * Compares dValues. Returns -1 if V1 < V2, 0 if dValues are
 * the same, 1 if V1 > V2.
 */
int compareVertices(void * pV1, void * pV2)
    {
    VERTEX_T * pVertex1 = (VERTEX_T*) pV1;
    VERTEX_T * pVertex2 = (VERTEX_T*) pV2;
    if (pVertex1->dValue < pVertex2->dValue)
        return -1;
    else if (pVertex1->dValue > pVertex2->dValue)
        return 1;
    else 
        return 0;
    }
/* Print out the longest weight path from one vertex to 
 * another through the network using Dijkstra's
 * algorithm. 
 * Arguments
 *    startKey    -  Key of start vertex
 *    endKey      -  Key of ending vertex
 * Returns the sum of the weights along the path.
 * Returns -1 if either key is invalid
 * Returns -2 if there is not reachable
 */
int printLongestPath(char* startKey, char* endKey)
    {
    int retval = 0; /* return value */

    VERTEX_T *pDummy1 = NULL;
    VERTEX_T *pDummy2 = NULL;

    char *pathKeies[256];
    int pathCount = 0;
    int i=0;
    memset(pathKeies,0,sizeof(pathKeies));

    /* find each vertex from key */
    VERTEX_T *pVertexStart = (VERTEX_T *) findVertexByKey(startKey,&pDummy1);
    VERTEX_T *pVertexStop = (VERTEX_T *) findVertexByKey(endKey,&pDummy2);
    
    VERTEX_T *pCurrent = NULL; /* current vertex to process */
    VERTEX_T *pVertexAdj = NULL; /* vertex from adjacent */
    ADJACENT_T *pAdjacent = NULL; /* adjacent to process */

    /* check for each key is exist */
    if (pVertexStart == NULL || pVertexStop == NULL)
        {
        retval = -1;
        }
    /* check for the two of vertex are reachable */
    if(retval == 0)
        {
        traverseBreadthFirst(pVertexStart,&notPrintVertexInfo);
        if (pVertexStop->color != BLACK)
            {
            retval = -2;
            }
        }

    /* Dijkstra's algorithm */
    if(retval == 0)
        {
        queueMinInit(&compareVertices); /* Put all vertices into the min priority queue */
        colorAll(WHITE); /* set all vertices color to white */
        initAll(); /* set initial value to all vertices */

        pVertexStart->dValue = 0;

        /* do until queue is empty */
        while (queueMinSize() > 0)
            {
            /* dequeue and set to current */
            pCurrent = (VERTEX_T *) dequeueMin();
            /*set current vertex to black */
            pCurrent->color = BLACK;
            
            pAdjacent = pCurrent->adjacentHead;

            /* loop every adjacent of current vertex */
            while (pAdjacent != NULL)
                {
                pVertexAdj = (VERTEX_T *) pAdjacent->pVertex;
                /* set new value if new weight is more than the old one */
                if ((pVertexAdj->color == WHITE) && ((pCurrent->dValue+pAdjacent->weight) > pVertexAdj->dValue))
                    {
                    pVertexAdj->dValue = pCurrent->dValue + pAdjacent->weight;
                    pVertexAdj->parent = pCurrent;
                    }
                pAdjacent = pAdjacent->next; 
                }
            }

        /* get longest weight path */
        retval = pVertexStop->dValue;
        pCurrent = pVertexStop;

        while (pCurrent!=pVertexStart)
            {
            pathKeies[pathCount] = pCurrent->key;
            pCurrent = pCurrent->parent;
            pathCount++;
            }
        pathKeies[pathCount] = pCurrent->key;

        printf("\nThe longest path is\n");
        for(i=pathCount; i>=0 ; i--)
            {
            printf("%s->",pathKeies[i]);
            }
        printf("END\n");
        }
    return retval;
    }

void *getVListHead()
    {
    return vListHead;
    }
