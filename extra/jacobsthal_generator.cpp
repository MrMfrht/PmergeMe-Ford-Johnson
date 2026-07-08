/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jacobsthal_generator.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:27:54 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:27:56 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <sstream>

// Based on PmergeMe::generateJacobsthalSequence
std::vector<int> generateJacobsthalSequence(int maxSize)
{
    std::vector<int> jacob;
    jacob.push_back(0);
    jacob.push_back(1);

    while (jacob.back() < maxSize)
    {
        int next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
        jacob.push_back(next);
    }
    return jacob;
}

// Based on PmergeMe::createInsertionOrder
std::vector<int> createInsertionOrder(int pendingSize)
{
    std::vector<int> jacob = generateJacobsthalSequence(pendingSize);
    std::vector<int> order;
    for (size_t i = 1; i < jacob.size(); ++i)
    {
        int curr = jacob[i];
        int prev = jacob[i - 1];
        for (int j = curr; j > prev; --j)
            if (j <= pendingSize)
                order.push_back(j);
    }
    return order;
}

void printUsage()
{
    std::cout << "Usage: ./jacobsthal_generator <number>" << std::endl;
    std::cout << "  Generates Jacobsthal sequence and insertion order for given number" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printUsage();
        return 1;
    }

    std::stringstream ss(argv[1]);
    int n;

    if (!(ss >> n) || n <= 0)
    {
        std::cerr << "Error: Please provide a positive integer" << std::endl;
        return 1;
    }

    std::cout << "Input number: " << n << std::endl;
    std::cout << std::endl;

    // Generate Jacobsthal sequence
    std::vector<int> jacobSequence = generateJacobsthalSequence(n);

    std::cout << "Jacobsthal Sequence (up to " << n << "):" << std::endl;
    for (size_t i = 0; i < jacobSequence.size(); ++i)
    {
        std::cout << jacobSequence[i];
        if (i < jacobSequence.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl << std::endl;

    // Generate insertion order
    std::vector<int> insertionOrder = createInsertionOrder(n);

    std::cout << "Insertion Order - (for pending size " << n << "):" << std::endl;
    for (size_t i = 0; i < insertionOrder.size(); ++i)
    {
        std::cout << insertionOrder[i];
        if (i < insertionOrder.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;

    return 0;
}
