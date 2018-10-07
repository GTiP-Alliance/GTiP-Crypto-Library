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

		static const NTL::GF2 p_zero = NTL::GF2(0);
		static const NTL::GF2 p_one = NTL::GF2(1);

		//------------------------------------------

		//!
		//! \brief The Patterson algorithm is one of the most common algorithms used for decoding ciphertexts from a binary Goppa code
		//!        It can correct up to t errors using a Goppa code over an irreducible polynomial of degree g(x)/t
		//!        It returns back an error vector with every flipped bit (set to one) indicating an error
		bool
		patterson(
				const NTL::vec_GF2& ciphertext,
				const NTL::mat_GF2E& H_a,
				const NTL::GF2EX& g,
				const NTL::vec_GF2E& support_set,
				NTL::vec_GF2& error_vector,
				const bool verbose = false);

		//!
		//! \brief The Patterson algorithm is one of the most common algorithms used for decoding ciphertexts from a binary Goppa code
		//!        It can correct up to t errors using a Goppa code over an irreducible polynomial of degree g(x)/t
		//!        It returns back an error vector with every flipped bit (set to one) indicating an error
		bool
		patterson(
				const NTL::GF2EX& S_X,
				const NTL::GF2EX& g,
				const NTL::vec_GF2E& support_set,
				NTL::vec_GF2& error_vector,
				const bool verbose = false);

		//------------------------------------------

		bool
		patterson(
				const NTL::vec_GF2& ciphertext,
				const goppa_code& gc,
				NTL::vec_GF2& error_vector,
				const bool verbose = false);

		//------------------------------------------

		bool
		patterson(
				const NTL::GF2EX& S_X,
				const goppa_code& gc,
				NTL::vec_GF2& error_vector,
				const bool verbose = false);

		//------------------------------------------

		bool
		is_syndrome_valid(
				const NTL::GF2EX& S_X);

		//------------------------------------------

		unsigned long
		number_of_errors(
				const NTL::vec_GF2& error_vector);

		//------------------------------------------

		NTL::vec_GF2
		correct_ciphertext(
				const NTL::vec_GF2& ciphertext,
				const NTL::vec_GF2& error_vector);

		//------------------------------------------

		//!
		//! \brief correct ciphertext in place
		void
		correct_ciphertext_inplace(
				NTL::vec_GF2& ciphertext,
				const NTL::vec_GF2& error_vector);

		//------------------------------------------
	};
}

#endif
