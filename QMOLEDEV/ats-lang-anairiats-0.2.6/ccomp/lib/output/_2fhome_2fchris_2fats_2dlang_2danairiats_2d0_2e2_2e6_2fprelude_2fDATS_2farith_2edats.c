/*
**
** The C code is generated by ATS/Anairiats
** The compilation time is: 2012-2-20: 21h:25m
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

/* include some .cats files */
#ifndef _ATS_PRELUDE_NONE
#include "prelude/CATS/basics.cats"
#include "prelude/CATS/bool.cats"
#include "prelude/CATS/byte.cats"
#include "prelude/CATS/char.cats"
#include "prelude/CATS/float.cats"
#include "prelude/CATS/integer.cats"
#include "prelude/CATS/integer_fixed.cats"
#include "prelude/CATS/integer_ptr.cats"
#include "prelude/CATS/lazy.cats"
#include "prelude/CATS/lazy_vt.cats"
#include "prelude/CATS/pointer.cats"
#include "prelude/CATS/printf.cats"
#include "prelude/CATS/reference.cats"
#include "prelude/CATS/sizetype.cats"
#include "prelude/CATS/string.cats"
#include "prelude/CATS/array.cats"
#include "prelude/CATS/list.cats"
#include "prelude/CATS/matrix.cats"
#include "prelude/CATS/option.cats"
#endif /* _ATS_PRELUDE_NONE */
/* prologues from statically loaded files */
/* external codes at top */
/* type definitions */
/* external typedefs */
/* external dynamic constructor declarations */
/* external dynamic constant declarations */

/* external dynamic terminating constant declarations */
#ifdef _ATS_PROOFCHECK
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* sum constructor declarations */
/* exn constructor declarations */
/* global dynamic (non-functional) constant declarations */
/* internal function declarations */

/* partial value template declarations */
/* static temporary variable declarations */
/* external value variable declarations */

/* function implementations */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 1615(line=44, offs=1) -- 2001(line=55, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () {
/* local vardec */
// ATSlocal_void (tmp0) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun) ;
return /* (tmp0) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 2025(line=57, offs=1) -- 2307(line=66, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () {
/* local vardec */
// ATSlocal_void (tmp1) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot) ;
return /* (tmp1) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 2352(line=70, offs=1) -- 2561(line=78, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () {
/* local vardec */
// ATSlocal_void (tmp2) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat) ;
return /* (tmp2) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 2591(line=80, offs=1) -- 2870(line=91, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos_prfck () {
/* local vardec */
// ATSlocal_void (tmp3) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos) ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos) ;
return /* (tmp3) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 3496(line=115, offs=1) -- 3813(line=128, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate_prfck () {
/* local vardec */
// ATSlocal_void (tmp4) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate) ;
return /* (tmp4) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 3838(line=130, offs=1) -- 3890(line=131, offs=43)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2_prfck () {
/* local vardec */
// ATSlocal_void (tmp5) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2) ;
return /* (tmp5) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 3913(line=135, offs=1) -- 4351(line=148, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () {
/* local vardec */
// ATSlocal_void (tmp6) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute) ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute) ;
return /* (tmp6) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 4398(line=152, offs=1) -- 4820(line=167, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () {
/* local vardec */
// ATSlocal_void (tmp7) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute) ;
return /* (tmp7) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 4849(line=169, offs=1) -- 4953(line=170, offs=66)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2_prfck () {
/* local vardec */
// ATSlocal_void (tmp8) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2) ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2) ;
return /* (tmp8) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 5004(line=175, offs=1) -- 6509(line=224, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate_prfck () {
/* local vardec */
// ATSlocal_void (tmp9) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate) ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate) ;
return /* (tmp9) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 6592(line=232, offs=1) -- 6758(line=235, offs=2)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot_prfck () {
/* local vardec */
// ATSlocal_void (tmp10) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot) ;
return /* (tmp10) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 6783(line=237, offs=1) -- 7191(line=249, offs=2)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun_prfck () {
/* local vardec */
// ATSlocal_void (tmp11) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun) ;
return /* (tmp11) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 7216(line=251, offs=1) -- 7437(line=260, offs=2)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos_prfck () {
/* local vardec */
// ATSlocal_void (tmp12) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos) ;
return /* (tmp12) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 7462(line=262, offs=1) -- 7888(line=272, offs=2)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone_prfck () {
/* local vardec */
// ATSlocal_void (tmp13) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone) ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone) ;
return /* (tmp13) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone_prfck] */
#endif /* _ATS_PROOFCHECK */

#ifdef _ATS_PROOFCHECK
/*
// /home/chris/ats-lang-anairiats-0.2.6/prelude/DATS/arith.dats: 7922(line=274, offs=1) -- 8556(line=290, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul_prfck () {
/* local vardec */
// ATSlocal_void (tmp14) ;

ats_proofcheck_beg_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul) ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () ;
ats_proofcheck_end_mac(ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul) ;
return /* (tmp14) */ ;
} /* end of [ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul_prfck] */
#endif /* _ATS_PROOFCHECK */

/* static load function */

// extern ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__staload () {
static int ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__staload_flag) return ;
ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__staload_flag = 1 ;

// ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__staload () ;

return ;
} /* staload function */

/* dynamic load function */

// dynload flag declaration
// extern ats_int_type ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__dynload_flag ;

ats_void_type
ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__dynload () {
// ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__dynload_flag = 1 ;
ATS_2d0_2e2_2e6_2prelude_2DATS_2arith_2edats__staload () ;

#ifdef _ATS_PROOFCHECK
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_istot_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_pos_pos_pos_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_negate2_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_distribute2_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_associate_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_istot_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_isfun_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_ispos_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_monotone_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__EXP2_mul_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* marking static variables for GC */

/* marking external values for GC */

/* code for dynamic loading */
return ;
} /* end of [dynload function] */

/* external codes at mid */
/* external codes at bot */

/* ****** ****** */

/* end of [/home/chris/ats-lang-anairiats-0.2.6/ccomp/lib/output/_2fhome_2fchris_2fats_2dlang_2danairiats_2d0_2e2_2e6_2fprelude_2fDATS_2farith_2edats.c] */
