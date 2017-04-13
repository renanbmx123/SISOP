#include<stdio.h>

int filesize(FILE* fp){
	int curr = ftell(fp);
	int sz;
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

int main(){
	FILE* fp;
	fp = fopen("Aula09b.pdf","r");
	printf ("%d\n", filesize(fp));
	fclose(fp);
}
