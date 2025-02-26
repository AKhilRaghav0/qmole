/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#ifndef corba_ops_H
#define corba_ops_H 1
#include <glib.h>
#define ORBIT_IDL_SERIAL 20
#include <orbit/orbit-types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** typedefs **/
#if !defined(ORBIT_DECL_ORBit_ObjectImpl) && !defined(_ORBit_ObjectImpl_defined)
#define ORBIT_DECL_ORBit_ObjectImpl 1
#define _ORBit_ObjectImpl_defined 1
#define ORBit_ObjectImpl__freekids CORBA_Object__freekids
typedef CORBA_Object ORBit_ObjectImpl;
extern CORBA_unsigned_long ORBit_ObjectImpl__classid;
#if !defined(TC_IMPL_TC_ORBit_ObjectImpl_0)
#define TC_IMPL_TC_ORBit_ObjectImpl_0 'c'
#define TC_IMPL_TC_ORBit_ObjectImpl_1 'o'
#define TC_IMPL_TC_ORBit_ObjectImpl_2 'r'
#define TC_IMPL_TC_ORBit_ObjectImpl_3 'b'
#define TC_IMPL_TC_ORBit_ObjectImpl_4 'a'
#define TC_IMPL_TC_ORBit_ObjectImpl_5 '_'
#define TC_IMPL_TC_ORBit_ObjectImpl_6 'o'
#define TC_IMPL_TC_ORBit_ObjectImpl_7 'p'
#define TC_IMPL_TC_ORBit_ObjectImpl_8 's'
#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static
#else
extern
#endif
ORBIT2_MAYBE_CONST struct CORBA_TypeCode_struct TC_ORBit_ObjectImpl_struct;
#define TC_ORBit_ObjectImpl ((CORBA_TypeCode)&TC_ORBit_ObjectImpl_struct)
#endif
#endif

/** POA structures **/
#ifndef _defined_POA_ORBit_ObjectImpl
#define _defined_POA_ORBit_ObjectImpl 1
typedef struct {
  void *_private;
CORBA_boolean (*_is_a)(PortableServer_Servant _servant, const CORBA_char * logical_type_id, CORBA_Environment *ev);
} POA_ORBit_ObjectImpl__epv;
typedef struct {
  PortableServer_ServantBase__epv *_base_epv;
  POA_ORBit_ObjectImpl__epv *ORBit_ObjectImpl_epv;
} POA_ORBit_ObjectImpl__vepv;
typedef struct {
  void *_private;
  POA_ORBit_ObjectImpl__vepv *vepv;
} POA_ORBit_ObjectImpl;
extern void POA_ORBit_ObjectImpl__init(PortableServer_Servant servant, CORBA_Environment *ev);
extern void POA_ORBit_ObjectImpl__fini(PortableServer_Servant servant, CORBA_Environment *ev);
#endif /* _defined_POA_ORBit_ObjectImpl */

/** skel prototypes **/
void _ORBIT_skel_small_ORBit_ObjectImpl__is_a(POA_ORBit_ObjectImpl *_ORBIT_servant, gpointer _ORBIT_retval, gpointer *_ORBIT_args, CORBA_Context ctx,CORBA_Environment *ev, CORBA_boolean (*_impl__is_a)(PortableServer_Servant _servant, const CORBA_char * logical_type_id, CORBA_Environment *ev));

/** stub prototypes **/
CORBA_boolean ORBit_ObjectImpl__is_a(ORBit_ObjectImpl _obj, const CORBA_char * logical_type_id, CORBA_Environment *ev);
#include <orbit/orb-core/orbit-interface.h>

#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static 
#else
extern 
#endif
ORBit_IInterface ORBit_ObjectImpl__iinterface;
#define ORBit_ObjectImpl_IMETHODS_LEN 1
#ifdef ORBIT_IDL_C_IMODULE_corba_ops
static 
#else
extern 
#endif
ORBit_IMethod ORBit_ObjectImpl__imethods[ORBit_ObjectImpl_IMETHODS_LEN];

/** IMethods index */

#ifndef __ORBit_ObjectImpl__imethods_index
#define __ORBit_ObjectImpl__imethods_index
typedef enum {
	ORBit_ObjectImpl__is_a__imethods_index
} ORBit_ObjectImpl__imethods_index;
#endif /* __ORBit_ObjectImpl__imethods_index */

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
