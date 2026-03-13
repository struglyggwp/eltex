#include <stdlib.h>
#include <stdio.h>
#include "libcalc.h"



typedef enum { PLUS = 1, MINES, MULTI, DIVIDE, EXIT } Command;

void executeCommand(Command cmd) {
  int a, b;
  int result;
  float f_result;

  if (cmd == EXIT) {
    printf("EXIT.\n");
    return;
  }


  printf("Enter two numbers separated by a space:\n");
  if (scanf("%d %d", &a, &b) != 2) {
    printf("Input error! You need to enter two numbers.\n");

    while (getchar() != '\n');
    return;
  }

  switch (cmd) {
    case PLUS:
      result = plus(a, b); 
      printf("Result: = %d\n", result); 
      break;
    case MINES:
      result = mines(a, b);
      printf("Result: = %d\n", result);
      break;
    case MULTI:
      result = multi(a, b);
      printf("Result:  = %d\n", result);
      break;
    case DIVIDE:
      if (b == 0) {
        printf("ERROR: Division by zero!\n");
      } else {
        f_result = divide(a, b);
        printf("Result:  = %.2f\n", f_result);
      }
    case EXIT:
      break;
      break;

  }
}

Command getUserCommand() {
  int input;

  while (1) {
    printf("\nError: Division by zeroSelect an action (1-5): ");

    if (scanf("%d", &input) != 1) {
      printf("Error! Please enter a NUMBER between 1 and 5.\n");

      while (getchar() != '\n');

      continue;
    }


    if (input >= 1 && input <= 5) {

      return (Command)input;
    } else {

      printf("Error! There is no such menu item. Enter a number from 1 to 5.\n");

      while (getchar() != '\n');
    }
  }
}





void printmenu() {
    
    printf("Calculator\n");
    printf("1. Addition \n");
    printf("2. Subtraction.\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Exit\n");

}



int main()
{  

    Command cmd;
    do {
      printmenu();
        cmd = getUserCommand();
        executeCommand(cmd);
    } while (cmd != EXIT);
    return 0;

}

