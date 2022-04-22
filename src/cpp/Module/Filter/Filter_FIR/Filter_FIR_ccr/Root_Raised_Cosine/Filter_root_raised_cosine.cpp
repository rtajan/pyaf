#include <iostream>
#include <complex>
#include <cmath>
#include <limits>

#include "Filter_root_raised_cosine.hpp"
using namespace aff3ct::module;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

template <typename R>
std::vector<R> Filter_root_raised_cosine<R>
::synthetize(const R rolloff, const int samples_per_symbol, const int delay_in_symbol)
{
	std::vector<R> coefs (2*delay_in_symbol*samples_per_symbol + 1, R(0));
	R eps = std::numeric_limits<R>::epsilon();

	coefs[delay_in_symbol*samples_per_symbol] = (R(1.0) - rolloff + R(4.0)*rolloff/(R)M_PI);
	R energie(coefs[delay_in_symbol*samples_per_symbol] * coefs[delay_in_symbol*samples_per_symbol]);
	for (int i = 1 ; i < delay_in_symbol*samples_per_symbol+1 ; i++)
	{
		R t = (R)i/(R)samples_per_symbol;
		R value(0);
        if ( std::abs(R(4.0)*rolloff*t - (R)1.0) <=  eps || std::abs(R(4)*rolloff*t + (R)1.0) <= eps )
		{
			value = rolloff/sqrt((R)2.0)*( ((R)1.0+(R)2.0/(R)M_PI)*sin((R)M_PI /((R)4.0*rolloff))+
			                               ((R)1.0-(R)2.0/(R)M_PI)*cos((R)M_PI /((R)4.0*rolloff))
			                             );
		}
        else
        {
            R denom = (R)M_PI * t * ((R)1.0 - (R)16.0*rolloff*rolloff*t*t);
            R numer = sin((R)M_PI * t * ((R)1.0 - rolloff)) + (R)4.0*rolloff*t*cos((R)M_PI * t * ((R)1.0 + rolloff));
            value   = numer/denom;

        }
		coefs[delay_in_symbol*samples_per_symbol + i] = value;
		coefs[delay_in_symbol*samples_per_symbol - i] = value;
		energie += value*value + value*value;
	}

	for (size_t i = 0 ; i < coefs.size() ; i++)
		coefs[i] /= std::sqrt(energie);

	return coefs;
}

template <typename R>
Filter_root_raised_cosine<R>
::Filter_root_raised_cosine(const int N, const R rolloff, const int samples_per_symbol, const int delay_in_symbol)
: Filter_FIR_ccr_fast<R>(N, Filter_root_raised_cosine<R>::synthetize(rolloff, samples_per_symbol, delay_in_symbol)), rolloff(rolloff), samples_per_symbol(samples_per_symbol), delay_in_symbol(delay_in_symbol)
{
}

template <typename R>
Filter_root_raised_cosine<R>
::~Filter_root_raised_cosine(){}

template <typename R>
Filter_root_raised_cosine<R>* Filter_root_raised_cosine<R>
::clone() const
{
	auto m = new Filter_root_raised_cosine(*this);
	m->deep_copy(*this);
	return m;
}
// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_root_raised_cosine<float>;
template class aff3ct::module::Filter_root_raised_cosine<double>;
// ==================================================================================== explicit template instantiation
