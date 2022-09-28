#ifndef Filter_root_raised_cosine_ccr_HPP
#define Filter_root_raised_cosine_ccr_HPP

#include <vector>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_root_raised_cosine_ccr : public Filter_FIR_ccr<R>
{
private:
	const R   rolloff;
	const int samples_per_symbol;
	const int delay_in_symbol;


public:
	static std::vector<R> synthetize(const R rolloff, const int samples_per_symbol, const int delay_in_symbol);
	Filter_root_raised_cosine_ccr (const int N, const R rolloff = 0.05f, const int samples_per_symbol = 4, const int delay_in_symbol = 50, const int n_frames= 1);
	virtual ~Filter_root_raised_cosine_ccr();
};
}
}

#endif //Filter_root_raised_cosine_ccr_HPP
