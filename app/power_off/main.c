#include <stdio.h>
#include <periph/pm.h>
#include <shell.h>


int turn_off(int argc, char **argv){
    (void) argc;
    (void) argv;
    pm_off();
    return 0;
}
SHELL_COMMAND(shutdown, "shutdown the system", turn_off);

int main(void)
{
    puts("Hello World!");
    puts(__TIME__);
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    puts("turning off failed");
    
    return 0;
}