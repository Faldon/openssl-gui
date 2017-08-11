/*
 * =============================================================================
 *
 *  Filename:  config.c
 *
 *  Description:  Reading and writing the configuration
 *
 *  Version:  1.0
 *  Created:  26.07.2017 09:03:12
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
#include<errno.h>
#include<openssl/opensslconf.h>
#include "config.h"

ca_config *config;

static char* normalize_path(const char *path) {
  char *fullpath = (char *)malloc(strlen(path));
  memset(fullpath, '\0', strlen(path));
  if(strrchr(path, '/')==NULL) {
    fullpath = (char *)realloc(fullpath, strlen(path)+sizeof(char));
    fullpath[0] = '/';
  }
  strcat(fullpath, path);
  if(strcmp(strrchr((const char *)fullpath, '/'), "/")!=0) {
    fullpath = (char *)realloc(fullpath, strlen(fullpath)+sizeof(char));
    strcat(fullpath, "/");
  }
  fullpath = (char *)realloc(fullpath,
      strlen((const char *)fullpath)+strlen(APPCONF));
  return fullpath;
}

static int write_to_file(FILE *fp, ca_config *conf) {
  if(fp==NULL) {
    return ERR_FWRTE;
  }
  int err = 0;
  err = (err | fprintf(fp, "%s = %s\n", "ca_root", conf->ca_root)) >> 31;
  err = (err | fprintf(fp, "%s = %s\n", "ssl_conf", conf->ssl_conf)) >> 31;
  err = (err | fprintf(fp, "%s = %s\n", "key_dir", conf->key_dir)) >> 31;
  err = (err | fprintf(fp, "%s = %s\n", "cert_dir", conf->cert_dir)) >> 31;
  err = (err | fprintf(fp, "%s = %s\n", "crl_dir", conf->crl_dir)) >> 31;
  err = (err | fprintf(fp, "%s = %s\n", "csr_dir", conf->csr_dir)) >> 31;
  return err;
}

int parse_conf(const char *path) {
  char *fullpath = normalize_path(path);
  strcat(fullpath, APPCONF);
  FILE *fp = fopen((const char *)fullpath, "r");
  if(fp==NULL) {
    if(errno==2) {
      int retv = write_def_conf((const char *)fullpath);
      free(fullpath);
      return retv;
    }
    free(fullpath);
    return errno;
  }
  config = (ca_config *)calloc(1, sizeof(ca_config));
  char line[4096];
  while(!feof(fp)) {
    int len;
    char *key = (char *)malloc(sizeof(char));
    char *value = (char *)malloc(sizeof(char));
    if(fgets(line, 4096, fp)!=NULL) {
      len = strcspn(line, " ");
      key = (char *)realloc(key, len*sizeof(char));
      key = strncpy(key, line, len);
      value = (char *)realloc(value, strlen(line)-3*sizeof(char));
      value = strcpy(value, &line[len+3]);
      if(strcmp(key, "ca_root")==0) {
        config->ca_root = (char *)malloc(strlen(value));
        strncpy(config->ca_root, value, strlen(value)-1);
      }
      if(strcmp(key, "ssl_conf")==0) {
        config->ssl_conf = (char *)malloc(strlen(value));
        strncpy(config->ssl_conf, value, strlen(value)-1);
      }
      if(strcmp(key, "key_dir")==0) {
        config->key_dir = (char *)malloc(strlen(value));
        strncpy(config->key_dir, value, strlen(value)-1);
      }
      if(strcmp(key, "cert_dir")==0) {
        config->cert_dir = (char *)malloc(strlen(value));
        strncpy(config->cert_dir, value, strlen(value)-1);
      }
      if(strcmp(key, "crl_dir")==0) {
        config->crl_dir = (char *)malloc(strlen(value));
        strncpy(config->crl_dir, value, strlen(value)-1);
      }
      if(strcmp(key, "csr_dir")==0) {
        config->csr_dir = (char *)malloc(strlen(value));
        strncpy(config->csr_dir, value, strlen(value)-1);
      }
      memset(line, '\0', 4096);
      memset(key, '\0', strlen(key));
      memset(value, '\0', strlen(value));
      free(key);
      free(value);
    }
  }
  return 0;
}

int write_conf(const char *path) {
  // Determine the default path to the config file
  char *confpath;
  if(strstr(path, APPCONF)==NULL) {
    confpath = normalize_path(path);
    strcat(confpath, APPCONF);
  } else {
    confpath = (char *)malloc(strlen(path));
    strcpy(confpath, path);
  }
  FILE *appcfg = fopen((const char *)confpath, "w");
  if(appcfg==NULL) {
    return ERR_DEFLT;
  }
  // Write config file
  int retv = write_to_file(appcfg, config);
  // Clean up
  fclose(appcfg);
  free(confpath);
  return retv;
}

int write_def_conf(const char *path) {
  config = (ca_config *)calloc(1, sizeof(ca_config));
  // Determine the default path to the config file
  char *confpath;
  if(strstr(path, APPCONF)==NULL) {
    confpath = normalize_path(path);
    strcat(confpath, APPCONF);
  } else {
    confpath = (char *)malloc(strlen(path));
    strcpy(confpath, path);
  }
  FILE *appcfg = fopen((const char *)confpath, "w");
  if(appcfg==NULL) {
    return ERR_DEFLT;
  }
  size_t pathlen = strlen((const char *)confpath)-strlen(APPCONF)-sizeof(char);
  // Set ca root path (Default: dir of app config)
  config->ca_root = (char *)malloc(pathlen);
  strncpy(config->ca_root, (const char *)confpath, pathlen);
  // Set path to openssl.cnf
  // 1. Search in ca root
  // 2. Test for env OPENSSL_CONF
  // 3. Leave empty
  char *sslpath = (char *)malloc(pathlen+sizeof(char)+strlen(SSLCONF));
  strncpy(sslpath, (const char *)confpath, pathlen);
  strcat(sslpath, "/");
  strcat(sslpath, SSLCONF);
  FILE *sslcfg = fopen((const char *)sslpath, "r");
  if(sslcfg!=NULL) {
    config->ssl_conf = (char *)malloc(strlen((const char*)sslcfg));
    strcpy(config->ssl_conf, (const char *)sslpath);
    fclose(sslcfg);
  } else if((sslpath = getenv("OPENSSL_CONF"))!=NULL) {
      config->ssl_conf = (char *)malloc(strlen((const char*)sslpath));
      strcpy(config->ssl_conf,(const char*)sslpath);
  } else {
      config->ssl_conf = "";
  }
  // Set default ca dirs
  config->key_dir = "private";
  config->cert_dir = "certs";
  config->crl_dir = "crl";
  config->csr_dir = "csr";
  // Write config file
  int retv = write_to_file(appcfg, config);
  // Clean up
  fclose(appcfg);
  free(sslpath);
  free(confpath);
  return retv;
}
