#include "signature.hpp"
#include "utilities.hpp"
#include "patterson.hpp"
#include "context.hpp"

#include <sodium.h>
#include <NTL/vec_GF2.h>
#include <glog/logging.h>

//------------------------------------------

using namespace gtip;

//------------------------------------------

signature::signature(
	const gtip_parameters& p) :
		m_p{p}
{}

//------------------------------------------

bool
signature::sign_participant(
		const std::vector<unsigned char>& message,
		const keygen& keygen,
		// contains the private key of the signer/participant
		const keygen::private_key_t& private_key,
		// contains the indices of the signers
		const std::set<unsigned long>& signer_indices,
		// contains all the public keys from all participants, should be N
		const std::vector<keygen::public_key_t>& public_keys,
		NTL::Vec<NTL::vec_GF2>& out,
		const bool verbose)
{
	if (public_keys.size() != m_p.number_of_participants)
	{
		LOG(ERROR) << "Number of public keys should be equal to number of participants";
		return false;
	}

	NTL::mat_GF2 hash;
	{
		hash.SetDims(m_p.n - m_p.k, 1);
		utilities::hash_to_matrix(message, hash);
	}

	bool should_stop = false;

	// holds the indices of non signers (N - t^tilde)
	std::vector<unsigned long> indices;
	{
		for (unsigned long i = 0; i < m_p.number_of_participants; ++i)
		{
			if (signer_indices.find(i) == signer_indices.end())
			{
				indices.push_back(i);
			}
		}
	}

	// Initialize E an (1 x N) matrix of vectors 1xN,
	// this should be (t^tilde x N) but since this is a call on each
	// signer there is not need to hold at each one
	NTL::Vec<NTL::vec_GF2> E;
	{
		E.SetLength(indices.size());
	}

	while (false == should_stop)
	{
		// a small memory optimization,
		// because there is no need to hold vec_GF2 for the signers,
		for (unsigned long i = 0; i < indices.size(); ++i)
		{
			E[i] = NTL::random_vec_GF2(m_p.n);
		}

		// Calculate Hxe
		NTL::mat_GF2 Hxe_sum = hash;
		{
			Hxe_sum.SetDims(m_p.n - m_p.k, 1);

			for (unsigned long i = 0; i < indices.size(); ++i)
			{
				unsigned long j = indices[i];

				Hxe_sum +=
						public_keys[j].random_matrix *
						utilities::vec_to_mat(E[i], true);
			}
		}

		// Create syndrome
		NTL::GF2EX S_X;
		{
			const auto syndrome =
					NTL::inv(private_key.random_binary_inv_matrix) * Hxe_sum;
			const auto syndrome_t = NTL::transpose(syndrome)[0];
			utilities::convert_to(syndrome_t, S_X);
		}

		// check if syndrome is decodable using patterson
		NTL::vec_GF2 error_vector;

		const auto computed =
					gtip::decoding::patterson(
						S_X,
						keygen.get_goppa_code(),
						error_vector,
						verbose);

		if (computed == false)
		{
			if (verbose == true)
			{
				LOG(WARNING) << "Patterson not found re-iterating";
			}

			continue;
		}

		const auto number_of_errors =
				gtip::decoding::number_of_errors(
						error_vector);

		// correct decoding if <= number of errors found
		should_stop = (number_of_errors <= m_p.t);

		if (should_stop == false)
		{
			if (verbose == true)
			{
				LOG(WARNING) << "Number of errors exceeding error-threshold";
			}

			continue;
		}

		out.append(
				NTL::transpose(private_key.random_permutation_matrix) *
				error_vector);

		for (unsigned long i = 0; i < indices.size(); ++i)
		{
			out.append(E[i]);
		}

		return true;
	}

	return false;
}

//------------------------------------------

bool
signature::sign_leader(
		const std::vector<unsigned char>& message,
		const keygen& keygen,
		const unsigned int leader_index,
		const keygen::private_key_t& private_key_leader,
		// contains the indices of the signers
		const std::set<unsigned long>& signer_indices,
		const std::vector<keygen::public_key_t>& public_keys,
		const std::map<unsigned long, NTL::Vec<NTL::vec_GF2>>& e_p,
		NTL::Vec<NTL::vec_GF2>& out,
		const bool verbose)
{
	if (message.empty() == true)
	{
		LOG(ERROR) << "input message should not be empty";
		return false;
	}

	if (public_keys.size() <= 0)
	{
		LOG(ERROR) << "public_keys_participants.size() should be equal > 0";
		return false;
	}

	if (signer_indices.find(leader_index) == signer_indices.end())
	{
		LOG(ERROR) << "leader_index should be in the signer_indices";
		return false;
	}

	bool syndrome_computed = false;

	// holds the indices of non signers which is in total = (N - t^tilde)
	std::vector<unsigned long> indices;
	{
		for (unsigned long i = 0; i < m_p.number_of_participants; ++i)
		{
			if (signer_indices.find(i) == signer_indices.end())
			{
				indices.push_back(i);
			}
		}
	}

	// prepare the output, fill in the e_j from the signers/participants
	{
		out.SetLength(m_p.number_of_participants);

		for (const auto e_p_i : e_p)
		{
			// e_p_i = (e_i, e_i(t^tilde), e_i(t^tilde+1), .. , , e_iN)
			// we need only the first element for signers
			out[e_p_i.first] = e_p_i.second[0];
		}
	}

	// hash message and push it in [(n - k) x 1] matrix
	NTL::mat_GF2 hash;
	{
		hash.SetDims(m_p.n - m_p.k, 1);
		utilities::hash_to_matrix(message, hash);
	}

	// find e_j for non signers, and e_l for leader
	do
	{
		NTL::Vec<NTL::vec_GF2> e_lj;
		e_lj.SetLength(indices.size());

		for (unsigned long i = 0; i < indices.size(); ++i)
		{
			const auto j = indices[i];
			bool found_vector = false;

			NTL::vec_GF2 sum_e_p_j_minus_leader, sum_e_p_j_with_leader;
			sum_e_p_j_with_leader.SetLength(m_p.n);
			sum_e_p_j_minus_leader.SetLength(m_p.n);

			for (const auto k : signer_indices)
			{
				const auto e_ij = e_p.find(k);

				if (e_ij == e_p.end())
				{
					LOG(ERROR) << "input e_p does not contain element [" << k << "]";
					return false;
				}

				// e_ij = (e_i, e_i(t^tilde), e_i(t^tilde+1), .. , , e_iN)
				// e_i is at position zero, 
				// we are iterating over e_ij so always start from element 1
				const auto internal_index = 1 + i;
				const auto& tmp_e_ijk =  e_ij->second[internal_index];

				if (k != leader_index)
				{
					sum_e_p_j_minus_leader += tmp_e_ijk;
				}

				sum_e_p_j_with_leader += tmp_e_ijk;
			}

			// find random vec_GF2 e_lj[i] such that
			// the weight of sum_e_p_j_minus_leader + e_lj[i] == 2
			do
			{
				const auto tmp_e_lj = NTL::random_vec_GF2(m_p.n);
				const auto weight = 
					utilities::calculate_weight(
						tmp_e_lj + 
						sum_e_p_j_minus_leader);

				if (weight == m_p.t)
				{
					e_lj[i] = tmp_e_lj;
					found_vector = true;
				}
			}
			while (found_vector == false);

			out[j] = sum_e_p_j_with_leader;
		}

		// Calculate Hxe
		NTL::mat_GF2 Hxe_sum;
		{
			Hxe_sum.SetDims(m_p.n - m_p.k, 1);

			for (unsigned long i = 0; i < indices.size(); ++i)
			{
				const auto j = indices[i];
				Hxe_sum +=
					public_keys[j].random_matrix *
					NTL::transpose(utilities::vec_to_mat(e_lj[i]));
			}
		}

		NTL::GF2EX S_X;
		{
			const NTL::mat_GF2 s_l = 
				(m_p.threshold % 2) == 1 ? 
				Hxe_sum + hash : 
				Hxe_sum;

			const auto syndrome =
					NTL::inv(private_key_leader.random_binary_inv_matrix) * s_l;

			const auto syndrome_t = NTL::transpose(syndrome)[0];

			utilities::convert_to(syndrome_t, S_X);
		}

		// check if syndrome is decodable using patterson
		NTL::vec_GF2 error_vector;

		const auto computed =
			gtip::decoding::patterson(
				S_X,
				keygen.get_goppa_code(),
				error_vector,
				verbose);

		if (computed == false)
		{
			if (verbose == true)
			{
				LOG(WARNING) << "Patterson not found re-iterating";
			}

			continue;
		}

		const auto number_of_errors =
				gtip::decoding::number_of_errors(
						error_vector);

		// correct decoding if <= number of errors found
		syndrome_computed = (number_of_errors <= m_p.t);

		if (syndrome_computed == false)
		{
			if (verbose == true)
			{
				LOG(WARNING) << "Number of errors exceeding error-threshold";
			}

			continue;
		}

		out[leader_index] =
				NTL::transpose(private_key_leader.random_permutation_matrix) *
				error_vector;
	}
	while (syndrome_computed == false);

	return true;
}

//------------------------------------------

bool
signature::verify(
		const std::vector<unsigned char>& message,
		const NTL::Vec<NTL::vec_GF2>& ring_signature,
		const std::vector<keygen::public_key_t>& public_keys)
{
	if (ring_signature.length() != public_keys.size())
	{
		LOG(ERROR) << "ring_signature elements should be equal to the number of public keys";
		return false;
	}

	if (ring_signature.length() != m_p.number_of_participants)
	{
		LOG(ERROR) << "ring_signature elements should be equal to number_of_participants";
		return false;
	}

	NTL::mat_GF2 hash;
	{
		hash.SetDims(m_p.n - m_p.k, 1);
		utilities::hash_to_matrix(message, hash);
	}

	bool result = true;

	for (unsigned long i = 0; i < m_p.number_of_participants; ++i)
	{
		const auto weight_i = utilities::calculate_weight(ring_signature[i]);

		if (weight_i != m_p.t)
		{
			result = false;
			break;
		}

		NTL::mat_GF2 Hxe_sum;
		Hxe_sum.SetDims(m_p.n - m_p.k, 1);

		for (unsigned long j = 0; j < m_p.number_of_participants; ++j)
		{
			Hxe_sum +=
				public_keys[j].random_matrix *
				NTL::transpose(utilities::vec_to_mat(ring_signature[j]));
		}

		if (Hxe_sum != hash)
		{
			result = false;
			break;
		}
	}

	return result;
}
//------------------------------------------
