#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr.hpp"
using namespace aff3ct::module;

template <typename R>
Filter_FIR_ccr<R>
::Filter_FIR_ccr(const int N, const std::vector<R> h)
: Filter_FIR<R>(N, h),
buff(4*h.size(), R(0)),
head(0),
b(h.size(), R(0))
{
	for (size_t i = 0; i < h.size(); i++)
		this->b[i] = h[h.size() - 1 - i];
}

template <typename R>
Filter_FIR_ccr<R>
::~Filter_FIR_ccr()
{}

template <typename R>
void Filter_FIR_ccr<R>
::_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
	for(auto i = 0; i < this->N; i+=2)
		step(&X_N1[i], &Y_N2[i]);
}

template <typename R>
void Filter_FIR_ccr<R>
::_reset()
{
	for (size_t i = 0; i < this->buff.size(); i++)
		this->buff[i] = std::complex<R>(R(0),R(0));

	this->head = 0;
}

template <typename R>
Filter_FIR_ccr<R>* Filter_FIR_ccr<R>
::clone() const
{
	auto m = new Filter_FIR_ccr(*this);
	m->deep_copy(*this);
	return m;
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_FIR_ccr<float>;
template class aff3ct::module::Filter_FIR_ccr<double>;
// ==================================================================================== explicit template instantiation
