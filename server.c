#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

/* Exemplo de servidor com sockets TCP que retorna data e hora do sistema. */

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, clientaddr;
    char sendBuff[1025];
    time_t ticks; 
    socklen_t sz;


    memset(sendBuff, '0', sizeof(sendBuff)); 

    /* Passo 1 - Criar Socket */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	  perror("socket: ");
	  exit(1);
    }

    /* Passo 2 - Configurar estrutura sockaddr_in */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 
    
    /* Passo 3 - Associar socket com a estrutura sockaddr_in     */
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
   	  perror("bind: ");
	  exit(1);
    }

    /* Passo 4 - Tornar o servidor ativo  */
    listen(listenfd, 10); 

    while(1)
    {
	/* Passo 5 - Aguardar conexão do cliente.  */
	sz = sizeof(clientaddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &sz)) < 0){
		perror("accept: ");
		continue;
	}
	
	/* Exibe ip e porta do cliente */
	printf("Origem cliente: %s:%d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);

	/* obtem data e hora do sistema e converte para string */
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
	
	/* Envia a mensagem para o cliente. */
        send(connfd, sendBuff, strlen(sendBuff), 0); 
	
	/* Encerra conexão com o cliente. */
        close(connfd);
     }
}

