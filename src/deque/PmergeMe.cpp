/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mofarhat <mofarhat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:28:22 by mofarhat          #+#    #+#             */
/*   Updated: 2026/07/01 13:28:24 by mofarhat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/deque/PmergeMe.hpp"
#include <algorithm>

PmergeMeDeque::PmergeMeDeque() : comparisonCount(0) {}

PmergeMeDeque::PmergeMeDeque(const PmergeMeDeque& other)
    : comparisonCount(other.comparisonCount) {}

PmergeMeDeque& PmergeMeDeque::operator=(const PmergeMeDeque& other)
{
    if (this != &other)
        comparisonCount = other.comparisonCount;
    return *this;
}

PmergeMeDeque::~PmergeMeDeque() {}

int PmergeMeDeque::getComparisonCount() const
{
    return comparisonCount;
}

void PmergeMeDeque::resetComparisonCount()
{
    comparisonCount = 0;
}

t_PairChainsDeque PmergeMeDeque::createPairsAndSplit(const std::deque<int>& data)
{
    t_PairChainsDeque result;
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

std::deque<int> PmergeMeDeque::reorderPending(const std::deque<int>& oldMain,
                                          const std::deque<int>& newMain,
                                          const std::deque<int>& oldPending)
{
    std::deque<int> newPending;
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


int PmergeMeDeque::binarySearch(const std::deque<int>& arr, int value, int maxIndex)
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

void PmergeMeDeque::insertPendingWithJacobsthal(std::deque<int>& main,
                                           const std::deque<int>& pending)
{
    if (pending.empty()) return;
    std::deque<int> order = createInsertionOrder(pending.size());
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

std::deque<int> PmergeMeDeque::sort(const std::deque<int>& data)
{
    if (data.empty()) return std::deque<int>();
    if (data.size() == 1) return data;

    t_PairChainsDeque chains = createPairsAndSplit(data);
    bool isOdd = (data.size() % 2 == 1);
    int oddElem = isOdd ? chains.pending.back() : 0;

    std::deque<int> sortedChain = sort(chains.main);
    std::deque<int> reorderedPend = reorderPending(chains.main, sortedChain, chains.pending);
    if (isOdd) reorderedPend.push_back(oddElem);
    insertPendingWithJacobsthal(sortedChain, reorderedPend);
    return sortedChain;
}
