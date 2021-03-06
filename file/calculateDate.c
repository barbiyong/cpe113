/*
 *  calculateDate.c
 *  use to calculate enddate from startDate and duration by avoide holiday and weekend
 *
 *  Created by Chalermpon Thongmotai Pao
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "abstractNetwork.h"
#include "structure.h"

/* use to hold all holiday */
char **holiday = NULL;
char **holidayDetail = NULL;
int count=0;

/* 
==========================================================================================================

                                            Private function 
                                            
==========================================================================================================
*/

/* isHoliday use to check does input day is holiday
 *  Arguments - struct tm *timeCheck is input day inform of tm struct
 *             showDetail 1 to show detail about holiday if it is 0 for not
 *  return 1 input is holiday 0 No
 */

int isHoliday(struct tm *timeCheck,int showDetail)
    {
    char date[64];
    int i=0;
    sprintf(date,"%02d-%02d-%4d",timeCheck->tm_mday,timeCheck->tm_mon+1,timeCheck->tm_year + 1900);
    /* loop check from our global store of holiday*/
    for(i=0;i<count;i++)
        {
        if(strcmp(date,holiday[i])==0)
            {
            if(showDetail)
                printf("%s\n",holidayDetail[i]);

            return 1;
            }
        }
    return 0;
    }

/* isWeekend use to check does input day is weekend
   Arguments - struct tm *timeCheck is input day inform of tm struct
               showDetail 1 to show detail about weekend if it is 0 for not
   return 1 input is weekend 0 No
 */
int isWeekend(struct tm *timeCheck,int showDetail)
    {
    /* check for weekend */
    if(timeCheck->tm_wday==0 || timeCheck->tm_wday==6)
        {
        if(showDetail)
            printf(" #%02d-%02d-%04d is Weekend\n\n",timeCheck->tm_mday,timeCheck->tm_mon+1,timeCheck->tm_year+1900);
        return 1;
        }
    else
        return 0;
    }

/* readHoliday use to read holiday from file to store in our variable
 *  Argument - void
 *  return - void
 */
void readHoliday()
    {
    if(holiday==NULL && holidayDetail==NULL && count==0)
        {
        char lineBuffer[128];
        char *tmp = NULL;
        FILE *pIn = NULL;
        int i=0;
        memset(lineBuffer,0,sizeof(lineBuffer));

        pIn = fopen("holiday.txt","r");

        if(pIn == NULL)
            {
            fprintf(stderr," #Error can't read holiday list\n\n");
            exit(0);
            }

        /* read for header of file */
        if(fgets(lineBuffer,sizeof(lineBuffer),pIn)==NULL)
            {
            fprintf(stderr," #Error can't read holiday size\n\n");
            exit(0);
            }
        sscanf(lineBuffer,"%d",&count);

        holiday = (char **) calloc(count,sizeof(char *));
        holidayDetail = (char **) calloc(count,sizeof(char *));

        if(holiday == NULL || holidayDetail==NULL)
            {
            fprintf(stderr," #Error can't allocate memories\n\n");
            exit(0);        
            }

        /* read for all lines */
        while(fgets(lineBuffer,sizeof(lineBuffer),pIn)!=NULL)
            {
            if(lineBuffer[strlen(lineBuffer)-1]=='\n')
                lineBuffer[strlen(lineBuffer)-1]='\0';

            tmp = &lineBuffer[11];
            lineBuffer[10]='\0';
            *(holidayDetail+i) = strdup(tmp);
            *(holiday+i) = strdup(lineBuffer);

            if(holiday[i] == NULL || holidayDetail[i] == NULL)
                {
                fprintf(stderr," #Error can't allocate memories\n\n");
                exit(0);        
                }
            else
                {
                i++;
                }
            }
        fclose(pIn);
        }
    }

/* 
==========================================================================================================

                                            Public function 
                                            
==========================================================================================================
*/

/* calculateEndDate use to calculate enddate from startDate and duration by avoide holiday and weekend
 *  Arguments - PROJECT_T *pProject pointer to struct of project information
 *             showLog 1 to show detail about weekend if it is 0 for not
 *
 *  return 1 input is weekend 0 No
 */
void calculateEndDate(PROJECT_T *pProject,int showLog)
    {

    int year = 0;
    int month = 0;
    int day = 0;
    int duration = 0 ;
    time_t rawtime;
    struct tm * timeInfo;

    readHoliday();
    sscanf(pProject->startDate,"%d-%d-%d",&day,&month,&year);

    /* count for all duration in all task*/
    VERTEX_T *pTask =  (VERTEX_T *) getVListHead();
    while(pTask!=NULL)
        {
        duration += pTask->duration;
        pTask = pTask->next;
        }

    time(&rawtime);
    timeInfo = localtime ( &rawtime );
    /* get the first start date */
    timeInfo->tm_year = year - 1900;
    timeInfo->tm_mon = month - 1;
    timeInfo->tm_mday = day-1;

    /* check for each date during the period of duration */
    do
        {
        timeInfo->tm_mday = timeInfo->tm_mday+1;
        mktime (timeInfo);
        
        if(isWeekend(timeInfo,showLog)==0 && isHoliday(timeInfo,showLog)==0)
            {   
            duration--;
            }
        }
    while(duration >= 0 );

    /* print out new information */
    printf("New End-date %02d-%02d-%04d\n\n",timeInfo->tm_mday,timeInfo->tm_mon+1,timeInfo->tm_year+1900);
    sprintf(pProject->endDate,"%02d-%02d-%04d",timeInfo->tm_mday,timeInfo->tm_mon+1,timeInfo->tm_year+1900);
    }

/* freeHoliday use to clear all data of holiday
 *  Arguments - void
 *  return - void
 */
void freeHoliday()
    {
    int i=0;
    for(i=0;i<count;i++)
        {
        free(holiday[i]);
        free(holidayDetail[i]);
        }
    free(holiday);
    free(holidayDetail);
    holiday = NULL;
    holidayDetail = NULL;
    }
