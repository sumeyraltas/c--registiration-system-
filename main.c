#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned int id; // person id
    char name[20]; // person name
    char surname[20]; // person surname
    char job[20]; // job of person
    int view; // match view number of person
} personData;

void showRecords(FILE *filePtr);

int updateView(FILE *filePtr, unsigned int id, int newView);
int addPerson(FILE *filePtr, unsigned int id, char name[], char surname[], char job[], int view);
int deletePerson(FILE *filePtr, unsigned int id);
int showJobRecord(FILE *filePtr, char job[]);

int main()
{
    unsigned int id;
    int newView;
    int status;
    char name[20];
    char surname[20];
    char job[20];
    int view;

    int count;

    FILE *filePtr;
    filePtr = fopen("match.bin","rb+");
    if (filePtr == NULL)
    {
        printf("Could not open match.bin");
        return 0;
    }

    showRecords(filePtr);

    int option;

    printf("\nPlease choose an operation:\n");
    printf("1 : Update View\n");
    printf("2 : Add Person\n");
    printf("3 : Delete Person\n");
    printf("4 : Show Job Record\n");
    printf("> ");
    scanf("%d",&option);

    switch (option)
    {
    case 1:
        printf("\nPerson id: ");
        scanf("%d",&id);
        printf("New view: ");
        scanf("%d",&newView);
        status = updateView(filePtr, id, newView);
        if (status == 1)
            showRecords(filePtr);
        else
            printf("No person with id %d\n", id);
        break;
    case 2:
        printf("\nPerson id: ");
        scanf("%d",&id);
        printf("Name: ");
        scanf("%s",name);
	      printf(" Surname: ");
        scanf("%s",surname);
        printf("Job: ");
        scanf("%s",job);
        printf("View: ");
        scanf("%d",&view);
        status = addPerson(filePtr, id, name, surname, job, view);
        if (status == 1)
            showRecords(filePtr);
        else
            printf("There is already a person with id %d\n", id);
        break;
    case 3:
        printf("\nPerson id: ");
        scanf("%d",&id);
        status = deletePerson(filePtr, id);
        if (status == 1)
            showRecords(filePtr);
        else
            printf("No person with id %d\n", id);
        break;
    case 4:
        printf("\nJob: ");
        scanf("%s",job);
        count = showJobRecords(filePtr, job);
        if (count == 0)
            printf("No person in job %s\n", job);
        else
            printf("There are %d people in job %s\n", count, job);
        break;
    }

    fclose(filePtr);
    return 0;
}

void showRecords(FILE *filePtr)
{
    fseek(filePtr, 0, SEEK_SET);

    printf("\n%-3s %-20s %-20s %-20s %s\n",
                   "ID",
                   "Name",
		               "Surname",
                   "Job",
                   "View");

    while (!feof(filePtr))
    {
        personData person;
        int result = fread(&person, sizeof(personData), 1, filePtr);
        if (result != 0 && person.id != 0)
        {
          printf("%-3d %-20s %-20s %-20s %d\n",
            person.id,
            person.name,
		        person.surname,
            person.job,
            person.view);
        }
    }
}

int updateView(FILE *filePtr, unsigned int id, int newView)
{
     fseek(filePtr, 0, SEEK_SET);
     int result;
     personData person;
        while (!feof(filePtr))
        {
                fread(&person, sizeof(personData), 1, filePtr);
                if (result != 0 && person.id == id)
                {
                  person.view = newView ;
                  fseek(filePtr,(int)sizeof(person), 1);
                  fwrite(&person, sizeof(person), 1, filePtr);
                  return 1;
                }
        }
        return 0;
}

int addPerson(FILE *filePtr, unsigned int id, char name[], char surname[], char job[], int view)
{
    personData person;
    person.id=id;
    strcpy(person.name,name);
    strcpy(person.surname,surname);
    strcpy(person.job,job);
    person.view = view;
    fwrite(&person, sizeof(personData), 1, filePtr);
    return 1;
}

int deletePerson(FILE *filePtr, unsigned int id)
{
  fseek(filePtr, 0, SEEK_SET);
  personData person;
        while (!feof(filePtr)){
                int result = fread(&person, sizeof(personData), 1, filePtr);
                if(result != 0 && person.id == id){
                   fseek(filePtr, (int)sizeof(person), 1);
                   fwrite(NULL, sizeof(person), 1, filePtr);
                return 1;
                }
        }
        return 0;
}

int showJobRecords(FILE *filePtr, char job[])
{
 fseek(filePtr, 0, SEEK_SET);
 printf("\n%-3s %-20s %-20s %-20s %s\n",
          "ID",
          "Name",
		      "Surname",
          "Job",
          "View");
  int count=0;
  personData person;
  while (!feof(filePtr))
  {
    int result = fread(&person, sizeof(personData), 1, filePtr);
    if (result != 0 && person.id != 0 && strcmp(person.job,job)==0)
    {
    printf("%-3d %-20s %-20s %-20s %d\n",
      person.id,
      person.name,
      person.surname,
      person.job,
      person.view);
        count++;
    }
  }
  return count;
}
