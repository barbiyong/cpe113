/* writeData
 * write all of project's data to output file.
 *
 * Arguments 
 * 		outputFile - char *outputFile output filename
 *				   - void* pProject pointer to project structer information
 * Returns : 1 for success, 0 for failure.
 */
int writeData(char *outputFile,void* pProject);

/* readData
 * read the data from files to store in the programe
 * Arguments 
 * 		outputFile - char *inputFile input filename
 *				   - void* pProject pointer to project structer information
 *				   - int showLog 1 is showlog 0 is none
 * Returns : 1 for success, 0 for failure.
 */
int readData(char *inputFile,void* pProject,int showLog);
