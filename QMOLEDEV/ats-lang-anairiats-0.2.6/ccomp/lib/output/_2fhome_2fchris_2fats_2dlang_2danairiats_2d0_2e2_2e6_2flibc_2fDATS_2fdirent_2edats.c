/*
**
** The C code is generated by ATS/Anairiats
** The compilation time is: 2012-2-20: 21h:26m
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

#include "libc/sys/CATS/types.cats"

#include "libc/CATS/dirent.cats" // included after types.cats
/* external codes at top */
/* type definitions */
typedef struct {
ats_dirent_type atslab_0 ;
ats_ptr_type atslab_1 ;
} anairiats_sum_0 ;

typedef struct {
ats_ptr_type atslab_0 ;
ats_ptr_type atslab_1 ;
} anairiats_sum_1 ;

/* external typedefs */
/* external dynamic constructor declarations */
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__stream_vt_nil) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__stream_vt_cons) ;

/* external dynamic constant declarations */
ATSextern_fun(ats_bool_type, atspre_neq_int_int) (ats_int_type, ats_int_type) ;
ATSextern_val(ats_ptr_type, atspre_null_ptr) ;
ATSextern_fun(ats_bool_type, atspre_peq) (ats_ptr_type, ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, atspre_ptr_alloc_tsz) (ats_size_type) ;
ATSextern_fun(ats_void_type, atspre_ptr_free) (ats_ptr_type) ;
ATSextern_fun(ats_void_type, atslib_closedir_exn) (ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__dirent_stream_vt_make_DIR) (ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__direntptr_stream_vt_make_DIR) (ats_ptr_type) ;

/* external dynamic terminating constant declarations */
#ifdef _ATS_PROOFCHECK
#endif /* _ATS_PROOFCHECK */

/* sum constructor declarations */
/* exn constructor declarations */
/* global dynamic (non-functional) constant declarations */
/* internal function declarations */
static
ats_void_type f_1 (ats_ptr_type arg0, ats_ref_type arg1) ;
static
ats_ptr_type __ats_fun_2 (ats_ptr_type env0, ats_bool_type arg0) ;
static
ats_clo_ptr_type __ats_fun_2_closure_make (ats_ptr_type env0) ;
static
ats_ptr_type __ats_fun_2_clofun (ats_clo_ptr_type cloptr, ats_bool_type arg0) ;
static
ats_void_type f_4 (ats_ptr_type arg0, ats_ref_type arg1) ;
static
ats_ptr_type __ats_fun_5 (ats_ptr_type env0, ats_bool_type arg0) ;
static
ats_clo_ptr_type __ats_fun_5_closure_make (ats_ptr_type env0) ;
static
ats_ptr_type __ats_fun_5_clofun (ats_clo_ptr_type cloptr, ats_bool_type arg0) ;

/* partial value template declarations */
/* static temporary variable declarations */
/* external value variable declarations */

/* function implementations */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 1961(line=55, offs=6) -- 2600(line=74, offs=6)
*/
ATSstaticdec()
ats_void_type
f_1 (ats_ptr_type arg0, ats_ref_type arg1) {
/* local vardec */
// ATSlocal_void (tmp1) ;
ATSlocal (ats_ptr_type, tmp2) ;
ATSlocal (ats_ptr_type, tmp3) ;
ATSlocal (ats_ptr_type, tmp4) ;
ATSlocal (ats_ptr_type, tmp5) ;
ATSlocal (ats_int_type, tmp6) ;
ATSlocal (ats_bool_type, tmp7) ;
ATSlocal (ats_bool_type, tmp8) ;
// ATSlocal_void (tmp9) ;
ATSlocal (ats_ptr_type, tmp10) ;
ATSlocal (ats_ptr_type, tmp11) ;

__ats_lab_f_1:
/* ats_ptr_type tmp2 ; */
tmp3 = ATS_MALLOC(sizeof(anairiats_sum_0)) ;
ats_ptrget_mac(ats_ptr_type, arg1) = tmp3 ;
// if (ats_ptrget_mac(ats_ptr_type, arg1) == (ats_sum_ptr_type)0) { ats_deadcode_failure_handle () ; }
tmp4 = &ats_caselptr_mac(anairiats_sum_0, ats_ptrget_mac(ats_ptr_type, arg1), atslab_0) ;
tmp5 = &ats_caselptr_mac(anairiats_sum_0, ats_ptrget_mac(ats_ptr_type, arg1), atslab_1) ;
tmp6 = atslib_readdir_r (arg0, tmp4, (&tmp2)) ;
tmp8 = atspre_neq_int_int (tmp6, 0) ;
if (tmp8) {
tmp7 = ats_true_bool ;
} else {
tmp7 = atspre_peq (tmp2, atspre_null_ptr) ;
} /* end of [if] */
if (tmp7) {
/* tmp9 = */ atslib_closedir_exn (arg0) ;
ATS_FREE(ats_ptrget_mac(ats_ptr_type, arg1)) ;
tmp10 = (ats_sum_ptr_type)0 ;
ats_ptrget_mac(ats_ptr_type, arg1) = tmp10 ;
} else {
tmp11 = ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__dirent_stream_vt_make_DIR (arg0) ;
ats_ptrget_mac(ats_ptr_type, tmp5) = tmp11 ;
} /* end of [if] */
return /* (tmp1) */ ;
} /* end of [f_1] */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 1671(line=46, offs=35) -- 1834(line=51, offs=2)
*/
ATSstaticdec()
ats_ptr_type
__ats_fun_2 (ats_ptr_type env0, ats_bool_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp12) ;
ATSlocal (ats_ptr_type, tmp13) ;
// ATSlocal_void (tmp14) ;

__ats_lab___ats_fun_2:
if (arg0) {
/* ats_ptr_type tmp13 ; */
/* tmp14 = */ f_1 (env0, (&tmp13)) ;
tmp12 = tmp13 ;
} else {
/* tmp12 = */ atslib_closedir_exn (env0) ;
} /* end of [if] */
return (tmp12) ;
} /* end of [__ats_fun_2] */

typedef struct {
ats_fun_ptr_type closure_fun ;
ats_ptr_type closure_env_0 ;
} __ats_fun_2_closure_type ;

ats_ptr_type
__ats_fun_2_clofun (ats_clo_ptr_type cloptr, ats_bool_type arg0) {
return __ats_fun_2 (((__ats_fun_2_closure_type*)cloptr)->closure_env_0, arg0) ;
} /* end of function */

ATSinline()
ats_void_type
__ats_fun_2_closure_init (__ats_fun_2_closure_type *p_clo, ats_ptr_type env0) {
p_clo->closure_fun = (ats_fun_ptr_type)&__ats_fun_2_clofun ;
p_clo->closure_env_0 = env0 ;
return ;
} /* end of function */

ats_clo_ptr_type
__ats_fun_2_closure_make (ats_ptr_type env0) {
__ats_fun_2_closure_type *p_clo = ATS_MALLOC(sizeof(__ats_fun_2_closure_type)) ;
__ats_fun_2_closure_init (p_clo, env0) ;
return (ats_clo_ptr_type)p_clo ;
} /* end of function */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 1652(line=46, offs=16) -- 2616(line=75, offs=2)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__dirent_stream_vt_make_DIR (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp0) ;

__ats_lab_ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__dirent_stream_vt_make_DIR:
ats_instr_move_lazy_ldelay_mac (tmp0, ats_ptr_type, __ats_fun_2_closure_make (arg0)) ;
return (tmp0) ;
} /* end of [ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__dirent_stream_vt_make_DIR] */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 3040(line=90, offs=6) -- 3892(line=113, offs=6)
*/
ATSstaticdec()
ats_void_type
f_4 (ats_ptr_type arg0, ats_ref_type arg1) {
/* local vardec */
// ATSlocal_void (tmp16) ;
ATSlocal (ats_ptr_type, tmp17) ;
ATSlocal (ats_ptr_type, tmp18) ;
ATSlocal (ats_ptr_type, tmp19) ;
ATSlocal (ats_int_type, tmp20) ;
ATSlocal (ats_bool_type, tmp21) ;
ATSlocal (ats_bool_type, tmp22) ;
// ATSlocal_void (tmp23) ;
// ATSlocal_void (tmp24) ;
ATSlocal (ats_ptr_type, tmp25) ;
ATSlocal (ats_ptr_type, tmp26) ;
ATSlocal (ats_ptr_type, tmp27) ;
ATSlocal (ats_ptr_type, tmp28) ;
ATSlocal (ats_ptr_type, tmp29) ;

__ats_lab_f_4:
/* ats_ptr_type tmp17 ; */
tmp18 = atspre_ptr_alloc_tsz (sizeof(ats_dirent_type)) ;
tmp19 = ats_selsin_mac(tmp18, atslab_2) ;
tmp20 = atslib_readdir_r (arg0, tmp19, (&tmp17)) ;
tmp22 = atspre_neq_int_int (tmp20, 0) ;
if (tmp22) {
tmp21 = ats_true_bool ;
} else {
tmp21 = atspre_peq (tmp17, atspre_null_ptr) ;
} /* end of [if] */
if (tmp21) {
/* tmp23 = */ atspre_ptr_free (tmp19) ;
/* tmp24 = */ atslib_closedir_exn (arg0) ;
tmp25 = (ats_sum_ptr_type)0 ;
ats_ptrget_mac(ats_ptr_type, arg1) = tmp25 ;
} else {
tmp26 = tmp19 ;
tmp27 = ATS_MALLOC(sizeof(anairiats_sum_1)) ;
ats_selptrset_mac(anairiats_sum_1, tmp27, atslab_0, tmp26) ;
ats_ptrget_mac(ats_ptr_type, arg1) = tmp27 ;
// if (ats_ptrget_mac(ats_ptr_type, arg1) == (ats_sum_ptr_type)0) { ats_deadcode_failure_handle () ; }
tmp28 = &ats_caselptr_mac(anairiats_sum_1, ats_ptrget_mac(ats_ptr_type, arg1), atslab_1) ;
tmp29 = ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__direntptr_stream_vt_make_DIR (arg0) ;
ats_ptrget_mac(ats_ptr_type, tmp28) = tmp29 ;
} /* end of [if] */
return /* (tmp16) */ ;
} /* end of [f_4] */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 2750(line=81, offs=35) -- 2913(line=86, offs=2)
*/
ATSstaticdec()
ats_ptr_type
__ats_fun_5 (ats_ptr_type env0, ats_bool_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp30) ;
ATSlocal (ats_ptr_type, tmp31) ;
// ATSlocal_void (tmp32) ;

__ats_lab___ats_fun_5:
if (arg0) {
/* ats_ptr_type tmp31 ; */
/* tmp32 = */ f_4 (env0, (&tmp31)) ;
tmp30 = tmp31 ;
} else {
/* tmp30 = */ atslib_closedir_exn (env0) ;
} /* end of [if] */
return (tmp30) ;
} /* end of [__ats_fun_5] */

typedef struct {
ats_fun_ptr_type closure_fun ;
ats_ptr_type closure_env_0 ;
} __ats_fun_5_closure_type ;

ats_ptr_type
__ats_fun_5_clofun (ats_clo_ptr_type cloptr, ats_bool_type arg0) {
return __ats_fun_5 (((__ats_fun_5_closure_type*)cloptr)->closure_env_0, arg0) ;
} /* end of function */

ATSinline()
ats_void_type
__ats_fun_5_closure_init (__ats_fun_5_closure_type *p_clo, ats_ptr_type env0) {
p_clo->closure_fun = (ats_fun_ptr_type)&__ats_fun_5_clofun ;
p_clo->closure_env_0 = env0 ;
return ;
} /* end of function */

ats_clo_ptr_type
__ats_fun_5_closure_make (ats_ptr_type env0) {
__ats_fun_5_closure_type *p_clo = ATS_MALLOC(sizeof(__ats_fun_5_closure_type)) ;
__ats_fun_5_closure_init (p_clo, env0) ;
return (ats_clo_ptr_type)p_clo ;
} /* end of function */

/*
// /home/chris/ats-lang-anairiats-0.2.6/libc/DATS/dirent.dats: 2731(line=81, offs=16) -- 3911(line=114, offs=2)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__direntptr_stream_vt_make_DIR (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp15) ;

__ats_lab_ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__direntptr_stream_vt_make_DIR:
ats_instr_move_lazy_ldelay_mac (tmp15, ats_ptr_type, __ats_fun_5_closure_make (arg0)) ;
return (tmp15) ;
} /* end of [ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__direntptr_stream_vt_make_DIR] */

/* static load function */

extern ats_void_type ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__staload () {
static int ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__staload_flag) return ;
ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__staload_flag = 1 ;

ATS_2d0_2e2_2e6_2libc_2SATS_2dirent_2esats__staload () ;

return ;
} /* staload function */

/* dynamic load function */

// dynload flag declaration
// extern ats_int_type ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__dynload_flag ;

ats_void_type
ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__dynload () {
// ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__dynload_flag = 1 ;
ATS_2d0_2e2_2e6_2libc_2DATS_2dirent_2edats__staload () ;

#ifdef _ATS_PROOFCHECK
#endif /* _ATS_PROOFCHECK */

/* marking static variables for GC */

/* marking external values for GC */

/* code for dynamic loading */
return ;
} /* end of [dynload function] */

/* external codes at mid */
/* external codes at bot */

/* ****** ****** */

/* end of [/home/chris/ats-lang-anairiats-0.2.6/ccomp/lib/output/_2fhome_2fchris_2fats_2dlang_2danairiats_2d0_2e2_2e6_2flibc_2fDATS_2fdirent_2edats.c] */
