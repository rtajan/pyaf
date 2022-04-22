#include "Module/Filter/Filter_FIR/Filter_FIR_ccr/Filter_FIR_ccr.hpp"
#include "Module/Filter/Filter_UPFIR/Filter_UPFIR.hpp"

using namespace aff3ct::module;

template <typename R, template <typename> class fltr>
Filter_UPFIR<R, fltr>
::Filter_UPFIR(const int N, const std::vector<R> h, const int F)
: Filter<R>(N, F * N), F(F), H(), flt_bank()
{
	this->H.resize(F);
	for(size_t i=0; i < h.size(); i++)
		this->H[i % F].push_back(h[i]);

	for (auto f=0;f<F; f++)
	{
		auto ptr = std::shared_ptr<fltr<R>>(new fltr<R>(N,this->H[f]));
		this->flt_bank.push_back(ptr);
	}
		
}

template <typename R, template <typename> class fltr>
Filter_UPFIR<R, fltr>
::~Filter_UPFIR()
{
}

template <typename R, template <typename> class fltr>
void Filter_UPFIR<R, fltr>
::step(const R* x_elt, R* y_F)
{
	for (auto f = 0; f<this->F; f++)
		this->flt_bank[f]->step(x_elt, y_F + f);
}

template <typename R, template <typename> class fltr>
void Filter_UPFIR<R, fltr>
::_reset()
{
	for (auto f = 0; f<this->F; f++)
		this->flt_bank[f]->reset();
}

// template <typename R>
// void Filter_UPFIR<R>
// ::_filter(const R *X_N1, R *Y_N2, const int frame_id)
// {
// 	auto cX_N1 = reinterpret_cast<const std::complex<R>* >(X_N1);
// 	auto cY_N2 = reinterpret_cast<std::complex<R>* >(Y_N2);

// 	for (auto f = 0; f<this->F; f++)
// 		for (auto i = 0; i < this->N/2; i++)
// 			this->flt_bank[f].step(cX_N1 + i, cY_N2 + i*this->F + f);
// }

template <typename R, template <typename> class fltr>
void Filter_UPFIR<R, fltr>
::_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
    std::vector<R> Y_N1 (this->get_N(), (R)0);
    auto cY_N1 = reinterpret_cast<std::complex<R>* >(Y_N1.data());
    auto cY_N2 = reinterpret_cast<std::complex<R>* >(Y_N2);
    for (int f = 0; f<this->F; f++)
    {
        this->flt_bank[f]->filter(X_N1, Y_N1.data());

        for (auto i = 0; i < this->N/2; i++)
            cY_N2[i*this->F + f] = cY_N1[i];
    }
}
template <typename R, template <typename> class fltr>
Filter_UPFIR<R, fltr>* Filter_UPFIR<R, fltr>
::clone() const
{
	auto m = new Filter_UPFIR(*this);
	m->deep_copy(*this);
	return m;
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_UPFIR<float,  aff3ct::module::Filter_FIR_ccr_fast>;
template class aff3ct::module::Filter_UPFIR<double, aff3ct::module::Filter_FIR_ccr_fast>;

template class aff3ct::module::Filter_UPFIR<float,  aff3ct::module::Filter_FIR_ccr>;
template class aff3ct::module::Filter_UPFIR<double, aff3ct::module::Filter_FIR_ccr>;
// ==================================================================================== explicit template instantiation
