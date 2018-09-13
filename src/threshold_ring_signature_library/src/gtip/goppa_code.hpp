#ifndef GTIP_GOPPA_CODE_HPP
#define GTIP_GOPPA_CODE_HPP

#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

#include "context.hpp"

namespace gtip
{
	class goppa_code
	{
		public:
			//------------------------------------------
			static constexpr auto MAX_TRIES = 128;
			//------------------------------------------
		protected:
			//------------------------------------------
			// parity check matrix H_a of dimensions t x n,
			// with elements being polynomial functions of a
			NTL::mat_GF2E m_H_a;
			//------------------------------------------
			// parity check matrix H_b of dimensions (m * t) x n,
			// which is a block matrix with elements being 0's and 1's
			NTL::mat_GF2 m_H_b;
			//------------------------------------------
			NTL::GF2EX m_g;
			// input integer m >= 3, typically between 10,11,12
			// see bernstein 2011 - List decoding for binary goppa codes
			const unsigned long  m_m = 3;
			//------------------------------------------
			// 2 <= t <= (2^m - 1) / m
			// goppa code will be degree-t code designed to correct t errors
			// choose intermediate value of t e.g for m = 11, choose t = 32
			const unsigned long  m_t = 2;
			//------------------------------------------
			const unsigned long  m_n = 0;
			//------------------------------------------
			bool m_verbose = true;
			//------------------------------------------
			bool m_compute_H_b = true;
			//------------------------------------------
			NTL::vec_GF2E m_support_set;
			//------------------------------------------
		public:
			//------------------------------------------
			goppa_code(
					const unsigned long m,
					const unsigned long t,
					const unsigned long n,
					const bool compute_H_b = true,
					const bool verbose = false);
			//------------------------------------------
			goppa_code(
					const unsigned long m,
					const unsigned long t);
			//------------------------------------------
			bool
			compute_support_set(
					const unsigned long m,
					NTL::vec_GF2E& results) const;
			//------------------------------------------
			const NTL::mat_GF2E& H_a() const;
			//------------------------------------------
			const NTL::mat_GF2& H_b() const;
			//------------------------------------------
			const NTL::GF2EX& g() const;
			//------------------------------------------
			const NTL::vec_GF2E& support_set() const;
			//------------------------------------------
	};
}

#endif
