#ifndef FILTER_FARROW_CCR_HPP
#define FILTER_FARROW_CCR_HPP

#include <cassert>
#include <iostream>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_Farrow_ccr : public Filter_FIR_ccr<R>
{
private:
	R mu;

public:

	Filter_Farrow_ccr (const int N, const R mu, const int n_frames = 1);
	virtual ~Filter_Farrow_ccr();
	inline void set_mu(R mu);
	inline void step(const std::complex<R>* x_elt, std::complex<R>* y_elt);
	inline void redo_step(R new_mu, std::complex<R>* y_elt);
};
}
}

#include "Module/Filter/Filter_FIR/Farrow/Filter_Farrow_ccr.hxx"

#endif //FILTER_FARROW_CCR_HPP