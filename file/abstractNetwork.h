/*  
 *  abstractNetwork.h
 *
 *  Defines the necessary functions for a network, that is a 
 *  graph with weights on each edge
 *
 *  Created by Sally Goldin on 1 Feb 2012 for CPE 113
 * 		Modified by Zodha
 *			3/04/14
 *			DEBUG function that we need to be fixed at the end
 *					of the file
 *
 */
#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H


/*
-------------------------------------------------------------
this function will check the network if all the task connect
	to last task or not

ARGUMENT: task name for return the name of the task that don't
			have connection to last task
RETURN: status that will tell us if the graph is all connect or not
		return 2 if all connect
		return other if graph have problem or not connect
-------------------------------------------------------------
*/
 int checkNetworkConnect(char *task);
/*
-------------------------------------------------------------
this function will set the graph to in progress or incomplete
ARGUMENT: name of the task 
			countRequire - check if this task have require or not
RETURN: void
-------------------------------------------------------------
*/

void setStatus(int countRequire,char* taskName);

/*
-------------------------------------------------------------
this function will add task to the linklist structure
ARGUMENT: struct pTask - struct that store the task information
RETURN: void
-------------------------------------------------------------
*/

 void addTaskToList(void *pTask);
 /*
-------------------------------------------------------------
this function will validate name of the task if the task name is
	duplicate or not
ARGUMENT:input - the task name
RETURN: status- tell that 1 -- success 0-- fail
-------------------------------------------------------------
*/
 
int validateTaskName(char input[]);



 /*
-------------------------------------------------------------
find the require task struct
ARGUMENT: input - name of the require task
RETURN: status 1--success 0--fail
-------------------------------------------------------------
*/
 int findRequireTask(char input[]);

 

 /* findTaskSubmit
 * This function will validate and submit task which is
 * in progress status.
 * Argument : input - task that user want to submit.
 * Returns : 1 for success , 0 for failure,
 * and -1 for task does not exist.
 * Created by Phasathorn Suwansri (Lukkid)
 *            ID : 56070503424
 * 
 */


 int findTaskSubmit(char *input);

 /* displayTask
 * This function will display all the task which is
 * in progress status (current task)
 *
 * Created by Phasathorn Suwansri (Lukkid)
 *            ID : 56070503424
 */
void findTaskToDisplay(void* project);

/* Color all vertices to the passed color.
 * Argument
 *    A color constant
 */
void colorAll(int color);

/* Initialize the graph.
 * Argument 
 *    maxVertices  - how many vertices can this graph
 *                   handle.
 *    bDirected    - If true this is a directed graph.
 *                   Otherwise undirected.
 * Returns 1 unless there is a memory allocation error,
 * in which case it returns zero.
 */
int initGraph();


/* Free all memory associated with the graph and
 * reset all parameters.
 */
void clearGraph();


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
int addVertex(char* key, void* pData);


/* Remove a vertex from the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex to remove
 * Returns a pointer to the data stored at that
 * vertex, or NULL if the vertex could not be 
 * found.
 */
void* removeTask(char* key);


/* Add an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 *    weight - Weight for this edge. Constrained to be positive
 * Returns 1 if successful, 0 if failed due to
 * memory allocation error, or if either vertex
 * is not found. Returns -1 if an edge already
 * exists in this direction.
 */
int addEdge(char* key1, char* key2);


/* Remove an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns 1 if successful, 0 if failed 
 * because either vertex is not found or there
 * is no edge between these items.
 */
int removeEdge(char* key1, char* key2);


/* Find a Task and return its data
 * Arguments
 *    key  -  Key for the vertex to find
 * Returns the data for the vertex or NULL
 * if not found.
 */
void* findTask(char* key);

/* Find the edge between two vertices (if any) and return
 * its weight
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns weight if successful and edge exists.
 * Returns -1 if an edge is not found
 */
int findEdge(char* key1, char* key2);


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
char** getAdjacentVertices(char* key, int* pCount);



void *getVListHead();


#endif
