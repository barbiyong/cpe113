/**************************************************
 *
 * validateFunctions.h
 *  Header file of validate functions that contain
 * every validate functions of this programe
 *
 * Created by Phasathorn Suwansri (Lukkid)
 *						ID : 56070503424
 * Date : 31 March 2014
 * Modified Date : 2 April 2014
 *
 **************************************************
 */
 
/* validateDuration
 * This function will validate the duration by
 * check the digits of them. After that save 
 * them to the task
 * Argument : input - string for duration
 * Returns : 1 for success, 0 for failure.
 */
int validateDuration(char input[]);

/* validateStartProjectDate
 * This function will validate the start project date by
 * check with today date.
 * Argument : input - string for start project date
 *            project - pointer to project node.
 * Returns : 1 for success, 0 for failure.
 */
int validateStartProjectDate(char input[],void* pProject);

