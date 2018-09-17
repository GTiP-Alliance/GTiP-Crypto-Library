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

NTL::vec_GF2
decoding::patterson(
		const NTL::vec_GF2& ciphertext,
		const goppa_code& gc,
		const bool verbose)
{
	return decoding::patterson(
			ciphertext,gc.H_a(),
			gc.g(),
			gc.support_set(),
			verbose);
}

//------------------------------------------

NTL::vec_GF2
decoding::patterson(
		const NTL::vec_GF2& ciphertext,
		const NTL::mat_GF2E& H_a,
		const NTL::GF2EX& g,
		const NTL::vec_GF2E& support_set,
		const bool verbose)
{
	static const auto p_zero = NTL::GF2(0);
	static const auto p_one = NTL::GF2(1);

	if (ciphertext.length() != H_a.NumCols())
	{
		LOG(ERROR) << "ciphertext length should be equal to H_a columns";
		throw std::invalid_argument("failed to patterson");
	}

	// Step 1: calculate syndrome polynomial S(X) = c x H^t
	NTL::GF2EX S_X;
	{
		// embedd ciphertext into a mat_GF2E
		NTL::mat_GF2E ciphertext_mat;
		ciphertext_mat.SetDims(1, ciphertext.length());

		unsigned long i = 0;

		for (auto& element : ciphertext_mat(1))
		{
			element.LoopHole() = ciphertext[i];
			++i;
		}

		NTL::mat_GF2E S;
		NTL::mul(
				S,
				ciphertext_mat,
				NTL::transpose(H_a));

		if (verbose == true)
		{
			LOG(INFO) << "ciphertext = " << ciphertext;
			LOG(INFO) << "S = y * H_a^T " << S;
		}

		const auto& S_row = S[0];

		S_X.SetLength(S_row.length());

		for (int i = 0; i < S_row.length(); ++i)
		{
			const auto tmp = S_row[i];
			S_X[i] = S_row[i];
		}

		if (verbose == true)
		{
			LOG(INFO) << "S_X = " << S_X;
		}
	}

	// Step 2: T = S^-1 (mod g)
	NTL::GF2EX T, X;
	{
		utilities::extended_euclidean_algorithm(
				S_X,
				g,
				T);

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
	NTL::GF2EX t;
	{
		t = NTL::sqr(T + X);

		if (verbose == true)
		{
			LOG(INFO) << "t = NTL::sqr(T + X) = " << t;
		}
	}

	// Step 4:
	NTL::vec_GF2 error_vector;
	{
		NTL::GF2EX Ax, Bx;
		NTL::vec_GF2E roots;
		utilities::extended_euclidean_algorithm(
				t,
				g,
				Bx,
				Ax);

		const NTL::GF2EX sigma_x = (Ax * Ax) + (Bx * Bx) * X;

		if (verbose == true)
		{
			LOG(INFO) << "Ax = " << Ax;
			LOG(INFO) << "Bx = " << Bx;
			LOG(INFO) << "sigma_x = (Ax * Ax) + (Bx * Bx) * X = " << sigma_x;
		}

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
			LOG(INFO) << "roots = " << roots;
			LOG(INFO) << "error_vector = " << error_vector;
		}
	}

	return error_vector;
}

//------------------------------------------

NTL::vec_GF2
decoding::correct_ciphertext(
		const NTL::vec_GF2& ciphertext,
		const NTL::vec_GF2& error_vector)
{
	NTL::vec_GF2 tmp = ciphertext;

	decoding::correct_ciphertext(
			tmp, error_vector);

	return tmp;
}

//------------------------------------------

void
decoding::correct_ciphertext(
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
