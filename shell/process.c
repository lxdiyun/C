/* process.c
 * command processing layer
 *
 * The process(char **list) function is called by the main loop
 * It sits in front of execute() function. This layer handles
 * two main classes of processing:
 * a) built-in functions(e.g. exit(),set, =, read,..)
 * b) control strutures(e.g. if,while,for)
 */
#include <stdio.h>
#include "smsh.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();

/*
 * purpose: process user command
 * 
 * returns: result of processing command
 * 
 * details: if a built-in then call appropriate function,if not
 * 	execute()
 * 
 * error: arise from subrouting, handled there
 */
int process(char **args) {
	int rv = 0;
	
	if(args[0] == NULL)
		rv = 0;
	else if(is_control_command(args[0]))
		rv = do_control_command(args);
	else if(ok_to_execute())
		rv = execute(args);

	return rv;
}
