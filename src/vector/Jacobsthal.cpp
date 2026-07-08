/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Jacobsthal.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:29 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:30 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/vector/PmergeMe.hpp"

std::vector<int> PmergeMeVector::generateJacobsthalSequence(int maxSize)
{
    std::vector<int> seq;
    seq.push_back(0);
    seq.push_back(1);
    while (seq.back() < maxSize) {
        int next = seq[seq.size() - 1] + 2 * seq[seq.size() - 2];
        seq.push_back(next);
    }
    return seq;
}

std::vector<int> PmergeMeVector::createInsertionOrder(int pendingSize)
{
    std::vector<int> jacobSeq = generateJacobsthalSequence(pendingSize);
    std::vector<int> result;
    for (size_t n = 1; n < jacobSeq.size(); ++n) {
        int c = jacobSeq[n];
        int p = jacobSeq[n - 1];
        for (int m = c; m > p; --m)
            if (m <= pendingSize)
                result.push_back(m);
    }
    return result;
}
