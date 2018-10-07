#ifndef GTIP_TEST_UTILITIES_HPP
#define GTIP_TEST_UTILITIES_HPP

#include <gtest/gtest.h>

#include <gtip/gtip.hpp>
#include <gtip_api/gtip_api.h>

//------------------------------------------

TEST(TestUtilities, HandlesPrintCoefficients)
{
	try
	{

	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesDimsToString)
{
	try
	{

	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesVecToMat)
{
	try
	{
		for (int iteration = 0; iteration < 10000; ++iteration)
		{
			const unsigned long length = 32;
			NTL::vec_GF2 vec_input;

			for (unsigned long i = 0; i < length; ++i)
			{
				vec_input.append(NTL::GF2(i));
			}

			const auto mat_output = gtip::utilities::vec_to_mat(vec_input);

			for (unsigned long i = 0; i < length; ++i)
			{
				ASSERT_EQ(mat_output[0][i], vec_input[i]);
			}
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesHashToMatrix)
{
	try
	{
		for (int iteration = 0; iteration < 10000; ++iteration)
		{
			const auto dimension = 256;

			const std::vector<unsigned char> expected{
				0xEF, 0xB6, 0xC5, 0x9D, 0xF0, 0xC6, 0x34, 0x32,
				0xCD, 0x6D, 0x96, 0xDD, 0x34, 0x5D, 0x4D, 0x8C,
				0xA6, 0x52, 0xCA, 0x2E, 0x0B, 0x06, 0xD4, 0x0B,
				0x0E, 0x44, 0xBA, 0x4F, 0x6E, 0xD3, 0x35, 0x80,
			};

			NTL::mat_GF2 expected_hash;

			expected_hash.SetDims(dimension, 1);

			for (unsigned long i = 0, count = 0; i < dimension / 8; ++i)
			{
				for (unsigned long j = 0; (j < 8) && (count < dimension); ++j)
				{
					expected_hash[count][0] = NTL::GF2(((expected[i] << j) & 0x80) >> 7);
					count += 1;
				}
			}

			const std::vector<unsigned char> message{
				0x69, 0x64, 0x73, 0x6a, 0x6e, 0x66, 0x6b, 0x66,
				0x6a, 0x61, 0x73, 0x6c, 0x64, 0x6b, 0x66, 0x61,
				0x64, 0x73, 0x6b, 0x6a, 0x73, 0x64, 0x66, 0x61 };


			NTL::mat_GF2 hash;
			hash.SetDims(dimension, 1);

			gtip::utilities::hash_to_matrix(
				message,
				hash);

			ASSERT_TRUE(hash == expected_hash);
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesCalculateWeight1)
{
	try
	{
		NTL::vec_GF2 v;
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(1));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));

		auto res = gtip::utilities::calculate_weight(v);

		ASSERT_EQ(1, res);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesCalculateWeight2)
{
	try
	{
		NTL::vec_GF2 v;
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(1));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(1));

		auto res = gtip::utilities::calculate_weight(v);

		ASSERT_EQ(2, res);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestUtilities, HandlesCalculateWeight3)
{
	try
	{
		NTL::vec_GF2 v;
		v.append(NTL::GF2(1));
		v.append(NTL::GF2(1));
		v.append(NTL::GF2(1));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));
		v.append(NTL::GF2(0));

		auto res = gtip::utilities::calculate_weight(v);

		ASSERT_EQ(3, res);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
