/*
 * =============================================================================
 *
 *  Filename:  ui.c
 *
 *  Description:  User interface for openssl-gui
 *
 *  Version:  1.0
 *  Created:  28.07.2017 07:32:31
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author:  Thomas Pulzer (thopu), t.pulzer@kniel.de
 *  Company:  Kniel System-Electronic GmbH
 *
 * =============================================================================
 * vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
 */

#include<ncurses.h>
#include<stdlib.h>
#include<string.h>
#include "ui.h"
#include "config.h"

const char menu[6][13] = {
    "CA", "Keys", "Requests", "Certs", "Revokes", "Config"
};
char item[13];
char accel;
char *input;
char *buffer;
int actv_page;
int crs_pos_y = 0;
int crs_pos_x = 0;

void clear_page(WINDOW *w) {
  wmove(w, 3, 1);
  wclrtobot(w);
  box(w, 0, 0);
  wmove(w, crs_pos_y, crs_pos_x);
}

void draw_cfg_page(WINDOW *w) {
  mvwprintw(w, 4, 1, "%20s", "CA Root Dir:");
  mvwprintw(w, 6, 1, "%20s", "SSL Config:");
  mvwprintw(w, 8, 1, "%20s", "Private Keys Dir:");
  mvwprintw(w, 10, 1, "%20s", "Certificates Dir:");
  mvwprintw(w, 12, 1, "%20s", "Signing Reqs Dir:");
  mvwprintw(w, 14, 1, "%20s", "Revocations Dir:");
  wattron( w, A_UNDERLINE);
  mvwprintw(w, 4, 25, "%s", config->ca_root);
  mvwprintw(w, 6, 25, "%s", config->ssl_conf);
  mvwprintw(w, 8, 25, "%s", config->key_dir);
  mvwprintw(w, 10, 25, "%s", config->cert_dir);
  mvwprintw(w, 12, 25, "%s", config->csr_dir);
  mvwprintw(w, 14, 25, "%s", config->crl_dir);
  wattroff( w, A_UNDERLINE);
  wmove(w, crs_pos_y, crs_pos_x);
}

void draw_menu(WINDOW *w, int item_highlight) {
  actv_page = item_highlight;
  for( int i=0; i<UI_MENU_ITEMS; i++ ) {
    if( i == item_highlight ) {
      wattron( w, A_STANDOUT );
    } else {
      wattroff( w, A_STANDOUT );
    }
    switch(i) {
      case 0:
        accel='A';
        break;
      case 1:
        accel='K';
        break;
      case 2:
        accel='R';
        break;
      case 3:
        accel='C';
        break;
      case 4:
        accel='v';
        break;
      case 5:
        accel='o';
        break;
      default:
        accel=' ';
    }
    sprintf(item, "%-8s (%c)",  menu[i], accel);
    mvwprintw( w, 1, 1+i*13, "%s", item );
    wattroff( w, A_STANDOUT );
    wmove(w, crs_pos_y, crs_pos_x);
  }
}

void draw_status(WINDOW *w, const char *message) {
  wmove(w, 22, 1);
  wclrtoeol(w);
  box(w, 0, 0);
  mvwprintw(w, 22, 1, "%s", message);
  wmove(w, crs_pos_y, crs_pos_x);
}

void handle_insert(WINDOW *w, int key) {
  switch( key ) {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_F0:
      break;
    case KEY_ENTER:
    case 0x0A:
      insert = 1-insert;
      if(actv_page == 5) {
        switch(crs_pos_y) {
          case 4:
            strcpy(config->ca_root, input);
            break;
          case 6:
            strcpy(config->ssl_conf, input);
            break;
          case 8:
            strcpy(config->key_dir, input);
            break;
          case 10:
            strcpy(config->cert_dir, input);
            break;
          case 12:
            strcpy(config->csr_dir, input);
            break;
          case 14:
            strcpy(config->crl_dir, input);
            break;
        }
        int r = write_conf(homedir);
        printf("%x\n", r);
        free(input);
        getyx(w, crs_pos_y, crs_pos_x);
      }
      break;
    case 0x1B:
      insert = 1-insert;
      memset(input, '\0', strlen(input));
      free(input);
      if(actv_page == 5) {
        mvwinsstr(w, crs_pos_y, 25, buffer);
        free(buffer);
        box(w, 0, 0);
        getyx(w, crs_pos_y, crs_pos_x);
      }
      break;
    case KEY_BACKSPACE:
    case 0x7F:
      mvwaddch(w, crs_pos_y, crs_pos_x-1, ' ');
      wmove(w, crs_pos_y, crs_pos_x-1);
      input[strlen(input)-1] = '\0';
      input = (char *)realloc(input, strlen(input));
      getyx(w, crs_pos_y, crs_pos_x);
      break;
    case KEY_DC:
      mvwdelch(w, crs_pos_y, 79);
      wmove(w, crs_pos_y, crs_pos_x);
      wdelch(w);
      box(w, 0, 0);
      break;
    default:
      strcat(input, ((char *)&key));
      input = (char *)realloc(input, strlen(input)+sizeof(char));
      mvwaddch(w, crs_pos_y, crs_pos_x, key);
      getyx(w, crs_pos_y, crs_pos_x);
      break;
  }
}

void handle_key(WINDOW *w, int key) {
  if (insert) {
    handle_insert(w, key);
  } else {
    switch( key ) {
      case KEY_F(1):
      case 'A':
        navigate_lr(w, 0);
        break;
      case KEY_F(2):
      case 'K':
        navigate_lr(w, 1);
        break;
      case KEY_F(3):
      case 'R':
        navigate_lr(w, 2);
        break;
      case KEY_F(4):
      case 'C':
        navigate_lr(w, 3);
        break;
      case KEY_F(5):
      case 'v':
        navigate_lr(w, 4);
        break;
      case KEY_F(6):
      case 'o':
        navigate_lr(w, 5);
        break;
      case KEY_DOWN:
        if(actv_page == 5) {
          navigate = 0;
          curs_set(2);
          if(crs_pos_y == 0) {
            draw_status(w, "Press <ENTER> for insert mode. Save with <ENTER>, discard changes with <ESC>");
            crs_pos_y = 4;
            crs_pos_x = 25;
          } else if (crs_pos_y < 14) {
            crs_pos_y = crs_pos_y + 2;
            crs_pos_x = 25;
          }
          draw_cfg_page(w);
        }
        break;
      case KEY_UP:
        if(actv_page == 5) {
          if(crs_pos_y == 4) {
            navigate = 1;
            curs_set(0);
            crs_pos_y = 0;
            crs_pos_x = 0;
          } else if (crs_pos_y > 4) {
            crs_pos_y = crs_pos_y - 2;
            crs_pos_x = 25;
          }
          draw_cfg_page(w);
        }
        break;
      case KEY_ENTER:
      case 0x0A:
        if(actv_page == 5 && crs_pos_y > 1 && crs_pos_x > 0) {
          insert = 1-insert;
          if(insert) {
            input = (char *)malloc(sizeof(char));
            buffer = (char *)malloc((80-crs_pos_x)*sizeof(char));
            winstr(w, buffer);
          } else {
            noecho();
          }
        }
        break;
      case 0x1B:
        if(insert) {
          insert = 0;
          memset(input, '\0', strlen(input));
          free(input);
        }
        break;
      default:
        break;
    }
  }
}

void navigate_lr(WINDOW *w, int page) {
  if(navigate) {
    draw_menu(w, page);
    clear_page(w);
    switch(actv_page) {
      case 5:
        draw_cfg_page(w);
        break;
      default:
        break;
    }
  }
}
