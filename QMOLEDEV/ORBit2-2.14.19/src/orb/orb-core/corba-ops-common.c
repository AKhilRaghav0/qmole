/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#include <string.h>
#define ORBIT2_STUBS_API
#define ORBIT_IDL_C_COMMON
#define corba_ops_COMMON
#include "corba-ops.h"

static const CORBA_unsigned_long ORBit_zero_int = 0;

#ifndef ORBIT_IDL_C_IMODULE_corba_ops
void _ORBIT_skel_small_ORBit_ObjectImpl__is_a(POA_ORBit_ObjectImpl             *_o_servant, gpointer            _o_retval,gpointer           *_o_args,CORBA_Context       _o_ctx,CORBA_Environment  *_o_ev,
CORBA_boolean (*_impl__is_a)(PortableServer_Servant _servant, const CORBA_char * logical_type_id, CORBA_Environment *ev)) {
*(CORBA_boolean *)_o_retval = _impl__is_a (_o_servant, *(const CORBA_char * *)_o_args[0], _o_ev);
}

#endif
#if ( (TC_IMPL_TC_ORBit_ObjectImpl_0 == 'c') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_1 == 'o') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_2 == 'r') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_3 == 'b') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_4 == 'a') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_5 == '_') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_6 == 'o') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_7 == 'p') \
&& (TC_IMPL_TC_ORBit_ObjectImpl_8 == 's') \
) && !defined(TC_DEF_TC_ORBit_ObjectImpl)
#define TC_DEF_TC_ORBit_ObjectImpl 1
#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static
#endif
ORBIT2_MAYBE_CONST struct CORBA_TypeCode_struct TC_ORBit_ObjectImpl_struct = {
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
(char *)"ObjectImpl",
(char *)"IDL:ORBit/ObjectImpl:1.0",
NULL,
NULL,
-1,
0,
0, 0
};
#endif

#ifndef ORBIT_IDL_C_IMODULE_corba_ops
CORBA_unsigned_long ORBit_ObjectImpl__classid = 0;
#endif

/* Interface type data */

static ORBit_IArg ORBit_ObjectImpl__is_a__arginfo [] = {
	{ TC_CORBA_string,  ORBit_I_ARG_IN , (char *)"logical_type_id" }
};

#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static
#endif
ORBit_IMethod ORBit_ObjectImpl__imethods [] = {
	{
		{ 1, 1, ORBit_ObjectImpl__is_a__arginfo, FALSE },
		{ 0, 0, NULL, FALSE },
		{ 0, 0, NULL, FALSE },
		TC_CORBA_boolean, (char *)"_is_a", 7,
		0| ORBit_I_COMMON_FIXED_SIZE
}
};

static CORBA_string ORBit_ObjectImpl__base_itypes[] = {
(char *)"IDL:omg.org/CORBA/Object:1.0"
};
#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static
#endif
ORBit_IInterface ORBit_ObjectImpl__iinterface = {
TC_ORBit_ObjectImpl,{1, 1, ORBit_ObjectImpl__imethods, FALSE},
{1, 1, ORBit_ObjectImpl__base_itypes, FALSE}
};

