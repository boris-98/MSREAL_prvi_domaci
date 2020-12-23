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
	char *option = (char*)malloc(op_bytes+1);
	char *string;

	while(1)
	{
		printf("----------Aplikacija za rad sa drajverom----------\n");
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
				fptr = fopen("/dev/stred", "r");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
					return -1;
				}
				string = (char*)malloc(string_bytes);
				getline(&string, &string_bytes, fptr);
			
				if(fclose(fptr))
				{
					printf("Problem pri zatvranju /dev/stred\n");
					return -1;
				}
				
				printf("Trenutni string je:%s\n\n", string);
				free(string);
				break;

			case '2':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	
				string = (char*)malloc(string_bytes);
				
				printf("Unesi string za upis:\n");
				getline(&string, &string_bytes, stdin);
				fprintf(fptr, "string=%s", string);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}

				free(string);
				break;

			case '3':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	
				string = (char*)malloc(string_bytes);
				
				printf("Unesi string za konkatanaciju:\n");
				getline(&string, &string_bytes, stdin);
				fprintf(fptr, "append=%s", string);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}

				free(string);
				break;

			case '4':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	

				fputs("clear\n", fptr);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}

				break;

			case '5':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	

				fputs("shrink\n", fptr);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}
				break;

			case '6':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	
				string = (char*)malloc(string_bytes);
				
				printf("Unesi izraz za brisanje iz stringa:\n");
				getline(&string, &string_bytes, stdin);
				fprintf(fptr, "remove=%s", string);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}

				free(string);
				
				break;
			case '7':
				fptr = fopen("/dev/stred", "w");
				if(fptr == NULL)
				{
					printf("Greska pri otvaranju /dev/stred\n");
				}	
				string = (char*)malloc(string_bytes);
				
				printf("Unesi koliko karaktera da se obrise sa kraja:\n");
				getline(&string, &string_bytes, stdin);
				fprintf(fptr, "truncate=%s", string);

				if(fclose(fptr))
				{
					printf("Greska prilikom zatvaranja /dev/stred\n");
					return -1;
				}

				free(string);
					
				break;
			case 'q':
				printf("pozz\n");
				free(option);
				return 0;
			default:
				printf("Greska\n");
				break;	
		}
	}	




	return 0;
}



