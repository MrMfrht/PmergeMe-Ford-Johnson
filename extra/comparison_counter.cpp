/* ************************************************************************** */
/*                                                                            */
/*   comparison_counter.cpp — defense/benchmark helper (NOT part of the      */
/*   graded program). Sorts the arguments with both PmergeMe implementations  */
/*   and prints the number of comparisons each one performed.                 */
/*                                                                            */
/*   Build & run (from ex02/):                                                */
/*     c++ -Wall -Wextra -Werror -std=c++98 -I. extra/comparison_counter.cpp \*/
/*         src/vector/PmergeMe.cpp src/vector/Jacobsthal.cpp \                */
/*         src/deque/PmergeMe.cpp  src/deque/Jacobsthal.cpp \                 */
/*         -o extra/comparison_counter                                        */
/*     ./extra/comparison_counter `shuf -i 1-10000 -n 21 | tr '\n' ' '`       */
/*                                                                            */
/*   Reference for n = 21 unique values: the Ford-Johnson worst case is      */
/*   F(21) = 66 comparisons (information-theoretic minimum: 65).              */
/*                                                                            */
/*   Note: values must be unique — like the main program, the algorithm      */
/*   does not support duplicates (re-pairing in reorderPending is by value).  */
/*                                                                            */
/* ************************************************************************** */

#include "lib/vector/PmergeMe.hpp"
#include "lib/deque/PmergeMe.hpp"
#include <cstdlib>
#include <iostream>

static bool isSortedVec(const std::vector<int> &v)
{
	for (size_t i = 1; i < v.size(); ++i)
		if (v[i - 1] > v[i])
			return (false);
	return (true);
}

static bool isSortedDeq(const std::deque<int> &d)
{
	for (size_t i = 1; i < d.size(); ++i)
		if (d[i - 1] > d[i])
			return (false);
	return (true);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./comparison_counter [unique positive integers]"
			<< std::endl;
		return (1);
	}

	std::vector<int> input;
	for (int i = 1; i < argc; ++i)
	{
		int val = std::atoi(argv[i]);
		if (val <= 0)
		{
			std::cerr << "Error: all numbers must be positive." << std::endl;
			return (1);
		}
		input.push_back(val);
	}
	for (size_t i = 0; i < input.size(); ++i)
		for (size_t j = i + 1; j < input.size(); ++j)
			if (input[i] == input[j])
			{
				std::cerr << "Error: duplicate values are not allowed."
					<< std::endl;
				return (1);
			}

	PmergeMeVector		vectorSorter;
	std::vector<int>	vectorSorted = vectorSorter.sort(input);

	std::deque<int>		dequeInput(input.begin(), input.end());
	PmergeMeDeque		dequeSorter;
	std::deque<int>		dequeSorted = dequeSorter.sort(dequeInput);

	bool okVec = (vectorSorted.size() == input.size() && isSortedVec(vectorSorted));
	bool okDeq = (dequeSorted.size() == input.size() && isSortedDeq(dequeSorted));

	std::cout << "vector " << vectorSorter.getComparisonCount()
		<< (okVec ? " OK" : " BAD") << std::endl;
	std::cout << "deque " << dequeSorter.getComparisonCount()
		<< (okDeq ? " OK" : " BAD") << std::endl;
	return ((okVec && okDeq) ? 0 : 1);
}
