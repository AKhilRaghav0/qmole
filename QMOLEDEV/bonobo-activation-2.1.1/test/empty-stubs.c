/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#include <string.h>
#define ORBIT2_STUBS_API
#include "empty.h"

void
Empty_doNothing(Empty _obj, CORBA_Environment * ev)
{
   POA_Empty__epv *_ORBIT_epv;

   if (ORBit_small_flags & ORBIT_SMALL_FAST_LOCALS &&
       ORBIT_STUB_IsBypass(_obj, Empty__classid) &&
       (_ORBIT_epv =
	(POA_Empty__epv *) ORBIT_STUB_GetEpv(_obj,
					     Empty__classid))->doNothing) {
      ORBIT_STUB_PreCall(_obj);
      _ORBIT_epv->doNothing(ORBIT_STUB_GetServant(_obj), ev);
      ORBIT_STUB_PostCall(_obj);
   } else {			/* remote marshal */
      ORBit_small_invoke_stub_n(_obj, &Empty__iinterface.methods, 0, NULL,
				NULL, NULL, ev);

   }
}
