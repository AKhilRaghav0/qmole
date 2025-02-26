/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#ifndef test1_H
#define test1_H 1
#include <glib.h>
#define ORBIT_IDL_SERIAL 20
#include <orbit/orbit-types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** typedefs **/
#if !defined(ORBIT_DECL_Test) && !defined(_Test_defined)
#define ORBIT_DECL_Test 1
#define _Test_defined 1
#define Test__freekids CORBA_Object__freekids
typedef CORBA_Object Test;
extern CORBA_unsigned_long Test__classid;
#if !defined(TC_IMPL_TC_Test_0)
#define TC_IMPL_TC_Test_0 't'
#define TC_IMPL_TC_Test_1 'e'
#define TC_IMPL_TC_Test_2 's'
#define TC_IMPL_TC_Test_3 't'
#define TC_IMPL_TC_Test_4 '1'
#ifdef ORBIT_IDL_C_IMODULE_test1
static
#else
extern
#endif
ORBIT2_MAYBE_CONST struct CORBA_TypeCode_struct TC_Test_struct;
#define TC_Test ((CORBA_TypeCode)&TC_Test_struct)
#endif
#endif

/** POA structures **/
#ifndef _defined_POA_Test
#define _defined_POA_Test 1
typedef struct {
  void *_private;
CORBA_string (*op)(PortableServer_Servant _servant, const CORBA_char * astr, CORBA_Environment *ev);
} POA_Test__epv;
typedef struct {
  PortableServer_ServantBase__epv *_base_epv;
  POA_Test__epv *Test_epv;
} POA_Test__vepv;
typedef struct {
  void *_private;
  POA_Test__vepv *vepv;
} POA_Test;
extern void POA_Test__init(PortableServer_Servant servant, CORBA_Environment *ev);
extern void POA_Test__fini(PortableServer_Servant servant, CORBA_Environment *ev);
#endif /* _defined_POA_Test */

/** skel prototypes **/
void _ORBIT_skel_small_Test_op(POA_Test *_ORBIT_servant, gpointer _ORBIT_retval, gpointer *_ORBIT_args, CORBA_Context ctx,CORBA_Environment *ev, CORBA_string (*_impl_op)(PortableServer_Servant _servant, const CORBA_char * astr, CORBA_Environment *ev));

/** stub prototypes **/
CORBA_string Test_op(Test _obj, const CORBA_char * astr, CORBA_Environment *ev);
#include <orbit/orb-core/orbit-interface.h>

#ifdef ORBIT_IDL_C_IMODULE_test1
static 
#else
extern 
#endif
ORBit_IInterface Test__iinterface;
#define Test_IMETHODS_LEN 1
#ifdef ORBIT_IDL_C_IMODULE_test1
static 
#else
extern 
#endif
ORBit_IMethod Test__imethods[Test_IMETHODS_LEN];

/** IMethods index */

#ifndef __Test__imethods_index
#define __Test__imethods_index
typedef enum {
	Test_op__imethods_index
} Test__imethods_index;
#endif /* __Test__imethods_index */

#ifndef __ORBIT_IMETHODS_INDEX
#define __ORBIT_IMETHODS_INDEX
#define ORBIT_IMETHODS_INDEX(m) (m ## __imethods_index)
#endif /* __ORBIT_IMETHODS_INDEX */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifndef EXCLUDE_ORBIT_H
#include <orbit/orbit.h>

#endif /* EXCLUDE_ORBIT_H */
#endif
#undef ORBIT_IDL_SERIAL
