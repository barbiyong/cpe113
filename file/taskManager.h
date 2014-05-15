/* 
==============================================================
taskManager.h

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

Create by Thanat Lapthawan ID: 56070503413
					DATE: 3/04/2014 
==============================================================
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

int createProject(void *project);
	
/*
-------------------------------------------------------------
editOption function
	+This function will edit the task that user want

ARGUMENT: pProject - struct that store data of the project
RETURN: status- return 1 if success,0 if fail
-------------------------------------------------------------
*/

int editOption(void *project);
/*
-------------------------------------------------------------
taskSubmit function
	+This function will ask for task name that user want to 
			submit and send it to findTaskSubmit in linkedListNetwork file
			
ARGUMENT: NONE
RETURN:status - return 1 if success, 0 if fail
-------------------------------------------------------------
*/

int taskSubmit();

/*
-------------------------------------------------------------
displayTask function
	+This function will display the current task and the 
			other task that is uncomplete

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/void displayTask(void* project);

/*
-------------------------------------------------------------
programManual function
	+This function will display the manual of the program 
		from the manual text file

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/

void programManual();
/*
-------------------------------------------------------------
deleteProject function
    +This function will display the manual of the program 
        from the manual text file

ARGUMENT:NONE
RETURN:NONE
-------------------------------------------------------------
*/
void deleteProject(void *project);
