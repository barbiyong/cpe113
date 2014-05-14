
 /* calculateEndDate
 * This function will calculate endDate by avoid holiday and weekend
 * Argument : startDate - start date informat dd-mm-yyyy
 *			  duration - how long
 *			  endDate - pointer forhold end date
 *            showLog - 1 for show log of holiday, 0 for display none
 * Returns : void
 */
void calculateEndDate(void *pProject,int showLog);


 /* freeHoliday
 * This function use to free all data of holiday
 * Argument : void
 * Returns : void
 */
void freeHoliday();
