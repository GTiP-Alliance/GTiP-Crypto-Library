#ifndef GTIP_TEST_SIGN_PARTICIPANT_HPP
#define GTIP_TEST_SIGN_PARTICIPANT_HPP

#include <gtest/gtest.h>
#include <glog/logging.h>

#include <gtip/gtip.hpp>
#include <gtip_api/gtip_api.h>

#include <sodium.h>

//------------------------------------------

TEST(TestSignParticipant, TestSignAndVerify)
{
	try
	{
		// m * t = n - k
		const unsigned long k = 8;
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);
		const unsigned long threshold = 5;
		const unsigned long number_of_participants = 10;

		//Define Parameters
		gtip::gtip_parameters parameters;
		{
			parameters.k = k;
			parameters.t = t;
			parameters.n = n;
			parameters.threshold = threshold;
			parameters.number_of_participants = number_of_participants;
		}

		gtip::context::init(parameters.k);

		// Generate Keys

		std::vector<gtip::keygen> keygen;
		std::vector<gtip::keygen::public_key_t> public_keys(parameters.number_of_participants);
		std::vector<gtip::keygen::private_key_t> private_keys(parameters.number_of_participants);

		for (unsigned long i = 0; i < parameters.number_of_participants; ++i)
		{
			gtip::keygen tmp_keygen(parameters);
			keygen.push_back(tmp_keygen);
			keygen.back().compute_private_key(private_keys[i]);
			keygen.back().compute_public_key(public_keys[i], private_keys[i]);
		}

		std::vector<unsigned char> message = {0x00, 0x00};
		gtip::signature signature(parameters);

		// index of the leader
		unsigned long leader = 2;

		// indices of signers, should contain leader index
		std::set<unsigned long> signer_indices{0,1,2,3,4};
		std::map<unsigned long, NTL::Vec<NTL::vec_GF2>> participant_results;

		// compute for all participants
		for (const auto i : signer_indices)
		{
			NTL::Vec<NTL::vec_GF2> result;

			const auto computed =
				signature.sign_participant(
					message,
					keygen[i],
					private_keys[i],
					signer_indices,
					public_keys,
					result);

			ASSERT_TRUE(computed) << "sign_participant should return true";
			ASSERT_EQ(result.length(), parameters.number_of_participants - parameters.threshold + 1);

			for (unsigned long j = 0; j < result.length(); ++j)
			{
				ASSERT_EQ(result[j].length(), n);
			}

			participant_results[i] = result;


			ASSERT_EQ(
					(unsigned long)parameters.t,
					gtip::utilities::calculate_weight(result[0])) << "e_i[0] should have a weight of " << parameters.t;
		}

		NTL::Vec<NTL::vec_GF2> ring_signature;

		const auto computed =
			signature.sign_leader(
					message,
					keygen[leader],
					leader,
					private_keys[leader],
					// contains the indices of the signers
					signer_indices,
					// all N public keys
					public_keys,
					// key is index of signer and value is the output of sign_participant
					participant_results,
					ring_signature);

		ASSERT_TRUE(computed) << "sign_leader should return true";

		LOG(INFO) << "ring_signature ";

		for (int i = 0; i < ring_signature.length(); ++i)
		{
			LOG(INFO) << "e[" << i << "] " << ring_signature[i];
		}

		const auto verified = signature.verify(message, ring_signature, public_keys);

		ASSERT_TRUE(verified) << "ring signature should be verified";
	}
	catch(const std::exception& exp)
	{
		FAIL() << exp.what();
	}
}

//------------------------------------------

#endif
