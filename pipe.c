/* pipe demo */
/* Paul Krzyzanowski */

#include <stdlib.h>
#include <stdio.h>


void runpipe();

static char memfile[1024];
static char format[] = "ping -c 1 -w 1 %s";
static char command[128] = "ping -c 1 -w 1 %s";

int main(int argc, char **argv)
{
	char str[1024];
	FILE *fpipe;
	
	sprintf(command, format, "10.32.175.201");

	if ( (fpipe = popen(command, "r"))<0){
		perror("popen");
	}
	
	/* Ignora primeira linha */
	/* PING 10.32.175.200 (10.32.175.200) 56(84) bytes of data. */
	fgets(str, 1023, fpipe);

	/* Verifica se teve sucesso no ping com a segunda linha */
	/* 64 bytes from 10.32.175.200: icmp_req=1 ttl=64 time=0.023 ms */
	fgets(str, 1023, fpipe);
	if(strcmp(str, "\n"))
		printf("sucesso!\n");
	else
		printf("falha!\n");

	fclose(fpipe);
}