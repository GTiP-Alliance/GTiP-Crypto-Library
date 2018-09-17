#include <map>
#include <string>
#include <iostream>

#include "gtip_api.h"

using namespace std;
using namespace gtip;

//------------------------------------------

extern "C" int
gtip_ring_keygen(
        unsigned char* public_key,
        unsigned char* secret_key,
		const gtip_parameters* parameters)
{
	return 0;
}

//------------------------------------------

extern "C" int
gtip_ring_participant_sign(
        unsigned char* signature,
        const gtip_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* secret_key)
{
	return 0;
}

//------------------------------------------

extern "C" int
gtip_ring_leader_sign(
        unsigned char* signature,
        const gtip_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* participant_signatures)
{
	return 0;
}

//------------------------------------------

extern "C" int
gtip_ring_verify(
		int* result,
        const gtip_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* ring_signature,
        const unsigned char* ring_public_key)
{
	return 0;
}

//------------------------------------------
