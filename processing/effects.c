#include "effects.h"



/* --- Setup --- */

void setupFxLowpass(fxLowpass *fx) { /* set default static values for new lowpass filter */
	fx->val = 0;

	fx->input.val = defParams + 0;
	setupParam(&(fx->input));

	fx->cutoff.val = defParams + 2;
	setupParam(&(fx->cutoff));
}

void setupFxHighpass(fxHighpass *fx) { /* set default static values for new highpass filter */
	fx->val = 0;

	fx->input.val = defParams + 0;
	setupParam(&(fx->input));

	fx->cutoff.val = defParams + 2;
	setupParam(&(fx->cutoff));
}

void setupFxBandpass(fxBandpass *fx) { /* set default static values for new bandpass filter */
	fx->val = 0;

	fx->input.val = defParams + 0;
	setupParam(&(fx->input));

	fx->freq.val = defParams + 2;
	setupParam(&(fx->freq));

	fx->bandwidth.val = defParams + 3;
	setupParam(&(fx->bandwidth));
}



/* --- Computing --- */

void compFxLowpass(fxLowpass *fx) { /* compute current value of a lowpass-filter */
	float coeffOmega = 2.0 * M_PI * compParam(&(fx->cutoff)) / sampleRate;
	float coeff = (2.0 - cos(coeffOmega)) - sqrt(pow(2.0 - cos(coeffOmega), 2.0) - 1.0);
	
	fx->val = (1 - coeff) * compParam(&(fx->input)) + coeff * fx->val;
}

void compFxHighpass(fxHighpass *fx) { /* compute current value of a highpass-filter */
	float in = compParam(&(fx->input));
	float coeffOmega = 2.0 * M_PI * compParam(&(fx->cutoff)) / sampleRate;
	float coeff = (2.0 - cos(coeffOmega)) - sqrt(pow(2.0 - cos(coeffOmega), 2.0) - 1.0);
			
	fx->tmp = (1 - coeff) * in + coeff * fx->tmp;

	fx->val = in - fx->tmp; /* TODO: is ths working proper (values between -1 and 1??)? */
}

void compFxBandpass(fxBandpass *fx) { /* compute current value of a bandpass-filter */
	float in = compParam(&(fx->input));
	float freq = compParam(&(fx->freq));
	float width = compParam(&(fx->bandwidth)) / 2.0;

	float hpCoeffOmega = 2.0 * M_PI * (freq - width) / sampleRate;
	float hpCoeff = (2.0 - cos(hpCoeffOmega)) - sqrt(pow(2.0 - cos(hpCoeffOmega), 2.0) - 1.0);

	float lpCoeffOmega = 2.0 * M_PI * (freq + width) / sampleRate;
	float lpCoeff = (2.0 - cos(lpCoeffOmega)) - sqrt(pow(2.0 - cos(lpCoeffOmega), 2.0) - 1.0);
			
	fx->tmp = (1 - hpCoeff) * in + hpCoeff * fx->tmp;

	fx->val = (1 - lpCoeff) * (in - fx->tmp) + lpCoeff * fx->val; /* TODO: same as highpass... */
}
