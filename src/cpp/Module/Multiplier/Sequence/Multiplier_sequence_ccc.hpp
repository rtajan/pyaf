#ifndef MULTIPLIER_SEQUENCE_CCC_HPP
#define MULTIPLIER_SEQUENCE_CCC_HPP

#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Module/Multiplier/Multiplier.hpp"

namespace aff3ct
{
namespace module
{
class Multiplier_sequence_ccc : public Multiplier<float>
{
private:
	std::vector<std::complex<float> > cplx_sequence;
	void _imultiply(const float *X_N,  float *Z_N, const int frame_id);

public:
	Multiplier_sequence_ccc (const int N, const std::vector<float>& sequence, const int n_frames = 1);
	virtual ~Multiplier_sequence_ccc();
};
}
}

#endif //MULTIPLIER_SEQUENCE_CCC_HPP
