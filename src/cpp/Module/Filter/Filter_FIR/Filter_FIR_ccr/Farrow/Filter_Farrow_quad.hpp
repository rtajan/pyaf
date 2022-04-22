#ifndef FILTER_FARROW_QUAD_HPP
#define FILTER_FARROW_QUAD_HPP

#include <cassert>
#include <iostream>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_Farrow_quad : public Filter_FIR_ccr_fast<R>
{
protected:
	R mu;
	std::vector<R> Y1_N1;
	std::vector<R> Y2_N1;
	std::vector<R> Y3_N1;

public:

	Filter_Farrow_quad (const int N, const R mu = 0.0f);
	virtual ~Filter_Farrow_quad();

	inline void step(const R* x_elt, R* y_elt);
	inline void step_flt_bank(const  R* x_elt,
	                                 R* y1_elt,
	                                 R* y2_elt,
	                                 R* y3_elt);

	void filter_bank(const R *X_N1, R *Y1_N2, R *Y2_N2, R *Y3_N2);

	void set_mu(R mu);
protected:
	void _filter(const R *X_N1, R *Y_N2, const int frame_id);

};
}
}

#include "Filter_Farrow_quad.hxx"

#endif //FILTER_FARROW_QUAD_HPP