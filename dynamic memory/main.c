#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "book.h"

int main(void) {


     struct List phoneBook1 = {NULL, NULL};

     Command cmd;

     
     do {
       printMenu();
       cmd = getUserCommand();
       executeCommand(cmd, &phoneBook1);
     } while (cmd != EXIT);


    return 0;
  }


