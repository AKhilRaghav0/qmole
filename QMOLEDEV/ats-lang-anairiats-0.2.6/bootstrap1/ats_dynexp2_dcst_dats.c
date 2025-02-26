/*
**
** The C code is generated by ATS/Anairiats
** The compilation time is: 2011-10-15: 19h:51m
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
#line 37 "ats_dynexp2_dcst.dats"

#include "ats_counter.cats" /* only needed for [ATS/Geizella] */

/* type definitions */
typedef struct {
ats_ptr_type atslab_d2cst_loc ;
ats_ptr_type atslab_d2cst_fil ;
ats_ptr_type atslab_d2cst_sym ;
ats_ptr_type atslab_d2cst_kind ;
ats_ptr_type atslab_d2cst_decarg ;
ats_ptr_type atslab_d2cst_arilst ;
ats_ptr_type atslab_d2cst_typ ;
ats_ptr_type atslab_d2cst_extdef ;
ats_ptr_type atslab_d2cst_def ;
atsopt_count_type atslab_d2cst_stamp ;
ats_ptr_type atslab_d2cst_hityp ;
} anairiats_rec_0 ;

/* external typedefs */
typedef anairiats_rec_0 d2cst_struct ;

/* external dynamic constructor declarations */
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__list_cons) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__list_nil) ;
ATSextern_val(ats_sum_type, ATS_2d0_2e2_2e6_2prelude_2basics_sta_2esats__None) ;

/* external dynamic constant declarations */
ATSextern_fun(ats_void_type, atspre_vbox_make_view_ptr) (ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, atspre_stdout_get) () ;
ATSextern_fun(ats_void_type, atspre_stdout_view_set) () ;
ATSextern_fun(ats_ptr_type, atspre_stderr_get) () ;
ATSextern_fun(ats_void_type, atspre_stderr_view_set) () ;
ATSextern_fun(ats_bool_type, atspre_ilt) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_bool_type, atspre_ilte) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_bool_type, atspre_ieq) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_bool_type, atspre_ineq) (ats_int_type, ats_int_type) ;
ATSextern_fun(ats_ptr_type, atspre_ptr_alloc_tsz) (ats_size_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_list_2esats__fprintlst) (ats_ref_type, ats_ptr_type, ats_ptr_type, ats_ptr_type) ;
ATSextern_fun(ats_int_type, ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__compare_stamp_stamp) (atsopt_count_type, atsopt_count_type) ;
ATSextern_fun(atsopt_count_type, ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__d2cst_stamp_make) () ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_symbol_2esats__fprint_symbol) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_fun) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_castfn) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_praxi) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_prfun) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_prval) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_proof) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstextdef_is_mac) (ats_ptr_type) ;
ATSextern_fun(ats_bool_type, ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstextdef_is_sta) (ats_ptr_type) ;
ATSextern_fun(ats_ptr_type, ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_sym) (ats_ptr_type) ;
ATSextern_fun(ats_int_type, ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst) (ats_ptr_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst) (ats_ref_type, ats_ptr_type) ;
ATSextern_fun(ats_void_type, ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst) (ats_ref_type, ats_ptr_type) ;

/* external dynamic terminating constant declarations */
#ifdef _ATS_PROOFCHECK
extern
ats_void_type ATS_2d0_2e2_2e6_2prelude_2basics_dyn_2esats__file_mode_lte_w_w_prfck () ;
#endif /* _ATS_PROOFCHECK */

/* sum constructor declarations */
/* exn constructor declarations */
/* global dynamic (non-functional) constant declarations */
/* internal function declarations */
static
ats_int_type _compare_d2cst_d2cst_18 (ats_ptr_type arg0, ats_ptr_type arg1) ;

/* partial value template declarations */
/* static temporary variable declarations */
/* external value variable declarations */

/* function implementations */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 2494(line=85, offs=3) -- 3078(line=111, offs=4)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_make (ats_ptr_type arg0, ats_ptr_type arg1, ats_ptr_type arg2, ats_ptr_type arg3, ats_ptr_type arg4, ats_ptr_type arg5, ats_ptr_type arg6, ats_ptr_type arg7) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp0) ;
ATSlocal (atsopt_count_type, tmp1) ;
ATSlocal (ats_ptr_type, tmp2) ;
ATSlocal (ats_ptr_type, tmp3) ;
ATSlocal (ats_ptr_type, tmp4) ;
ATSlocal (ats_ptr_type, tmp5) ;
// ATSlocal_void (tmp6) ;
// ATSlocal_void (tmp7) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_make:
#line 87 "ats_dynexp2_dcst.dats"
tmp1 = ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__d2cst_stamp_make () ;
#line 88 "ats_dynexp2_dcst.dats"
tmp2 = atspre_ptr_alloc_tsz (sizeof(anairiats_rec_0)) ;
#line 88 "ats_dynexp2_dcst.dats"
tmp3 = ats_selsin_mac(tmp2, atslab_2) ;
#line 92 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_loc) = arg0 ;
#line 93 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_fil) = arg1 ;
#line 94 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_sym) = arg2 ;
#line 95 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_kind) = arg3 ;
#line 96 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_decarg) = arg4 ;
#line 97 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_arilst) = arg5 ;
#line 98 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_typ) = arg6 ;
#line 99 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_extdef) = arg7 ;
#line 100 "ats_dynexp2_dcst.dats"
tmp4 = (ats_sum_ptr_type)0 ;
#line 100 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_def) = tmp4 ;
#line 101 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_stamp) = tmp1 ;
#line 102 "ats_dynexp2_dcst.dats"
tmp5 = (ats_sum_ptr_type)0 ;
#line 102 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp3), atslab_d2cst_hityp) = tmp5 ;
#line 105 "ats_dynexp2_dcst.dats"
/* tmp6 = */ atspre_vbox_make_view_ptr (tmp3) ;
#line 105 "ats_dynexp2_dcst.dats"
/* tmp7 = ats_selsin_mac(tmp6, atslab_1) */ ;
#line 109 "ats_dynexp2_dcst.dats"
tmp0 = tmp3 ;
return (tmp0) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_make] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3127(line=113, offs=25) -- 3184(line=114, offs=50)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_loc (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp8) ;
ATSlocal (ats_ptr_type, tmp9) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_loc:
#line 114 "ats_dynexp2_dcst.dats"
tmp9 = ats_selsin_mac(arg0, atslab_1) ;
#line 114 "ats_dynexp2_dcst.dats"
tmp8 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp9), atslab_d2cst_loc) ;
return (tmp8) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_loc] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3210(line=116, offs=25) -- 3267(line=117, offs=50)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_fil (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp10) ;
ATSlocal (ats_ptr_type, tmp11) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_fil:
#line 117 "ats_dynexp2_dcst.dats"
tmp11 = ats_selsin_mac(arg0, atslab_1) ;
#line 117 "ats_dynexp2_dcst.dats"
tmp10 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp11), atslab_d2cst_fil) ;
return (tmp10) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_fil] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3293(line=119, offs=25) -- 3350(line=120, offs=50)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_sym (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp12) ;
ATSlocal (ats_ptr_type, tmp13) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_sym:
#line 120 "ats_dynexp2_dcst.dats"
tmp13 = ats_selsin_mac(arg0, atslab_1) ;
#line 120 "ats_dynexp2_dcst.dats"
tmp12 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp13), atslab_d2cst_sym) ;
return (tmp12) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_sym] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3377(line=122, offs=26) -- 3435(line=123, offs=51)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_kind (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp14) ;
ATSlocal (ats_ptr_type, tmp15) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_kind:
#line 123 "ats_dynexp2_dcst.dats"
tmp15 = ats_selsin_mac(arg0, atslab_1) ;
#line 123 "ats_dynexp2_dcst.dats"
tmp14 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp15), atslab_d2cst_kind) ;
return (tmp14) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_kind] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3464(line=125, offs=28) -- 3524(line=126, offs=53)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_arilst (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp16) ;
ATSlocal (ats_ptr_type, tmp17) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_arilst:
#line 126 "ats_dynexp2_dcst.dats"
tmp17 = ats_selsin_mac(arg0, atslab_1) ;
#line 126 "ats_dynexp2_dcst.dats"
tmp16 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp17), atslab_d2cst_arilst) ;
return (tmp16) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_arilst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3553(line=128, offs=28) -- 3613(line=129, offs=53)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_decarg (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp18) ;
ATSlocal (ats_ptr_type, tmp19) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_decarg:
#line 129 "ats_dynexp2_dcst.dats"
tmp19 = ats_selsin_mac(arg0, atslab_1) ;
#line 129 "ats_dynexp2_dcst.dats"
tmp18 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp19), atslab_d2cst_decarg) ;
return (tmp18) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_decarg] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3642(line=131, offs=28) -- 3720(line=132, offs=63)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_decarg (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp20) ;
ATSlocal (ats_ptr_type, tmp21) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_decarg:
#line 132 "ats_dynexp2_dcst.dats"
tmp21 = ats_selsin_mac(arg0, atslab_1) ;
#line 132 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp21), atslab_d2cst_decarg) = arg1 ;
return /* (tmp20) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_decarg] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3746(line=134, offs=25) -- 3803(line=135, offs=50)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_typ (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp22) ;
ATSlocal (ats_ptr_type, tmp23) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_typ:
#line 135 "ats_dynexp2_dcst.dats"
tmp23 = ats_selsin_mac(arg0, atslab_1) ;
#line 135 "ats_dynexp2_dcst.dats"
tmp22 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp23), atslab_d2cst_typ) ;
return (tmp22) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_typ] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3832(line=137, offs=28) -- 3892(line=138, offs=53)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_extdef (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp24) ;
ATSlocal (ats_ptr_type, tmp25) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_extdef:
#line 138 "ats_dynexp2_dcst.dats"
tmp25 = ats_selsin_mac(arg0, atslab_1) ;
#line 138 "ats_dynexp2_dcst.dats"
tmp24 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp25), atslab_d2cst_extdef) ;
return (tmp24) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_extdef] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 3918(line=140, offs=25) -- 3975(line=141, offs=50)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_def (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp26) ;
ATSlocal (ats_ptr_type, tmp27) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_def:
#line 141 "ats_dynexp2_dcst.dats"
tmp27 = ats_selsin_mac(arg0, atslab_1) ;
#line 141 "ats_dynexp2_dcst.dats"
tmp26 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp27), atslab_d2cst_def) ;
return (tmp26) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_def] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4001(line=143, offs=25) -- 4070(line=144, offs=57)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_def (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp28) ;
ATSlocal (ats_ptr_type, tmp29) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_def:
#line 144 "ats_dynexp2_dcst.dats"
tmp29 = ats_selsin_mac(arg0, atslab_1) ;
#line 144 "ats_dynexp2_dcst.dats"
ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp29), atslab_d2cst_def) = arg1 ;
return /* (tmp28) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_set_def] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4098(line=146, offs=27) -- 4157(line=147, offs=52)
*/
ATSglobaldec()
atsopt_count_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_stamp (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (atsopt_count_type, tmp30) ;
ATSlocal (ats_ptr_type, tmp31) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_stamp:
#line 147 "ats_dynexp2_dcst.dats"
tmp31 = ats_selsin_mac(arg0, atslab_1) ;
#line 147 "ats_dynexp2_dcst.dats"
tmp30 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp31), atslab_d2cst_stamp) ;
return (tmp30) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_stamp] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4238(line=150, offs=27) -- 4297(line=151, offs=52)
*/
ATSglobaldec()
ats_ptr_type
ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__d2cst_get_hityp (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_ptr_type, tmp32) ;
ATSlocal (ats_ptr_type, tmp33) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__d2cst_get_hityp:
#line 151 "ats_dynexp2_dcst.dats"
tmp33 = ats_selsin_mac(arg0, atslab_1) ;
#line 151 "ats_dynexp2_dcst.dats"
tmp32 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp33), atslab_d2cst_hityp) ;
return (tmp32) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__d2cst_get_hityp] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4347(line=156, offs=3) -- 4398(line=156, offs=54)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lt_d2cst_d2cst (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_bool_type, tmp34) ;
ATSlocal (ats_int_type, tmp35) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lt_d2cst_d2cst:
#line 156 "ats_dynexp2_dcst.dats"
tmp35 = ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst (arg0, arg1) ;
#line 156 "ats_dynexp2_dcst.dats"
tmp34 = atspre_ilt (tmp35, 0) ;
return (tmp34) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lt_d2cst_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4427(line=158, offs=3) -- 4479(line=158, offs=55)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lte_d2cst_d2cst (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_bool_type, tmp36) ;
ATSlocal (ats_int_type, tmp37) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lte_d2cst_d2cst:
#line 158 "ats_dynexp2_dcst.dats"
tmp37 = ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst (arg0, arg1) ;
#line 158 "ats_dynexp2_dcst.dats"
tmp36 = atspre_ilte (tmp37, 0) ;
return (tmp36) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__lte_d2cst_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4508(line=161, offs=3) -- 4559(line=161, offs=54)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__eq_d2cst_d2cst (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_bool_type, tmp38) ;
ATSlocal (ats_int_type, tmp39) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__eq_d2cst_d2cst:
#line 161 "ats_dynexp2_dcst.dats"
tmp39 = ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst (arg0, arg1) ;
#line 161 "ats_dynexp2_dcst.dats"
tmp38 = atspre_ieq (tmp39, 0) ;
return (tmp38) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__eq_d2cst_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4588(line=163, offs=3) -- 4640(line=163, offs=55)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__neq_d2cst_d2cst (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_bool_type, tmp40) ;
ATSlocal (ats_int_type, tmp41) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__neq_d2cst_d2cst:
#line 163 "ats_dynexp2_dcst.dats"
tmp41 = ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst (arg0, arg1) ;
#line 163 "ats_dynexp2_dcst.dats"
tmp40 = atspre_ineq (tmp41, 0) ;
return (tmp40) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__neq_d2cst_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 4645(line=165, offs=4) -- 4944(line=175, offs=4)
*/
ATSstaticdec()
ats_int_type
_compare_d2cst_d2cst_18 (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_int_type, tmp42) ;
ATSlocal (ats_ptr_type, tmp43) ;
ATSlocal (atsopt_count_type, tmp44) ;
ATSlocal (ats_ptr_type, tmp45) ;
ATSlocal (atsopt_count_type, tmp46) ;

__ats_lab__compare_d2cst_d2cst_18:
#line 168 "ats_dynexp2_dcst.dats"
tmp43 = ats_selsin_mac(arg0, atslab_1) ;
#line 168 "ats_dynexp2_dcst.dats"
tmp44 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp43), atslab_d2cst_stamp) ;
#line 171 "ats_dynexp2_dcst.dats"
tmp45 = ats_selsin_mac(arg1, atslab_1) ;
#line 171 "ats_dynexp2_dcst.dats"
tmp46 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp45), atslab_d2cst_stamp) ;
#line 174 "ats_dynexp2_dcst.dats"
tmp42 = ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__compare_stamp_stamp (tmp44, tmp46) ;
return (tmp42) ;
} /* end of [_compare_d2cst_d2cst_18] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 5009(line=178, offs=21) -- 5074(line=179, offs=51)
*/
ATSglobaldec()
ats_int_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst (ats_ptr_type arg0, ats_ptr_type arg1) {
/* local vardec */
ATSlocal (ats_int_type, tmp47) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst:
#line 179 "ats_dynexp2_dcst.dats"
tmp47 = _compare_d2cst_d2cst_18 (arg0, arg1) ;
return (tmp47) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__compare_d2cst_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 5154(line=184, offs=24) -- 5301(line=190, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_fun (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp48) ;
ATSlocal (ats_ptr_type, tmp49) ;
ATSlocal (ats_ptr_type, tmp50) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_fun:
#line 186 "ats_dynexp2_dcst.dats"
tmp49 = ats_selsin_mac(arg0, atslab_1) ;
#line 186 "ats_dynexp2_dcst.dats"
tmp50 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp49), atslab_d2cst_kind) ;
#line 189 "ats_dynexp2_dcst.dats"
tmp48 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_fun (tmp50) ;
return (tmp48) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_fun] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 5375(line=194, offs=27) -- 5532(line=200, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extmac (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp51) ;
ATSlocal (ats_ptr_type, tmp52) ;
ATSlocal (ats_ptr_type, tmp53) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extmac:
#line 196 "ats_dynexp2_dcst.dats"
tmp52 = ats_selsin_mac(arg0, atslab_1) ;
#line 196 "ats_dynexp2_dcst.dats"
tmp53 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp52), atslab_d2cst_extdef) ;
#line 199 "ats_dynexp2_dcst.dats"
tmp51 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstextdef_is_mac (tmp53) ;
return (tmp51) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extmac] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 5589(line=203, offs=27) -- 5746(line=209, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extsta (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp54) ;
ATSlocal (ats_ptr_type, tmp55) ;
ATSlocal (ats_ptr_type, tmp56) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extsta:
#line 205 "ats_dynexp2_dcst.dats"
tmp55 = ats_selsin_mac(arg0, atslab_1) ;
#line 205 "ats_dynexp2_dcst.dats"
tmp56 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp55), atslab_d2cst_extdef) ;
#line 208 "ats_dynexp2_dcst.dats"
tmp54 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstextdef_is_sta (tmp56) ;
return (tmp54) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_extsta] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 5823(line=213, offs=27) -- 5973(line=219, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_castfn (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp57) ;
ATSlocal (ats_ptr_type, tmp58) ;
ATSlocal (ats_ptr_type, tmp59) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_castfn:
#line 215 "ats_dynexp2_dcst.dats"
tmp58 = ats_selsin_mac(arg0, atslab_1) ;
#line 215 "ats_dynexp2_dcst.dats"
tmp59 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp58), atslab_d2cst_kind) ;
#line 218 "ats_dynexp2_dcst.dats"
tmp57 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_castfn (tmp59) ;
return (tmp57) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_castfn] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 6032(line=223, offs=26) -- 6181(line=229, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_praxi (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp60) ;
ATSlocal (ats_ptr_type, tmp61) ;
ATSlocal (ats_ptr_type, tmp62) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_praxi:
#line 225 "ats_dynexp2_dcst.dats"
tmp61 = ats_selsin_mac(arg0, atslab_1) ;
#line 225 "ats_dynexp2_dcst.dats"
tmp62 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp61), atslab_d2cst_kind) ;
#line 228 "ats_dynexp2_dcst.dats"
tmp60 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_praxi (tmp62) ;
return (tmp60) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_praxi] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 6235(line=231, offs=26) -- 6384(line=237, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prfun (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp63) ;
ATSlocal (ats_ptr_type, tmp64) ;
ATSlocal (ats_ptr_type, tmp65) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prfun:
#line 233 "ats_dynexp2_dcst.dats"
tmp64 = ats_selsin_mac(arg0, atslab_1) ;
#line 233 "ats_dynexp2_dcst.dats"
tmp65 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp64), atslab_d2cst_kind) ;
#line 236 "ats_dynexp2_dcst.dats"
tmp63 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_prfun (tmp65) ;
return (tmp63) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prfun] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 6438(line=239, offs=26) -- 6587(line=245, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prval (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp66) ;
ATSlocal (ats_ptr_type, tmp67) ;
ATSlocal (ats_ptr_type, tmp68) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prval:
#line 241 "ats_dynexp2_dcst.dats"
tmp67 = ats_selsin_mac(arg0, atslab_1) ;
#line 241 "ats_dynexp2_dcst.dats"
tmp68 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp67), atslab_d2cst_kind) ;
#line 244 "ats_dynexp2_dcst.dats"
tmp66 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_prval (tmp68) ;
return (tmp66) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_prval] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 6641(line=247, offs=26) -- 6790(line=253, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_proof (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp69) ;
ATSlocal (ats_ptr_type, tmp70) ;
ATSlocal (ats_ptr_type, tmp71) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_proof:
#line 249 "ats_dynexp2_dcst.dats"
tmp70 = ats_selsin_mac(arg0, atslab_1) ;
#line 249 "ats_dynexp2_dcst.dats"
tmp71 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp70), atslab_d2cst_kind) ;
#line 252 "ats_dynexp2_dcst.dats"
tmp69 = ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__dcstkind_is_proof (tmp71) ;
return (tmp69) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_proof] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 6847(line=257, offs=25) -- 7030(line=263, offs=4)
*/
ATSglobaldec()
ats_bool_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_temp (ats_ptr_type arg0) {
/* local vardec */
ATSlocal (ats_bool_type, tmp72) ;
ATSlocal (ats_ptr_type, tmp73) ;
ATSlocal (ats_ptr_type, tmp74) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_temp:
#line 259 "ats_dynexp2_dcst.dats"
tmp73 = ats_selsin_mac(arg0, atslab_1) ;
#line 259 "ats_dynexp2_dcst.dats"
tmp74 = ats_selptr_mac(ats_castptr_mac(anairiats_rec_0, tmp73), atslab_d2cst_decarg) ;
#line 262 "ats_dynexp2_dcst.dats"
do {
/* branch: __ats_lab_0 */
#line 262 "ats_dynexp2_dcst.dats"
__ats_lab_0_0:
#line 262 "ats_dynexp2_dcst.dats"
if (tmp74 == (ats_sum_ptr_type)0) { goto __ats_lab_1_0 ; }
#line 262 "ats_dynexp2_dcst.dats"
__ats_lab_0_1:
#line 262 "ats_dynexp2_dcst.dats"
tmp72 = ats_true_bool ;
break ;

/* branch: __ats_lab_1 */
#line 262 "ats_dynexp2_dcst.dats"
__ats_lab_1_0:
#line 262 "ats_dynexp2_dcst.dats"
// if (tmp74 != (ats_sum_ptr_type)0) { ats_deadcode_failure_handle () ; }
#line 262 "ats_dynexp2_dcst.dats"
__ats_lab_1_1:
#line 262 "ats_dynexp2_dcst.dats"
tmp72 = ats_false_bool ;
break ;
} while (0) ;
return (tmp72) ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_is_temp] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7148(line=270, offs=14) -- 7234(line=272, offs=4)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp75) ;
ATSlocal (ats_ptr_type, tmp76) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst:
#line 271 "ats_dynexp2_dcst.dats"
tmp76 = ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__d2cst_get_sym (arg1) ;
#line 271 "ats_dynexp2_dcst.dats"
/* tmp75 = */ ATS_2d0_2e2_2e6_2src_2ats_symbol_2esats__fprint_symbol (arg0, tmp76) ;
return /* (tmp75) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7283(line=274, offs=23) -- 7320(line=274, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp77) ;
ATSlocal (ats_ptr_type, tmp78) ;
ATSlocal (ats_ptr_type, tmp79) ;
// ATSlocal_void (tmp80) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cst:
#line 274 "ats_dynexp2_dcst.dats"
tmp78 = atspre_stdout_get () ;
#line 274 "ats_dynexp2_dcst.dats"
tmp79 = ats_selsin_mac(tmp78, atslab_1) ;
#line 274 "ats_dynexp2_dcst.dats"
/* tmp80 = */ ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst (tmp79, arg0) ;
#line 274 "ats_dynexp2_dcst.dats"
/* tmp77 = */ atspre_stdout_view_set () ;
return /* (tmp77) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7343(line=275, offs=23) -- 7380(line=275, offs=60)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp81) ;
ATSlocal (ats_ptr_type, tmp82) ;
ATSlocal (ats_ptr_type, tmp83) ;
// ATSlocal_void (tmp84) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cst:
#line 275 "ats_dynexp2_dcst.dats"
tmp82 = atspre_stderr_get () ;
#line 275 "ats_dynexp2_dcst.dats"
tmp83 = ats_selsin_mac(tmp82, atslab_1) ;
#line 275 "ats_dynexp2_dcst.dats"
/* tmp84 = */ ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst (tmp83, arg0) ;
#line 275 "ats_dynexp2_dcst.dats"
/* tmp81 = */ atspre_stderr_view_set () ;
return /* (tmp81) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7429(line=280, offs=17) -- 7497(line=281, offs=52)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst (ats_ref_type arg0, ats_ptr_type arg1) {
/* local vardec */
// ATSlocal_void (tmp85) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst:
#line 281 "ats_dynexp2_dcst.dats"
/* tmp85 = */ ATS_2d0_2e2_2e6_2src_2ats_list_2esats__fprintlst (arg0, arg1, ATSstrcst(", "), &ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cst) ;
return /* (tmp85) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7552(line=284, offs=26) -- 7594(line=284, offs=68)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cstlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp86) ;
ATSlocal (ats_ptr_type, tmp87) ;
ATSlocal (ats_ptr_type, tmp88) ;
// ATSlocal_void (tmp89) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cstlst:
#line 284 "ats_dynexp2_dcst.dats"
tmp87 = atspre_stdout_get () ;
#line 284 "ats_dynexp2_dcst.dats"
tmp88 = ats_selsin_mac(tmp87, atslab_1) ;
#line 284 "ats_dynexp2_dcst.dats"
/* tmp89 = */ ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst (tmp88, arg0) ;
#line 284 "ats_dynexp2_dcst.dats"
/* tmp86 = */ atspre_stdout_view_set () ;
return /* (tmp86) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__print_d2cstlst] */

/*
// /home/hwxi/Research/Anairiats/src/ats_dynexp2_dcst.dats: 7620(line=285, offs=26) -- 7662(line=285, offs=68)
*/
ATSglobaldec()
ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cstlst (ats_ptr_type arg0) {
/* local vardec */
// ATSlocal_void (tmp90) ;
ATSlocal (ats_ptr_type, tmp91) ;
ATSlocal (ats_ptr_type, tmp92) ;
// ATSlocal_void (tmp93) ;

__ats_lab_ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cstlst:
#line 285 "ats_dynexp2_dcst.dats"
tmp91 = atspre_stderr_get () ;
#line 285 "ats_dynexp2_dcst.dats"
tmp92 = ats_selsin_mac(tmp91, atslab_1) ;
#line 285 "ats_dynexp2_dcst.dats"
/* tmp93 = */ ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__fprint_d2cstlst (tmp92, arg0) ;
#line 285 "ats_dynexp2_dcst.dats"
/* tmp90 = */ atspre_stderr_view_set () ;
return /* (tmp90) */ ;
} /* end of [ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__prerr_d2cstlst] */

/* static load function */

extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_list_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload (void) ;
extern ats_void_type ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__staload (void) ;

ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__staload () {
static int ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__staload_flag = 0 ;
if (ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__staload_flag) return ;
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__staload_flag = 1 ;

ATS_2d0_2e2_2e6_2src_2ats_list_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_stamp_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_syntax_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_hiexp_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_staexp2_2esats__staload () ;
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_2esats__staload () ;

return ;
} /* staload function */

/* dynamic load function */

// dynload flag declaration
extern ats_int_type ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__dynload_flag ;

ats_void_type
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__dynload () {
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__dynload_flag = 1 ;
ATS_2d0_2e2_2e6_2src_2ats_dynexp2_dcst_2edats__staload () ;

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
#line 295 "ats_dynexp2_dcst.dats"


ats_void_type
atsopt_d2cst_set_hityp (
  ats_ptr_type d2c, ats_ptr_type ohit
) {
  ((d2cst_struct*)d2c)->atslab_d2cst_hityp = ohit ; return ;
} /* end of [atsopt_d2cst_set_hityp] */



/* ****** ****** */

/* end of [ats_dynexp2_dcst_dats.c] */
