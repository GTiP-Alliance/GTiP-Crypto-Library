#include "utilities.hpp"
#include <sodium.h>

//------------------------------------------

using namespace gtip;

//------------------------------------------

bool
utilities::extended_euclidean_algorithm(
		const NTL::GF2EX& t,
		const NTL::GF2EX& g,
		NTL::GF2EX& T,
		NTL::GF2EX& d)
{
	NTL::GF2EX a;
	// d = gcd(t, g), t * T + g * a = d
	NTL::XGCD(d, T, a, t, g);
	return true;
}

//------------------------------------------

bool
utilities::extended_euclidean_algorithm(
		const NTL::GF2EX& S,
		const NTL::GF2EX& G,
		NTL::GF2EX& T)
{
	NTL::GF2EX d;
	NTL::GF2EX a;
	// d = gcd(S, G), S * T + G * a = d
	NTL::XGCD(d, T, a, S, G);
	return true;
}

//------------------------------------------

unsigned long
utilities::hash_to_matrix(
		const std::vector<unsigned char>& message,
		NTL::mat_GF2& hash)
{
	return hash_to_matrix(
			message.data(),
			(unsigned long)message.size(),
			hash);
}

//------------------------------------------

unsigned long
utilities::hash_to_matrix(
		const unsigned char* message,
		const unsigned long message_length,
		NTL::mat_GF2& hash)
{
	// Hash the message
	const unsigned int dimension = hash.NumRows();
	unsigned char hash_buf[crypto_hash_sha256_BYTES] = {0};
	crypto_hash_sha256(hash_buf, message, message_length);

	const auto bytes_needed = std::ceil((dimension) / 8);

	unsigned long count = 0;

	for (unsigned long i = 0; i < bytes_needed; ++i)
	{
		for (unsigned long j = 0; (j < 8) && (count < dimension); ++j)
		{
			// TODO make this macro, reverse endian
			hash[count][0] = NTL::GF2(((hash_buf[i] << j) & 0x80) >> 7);
			count += 1;
		}		
	}

	return count;
}

//------------------------------------------

unsigned long
utilities::calculate_weight(
	const NTL::vec_GF2& v)
{
	unsigned long non_zero_counter = 0;

	for (unsigned long i = 0; i < v.length(); ++i)
	{
		if (false == NTL::IsZero(v[i]))
		{
			non_zero_counter += 1;
		}
	}

	return non_zero_counter;
}

//------------------------------------------

bool
utilities::convert_to(
		const NTL::vec_GF2& v,
		NTL::GF2EX& result)
{
	const unsigned long length = v.length();

	result.SetLength(length);

	for (unsigned long i = 0; i < length; ++i)
	{
		result[i] = v[i];
	}

	return true;
}

//------------------------------------------
