/*
 * This file was generated by orbit-idl - DO NOT EDIT!
 */

#include <string.h>
#define ORBIT2_STUBS_API
#include "Bonobo_Sample_Echo.h"

void
_ORBIT_skel_small_Bonobo_Sample_Echo_echo(POA_Bonobo_Sample_Echo * _o_servant,
					  gpointer _o_retval,
					  gpointer * _o_args,
					  CORBA_Context _o_ctx,
					  CORBA_Environment * _o_ev,
					  void (*_impl_echo)
					  (PortableServer_Servant _servant,
					   const CORBA_char * message,
					   CORBA_Environment * ev))
{
   _impl_echo(_o_servant, *(const CORBA_char * *) _o_args[0], _o_ev);
}
static ORBitSmallSkeleton
get_skel_small_Bonobo_Sample_Echo(POA_Bonobo_Sample_Echo * servant,
				  const char *opname, gpointer * m_data,
				  gpointer * impl)
{
   switch (opname[0]) {
     case 'e':
	if (strcmp((opname + 1), "cho"))
	   break;
	*impl = (gpointer) servant->vepv->Bonobo_Sample_Echo_epv->echo;
	*m_data =
	   (gpointer) & Bonobo_Sample_Echo__iinterface.methods._buffer[0];
	return (ORBitSmallSkeleton) _ORBIT_skel_small_Bonobo_Sample_Echo_echo;
	break;
     case 'q':
	if (strcmp((opname + 1), "ueryInterface"))
	   break;
	*impl = (gpointer) servant->vepv->Bonobo_Unknown_epv->queryInterface;
	*m_data = (gpointer) & Bonobo_Unknown__iinterface.methods._buffer[2];
	return (ORBitSmallSkeleton)
	   _ORBIT_skel_small_Bonobo_Unknown_queryInterface;
	break;
     case 'r':
	if (strcmp((opname + 1), "ef"))
	   break;
	*impl = (gpointer) servant->vepv->Bonobo_Unknown_epv->ref;
	*m_data = (gpointer) & Bonobo_Unknown__iinterface.methods._buffer[0];
	return (ORBitSmallSkeleton) _ORBIT_skel_small_Bonobo_Unknown_ref;
	break;
     case 'u':
	if (strcmp((opname + 1), "nref"))
	   break;
	*impl = (gpointer) servant->vepv->Bonobo_Unknown_epv->unref;
	*m_data = (gpointer) & Bonobo_Unknown__iinterface.methods._buffer[1];
	return (ORBitSmallSkeleton) _ORBIT_skel_small_Bonobo_Unknown_unref;
	break;
     default:
	break;
   }
   return NULL;
}

void
POA_Bonobo_Sample_Echo__init(PortableServer_Servant servant,
			     CORBA_Environment * env)
{
   static PortableServer_ClassInfo class_info =
      { NULL, (ORBit_small_impl_finder) & get_skel_small_Bonobo_Sample_Echo,
"IDL:Bonobo/Sample/Echo:1.0", &Bonobo_Sample_Echo__classid, NULL, &Bonobo_Sample_Echo__iinterface };
   POA_Bonobo_Sample_Echo__vepv *fakevepv = NULL;

   if (((PortableServer_ServantBase *) servant)->vepv[0]->finalize == 0) {
      ((PortableServer_ServantBase *) servant)->vepv[0]->finalize =
	 POA_Bonobo_Sample_Echo__fini;
   }
   PortableServer_ServantBase__init(((PortableServer_ServantBase *) servant),
				    env);
   POA_Bonobo_Unknown__init(servant, env);
   ORBit_classinfo_register(&class_info);
   ORBIT_SERVANT_SET_CLASSINFO(servant, &class_info);

   if (!class_info.vepvmap) {
      class_info.vepvmap =
	 g_new0(ORBit_VepvIdx, Bonobo_Sample_Echo__classid + 1);
      class_info.vepvmap[Bonobo_Unknown__classid] =
	 (((char *) &(fakevepv->Bonobo_Unknown_epv)) -
	  ((char *) (fakevepv))) / sizeof(GFunc);
      class_info.vepvmap[Bonobo_Sample_Echo__classid] =
	 (((char *) &(fakevepv->Bonobo_Sample_Echo_epv)) -
	  ((char *) (fakevepv))) / sizeof(GFunc);
   }
}

void
POA_Bonobo_Sample_Echo__fini(PortableServer_Servant servant,
			     CORBA_Environment * env)
{
   POA_Bonobo_Unknown__fini(servant, env);
   PortableServer_ServantBase__fini(servant, env);
}
