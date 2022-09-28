/*!
 * \file
 * \brief Generates a message.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifdef LINK_FFTW

#ifndef SPECTRUM_HPP_
#define SPECTRUM_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include <fftw3.h>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Spectrum
 *
 * \brief Compute the averaged power spectrum of the input signal.
 *
 */
class Spectrum : public Module
{

protected:
	const int     N; /*!< Input size*/
	const int     W; /*!< Input size*/
	const int     P;
	const float   alpha; /*!< Coeficient, default is 0.0 */
	const int     Nfft; /*!< Size of the FFT, default is 1024 */
	const float   Fs; /*!< Sampling frequency, default is 1.0 */

private:
	std::vector<float> window; /*!< window */
	std::vector<float> current_window; /*!< current window */
	std::vector<float> current_spectrum; /*!< current spectrum */
	std::vector<float> averaged_spectrum; /*!< held spectrum */
	std::vector<float> freq;     /*!< frequency */
	fftwf_plan fft_plan;

public:
	/*!
	 * \brief Constructor.
	 */
	Spectrum(const int N, const int W, const float alpha = 0.0f, const float Fs = 1.0f, const int Nfft = 1024);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Spectrum() = default;

	//virtual Spectrum<float>* clone() const;

	void get_spectrum(float* spectrum);
	void get_freq    (float* freq    );
	int  get_Nfft    (               ) const;

protected:
	virtual void _analyze  (const float *X, float *F, float *S, const int frame_id);
};
}
}

#endif /* SPECTRUM_HPP_ */
#endif /* LINK_FFTW */
