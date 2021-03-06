/**
 * Digital 1-st order filters 
 * if f >> f_cut: Amp = sqrt(1 + w^2*Tf^2). In f_cut Amp = -3db = 0.70794578
 * Phase = -arctg(w*T). In f_cut, Phase = 45deg = PI/4 = 0.78539816
 * for LPF: T = Ts / (Tf + Ts), Ts - sample time, Tf - filter time constant
 * for HPF: T = Tf/(Tf + Ts)
 * 
 * cast example:
 * in main: lpf1.T = _IQ(T_S / (1.0f / (M_2PI * 50.0f) + T_S));
 * in ISR: lpf1.in = adcData;
 *  	   LPF_calc(&lpf1);
 *		   filteredData = lpf1.out;
 */
#ifndef __DIGITALFILTERS_H__
#define __DIGITALFILTERS_H__


typedef struct
{
	volatile int32_t in;
	volatile int32_t inPr;
	volatile int32_t out;
	volatile int32_t T; 
} FILTER_DATA;

#define FILTER_DEFAULTS \
	{                   \
		0, 0, 0,        \
	}

static inline void LPF_calc(FILTER_DATA *p)
{
	p->out = _IQmpy(p->T, (p->in - p->out)) + p->out;
}

static inline void HPF_calc(FILTER_DATA *p)
{
	p->out = _IQmpy(p->T, (p->out + (p->in - p->inPr)));
	p->inPr = p->in;
}

#endif

/***************************** END OF FILE ****/
