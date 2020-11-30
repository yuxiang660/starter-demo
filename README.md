# starter-demo
starter program for several topics

# Combining C and tcl/tk
## Initialization and Registering Commands
* "Tk_Main" function
   * hand over control of the program to the tcl/tk interpreter

```c
// prototype for the initialization function
int InitProc( Tcl_Interp *interp );

// declare an array for two strings
char *ppszArg[2];

// allocate strings and set their contents
ppszArg[0] = (char *)malloc( sizeof( char ) * 12 );
ppszArg[1] = (char *)malloc( sizeof( char ) * 12 );
strcpy( ppszArg[0], "Hello World" );
strcpy( ppszArg[1], "./hello.tcl" );

// the following call does not return
Tk_Main( 2, ppszArg, InitProc );
```

* Initialization Functions
   * "Tk_Main" hands over control of your program to the tcl/tk interpreter, but after some base initialization is done and before the tcl/tk script is run **a user defined function can be executed**, such as `InitProc` above.
   * The user defined initialization function must return an integer and takes one parameter `Tcl_INTERP *`.

## Useful Links
* [Combining C and tcl/tk](http://davesource.com/Fringe/Fringe/Computers/Languages/tcl_tk/tcl_C.html)
