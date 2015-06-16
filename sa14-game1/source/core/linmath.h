/*------------------------------------------------------------------------------
 * File: linmath.h
 * Created: June 14, 2015
 * Last changed: June 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Math related types and functions, specifically for linear algebra.
 *
 *----------------------------------------------------------------------------*/

#ifndef linmath_h_
#define linmath_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include <limits.h>
#include <math.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Vector swizzle macros.
 *----------------------------------------------------------------------------*/

#ifdef VectorSwizzle

#define vec2_xx(v) ((vec2) { v.x, v.x })
#define vec2_yx(v) ((vec2) { v.y, v.x })
#define vec2_yy(v) ((vec2) { v.y, v.y })

#define vec3_xxx(v) ((vec3) { v.x, v.x, v.x })
#define vec3_xxy(v) ((vec3) { v.x, v.x, v.y })
#define vec3_xxz(v) ((vec3) { v.x, v.x, v.z })
#define vec3_xyx(v) ((vec3) { v.x, v.y, v.x })
#define vec3_xyy(v) ((vec3) { v.x, v.y, v.y })
#define vec3_xzx(v) ((vec3) { v.x, v.z, v.x })
#define vec3_xzy(v) ((vec3) { v.x, v.z, v.y })
#define vec3_xzz(v) ((vec3) { v.x, v.z, v.z })
#define vec3_yxx(v) ((vec3) { v.y, v.x, v.x })
#define vec3_yxy(v) ((vec3) { v.y, v.x, v.y })
#define vec3_yxz(v) ((vec3) { v.y, v.x, v.z })
#define vec3_yyx(v) ((vec3) { v.y, v.y, v.x })
#define vec3_yyy(v) ((vec3) { v.y, v.y, v.y })
#define vec3_yyz(v) ((vec3) { v.y, v.y, v.z })
#define vec3_yzx(v) ((vec3) { v.y, v.z, v.x })
#define vec3_yzy(v) ((vec3) { v.y, v.z, v.y })
#define vec3_yzz(v) ((vec3) { v.y, v.z, v.z })
#define vec3_zxx(v) ((vec3) { v.z, v.x, v.x })
#define vec3_zxy(v) ((vec3) { v.z, v.x, v.y })
#define vec3_zxz(v) ((vec3) { v.z, v.x, v.z })
#define vec3_zyx(v) ((vec3) { v.z, v.y, v.x })
#define vec3_zyy(v) ((vec3) { v.z, v.y, v.y })
#define vec3_zyz(v) ((vec3) { v.z, v.y, v.z })
#define vec3_zzx(v) ((vec3) { v.z, v.z, v.x })
#define vec3_zzy(v) ((vec3) { v.z, v.z, v.y })
#define vec3_zzz(v) ((vec3) { v.z, v.z, v.z })

#define vec4_xxxx(v) ((vec4) { v.x, v.x, v.x, v.x })
#define vec4_xxxy(v) ((vec4) { v.x, v.x, v.x, v.y })
#define vec4_xxxz(v) ((vec4) { v.x, v.x, v.x, v.z })
#define vec4_xxxw(v) ((vec4) { v.x, v.x, v.x, v.w })
#define vec4_xxyx(v) ((vec4) { v.x, v.x, v.y, v.x })
#define vec4_xxyy(v) ((vec4) { v.x, v.x, v.y, v.y })
#define vec4_xxyz(v) ((vec4) { v.x, v.x, v.y, v.z })
#define vec4_xxyw(v) ((vec4) { v.x, v.x, v.y, v.w })
#define vec4_xxzx(v) ((vec4) { v.x, v.x, v.z, v.x })
#define vec4_xxzy(v) ((vec4) { v.x, v.x, v.z, v.y })
#define vec4_xxzz(v) ((vec4) { v.x, v.x, v.z, v.z })
#define vec4_xxzw(v) ((vec4) { v.x, v.x, v.z, v.w })
#define vec4_xxwx(v) ((vec4) { v.x, v.x, v.w, v.x })
#define vec4_xxwy(v) ((vec4) { v.x, v.x, v.w, v.y })
#define vec4_xxwz(v) ((vec4) { v.x, v.x, v.w, v.z })
#define vec4_xxww(v) ((vec4) { v.x, v.x, v.w, v.w })
#define vec4_xyxx(v) ((vec4) { v.x, v.y, v.x, v.x })
#define vec4_xyxy(v) ((vec4) { v.x, v.y, v.x, v.y })
#define vec4_xyxz(v) ((vec4) { v.x, v.y, v.x, v.z })
#define vec4_xyxw(v) ((vec4) { v.x, v.y, v.x, v.w })
#define vec4_xyyx(v) ((vec4) { v.x, v.y, v.y, v.x })
#define vec4_xyyy(v) ((vec4) { v.x, v.y, v.y, v.y })
#define vec4_xyyz(v) ((vec4) { v.x, v.y, v.y, v.z })
#define vec4_xyyw(v) ((vec4) { v.x, v.y, v.y, v.w })
#define vec4_xyzx(v) ((vec4) { v.x, v.y, v.z, v.x })
#define vec4_xyzy(v) ((vec4) { v.x, v.y, v.z, v.y })
#define vec4_xyzz(v) ((vec4) { v.x, v.y, v.z, v.z })
#define vec4_xywx(v) ((vec4) { v.x, v.y, v.w, v.x })
#define vec4_xywy(v) ((vec4) { v.x, v.y, v.w, v.y })
#define vec4_xywz(v) ((vec4) { v.x, v.y, v.w, v.z })
#define vec4_xyww(v) ((vec4) { v.x, v.y, v.w, v.w })
#define vec4_xzxx(v) ((vec4) { v.x, v.z, v.x, v.x })
#define vec4_xzxy(v) ((vec4) { v.x, v.z, v.x, v.y })
#define vec4_xzxz(v) ((vec4) { v.x, v.z, v.x, v.z })
#define vec4_xzxw(v) ((vec4) { v.x, v.z, v.x, v.w })
#define vec4_xzyx(v) ((vec4) { v.x, v.z, v.y, v.x })
#define vec4_xzyy(v) ((vec4) { v.x, v.z, v.y, v.y })
#define vec4_xzyz(v) ((vec4) { v.x, v.z, v.y, v.z })
#define vec4_xzyw(v) ((vec4) { v.x, v.z, v.y, v.w })
#define vec4_xzzx(v) ((vec4) { v.x, v.z, v.z, v.x })
#define vec4_xzzy(v) ((vec4) { v.x, v.z, v.z, v.y })
#define vec4_xzzz(v) ((vec4) { v.x, v.z, v.z, v.z })
#define vec4_xzzw(v) ((vec4) { v.x, v.z, v.z, v.w })
#define vec4_xzwx(v) ((vec4) { v.x, v.z, v.w, v.x })
#define vec4_xzwy(v) ((vec4) { v.x, v.z, v.w, v.y })
#define vec4_xzwz(v) ((vec4) { v.x, v.z, v.w, v.z })
#define vec4_xzww(v) ((vec4) { v.x, v.z, v.w, v.w })
#define vec4_xwxx(v) ((vec4) { v.x, v.w, v.x, v.x })
#define vec4_xwxy(v) ((vec4) { v.x, v.w, v.x, v.y })
#define vec4_xwxz(v) ((vec4) { v.x, v.w, v.x, v.z })
#define vec4_xwxw(v) ((vec4) { v.x, v.w, v.x, v.w })
#define vec4_xwyx(v) ((vec4) { v.x, v.w, v.y, v.x })
#define vec4_xwyy(v) ((vec4) { v.x, v.w, v.y, v.y })
#define vec4_xwyz(v) ((vec4) { v.x, v.w, v.y, v.z })
#define vec4_xwyw(v) ((vec4) { v.x, v.w, v.y, v.w })
#define vec4_xwzx(v) ((vec4) { v.x, v.w, v.z, v.x })
#define vec4_xwzy(v) ((vec4) { v.x, v.w, v.z, v.y })
#define vec4_xwzz(v) ((vec4) { v.x, v.w, v.z, v.z })
#define vec4_xwzw(v) ((vec4) { v.x, v.w, v.z, v.w })
#define vec4_xwwx(v) ((vec4) { v.x, v.w, v.w, v.x })
#define vec4_xwwy(v) ((vec4) { v.x, v.w, v.w, v.y })
#define vec4_xwwz(v) ((vec4) { v.x, v.w, v.w, v.z })
#define vec4_xwww(v) ((vec4) { v.x, v.w, v.w, v.w })
#define vec4_yxxx(v) ((vec4) { v.y, v.x, v.x, v.x })
#define vec4_yxxy(v) ((vec4) { v.y, v.x, v.x, v.y })
#define vec4_yxxz(v) ((vec4) { v.y, v.x, v.x, v.z })
#define vec4_yxxw(v) ((vec4) { v.y, v.x, v.x, v.w })
#define vec4_yxyx(v) ((vec4) { v.y, v.x, v.y, v.x })
#define vec4_yxyy(v) ((vec4) { v.y, v.x, v.y, v.y })
#define vec4_yxyz(v) ((vec4) { v.y, v.x, v.y, v.z })
#define vec4_yxyw(v) ((vec4) { v.y, v.x, v.y, v.w })
#define vec4_yxzx(v) ((vec4) { v.y, v.x, v.z, v.x })
#define vec4_yxzy(v) ((vec4) { v.y, v.x, v.z, v.y })
#define vec4_yxzz(v) ((vec4) { v.y, v.x, v.z, v.z })
#define vec4_yxzw(v) ((vec4) { v.y, v.x, v.z, v.w })
#define vec4_yxwx(v) ((vec4) { v.y, v.x, v.w, v.x })
#define vec4_yxwy(v) ((vec4) { v.y, v.x, v.w, v.y })
#define vec4_yxwz(v) ((vec4) { v.y, v.x, v.w, v.z })
#define vec4_yxww(v) ((vec4) { v.y, v.x, v.w, v.w })
#define vec4_yyxx(v) ((vec4) { v.y, v.y, v.x, v.x })
#define vec4_yyxy(v) ((vec4) { v.y, v.y, v.x, v.y })
#define vec4_yyxz(v) ((vec4) { v.y, v.y, v.x, v.z })
#define vec4_yyxw(v) ((vec4) { v.y, v.y, v.x, v.w })
#define vec4_yyyx(v) ((vec4) { v.y, v.y, v.y, v.x })
#define vec4_yyyy(v) ((vec4) { v.y, v.y, v.y, v.y })
#define vec4_yyyz(v) ((vec4) { v.y, v.y, v.y, v.z })
#define vec4_yyyw(v) ((vec4) { v.y, v.y, v.y, v.w })
#define vec4_yyzx(v) ((vec4) { v.y, v.y, v.z, v.x })
#define vec4_yyzy(v) ((vec4) { v.y, v.y, v.z, v.y })
#define vec4_yyzz(v) ((vec4) { v.y, v.y, v.z, v.z })
#define vec4_yyzw(v) ((vec4) { v.y, v.y, v.z, v.w })
#define vec4_yywx(v) ((vec4) { v.y, v.y, v.w, v.x })
#define vec4_yywy(v) ((vec4) { v.y, v.y, v.w, v.y })
#define vec4_yywz(v) ((vec4) { v.y, v.y, v.w, v.z })
#define vec4_yyww(v) ((vec4) { v.y, v.y, v.w, v.w })
#define vec4_yzxx(v) ((vec4) { v.y, v.z, v.x, v.x })
#define vec4_yzxy(v) ((vec4) { v.y, v.z, v.x, v.y })
#define vec4_yzxz(v) ((vec4) { v.y, v.z, v.x, v.z })
#define vec4_yzxw(v) ((vec4) { v.y, v.z, v.x, v.w })
#define vec4_yzyx(v) ((vec4) { v.y, v.z, v.y, v.x })
#define vec4_yzyy(v) ((vec4) { v.y, v.z, v.y, v.y })
#define vec4_yzyz(v) ((vec4) { v.y, v.z, v.y, v.z })
#define vec4_yzyw(v) ((vec4) { v.y, v.z, v.y, v.w })
#define vec4_yzzx(v) ((vec4) { v.y, v.z, v.z, v.x })
#define vec4_yzzy(v) ((vec4) { v.y, v.z, v.z, v.y })
#define vec4_yzzz(v) ((vec4) { v.y, v.z, v.z, v.z })
#define vec4_yzzw(v) ((vec4) { v.y, v.z, v.z, v.w })
#define vec4_yzwx(v) ((vec4) { v.y, v.z, v.w, v.x })
#define vec4_yzwy(v) ((vec4) { v.y, v.z, v.w, v.y })
#define vec4_yzwz(v) ((vec4) { v.y, v.z, v.w, v.z })
#define vec4_yzww(v) ((vec4) { v.y, v.z, v.w, v.w })
#define vec4_ywxx(v) ((vec4) { v.y, v.w, v.x, v.x })
#define vec4_ywxy(v) ((vec4) { v.y, v.w, v.x, v.y })
#define vec4_ywxz(v) ((vec4) { v.y, v.w, v.x, v.z })
#define vec4_ywxw(v) ((vec4) { v.y, v.w, v.x, v.w })
#define vec4_ywyx(v) ((vec4) { v.y, v.w, v.y, v.x })
#define vec4_ywyy(v) ((vec4) { v.y, v.w, v.y, v.y })
#define vec4_ywyz(v) ((vec4) { v.y, v.w, v.y, v.z })
#define vec4_ywyw(v) ((vec4) { v.y, v.w, v.y, v.w })
#define vec4_ywzx(v) ((vec4) { v.y, v.w, v.z, v.x })
#define vec4_ywzy(v) ((vec4) { v.y, v.w, v.z, v.y })
#define vec4_ywzz(v) ((vec4) { v.y, v.w, v.z, v.z })
#define vec4_ywzw(v) ((vec4) { v.y, v.w, v.z, v.w })
#define vec4_ywwx(v) ((vec4) { v.y, v.w, v.w, v.x })
#define vec4_ywwy(v) ((vec4) { v.y, v.w, v.w, v.y })
#define vec4_ywwz(v) ((vec4) { v.y, v.w, v.w, v.z })
#define vec4_ywww(v) ((vec4) { v.y, v.w, v.w, v.w })
#define vec4_zxxx(v) ((vec4) { v.z, v.x, v.x, v.x })
#define vec4_zxxy(v) ((vec4) { v.z, v.x, v.x, v.y })
#define vec4_zxxz(v) ((vec4) { v.z, v.x, v.x, v.z })
#define vec4_zxxw(v) ((vec4) { v.z, v.x, v.x, v.w })
#define vec4_zxyx(v) ((vec4) { v.z, v.x, v.y, v.x })
#define vec4_zxyy(v) ((vec4) { v.z, v.x, v.y, v.y })
#define vec4_zxyz(v) ((vec4) { v.z, v.x, v.y, v.z })
#define vec4_zxyw(v) ((vec4) { v.z, v.x, v.y, v.w })
#define vec4_zxzx(v) ((vec4) { v.z, v.x, v.z, v.x })
#define vec4_zxzy(v) ((vec4) { v.z, v.x, v.z, v.y })
#define vec4_zxzz(v) ((vec4) { v.z, v.x, v.z, v.z })
#define vec4_zxzw(v) ((vec4) { v.z, v.x, v.z, v.w })
#define vec4_zxwx(v) ((vec4) { v.z, v.x, v.w, v.x })
#define vec4_zxwy(v) ((vec4) { v.z, v.x, v.w, v.y })
#define vec4_zxwz(v) ((vec4) { v.z, v.x, v.w, v.z })
#define vec4_zxww(v) ((vec4) { v.z, v.x, v.w, v.w })
#define vec4_zyxx(v) ((vec4) { v.z, v.y, v.x, v.x })
#define vec4_zyxy(v) ((vec4) { v.z, v.y, v.x, v.y })
#define vec4_zyxz(v) ((vec4) { v.z, v.y, v.x, v.z })
#define vec4_zyxw(v) ((vec4) { v.z, v.y, v.x, v.w })
#define vec4_zyyx(v) ((vec4) { v.z, v.y, v.y, v.x })
#define vec4_zyyy(v) ((vec4) { v.z, v.y, v.y, v.y })
#define vec4_zyyz(v) ((vec4) { v.z, v.y, v.y, v.z })
#define vec4_zyyw(v) ((vec4) { v.z, v.y, v.y, v.w })
#define vec4_zyzx(v) ((vec4) { v.z, v.y, v.z, v.x })
#define vec4_zyzy(v) ((vec4) { v.z, v.y, v.z, v.y })
#define vec4_zyzz(v) ((vec4) { v.z, v.y, v.z, v.z })
#define vec4_zyzw(v) ((vec4) { v.z, v.y, v.z, v.w })
#define vec4_zywx(v) ((vec4) { v.z, v.y, v.w, v.x })
#define vec4_zywy(v) ((vec4) { v.z, v.y, v.w, v.y })
#define vec4_zywz(v) ((vec4) { v.z, v.y, v.w, v.z })
#define vec4_zyww(v) ((vec4) { v.z, v.y, v.w, v.w })
#define vec4_zzxx(v) ((vec4) { v.z, v.z, v.x, v.x })
#define vec4_zzxy(v) ((vec4) { v.z, v.z, v.x, v.y })
#define vec4_zzxz(v) ((vec4) { v.z, v.z, v.x, v.z })
#define vec4_zzxw(v) ((vec4) { v.z, v.z, v.x, v.w })
#define vec4_zzyx(v) ((vec4) { v.z, v.z, v.y, v.x })
#define vec4_zzyy(v) ((vec4) { v.z, v.z, v.y, v.y })
#define vec4_zzyz(v) ((vec4) { v.z, v.z, v.y, v.z })
#define vec4_zzyw(v) ((vec4) { v.z, v.z, v.y, v.w })
#define vec4_zzzx(v) ((vec4) { v.z, v.z, v.z, v.x })
#define vec4_zzzy(v) ((vec4) { v.z, v.z, v.z, v.y })
#define vec4_zzzz(v) ((vec4) { v.z, v.z, v.z, v.z })
#define vec4_zzzw(v) ((vec4) { v.z, v.z, v.z, v.w })
#define vec4_zzwx(v) ((vec4) { v.z, v.z, v.w, v.x })
#define vec4_zzwy(v) ((vec4) { v.z, v.z, v.w, v.y })
#define vec4_zzwz(v) ((vec4) { v.z, v.z, v.w, v.z })
#define vec4_zzww(v) ((vec4) { v.z, v.z, v.w, v.w })
#define vec4_zwxx(v) ((vec4) { v.z, v.w, v.x, v.x })
#define vec4_zwxy(v) ((vec4) { v.z, v.w, v.x, v.y })
#define vec4_zwxz(v) ((vec4) { v.z, v.w, v.x, v.z })
#define vec4_zwxw(v) ((vec4) { v.z, v.w, v.x, v.w })
#define vec4_zwyx(v) ((vec4) { v.z, v.w, v.y, v.x })
#define vec4_zwyy(v) ((vec4) { v.z, v.w, v.y, v.y })
#define vec4_zwyz(v) ((vec4) { v.z, v.w, v.y, v.z })
#define vec4_zwyw(v) ((vec4) { v.z, v.w, v.y, v.w })
#define vec4_zwzx(v) ((vec4) { v.z, v.w, v.z, v.x })
#define vec4_zwzy(v) ((vec4) { v.z, v.w, v.z, v.y })
#define vec4_zwzz(v) ((vec4) { v.z, v.w, v.z, v.z })
#define vec4_zwzw(v) ((vec4) { v.z, v.w, v.z, v.w })
#define vec4_zwwx(v) ((vec4) { v.z, v.w, v.w, v.x })
#define vec4_zwwy(v) ((vec4) { v.z, v.w, v.w, v.y })
#define vec4_zwwz(v) ((vec4) { v.z, v.w, v.w, v.z })
#define vec4_zwww(v) ((vec4) { v.z, v.w, v.w, v.w })
#define vec4_wxxx(v) ((vec4) { v.w, v.x, v.x, v.x })
#define vec4_wxxy(v) ((vec4) { v.w, v.x, v.x, v.y })
#define vec4_wxxz(v) ((vec4) { v.w, v.x, v.x, v.z })
#define vec4_wxxw(v) ((vec4) { v.w, v.x, v.x, v.w })
#define vec4_wxyx(v) ((vec4) { v.w, v.x, v.y, v.x })
#define vec4_wxyy(v) ((vec4) { v.w, v.x, v.y, v.y })
#define vec4_wxyz(v) ((vec4) { v.w, v.x, v.y, v.z })
#define vec4_wxyw(v) ((vec4) { v.w, v.x, v.y, v.w })
#define vec4_wxzx(v) ((vec4) { v.w, v.x, v.z, v.x })
#define vec4_wxzy(v) ((vec4) { v.w, v.x, v.z, v.y })
#define vec4_wxzz(v) ((vec4) { v.w, v.x, v.z, v.z })
#define vec4_wxzw(v) ((vec4) { v.w, v.x, v.z, v.w })
#define vec4_wxwx(v) ((vec4) { v.w, v.x, v.w, v.x })
#define vec4_wxwy(v) ((vec4) { v.w, v.x, v.w, v.y })
#define vec4_wxwz(v) ((vec4) { v.w, v.x, v.w, v.z })
#define vec4_wxww(v) ((vec4) { v.w, v.x, v.w, v.w })
#define vec4_wyxx(v) ((vec4) { v.w, v.y, v.x, v.x })
#define vec4_wyxy(v) ((vec4) { v.w, v.y, v.x, v.y })
#define vec4_wyxz(v) ((vec4) { v.w, v.y, v.x, v.z })
#define vec4_wyxw(v) ((vec4) { v.w, v.y, v.x, v.w })
#define vec4_wyyx(v) ((vec4) { v.w, v.y, v.y, v.x })
#define vec4_wyyy(v) ((vec4) { v.w, v.y, v.y, v.y })
#define vec4_wyyz(v) ((vec4) { v.w, v.y, v.y, v.z })
#define vec4_wyyw(v) ((vec4) { v.w, v.y, v.y, v.w })
#define vec4_wyzx(v) ((vec4) { v.w, v.y, v.z, v.x })
#define vec4_wyzy(v) ((vec4) { v.w, v.y, v.z, v.y })
#define vec4_wyzz(v) ((vec4) { v.w, v.y, v.z, v.z })
#define vec4_wyzw(v) ((vec4) { v.w, v.y, v.z, v.w })
#define vec4_wywx(v) ((vec4) { v.w, v.y, v.w, v.x })
#define vec4_wywy(v) ((vec4) { v.w, v.y, v.w, v.y })
#define vec4_wywz(v) ((vec4) { v.w, v.y, v.w, v.z })
#define vec4_wyww(v) ((vec4) { v.w, v.y, v.w, v.w })
#define vec4_wzxx(v) ((vec4) { v.w, v.z, v.x, v.x })
#define vec4_wzxy(v) ((vec4) { v.w, v.z, v.x, v.y })
#define vec4_wzxz(v) ((vec4) { v.w, v.z, v.x, v.z })
#define vec4_wzxw(v) ((vec4) { v.w, v.z, v.x, v.w })
#define vec4_wzyx(v) ((vec4) { v.w, v.z, v.y, v.x })
#define vec4_wzyy(v) ((vec4) { v.w, v.z, v.y, v.y })
#define vec4_wzyz(v) ((vec4) { v.w, v.z, v.y, v.z })
#define vec4_wzyw(v) ((vec4) { v.w, v.z, v.y, v.w })
#define vec4_wzzx(v) ((vec4) { v.w, v.z, v.z, v.x })
#define vec4_wzzy(v) ((vec4) { v.w, v.z, v.z, v.y })
#define vec4_wzzz(v) ((vec4) { v.w, v.z, v.z, v.z })
#define vec4_wzzw(v) ((vec4) { v.w, v.z, v.z, v.w })
#define vec4_wzwx(v) ((vec4) { v.w, v.z, v.w, v.x })
#define vec4_wzwy(v) ((vec4) { v.w, v.z, v.w, v.y })
#define vec4_wzwz(v) ((vec4) { v.w, v.z, v.w, v.z })
#define vec4_wzww(v) ((vec4) { v.w, v.z, v.w, v.w })
#define vec4_wwxx(v) ((vec4) { v.w, v.w, v.x, v.x })
#define vec4_wwxy(v) ((vec4) { v.w, v.w, v.x, v.y })
#define vec4_wwxz(v) ((vec4) { v.w, v.w, v.x, v.z })
#define vec4_wwxw(v) ((vec4) { v.w, v.w, v.x, v.w })
#define vec4_wwyx(v) ((vec4) { v.w, v.w, v.y, v.x })
#define vec4_wwyy(v) ((vec4) { v.w, v.w, v.y, v.y })
#define vec4_wwyz(v) ((vec4) { v.w, v.w, v.y, v.z })
#define vec4_wwyw(v) ((vec4) { v.w, v.w, v.y, v.w })
#define vec4_wwzx(v) ((vec4) { v.w, v.w, v.z, v.x })
#define vec4_wwzy(v) ((vec4) { v.w, v.w, v.z, v.y })
#define vec4_wwzz(v) ((vec4) { v.w, v.w, v.z, v.z })
#define vec4_wwzw(v) ((vec4) { v.w, v.w, v.z, v.w })
#define vec4_wwwx(v) ((vec4) { v.w, v.w, v.w, v.x })
#define vec4_wwwy(v) ((vec4) { v.w, v.w, v.w, v.y })
#define vec4_wwwz(v) ((vec4) { v.w, v.w, v.w, v.z })
#define vec4_wwww(v) ((vec4) { v.w, v.w, v.w, v.w })

#endif /* VectorSwizzle */

#define vec_float_padding int:sizeof(float) * CHAR_BIT

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: vec2
 *
 * Description:
 *   Vector type with two components (x and y).
 *------------------------------------*/
typedef union {
    float coord[2];
    struct { float x, y; };
} vec2;

/*--------------------------------------
 * Type: vec3
 *
 * Description:
 *   Vector type with three components (x, y and z or r, g and b).
 *------------------------------------*/
typedef union {
    float coord[3];
    struct { float x, y, z; };

    vec2 xy;
    struct { vec_float_padding; vec2 yz; };
} vec3;

/*--------------------------------------
 * Type: vec4
 *
 * Description:
 *   Vector type with four components (x, y, z and w or r, g, b and a).
 *------------------------------------*/
typedef union {
    float coord[4];
    struct { float x, y, z, w; };

    vec2 xy;
    struct { vec_float_padding; vec2 yz; };
    struct { vec_float_padding; vec_float_padding; vec2 zw; };

    vec3 xyz;
    struct { vec_float_padding; vec3 yzw; };
} vec4;

#undef vec_float_padding

typedef union {
    float value[4];
    vec2 row[2];
    struct { vec2 x; vec2 y; };
} mat2x2;

typedef union {
    float value[9];
    vec3 row[3];
    struct { vec3 x; vec3 y; vec3 z; };
} mat3x3;

typedef union {
    float value[16];
    vec4 row[4];
    struct { vec4 x; vec4 y; vec4 z; vec4 w; };
} mat4x4;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Unit vector macros.
 *----------------------------------------------------------------------------*/

#define vec2_unitX() (vec2) { 1.0f, 0.0f }
#define vec2_unitY() (vec2) { 0.0f, 1.0f }

#define vec3_unitX() (vec3) { 1.0f, 0.0f, 0.0f }
#define vec3_unitY() (vec3) { 0.0f, 1.0f, 0.0f }
#define vec3_unitZ() (vec3) { 0.0f, 0.0f, 1.0f }

#define vec4_unitX() (vec4) { 1.0f, 0.0f, 0.0f, 0.0f }
#define vec4_unitY() (vec4) { 0.0f, 1.0f, 0.0f, 0.0f }
#define vec4_unitZ() (vec4) { 0.0f, 0.0f, 1.0f, 0.0f }
#define vec4_unitW() (vec4) { 0.0f, 0.0f, 0.0f, 1.0f }

/*------------------------------------------------------------------------------
 * Vector-vector operations.
 *----------------------------------------------------------------------------*/

#define vec2_add(a, b) ((vec2) { a.x+b.x, a.y+b.y })
#define vec2_mul(a, b) ((vec2) { a.x*b.x, a.y*b.y })
#define vec2_sub(a, b) ((vec2) { a.x-b.x, a.y-b.y })

#define vec3_add(a, b) ((vec3) { a.x+b.x, a.y+b.y, a.z+b.z })
#define vec3_mul(a, b) ((vec3) { a.x*b.x, a.y*b.y, a.z*b.z })
#define vec3_sub(a, b) ((vec3) { a.x-b.x, a.y-b.y, a.z-b.z })

#define vec4_add(a, b) ((vec4) { a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w })
#define vec4_mul(a, b) ((vec4) { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w })
#define vec4_sub(a, b) ((vec4) { a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w })

#define vec2_dot(a, b) (a.x*b.x+a.y*b.y)
#define vec3_dot(a, b) (a.x*b.x+a.y*b.y+a.z*b.z)
#define vec4_dot(a, b) (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)

#define vec3_cross(a, b) ((vec3) { a.y*b.z-a.z*b.y,   \
                                   a.z*b.x-a.x*b.z,   \
                                   a.x*b.y-a.y*b.x })

/*------------------------------------------------------------------------------
 * Vector-matrix operations.
 *----------------------------------------------------------------------------*/

#define vec2_mat2x2_mul(v, m) ((vec2) { \
            v.x*m.x.x+v.y*m.x.y,        \
            v.x*m.y.x+v.y*m.y.y         \
        })

#define vec3_mat3x3_mul(v, m) ((vec3) {    \
            v.x*m.x.x+v.y*m.x.y+v.z*m.x.z, \
            v.x*m.y.x+v.y*m.y.y+v.z*m.y.z, \
            v.x*m.z.x+v.y*m.z.y+v.z*m.z.z  \
        })

#define vec4_mat4x4_mul(v, m) ((vec4) {             \
            v.x*m.x.x+v.y*m.x.y+vz*m.x.z+v.w*m.x.w, \
            v.x*m.y.x+v.y*m.y.y+vz*m.y.z+v.w*m.y.w, \
            v.x*m.z.x+v.y*m.z.y+vz*m.z.z+v.w*m.z.w, \
            v.x*m.w.x+v.y*m.w.y+vz*m.w.z+v.w*m.w.w  \
        })

/*------------------------------------------------------------------------------
 * Matrix macros.
 *----------------------------------------------------------------------------*/

#define mat2x2_identity() ((mat2x2) { 1.0f, 0.0f, \
                                      0.0f, 1.0f })

#define mat3x3_identity() ((mat3x3) { 1.0f, 0.0f, 0.0f,   \
                                      0.0f, 1.0f, 0.0f,   \
                                      0.0f, 0.0f, 1.0f })

#define mat4x4_identity() ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                      0.0f, 1.0f, 0.0f, 0.0f,   \
                                      0.0f, 0.0f, 1.0f, 0.0f,   \
                                      0.0f, 0.0f, 0.0f, 1.0f })

#define mat2x2_rotateZ(a) ((mat2x2) { cos(a), -sin(a),   \
                                      sin(a),  cos(a) })

#define mat3x3_rotateX(a) ((mat3x3) { 1.0f, 0.0f,    0.0f,     \
                                      0.0f, cos(a), -sin(a),   \
                                      0.0f, sin(a),  cos(a) })

#define mat3x3_rotateY(a) ((mat3x3) {  cos(a), 0.0f, sin(a),   \
                                       0.0f,   1.0f, 0.0f,     \
                                      -sin(a), 0.0f, cos(a) })

#define mat3x3_rotateZ(a) ((mat3x3) { cos(a), -sin(a), 0.0f,   \
                                      sin(a),  cos(a), 0.0f,   \
                                      0.0f,    0.0f,   1.0f })

#define mat4x4_rotateX(a) ((mat4x4) { 1.0f, 0.0f,    0.0f,   0.0f,   \
                                      0.0f, cos(a), -sin(a), 0.0f,   \
                                      0.0f, sin(a),  cos(a), 0.0f,   \
                                      0.0f, 0.0f,    0.0f,   1.0f })

#define mat4x4_rotateY(a) ((mat4x4) {  cos(a), 0.0f, sin(a), 0.0f,   \
                                       0.0f,   1.0f, 0.0f,   0.0f,   \
                                      -sin(a), 0.0f, cos(a), 0.0f,   \
                                       0.0f,   0.0f, 0.0f,   1.0f })

#define mat4x4_rotateZ(a) ((mat4x4) { cos(a), -sin(a), 0.0f, 0.0f,   \
                                      sin(a),  cos(a), 0.0f, 0.0f,   \
                                      0.0f,    0.0f,   1.0f, 0.0f,   \
                                      0.0f,    0.0f,   0.0f, 1.0f })

#define mat4x4_translateX(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, d,      \
                                         0.0f, 1.0f, 0.0f, 0.0f,   \
                                         0.0f, 0.0f, 1.0f, 0.0f,   \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat4x4_translateY(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                         0.0f, 1.0f, 0.0f, d,      \
                                         0.0f, 0.0f, 1.0f, 0.0f,   \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat4x4_translateZ(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                         0.0f, 1.0f, 0.0f, 0.0f,   \
                                         0.0f, 0.0f, 1.0f, d,      \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat2x2_toMat3x3(m) ((mat3x3) { m.x.x, m.x.y, 0.0f,   \
                                       m.y.x, m.y.y, 0.0f,   \
                                       0.0f,  0.0f,  0.0f })

#define mat2x2_toMat4x4(m) ((mat4x4) { m.x.x, m.x.y, 0.0f, 0.0f,   \
                                       m.y.x, m.y.y, 0.0f, 0.0f,   \
                                       0.0f,  0.0f,  0.0f, 0.0f,   \
                                       0.0f,  0.0f,  0.0f, 0.0f })

#define mat3x3_toMat2x2(m) ((mat2x2) { m.x.x, m.x.y,   \
                                       m.y.x, m.y.y })

#define mat3x3_toMat4x4(m) ((mat4x4) { m.x.x, m.x.y, m.x.z, 0.0f,   \
                                       m.y.x, m.y.y, m.y.z, 0.0f,   \
                                       m.z.x, m.z.y, m.z.z, 0.0f,   \
                                       0.0f,  0.0f,  0.0f,  0.0f })

#define mat4x4_toMat2x2(m) ((mat2x2) { m.x.x, m.x.y,   \
                                       m.y.x, m.y.y })

#define mat4x4_toMat3x3(m) ((mat3x3) { m.x.x, m.x.y, m.x.z,   \
                                       m.y.x, m.y.y, m.y.z,   \
                                       m.z.x, m.z.y, m.z.z })

#define mat2x2_transpose(m) ((mat2x2) { m.x.x, m.y.x,   \
                                        m.x.y, m.y.y })

#define mat3x3_transpose(m) ((mat3x3) { m.x.x, m.y.x, m.z.x,   \
                                        m.x.y, m.y.y, m.z.y,   \
                                        m.x.z, m.y.z, m.z.z })

#define mat4x4_transpose(m) ((mat4x4) { m.x.x, m.y.x, m.z.x, m.w.x,   \
                                        m.x.y, m.y.y, m.z.y, m.w.y,   \
                                        m.x.z, m.y.z, m.z.z, m.w.z,   \
                                        m.x.w, m.y.w, m.z.w, m.w.w })

/*------------------------------------------------------------------------------
 * Matrix-matrix operations.
 *----------------------------------------------------------------------------*/

static inline mat2x2 mat2x2_add(mat2x2 a, mat2x2 b) {
    return ((mat2x2) {
        a.x.x + b.x.x, a.x.y + b.x.y,
        a.y.x + b.y.x, a.y.y + b.y.y
    });
}

static inline mat2x2 mat2x2_mul(mat2x2 a, mat2x2 b) {
    return ((mat2x2) {
        a.x.x*b.x.x + a.x.y*b.y.x,
        a.x.x*b.x.y + a.x.y*b.y.y,
        a.y.x*b.x.x + a.y.y*b.y.x,
        a.y.x*b.x.y + a.y.y*b.y.y
    });
}

static inline mat2x2 mat2x2_sub(mat2x2 a, mat2x2 b) {
    return ((mat2x2) {
        a.x.x - b.x.x, a.x.y - b.x.y,
        a.y.x - b.y.x, a.y.y - b.y.y
    });
}

static inline mat3x3 mat3x3_add(mat3x3 a, mat3x3 b) {
    return ((mat3x3) {
        a.x.x + b.x.x, a.x.y + b.x.y, a.x.z + b.x.z,
        a.y.x + b.y.x, a.y.y + b.y.y, a.y.z + b.y.z,
        a.z.x + b.z.x, a.z.y + b.z.y, a.z.z + b.z.z
    });
}

static inline mat3x3 mat3x3_mul(mat3x3 a, mat3x3 b) {
    return ((mat3x3) {
        a.x.x*b.x.x + a.x.y*b.y.x + a.x.z*b.z.x,
        a.x.x*b.x.y + a.x.y*b.y.y + a.x.z*b.z.y,
        a.x.x*b.x.z + a.x.y*b.y.z + a.x.z*b.z.z,
        a.y.x*b.x.x + a.y.y*b.y.x + a.y.z*b.z.x,
        a.y.x*b.x.y + a.y.y*b.y.y + a.y.z*b.z.y,
        a.y.x*b.x.z + a.y.y*b.y.z + a.y.z*b.z.z,
        a.z.x*b.x.x + a.z.y*b.y.x + a.z.z*b.z.x,
        a.z.x*b.x.y + a.z.y*b.y.y + a.z.z*b.z.y,
        a.z.x*b.x.z + a.z.y*b.y.z + a.z.z*b.z.z
    });
}

static inline mat3x3 mat3x3_sub(mat3x3 a, mat3x3 b) {
    return ((mat3x3) {
        a.x.x - b.x.x, a.x.y - b.x.y, a.x.z - b.x.z,
        a.y.x - b.y.x, a.y.y - b.y.y, a.y.z - b.y.z,
        a.z.x - b.z.x, a.z.y - b.z.y, a.z.z - b.z.z
    });
}

static inline mat4x4 mat4x4_add(mat4x4 a, mat4x4 b) {
    return ((mat4x4) {
        a.x.x + b.x.x, a.x.y + b.x.y, a.x.z + b.x.z, a.x.w + b.x.w,
        a.y.x + b.y.x, a.y.y + b.y.y, a.y.z + b.y.z, a.y.w + b.y.w,
        a.z.x + b.z.x, a.z.y + b.z.y, a.z.z + b.z.z, a.z.w + b.z.w,
        a.w.x + b.w.x, a.w.y + b.w.y, a.w.z + b.w.z, a.w.w + b.w.w
    });
}

static inline void mat4x4_mul(const mat4x4 *a, const mat4x4 *b, mat4x4 *r) {
        r->x.x = a->x.x*b->x.x+a->x.y*b->y.x+a->x.z*b->z.x+a->x.w*b->w.x;
        r->x.y = a->x.x*b->x.y+a->x.y*b->y.y+a->x.z*b->z.y+a->x.w*b->w.y;
        r->x.z = a->x.x*b->x.z+a->x.y*b->y.z+a->x.z*b->z.z+a->x.w*b->w.z;
        r->x.w = a->x.x*b->x.w+a->x.y*b->y.w+a->x.z*b->z.w+a->x.w*b->w.w;
        r->y.x = a->y.x*b->x.x+a->y.y*b->y.x+a->y.z*b->z.x+a->y.w*b->w.x;
        r->y.y = a->y.x*b->x.y+a->y.y*b->y.y+a->y.z*b->z.y+a->y.w*b->w.y;
        r->y.z = a->y.x*b->x.z+a->y.y*b->y.z+a->y.z*b->z.z+a->y.w*b->w.z;
        r->y.w = a->y.x*b->x.w+a->y.y*b->y.w+a->y.z*b->z.w+a->y.w*b->w.w;
        r->z.x = a->z.x*b->x.x+a->z.y*b->y.x+a->z.z*b->z.x+a->z.w*b->w.x;
        r->z.y = a->z.x*b->x.y+a->z.y*b->y.y+a->z.z*b->z.y+a->z.w*b->w.y;
        r->z.z = a->z.x*b->x.z+a->z.y*b->y.z+a->z.z*b->z.z+a->z.w*b->w.z;
        r->z.w = a->z.x*b->x.w+a->z.y*b->y.w+a->z.z*b->z.w+a->z.w*b->w.w;
        r->w.x = a->w.x*b->x.x+a->w.y*b->y.x+a->w.z*b->z.x+a->w.w*b->w.x;
        r->w.y = a->w.x*b->x.y+a->w.y*b->y.y+a->w.z*b->z.y+a->w.w*b->w.y;
        r->w.z = a->w.x*b->x.z+a->w.y*b->y.z+a->w.z*b->z.z+a->w.w*b->w.z;
        r->w.w = a->w.x*b->x.w+a->w.y*b->y.w+a->w.z*b->z.w+a->w.w*b->w.w;
}

static inline mat4x4 mat4x4_sub(mat4x4 a, mat4x4 b) {
    return ((mat4x4) {
        a.x.x - b.x.x, a.x.y - b.x.y, a.x.z - b.x.z, a.x.w - b.x.w,
        a.y.x - b.y.x, a.y.y - b.y.y, a.y.z - b.y.z, a.y.w - b.y.w,
        a.z.x - b.z.x, a.z.y - b.z.y, a.z.z - b.z.z, a.z.w - b.z.w,
        a.w.x - b.w.x, a.w.y - b.w.y, a.w.z - b.w.z, a.w.w - b.w.w
    });
}

static inline vec2 vec2_normalize(vec2 v) {
    float d = v.x*v.x + v.y*v.y;

    if (d > 0.0f) {
        d = sqrt(d);
        return ((vec2) { v.x/d, v.y/d });
    }

    return (v);
}

static inline vec3 vec3_normalize(vec3 v) {
    float d = v.x*v.x + v.y*v.y + v.z*v.z;

    if (d > 0.0f) {
        d = sqrt(d);
        return ((vec3) { v.x/d, v.y/d, v.z/d });
    }

    return (v);
}

static inline vec4 vec4_normalize(vec4 v) {
    float d = v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;

    if (d > 0.0f) {
        d = sqrt(d);
        return ((vec4) { v.x/d, v.y/d, v.z/d, v.w/d });
    }

    return (v);
}

static inline mat4x4 mat4x4_lookAt(vec3 pos, vec3 at, vec3 up) {
    vec3 z_axis = vec3_sub(pos, at);

    z_axis = vec3_normalize(z_axis);

    vec3 x_axis = vec3_normalize(vec3_cross(up, z_axis));
    vec3 y_axis = vec3_cross(z_axis, x_axis);

    float x_dot = vec3_dot(x_axis, pos);
    float y_dot = vec3_dot(y_axis, pos);
    float z_dot = vec3_dot(z_axis, pos);

    mat4x4 m = {
         x_axis.x,  y_axis.x,  z_axis.x, 0.0f,
         x_axis.y,  y_axis.y,  z_axis.y, 0.0f,
         x_axis.z,  y_axis.z,  z_axis.z, 0.0f,
        -x_dot,    -y_dot,    -z_dot,    1.0f
    };

    return (mat4x4_transpose(m));
}

static inline mat4x4 mat4x4_ortho(float left, float right, float bottom,
                                  float top, float near, float far)
{
    float l=left, r=right, b=bottom, t=top, n=near, f=far;

    mat4x4 m = {
        2.0f/(r-l), 0.0f,        0.0f,       -(r+l)/(r-l),
        0.0f,       2.0f/(t-b),  0.0f,       -(t+b)/(t-b),
        0.0f,       0.0f,        2.0f/(n-f), -(n+f)/(n-f),
        0.0f,       0.0f,        0.0f,        1.0f
    };
    
    return (m);
}

static inline mat4x4 mat4x4_perspective(float left, float right, float bottom,
                                        float top, float near, float far)
{
    float l=left, r=right, b=bottom, t=top, n=near, f=far;

    mat4x4 m = {
        (-2.0f*f)/(r-l),  0.0f,            (r+l)/(r-l), 0.0f,
         0.0f,           -(2.0f*f)/(t-b),  (t+b)/(t-b), 0.0f,
         0.0f,            0.0f,           -(n+f)/(n-f), (2.0f*n*f)/(n-f),
         0.0f,            0.0f,           -1.0f,        0.0f
    };

    return (m);
}

#endif /* linalg_h_ */
