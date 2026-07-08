/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:01 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:03 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_DEQUE_HPP
#define PMERGEME_DEQUE_HPP

#include <iostream>
#include <stdlib.h>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <iomanip>


typedef struct s_PairChainsDeque{
    std::deque<int> main;
    std::deque<int> pending;
}   t_PairChainsDeque;

class PmergeMeDeque{
    private:
        int comparisonCount;

        t_PairChainsDeque createPairsAndSplit(const std::deque<int>& data);

        std::deque<int> reorderPending(const std::deque<int>& oldMain,
                                        const std::deque<int>& newMain,
                                        const std::deque<int>& oldPending);

        std::deque<int> generateJacobsthalSequence(int maxSize);
        std::deque<int> createInsertionOrder(int pendingSize);

        int binarySearch(const std::deque<int>& arr, int value, int maxIndex);
        void insertPendingWithJacobsthal(std::deque<int>& main,
                                         const std::deque<int>& pending);

    public:
        PmergeMeDeque();
        PmergeMeDeque(const PmergeMeDeque& other);
        PmergeMeDeque& operator=(const PmergeMeDeque& other);
        ~PmergeMeDeque();

        std::deque<int> sort(const std::deque<int>& data);
        int getComparisonCount() const;
        void resetComparisonCount();
};

#endif