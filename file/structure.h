/* 
==============================================================
structure.h

  + ADJACENT_T
  + VERTEX_T
  + DEVELOPER_T
  + PROJECT_T


Create by Zodha 
          DATE: 3/04/2014 
==============================================================
*/


/* List items for the adjacency list.
 * Each one is a reference to an existing vertex
 */
typedef struct _adjacent
    {
    void *pVertex;           /* pointer to the VERTEX_T this 
                               * item refers to.
                               */
    unsigned int weight;      /* weight of this edge */
    struct _adjacent * next;  /* next item in the ajacency list */ 

    } ADJACENT_T;

typedef struct _vertex
    {
    char key[64];           /* key for this vertex */
    char task[64];
    char information[256];
    int duration; 
    char owner[128];
    int color;              /* used to mark nodes as visited */
    int dValue;             /* sum of weights for shortest path so far to this vertex */
    int bDone;        /*finish the task?*/
    
    struct _vertex * parent;  /* pointer to parent found in Dijkstra's algorithm */
    struct _vertex *next ;

    ADJACENT_T *adjacentHead; /* list of require task */
    ADJACENT_T *adjacentTail;

    } VERTEX_T;

typedef struct _developer
    {
    char name[32];
    struct _developer *pNext;

    } DEVELOPER_T;

typedef struct 
    {
    char projectName[64];
    char startDate[32]; 
    char endDate[32]; /* month:date*/
    int exist;/* check if program exist or not*/
    }PROJECT_T;

