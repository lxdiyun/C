#include <stdio.h>
#include <string.h>

#define MAX_MACHINES_C 65535
#define MAX_COMPOUBD_C 1000

int machines[MAX_MACHINES_C][5];
int sortedMachines[MAX_MACHINES_C];
int compoubds[MAX_COMPOUBD_C][MAX_COMPOUBD_C];
int machineAmount, compoubdAmount;

void insertMachines(int machine, int price)
{
   int treeNode = 0;

   while (0 != sortedMachines[treeNode])
   {
      if (price < machines[sortedMachines[treeNode]][0])
      {
         treeNode = 2 * treeNode + 1;
      }
      else 
      {
         treeNode = 2 * treeNode + 2;
      }
   }

   sortedMachines[treeNode] = machine;
}

void listMachines(int treeNode)
{
   int machine = sortedMachines[treeNode];

   if (0 != machine)
   {

      listMachines(2 * treeNode + 1);

      printf("%d %d %d %d\n", 
             machines[machine][3],
             machines[machine][0],
             machines[machine][1],
             machines[machine][2]);

      listMachines(2 * treeNode + 2);
   }

}

void checkMachines(int treeNode)
{
   int machine = sortedMachines[treeNode];

   if (0 != machine)
   {
      int price = machines[machine][0];
      int origComp = machines[machine][1];
      int desComp = machines[machine][2];
      int i,j;

      checkMachines(2 * treeNode + 1);

      printf("%d %d %d %d\n", 
             machines[machine][3],
             machines[machine][0],
             machines[machine][1],
             machines[machine][2]);

      if (0 == compoubds[origComp][desComp])
      {
         machines[machine][4] = 1;

         for (i = 1; i <= compoubdAmount; ++i)
         {
            for (j = 1; j <= compoubdAmount; ++j)
            {
               if (0 == compoubds[i][j])
               { 
                  if ((0 != compoubds[i][origComp]) &&
                      (0 != compoubds[desComp][j]))
                  {

                     if (i == origComp)
                     {
                        compoubds[i][j] = machine;
                        printf("\n|i=%d j=%d machine=%d orig=%d des=%d |\n", i, j, machine, origComp, desComp);
                     }
                     else 
                     {
                        compoubds[i][j] = compoubds[i][origComp];
                     }
                  }
               }
            }
         }
      }

      checkMachines(2 * treeNode + 2);
   }
}

int main(int argc, char *argv[])
{
   int compoubd,compoubd1, compoubd2;
   int machine, origComp, desComp, price;
   int totalPrice, firstMachine;
   FILE* iFile = fopen(argv[1], "r");

   machineAmount = 1;
   compoubdAmount = 0;

   bzero((void*)compoubds, MAX_COMPOUBD_C * MAX_COMPOUBD_C * sizeof(int));
   bzero((void*)sortedMachines, MAX_MACHINES_C * sizeof(int));
   bzero((void*)machines, MAX_MACHINES_C * 5 * sizeof(int));

   while (EOF != fscanf(iFile, 
                        "M%d C%d C%d %d\n", 
                        &machine,
                        &origComp,
                        &desComp,
                        &price))
   {
      machines[machineAmount][0] = price;
      machines[machineAmount][1] = origComp;
      machines[machineAmount][2] = desComp;
      machines[machineAmount][3] = machine;

      insertMachines(machineAmount, price);
      ++machineAmount;

      if (0 == compoubds[origComp][origComp]) 
      {
         compoubds[origComp][origComp] = -1;
         ++compoubdAmount;
      }

      if (0 == compoubds[desComp][desComp])
      {
         compoubds[desComp][desComp] = -1;
         ++compoubdAmount;
      }
   }

   fclose(iFile);

   for(machine = 1; machine < machineAmount; ++machine)
   {
      printf("%d %d %d\n", 
             machines[machine][0],
             machines[machine][1],
             machines[machine][2]);
   }
   printf("------------------------\n");

   listMachines(0);
   printf("------------------------\n");
   checkMachines(0);

   printf("------------------------\n");
   for (compoubd1 = 1; compoubd1 <= compoubdAmount; ++compoubd1)
   {
      for (compoubd2 = 1; compoubd2 <= compoubdAmount; ++compoubd2)
      {
         printf("%d ", compoubds[compoubd1][compoubd2]);
      }
      printf("\n");
   }



   firstMachine = 1;
   totalPrice = 0;
   for (machine = 1; machine < machineAmount; ++machine)
   {
      if (1 == machines[machine][4]) 
      {
         totalPrice += machines[machine][0];
         if (1 == firstMachine)
         {
            printf("%d", machines[machine][3]);
            firstMachine = 0;
         }
         else
         {
            printf(" %d", machines[machine][3]);
         }
      } 
   } 
   printf("\n");
   printf("%d\n", totalPrice);

   return 0;
}
