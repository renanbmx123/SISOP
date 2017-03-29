#include <stdio.h>

int main( void ) {
	char *argv[4] = {".", "-name", "*.c", NULL};

	int pid = fork();

	if ( pid == 0 ) {
		execvp( "find", argv );
	}

	/* Put the parent to sleep for 2 seconds--let the child finished executing */
	wait( );

	printf("Finished executing the parent process\n"
	        " - the child won't get here--you will only see this once\n");

	return 0;
}