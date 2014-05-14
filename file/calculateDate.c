/*
 *  calculateDate.c
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

char **holiday = NULL;
char **holidayDetail = NULL;
int count=0;

/* 
==========================================================================================================

                                            Private function 
                                            
==========================================================================================================
*/
int isHoliday(struct tm *timeCheck,int showDetail)
    {
    char date[64];
    int i=0;
    sprintf(date,"%02d-%02d-%4d",timeCheck->tm_mday,timeCheck->tm_mon+1,timeCheck->tm_year + 1900);
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
int isWeekend(struct tm *timeCheck,int showDetail)
    {
    if(timeCheck->tm_wday==0 || timeCheck->tm_wday==6)
        {
        if(showDetail)
            printf("Weekend\n");
        return 1;
        }
    else
        return 0;
    }


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
            fprintf(stderr,"Error can't read holiday list\n");
            exit(0);
            }
        if(fgets(lineBuffer,sizeof(lineBuffer),pIn)==NULL)
            {
            fprintf(stderr,"Error can't read holiday size\n");
            exit(0);
            }
        sscanf(lineBuffer,"%d",&count);

        holiday = (char **) calloc(count,sizeof(char *));
        holidayDetail = (char **) calloc(count,sizeof(char *));

        if(holiday == NULL || holidayDetail==NULL)
            {
            fprintf(stderr,"Error can't allocate memories\n");
            exit(0);        
            }

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
                fprintf(stderr,"Error can't allocate memories\n");
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

    VERTEX_T *pTask =  (VERTEX_T *) getVListHead();
    while(pTask!=NULL)
        {
        duration += pTask->duration;
        pTask = pTask->next;
        }

    time(&rawtime);
    timeInfo = localtime ( &rawtime );

    timeInfo->tm_year = year - 1900;
    timeInfo->tm_mon = month - 1;
    timeInfo->tm_mday = day-1;


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

    printf("New calclulate End-date %02d-%02d-%04d\n",timeInfo->tm_mday,timeInfo->tm_mon+1,timeInfo->tm_year+1900);
    sprintf(pProject->endDate,"%02d-%02d-%04d",timeInfo->tm_mday,timeInfo->tm_mon+1,timeInfo->tm_year+1900);
    }


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
