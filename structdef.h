/*
 * =============================================================================
 *
 *  Filename:  structdef.h
 *
 *  Description:  Struct definitions
 *
 *  Version:  1.0
 *  Created:  02.08.2017 07:52:22
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author:  Thomas Pulzer (thopu), t.pulzer@kniel.de
 *  Company:  Kniel System-Electronic GmbH
 *
 * =============================================================================
 * vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
 */

#ifndef OPENSSLGUI_STRUCTDEF_H
#define OPENSSLGUI_STRUCTDEF_H
typedef struct {
  char *ssl_conf;
  char *ca_root;
  char *cert_dir;
  char *key_dir;
  char *csr_dir;
  char *crl_dir;
} ca_config;
#endif
