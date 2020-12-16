#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRING_SIZE 101

int main(int argc, char **argv)
{
	FILE *fptr;
	size_t op_bytes = 1;
	size_t string_bytes = STRING_SIZE;
	char *option = (char*)malloc(op_bytes);
	unsigned char kraj = 0;

	while(1)
	{
		printf("-----Aplikacija za rad sa drajverom-----\n");
		printf("Odaberi neku od opcija 1...7 ili q za izlaz\n");
		printf("\t1: Procitaj trenutno stanje stringa\n");
		printf("\t2: Upisi novi string\n");
		printf("\t3: Konkataniraj string na trenutni\n");
		printf("\t4: Izbrisi citav string\n");
		printf("\t5: Izbrisi vodece i pratece space karaktere\n");
		printf("\t6: Izbrisi izraz is stringa\n");
		printf("\t7: Izbrisi poslednjih n karaktera stringa\n");
		getline(&option, &op_bytes, stdin);

		switch(*option)
		{
			case '1':
				printf("Proba....1\n");
				break;
			case '2':
				printf("Proba....2\n");
				break;
			case '3':
				printf("Proba....3\n");
				break;
			case '4':
				printf("Proba....4\n");
				break;
			case '5':
				printf("Proba....5\n");
				break;
			case '6':
				printf("Proba....6\n");
				break;
			case '7':
				printf("Proba....7\n");
				break;
			case 'q':
				printf("pozz\n");
				kraj = 1;
				break;
			default:
				printf("Greska\n");
				break;	
		}
		if(kraj)
		{
			free(option);
			break;
		}
	}	




	return 0;
}



