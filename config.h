/*
 * =============================================================================
 *
 *  Filename:  config.h
 *
 *  Description:  Header guard for config.c
 *
 *  Version:  1.0
 *  Created:  26.07.2017 08:33:40
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author:  Thomas Pulzer (thopu), t.pulzer@kniel.de
 *  Company:  Kniel System-Electronic GmbH
 *
 * =============================================================================
 * vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
 */

#ifndef CONFIG_H
#define CONFIG_H
#include "structdef.h"

enum {
  ERR_FOPEN = 0x10000001,
  ERR_FWRTE = 0x10000002,
  ERR_DEFLT = 0x10000004
};

static const char* APPCONF = ".openssl-gui.conf";
static const char* SSLCONF = "openssl.cnf";
ca_config *config;
static char* normalize_path(const char *path);
static int write_to_file(FILE *fp, ca_config *conf);
char *homedir;
int parse_conf(const char *path);
int write_conf(const char *path);
int write_def_conf(const char *path);
#endif
