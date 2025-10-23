#include "../include/md.h"




struct md_ctx *md_init(void) {
    

    struct md_ctx *ctx = malloc(sizeof(struct md_ctx));
    
    if (!ctx) return NULL;

    ctx->capacity = 10;
    

    ctx->count = 0;
    
    ctx->modules = malloc(sizeof(struct md_module) * ctx->capacity);

    return ctx;
}

void md_free(struct md_ctx *ctx) {
    if (!ctx) return;


    for (int i = 0; i < ctx->count; i++) {
        for (int j = 0; j < ctx->modules[i].dep_count; j++) {
        

            free(ctx->modules[i].deps[j]);
        }
    
        free(ctx->modules[i].deps);
    }
    free(ctx->modules);
    free(ctx);
}

int md_module_add(struct md_ctx *ctx, const char *name, const char *source_file) {
    if (ctx->count >= ctx->capacity) {
        ctx->capacity *= 2;
        struct md_module *new_modules = realloc(ctx->modules, sizeof(struct md_module) * ctx->capacity);
        if (!new_modules) return 0;
        ctx->modules = new_modules;
    

    }

    struct md_module *mod = &ctx->modules[ctx->count];
    

    strncpy(mod->name, name, 255);

    strncpy(mod->source_file, source_file, 255);

    snprintf(mod->object_file, 255, "%s.o", name);

    struct stat st;
    if (stat(source_file, &st) == 0) {
        mod->last_mtime = st.st_mtime;
    }

    mod->deps = NULL;
    mod->dep_count = 0;

    strcpy(mod->author, "Unknown");
    strcpy(mod->description, "No description");
    strcpy(mod->license, "Proprietary");
    strcpy(mod->version, "1.0");

    ctx->count++;
    return 1;
}

void md_module_set_info(struct md_ctx *ctx, const char *name, const char *author, const char *desc, const char *license, const char *version) {
    for (int i = 0; i < ctx->count; i++) {
        if (strcmp(ctx->modules[i].name, name) == 0) {

            if (author) strncpy(ctx->modules[i].author, author, 255);
            
            if (desc) strncpy(ctx->modules[i].description, desc, 255);
            
            if (license) strncpy(ctx->modules[i].license, license, 255);
            
            if (version) strncpy(ctx->modules[i].version, version, 255);
            


            break;
        }
    }
}









int md_module_need_recompile(struct md_ctx *ctx, const char *name) {
    for (int i = 0; i < ctx->count; i++) {
        if (strcmp(ctx->modules[i].name, name) == 0) {
            struct stat st_src, st_obj;
            if (stat(ctx->modules[i].source_file, &st_src) != 0) return 1;




            if (stat(ctx->modules[i].object_file, &st_obj) != 0) return 1;

            return st_src.st_mtime > st_obj.st_mtime;
        }
    }
    return 0;
}



int md_module_compile(struct md_ctx *ctx, const char *name, const char *compiler) {
    for (int i = 0; i < ctx->count; i++) {
        if (strcmp(ctx->modules[i].name, name) == 0) {
            if (!md_module_need_recompile(ctx, name)) {
                printf("  %s: [SKIP] - no changes\n", name);
                return 1;
            }
            //printf("=============================================\n");            
            printf("  LIBMD - MODULE LOADED [SUCESS]  %s/%s\n", ctx->modules[i].object_file, name);
            printf("    MODULE: %s\n", name);
            printf("    AUTHOR: %s\n", ctx->modules[i].author);
            printf("    DESCR:  %s\n", ctx->modules[i].description);
            printf("    LICENSE: %s\n", ctx->modules[i].license);
            printf("    VERSION: %s\n", ctx->modules[i].version);
            //printf("\n=============================================\n");
            
            char cmd[1024];
            snprintf(cmd, 1023, "%s -c %s -o %s", 
                    compiler, ctx->modules[i].source_file, ctx->modules[i].object_file);
            
            return system(cmd) == 0;
        }
    }
    return 0;
}

int md_build_exec(struct md_ctx *ctx, const char *output_name, const char *compiler) {
    char cmd[2048];
    snprintf(cmd, 2047, "%s -o %s", compiler, output_name);

    for (int i = 0; i < ctx->count; i++) {
        strncat(cmd, " ", 2047 - strlen(cmd));
        strncat(cmd, ctx->modules[i].object_file, 2047 - strlen(cmd));
    }

    return system(cmd) == 0;
}
