#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>          
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_VALUE 255

int readLine(int fd, char* output);
int readSubFolder(char* StudentPath, char* InputPath, char* OutputPath, char* folderName);
int exeHelp(char* args[]);
int writeToCSVFile(char* studentName, char* result);


int my_strcmp(const char* s1, const char* s2);
void my_strcat(char* str1, char* str2);
int my_strlen(char* s);
char* my_strcpy(char* destination, const char* source);


int main(int argc, char* argv[])
{
	DIR* studentDir;
	struct dirent* folder;
	int configFile, CSVFile;
	char StudentPath[MAX_VALUE], InputPath[MAX_VALUE], OutputPath[MAX_VALUE];
	//Check if the given number of arguments are ok
	if (argc != 2) 
	{
		perror("Problem with number of arguments\n");
		exit(-1);
	}
	//open the configuration file
	configFile = open(argv[1], O_RDONLY);
	if (configFile < 0)
	{
		perror("Problem with opening the config file\n");
		close(configFile);
		exit(-1);
	}
	//reads the 3 path that are in the configuration file
	readLine(configFile, StudentPath);
	readLine(configFile, InputPath);
	readLine(configFile, OutputPath);
	//Print the paths
	printf("The path in the file are:\n");
	printf("First Path: %s\n", StudentPath);
	printf("Second Path: %s\n", InputPath);
	printf("Third Path: %s\n", OutputPath);
	printf("\n\n");

	//Create a new results file for every run
	CSVFile = open("results.csv", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (CSVFile < 0) {
		printf("Problem with opening the CSV file \n");
		close(configFile);
		close(CSVFile);
		exit(-1);
	}
	//Opens the Student folder using the first path in the configuration file
	studentDir = opendir(StudentPath);
	if (studentDir == NULL) {
		printf("Problem with opening students folder");
		close(configFile);
		closedir(studentDir);
		exit(-1);
	}

	//open each student folder and compile his C program
	while ((folder = readdir(studentDir)) != NULL)
	{
		if (readSubFolder(StudentPath, InputPath, OutputPath, folder->d_name) < 0) {
			perror("Problem with reading from students folders");
			close(configFile);
			closedir(studentDir);
			exit(-1);
		}
	}
	printf("Results file:\n");
	char* show[] = { "cat","results.csv",NULL };
	if (exeHelp(show) < 0)
	{
		perror("Prbolem with CSV file");
		close(configFile);
		closedir(studentDir);
		exit(-1);
	}

	close(configFile);
	closedir(studentDir);
	return 0;

}

//the config file is given by argument and ee assume it is already check and without any problem and bugs.
int readLine(int fd, char* output)
{
	int it = 0;
	int currentLine = 0;
	char buf;
	while (read(fd, &buf, 1) > 0)
	{
		if (buf != '\n')
		{
			output[it++] = buf;
		}
		else if (buf == '\n')
		{
			currentLine++;
			break;
		}
	}
	if (it == 0)
		return 0;
	output[it] = '\0';
	return 1;
}

//Help function that will execute each student program with help of father-son process using fork
int exeHelp(char* args[]) {

	pid_t pid;
	int returned_value;

	pid = fork();
	if (pid < 0) //There is some kind of problem
		return -1;

	if (pid == 0) { //if pid is 0 so I am the son process
		execvp(args[0], args);
		exit(-1);// If exec will work propertly the program will never reach to this point
	}
	wait(&returned_value);
	//if pid>0 I am the father process
	return WEXITSTATUS(returned_value);
}
//Function thats write to the CSV file the grades of each student
int writeToCSVFile(char* studentName, char* result) {

	char studentResult[MAX_VALUE] = "";
	int size;
	int resultsFile; 
	resultsFile = open("results.csv", O_WRONLY | O_APPEND);
	if (resultsFile < 0)
	{
		perror("Problem with opening the results file\n");
		close(resultsFile);
		exit(-1);

	}
	//Write socre to file
	my_strcat(studentResult, studentName); 
	my_strcat(studentResult, ",");
	my_strcat(studentResult, result);
	my_strcat(studentResult, "\n");
	size = my_strlen(studentResult);
	if (write(resultsFile, studentResult, size) < 0)
		return -1;
	return 0;
}

int readSubFolder(char* StudentPath, char* InputPath, char* OutputPath, char* folderName)
{
	int keyboard = dup(0);
	int screen = dup(1);
	struct stat mystat;
	char path[MAX_VALUE] = "";
	char sourcepath[MAX_VALUE] = "";
	char destpath[MAX_VALUE] = "";
	int retValue, input, output;

	if ((my_strcmp(folderName, ".") == 0) || (my_strcmp(folderName, "..") == 0)) //the folder are in the wrong location 
		return 0;

	my_strcat(path, StudentPath);
	my_strcat(path, folderName);
	if (stat(path, &mystat) < 0) {
		perror("Problem with stat\n");
		return -1;
	}

	// Source
	my_strcat(path, "/");
	my_strcat(sourcepath, path); 
	my_strcat(sourcepath, folderName); 
	my_strcat(sourcepath, ".c");

	//Destination
	my_strcat(destpath, path);
	my_strcat(destpath, "main.out");

	//Compilation
	char* args[] = { "gcc" , sourcepath , "-o" , destpath , NULL };   //Build gcc command
	if (exeHelp(args) < 0) {
		perror("Problem with compilation of the files\n");
		return -1;
	}
	
	//open the input file from the second path in the configuration file and create a output text file that we willl compare with the expected output
	input = open(InputPath, O_RDONLY);
	if (input < 0)
	{
		perror("Problem with opening input file\n");
		close(input);
		return -1;
	}
	output = open("ProgramOutput.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (output < 0) {
		printf("Problem with opening outputfile\n");
		close(input); 
		close(output); 
		return -1;
	}

	//Read from input file and write to program output file
	dup2(input, 0);
	dup2(output, 1);

	//Run ./ command on main.out for each student
	char command[MAX_VALUE] = "";
	my_strcat(command, "./students/"); 
	my_strcat(command, folderName);
	my_strcat(command, "/main.out");


	char* runArgs[] = { command , NULL }; //execute each student program 
	if (exeHelp(runArgs) < 0) {
		perror("Problem with executing main.out\n");
		close(input); 
		close(output); 
		return -1;
	}
	//Default of the operation system
	dup2(keyboard, 0);
	dup2(screen, 1);

	//Run ./ command on comp.out
	char* programArgs[] = { "./comp.out" , "ProgramOutput.txt" , OutputPath , NULL }; //execute the prgram from the first section of the HW thats compare 2 files
	retValue = exeHelp(programArgs);
	if (retValue < 0) {
		printf("Problem with the first program \n");
		close(input); 
		close(output); 
		return -1;
	}
	if (retValue == 2) {
		if (writeToCSVFile(folderName, "2") < 0) 
		{
			printf("Problem with writing to the csv file\n");
			close(input); 
			close(output); 
			return -1;
		}
	}
	else {
		if (writeToCSVFile(folderName, "1") < 0) 
		{
			printf("Problem with writing to the csv file\n");
			close(input); close(output);
			return -1;
		}
	}

	close(input);
	close(output);
	return 0;
}

//---------------------STRING FUNCTION------------------------------------------------------
// Function to implement my_strcpy() function
char* my_strcpy(char* destination, const char* source)
{
	// return if no memory is allocated to the destination
	if (destination == NULL)
		return NULL;

	// take a pointer pointing to the beginning of destination string
	char* ptr = destination;

	// copy the C-string pointed by source into the array
	// pointed by destination
	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;
	}

	// include the terminating null character
	*destination = '\0';

	// destination is returned by standard my_strcpy()
	return ptr;
}
int my_strlen(char* s)
{
	int i;
	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return i;
}
void my_strcat(char* str1,  char* str2)
{
	int i = 0, j = 0;

	while (str1[i] != '\0')
	{
		i++;
	}
	while (str2[j] != '\0') { 
		str1[i] = str2[j];
		i++;
		j++;
	}
	str1[i] = '\0';
}
int my_strcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
//---------------------STRING FUNCTION------------------------------------------------------

