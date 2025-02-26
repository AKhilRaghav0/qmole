/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#include <string.h>
#define ORBIT2_STUBS_API
#include "everything.h"

test_BasicServer test_TestFactory_getBasicServer(test_TestFactory _obj, CORBA_Environment *ev){
test_BasicServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 0, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getBasicServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getBasicServer);

return _ORBIT_retval;
}
test_StructServer test_TestFactory_getStructServer(test_TestFactory _obj, CORBA_Environment *ev){
test_StructServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 1, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getStructServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getStructServer);

return _ORBIT_retval;
}
CORBA_string test_TestFactory_getStructServerIOR(test_TestFactory _obj, CORBA_Environment *ev){
CORBA_string _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 2, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getStructServerIOR),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getStructServerIOR);

return _ORBIT_retval;
}
test_SequenceServer test_TestFactory_getSequenceServer(test_TestFactory _obj, CORBA_Environment *ev){
test_SequenceServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 3, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getSequenceServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getSequenceServer);

return _ORBIT_retval;
}
test_UnionServer test_TestFactory_getUnionServer(test_TestFactory _obj, CORBA_Environment *ev){
test_UnionServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 4, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getUnionServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getUnionServer);

return _ORBIT_retval;
}
test_ArrayServer test_TestFactory_getArrayServer(test_TestFactory _obj, CORBA_Environment *ev){
test_ArrayServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 5, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getArrayServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getArrayServer);

return _ORBIT_retval;
}
test_AnyServer test_TestFactory_getAnyServer(test_TestFactory _obj, CORBA_Environment *ev){
test_AnyServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 6, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getAnyServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getAnyServer);

return _ORBIT_retval;
}
test_ContextServer test_TestFactory_getContextServer(test_TestFactory _obj, CORBA_Environment *ev){
test_ContextServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 7, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getContextServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getContextServer);

return _ORBIT_retval;
}
void test_TestFactory_segv(test_TestFactory _obj, const CORBA_char * when, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&when;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 8, NULL, _args, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, segv),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_segv);

}
test_BaseServer test_TestFactory_getBaseServer(test_TestFactory _obj, CORBA_Environment *ev){
test_BaseServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 9, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getBaseServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getBaseServer);

return _ORBIT_retval;
}
test_DerivedServer test_TestFactory_getDerivedServer(test_TestFactory _obj, CORBA_Environment *ev){
test_DerivedServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 10, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getDerivedServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getDerivedServer);

return _ORBIT_retval;
}
test_BaseServer test_TestFactory_getDerivedServerAsBaseServer(test_TestFactory _obj, CORBA_Environment *ev){
test_BaseServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 11, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getDerivedServerAsBaseServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getDerivedServerAsBaseServer);

return _ORBIT_retval;
}
test_B2 test_TestFactory_getDerivedServerAsB2(test_TestFactory _obj, CORBA_Environment *ev){
test_B2 _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 12, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, getDerivedServerAsB2),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_getDerivedServerAsB2);

return _ORBIT_retval;
}
test_TransientObj test_TestFactory_createTransientObj(test_TestFactory _obj, CORBA_Environment *ev){
test_TransientObj _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 13, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, createTransientObj),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_createTransientObj);

return _ORBIT_retval;
}
test_DeadReferenceObj test_TestFactory_createDeadReferenceObj(test_TestFactory _obj, CORBA_Environment *ev){
test_DeadReferenceObj _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 14, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, createDeadReferenceObj),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_createDeadReferenceObj);

return _ORBIT_retval;
}
test_PingPongServer test_TestFactory_createPingPongServer(test_TestFactory _obj, CORBA_Environment *ev){
test_PingPongServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 15, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, createPingPongServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_createPingPongServer);

return _ORBIT_retval;
}
test_LifeCycleServer test_TestFactory_createLifeCycleServer(test_TestFactory _obj, CORBA_Environment *ev){
test_LifeCycleServer _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 16, &_ORBIT_retval, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, createLifeCycleServer),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_createLifeCycleServer);

return _ORBIT_retval;
}
void test_TestFactory_noOp(test_TestFactory _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_TestFactory__iinterface.methods, 17, NULL, NULL, NULL, ev, test_TestFactory__classid, G_STRUCT_OFFSET (POA_test_TestFactory__epv, noOp),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TestFactory_noOp);

}
void test_LifeCycleServer_deactivateOnReturn(test_LifeCycleServer _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_LifeCycleServer__iinterface.methods, 0, NULL, NULL, NULL, ev, test_LifeCycleServer__classid, G_STRUCT_OFFSET (POA_test_LifeCycleServer__epv, deactivateOnReturn),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_LifeCycleServer_deactivateOnReturn);

}
void test_LifeCycleServer_deactivateUnrefOnReturn(test_LifeCycleServer _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_LifeCycleServer__iinterface.methods, 1, NULL, NULL, NULL, ev, test_LifeCycleServer__classid, G_STRUCT_OFFSET (POA_test_LifeCycleServer__epv, deactivateUnrefOnReturn),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_LifeCycleServer_deactivateUnrefOnReturn);

}
void test_DeadReferenceObj_test(test_DeadReferenceObj _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_DeadReferenceObj__iinterface.methods, 0, NULL, NULL, NULL, ev, test_DeadReferenceObj__classid, G_STRUCT_OFFSET (POA_test_DeadReferenceObj__epv, test),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_DeadReferenceObj_test);

}
void test_TransientObj_remove(test_TransientObj _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_TransientObj__iinterface.methods, 0, NULL, NULL, NULL, ev, test_TransientObj__classid, G_STRUCT_OFFSET (POA_test_TransientObj__epv, remove),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_TransientObj_remove);

}
test_StrSeq* test_SequenceServer_opStrSeq(test_SequenceServer _obj, const test_StrSeq* inArg, test_StrSeq* inoutArg, test_StrSeq** outArg, CORBA_Environment *ev){
test_StrSeq* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_SequenceServer__iinterface.methods, 0, &_ORBIT_retval, _args, NULL, ev, test_SequenceServer__classid, G_STRUCT_OFFSET (POA_test_SequenceServer__epv, opStrSeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_SequenceServer_opStrSeq);

return _ORBIT_retval;
}
test_BoundedStructSeq* test_SequenceServer_opBoundedStructSeq(test_SequenceServer _obj, const test_BoundedStructSeq* inArg, test_BoundedStructSeq* inoutArg, test_BoundedStructSeq** outArg, CORBA_Environment *ev){
test_BoundedStructSeq* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_SequenceServer__iinterface.methods, 1, &_ORBIT_retval, _args, NULL, ev, test_SequenceServer__classid, G_STRUCT_OFFSET (POA_test_SequenceServer__epv, opBoundedStructSeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_SequenceServer_opBoundedStructSeq);

return _ORBIT_retval;
}
test_LongSeq* test_SequenceServer_opMassiveSeq(test_SequenceServer _obj, CORBA_Environment *ev){
test_LongSeq* _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_SequenceServer__iinterface.methods, 2, &_ORBIT_retval, NULL, NULL, ev, test_SequenceServer__classid, G_STRUCT_OFFSET (POA_test_SequenceServer__epv, opMassiveSeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_SequenceServer_opMassiveSeq);

return _ORBIT_retval;
}
test_AnySeq* test_SequenceServer_opAnySeq(test_SequenceServer _obj, CORBA_Environment *ev){
test_AnySeq* _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_SequenceServer__iinterface.methods, 3, &_ORBIT_retval, NULL, NULL, ev, test_SequenceServer__classid, G_STRUCT_OFFSET (POA_test_SequenceServer__epv, opAnySeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_SequenceServer_opAnySeq);

return _ORBIT_retval;
}
test_LongArray_slice* test_ArrayServer_opLongArray(test_ArrayServer _obj, const test_LongArray inArg, test_LongArray inoutArg, test_LongArray outArg, CORBA_Environment *ev){
test_LongArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_ArrayServer__iinterface.methods, 0, &_ORBIT_retval, _args, NULL, ev, test_ArrayServer__classid, G_STRUCT_OFFSET (POA_test_ArrayServer__epv, opLongArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ArrayServer_opLongArray);

return _ORBIT_retval;
}
test_OctetArray_slice* test_ArrayServer_opOctetArray(test_ArrayServer _obj, const test_OctetArray inArg, test_OctetArray inoutArg, test_OctetArray outArg, CORBA_Environment *ev){
test_OctetArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_ArrayServer__iinterface.methods, 1, &_ORBIT_retval, _args, NULL, ev, test_ArrayServer__classid, G_STRUCT_OFFSET (POA_test_ArrayServer__epv, opOctetArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ArrayServer_opOctetArray);

return _ORBIT_retval;
}
test_FixedLengthStructArray_slice* test_ArrayServer_opFixedLengthStructArray(test_ArrayServer _obj, const test_FixedLengthStructArray inArg, test_FixedLengthStructArray inoutArg, test_FixedLengthStructArray outArg, CORBA_Environment *ev){
test_FixedLengthStructArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_ArrayServer__iinterface.methods, 2, &_ORBIT_retval, _args, NULL, ev, test_ArrayServer__classid, G_STRUCT_OFFSET (POA_test_ArrayServer__epv, opFixedLengthStructArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ArrayServer_opFixedLengthStructArray);

return _ORBIT_retval;
}
test_StrArray_slice* test_ArrayServer_opStrArray(test_ArrayServer _obj, const test_StrArray inArg, test_StrArray inoutArg, test_StrArray_slice** outArg, CORBA_Environment *ev){
test_StrArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_ArrayServer__iinterface.methods, 3, &_ORBIT_retval, _args, NULL, ev, test_ArrayServer__classid, G_STRUCT_OFFSET (POA_test_ArrayServer__epv, opStrArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ArrayServer_opStrArray);

return _ORBIT_retval;
}
test_AlignHoleStructArray_slice* test_ArrayServer_opAlignHoleStructArray(test_ArrayServer _obj, const test_AlignHoleStructArray inArg, test_AlignHoleStructArray inoutArg, test_AlignHoleStructArray outArg, CORBA_Environment *ev){
test_AlignHoleStructArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_ArrayServer__iinterface.methods, 4, &_ORBIT_retval, _args, NULL, ev, test_ArrayServer__classid, G_STRUCT_OFFSET (POA_test_ArrayServer__epv, opAlignHoleStructArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ArrayServer_opAlignHoleStructArray);

return _ORBIT_retval;
}
CORBA_string test_BasicServer__get_foo(test_BasicServer _obj, CORBA_Environment *ev){
CORBA_string _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 0, &_ORBIT_retval, NULL, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, _get_foo),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer__get_foo);

return _ORBIT_retval;
}
void test_BasicServer__set_foo(test_BasicServer _obj, const CORBA_char * value, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&value;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 1, NULL, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, _set_foo),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer__set_foo);

}
CORBA_long test_BasicServer__get_bah(test_BasicServer _obj, CORBA_Environment *ev){
CORBA_long _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 2, &_ORBIT_retval, NULL, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, _get_bah),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer__get_bah);

return _ORBIT_retval;
}
CORBA_string test_BasicServer_opString(test_BasicServer _obj, const CORBA_char * inArg, CORBA_string* inoutArg, CORBA_string* outArg, CORBA_Environment *ev){
CORBA_string _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 3, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opString),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opString);

return _ORBIT_retval;
}
CORBA_long test_BasicServer_opLong(test_BasicServer _obj, const CORBA_long inArg, CORBA_long* inoutArg, CORBA_long* outArg, CORBA_Environment *ev){
CORBA_long _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 4, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opLong),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opLong);

return _ORBIT_retval;
}
CORBA_long_long test_BasicServer_opLongLong(test_BasicServer _obj, const CORBA_long_long inArg, CORBA_long_long* inoutArg, CORBA_long_long* outArg, CORBA_Environment *ev){
CORBA_long_long _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 5, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opLongLong),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opLongLong);

return _ORBIT_retval;
}
CORBA_float test_BasicServer_opFloat(test_BasicServer _obj, const CORBA_float inArg, CORBA_float* inoutArg, CORBA_float* outArg, CORBA_Environment *ev){
CORBA_float _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 6, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opFloat),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opFloat);

return _ORBIT_retval;
}
CORBA_double test_BasicServer_opDouble(test_BasicServer _obj, const CORBA_double inArg, CORBA_double* inoutArg, CORBA_double* outArg, CORBA_Environment *ev){
CORBA_double _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 7, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opDouble),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opDouble);

return _ORBIT_retval;
}
CORBA_long_double test_BasicServer_opLongDouble(test_BasicServer _obj, const CORBA_long_double inArg, CORBA_long_double* inoutArg, CORBA_long_double* outArg, CORBA_Environment *ev){
CORBA_long_double _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 8, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opLongDouble),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opLongDouble);

return _ORBIT_retval;
}
test_AnEnum test_BasicServer_opEnum(test_BasicServer _obj, const test_AnEnum inArg, test_AnEnum* inoutArg, test_AnEnum* outArg, CORBA_Environment *ev){
test_AnEnum _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 9, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opEnum),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opEnum);

return _ORBIT_retval;
}
void test_BasicServer_opException(test_BasicServer _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 10, NULL, NULL, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opException),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opException);

}
void test_BasicServer_opOneWay(test_BasicServer _obj, const CORBA_char * inArg, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&inArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 11, NULL, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, opOneWay),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_opOneWay);

}
void test_BasicServer_noImplement(test_BasicServer _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 12, NULL, NULL, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, noImplement),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_noImplement);

}
void test_BasicServer_testLargeStringSeq(test_BasicServer _obj, const test_StrSeq* seq, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)seq;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 13, NULL, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, testLargeStringSeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_testLargeStringSeq);

}
CORBA_long test_BasicServer_getObjectCount(test_BasicServer _obj, CORBA_Environment *ev){
CORBA_long _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 14, &_ORBIT_retval, NULL, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, getObjectCount),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_getObjectCount);

return _ORBIT_retval;
}
CORBA_Object test_BasicServer_getObject(test_BasicServer _obj, const CORBA_long which, CORBA_Environment *ev){
CORBA_Object _ORBIT_retval;
gpointer _args[1];
_args[0] = (gpointer)&which;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 15, &_ORBIT_retval, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, getObject),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_getObject);

return _ORBIT_retval;
}
void test_BasicServer_testBoolString(test_BasicServer _obj, const CORBA_boolean inBool, const CORBA_char * inArg, CORBA_string* inoutArg, CORBA_Environment *ev){
gpointer _args[3];
_args[0] = (gpointer)&inBool;
_args[1] = (gpointer)&inArg;
_args[2] = inoutArg;
ORBit_c_stub_invoke (_obj, &test_BasicServer__iinterface.methods, 16, NULL, _args, NULL, ev, test_BasicServer__classid, G_STRUCT_OFFSET (POA_test_BasicServer__epv, testBoolString),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BasicServer_testBoolString);

}
test_FixedLengthStruct test_StructServer_opFixed(test_StructServer _obj, const test_FixedLengthStruct* inArg, test_FixedLengthStruct* inoutArg, test_FixedLengthStruct* outArg, CORBA_Environment *ev){
test_FixedLengthStruct _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 0, &_ORBIT_retval, _args, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opFixed),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opFixed);

return _ORBIT_retval;
}
test_VariableLengthStruct* test_StructServer_opVariable(test_StructServer _obj, const test_VariableLengthStruct* inArg, test_VariableLengthStruct* inoutArg, test_VariableLengthStruct** outArg, CORBA_Environment *ev){
test_VariableLengthStruct* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 1, &_ORBIT_retval, _args, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opVariable),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opVariable);

return _ORBIT_retval;
}
test_CompoundStruct* test_StructServer_opCompound(test_StructServer _obj, const test_CompoundStruct* inArg, test_CompoundStruct* inoutArg, test_CompoundStruct** outArg, CORBA_Environment *ev){
test_CompoundStruct* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 2, &_ORBIT_retval, _args, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opCompound),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opCompound);

return _ORBIT_retval;
}
test_AlignHoleStruct test_StructServer_opAlignHole(test_StructServer _obj, const test_AlignHoleStruct* inArg, test_AlignHoleStruct* inoutArg, test_AlignHoleStruct* outArg, CORBA_Environment *ev){
test_AlignHoleStruct _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 3, &_ORBIT_retval, _args, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opAlignHole),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opAlignHole);

return _ORBIT_retval;
}
void test_StructServer_opObjectStruct(test_StructServer _obj, const test_ObjectStruct* inArg, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)inArg;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 4, NULL, _args, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opObjectStruct),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opObjectStruct);

}
test_StructAny* test_StructServer_opStructAny(test_StructServer _obj, CORBA_Environment *ev){
test_StructAny* _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_StructServer__iinterface.methods, 5, &_ORBIT_retval, NULL, NULL, ev, test_StructServer__classid, G_STRUCT_OFFSET (POA_test_StructServer__epv, opStructAny),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_StructServer_opStructAny);

return _ORBIT_retval;
}
CORBA_long test_BaseServer_opPolymorphic(test_BaseServer _obj, CORBA_Environment *ev){
CORBA_long _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_BaseServer__iinterface.methods, 0, &_ORBIT_retval, NULL, NULL, ev, test_BaseServer__classid, G_STRUCT_OFFSET (POA_test_BaseServer__epv, opPolymorphic),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BaseServer_opPolymorphic);

return _ORBIT_retval;
}
CORBA_string test_BaseServer__get_attribPolymorphic(test_BaseServer _obj, CORBA_Environment *ev){
CORBA_string _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_BaseServer__iinterface.methods, 1, &_ORBIT_retval, NULL, NULL, ev, test_BaseServer__classid, G_STRUCT_OFFSET (POA_test_BaseServer__epv, _get_attribPolymorphic),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BaseServer__get_attribPolymorphic);

return _ORBIT_retval;
}
void test_BaseServer__set_attribPolymorphic(test_BaseServer _obj, const CORBA_char * value, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&value;
ORBit_c_stub_invoke (_obj, &test_BaseServer__iinterface.methods, 2, NULL, _args, NULL, ev, test_BaseServer__classid, G_STRUCT_OFFSET (POA_test_BaseServer__epv, _set_attribPolymorphic),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_BaseServer__set_attribPolymorphic);

}
test_FixedLengthUnion test_UnionServer_opFixed(test_UnionServer _obj, const test_FixedLengthUnion* inArg, test_FixedLengthUnion* inoutArg, test_FixedLengthUnion* outArg, CORBA_Environment *ev){
test_FixedLengthUnion _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_UnionServer__iinterface.methods, 0, &_ORBIT_retval, _args, NULL, ev, test_UnionServer__classid, G_STRUCT_OFFSET (POA_test_UnionServer__epv, opFixed),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_UnionServer_opFixed);

return _ORBIT_retval;
}
test_VariableLengthUnion* test_UnionServer_opVariable(test_UnionServer _obj, const test_VariableLengthUnion* inArg, test_VariableLengthUnion* inoutArg, test_VariableLengthUnion** outArg, CORBA_Environment *ev){
test_VariableLengthUnion* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_UnionServer__iinterface.methods, 1, &_ORBIT_retval, _args, NULL, ev, test_UnionServer__classid, G_STRUCT_OFFSET (POA_test_UnionServer__epv, opVariable),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_UnionServer_opVariable);

return _ORBIT_retval;
}
test_EnumUnion test_UnionServer_opMisc(test_UnionServer _obj, const test_unionSeq* inSeq, const test_BooleanUnion* inArg, test_ArrayUnion** outArg, CORBA_Environment *ev){
test_EnumUnion _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inSeq;
_args[1] = (gpointer)inArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_UnionServer__iinterface.methods, 2, &_ORBIT_retval, _args, NULL, ev, test_UnionServer__classid, G_STRUCT_OFFSET (POA_test_UnionServer__epv, opMisc),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_UnionServer_opMisc);

return _ORBIT_retval;
}
test_FixedLengthUnionArray_slice* test_UnionServer_opFixedLengthUnionArray(test_UnionServer _obj, const test_FixedLengthUnionArray inArg, test_FixedLengthUnionArray inoutArg, test_FixedLengthUnionArray outArg, CORBA_Environment *ev){
test_FixedLengthUnionArray_slice* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_UnionServer__iinterface.methods, 3, &_ORBIT_retval, _args, NULL, ev, test_UnionServer__classid, G_STRUCT_OFFSET (POA_test_UnionServer__epv, opFixedLengthUnionArray),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_UnionServer_opFixedLengthUnionArray);

return _ORBIT_retval;
}
CORBA_any* test_AnyServer_opAnyStrSeq(test_AnyServer _obj, CORBA_Environment *ev){
CORBA_any* _ORBIT_retval;
ORBit_c_stub_invoke (_obj, &test_AnyServer__iinterface.methods, 0, &_ORBIT_retval, NULL, NULL, ev, test_AnyServer__classid, G_STRUCT_OFFSET (POA_test_AnyServer__epv, opAnyStrSeq),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_AnyServer_opAnyStrSeq);

return _ORBIT_retval;
}
CORBA_any* test_AnyServer_opAnyLong(test_AnyServer _obj, const CORBA_any* inArg, CORBA_any* inoutArg, CORBA_any** outArg, CORBA_Environment *ev){
CORBA_any* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_AnyServer__iinterface.methods, 1, &_ORBIT_retval, _args, NULL, ev, test_AnyServer__classid, G_STRUCT_OFFSET (POA_test_AnyServer__epv, opAnyLong),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_AnyServer_opAnyLong);

return _ORBIT_retval;
}
CORBA_any* test_AnyServer_opAnyString(test_AnyServer _obj, const CORBA_any* inArg, CORBA_any* inoutArg, CORBA_any** outArg, CORBA_Environment *ev){
CORBA_any* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_AnyServer__iinterface.methods, 2, &_ORBIT_retval, _args, NULL, ev, test_AnyServer__classid, G_STRUCT_OFFSET (POA_test_AnyServer__epv, opAnyString),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_AnyServer_opAnyString);

return _ORBIT_retval;
}
CORBA_any* test_AnyServer_opAnyStruct(test_AnyServer _obj, const CORBA_any* inArg, CORBA_any* inoutArg, CORBA_any** outArg, CORBA_Environment *ev){
CORBA_any* _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)inArg;
_args[1] = inoutArg;
_args[2] = outArg;
ORBit_c_stub_invoke (_obj, &test_AnyServer__iinterface.methods, 3, &_ORBIT_retval, _args, NULL, ev, test_AnyServer__classid, G_STRUCT_OFFSET (POA_test_AnyServer__epv, opAnyStruct),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_AnyServer_opAnyStruct);

return _ORBIT_retval;
}
CORBA_TypeCode test_AnyServer_opTypeCode(test_AnyServer _obj, const CORBA_TypeCode inArg, CORBA_TypeCode* inoutArg, CORBA_TypeCode* outArg, CORBA_Environment *ev){
CORBA_TypeCode _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_AnyServer__iinterface.methods, 4, &_ORBIT_retval, _args, NULL, ev, test_AnyServer__classid, G_STRUCT_OFFSET (POA_test_AnyServer__epv, opTypeCode),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_AnyServer_opTypeCode);

return _ORBIT_retval;
}
CORBA_Object test_ContextServer_opWithContext(test_ContextServer _obj, const CORBA_Object inArg, CORBA_Object* inoutArg, CORBA_Object* outArg, CORBA_Context _ctx, CORBA_Environment *ev){
CORBA_Object _ORBIT_retval;
gpointer _args[3];
_args[0] = (gpointer)&inArg;
_args[1] = inoutArg;
_args[2] = &outArg;
ORBit_c_stub_invoke (_obj, &test_ContextServer__iinterface.methods, 0, &_ORBIT_retval, _args, _ctx, ev, test_ContextServer__classid, G_STRUCT_OFFSET (POA_test_ContextServer__epv, opWithContext),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_ContextServer_opWithContext);

return _ORBIT_retval;
}
void test_PingPongServer_opSleep(test_PingPongServer _obj, const CORBA_char * large_string, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&large_string;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 0, NULL, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, opSleep),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_opSleep);

}
void test_PingPongServer_opOneWay(test_PingPongServer _obj, const CORBA_long l, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&l;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 1, NULL, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, opOneWay),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_opOneWay);

}
void test_PingPongServer_opOneWayCallback(test_PingPongServer _obj, const test_PingPongServer obj, CORBA_Environment *ev){
gpointer _args[1];
_args[0] = (gpointer)&obj;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 2, NULL, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, opOneWayCallback),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_opOneWayCallback);

}
void test_PingPongServer_opRoundTrip(test_PingPongServer _obj, CORBA_Environment *ev){
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 3, NULL, NULL, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, opRoundTrip),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_opRoundTrip);

}
CORBA_long test_PingPongServer_pingPong(test_PingPongServer _obj, const test_PingPongServer replyTo, const CORBA_long idx, CORBA_Environment *ev){
CORBA_long _ORBIT_retval;
gpointer _args[2];
_args[0] = (gpointer)&replyTo;
_args[1] = (gpointer)&idx;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 4, &_ORBIT_retval, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, pingPong),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_pingPong);

return _ORBIT_retval;
}
void test_PingPongServer_set(test_PingPongServer _obj, const test_PingPongServer reg, const CORBA_char * name, CORBA_Environment *ev){
gpointer _args[2];
_args[0] = (gpointer)&reg;
_args[1] = (gpointer)&name;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 5, NULL, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, set),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_set);

}
CORBA_Object test_PingPongServer_get(test_PingPongServer _obj, const CORBA_char * name, CORBA_Environment *ev){
CORBA_Object _ORBIT_retval;
gpointer _args[1];
_args[0] = (gpointer)&name;
ORBit_c_stub_invoke (_obj, &test_PingPongServer__iinterface.methods, 6, &_ORBIT_retval, _args, NULL, ev, test_PingPongServer__classid, G_STRUCT_OFFSET (POA_test_PingPongServer__epv, get),
(ORBitSmallSkeleton) _ORBIT_skel_small_test_PingPongServer_get);

return _ORBIT_retval;
}
