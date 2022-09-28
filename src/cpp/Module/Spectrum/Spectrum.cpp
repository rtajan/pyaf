/*!
 * \file
 * \brief Computes the spectrum of a complex signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifdef LINK_FFTW
#include <sstream>
#include <complex>

#include "Tools/Exception/exception.hpp"

#include "Module/Spectrum/Spectrum.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Spectrum::
Spectrum(const int N, const int W, const float alpha, const float Fs, const int Nfft)
: Module(), N(N), W(W), P(std::ceil(Nfft/2)), alpha(alpha), Nfft(Nfft), Fs(Fs), window(W,0.0f), current_window(2*Nfft,0.0f), current_spectrum(2*Nfft,0.0f), averaged_spectrum(Nfft,0.0f), freq(Nfft),
  fft_plan()
{
	float a0 = 0.35875;
	float a1 = 0.48829;
	float a2 = 0.14128;
	float a3 = 0.01168;

	for (auto i = 0; i < W; i++)
	{
		// window[i  ] = 0.54-0.46*cos(2*M_PI*i/(N/2));
		// window[i+1] = 0.54-0.46*cos(2*M_PI*i/(N/2));
		float w = a0 - a1*cos(2*M_PI*i/W) + a2*cos(4*M_PI*i/W) - a3*cos(6*M_PI*i/W);
		window[i] = w;
	}

	for (auto i = 0; i < P; i++)
	{
		freq[i]  = (float)(P+i-Nfft)/(float)Nfft;
		freq[i] *= Fs;
	}
	for (auto i = P; i < this->Nfft; i++)
	{
		freq[i] = (float)(i-P)/(float)Nfft;
		freq[i] *= Fs;
	}



	this->fft_plan = fftwf_plan_dft_1d(Nfft, reinterpret_cast<fftwf_complex*>(current_window.data()), reinterpret_cast<fftwf_complex*>(current_spectrum.data()), FFTW_FORWARD, FFTW_ESTIMATE);
	const std::string name = "Spectrum";
	this->set_name(name);
	this->set_short_name(name);

	auto &p1 = this->create_task("analyze");
	auto p1s_X = this->template create_socket_in <float>(p1, "X", this->N   );
	auto p1s_F = this->template create_socket_out<float>(p1, "F", this->Nfft);
	auto p1s_S = this->template create_socket_out<float>(p1, "S", this->Nfft);
	this->create_codelet(p1, [p1s_X, p1s_F, p1s_S](Module &m, Task &t, const size_t frame_id) -> int
	{
		static_cast<Spectrum&>(m)._analyze(static_cast<float*>(t[p1s_X].get_dataptr()),
		                                   static_cast<float*>(t[p1s_F].get_dataptr()),
										   static_cast<float*>(t[p1s_S].get_dataptr()),
										   frame_id);
		return 0;
	});
}

int Spectrum::
get_Nfft() const
{
	return Nfft;
}
void Spectrum::
get_spectrum(float *spectrum)
{
	std::copy(this->averaged_spectrum.begin(), this->averaged_spectrum.end(), spectrum);
}

void Spectrum::
get_freq(float *freq)
{
	std::copy(this->freq.begin(), this->freq.end(), freq);
}

void Spectrum::
_analyze(const float *X, float *F, float *S, const int frame_id)
{
	int idx = 0;
	for (auto p = 0; p < this->N - 2*this->W; p+= current_window.size())
	{
		std::copy(X + idx, X + idx + 2*this->W, &current_window[0]);
		for (auto i = 0; i < this->W; i++)
		{
			current_window[2*i  ] *= this->window[i];
			current_window[2*i+1] *= this->window[i];
		}

		fftwf_execute(fft_plan);

		for (auto i = 0; i < P; i++)
			S[Nfft - P + i] = alpha * S[Nfft - P + i] + (1-alpha)*(current_spectrum[2*i]*current_spectrum[2*i]+current_spectrum[2*i+1]*current_spectrum[2*i+1]);

		for (auto i = P; i < this->Nfft; i++)
			S[       i - P] = alpha * S[       i - P] + (1-alpha)*(current_spectrum[2*i]*current_spectrum[2*i]+current_spectrum[2*i+1]*current_spectrum[2*i+1]);
		idx += 2*this->W;
	}
	std::copy(this->freq.begin(), this->freq.end(), F);
}

#endif //LINK_FFTW
