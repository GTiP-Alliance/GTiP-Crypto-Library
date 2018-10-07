#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <glog/logging.h>

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>
#include <NTL/GF2XFactoring.h>
#include <NTL/GF2EXFactoring.h>

#include "goppa_code.hpp"

//------------------------------------------

using namespace NTL;
using namespace gtip;

//------------------------------------------

goppa_code::goppa_code(
		const gtip_parameters& parameters,
		const bool compute_H_b,
		const bool verbose) :
				goppa_code(
						// TODO check this
						(unsigned long)std::round(log2(double(parameters.n))),
						parameters.t,
						parameters.n,
						compute_H_b,
						verbose)
{}

//------------------------------------------

goppa_code::goppa_code(
		const unsigned long m,
		const unsigned long t) :
				goppa_code(m, t, std::pow(2, m))
{}

//------------------------------------------

goppa_code::goppa_code(
		const unsigned long m,
		const unsigned long t,
		const unsigned long n,
		const bool compute_H_b,
		const bool verbose) :
				m_m{m},
				m_t{t},
				m_n{n},
				m_compute_H_b{compute_H_b},
				m_verbose{verbose}
{
	// Check arguments
	if (m < 3)
	{
		LOG(ERROR) << "m [" << m << "] should be >= 3";
		throw std::invalid_argument("Failed to initialize");
	}

	//
	if (t < 2 ||
		t > (std::pow(2, m) - 1) / m)
	{
		LOG(ERROR) << "t [" << t << "] should be >= 2 and <= (2^m - 1) / m";
		throw std::invalid_argument("Failed to initialize");
	}

	if (n < (m * t + 1) ||
		n > std::pow(2, m))
	{
		LOG(ERROR) << "n [" << n << "] should be >= m*t + 1 and <= 2^m";
		throw std::invalid_argument("Failed to initialize");
	}

	// build goppa codes H_a and H_b
	// choose randomly irreducible polynomial k
	// generate support set L
	{
		bool found = false;
		unsigned int tries = 0;

		while (found == false &&
				++tries < MAX_TRIES)
		{
			// generate support set L
			NTL::vec_GF2E tmp_support_set;

			if (goppa_code::compute_support_set(this->m_n, tmp_support_set) == false)
			{
				continue;
			}

			found = true;
			this->m_support_set = tmp_support_set;

			if (m_verbose == true)
			{
				LOG(INFO) << "k = " << context::get_k();

				for (int i = 0; i < this->m_support_set.length(); ++i)
				{
					LOG(INFO) << "a[" << i << "] = " << this->m_support_set[i];
				}
			}
		}

		if (found == false)
		{
			LOG(ERROR) << "Failed to find irreducible polynomial k";
			throw std::runtime_error("Failed to initialize");
		}
	}

	// create random irreducible polynomial g of degree t
	{
		bool found = false;
		unsigned int tries = 0;

		while (found == false &&
				++tries < MAX_TRIES)
		{
			found = true;
			NTL::GF2EX tmp_g;
			NTL::BuildIrred(tmp_g, this->m_t);

			for (const auto& s : this->m_support_set)
			{
				if (NTL::IsZero(NTL::eval(tmp_g, s)) == true)
				{
					if (this->m_verbose == true)
					{
						LOG(INFO) << "Changing seed";
					}

					found = false;
					break;
				}
			}

			this->m_g = tmp_g;
		}

		if (this->m_verbose == true)
		{
			LOG(INFO) << "g = " << this->m_g;
		}

		if (found == false)
		{
			LOG(ERROR) << "Failed to find irreducible polynomial g";
			throw std::runtime_error("Failed to initialize");
		}
	}

	// using g and L create parity check matrix H_a
	{
		NTL::vec_GF2E inv_eval_g;

		for (auto&& eval_g : NTL::eval(this->m_g, this->m_support_set))
		{
			inv_eval_g.append(NTL::inv(eval_g));
		}

		if (this->m_verbose == true)
		{
			for (unsigned long i = 0; i < inv_eval_g.length(); ++i)
			{
				LOG(INFO) << "1/g(" << i << ") = " << inv_eval_g[i];
			}
		}

		NTL::mat_GF2E X;
		X.SetDims(this->m_t, this->m_n);

		for (unsigned long j = 0; j < this->m_n; ++j)
		{
			X[0][j] =  inv_eval_g[j];
			NTL::GF2E a_t = this->m_support_set[j];

			for (unsigned long i = 1; i < this->m_t; ++i)
			{
				X[i][j] = a_t * inv_eval_g[j];
				a_t *= this->m_support_set[j];
			}
		}

		if (this->m_verbose == true)
		{
			LOG(INFO) << "X = \n" << X;
		}

		NTL::mat_GF2E T;
		T.SetDims(t, t);

		for (unsigned long i = 0; i < this->m_t; ++i)
		{
			for (unsigned long j = 0; j <= i; ++j)
			{
				T[i][j] = this->m_g[this->m_t - i + j];
			}
		}

		if (this->m_verbose == true)
		{
			LOG(INFO) << "T = \n" << T;
		}

		this->m_H_a = T * X;

		if (this->m_verbose == true)
		{
			LOG(INFO) << "H_a = \n" << this->m_H_a;
		}
	}

	// unfold H_a to create binary matrix H_b
	if (this->m_compute_H_b == true)
	{
		this->m_H_b.SetDims(this->m_t * this->m_m, this->m_n);

		const auto degree = this->m_m;

		for (unsigned long i = 0; i < this->m_t; ++i)
		{
			for (unsigned long j = 0; j < this->m_n; ++j)
			{
				auto& p = this->m_H_a[i][j];
				const auto& l = p.LoopHole();

				if (this->m_verbose == true)
				{
					LOG(INFO) << "[" << i << "," << j << "] = " << p;
				}

				for (unsigned long z = 0; z < degree; ++z)
				{
					this->m_H_b[i * degree + z][j] = l[z];
				}
			}
		}

		if (this->m_verbose == true)
		{
			LOG(INFO) << "H_b = \n" << this->m_H_b;
		}
	}
}

//------------------------------------------

bool
goppa_code::compute_support_set(
		const unsigned long n,
		NTL::vec_GF2E& results) const
{
	static const auto p_zero = NTL::GF2E(0);
	static const auto p_one = NTL::GF2E(1);

	for (unsigned long j = 0; j < (n / 2 + 1); ++j)
	{
		bool correct = true;
		NTL::GF2E seed = NTL::random_GF2E();

		if (seed == p_one)
		{
			continue;
		}

		NTL::vec_GF2E tmp_results;
		tmp_results.append(p_zero);
		tmp_results.append(p_one);
		tmp_results.append(seed);

		NTL::GF2E tmp = seed;

		for (unsigned long i = 3; i < n; ++i)
		{
			tmp *= seed;

			if (tmp == p_one || tmp == seed)
			{
				if (this->m_verbose == true)
				{
					LOG(INFO) << "Changing seed [" << i << "]";
				}
				correct = false;
				break;
			}

			tmp_results.append(tmp);
		}

		if (correct == true)
		{
			results.append(tmp_results);

			return true;
		}
	}

	if (this->m_verbose == true)
	{
		LOG(INFO) << "Failed to compute support set";
	}

	return false;
}

//------------------------------------------

const NTL::mat_GF2E&
goppa_code::H_a() const
{
	return this->m_H_a;
}

//------------------------------------------

const NTL::mat_GF2&
goppa_code::H_b() const
{
	return this->m_H_b;
}

//------------------------------------------

const unsigned long
goppa_code::t() const
{
	return this->m_t;
}

//------------------------------------------

const NTL::GF2EX&
goppa_code::g() const
{
	return this->m_g;
}

//------------------------------------------

const NTL::vec_GF2E&
goppa_code::support_set() const
{
	return this->m_support_set;
}

