#include <ncurses.h>

int main() {

    // initialise ncurses
    initscr();

    // if null
    if (initscr() == NULL) {
        fprintf(stderr, "Error initializing ncurses.\n");
        return 1;
    }

    // print text to window 
    printw("Hello, ncurses!\n");  // Display text

    // call refresh to display changes
    refresh(); 

    // Wait for user input by blocking the program until a key is pressed
    int ch = getch();

    // Display the key code
    printw("\nYou pressed: %c\n", ch);

    getch();
    
    // ends and clean up 
    endwin();

    return 0;
}