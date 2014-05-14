/*******************************************************
 *
 * validateFunctions.c
 *  This c file is use for validate the data
 * from the main file. Is it incorrect format.
 *
 * Created by Phasathorn Suwansri (Lukkid)
 *                      ID : 56070503424
 * Date : 31 March 2014
 * Modified Date : 2 April 2014
 *
 * NOTE : Some private functions in this files
 *        is Authored by Aj.Sally
 *              - date structure
 *                  - isLeapYear
 *                  - dateToday
 *                  - dateCompare
 *                  - colorAll
 *                  - traverseDepthFirst (modified by Lukkid)
 *
 *******************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "abstractNetwork.h"
#include "structure.h"

/**************************************************/
/* Private Function */
/* dateFunctions
 * Author : Aj.Sally
 */

typedef struct
{
    int year;    /* C.E. year */
    int month;   /* month, from 1 to 12 */
    int day;     /* day, from 1 to a max of 31 */ 
} DATE_T;

int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
/* 
==========================================================================================================

                                          Private function 
                      
==========================================================================================================
*/

/* Local function. , not exposed in the header file.
 * Returns true (1) if 'year' is leap year, else false (0)
 */
int isLeapYear(int year)
    {
    if ((year % 4 == 0) && (year % 100 != 0)) 
        return 1;
    else
        return 0;  
    }

/* Sets the fields of 'pResult' to the correct values for today. */
void dateToday(DATE_T* pResult)
    {
    time_t seconds = 0;    
    struct tm * pTimeStruct;

    seconds = time(&seconds);
    pTimeStruct = localtime(&seconds);
    pResult->day = pTimeStruct->tm_mday;
    pResult->month = pTimeStruct->tm_mon + 1;
    pResult->year = pTimeStruct->tm_year + 1900;
    }


/* Compares two DATE_T structures. Returns 1 if the 
 * first date is later than the second, -1 if the 
 * first date is earlier than the second, and 0 if
 * they are the same.
 */
int dateCompare(DATE_T firstDate, DATE_T secondDate)
    {
    int compareValue = 0;
    if (firstDate.year > secondDate.year)
        {
        compareValue = 1;
        }
    else if (firstDate.year < secondDate.year)
        {
        compareValue = -1;
        } 
    else if (firstDate.month > secondDate.month)
        {
        compareValue = 1;
        }
    else if (firstDate.month < secondDate.month)
        {
        compareValue = -1;
        } 
    else if (firstDate.day > secondDate.day)
        {
        compareValue = 1;
        }
    else if (firstDate.day < secondDate.day)
        {
        compareValue = -1;
        } 
    /* otherwise they are the same */
    return compareValue;
    }


/* 
==========================================================================================================

                                          Public function 
                      
==========================================================================================================
*/

/* validateDuration
 * This function will validate the duration by
 * check the digits of them. After that save 
 * them to the task
 * Argument : input - string for duration
 * Returns : 1 for success, 0 for failure.
 */
int validateDuration(char input[])
    {
    int bOk = 1;                /* keep status of this function */
    int length = 0;         /* length of string */  
    int i = 0;                  /* index */

    input[strlen(input)-1] = '\0';
    length = strlen(input);
    if (length != 10)
        bOk = 0;

    for (i = 0;i < length;i++)
        {
        if (isdigit(input[i]))
            bOk = 1;
        else
            bOk = 0;
        }

    return bOk;
    }

/* validateStartProjectDate
 * This function will validate the start project date by
 * check with today date.
 * Argument : input - string for start project date
 *            project - pointer to project node.
 * Returns : 1 for success, 0 for failure.
 */
int validateStartProjectDate(char input[],void* pProject)
    {
    int bOk = 1;                        /* status of this function */
    int length = 0;                 /* length of string */
    int i = 0;                          /* index */
    int day = 0;                        /* day buffer */
    int month = 0;                  /* month buffer */
    int year = 0;                       /* year buffer */
    char * pToken = NULL;       /* token for split the string */
    char startDate[32];
    DATE_T todayDate;               /* today date structure */
    DATE_T dateBuffer;          /* store buffer for date */

    /* set "\n" to "\0" */
    input[strlen(input)-1] = '\0';
    length = strlen(input);
    if (length != 10)
        {
        printf("Please use date format dd-mm-yyyy\n");
        return 0;
        }
    
    for (i = 0;i < length;i++)
        {
        if (isalpha(input[i]))
            {
            printf("Date don't allow to use any alphabet\n");
            return 0;
            }
        }
    
    pToken = strtok(input,"-");
    sscanf(pToken,"%d",&day);
    pToken = strtok(NULL,"-");
    sscanf(pToken,"%d",&month);
    pToken = strtok(NULL,"-");
    sscanf(pToken,"%d",&year);

    if (year <= 0)
        {
        printf("6\n");
        bOk = 0;
        }  
    if ((month <= 0) || (month > 12) )
        {
        printf("7\n");
        bOk = 0;
        }    
    if ((day <= 0) || (day > monthDays[month-1]))
        {
        printf("4\n");
        bOk = 0;
        }

    dateBuffer.year = year;
    dateBuffer.month = month;
    dateBuffer.day = day;

    
    dateToday(&todayDate);
    if( dateCompare(todayDate,dateBuffer) == 1)
        {
        printf("8\n");
        return 0;
        }  
    else
        {
        sprintf(startDate,"%02d-%02d-%04d",dateBuffer.day,dateBuffer.month,dateBuffer.year);
        strcpy(((PROJECT_T*)pProject)->startDate,startDate);
        }

    return bOk;
    }

