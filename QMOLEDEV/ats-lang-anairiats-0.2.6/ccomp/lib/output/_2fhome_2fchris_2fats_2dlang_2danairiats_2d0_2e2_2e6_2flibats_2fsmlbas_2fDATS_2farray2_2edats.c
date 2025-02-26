/*
**
** The C code is generated by ATS/Anairiats
** The compilation time is: 2012-2-20: 21h:29m
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
typedef struct {
ats_size_type atslab_0 ;
ats_size_type atslab_1 ;
} anairiats_rec_0 ;

typedef struct {
ats_ptr_type atslab_data ;
ats_size_type atslab_row ;
ats_size_type atslab_col ;
} anairiats_rec_1 ;

/* external typedefs */
/* external dynamic constructor declarations */
/* external dynamic constant declarations */
ATSextern_fun(ats_bool_type, atspre_eq_int_int) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_bool_type, atspre_igt) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_ptr_type, atspre_padd_size) (ats_ptr_type, ats_size_type) ;
ATSextern_fun(ats_size_type, atspre_add_size_size) (ats_size_type, ats_size_type) ;
ATSextern_fun(ats_size_type, atspre_size1_of_int1) (ats_int_type) ;
ATSextern_fun(ats_size_type, atspre_add_size1_size1) (ats_size_type, ats_size_type) ;
ATSextern_fun(ats_size_type, atspre_mul2_size1_size1) (ats_size_type, ats_size_type) ;
ATSextern_fun(ats_bool_type, atspre_lt_size1_size1) (ats_size_type, ats_size_type) ;
ATSextern_fun(ats_bool_type, atspre_gt_size1_int1) (ats_size_type, ats_int_type) ;
ATSextern_fun(ats_ptr_type, atspre_array_ptr_alloc_tsz) (ats_size_type, ats_size_type) ;
ATSextern_fun(ats_void_type, atspre_vbox_make_view_ptr) (ats_ptr_type) ;

/* external dynamic terminating constant declarations */
#ifdef _ATS_PROOFCHECK
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2array_2esats__array_v_split_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2array_2esats__array_v_unsplit_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2matrix_2esats__array_v_of_matrix_v_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2SATS_2matrix_2esats__matrix_v_of_array_v_prfck () ;
extern
ats_void_type ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats____cast_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* sum constructor declarations */
/* exn constructor declarations */
/* global dynamic (non-functional) constant declarations */
/* internal function declarations */

/* partial value template declarations */
/* static temporary variable declarations */
/* external value variable declarations */

/* function implementations */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libats/smlbas/DATS/array2.dats: 9710(line=290, offs=22) -- 9731(line=290, offs=43)
*/
ATSglobaldec()
anairiats_rec_0
ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__dimensions (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (anairiats_rec_0, tmp0) ;
ATSlocal (ats_size_type, tmp1) ;
ATSlocal (ats_size_type, tmp2) ;

__ats_lab_ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__dimensions:
tmp1 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_1, arg0), atslab_row) ;
tmp2 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_1, arg0), atslab_col) ;
tmp0.atslab_0 = tmp1 ;
tmp0.atslab_1 = tmp2 ;

return (tmp0) ;
} /* end of [ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__dimensions] */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libats/smlbas/DATS/array2.dats: 9750(line=292, offs=17) -- 9761(line=292, offs=28)
*/
ATSglobaldec()
ats_size_type
ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nRows (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_size_type, tmp3) ;

__ats_lab_ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nRows:
tmp3 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_1, arg0), atslab_row) ;
return (tmp3) ;
} /* end of [ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nRows] */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libats/smlbas/DATS/array2.dats: 9778(line=293, offs=17) -- 9789(line=293, offs=28)
*/
ATSglobaldec()
ats_size_type
ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nCols (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_size_type, tmp4) ;

__ats_lab_ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nCols:
tmp4 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_1, arg0), atslab_col) ;
return (tmp4) ;
} /* end of [ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__nCols] */

/* static load function */

extern ats_void_type ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2general_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__staload () {
static int ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__staload_flag) return ;
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__staload_flag = 1 ;

ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2general_2esats__staload () ;
ATS_2d0_2e2_2e6_2libats_2smlbas_2SATS_2array2_2esats__staload () ;

return ;
} /* staload function */

/* dynamic load function */

// dynload flag declaration
extern ats_int_type ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__dynload_flag ;

ats_void_type
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__dynload () {
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__dynload_flag = 1 ;
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats__staload () ;

#ifdef _ATS_PROOFCHECK
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_isfun_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_nat_nat_nat_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2arith_2esats__mul_commute_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2array_2esats__array_v_split_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2array_2esats__array_v_unsplit_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2matrix_2esats__array_v_of_matrix_v_prfck () ;
ATS_2d0_2e2_2e6_2prelude_2SATS_2matrix_2esats__matrix_v_of_array_v_prfck () ;
ATS_2d0_2e2_2e6_2libats_2smlbas_2DATS_2array2_2edats____cast_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* marking static variables for GC */

/* marking external values for GC */

/* code for dynamic loading */
return ;
} /* end of [dynload function] */

/* external codes at mid */
/* external codes at bot */

/* ****** ****** */

/* end of [/home/chris/ats-lang-anairiats-0.2.6/ccomp/lib/output/_2fhome_2fchris_2fats_2dlang_2danairiats_2d0_2e2_2e6_2flibats_2fsmlbas_2fDATS_2farray2_2edats.c] */
