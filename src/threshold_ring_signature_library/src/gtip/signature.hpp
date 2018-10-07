#ifndef GTIP_SIGNATURE_HPP
#define GTIP_SIGNATURE_HPP

#include <set>
#include <map>
#include <vector>
#include <NTL/vec_GF2.h>

#include <gtip_api/gtip_api.h>
#include "keygen.hpp"
#include "goppa_code.hpp"

#include <vector>

namespace gtip
{
	class signature
	{
		protected:
			//------------------------------------------
			const gtip::gtip_parameters m_p;
			//------------------------------------------
		public:
			//------------------------------------------
			signature(
					const gtip::gtip_parameters& p);
			//------------------------------------------
			/*
			 * private_key : the private key of the signer/participant
			 * public_keys : all the public keys (N), assumes there is no shuffling
			 */
			bool
			sign_participant(
					const std::vector<unsigned char>& message,
					const keygen& keygen,
					const keygen::private_key_t& private_key,
					// contains the indices of the signers
					const std::set<unsigned long>& signer_indices,
					// all N public keys
					const std::vector<keygen::public_key_t>& public_keys,
					// out holds (e_i, e_i(t_tilde + 1), e_i(t_tilde + 2), ..., e_iN)
					NTL::Vec<NTL::vec_GF2>& out,
					const bool verbose = false);
			//------------------------------------------
			/*
			 * private_key : the private key of the signer/leader
			 * public_keys : all the public keys (N), assumes there is no shuffling
			 * e_p : key is index of signer and value is the output of sign_participant
			 */
			bool
			sign_leader(
					const std::vector<unsigned char>& message,
					const keygen& keygen,
					const unsigned int leader_index,
					const keygen::private_key_t& private_key_leader,
					// contains the indices of the signers
					const std::set<unsigned long>& signer_indices,
					// all N public keys
					const std::vector<keygen::public_key_t>& public_keys,
					// key is index of signer and value is the output of sign_participant
					const std::map<unsigned long, NTL::Vec<NTL::vec_GF2>>& e_p,
					NTL::Vec<NTL::vec_GF2>& out,
					const bool verbose = false);
			//------------------------------------------
			bool
			verify(
					const std::vector<unsigned char>& message,
					const NTL::Vec<NTL::vec_GF2>& ring_signature,
					const std::vector<keygen::public_key_t>& public_keys);
			//------------------------------------------
	};
}

#endif
