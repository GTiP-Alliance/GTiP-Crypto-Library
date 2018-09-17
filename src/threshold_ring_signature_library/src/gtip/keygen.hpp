#ifndef GTIP_KEYGEN_HPP
#define GTIP_KEYGEN_HPP

#include <NTL/mat_GF2.h>

#include <vector>

#include "gtip_api/gtip_api.h"

namespace gtip
{
	class keygen
	{
		protected:
			//------------------------------------------
			// System parameters structure which contains
			// all relevent parameters for the construction
			// of the underlying goppa code. See GTIP_ring.h
			// for specifics.
			//------------------------------------------
			gtip::gtip_parameters m_p;
			//------------------------------------------
		public:
			//------------------------------------------
			typedef struct
			{
				NTL::mat_GF2 random_binary_inv_matrix;
				NTL::mat_GF2 binary_parity_check_matrix;
				NTL::mat_GF2 random_permutation_matrix;
			} private_key_t;
			//------------------------------------------
			typedef struct
			{
				NTL::mat_GF2 random_matrix;
			} public_key_t;
			//------------------------------------------
			keygen(
					gtip::gtip_parameters& p);
			//------------------------------------------
			bool
			compute_private_key(
					private_key_t& private_key);
			//------------------------------------------
			bool
			compute_public_key(
					public_key_t& public_key,
					const private_key_t& private_key);
			//------------------------------------------
			// Computes ring key by concatenating all 
			// parity check matrices together
			//------------------------------------------
			bool
			compute_ring_key(
					std::vector<NTL::mat_GF2>& ring_key,
					const NTL::mat_GF2& public_key);
			//------------------------------------------
	};	
}

#endif
