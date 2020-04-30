#include "main.h"
#include "stdint.h"
#include "stdlib.h"

#define _HI(x) (uint16_t)((x) >> 8)
#define _LO(x) (uint16_t)((x)&0xFF)
#define _HI16(x) (uint16_t)((x) >> 16)
#define _LO16(x) (uint16_t)((x)&0xFFFF)

#define ATAN2_K0 0.1963f/(2.0f*M_PI)
#define ATAN2_K1 0.9817f/(2.0f*M_PI)
#define ATAN2_K2 M_PI/4.0f/(2.0f*M_PI)
#define ATAN2_K3 3.0f*M_PI/4.0f/(2.0f*M_PI)

typedef int32_t _iq;

#define GLOBAL_Q 24
#define HALF_GLOBAL_Q (GLOBAL_Q / 2)

#define AM_PI 3.141592653589793f
#define M_2PI (M_PI * 2.0f)
#define M_PI2 (M_PI / 2.0f)
#define AM_SQRT3 1.7320508075688772f
/* Base IQ types */
#define _IQ30(A) (int32_t)((A)*1073741824.0f)
#define _IQ29(A) (int32_t)((A)*536870912.0f)
#define _IQ28(A) (int32_t)((A)*268435456.0f)
#define _IQ27(A) (int32_t)((A)*134217728.0f)
#define _IQ26(A) (int32_t)((A)*67108864.0f)
#define _IQ25(A) (int32_t)((A)*33554432.0f)
#define _IQ24(A) (int32_t)((A)*16777216.0f)
#define _IQ23(A) (int32_t)((A)*8388608.0f)
#define _IQ22(A) (int32_t)((A)*4194304.0f)
#define _IQ21(A) (int32_t)((A)*2097152.0f)
#define _IQ20(A) (int32_t)((A)*1048576.0f)
#define _IQ19(A) (int32_t)((A)*524288.0f)
#define _IQ18(A) (int32_t)((A)*262144.0f)
#define _IQ17(A) (int32_t)((A)*131072.0f)
#define _IQ16(A) (int32_t)((A)*65536.0f)
#define _IQ15(A) (int32_t)((A)*32768.0f)
#define _IQ14(A) (int32_t)((A)*16384.0f)
#define _IQ13(A) (int32_t)((A)*8192.0f)
#define _IQ12(A) (int32_t)((A)*4096.0f)
#define _IQ11(A) (int32_t)((A)*2048.0f)
#define _IQ10(A) (int32_t)((A)*1024.0f)
#define _IQ9(A) (int32_t)((A)*512.0f)
#define _IQ8(A) (int32_t)((A)*256.0f)
#define _IQ7(A) (int32_t)((A)*128.0f)
#define _IQ6(A) (int32_t)((A)*64.0f)
#define _IQ5(A) (int32_t)((A)*32.0f)
#define _IQ4(A) (int32_t)((A)*16.0f)
#define _IQ3(A) (int32_t)((A)*8.0f)
#define _IQ2(A) (int32_t)((A)*4.0f)
#define _IQ1(A) (int32_t)((A)*2.0f)

#define _IQ(A) _IQ24(A)
/* transform IQ values */
#define _IQ8toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 8))
#define _IQ9toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 9))
#define _IQ10toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 10))
#define _IQ11toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 11))
#define _IQ12toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 12))
#define _IQ13toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 13))
#define _IQ14toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 14))
#define _IQ15toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 15))
#define _IQ16toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 16))
#define _IQ21toIQ(A) ((int32_t)(A) << (GLOBAL_Q - 21))

#define _IQtoIQ8(A) ((int32_t)(A) >> (GLOBAL_Q - 8))
#define _IQtoIQ9(A) ((int32_t)(A) >> (GLOBAL_Q - 9))
#define _IQtoIQ10(A) ((int32_t)(A) >> (GLOBAL_Q - 10))
#define _IQtoIQ11(A) ((int32_t)(A) >> (GLOBAL_Q - 11))
#define _IQtoIQ12(A) ((int32_t)(A) >> (GLOBAL_Q - 12))
#define _IQtoIQ13(A) ((int32_t)(A) >> (GLOBAL_Q - 13))
#define _IQtoIQ14(A) ((int32_t)(A) >> (GLOBAL_Q - 14))
#define _IQtoIQ15(A) ((int32_t)(A) >> (GLOBAL_Q - 15))
#define _IQtoIQ16(A) ((int32_t)(A) >> (GLOBAL_Q - 16))
#define _IQtoIQ21(A) ((int32_t)(A) >> (GLOBAL_Q - 21))

#define _IQ24toF(A) ((float)(A) * (1.0f / 16777216.0f))
/* divide func */
#define _IQdiv2(A) ((A) >> 1)
#define _IQdiv4(A) ((A) >> 2)
#define _IQdiv8(A) ((A) >> 3)
#define _IQdiv16(A) ((A) >> 4)
#define _IQdiv32(A) ((A) >> 5)
#define _IQdiv64(A) ((A) >> 6)
#define _IQdiv128(A) ((A) >> 7)
#define _IQdiv256(A) ((A) >> 8)
#define _IQdiv(A, B) (int32_t)(((int64_t)(A) << GLOBAL_Q) / (B))	// use if target dont have FPU
#define _IQdivlp(A, B) (int32_t)(((A) << HALF_GLOBAL_Q) / ((B) << HALF_GLOBAL_Q))
#define _IQ24div(A, B) (_IQ24((float) (A) / (B) )) // use if target have FPU

#define _IQmpy2(A) ((A) << 1)
#define _IQmpy4(A) ((A) << 2)
#define _IQmpy8(A) ((A) << 3)
#define _IQmpy(A, B) (int32_t)(((int64_t)(A) * (B)) >> GLOBAL_Q)
#define _IQmpylp(A, B) (int32_t)(((A) >> HALF_GLOBAL_Q) * ((B) >> HALF_GLOBAL_Q))

#define _IQb2u(x, BASE) ((uint32_t)((int32_t)(x) + ((1 << BASE) - 1)))

#define _IQsat(A, Pos, Neg) (((A) > (Pos)) ? (Pos) : (((A) < (Neg)) ? (Neg) : (A)))

#define _IQabs(x) ((x < 0) ? x = -x : x)

inline _iq _IQsin(_iq VAR);

inline _iq _IQsin(_iq VAR)
{
	VAR = VAR > _IQ(M_PI2) ? -VAR + _IQ(M_PI)
						   : VAR < -_IQ(M_PI2) ? -VAR - _IQ(M_PI) : VAR;
	VAR = _IQmpy(VAR, (_IQ(0.98557) - _IQmpy(VAR, _IQmpy(VAR, _IQ(0.142595)))));
	return VAR;
}

inline _iq _IQcos(_iq VAR);

inline _iq _IQcos(_iq VAR)
{
	VAR = (VAR > 0 ? -VAR : VAR) + _IQ(M_PI2);
	VAR = _IQmpy(VAR, (_IQ(0.98557) - _IQmpy(VAR, _IQmpy(VAR, _IQ(0.142595)))));
	return VAR;
}

//! Функция синуса, приближенная, быстрая.

//!A sine approximation via  a fourth-order cosine approx.
//! Внутри работает с точностью 16.16, но конвертируется в 8.24
//! http://www.coranac.com/2009/07/sines/

static inline int32_t _IQ24sinPU(int32_t x)
{
	int32_t c, y;
	static const int32_t qN = 13, qA = 12, B = 19900, C = 3516;
	x = x >> 9;			//from 8.24
	c = x << (30 - qN); // Semi-circle info into carry.
	x -= 1 << qN;		// sine -> cosine calc

	x = x << (31 - qN);			// Mask with PI
	x = x >> (31 - qN);			// Note: SIGNED shift! (to qN)
	x = x * x >> (2 * qN - 14); // x=x^2 To Q14

	y = B - (x * C >> 14);		   // B - x^2*C
	y = (1 << qA) - (x * y >> 16); // A - x^2*(B-x^2*C)
	y = y << 12;				   // to 8.24
	return c >= 0 ? y : -y;
}
static inline int32_t _IQ24cosPU(int32_t inAngle)
{
	return _IQ24sinPU(inAngle + _IQ24(0.25));
}

static inline int32_t _IQ24sin(int32_t x)
{
	x = _IQmpy(x, _IQ(0.15915494309189533576));
	return _IQ24sinPU(x);
}

static inline int32_t _IQ24cos(int32_t x)
{
	x = _IQmpy(x, _IQ(0.15915494309189533576));
	return _IQ24sinPU(x + _IQ24(0.25));
}

/** Функция atan2.
 * http://www.dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization
 * 94 cpu cycles 
 * */

inline int32_t _IQ24atan2PU(int32_t inY , int32_t inX) {
    int32_t abs_inY, angle, r, r_3;

    abs_inY = abs(inY)+1;// kludge to prevent 0/0 condition

    if (inX >= 0) {
        r = _IQ24div( (inX - abs_inY), (inX + abs_inY));
        r_3=_IQmpy(_IQmpy(r,r),r);
        angle = _IQmpy(_IQ(ATAN2_K0),r_3)-_IQmpy(_IQ(ATAN2_K1),r)+_IQ(ATAN2_K2);
    } else {
        r = _IQ24div( (inX + abs_inY), (abs_inY - inX));
        r_3=_IQmpy(_IQmpy(r,r),r);
        angle = _IQmpy(_IQ(ATAN2_K0),r_3)-_IQmpy(_IQ(ATAN2_K1),r)+_IQ(ATAN2_K3);
    }
    if (inY < 0) {
        angle = -angle;
    }
    angle=angle&0xFFFFFF;
    return angle;
}

inline _iq _IQsqrt(_iq v)
{
	_iq temp, nHat = 0, b = 32768, bshft = 15;
	do
	{
		if (v >= (temp = (((nHat << 1) + b) << bshft--)))
		{
			nHat += b;
			v -= temp;
		}
	} while (b >>= 1);
	return nHat;
}

//! Квадратный корень в формате 8.24. Работает через аппаратный float, занимает 43 такта.
//! В CodeMaster нет функции sqrtf, поэтому для него вызывается sqrt. Для GCC вызывать
//! sqrt нельзя, т.к. тогда он попытается работать с double, а не float.

inline int32_t _IQ24sqrtf(int32_t number)
{
	return _IQ(sqrtf((float)_IQ24toF(number)));
}

inline _iq _IQmag(_iq x, _iq y)
{
	return _IQsqrt(_IQmpy(x, x) + _IQmpy(y, y));
}
