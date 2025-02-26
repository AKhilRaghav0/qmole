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
typedef struct {
int tag ;
ats_ptr_type atslab_0 ;
} anairiats_sum_0 ;

typedef struct {
ats_ptr_type atslab_0 ;
} anairiats_sum_1 ;

typedef struct {
int tag ;
ats_int_type atslab_0 ;
} anairiats_sum_2 ;

typedef struct {
ats_ptr_type atslab_c3str_loc ;
ats_ptr_type atslab_c3str_kind ;
ats_ptr_type atslab_c3str_node ;
} anairiats_rec_3 ;

typedef struct {
ats_ptr_type atslab_h3ypo_loc ;
ats_ptr_type atslab_h3ypo_node ;
} anairiats_rec_4 ;

typedef struct {
int tag ;
ats_ptr_type atslab_0 ;
ats_ptr_type atslab_1 ;
} anairiats_sum_5 ;

/* external typedefs */
/* external dynamic constructor declarations */
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__None) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__Some) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDnone) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDmetric_nat) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDmetric_dec) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDpattern_match_exhaustiveness) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDvarfin) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRKINDloop) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__H3YPOprop) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__H3YPObind) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__H3YPOeqeq) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRprop) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__C3STRitmlst) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMcstr) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMcstr_ref) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMdisj) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMhypo) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMsvar) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__S3ITEMsVar) ;

/* external dynamic constant declarations */
ATSextern_fun(ats_ptr_type, atspre_stdout_get) () ;
ATSextern_fun(ats_void_type, atspre_stdout_view_set) () ;
ATSextern_fun(ats_ptr_type, atspre_stderr_get) () ;
ATSextern_fun(ats_void_type, atspre_stderr_view_set) () ;
ATSextern_fun(ats_void_type, atspre_fprint_int) (ats_ref_type, ats_int_type) ;
ATSextern_fun(ats_void_type, atspre_fprint_string) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_list_2esats__fprintlst) (ats_ref_type, ats_ptr_type, ats_ptr_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2var) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__s2Var_get_link) (ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2Var) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3item) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo) (ats_ref_type, ats_ptr_type) ;

/* external dynamic terminating constant declarations */
#ifdef _ATS_PROOFCHECK
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2basics_dyn_2esats__file_mode_lte_w_w_prfck () ;
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
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 1677(line=47, offs=15) -- 2805(line=80, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3item (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp0) ;
ATSlocal (ats_ptr_type, tmp1) ;
// ATSlocal_void (tmp2) ;
// ATSlocal_void (tmp3) ;
ATSlocal (ats_ptr_type, tmp4) ;
// ATSlocal_void (tmp5) ;
// ATSlocal_void (tmp6) ;
ATSlocal (ats_ptr_type, tmp7) ;
// ATSlocal_void (tmp8) ;
// ATSlocal_void (tmp9) ;
ATSlocal (ats_ptr_type, tmp10) ;
// ATSlocal_void (tmp11) ;
// ATSlocal_void (tmp12) ;
// ATSlocal_void (tmp13) ;
ATSlocal (ats_ptr_type, tmp14) ;
ATSlocal (ats_ptr_type, tmp15) ;
// ATSlocal_void (tmp16) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3item:
#line 50 "ats_trans3_env_print.dats"
do {
/* branch: __ats_lab_0 */
#line 51 "ats_trans3_env_print.dats"
__ats_lab_0_0:
#line 51 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 0) { goto __ats_lab_1_0 ; }
#line 51 "ats_trans3_env_print.dats"
__ats_lab_0_1:
#line 51 "ats_trans3_env_print.dats"
tmp1 = ats_caselptr_mac(anairiats_sum_0, arg1, atslab_0) ;
#line 52 "ats_trans3_env_print.dats"
/* tmp2 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMcstr(")) ;
#line 52 "ats_trans3_env_print.dats"
/* tmp3 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str (arg0, tmp1) ;
#line 52 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_1 */
#line 54 "ats_trans3_env_print.dats"
__ats_lab_1_0:
#line 54 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 1) { goto __ats_lab_2_0 ; }
#line 54 "ats_trans3_env_print.dats"
__ats_lab_1_1:
#line 55 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMcstr_ref(...)")) ;
break ;

/* branch: __ats_lab_2 */
#line 57 "ats_trans3_env_print.dats"
__ats_lab_2_0:
#line 57 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 2) { goto __ats_lab_3_0 ; }
#line 57 "ats_trans3_env_print.dats"
__ats_lab_2_1:
#line 58 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMdisj(...)")) ;
break ;

/* branch: __ats_lab_3 */
#line 60 "ats_trans3_env_print.dats"
__ats_lab_3_0:
#line 60 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 3) { goto __ats_lab_4_0 ; }
#line 60 "ats_trans3_env_print.dats"
__ats_lab_3_1:
#line 60 "ats_trans3_env_print.dats"
tmp4 = ats_caselptr_mac(anairiats_sum_0, arg1, atslab_0) ;
#line 61 "ats_trans3_env_print.dats"
/* tmp5 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMhypo(")) ;
#line 61 "ats_trans3_env_print.dats"
/* tmp6 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo (arg0, tmp4) ;
#line 61 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_4 */
#line 63 "ats_trans3_env_print.dats"
__ats_lab_4_0:
#line 63 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 4) { goto __ats_lab_5_0 ; }
#line 63 "ats_trans3_env_print.dats"
__ats_lab_4_1:
#line 63 "ats_trans3_env_print.dats"
tmp7 = ats_caselptr_mac(anairiats_sum_0, arg1, atslab_0) ;
#line 64 "ats_trans3_env_print.dats"
/* tmp8 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMsvar(")) ;
#line 64 "ats_trans3_env_print.dats"
/* tmp9 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2var (arg0, tmp7) ;
#line 64 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_5 */
#line 66 "ats_trans3_env_print.dats"
__ats_lab_5_0:
#line 66 "ats_trans3_env_print.dats"
// if (((ats_sum_ptr_type)arg1)->tag != 5) { ats_deadcode_failure_handle () ; }
#line 66 "ats_trans3_env_print.dats"
__ats_lab_5_1:
#line 66 "ats_trans3_env_print.dats"
tmp10 = ats_caselptr_mac(anairiats_sum_0, arg1, atslab_0) ;
#line 67 "ats_trans3_env_print.dats"
/* tmp11 = */ atspre_fprint_string (arg0, ATSstrcst("S3ITEMsVar(")) ;
#line 68 "ats_trans3_env_print.dats"
/* tmp12 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2Var (arg0, tmp10) ;
#line 70 "ats_trans3_env_print.dats"
tmp14 = ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__s2Var_get_link (tmp10) ;
#line 70 "ats_trans3_env_print.dats"
do {
/* branch: __ats_lab_6 */
#line 71 "ats_trans3_env_print.dats"
__ats_lab_6_0:
#line 71 "ats_trans3_env_print.dats"
if (tmp14 == (ats_sum_ptr_type)0) { goto __ats_lab_7_0 ; }
#line 71 "ats_trans3_env_print.dats"
__ats_lab_6_1:
#line 71 "ats_trans3_env_print.dats"
tmp15 = ats_caselptr_mac(anairiats_sum_1, tmp14, atslab_0) ;
#line 72 "ats_trans3_env_print.dats"
/* tmp16 = */ atspre_fprint_string (arg0, ATSstrcst("= ")) ;
#line 72 "ats_trans3_env_print.dats"
/* tmp13 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp15) ;
break ;

/* branch: __ats_lab_7 */
#line 74 "ats_trans3_env_print.dats"
__ats_lab_7_0:
#line 74 "ats_trans3_env_print.dats"
// if (tmp14 != (ats_sum_ptr_type)0) { ats_deadcode_failure_handle () ; }
#line 74 "ats_trans3_env_print.dats"
__ats_lab_7_1:
break ;
} while (0) ;
#line 76 "ats_trans3_env_print.dats"
/* tmp0 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;
} while (0) ;
return /* (tmp0) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3item] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 2881(line=85, offs=18) -- 2950(line=86, offs=53)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp17) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst:
#line 86 "ats_trans3_env_print.dats"
/* tmp17 = */ ATS_2d0_2e2_2e6_2src_2ats_list_2esats__fprintlst (arg0, arg1, ATSstrcst(", "), &ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3item) ;
return /* (tmp17) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3007(line=89, offs=27) -- 3050(line=89, offs=70)
*/
ATSglobaldec()
ats_void_type
atsopt_print_s3itemlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp18) ;
ATSlocal (ats_ptr_type, tmp19) ;
ATSlocal (ats_ptr_type, tmp20) ;
// ATSlocal_void (tmp21) ;

__ats_lab_atsopt_print_s3itemlst:
#line 89 "ats_trans3_env_print.dats"
tmp19 = atspre_stdout_get () ;
#line 89 "ats_trans3_env_print.dats"
tmp20 = ats_selsin_mac(tmp19, atslab_1) ;
#line 89 "ats_trans3_env_print.dats"
/* tmp21 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst (tmp20, arg0) ;
#line 89 "ats_trans3_env_print.dats"
/* tmp18 = */ atspre_stdout_view_set () ;
return /* (tmp18) */ ;
} /* end of [atsopt_print_s3itemlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3077(line=90, offs=27) -- 3120(line=90, offs=70)
*/
ATSglobaldec()
ats_void_type
atsopt_prerr_s3itemlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp22) ;
ATSlocal (ats_ptr_type, tmp23) ;
ATSlocal (ats_ptr_type, tmp24) ;
// ATSlocal_void (tmp25) ;

__ats_lab_atsopt_prerr_s3itemlst:
#line 90 "ats_trans3_env_print.dats"
tmp23 = atspre_stderr_get () ;
#line 90 "ats_trans3_env_print.dats"
tmp24 = ats_selsin_mac(tmp23, atslab_1) ;
#line 90 "ats_trans3_env_print.dats"
/* tmp25 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst (tmp24, arg0) ;
#line 90 "ats_trans3_env_print.dats"
/* tmp22 = */ atspre_stderr_view_set () ;
return /* (tmp22) */ ;
} /* end of [atsopt_prerr_s3itemlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3173(line=95, offs=21) -- 3247(line=96, offs=57)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp26) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst:
#line 96 "ats_trans3_env_print.dats"
/* tmp26 = */ ATS_2d0_2e2_2e6_2src_2ats_list_2esats__fprintlst (arg0, arg1, ATSstrcst("; "), &ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst) ;
return /* (tmp26) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3310(line=99, offs=30) -- 3358(line=99, offs=78)
*/
ATSglobaldec()
ats_void_type
atsopt_print_s3itemlstlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp27) ;
ATSlocal (ats_ptr_type, tmp28) ;
ATSlocal (ats_ptr_type, tmp29) ;
// ATSlocal_void (tmp30) ;

__ats_lab_atsopt_print_s3itemlstlst:
#line 99 "ats_trans3_env_print.dats"
tmp28 = atspre_stdout_get () ;
#line 99 "ats_trans3_env_print.dats"
tmp29 = ats_selsin_mac(tmp28, atslab_1) ;
#line 99 "ats_trans3_env_print.dats"
/* tmp30 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst (tmp29, arg0) ;
#line 99 "ats_trans3_env_print.dats"
/* tmp27 = */ atspre_stdout_view_set () ;
return /* (tmp27) */ ;
} /* end of [atsopt_print_s3itemlstlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3388(line=100, offs=30) -- 3436(line=100, offs=78)
*/
ATSglobaldec()
ats_void_type
atsopt_prerr_s3itemlstlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp31) ;
ATSlocal (ats_ptr_type, tmp32) ;
ATSlocal (ats_ptr_type, tmp33) ;
// ATSlocal_void (tmp34) ;

__ats_lab_atsopt_prerr_s3itemlstlst:
#line 100 "ats_trans3_env_print.dats"
tmp32 = atspre_stderr_get () ;
#line 100 "ats_trans3_env_print.dats"
tmp33 = ats_selsin_mac(tmp32, atslab_1) ;
#line 100 "ats_trans3_env_print.dats"
/* tmp34 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlstlst (tmp33, arg0) ;
#line 100 "ats_trans3_env_print.dats"
/* tmp31 = */ atspre_stderr_view_set () ;
return /* (tmp31) */ ;
} /* end of [atsopt_prerr_s3itemlstlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 3486(line=105, offs=18) -- 3950(line=117, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp35) ;
ATSlocal (ats_int_type, tmp36) ;
// ATSlocal_void (tmp37) ;
// ATSlocal_void (tmp38) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind:
#line 108 "ats_trans3_env_print.dats"
do {
/* branch: __ats_lab_8 */
#line 109 "ats_trans3_env_print.dats"
__ats_lab_8_0:
#line 109 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 0) { goto __ats_lab_9_0 ; }
#line 109 "ats_trans3_env_print.dats"
__ats_lab_8_1:
#line 109 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst("none")) ;
break ;

/* branch: __ats_lab_9 */
#line 110 "ats_trans3_env_print.dats"
__ats_lab_9_0:
#line 110 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 1) { goto __ats_lab_10_0 ; }
#line 110 "ats_trans3_env_print.dats"
__ats_lab_9_1:
#line 110 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst("metric_nat")) ;
break ;

/* branch: __ats_lab_10 */
#line 111 "ats_trans3_env_print.dats"
__ats_lab_10_0:
#line 111 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 2) { goto __ats_lab_11_0 ; }
#line 111 "ats_trans3_env_print.dats"
__ats_lab_10_1:
#line 111 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst("metric_dec")) ;
break ;

/* branch: __ats_lab_11 */
#line 112 "ats_trans3_env_print.dats"
__ats_lab_11_0:
#line 112 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 3) { goto __ats_lab_12_0 ; }
#line 112 "ats_trans3_env_print.dats"
__ats_lab_11_1:
#line 112 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst("pattern")) ;
break ;

/* branch: __ats_lab_12 */
#line 113 "ats_trans3_env_print.dats"
__ats_lab_12_0:
#line 113 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)arg1)->tag != 4) { goto __ats_lab_13_0 ; }
#line 113 "ats_trans3_env_print.dats"
__ats_lab_12_1:
#line 113 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst("varfin")) ;
break ;

/* branch: __ats_lab_13 */
#line 114 "ats_trans3_env_print.dats"
__ats_lab_13_0:
#line 114 "ats_trans3_env_print.dats"
// if (((ats_sum_ptr_type)arg1)->tag != 5) { ats_deadcode_failure_handle () ; }
#line 114 "ats_trans3_env_print.dats"
__ats_lab_13_1:
#line 114 "ats_trans3_env_print.dats"
tmp36 = ats_caselptr_mac(anairiats_sum_2, arg1, atslab_0) ;
#line 115 "ats_trans3_env_print.dats"
/* tmp37 = */ atspre_fprint_string (arg0, ATSstrcst("loop(")) ;
#line 115 "ats_trans3_env_print.dats"
/* tmp38 = */ atspre_fprint_int (arg0, tmp36) ;
#line 115 "ats_trans3_env_print.dats"
/* tmp35 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;
} while (0) ;
return /* (tmp35) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 4025(line=121, offs=24) -- 4552(line=139, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp39) ;
ATSlocal (ats_ptr_type, tmp40) ;
ATSlocal (ats_ptr_type, tmp41) ;
// ATSlocal_void (tmp42) ;
// ATSlocal_void (tmp43) ;
ATSlocal (ats_ptr_type, tmp44) ;
// ATSlocal_void (tmp45) ;
// ATSlocal_void (tmp46) ;
ATSlocal (ats_ptr_type, tmp47) ;
// ATSlocal_void (tmp48) ;
// ATSlocal_void (tmp49) ;
ATSlocal (ats_ptr_type, tmp50) ;
// ATSlocal_void (tmp51) ;
// ATSlocal_void (tmp52) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str:
#line 124 "ats_trans3_env_print.dats"
tmp40 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_3, arg1), atslab_c3str_node) ;
#line 124 "ats_trans3_env_print.dats"
do {
/* branch: __ats_lab_14 */
#line 125 "ats_trans3_env_print.dats"
__ats_lab_14_0:
#line 125 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)tmp40)->tag != 0) { goto __ats_lab_15_0 ; }
#line 125 "ats_trans3_env_print.dats"
__ats_lab_14_1:
#line 125 "ats_trans3_env_print.dats"
tmp41 = ats_caselptr_mac(anairiats_sum_0, tmp40, atslab_0) ;
#line 126 "ats_trans3_env_print.dats"
/* tmp42 = */ atspre_fprint_string (arg0, ATSstrcst("C3STRprop(")) ;
#line 127 "ats_trans3_env_print.dats"
tmp44 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_3, arg1), atslab_c3str_kind) ;
#line 127 "ats_trans3_env_print.dats"
/* tmp43 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind (arg0, tmp44) ;
#line 128 "ats_trans3_env_print.dats"
/* tmp45 = */ atspre_fprint_string (arg0, ATSstrcst("; ")) ;
#line 129 "ats_trans3_env_print.dats"
/* tmp46 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp41) ;
#line 130 "ats_trans3_env_print.dats"
/* tmp39 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_15 */
#line 132 "ats_trans3_env_print.dats"
__ats_lab_15_0:
#line 132 "ats_trans3_env_print.dats"
// if (((ats_sum_ptr_type)tmp40)->tag != 1) { ats_deadcode_failure_handle () ; }
#line 132 "ats_trans3_env_print.dats"
__ats_lab_15_1:
#line 132 "ats_trans3_env_print.dats"
tmp47 = ats_caselptr_mac(anairiats_sum_0, tmp40, atslab_0) ;
#line 133 "ats_trans3_env_print.dats"
/* tmp48 = */ atspre_fprint_string (arg0, ATSstrcst("C3STRitmlst(")) ;
#line 134 "ats_trans3_env_print.dats"
tmp50 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_3, arg1), atslab_c3str_kind) ;
#line 134 "ats_trans3_env_print.dats"
/* tmp49 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3strkind (arg0, tmp50) ;
#line 135 "ats_trans3_env_print.dats"
/* tmp51 = */ atspre_fprint_string (arg0, ATSstrcst("; ")) ;
#line 136 "ats_trans3_env_print.dats"
/* tmp52 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_s3itemlst (arg0, tmp47) ;
#line 137 "ats_trans3_env_print.dats"
/* tmp39 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;
} while (0) ;
return /* (tmp39) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 4601(line=141, offs=23) -- 4638(line=141, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_c3str (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp53) ;
ATSlocal (ats_ptr_type, tmp54) ;
ATSlocal (ats_ptr_type, tmp55) ;
// ATSlocal_void (tmp56) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_c3str:
#line 141 "ats_trans3_env_print.dats"
tmp54 = atspre_stdout_get () ;
#line 141 "ats_trans3_env_print.dats"
tmp55 = ats_selsin_mac(tmp54, atslab_1) ;
#line 141 "ats_trans3_env_print.dats"
/* tmp56 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str (tmp55, arg0) ;
#line 141 "ats_trans3_env_print.dats"
/* tmp53 = */ atspre_stdout_view_set () ;
return /* (tmp53) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_c3str] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 4661(line=142, offs=23) -- 4698(line=142, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_c3str (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp57) ;
ATSlocal (ats_ptr_type, tmp58) ;
ATSlocal (ats_ptr_type, tmp59) ;
// ATSlocal_void (tmp60) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_c3str:
#line 142 "ats_trans3_env_print.dats"
tmp58 = atspre_stderr_get () ;
#line 142 "ats_trans3_env_print.dats"
tmp59 = ats_selsin_mac(tmp58, atslab_1) ;
#line 142 "ats_trans3_env_print.dats"
/* tmp60 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_c3str (tmp59, arg0) ;
#line 142 "ats_trans3_env_print.dats"
/* tmp57 = */ atspre_stderr_view_set () ;
return /* (tmp57) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_c3str] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 4744(line=146, offs=24) -- 5370(line=167, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp61) ;
ATSlocal (ats_ptr_type, tmp62) ;
ATSlocal (ats_ptr_type, tmp63) ;
// ATSlocal_void (tmp64) ;
// ATSlocal_void (tmp65) ;
ATSlocal (ats_ptr_type, tmp66) ;
ATSlocal (ats_ptr_type, tmp67) ;
// ATSlocal_void (tmp68) ;
// ATSlocal_void (tmp69) ;
// ATSlocal_void (tmp70) ;
// ATSlocal_void (tmp71) ;
ATSlocal (ats_ptr_type, tmp72) ;
ATSlocal (ats_ptr_type, tmp73) ;
// ATSlocal_void (tmp74) ;
// ATSlocal_void (tmp75) ;
// ATSlocal_void (tmp76) ;
// ATSlocal_void (tmp77) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo:
#line 149 "ats_trans3_env_print.dats"
tmp62 = ats_selbox_mac(ats_castptr_mac(anairiats_rec_4, arg1), atslab_h3ypo_node) ;
#line 149 "ats_trans3_env_print.dats"
do {
/* branch: __ats_lab_16 */
#line 150 "ats_trans3_env_print.dats"
__ats_lab_16_0:
#line 150 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)tmp62)->tag != 0) { goto __ats_lab_17_0 ; }
#line 150 "ats_trans3_env_print.dats"
__ats_lab_16_1:
#line 150 "ats_trans3_env_print.dats"
tmp63 = ats_caselptr_mac(anairiats_sum_0, tmp62, atslab_0) ;
#line 151 "ats_trans3_env_print.dats"
/* tmp64 = */ atspre_fprint_string (arg0, ATSstrcst("H2YPOprop(")) ;
#line 151 "ats_trans3_env_print.dats"
/* tmp65 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp63) ;
#line 151 "ats_trans3_env_print.dats"
/* tmp61 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_17 */
#line 153 "ats_trans3_env_print.dats"
__ats_lab_17_0:
#line 153 "ats_trans3_env_print.dats"
if (((ats_sum_ptr_type)tmp62)->tag != 1) { goto __ats_lab_18_0 ; }
#line 153 "ats_trans3_env_print.dats"
__ats_lab_17_1:
#line 153 "ats_trans3_env_print.dats"
tmp66 = ats_caselptr_mac(anairiats_sum_5, tmp62, atslab_0) ;
#line 153 "ats_trans3_env_print.dats"
tmp67 = ats_caselptr_mac(anairiats_sum_5, tmp62, atslab_1) ;
#line 154 "ats_trans3_env_print.dats"
/* tmp68 = */ atspre_fprint_string (arg0, ATSstrcst("H2YPObind(")) ;
#line 155 "ats_trans3_env_print.dats"
/* tmp69 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2var (arg0, tmp66) ;
#line 156 "ats_trans3_env_print.dats"
/* tmp70 = */ atspre_fprint_string (arg0, ATSstrcst(", ")) ;
#line 157 "ats_trans3_env_print.dats"
/* tmp71 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp67) ;
#line 158 "ats_trans3_env_print.dats"
/* tmp61 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;

/* branch: __ats_lab_18 */
#line 160 "ats_trans3_env_print.dats"
__ats_lab_18_0:
#line 160 "ats_trans3_env_print.dats"
// if (((ats_sum_ptr_type)tmp62)->tag != 2) { ats_deadcode_failure_handle () ; }
#line 160 "ats_trans3_env_print.dats"
__ats_lab_18_1:
#line 160 "ats_trans3_env_print.dats"
tmp72 = ats_caselptr_mac(anairiats_sum_5, tmp62, atslab_0) ;
#line 160 "ats_trans3_env_print.dats"
tmp73 = ats_caselptr_mac(anairiats_sum_5, tmp62, atslab_1) ;
#line 161 "ats_trans3_env_print.dats"
/* tmp74 = */ atspre_fprint_string (arg0, ATSstrcst("H2YPOeqeq(")) ;
#line 162 "ats_trans3_env_print.dats"
/* tmp75 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp72) ;
#line 163 "ats_trans3_env_print.dats"
/* tmp76 = */ atspre_fprint_string (arg0, ATSstrcst(", ")) ;
#line 164 "ats_trans3_env_print.dats"
/* tmp77 = */ ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__fprint_s2exp (arg0, tmp73) ;
#line 165 "ats_trans3_env_print.dats"
/* tmp61 = */ atspre_fprint_string (arg0, ATSstrcst(")")) ;
break ;
} while (0) ;
return /* (tmp61) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 5419(line=169, offs=23) -- 5456(line=169, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_h3ypo (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp78) ;
ATSlocal (ats_ptr_type, tmp79) ;
ATSlocal (ats_ptr_type, tmp80) ;
// ATSlocal_void (tmp81) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_h3ypo:
#line 169 "ats_trans3_env_print.dats"
tmp79 = atspre_stdout_get () ;
#line 169 "ats_trans3_env_print.dats"
tmp80 = ats_selsin_mac(tmp79, atslab_1) ;
#line 169 "ats_trans3_env_print.dats"
/* tmp81 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo (tmp80, arg0) ;
#line 169 "ats_trans3_env_print.dats"
/* tmp78 = */ atspre_stdout_view_set () ;
return /* (tmp78) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__print_h3ypo] */

/*
// /home/hwxi/Research/Anairiats/src/ats_trans3_env_print.dats: 5479(line=170, offs=23) -- 5516(line=170, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_h3ypo (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp82) ;
ATSlocal (ats_ptr_type, tmp83) ;
ATSlocal (ats_ptr_type, tmp84) ;
// ATSlocal_void (tmp85) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_h3ypo:
#line 170 "ats_trans3_env_print.dats"
tmp83 = atspre_stderr_get () ;
#line 170 "ats_trans3_env_print.dats"
tmp84 = ats_selsin_mac(tmp83, atslab_1) ;
#line 170 "ats_trans3_env_print.dats"
/* tmp85 = */ ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__fprint_h3ypo (tmp84, arg0) ;
#line 170 "ats_trans3_env_print.dats"
/* tmp82 = */ atspre_stderr_view_set () ;
return /* (tmp82) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__prerr_h3ypo] */

/* static load function */

extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_list_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__staload () {
static int ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__staload_flag) return ;
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__staload_flag = 1 ;

ATS_2d0_2e2_2e6_2src_2ats_list_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_2esats__staload () ;

return ;
} /* staload function */

/* dynamic load function */

// dynload flag declaration
extern ats_int_type ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__dynload_flag ;

ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__dynload () {
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__dynload_flag = 1 ;
ATS_2d0_2e2_2e6_2src_2ats_trans3_env_print_2edats__staload () ;

#ifdef _ATS_PROOFCHECK
ATS_2d0_2e2_2e6_2prelude_2basics_dyn_2esats__file_mode_lte_w_w_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* marking static variables for GC */

/* marking external values for GC */

/* code for dynamic loading */
return ;
} /* end of [dynload function] */

/* external codes at mid */
/* external codes at bot */

/* ****** ****** */

/* end of [ats_trans3_env_print_dats.c] */
