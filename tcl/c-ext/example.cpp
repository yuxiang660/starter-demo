#include "tcl.h"

#include <iostream>
#include <cstring>

class MyObjectData
{
   int id;

public:
   MyObjectData(int iid) : id(iid) {}
   void ping() { std::cout << "pong from " << id << std::endl; }
};

extern "C" int example_obj_command(ClientData clientData,
                                   Tcl_Interp *interp,
                                   int objc,
                                   Tcl_Obj *const objv[])
{
   if (objc < 2)
   {
      Tcl_WrongNumArgs(interp, 1, objv, "method ?argument ...?");
      return TCL_ERROR;
   }

   static const char *methods[] = {"ping", "destroy", NULL};
   enum ExObjMethods
   {
      EXOBJ_PING,
      EXOBJ_DESTROY
   };

   int index = -1;

   if (Tcl_GetIndexFromObj(interp, objv[1], methods, "method", 0,
                           &index) != TCL_OK)
      return TCL_ERROR;

   MyObjectData *p = (MyObjectData *)clientData;

   switch ((ExObjMethods)index)
   {
   case EXOBJ_PING:
      p->ping();
      break;
   case EXOBJ_DESTROY:
      Tcl_DeleteCommand(interp, Tcl_GetString(objv[0]));
      delete p;
      break;
   }

   return TCL_OK;
}

extern "C" int example_create(ClientData clientData,
                              Tcl_Interp *interp,
                              int objc,
                              Tcl_Obj *const objv[])
{
   static int obj_count = 0;

   MyObjectData *p = new MyObjectData(obj_count);

   char obj_name[13 + TCL_INTEGER_SPACE];
   sprintf(obj_name, "::exampleobj%d", obj_count++);

   Tcl_CreateObjCommand(interp, obj_name,
                        (Tcl_ObjCmdProc *)example_obj_command,
                        (ClientData)p, (Tcl_CmdDeleteProc *)NULL);

   Tcl_SetObjResult(interp, Tcl_NewStringObj(obj_name, strlen(obj_name)));

   return TCL_OK;
}

extern "C" DLLEXPORT int Example_Init(Tcl_Interp *interp)
{
   if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL)
   {
      return TCL_ERROR;
   }
   if (Tcl_PkgRequire(interp, "Tcl", TCL_VERSION, 0) == NULL)
   {
      return TCL_ERROR;
   }
   if (Tcl_PkgProvide(interp, "example", "0.1") != TCL_OK)
   {
      return TCL_ERROR;
   }

   Tcl_CreateObjCommand(interp, "example::create",
                        (Tcl_ObjCmdProc *)example_create,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

   return TCL_OK;
}
