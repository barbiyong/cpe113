
/* calculateEndDate use to calculate enddate from startDate and duration by avoide holiday and weekend
 * Arguments - PROJECT_T *pProject pointer to struct of project information
 *             showLog 1 to show detail about weekend if it is 0 for not
 *
 * return 1 input is weekend 0 No
 */
void calculateEndDate(void *pProject,int showLog);


/* freeHoliday
 * This function use to free all data of holiday
 * Argument : void
 * Returns : void
 */
void freeHoliday();
