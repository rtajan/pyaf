#ifndef FILTER_FIR_CCR_HPP
#define FILTER_FIR_CCR_HPP

#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_FIR_ccr : public Filter_FIR<R>
{
protected:
	std::vector<std::complex<R>> buff;
	int head;
	std::vector<R> b; // reversed coefs

public:
	Filter_FIR_ccr (const int N, const std::vector<R> h);
	virtual ~Filter_FIR_ccr();

	virtual Filter_FIR_ccr<R>* clone() const;
	inline void step(const R* x_elt, R* y_elt);

protected:
	virtual void _filter(const R *X_N1,  R *Y_N2, const int frame_id);
	virtual void _reset();
};

// Adrien: I put this function here because you wanted to be inlined
template <typename R>
void Filter_FIR_ccr<R>
::step(const R* x_elt, R* y_elt)
{
	auto cx_elt = reinterpret_cast<const std::complex<R>* >(x_elt);
	auto cy_elt = reinterpret_cast<      std::complex<R>* >(y_elt);
	int sz = this->h.size();

	this->buff[this->head     ] = *cx_elt;
	this->buff[this->head + sz] = *cx_elt;

	*cy_elt = this->buff[this->head+1] * this->b[0];
	for (auto i = 1; i < sz ; i++)
		*cy_elt += this->buff[this->head + 1 + i] * this->b[i];

	this->head++;
	this->head %= sz;
}

}
}

#endif //FILTER_FIR_CCR_HPP
