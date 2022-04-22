#ifndef FILTER_UPFIR_HPP
#define FILTER_UPFIR_HPP

#include <vector>
#include <complex>


#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr_fast.hpp"

#include "Module/Filter/Filter.hpp"


namespace aff3ct
{
namespace module
{
template <typename R = float, template <typename> class fltr=aff3ct::module::Filter_FIR_ccr_fast>
class Filter_UPFIR : public Filter<R>
{
public:
	Filter_UPFIR(const int N, const std::vector<R> h, const int F = 1);
	virtual ~Filter_UPFIR();
	inline void step  (const R* x_elt, R* y_F);

	virtual Filter_UPFIR<R, fltr>* clone() const;

protected:
	virtual void _reset();
	virtual void _filter(const R *X_N1, R *Y_N2, const int frame_id);

private:
	const int                             F;       // Upsampling Factor
	std::vector<std::vector<R>>           H;       // Impulse response
	std::vector<std::shared_ptr<fltr<R>>> flt_bank;// Filter bank
};
}
}

#endif //FILTER_UPFIR_HPP
