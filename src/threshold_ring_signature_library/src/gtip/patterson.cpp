#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <glog/logging.h>

#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>
#include <NTL/GF2EXFactoring.h>

#include "patterson.hpp"
#include "utilities.hpp"

//------------------------------------------

using namespace gtip;
using namespace decoding;

//------------------------------------------

bool
decoding::patterson(
		const NTL::vec_GF2& ciphertext,
		const goppa_code& gc,
		NTL::vec_GF2& error_vector,
		const bool verbose)
{
	return decoding::patterson(
			ciphertext,gc.H_a(),
			gc.g(),
			gc.support_set(),
			error_vector,
			verbose);
}

//------------------------------------------

bool
decoding::patterson(
		const NTL::GF2EX& S_X,
		const goppa_code& gc,
		NTL::vec_GF2& error_vector,
		const bool verbose)
{
	return decoding::patterson(
			S_X,
			gc.g(),
			gc.support_set(),
			error_vector,
			verbose);
}

//------------------------------------------

bool
decoding::patterson(
		const NTL::vec_GF2& ciphertext,
		const NTL::mat_GF2E& H_a,
		const NTL::GF2EX& g,
		const NTL::vec_GF2E& support_set,
		NTL::vec_GF2& error_vector,
		const bool verbose)
{
	if (ciphertext.length() != H_a.NumCols())
	{
		if (verbose == true)
		{
			LOG(ERROR) << "ciphertext length should be equal to H_a columns";
		}

		return false;
	}

	// Step 1: calculate syndrome polynomial S(X) = c x H^t
	NTL::GF2EX S_X;
	{
		NTL::mat_GF2E S;
		{
			// embedd ciphertext into a mat_GF2E
			NTL::mat_GF2E ciphertext_mat;
			{
				unsigned long i = 0;
				ciphertext_mat.SetDims(1, ciphertext.length());

				for (auto& element : ciphertext_mat(1))
				{
					element.LoopHole() = ciphertext[i];
					++i;
				}
			}
			NTL::mul(
					S,
					ciphertext_mat,
					NTL::transpose(H_a));
		};

		const auto& S_row = S[0];
		const unsigned long S_row_length = S_row.length();

		S_X.SetLength(S_row_length);

		for (unsigned long i = 0; i < S_row_length; ++i)
		{
			S_X[i] = S_row[i];
		}

		if (verbose == true)
		{
			LOG(INFO) << "ciphertext = " << ciphertext;
			LOG(INFO) << "S = y * H_a^T " << S;
			LOG(INFO) << "S_X = " << S_X;
		}
	}

	// Step 2: T = S^-1 (mod g)
	// Step 3: sqrt(T + X)
	// Step 4:
	return patterson(
			S_X,
			g,
			support_set,
			error_vector,
			verbose);
}

//------------------------------------------

bool
decoding::patterson(
		const NTL::GF2EX& S_X,
		const NTL::GF2EX& g,
		const NTL::vec_GF2E& support_set,
		NTL::vec_GF2& error_vector,
		const bool verbose)
{
	if (is_syndrome_valid(S_X) == false)
	{
		if (verbose == true)
		{
			LOG(ERROR) << "invalid syndrome S_X = " << S_X;
		}

		return false;
	}

	// Step 2: T = S^-1 (mod g)
	NTL::GF2EX T, X;
	{
		utilities::extended_euclidean_algorithm(
				S_X,
				g,
				T);

		if (T.rep.length() == 0)
		{
			return false;
		}

		X.SetLength(T.rep.length());
		X[1] = NTL::GF2E(1);

		if (verbose == true)
		{
			LOG(INFO) << "g = " << g;
			LOG(INFO) << "T = " << T;
			LOG(INFO) << "X = " << X;
		}
	}

	// Step 3: sqrt(T + X)
	const NTL::GF2EX t = NTL::sqr(T + X);
	{
		if (verbose == true)
		{
			LOG(INFO) << "t = NTL::sqr(T + X) = " << t;
		}
	}

	// Step 4:
	// compute error vector
	{
		NTL::GF2EX Ax, Bx;
		NTL::vec_GF2E roots;
		utilities::extended_euclidean_algorithm(
				t,
				g,
				Bx,
				Ax);

		const NTL::GF2EX sigma_x = (Ax * Ax) + (Bx * Bx) * X;

		for (const auto& s : support_set)
		{
			if (NTL::IsZero(NTL::eval(sigma_x, s)) == true)
			{
				error_vector.append(p_one);

				if (verbose == true)
				{
					roots.append(s);
				}
			}
			else
			{
				error_vector.append(p_zero);
			}
		}

		if (verbose == true)
		{
			LOG(INFO) << "Ax = " << Ax;
			LOG(INFO) << "Bx = " << Bx;
			LOG(INFO) << "sigma_x = " << sigma_x;
			LOG(INFO) << "roots = " << roots;
			LOG(INFO) << "error_vector = " << error_vector;
		}
	}

	return true;
}

//------------------------------------------

bool
decoding::is_syndrome_valid(
		const NTL::GF2EX& S_X)
{
	if (NTL::IsZero(NTL::LeadCoeff(S_X)) == true)
	{
		return false;
	}

	return true;
}

//------------------------------------------

unsigned long
decoding::number_of_errors(
		const NTL::vec_GF2& error_vector)
{
	unsigned long number_of_errors = 0;

	for (const auto& e: error_vector)
	{
		if (e == p_one)
		{
			++number_of_errors;
		}
	}

	return number_of_errors;
};

//------------------------------------------

NTL::vec_GF2
decoding::correct_ciphertext(
		const NTL::vec_GF2& ciphertext,
		const NTL::vec_GF2& error_vector)
{
	NTL::vec_GF2 tmp = ciphertext;

	decoding::correct_ciphertext_inplace(
			tmp, error_vector);

	return tmp;
}

//------------------------------------------

void
decoding::correct_ciphertext_inplace(
		NTL::vec_GF2& ciphertext,
		const NTL::vec_GF2& error_vector)
{
	if (ciphertext.length() != error_vector.length())
	{
		LOG(ERROR) << "ciphertext and error_vector should have the same length";
		throw std::invalid_argument("failed to correct_ciphertext");
	}

	ciphertext -= error_vector;
}

//------------------------------------------
