/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:35 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:36 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/vector/PmergeMe.hpp"
#include <algorithm>

PmergeMeVector::PmergeMeVector() : comparisonCount(0) {}

PmergeMeVector::PmergeMeVector(const PmergeMeVector& other)
    : comparisonCount(other.comparisonCount) {}

PmergeMeVector& PmergeMeVector::operator=(const PmergeMeVector& other)
{
    if (this != &other)
        comparisonCount = other.comparisonCount;
    return *this;
}

PmergeMeVector::~PmergeMeVector() {}

int PmergeMeVector::getComparisonCount() const
{
    return comparisonCount;
}

void PmergeMeVector::resetComparisonCount()
{
    comparisonCount = 0;
}

t_PairChainsVector PmergeMeVector::createPairsAndSplit(const std::vector<int>& data)
{
    t_PairChainsVector result;
    if (data.empty() || data.size() == 1) {
        if (data.size() == 1) result.main.push_back(data[0]);
        return result;
    }

    bool hasOdd = (data.size() % 2 == 1);
    for (size_t idx = 0; idx + 1 < data.size(); idx += 2) {
        comparisonCount++;
        if (data[idx] > data[idx + 1]) {
            result.main.push_back(data[idx]);
            result.pending.push_back(data[idx + 1]);
        } else {
            result.main.push_back(data[idx + 1]);
            result.pending.push_back(data[idx]);
        }
    }
    if (hasOdd) result.pending.push_back(data.back());
    return result;
}

std::vector<int> PmergeMeVector::reorderPending(const std::vector<int>& oldMain,
                                          const std::vector<int>& newMain,
                                          const std::vector<int>& oldPending)
{
    std::vector<int> newPending;
    newPending.resize(newMain.size());

    for (size_t x = 0; x < newMain.size(); ++x) {
        for (size_t y = 0; y < oldMain.size(); ++y) {
            if (oldMain[y] == newMain[x]) {
                newPending[x] = oldPending[y];
                break;
            }
        }
    }
    return newPending;
}


int PmergeMeVector::binarySearch(const std::vector<int>& arr, int value, int maxIndex)
{
    if (arr.empty()) return 0;
    int lo = 0, hi = std::min(maxIndex, (int)arr.size() - 1);
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        comparisonCount++;
        if (arr[mid] > value) hi = mid - 1;
        else lo = mid + 1;
    }
    return lo;
}

void PmergeMeVector::insertPendingWithJacobsthal(std::vector<int>& main,
                                           const std::vector<int>& pending)
{
    if (pending.empty()) return;
    std::vector<int> order = createInsertionOrder(pending.size());
    main.insert(main.begin(), pending[0]);
    int limit = 3;
    for (size_t k = 1; k < order.size(); ++k) {
        int pendIdx = order[k] - 1;
        if (k > 1 && order[k] > order[k - 1])
            limit = 2 * limit + 1;
        int pos = binarySearch(main, pending[pendIdx], limit - 1);
        main.insert(main.begin() + pos, pending[pendIdx]);
    }
}

std::vector<int> PmergeMeVector::sort(const std::vector<int>& data)
{
    if (data.empty()) return std::vector<int>();
    if (data.size() == 1) return data;

    t_PairChainsVector chains = createPairsAndSplit(data);
    bool isOdd = (data.size() % 2 == 1);
    int oddElem = isOdd ? chains.pending.back() : 0;

    std::vector<int> sortedChain = sort(chains.main);
    std::vector<int> reorderedPend = reorderPending(chains.main, sortedChain, chains.pending);
    if (isOdd) reorderedPend.push_back(oddElem);
    insertPendingWithJacobsthal(sortedChain, reorderedPend);
    return sortedChain;
}
