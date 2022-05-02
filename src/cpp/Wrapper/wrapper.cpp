#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>

#include <vector>

#include "Module/Filter/Filter.hpp"
#include "Module/Filter/Filter_FIR/Filter_FIR.hpp"
#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr.hpp"
#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr_fast.hpp"
#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Root_Raised_Cosine/Filter_root_raised_cosine.hpp"
#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Farrow/Filter_Farrow_quad.hpp"

#include "Module/Filter/Filter_UPFIR/Filter_UPFIR.hpp"

#include "Module/Upsampling/Upsampling_cplx.hpp"

namespace py = pybind11;
using namespace py::literals;
using namespace aff3ct;

// Create a	python module using PYBIND11, here our module will be named pyaf
PYBIND11_MODULE(pyaf, m){
	py::scoped_ostream_redirect stream_cout(
	std::cout,                                // std::ostream&
	py::module_::import("sys").attr("stdout") // Python output
	);
	py::scoped_ostream_redirect stream_cerr(
	std::cerr,                                // std::ostream&
	py::module_::import("sys").attr("stderr") // Python output
	);

	// Import AFF3CT module from py_AFF3CT
	py::object py_aff3ct_module = (py::object) py::module_::import("py_aff3ct").attr("module").attr("Module");


	// Create a submodule, here for upsampling (optionnal)
	py::module_ m_upsampling = m.def_submodule("upsampling");
	py::class_<aff3ct::module::Upsampling_cplx>(m_upsampling,"Upsampling_cplx", py_aff3ct_module)
		.def(py::init<const int, const int>(), "N"_a, "F"_a);


	// Create a submodule, here for filter (optionnal)
	py::module_ m_filter = m.def_submodule("filter");

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter<float>" class.
	// py_aff3ct_module is here to indicate to pybind11 that aff3ct::module exists in py_AFF3CT
	py::class_<aff3ct::module::Filter<float>>(m_filter,"Filter", py_aff3ct_module)
		.def(py::init<const int, const int>(), "N"_a, "N_fil"_a);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_FIR<float>" class.
	// aff3ct::module::Filter<float> is here to indicate inheritance
	py::class_<aff3ct::module::Filter_FIR<float>, aff3ct::module::Filter<float>>(m_filter,"Filter_FIR")
		.def(py::init<const int, const std::vector<float>>(), "N"_a, "h"_a);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_FIR_ccr<float>" class.
	py::class_<aff3ct::module::Filter_FIR_ccr<float>, aff3ct::module::Filter_FIR<float>>(m_filter,"Filter_FIR_ccr")
		.def(py::init<const int, const std::vector<float>>(), "N"_a, "h"_a);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_FIR_ccr_fast<float>" class.
	py::class_<aff3ct::module::Filter_FIR_ccr_fast<float>, aff3ct::module::Filter_FIR_ccr<float>>(m_filter,"Filter_FIR_ccr_fast")
		.def(py::init<const int, const std::vector<float>>(), "N"_a, "h"_a);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_root_raised_cosine<float>" class.
	py::class_<aff3ct::module::Filter_root_raised_cosine<float>, aff3ct::module::Filter_FIR_ccr_fast<float>>(m_filter,"Filter_root_raised_cosine")
		.def(py::init<const int,  const float, const int, const int>(), "N"_a, "rolloff"_a = 0.05, "samples_per_symbol"_a = 4, "delay_in_symbol"_a = 50)
		.def_static("synthetize", &aff3ct::module::Filter_root_raised_cosine<float>::synthetize);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_UPFIR<float>" class.
	py::class_<aff3ct::module::Filter_UPFIR<float, aff3ct::module::Filter_FIR_ccr>, aff3ct::module::Filter<float>>(m_filter,"Filter_UPFIR")
		.def(py::init<const int, const std::vector<float>, const int>(), "N"_a, "h"_a, "osf"_a = 1);

	// Bind a custom class, here is the binding for the "aff3ct::module::Filter_Farrow_quad<float>" class.
	py::class_<aff3ct::module::Filter_Farrow_quad<float>, aff3ct::module::Filter_FIR_ccr_fast<float>>(m_filter,"Filter_Farrow_quad")
		.def(py::init<const int, const float>(), "N"_a, "mu"_a=0.0);

}