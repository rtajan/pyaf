#ifndef FILTER_FIR_HPP
#define FILTER_FIR_HPP

#include <vector>
#include <complex>

#include "Module/Filter/Filter.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_FIR : public Filter<R>
{
protected:
	std::vector<R> h; // the filter impulse response

public:
	Filter_FIR (const int N, const std::vector<R> h);
	virtual ~Filter_FIR();

	virtual Filter_FIR<R>* clone() const;

	std::vector<R> get_coefs() {return this->h;};
};
}
}

#endif //FILTER_FIR_HPP
