#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Filter_Farrow_quad.hpp"

using namespace aff3ct::module;

template <typename R>
Filter_Farrow_quad<R>
::Filter_Farrow_quad(const int N, const R mu)
: Filter_FIR_ccr_fast<R>(N, std::vector<R>(4,(R)0)), mu(mu),
Y1_N1(N, (R)0), Y2_N1(N, (R)0), Y3_N1(N, (R)0)
{
	this->set_mu(mu);
}

template <typename R>
Filter_Farrow_quad<R>
::~Filter_Farrow_quad()
{
}

template <typename R>
void Filter_Farrow_quad<R>
::_filter(const R* X_N1, R* Y_N2, const int frame_id)
{
	this->filter_bank(X_N1, Y1_N1.data(), Y2_N1.data(), Y3_N1.data());

	R mu  = this->mu;
	R mu2 = mu * mu;
	for (auto i=0; i<this->N; i++)
		Y_N2[i]  = mu2 * Y1_N1[i] + mu * Y2_N1[i] + Y3_N1[i];
}

template <typename R>
void Filter_Farrow_quad<R>
::filter_bank(const R *X_N1, R *Y1_N2, R *Y2_N2, R *Y3_N2)
{
	auto cX_N1  = reinterpret_cast<const std::complex<R>* >(X_N1 );

	int rest = this->N - this->P * this->M;

	for(auto i = 0; i < rest/2; i++)
		step_flt_bank(&X_N1[2*i], &Y1_N2[2*i], &Y2_N2[2*i], &Y3_N2[2*i]);

	mipp::Reg<R> ps;

	mipp::Reg<R> reg_x0;
	mipp::Reg<R> reg_x1;
	mipp::Reg<R> reg_x2;
	mipp::Reg<R> reg_x3;
	mipp::Reg<R> reg_3 = (R)3.0f;

	for (auto i = rest; i < this->N ; i += this->M)
	{
		reg_x0 = &X_N1[i - 6];
		reg_x1 = &X_N1[i - 4];
		reg_x2 = &X_N1[i - 2];
		reg_x3 = &X_N1[i    ];

		ps = reg_x0 - reg_x1 - reg_x2 + reg_x3;
		ps *= 0.5;
		ps.store(Y1_N2 + i);

		ps = reg_3*reg_x1 - (reg_x3 + reg_x0 + reg_x2);
		ps *= 0.5;
		ps.store(Y2_N2 + i);

		ps = reg_x2;
		ps.store(Y3_N2 + i);
	}

	int sz = this->N/2;
	std::copy(&cX_N1[sz - 4], &cX_N1[sz], &this->buff[0]);
	std::copy(&cX_N1[sz - 4], &cX_N1[sz], &this->buff[4]);
	this->head = 0;
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_Farrow_quad<float>;
template class aff3ct::module::Filter_Farrow_quad<double>;
// ==================================================================================== explicit template instantiation