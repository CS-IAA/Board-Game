#include "apue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXPLAYERS 1000
#define MAXLEN 100
#define LINESIZE 16

double amounts[MAXPLAYERS];
char names[MAXPLAYERS][MAXLEN];
int numPlayers=0;

int findIndex(char *name);

int main(int argc, char *argv[])
{
//	char space = ' ';
//	int save = 0;
//        int j=0;
	if (strcmp(argv[1], "new") == 0)
	{
		//create a new file
		FILE *foutput = fopen("gifts.dat", "w");
		for(int i=2; i<argc; i+=2)
		{
  //                      for(int column=0; column<LINESIZE; column++)
//				fwrite(&space, sizeof(char), 1, foutput);
//                        fseek(foutput, (LINESIZE*j), SEEK_SET);
			
			int len = strlen(argv[i]) + 1;
			//write the name length
			fwrite(&len, sizeof(int), 1, foutput);

			//write the name
			fwrite(argv[i], sizeof(char), len, foutput);
		
			//write the amount
			double amount = atof(argv[i+1]);
			fwrite(&amount, sizeof(double), 1, foutput);

//			j++;
			printf("%10s %5.2lf\n", argv[i], amount);
		}
		fclose(foutput);
		return 0;
	}
	else
	{
		// WHILE LOOP DOESNT INCRMENT i
		//process transaction
		FILE *finput = fopen("gifts.dat", "r");
		int i=0, len=0;
		while (fread(&len, sizeof(int), 1, finput) > 0)
		{
			//read that many characters as the name
			fread(&names[i][0], sizeof(char), len, finput);
			fread(&amounts[i], sizeof(double), 1, finput);
			i++;
		}
		fclose(finput);
		numPlayers = i;
	}

	int giverIndex = findIndex(argv[1]);
	double amountToSplit = atof(argv[2]);
	amounts[giverIndex] -= amountToSplit;

	double delta = amountToSplit / (argc-3);
	for(int i=3; i<argc; i++)
	{
		int receiverIndex = findIndex(argv[i]);
		amounts[receiverIndex] += delta;	
	}

	FILE *foutput = fopen("gifts.dat", "w");
	for(int i=0; i<numPlayers; i++)
	{
		int len = strlen(names[i]) + 1;
		//write the name length
		fwrite(&len, sizeof(int), 1, foutput);
		//write the name
		fwrite(names[i], sizeof(char), len, foutput);
		fwrite(&amounts[i], sizeof(double), 1, foutput);
		printf("%10s %5.2lf\n", names[i], amounts[i]);
	}
	fclose(foutput);

	return 0;	
}


int findIndex(char *name)
{
	for(int i=0; i<numPlayers; i++)
		if (strcmp(name, names[i]) == 0)
		return i;
	return -1;
}
