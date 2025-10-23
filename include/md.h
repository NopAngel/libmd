#ifndef _LIBMD_H
#define _LIBMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define MODULE_AUTHOR(_author) 
#define MODULE_DESCRIPTION(_desc) 
#define MODULE_LICENSE(_license) 
#define MODULE_VERSION(_version) 

struct md_module {
    char name[256];
    char source_file[256];
    char object_file[256];
    time_t last_mtime;
    char author[256];
    char description[256];
    char license[256];
    char version[256];
    char **deps;
    int dep_count;
};

struct md_ctx {
    struct md_module *modules;
    int count;
    int capacity;
};

struct md_ctx *md_init(void);
void md_free(struct md_ctx *ctx);
int md_module_add(struct md_ctx *ctx, const char *name, const char *source_file);
int md_module_compile(struct md_ctx *ctx, const char *name, const char *compiler);
int md_build_exec(struct md_ctx *ctx, const char *output_name, const char *compiler);
int md_module_need_recompile(struct md_ctx *ctx, const char *name);
void md_module_set_info(struct md_ctx *ctx, const char *name, const char *author, const char *desc, const char *license, const char *version);

#endif
