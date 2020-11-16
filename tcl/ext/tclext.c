#define USE_TCL_STUBS
#include <tcl.h>
#define USE_TCLOO_STUBS
#include <tclOO.h>

#include "tclext.h"

#define METHOD_ARGS ClientData clientData ,Tcl_Interp *interp ,Tcl_ObjectContext objectContext ,int objc ,Tcl_Obj *const *objv

int fly (METHOD_ARGS) {
    Tcl_Eval(interp ,"puts {I'm Flying!}");
}

const static Tcl_MethodType FlyMethodType = {
    TCL_OO_METHOD_VERSION_CURRENT
    ,"fly"
    ,fly
    ,NULL
    ,NULL
};

/* This function extends Tcl */
int example_main(Tcl_Interp *interp) {
    if (Tcl_InitStubs(interp ,"8.6" ,0) == NULL) {
        return TCL_ERROR;
    }
    if (Tcl_OOInitStubs(interp) == NULL) {
        return TCL_ERROR;
    }
    Tcl_Channel chan = Tcl_GetStdChannel(TCL_STDOUT);
    Tcl_Obj *name = Tcl_NewStringObj("::oo::class" ,-1);
    Tcl_Object classobj = Tcl_GetObjectFromObj(interp ,name);
    Tcl_Class class = Tcl_GetObjectAsClass(classobj);
    Tcl_Obj *objectname = Tcl_NewStringObj("fly" ,-1);
    Tcl_Object o = Tcl_NewObjectInstance(
        interp ,class ,"bigbird" ,NULL ,0 ,0 ,0);
    Tcl_NewInstanceMethod(interp ,o ,objectname ,1 ,&FlyMethodType ,NULL);
    Tcl_Eval(interp ,"bigbird fly");
    return TCL_OK;
}
