/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:40 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 18:02:06 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/deque/PmergeMe.hpp"
#include "lib/vector/PmergeMe.hpp"
#include <ctime>
#include <iomanip>
#include <string>

// Parse argv into a vector of positive ints; false (with message) on bad input
static bool	parseArguments(int argc, char **argv, std::vector<int> &out)
{
	for (int i = 1; i < argc; ++i)
	{
		int val = std::atoi(argv[i]);
		if (val <= 0)
		{
			std::cerr << "Error: all numbers must be positive." << std::endl;
			return (false);
		}
		out.push_back(val);
	}
	return (true);
}

// The algorithm does not support repeated values, so reject them up front
static bool	hasDuplicates(const std::vector<int> &v)
{
	for (size_t i = 0; i < v.size(); ++i)
		for (size_t j = i + 1; j < v.size(); ++j)
			if (v[i] == v[j])
				return (true);
	return (false);
}

// "Before:" must echo the raw arguments (e.g. "007"), not the parsed ints
static void	printBefore(int argc, char **argv)
{
	std::cout << "Before: ";
	for (int i = 1; i < argc; ++i)
	{
		std::cout << argv[i];
		if (i < argc - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

// Space-separated print of any container ("After:" line)
template <typename Container>
static void	printSequence(const Container &c)
{
	for (size_t i = 0; i < c.size(); ++i)
	{
		std::cout << c[i];
		if (i + 1 < c.size())
			std::cout << " ";
	}
	std::cout << std::endl;
}

// Return the elapsed time in microseconds; the timed window covers building
// the container from the parsed values AND the sort, because the subject
// requires timing "both the sorting part and the data management part"
template <typename Sorter, typename Container>
static double	timedSort(Sorter &sorter, const std::vector<int> &values,
		Container &output)
{
	clock_t	start = clock();
	Container	input(values.begin(), values.end());
	output = sorter.sort(input);
	clock_t	end = clock();
	return (static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0);
}

static void	printTime(const std::string &container, size_t count, double micros)
{
	std::cout << "Time to process a range of " << count << " elements with "
		<< container << " : " << std::fixed << std::setprecision(5)
		<< micros << " us" << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./PmergeMe [positive integers]" << std::endl;
		return (1);
	}

	std::vector<int> vectorData;
	if (!parseArguments(argc, argv, vectorData))
		return (1);
	if (hasDuplicates(vectorData))
	{
		std::cerr << "Error: duplicate values are not allowed." << std::endl;
		return (1);
	}

	printBefore(argc, argv);

	// First container: std::vector
	PmergeMeVector		vectorSorter;
	std::vector<int>	vectorSorted;
	double				vectorMicros = timedSort(vectorSorter, vectorData,
			vectorSorted);

	std::cout << "After: ";
	printSequence(vectorSorted);

	// Second container: std::deque (same input, fed through its own sorter)
	PmergeMeDeque		dequeSorter;
	std::deque<int>		dequeSorted;
	double				dequeMicros = timedSort(dequeSorter, vectorData,
			dequeSorted);

	printTime("std::vector", vectorSorted.size(), vectorMicros);
	printTime("std::deque", dequeSorted.size(), dequeMicros);
	return (0);
}
