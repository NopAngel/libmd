#include "../include/md.h"
#include <stdio.h>

int main(void) {
    struct md_ctx *ctx = md_init();
    
    md_module_add(ctx, "math", "math.c");
    md_module_set_info(ctx, "math", 
                      "MAIL@MAIL.COM",
                      "Mathematics operations module", 
                      "GPL v2",
                      "2.6.0");
    
    md_module_add(ctx, "main", "main.c");
    md_module_set_info(ctx, "main",
                      "MAIN FNC main@main.com",
                      "Main application module",
                      "GPL",
                      "1.0");

    md_module_compile(ctx, "math", "gcc");
    md_module_compile(ctx, "main", "gcc");
   // BUILD APP 
    md_build_exec(ctx, "math_app", "gcc");
    
    md_free(ctx);
    return 0;
}
