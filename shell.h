#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* loophsh.c */
int loophsh(char **);

/* sfunction.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strconc(char *, char *);

/* ash.c */
int hshloop(info_t *, char **);
int find_builtin_t(info_t *);
void find_cmdpath(info_t *);
void fork_cmdpath(info_t *);

/* sfunction2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* pathlink.c */
int is_cmdFlag(info_t *, char *);
char *d_chars(char *, int, int);
char *get_path(info_t *, char *, char *);

/* sfunction3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* sfunction4.c */
char **strtwo(char *, char *);
char **strtwo2(char *, char);

/* rsfunction.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* mem_fun2.c */
int buffree(void **);

/* mem_fun */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* mfun.c */
int interact(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* mfun2.c */
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *conv_num(long int, int, int);
void rem_comm(char *);

/* b_emu.c */
int _mexit(info_t *);
int _mcd(info_t *);
int _mhelp(info_t *);

/* b_emu2.c */
int _mhistory(info_t *);
int _malias(info_t *);

/* getline.c module */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* ffunction.c */
char *g_h_file(info_t *info);
int w_history(info_t *info);
int r_history(info_t *info);
int b_history(info_t *info, char *buf, int linecount);
int renum_history(info_t *info);

/* info.c module */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* envir.c module */
char *_getenviro(info_t *, const char *);
int _myenviro(info_t *);
int pop_enviro_list(info_t *);
int _myunsetenviro(info_t *);
int _mysetenviro(info_t *);

/* envir2.c module */
char **get_environm(info_t *);
int _unsetenviro(info_t *, char *);
int _setenviro(info_t *, char *, char *);

/* lstr.c module */
list_t *add_n(list_t **, const char *, int);
list_t *add_nend(list_t **, const char *, int);
size_t printlstr(const list_t *);
int d_nindex(list_t **, unsigned int);
void f_list(list_t **);

/* lstr2.c module */
size_t llen(const list_t *);
char **ltostr(list_t *);
size_t p_list(const list_t *);
list_t *ns_with(list_t *, char *, char);
ssize_t gnindex(list_t *, list_t *);

/* ch.c */
int is_chain(info_t *, char *, size_t *);
void chk_chain(info_t *, char *, size_t *, size_t, size_t);
int rep_alias(info_t *);
int rep_vars(info_t *);
int rep_string(char **, char *);

#endif

