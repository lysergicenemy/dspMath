/** zero-phase, zero-attenuation filter
     * input: msin - input signal
     *        freq - input signal frequency
     * output: mcos - output filtered signal
     * */

    float tmp_Tf = 1.0f / (M_2PI * freq);     // recalculate filter time constant (continuous time)

    lpf1.in = msin;
    lpf1.T = T_S / (tmp_Tf + T_S);            // time constant in descrete time dominian for LPF
    LPF_calc(&lpf1);						  // calculate simple low-pass filter 

    mcos = lpf1.out * 1.41421603665f;         // in f_cut attenuation is -3db, miltiply on 1 / (-3db)0.70794578

    hpf1.T = tmp_Tf/(tmp_Tf + T_S);           // time constant in descrete time dominian for HPF
    hpf1.in = mcos;
    HPF_calc(&hpf1);						  // calculate simple high-pass filter 

    mcos = hpf1.out * 1.41421603665f;         // final value