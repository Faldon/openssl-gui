/*
 * =============================================================================
 *
 *  Filename:  main.c
 *
 *  Description:  GUI for openssl
 *
 *  Version:  1.0
 *  Created:  25.07.2017 13:44:47
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author:  Thomas Pulzer (thopu), t.pulzer@kniel.de
 *  Company:  Kniel System-Electronic GmbH
 *
 * =============================================================================
 * vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ncurses.h>
#include "config.h"
#include "ui.h"

int main() {
  homedir = getenv("HOME");
  int retv = parse_conf((const char *)homedir);
  if(retv!=0) {
    return retv;
  }
  WINDOW *w;
  int ch, i = 0, width = 10;
  insert = 0;
  navigate = 1;

  initscr(); // initialize Ncurses
  w = newwin( 24, 80, 1, 1 ); // create a new window
  box( w, 0, 0 ); // sets default borders for the window

  draw_menu(w, -1);

  wrefresh( w ); // update the terminal screen

  i = 0;
  noecho(); // disable echoing of characters on the screen
  keypad( w, TRUE ); // enable keyboard input for the window.
  curs_set( 0 ); // hide the default screen cursor.
   // get the input
  do {
    while((ch = wgetch(w)) != 'q' ) {
      handle_key(w, ch);
    }
  } while (insert > 0 );
  delwin( w );
  endwin();
  return 0;
}
