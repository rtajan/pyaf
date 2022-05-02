/*!
 * \file
 * \brief Filters a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */

#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Upsampling/Upsampling_cplx.hpp"

namespace aff3ct
{
namespace module
{

Upsampling_cplx::
Upsampling_cplx(const int N, const int F)
: Module(), N(N), F(F)
{
	const std::string name = "Upsampling_cplx";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (F <= 0)
	{
		std::stringstream message;
		message << "'F' has to be greater than 0 ('F' = " << F << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("upsample");
	auto p1s_X_N1 = this->template create_socket_in <float>(p1, "X_N1", this->N    );
	auto p1s_Y_N2 = this->template create_socket_out<float>(p1, "Y_N2", this->N * this->N);
	this->create_codelet(p1, [p1s_X_N1, p1s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		static_cast<Upsampling_cplx&>(m)._upsample(static_cast<float*>(t[p1s_X_N1].get_dataptr()),
		                                           static_cast<float*>(t[p1s_Y_N2].get_dataptr()),
										frame_id);

		return 0;
	});
}

void Upsampling_cplx::
_upsample(const float *X_N1, float *Y_N2, const int frame_id)
{
	for (int i = 0; i < this->N/2; i++)
	{
		Y_N2[2*this->F*i   ]  = X_N1[2*i    ];
		Y_N2[2*this->F*i + 1] = X_N1[2*i + 1];
	}
}
}
}
