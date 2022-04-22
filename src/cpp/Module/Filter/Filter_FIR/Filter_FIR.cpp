#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR.hpp"
using namespace aff3ct::module;

template <typename R>
Filter_FIR<R>
::Filter_FIR(const int N, const std::vector<R> h)
: Filter<R>(N, N), h(h)
{
}

template <typename R>
Filter_FIR<R>
::~Filter_FIR()
{}


template <typename R>
Filter_FIR<R>* Filter_FIR<R>
::clone() const
{
	auto m = new Filter_FIR(*this);
	m->deep_copy(*this);
	return m;
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_FIR<float>;
template class aff3ct::module::Filter_FIR<double>;
// ==================================================================================== explicit template instantiation
