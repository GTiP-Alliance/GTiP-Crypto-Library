#ifndef GTIP_KEYGEN_HPP
#define GTIP_KEYGEN_HPP

#include <NTL/mat_GF2.h>

#include <vector>

#include <gtip_api/gtip_api.h>
#include "goppa_code.hpp"

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
			const gtip::gtip_parameters m_p;
			//------------------------------------------
			const gtip::goppa_code m_gc;
			//------------------------------------------
		public:
			//------------------------------------------
			typedef struct
			{
				// Q_i
				NTL::mat_GF2 random_binary_inv_matrix;
				// H_i == H_b from goppa code
				NTL::mat_GF2 binary_parity_check_matrix;
				// P_i
				NTL::mat_GF2 random_permutation_matrix;
			} private_key_t;
			//------------------------------------------
			typedef struct
			{
				// H_i^tilde
				NTL::mat_GF2 random_matrix;
			} public_key_t;
			//------------------------------------------
			keygen(
					const gtip::gtip_parameters& p);
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
			const goppa_code&
			get_goppa_code() const;
			//------------------------------------------
	};	
}

#endif
