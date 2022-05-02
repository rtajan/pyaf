/*!
 * \file
 * \brief Filters a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef UPSAMPLING_CPLX
#define UPSAMPLING_CPLX

#include <vector>

#include "Module/Module.hpp"
#include "Tools/Interface/Interface_reset.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Upsampling_cplx
 *
 * \brief Upsamples a complex signal.
 *
 */
class Upsampling_cplx : public Module
{
protected:
	const int N;     /*!< Size of one frame (= number of samples in one frame) */
	const int F;     /*!< Upsampling factor */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame (= number of samples in one frame).
	 * \param F:        Upsampling factor
	 */
	Upsampling_cplx(const int N, const int F);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Upsampling_cplx() = default;

protected:
	virtual void _upsample(const float *X_N1,  float *Y_N2, const int frame_id);
};
}
}

#endif /* UPSAMPLING_CPLX */
