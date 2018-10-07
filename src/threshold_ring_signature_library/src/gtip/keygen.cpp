#include <random>
#include <algorithm>
#include <glog/logging.h>

#include "keygen.hpp"
#include "context.hpp"

//------------------------------------------

using namespace gtip;

//------------------------------------------

keygen::keygen(
	const gtip_parameters& p) :
		m_p{p},
		m_gc{p}
{}

//------------------------------------------

bool
keygen::compute_ring_key(
		std::vector<NTL::mat_GF2>& ring_key,
		const NTL::mat_GF2& public_key)
{
	ring_key.push_back(public_key);
	return true;
}

//------------------------------------------

bool
keygen::compute_public_key(
		public_key_t& public_key,
		const private_key_t& private_key)
{
	try
	{
		NTL::mul(
				public_key.random_matrix,
				private_key.random_binary_inv_matrix,
				private_key.binary_parity_check_matrix);
	
		NTL::mul(
				public_key.random_matrix,
				public_key.random_matrix,
				private_key.random_permutation_matrix);

		return true;
	}
	catch(const std::exception& exp)
	{
		LOG(ERROR) << "Failed to compute_public_key : " << exp.what();
		return false;
	}
}

//------------------------------------------

bool
keygen::compute_private_key(
		private_key_t& private_key)
{
	try
	{
		private_key.binary_parity_check_matrix = m_gc.H_b();
		private_key.random_binary_inv_matrix =
				NTL::random_mat_GF2(m_p.n - m_p.k, m_p.n - m_p.k);
		{
			while(true == NTL::IsZero(NTL::determinant(private_key.random_binary_inv_matrix)))
			{
				private_key.random_binary_inv_matrix =
						NTL::random_mat_GF2(m_p.n - m_p.k, m_p.n - m_p.k);
			}
		}

		std::vector<unsigned long> permutation(m_p.n, 0);
		std::iota(
				permutation.begin(),
				permutation.end(),
				0);

		std::shuffle(
				permutation.begin(),
				permutation.end(),
				context::get_random_engine());

		private_key.random_permutation_matrix = NTL::ident_mat_GF2(m_p.n);

		for (unsigned long j = 0; j < m_p.n; ++j)
		{
			private_key.random_permutation_matrix[j] =
					NTL::shift(
							private_key.random_permutation_matrix[j],
							permutation[j] - j);
		}

		return true;
	}
	catch(const std::exception& exp)
	{
		LOG(ERROR) << "Failed to compute_private_key : " << exp.what();
		return false;
	}
}

//------------------------------------------

const goppa_code&
keygen::get_goppa_code() const
{
	return m_gc;
}

//------------------------------------------
