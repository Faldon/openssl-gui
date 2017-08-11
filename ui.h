/*
 * =============================================================================
 *
 *  Filename:  ui.h
 *
 *  Description: Header guard for config.h
 *
 *  Version:  1.0
 *  Created:  28.07.2017 07:36:09
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author:  Thomas Pulzer (thopu), t.pulzer@kniel.de
 *  Company:  Kniel System-Electronic GmbH
 *
 * =============================================================================
 * vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
 */

#ifndef UI_H
#define UI_H
#include "structdef.h"

static const int UI_MENU_ITEMS = 6;
int insert;
int navigate;

void clear_page(WINDOW *w);
void draw_cfg_page(WINDOW *w);
void draw_menu(WINDOW *w, int item_highlight);
void draw_status(WINDOW *w, const char *message);
void handle_insert(WINDOW *w, int key);
void handle_key(WINDOW *w, int key);
void navigate_lr(WINDOW *w, int page);
#endif
