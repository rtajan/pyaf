#ifndef FILTER_ROOT_RAISED_COSINE_HPP
#define FILTER_ROOT_RAISED_COSINE_HPP

#include <vector>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_root_raised_cosine : public Filter_FIR_ccr_fast<R>
{
private:
	const R   rolloff;
	const int samples_per_symbol;
	const int delay_in_symbol;

public:
	Filter_root_raised_cosine (const int N, const R rolloff = (R)0.05, const int samples_per_symbol = 4, const int delay_in_symbol = 50);
	virtual ~Filter_root_raised_cosine();
	virtual Filter_root_raised_cosine<R>* clone() const;
	static std::vector<R> synthetize(const R rolloff, const int samples_per_symbol, const int delay_in_symbol);
};
}
}

#endif //FILTER_ROOT_RAISED_COSINE_HPP
