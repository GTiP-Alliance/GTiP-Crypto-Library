#ifndef QREDO_RING_API_H
#define QREDO_RING_API_H

#ifdef __cplusplus
namespace qredo_ring_api
{
#endif

	//------------------------------------------
	// All unsigned char* variables are considered preallocated with known length
	// All functions are considered stateless
	// All functions return UNIX style, 0 is succesful computation
	//------------------------------------------

	// placeholder lengths, these are calculated from the parameters and will change

	#define PUBLIC_KEY_LENGTH            256;
	#define SECRET_KEY_LENGTH            256;
	#define LEADER_SIGNATURE_LENGTH      256;
	#define PARTICIPANT_SIGNATURE_LENGTH 256;
	#define RING_SIGNATURE_LENGTH        256;
	#define RING_PUBLIC_KEY_LENGTH       256;

	//------------------------------------------

	//!
	//! \brief The system parameters
	//!
	typedef struct
	{
		// code length
		unsigned int n;
		// code dimension
		unsigned int k;
		// minimum distance
		unsigned int d;
		// error correcting ability of underpinning code
		unsigned int w;
		// security parameter
		unsigned int l;
		// number of participants
		unsigned int number_of_participants;
		// threshold number of participants
		unsigned int threshold;
	} qredo_parameters;

	//------------------------------------------

	//!
	//! \brief Creates the public key and private key, returns 0 if computed succesfully
	//!
	//! \param public_key  output : the public key created, should be preallocated, length is predefined
	//! \param secret_key  output : the private key created, should be preallocated, length is predefined
	//! \param parameters  input  : the system parameters
	//!
	extern "C" int
	qredo_ring_keygen(
			unsigned char* public_key,
			unsigned char* secret_key,
			const qredo_parameters* parameters);

	//------------------------------------------

	//!
	//! \brief Takes an arbitrary length buffer hashes it and creates signature out of it, returns 0 if computed succesfully
	//!
	//! \param signature        output : the result signature buffer, size is known and memory preallocated
	//! \param parameters       input  : the system parameters
	//! \param message          input  : the message to be signed,
	//! \param message_len      input  : the the length of the message to be signed
	//! \param secret_key       input  : the private key of the participant
	//!
	extern "C" int
	qredo_ring_participant_sign(
			unsigned char* signature,
			const qredo_parameters* parameters,
			const unsigned char* message,
			const unsigned int message_len,
			const unsigned char* secret_key);

	//------------------------------------------

	//!
	//! \brief Takes an threshold-concatenated participant signatures and creates a ring leader signature, returns 0 if computed succesfully
	//!
	//! \param signature              output : the rign leader signature, size is known and memory preallocated
	//! \param parameters             input  : the system parameters
	//! \param message                input  : the message to be signed,
	//! \param message_len            input  : the the length of the message to be signed
	//! \param participant_signatures input  : threshold-concatenated participants signatures, size is known
	//!
	extern "C" int
	qredo_ring_leader_sign(
			unsigned char* signature,
			const qredo_parameters* parameters,
			const unsigned char* message,
			const unsigned int message_len,
			const unsigned char* participant_signatures);

	//------------------------------------------

	//!
	//! \brief Verifies that the message was correctly signed, returns 0 if computed succesfully
	//!
	//! \param result            output : 1 if given message was verified, 0 otherwise
	//! \param parameters        input  : the system parameters
	//! \param message           input  : the buffer to be signed,
	//! \param message_length    input  : the the length of the buffer to be signed
	//! \param ring_signature    input  : the ring signature,
	//! \param ring_public_key   input  : the ring public key
	//!
	extern "C" int
	qredo_ring_verify(
			int* result,
			const qredo_parameters* parameters,
			const unsigned char* message,
			const unsigned int message_len,
			const unsigned char* ring_signature,
			const unsigned char* ring_public_key);

	//------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

