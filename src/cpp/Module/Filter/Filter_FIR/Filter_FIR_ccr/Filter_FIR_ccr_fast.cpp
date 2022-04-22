#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr_fast.hpp"
using namespace aff3ct::module;

template <typename R>
Filter_FIR_ccr_fast<R>
::Filter_FIR_ccr_fast(const int N, const std::vector<R> b)
: Filter_FIR_ccr<R>(N, b),
M(mipp::N<R>()),
P((N-2*(b.size()-1))/mipp::N<R>())
{
	assert(mipp::N<R>() > 1);

	if (P < 0)
		P = 0;
}

template <typename R>
Filter_FIR_ccr_fast<R>
::~Filter_FIR_ccr_fast()
{}

template <typename R>
Filter_FIR_ccr_fast<R>* Filter_FIR_ccr_fast<R>
::clone() const
{
	auto m = new Filter_FIR_ccr_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Filter_FIR_ccr_fast<R>
::_filter_old(const R *X_N1, R *Y_N2, const int frame_id)
{
	int rest = this->N - this->P * this->M;

	for(auto i = 0; i < rest; i+=2)
		this->step(&X_N1[i], &Y_N2[i]);

	mipp::Reg<R> ps;
	mipp::Reg<R> reg_x;
	mipp::Reg<R> reg_b;
	size_t b_size = this->b.size();
	for(auto i = rest ; i < this->N ; i += this->M)
	{
		ps = (R)0;
		for(size_t k = 0; k < b_size ; k++)
		{
			reg_b = this->b[k];
			reg_x.load(X_N1 + i - 2*(b_size - 1 - k));
			ps = mipp::fmadd(reg_b, reg_x, ps); // same as 'ps += reg_b * reg_x'
		}
		ps.store(Y_N2 + i);
	}

	if (P > 0)
	{
		int sz = this->N/2;
		auto cX_N1 = reinterpret_cast<const std::complex<R>* >(X_N1);
		std::copy(&cX_N1[sz - b_size], &cX_N1[sz], &this->buff[0]);
		std::copy(&cX_N1[sz - b_size], &cX_N1[sz], &this->buff[b_size]);
		this->head = 0;
	}
}

template <typename R>
void Filter_FIR_ccr_fast<R>
::_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
	int rest = this->N - this->P * this->M;

	for(auto i = 0; i < rest; i+=2)
		this->step(&X_N1[i], &Y_N2[i]);

	mipp::Reg<R> ps = (R)0;
	mipp::Reg<R> ps0;
	mipp::Reg<R> ps1;
	mipp::Reg<R> ps2;
	mipp::Reg<R> ps3;

	mipp::Reg<R> reg_x0;
	mipp::Reg<R> reg_x1;
	mipp::Reg<R> reg_x2;
	mipp::Reg<R> reg_x3;

	mipp::Reg<R> reg_b0;
	mipp::Reg<R> reg_b1;
	mipp::Reg<R> reg_b2;
	mipp::Reg<R> reg_b3;

	size_t b_size = this->b.size();
	size_t b_size_unrolled4 = (b_size / 4) * 4;

	for (auto i = rest; i < this->N ; i += this->M)
	{
		ps0 = (R)0;
		ps1 = (R)0;
		ps2 = (R)0;
		ps3 = (R)0;

		for (size_t k = 0; k < b_size_unrolled4; k += 4)
		{
			reg_b0 = this->b[k +0];
			reg_b1 = this->b[k +1];
			reg_b2 = this->b[k +2];
			reg_b3 = this->b[k +3];

			reg_x0 = &X_N1[-2 * (b_size -1) + i + 2 * (k + 0)];
			reg_x1 = &X_N1[-2 * (b_size -1) + i + 2 * (k + 1)];
			reg_x2 = &X_N1[-2 * (b_size -1) + i + 2 * (k + 2)];
			reg_x3 = &X_N1[-2 * (b_size -1) + i + 2 * (k + 3)];

			ps0 = mipp::fmadd(reg_b0, reg_x0, ps0); // same as 'ps0 += reg_b0 * reg_x0'
			ps1 = mipp::fmadd(reg_b1, reg_x1, ps1); // same as 'ps1 += reg_b1 * reg_x1'
			ps2 = mipp::fmadd(reg_b2, reg_x2, ps2); // same as 'ps2 += reg_b2 * reg_x2'
			ps3 = mipp::fmadd(reg_b3, reg_x3, ps3); // same as 'ps3 += reg_b3 * reg_x3'
		}

		ps0 += ps1;
		ps2 += ps3;
		ps = ps0 + ps2;

		for (size_t k = b_size_unrolled4; k < b_size; k++)
		{
			reg_b0 = this->b[k];
			reg_x0.load(X_N1 - 2 * (b_size -1) + i + (2 * k));
			ps = mipp::fmadd(reg_b0, reg_x0, ps); // same as 'ps += reg_b0 * reg_x0'
		}

		ps.store(Y_N2 + i);
	}

	if (P > 0)
	{
		int sz = this->N/2;
		auto cX_N1 = reinterpret_cast<const std::complex<R>* >(X_N1);
		std::copy(&cX_N1[sz - b_size], &cX_N1[sz], &this->buff[0]);
		std::copy(&cX_N1[sz - b_size], &cX_N1[sz], &this->buff[b_size]);
		this->head = 0;
	}

}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_FIR_ccr_fast<float>;
template class aff3ct::module::Filter_FIR_ccr_fast<double>;
// ==================================================================================== explicit template instantiation
