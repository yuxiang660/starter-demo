#include <stdlib.h>
#include <tcl.h>
#include "tclext.h"


Tcl_Interp *interp;
/* This function embeds Tcl */
int main (int argc ,char * argv[]) {
    Tcl_FindExecutable(argv[0]);
    interp = Tcl_CreateInterp();
    if (Tcl_Init(interp) != TCL_OK) {
        fprintf (
            stderr ,"Tcl_Init error: %s\n" ,Tcl_GetStringResult (interp));
        exit(EXIT_FAILURE);
    }
    if (example_main(interp) != TCL_OK) {
        fprintf (
            stderr ,"example_main error: %s\n" ,Tcl_GetStringResult (interp));
        exit(EXIT_FAILURE);
    }
    Tcl_Finalize();
    exit(EXIT_SUCCESS);
}
