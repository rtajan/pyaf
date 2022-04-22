#ifndef FILTER_FIR_CCR_FAST_HPP
#define FILTER_FIR_CCR_FAST_HPP

#include <vector>
#include <complex>

#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Filter_FIR_ccr_fast : public Filter_FIR_ccr<R>
{
protected:
	int M;
	int P;

public:
	Filter_FIR_ccr_fast (const int N, const std::vector<R> b);
	virtual ~Filter_FIR_ccr_fast();
	virtual Filter_FIR_ccr_fast<R>* clone() const;

protected:
	virtual void _filter(const R *X_N1,  R *Y_N2, const int frame_id);
	virtual void _filter_old(const R *X_N1,  R *Y_N2, const int frame_id);

};

}
}

#endif //FILTER_FIR_CCR_FAST_HPP
