#ifndef GTIP_PATTERSON_HPP
#define GTIP_PATTERSON_HPP

#include <NTL/GF2E.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

#include "goppa_code.hpp"

namespace gtip
{
	namespace decoding
	{
		//------------------------------------------

		//!
		//! \brief The Patterson algorithm is one of the most common algorithms used for decoding ciphertexts from a binary Goppa code
		//!        It can correct up to t errors using a Goppa code over an irreducible polynomial of degree g(x)/t
		//!        It returns back an error vector with every flipped bit (set to one) indicating an error
		NTL::vec_GF2
		patterson(
				const NTL::vec_GF2& ciphertext,
				const NTL::mat_GF2E& H_a,
				const NTL::GF2EX& g,
				const NTL::vec_GF2E& support_set,
				const bool verbose = false);

		//------------------------------------------

		NTL::vec_GF2
		patterson(
				const NTL::vec_GF2& ciphertext,
				const goppa_code& gc,
				const bool verbose = false);

		//------------------------------------------

		NTL::vec_GF2
		correct_ciphertext(
				const NTL::vec_GF2& ciphertext,
				const NTL::vec_GF2& error_vector);

		//------------------------------------------

		//!
		//! \brief correct ciphertext in place
		void
		correct_ciphertext(
				NTL::vec_GF2& ciphertext,
				const NTL::vec_GF2& error_vector);

		//------------------------------------------
	};
}

#endif
