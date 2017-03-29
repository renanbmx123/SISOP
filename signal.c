#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
  if (signo == SIGUSR1){
	printf("received SIGUSR1\n");
  }
}

int main(void)
{
  if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	printf("\ncan't catch SIGUSR1\n");
  
  while(1){
	sleep(1);
  }
  
  return 0;
}