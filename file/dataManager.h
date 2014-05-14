/* writeData
 * write all of project's data to output file.
 *
 * Arguments 
 * 		outputFile - output filename
 * Returns : 1 for success, 0 for failure.
 */
int writeData(char *outputFile,void* pProject);

/* readData
 * read the data from files to store in the programe
 * Arguments 
 * 		outputFile - output filename
 *
 * Returns : 1 for success, 0 for failure.
 */
int readData(char *inputFile,void* pProject,int showLog);