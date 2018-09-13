#ifndef GTIP_CONTEXT_HPP
#define GTIP_CONTEXT_HPP

// stl includes
#include <random>
#include <algorithm>

// NTL includes
#include <NTL/GF2E.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

namespace gtip
{
	class context
	{
		public:
			//------------------------------------------
			static void
			init(const unsigned long m);
			//------------------------------------------
			static void
			init(const NTL::GF2X& k, const unsigned long m);
			//------------------------------------------
			static bool
			has_support_set(
					const unsigned long m);
			//------------------------------------------
			static bool
			is_initialized();
			//------------------------------------------
			static const NTL::GF2X& get_k();
			//------------------------------------------
			static std::mt19937& get_random_engine();
			//------------------------------------------
		private:
			//------------------------------------------
			static NTL::GF2X k;
			//------------------------------------------
			static bool initialized;
			//------------------------------------------
			static std::mt19937 random_engine;
			//------------------------------------------
			static std::random_device random_device;
			//------------------------------------------
		private:
			//------------------------------------------
			context(){};
			//------------------------------------------
			virtual ~context(){};
			//------------------------------------------
	};
}

#endif

