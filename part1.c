#include <sys/fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include  <unistd.h>
int main(int argc,char* argv[])
{
	int fd1,fd2;
	char in1,in2;
	
	if(argc!=3) //Check if there is 3 arguments 
	{
		perror("Problem with number of arguments");
		return 1;
	}
	
	fd1=open(argv[1], O_RDONLY); //open the file1 with read only permission
	if(fd1<0) 
	{
	perror("Problem with opening file number 1");
	return 1;
	}
	fd2=open(argv[2], O_RDONLY); //open the file2 with read only permission
	if(fd2<0) 
	{
	perror("Problem with opening file number 2");
	return 1;
	}
	
	while(read(fd1,&in1,1) != 0 && read(fd2, &in2, 1) != 0) 
	{
		if(in1!=in2) 
		{
			return 1; //the file are diffrent
		}
	}
	close(fd1);
	close(fd2);
	return 2; //the loop end and thats means that the file are the same
	
	
}
