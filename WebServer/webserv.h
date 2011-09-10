#ifndef WEBSERV_H_
#define WEBSERV_H_

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<string.h>
#include 	<stdlib.h>
#include 	<unistd.h>

#include	"socklib.h"

void read_til_crnl(FILE *);
void process_rq(char *, int);
void header(FILE *, char *);
void cannot_do(int);
void do_404(char *, int);
int isadir(char *);
int not_exist(char *);
void do_ls(char *, int);
char * file_type(char *f);
int ends_in_cgi(char *f);
void do_exec(char *prog, int fd);
void do_cat(char *f, int fd);

#endif /*WEBSERV_H_*/
