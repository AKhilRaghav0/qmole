/*
**
** The C code is generated by ATS/Anairiats
** The compilation time is: 2011-10-15: 19h:52m
**
*/

/* include some .h files */
#ifndef _ATS_HEADER_NONE
#include "ats_config.h"
#include "ats_basics.h"
#include "ats_types.h"
#include "ats_exception.h"
#include "ats_memory.h"
#endif /* _ATS_HEADER_NONE */

/* prologues from statically loaded files */

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"

#include "ats_counter.cats"

#include "ats_intinf.cats"

#include "ats_counter.cats"
/* external codes at top */
/* type definitions */
/* external typedefs */
/* sum constructor declarations */
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__OFFSETlab) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__OFFSETind) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VParg) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPargref) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPargtmpref) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPbool) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcastfn) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPchar) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPclo) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcst) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcstsp) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPenv) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPext) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfix) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfloat) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfloatsp) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfun) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPint) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPintsp) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof_ptr_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof_var_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPsizeof) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPstring) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtmp) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtmpref) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtop) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPvoid) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__LABVALPRIMLSTcons) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__LABVALPRIMLSTnil) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKbool) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKchar) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKcon) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKexn) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKfloat) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKint) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKstring) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTnone) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTtmplab) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTtmplabint) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTcaseof_fail) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTfunarg_fail) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTraise) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTmatpnt) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRarr_heap) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRarr_stack) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRassgn_arr) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRassgn_clo) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcall) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcall_tail) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcond) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_clo) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_fun) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_val) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_partval) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRextern) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRextval) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfreeptr) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfunction) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfunlab) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdynload_file) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_ptr) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_ptr_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_var) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_var_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRloop) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRloopexn) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_arg) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_con) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_lazy_delay) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_lazy_force) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_rec_box) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_rec_flt) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_ref) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_val) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRpatck) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRraise) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselect) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselcon) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselcon_ptr) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRswitch) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_ptr) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_ptr_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_var) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_var_offs) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRtmplabint) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_beg) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_end) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_tst) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRtrywith) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRvardec) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__TAILJOINLSTcons) ;
ATSglobal(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__TAILJOINLSTnil) ;

/* exn constructor declarations */
/* static load function */

extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_filename_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_location_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__staload () {
static int ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__staload_flag) return ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__staload_flag = 1 ;

ATS_2d0_2e2_2e6_2src_2ats_filename_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_location_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__staload () ;

// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__OFFSETlab.tag = 0 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__OFFSETind.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VParg.tag = 0 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPargref.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPargtmpref.tag = 2 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPbool.tag = 3 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcastfn.tag = 4 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPchar.tag = 5 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPclo.tag = 6 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcst.tag = 7 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPcstsp.tag = 8 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPenv.tag = 9 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPext.tag = 10 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfix.tag = 11 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfloat.tag = 12 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfloatsp.tag = 13 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPfun.tag = 14 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPint.tag = 15 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPintsp.tag = 16 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof.tag = 17 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof_ptr_offs.tag = 18 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPptrof_var_offs.tag = 19 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPsizeof.tag = 20 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPstring.tag = 21 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtmp.tag = 22 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtmpref.tag = 23 ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPtop.tag = 24 ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__VPvoid.tag = 25 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__LABVALPRIMLSTcons.tag = 0 ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__LABVALPRIMLSTnil.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKbool.tag = 0 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKchar.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKcon.tag = 2 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKexn.tag = 3 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKfloat.tag = 4 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKint.tag = 5 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__PATCKstring.tag = 6 ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTnone.tag = 0 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTtmplab.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTtmplabint.tag = 2 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTcaseof_fail.tag = 3 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTfunarg_fail.tag = 4 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTraise.tag = 5 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__KONTmatpnt.tag = 6 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRarr_heap.tag = 0 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRarr_stack.tag = 1 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRassgn_arr.tag = 2 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRassgn_clo.tag = 3 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcall.tag = 4 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcall_tail.tag = 5 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRcond.tag = 6 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_clo.tag = 7 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_fun.tag = 8 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_val.tag = 9 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdefine_partval.tag = 10 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRextern.tag = 11 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRextval.tag = 12 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfreeptr.tag = 13 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfunction.tag = 14 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRfunlab.tag = 15 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRdynload_file.tag = 16 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_ptr.tag = 17 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_ptr_offs.tag = 18 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_var.tag = 19 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRload_var_offs.tag = 20 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRloop.tag = 21 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRloopexn.tag = 22 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_arg.tag = 23 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_con.tag = 24 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_lazy_delay.tag = 25 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_lazy_force.tag = 26 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_rec_box.tag = 27 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_rec_flt.tag = 28 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_ref.tag = 29 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRmove_val.tag = 30 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRpatck.tag = 31 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRraise.tag = 32 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselect.tag = 33 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselcon.tag = 34 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRselcon_ptr.tag = 35 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRswitch.tag = 36 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_ptr.tag = 37 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_ptr_offs.tag = 38 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_var.tag = 39 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRstore_var_offs.tag = 40 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRtmplabint.tag = 41 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_beg.tag = 42 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_end.tag = 43 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRprfck_tst.tag = 44 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRtrywith.tag = 45 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__INSTRvardec.tag = 46 ;
// ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__TAILJOINLSTcons.tag = 0 ;
ATS_2d0_2e2_2e6_2src_2ats_ccomp_2esats__TAILJOINLSTnil.tag = 1 ;
return ;
} /* staload function */

/* external codes at mid */
/* external codes at bot */

/* ****** ****** */

/* end of [ats_ccomp_sats.c] */
