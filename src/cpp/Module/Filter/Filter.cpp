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

#include "Module/Filter/Filter.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
Filter<R>::
Filter(const int N, const int N_fil)
: Module(), N(N), N_fil(N_fil)
{
	const std::string name = "Filter";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_fil <= 0)
	{
		std::stringstream message;
		message << "'N_fil' has to be greater than 0 ('N_fil' = " << N_fil << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init_processes();
}

template <typename R>
void Filter<R>::
init_processes()
{
	auto &p1 = this->create_task("filter");
	auto p1s_X_N1 = this->template create_socket_in <R>(p1, "X_N1", this->N    );
	auto p1s_Y_N2 = this->template create_socket_out<R>(p1, "Y_N2", this->N_fil);
	this->create_codelet(p1, [p1s_X_N1, p1s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		static_cast<Filter<R>&>(m)._filter(static_cast<R*>(t[p1s_X_N1].get_dataptr()),
		                                   static_cast<R*>(t[p1s_Y_N2].get_dataptr()),
										   frame_id);

		return 0;
	});

}

template <typename R>
int Filter<R>::
get_N() const
{
	return this->N;
}

template <typename R>
int Filter<R>::
get_N_fil() const
{
	return this->N_fil;
}

template <typename R>
template <class AR>
void Filter<R>::
filter(const std::vector<R,AR>& X_N1, std::vector<R,AR>& Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)["filter::X_N1"].bind(X_N1);
	(*this)["filter::Y_N2"].bind(Y_N2);
	(*this)("filter").exec(frame_id, managed_memory);
}

template <typename R>
Filter<R>* Filter<R>
::clone() const
{
	auto m = new Filter(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Filter<R>::
filter(const R *X_N1, R *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)["filter::X_N1"].bind(X_N1);
	(*this)["filter::Y_N2"].bind(Y_N2);
	(*this)("filter").exec(frame_id, managed_memory);
}

template <typename R>
void Filter<R>::
reset()
{
	this->_reset();
}

template <typename R>
void Filter<R>::
step(const R *X_elt, R *Y_elt)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
void Filter<R>::
_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
void Filter<R>::
_reset()
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter<float>;
template class aff3ct::module::Filter<double>;
// ==================================================================================== explicit template instantiation

