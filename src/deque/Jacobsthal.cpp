/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Jacobsthal.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:17 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:19 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/deque/PmergeMe.hpp"

std::deque<int> PmergeMeDeque::generateJacobsthalSequence(int maxSize)
{
	int	next;

	std::deque<int> seq;
	seq.push_back(0);
	seq.push_back(1);
	while (seq.back() < maxSize)
	{
		next = seq[seq.size() - 1] + 2 * seq[seq.size() - 2];
		seq.push_back(next);
	}
	return (seq);
}

std::deque<int> PmergeMeDeque::createInsertionOrder(int pendingSize)
{
	int	c;
	int	p;

	std::deque<int> jacobSeq = generateJacobsthalSequence(pendingSize);
	std::deque<int> result;
	for (size_t n = 1; n < jacobSeq.size(); ++n)
	{
		c = jacobSeq[n];
		p = jacobSeq[n - 1];
		for (int m = c; m > p; --m)
			if (m <= pendingSize)
				result.push_back(m);
	}
	return (result);
}
