/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2017 09:29:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  J.T. Nunley (JaydomStudios), jtnunley01@gmail.com
 *   Organization:  JaydomStudios
 *
 * =====================================================================================
 */

#include "err.h"
#include "shortcuts.h"
#include "program.h"
#include "branches_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAX_SIZE 256

// defined in brstring.cpp
char *brToString(branch *br);
branch *brFromString(char *str);

// defined in string_helper.cpp
char *getAllLinesFromFile(char *filename);

// prints version information
void print_version(int verbose) {
  if (!verbose)
    printf("====== Welcome to Branches v%i.%i.%i ======",br_VERSION,br_REVISION,br_MINOR_REVISION);
  else
    printf("You are using: Branches Version %i Revision %i Minor Revision %i\n",br_VERSION,br_REVISION,br_MINOR_REVISION);
}

// prints help menu
void print_cmds() {
  puts("List of commands:");
  puts("1 - select branch number one");
  puts("2 - select branch number two");
  puts("r - return to start");
  puts("o - go backwards by one node");
  puts("p - reprints the current node to the console");
  puts("s - saves current tree to a file");
  puts("v - prints version information");
  puts("c - prints change log");
  puts("h - list this help menu again");
  puts("e - exit the game");
  puts("Enter the letter and press the Enter key to use the command");
}

// make a new branch from user input
branch *usr_input_branch() {
  char main_text[INPUT_MAX_SIZE];
  char b1[INPUT_MAX_SIZE];
  char b2[INPUT_MAX_SIZE];
  char response[4];
  char dummy[4];

  do {
    newline();
    /*
    printf("Enter main text: ");
    //scanf("%s",main_text);
    fgets(main_text,INPUT_MAX_SIZE,stdin);
    if ((strlen(main_text)>0) && (main_text[strlen (main_text) - 1] == '\n'))
        main_text[strlen (main_text) - 1] = '\0';

    printf("Enter Option 1: ");
    //scanf("%s",b1);
    fgets(b1,INPUT_MAX_SIZE,stdin);
    if ((strlen(b1)>0) && (b1[strlen (b1) - 1] == '\n'))
        b1[strlen (b1) - 1] = '\0';

    printf("Enter Option 2: ");
    //scanf("%s",b2);
    fgets(b2,INPUT_MAX_SIZE,stdin);
    if ((strlen(b2)>0) && (b2[strlen (b2) - 1] == '\n'))
        b2[strlen (b2) - 1] = '\0';
    */
    fgets(dummy,4,stdin);
    getInput("Enter main text",main_text);
    getInput("Enter Option 1",b1);
    getInput("Enter Option 2",b2);
    newline();
    puts("Here is your branch:");
    newline();
    branch *br = brCreate(b1,b2,main_text);
    //branch br = brCreate("test","test",main_text);
    brPrint(br);
    if (yesno("Is this okay?")) {
      newline();
      return br;
    }
  } while (1); 
  // inaccessible code
  error("Inaccessible code",0);
  // even more inaccessible code
  return brCreate("","","");
}

// program entry point
int runProgram() {
  // introduce the help menu and version info
  print_version(0);
  newline();

  puts("Select mode:");
  char cmd[5];
  int thing = 1;
  do {
    puts("-1: Singleplayer Mode");
    puts("-2: Connect to TBranches Server");
    puts("-3: Connect to other server");
    newline();
    shell_tx();
    scanf("%4s",cmd);
    switch (cmd[0]) {
      case 'e':
        puts("Exiting branches...");
        return 0;
      case '1':
	thing = 0;
	break;
      case '2':
      case '3':
        puts("Internet connection not implemented as of this version.");
      default:
        puts("Invalid command");
        shell_tx();
	scanf("%4s",cmd);
	break;
    }
  } while (thing);
  newline();
  print_cmds();
  newline();

  // initial branch
  branch *root = brCreate("Take the left path", "Take the right path", "You come across a fork in the road.");
  brPrint(root);
//  initializeTree(&root);
  branch *br1;
  branch *br2;
 
  branch *third_tree = brCreate("Drop kick it","Stab someone","You pick up the fork");

  // other two branches
  br1 = brCreate("Knock on the door","Peer through the window","You come across a house on the prarie.");
  br2 = brCreate("Cross the bridge","Jump into the water","You come across a bridge crossing a river.");
  brSetBranch1(root,br1);
  brSetBranch2(root,br2);
 
  // current branch
  branch *current = root;
  
  // don't use loop.h for this; loop.h is needed for inner loops
  // also, this is the main loop
  int cont = 1;
  
  do {
    // this makes the > thing
    shell_tx();
    // get the command
    scanf("%s",cmd); 

    // outdated debugging stuff
    #ifdef DEBUG
    printf("You entered: %s",cmd);
    #endif

    // wat do?
    switch (cmd[0]) {
      case 'e':
        // user wants to exit the game
        printf("Exiting Branches...\n");
	cont = 0;
	break;
      case 'v':
        // print verbose game information
        print_version(1);
	break;
      case 'h':
        // reprint help information
        print_cmds();
	break;
      case '2':
        if (brHasBranch2(current))
          current = brGetBranch2(current);
	else {
	  if (yesno("You have reached the end. Would you like to create a new node?")) {
	      branch *br = usr_input_branch();
	      brSetBranch2(current,br);
	      current = br;
	  }
	  else
	      current = root;
	}
	brPrint(current);
	break;
      case '1':
        if (brHasBranch1(current))
          current = brGetBranch1((current));
	else {
	  if (yesno("You have reached the end. Would you like to create a new node?")) {
	      branch *br = usr_input_branch();
	      brSetBranch1(current,br);
	      current = br;
	  }
	  else
	      current = root;
	  
	}
      case 'p':
        brPrint(current);
	break;
      case 'c':
        printf("New in Branches v%i.%i.%i...\n",br_VERSION,br_REVISION,br_MINOR_REVISION);
        printf("%s",br_CHANGELOG);
        break;
      case 'o': 
        if (current == root) {
	  puts("Unable to reach previous node");
	  break;
	}
	current = brGetPrevious(current);
	brPrint(current);
	break;
      case 'r':
        current = root;
	brPrint(root);
	break;
      case 's':
         if (yesno("Do you really want to save your progress?")) {
          char dummy[3];
          fgets(dummy,3,stdin);
          char filename[256];
          printf("Enter filename: ");
          fgets(filename,256,stdin);
          if (filename[strlen(filename) - 1] == '\n')
            filename[strlen(filename) - 1] = '\0';
          printf("Saving progress to %s...\n",filename);
          FILE *output = fopen(filename,"w");
          fprintf(output,brToString(root));
          fclose(output);
          puts("Sucessfully saved file!");
        }
        
        break;
      case 'a':
#ifdef DEBUG
        puts("Entering read");
#endif 
        printf("");
        char dummy[3];
        fgets(dummy,3,stdin);
        char filename[256];
        printf("Enter filename: ");
        fgets(filename,256,stdin);
        if (filename[strlen(filename) - 1] == '\n')
          filename[strlen(filename) - 1] = '\0';
        if (check_file(filename)) {
          fprintf(stderr,"Read Error\n");
          break;
        }
#ifdef DEBUG
        puts("Entering getAllLines");
#endif
        char *result = getAllLinesFromFile(filename);

        root = brFromString(result);
        current = root;
        brPrint(root);
        //puts("Feature currently unavailible");
        break; 
      case '3':
        if (current == root)
	{
	  current = third_tree;
	  brPrint(current);
	  break;
	}
      default:
        fprintf(stderr,"Invalid command, please reenter\n");
	break;
    }
  } while (cont);
  brRelease(root);
  return 0;
}