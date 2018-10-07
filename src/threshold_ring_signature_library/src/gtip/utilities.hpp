#ifndef GTIP_UTILITIES_HPP
#define GTIP_UTILITIES_HPP

#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

// NTL includes
#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

#include <glog/logging.h>

namespace gtip
{
	namespace utilities
	{
		//------------------------------------------

		template<typename T>
		std::string
		print_coefficients(
				const T& p)
		{
			std::stringstream ss;

			for (int i = 0; i <= NTL::deg(p); ++i)
			{
				if (NTL::IsZero(NTL::coeff(p, i)) == false)
				{
					ss << "X^" << i;

					if (i != NTL::deg(p))
					{
						ss << " + ";
					}
				}
			}

			return ss.str();
		}

		//------------------------------------------

		/*
		 * Returns string with [rows x cols] dimensions
		 */
		template<typename T>
		std::string
		dims_to_string(
				const NTL::Mat<T>& matrix)
		{
			std::stringstream ss;
			ss << "["
					<< matrix.NumRows()
					<< "x"
					<< matrix.NumCols()
					<< "]";
			return ss.str();
		}

		//------------------------------------------

		/*
		 * Returns matrix of
		 * [1 x number_of_elements] dimensions or
		 * [number_of_elements x 1] dimensions
		 */
		template<typename T>
		NTL::Mat<T>
		vec_to_mat(
				const NTL::Vec<T>& vec,
				const bool column_output = false,
				const bool verbose = false)
		{
			NTL::Mat<T> result;
			const unsigned int number_of_elements = vec.length();

			if (number_of_elements <= 0)
			{
				if (verbose == true)
				{
					LOG(ERROR) << "Input vector should not be empty";
				}

				throw std::invalid_argument("Failed to convert vec_to_mat");
			}

			result.SetDims(1, vec.length());
			auto& tmp_vec = result(1);

			for (unsigned int i = 0; i < number_of_elements; ++i)
			{
				tmp_vec[i] = vec[i];
			}

			if (column_output == true)
			{
				return NTL::transpose(result);
			}

			return result;
		}

		//------------------------------------------

		bool
		extended_euclidean_algorithm(
				const NTL::GF2EX& p0,
				const NTL::GF2EX& p1,
				NTL::GF2EX& inv);

		//------------------------------------------

		bool
		extended_euclidean_algorithm(
				const NTL::GF2EX& S,
				const NTL::GF2EX& G,
				NTL::GF2EX& T,
				NTL::GF2EX& d);

		//------------------------------------------

		unsigned long
		hash_to_matrix(
			const unsigned char* message,
			const unsigned long message_length,
			NTL::mat_GF2& hash);

		//------------------------------------------

		unsigned long
		hash_to_matrix(
				const std::vector<unsigned char>& message,
				NTL::mat_GF2& hash);

		//------------------------------------------

		unsigned long
		calculate_weight(
			const NTL::vec_GF2& v);

		//------------------------------------------

		bool
		convert_to(
				const NTL::vec_GF2& v,
				NTL::GF2EX& result);

		//------------------------------------------

		template<typename T>
		std::ostream& operator<<(
				std::ostream& os,
				const std::vector<T>& vec)
		{
		    for (auto& el : vec)
		    {
		        os << el << ' ';
		    }

		    return os;
		}

		//------------------------------------------

	}
}

#endif
