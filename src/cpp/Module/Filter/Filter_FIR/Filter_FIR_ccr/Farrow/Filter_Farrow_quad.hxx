#include "Filter_Farrow_quad.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
void Filter_Farrow_quad<R>
::step(const R* x_elt, R* y_elt)
{
	std::complex<R> cs1, cs2, cs3;
	auto s1  = reinterpret_cast<R* >(&cs1);
	auto s2  = reinterpret_cast<R* >(&cs2);
	auto s3  = reinterpret_cast<R* >(&cs3);
	step_flt_bank(x_elt, s1, s2, s3);
	y_elt[0] = ((s1[0] * this->mu) + s2[0])* this->mu + s3[0];
	y_elt[1] = ((s1[1] * this->mu) + s2[1])* this->mu + s3[1];
}


template <typename R>
void Filter_Farrow_quad<R>
::step_flt_bank(const R* x_elt, R* y1_elt, R* y2_elt, R* y3_elt)
{
	auto cx_elt  = reinterpret_cast<const std::complex<R>* >(x_elt );
	auto cy1_elt = reinterpret_cast<      std::complex<R>* >(y1_elt);
	auto cy2_elt = reinterpret_cast<      std::complex<R>* >(y2_elt);
	auto cy3_elt = reinterpret_cast<      std::complex<R>* >(y3_elt);

	this->buff[this->head    ] = *cx_elt;
	this->buff[this->head + 4] = *cx_elt;

	*cy1_elt = this->buff[this->head+1] - this->buff[this->head+2] - this->buff[this->head+3] + this->buff[this->head+4];
	y1_elt[0] /= 2;
	y1_elt[1] /= 2;

	*cy2_elt = -this->buff[this->head+1]  + (R)3.0f*this->buff[this->head+2] - this->buff[this->head+3] - this->buff[this->head+4];
	y2_elt[0] /= 2;
	y2_elt[1] /= 2;

	*cy3_elt = this->buff[this->head+3];

	this->head++;
	this->head %= 4;
}

template <typename R>
void Filter_Farrow_quad<R>
::set_mu(R mu_)
{
	this->mu = mu_;
}
}
}
