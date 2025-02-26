/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#include <string.h>
#define ORBIT2_STUBS_API
#define ORBIT_IDL_C_COMMON
#define Bonobo_Unknown_COMMON
#include "Bonobo_Unknown.h"

static const CORBA_unsigned_long ORBit_zero_int = 0;

#ifndef ORBIT_IDL_C_IMODULE_Bonobo_Unknown
void _ORBIT_skel_small_Bonobo_Unknown_ref(POA_Bonobo_Unknown             *_o_servant, gpointer            _o_retval,gpointer           *_o_args,CORBA_Context       _o_ctx,CORBA_Environment  *_o_ev,
void (*_impl_ref)(PortableServer_Servant _servant, CORBA_Environment *ev)) {
_impl_ref (_o_servant, _o_ev);
}
void _ORBIT_skel_small_Bonobo_Unknown_unref(POA_Bonobo_Unknown             *_o_servant, gpointer            _o_retval,gpointer           *_o_args,CORBA_Context       _o_ctx,CORBA_Environment  *_o_ev,
void (*_impl_unref)(PortableServer_Servant _servant, CORBA_Environment *ev)) {
_impl_unref (_o_servant, _o_ev);
}
void _ORBIT_skel_small_Bonobo_Unknown_queryInterface(POA_Bonobo_Unknown             *_o_servant, gpointer            _o_retval,gpointer           *_o_args,CORBA_Context       _o_ctx,CORBA_Environment  *_o_ev,
Bonobo_Unknown (*_impl_queryInterface)(PortableServer_Servant _servant, const CORBA_char * repoid, CORBA_Environment *ev)) {
*(Bonobo_Unknown *)_o_retval = _impl_queryInterface (_o_servant, *(const CORBA_char * *)_o_args[0], _o_ev);
}

#endif
#if ( (TC_IMPL_TC_Bonobo_Unknown_0 == 'B') \
&& (TC_IMPL_TC_Bonobo_Unknown_1 == 'o') \
&& (TC_IMPL_TC_Bonobo_Unknown_2 == 'n') \
&& (TC_IMPL_TC_Bonobo_Unknown_3 == 'o') \
&& (TC_IMPL_TC_Bonobo_Unknown_4 == 'b') \
&& (TC_IMPL_TC_Bonobo_Unknown_5 == 'o') \
&& (TC_IMPL_TC_Bonobo_Unknown_6 == '_') \
&& (TC_IMPL_TC_Bonobo_Unknown_7 == 'U') \
&& (TC_IMPL_TC_Bonobo_Unknown_8 == 'n') \
&& (TC_IMPL_TC_Bonobo_Unknown_9 == 'k') \
&& (TC_IMPL_TC_Bonobo_Unknown_10 == 'n') \
&& (TC_IMPL_TC_Bonobo_Unknown_11 == 'o') \
&& (TC_IMPL_TC_Bonobo_Unknown_12 == 'w') \
&& (TC_IMPL_TC_Bonobo_Unknown_13 == 'n') \
) && !defined(TC_DEF_TC_Bonobo_Unknown)
#define TC_DEF_TC_Bonobo_Unknown 1
#ifdef ORBIT_IDL_C_IMODULE_Bonobo_Unknown
static
#endif
ORBIT2_MAYBE_CONST struct CORBA_TypeCode_struct TC_Bonobo_Unknown_struct = {
{&ORBit_TypeCode_epv, ORBIT_REFCOUNT_STATIC},
CORBA_tk_objref,
0,
0,
ORBIT_ALIGNOF_CORBA_POINTER,
0,
0
,
NULL,
CORBA_OBJECT_NIL,
(char *)"Unknown",
(char *)"IDL:Bonobo/Unknown:1.0",
NULL,
NULL,
-1,
0,
0, 0
};
#endif

#ifndef ORBIT_IDL_C_IMODULE_Bonobo_Unknown
CORBA_unsigned_long Bonobo_Unknown__classid = 0;
#endif

/* Interface type data */

static ORBit_IArg Bonobo_Unknown_queryInterface__arginfo [] = {
	{ TC_CORBA_string,  ORBit_I_ARG_IN , (char *)"repoid" }
};

#ifdef ORBIT_IDL_C_IMODULE_Bonobo_Unknown
static
#endif
ORBit_IMethod Bonobo_Unknown__imethods [] = {
	{
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
TC_void, (char *)"ref", 3,
		0
}
, 	{
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
TC_void, (char *)"unref", 5,
		0
}
, 	{
		{ 1, 1, Bonobo_Unknown_queryInterface__arginfo, FALSE },
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
		TC_Bonobo_Unknown, (char *)"queryInterface", 14,
		0
}
};

static CORBA_string Bonobo_Unknown__base_itypes[] = {
(char *)"IDL:omg.org/CORBA/Object:1.0"
};
#ifdef ORBIT_IDL_C_IMODULE_Bonobo_Unknown
static
#endif
ORBit_IInterface Bonobo_Unknown__iinterface = {
TC_Bonobo_Unknown,{3, 3, Bonobo_Unknown__imethods, FALSE},
{1, 1, Bonobo_Unknown__base_itypes, FALSE}
};

