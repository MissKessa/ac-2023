#include <stdio.h>
#include <string.h>
struct _Person
{
char name[30];
int heightcm;
double weightkg;
};
// This abbreviates the type name
typedef struct _Person Person;
typedef Person* PPerson;
int main(int argc, char* argv[])
{
Person Peter;
PPerson pointerPeter;
pointerPeter = &Peter;

strcpy(Peter.name, "Peter");
Peter.heightcm = 175;
// TODO: Assign the weight
Peter.weightkg  = 2;
pointerPeter->weightkg  = 78.7;
// TODO: Show the information of the Peter data structure on the screen
printf("%s's height: %d cm; %s's weight: %.2f kg \n",Peter.name,Peter.heightcm,Peter.name, Peter.weightkg);
return 0;
}
