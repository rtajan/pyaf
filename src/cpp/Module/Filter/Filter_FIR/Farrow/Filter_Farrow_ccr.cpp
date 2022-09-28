#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Farrow/Filter_Farrow_ccr.hpp"

using namespace aff3ct::module;

template <typename R>
Filter_Farrow_ccr<R>
::Filter_Farrow_ccr(const int N, const R mu, const int n_frames)
: Filter_FIR_ccr<R>(N, std::vector<R>(4,(R)0.0), n_frames), mu(mu)
{
	this->set_mu(mu);
}

template <typename R>
Filter_Farrow_ccr<R>
::~Filter_Farrow_ccr()
{
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_Farrow_ccr<float>;
template class aff3ct::module::Filter_Farrow_ccr<double>;
// ==================================================================================== explicit template instantiation