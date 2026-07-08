/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:06 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:08 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_VECTOR_HPP
#define PMERGEME_VECTOR_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <iomanip>


typedef struct s_PairChainsVector{
    std::vector<int> main;
    std::vector<int> pending;
}   t_PairChainsVector;

class PmergeMeVector{
    private:
        int comparisonCount;

        t_PairChainsVector createPairsAndSplit(const std::vector<int>& data);

        std::vector<int> reorderPending(const std::vector<int>& oldMain,
                                        const std::vector<int>& newMain,
                                        const std::vector<int>& oldPending);

        std::vector<int> generateJacobsthalSequence(int maxSize);
        std::vector<int> createInsertionOrder(int pendingSize);

        int binarySearch(const std::vector<int>& arr, int value, int maxIndex);
        void insertPendingWithJacobsthal(std::vector<int>& main,
                                         const std::vector<int>& pending);

    public:
        PmergeMeVector();
        PmergeMeVector(const PmergeMeVector& other);
        PmergeMeVector& operator=(const PmergeMeVector& other);
        ~PmergeMeVector();

        std::vector<int> sort(const std::vector<int>& data);
        int getComparisonCount() const;
        void resetComparisonCount();
};

#endif