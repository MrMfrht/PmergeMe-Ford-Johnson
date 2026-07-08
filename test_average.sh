#!/bin/bash

# Benchmarks the Ford-Johnson implementation:
#   1. checks that ./PmergeMe actually sorts,
#   2. measures comparison counts over many random inputs using
#      extra/comparison_counter (the graded program does not print counts).
#
# Values are generated UNIQUE (shuf) because the algorithm does not support
# duplicates — PmergeMe rejects them by design.

# Configuration
NUM_TESTS=100
NUM_ELEMENTS=21
MAX_VALUE=10000

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Build the main program if needed
if [ ! -f "./PmergeMe" ]; then
    echo -e "${YELLOW}PmergeMe executable not found. Attempting to compile...${NC}"
    make
    if [ $? -ne 0 ]; then
        echo "Compilation failed!"
        exit 1
    fi
fi

# Build the comparison counter if needed
COUNTER=./extra/comparison_counter
if [ ! -f "$COUNTER" ]; then
    echo -e "${YELLOW}Building extra/comparison_counter...${NC}"
    c++ -Wall -Wextra -Werror -std=c++98 -I. extra/comparison_counter.cpp \
        src/vector/PmergeMe.cpp src/vector/Jacobsthal.cpp \
        src/deque/PmergeMe.cpp src/deque/Jacobsthal.cpp \
        -o "$COUNTER"
    if [ $? -ne 0 ]; then
        echo "Compilation of comparison_counter failed!"
        exit 1
    fi
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Sanity check: does ./PmergeMe sort?${NC}"
echo -e "${BLUE}========================================${NC}"
numbers=$(shuf -i 1-$MAX_VALUE -n $NUM_ELEMENTS | tr '\n' ' ')
after=$(./PmergeMe $numbers | sed -n '2p' | sed 's/^After: //')
expected=$(echo $numbers | tr ' ' '\n' | sort -n | tr '\n' ' ' | sed 's/ $//')
if [ "$after" == "$expected" ]; then
    echo -e "${GREEN}OK — output matches sort -n${NC}\n"
else
    echo -e "${RED}FAIL — ./PmergeMe output does not match sort -n${NC}"
    exit 1
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Counting comparisons: $NUM_ELEMENTS unique elements${NC}"
echo -e "${BLUE}Running $NUM_TESTS iterations${NC}"
echo -e "${BLUE}========================================${NC}\n"

# Statistics for vector
total_comparisons_vector=0
min_comparisons_vector=999999
max_comparisons_vector=0

# Statistics for deque
total_comparisons_deque=0
min_comparisons_deque=999999
max_comparisons_deque=0

failures=0

# Run tests
for i in $(seq 1 $NUM_TESTS); do
    # Unique random numbers (the algorithm does not support duplicates)
    numbers=$(shuf -i 1-$MAX_VALUE -n $NUM_ELEMENTS | tr '\n' ' ')

    output=$($COUNTER $numbers 2>&1)

    comparisons_vector=$(echo "$output" | awk '$1=="vector"{print $2}')
    comparisons_deque=$(echo "$output" | awk '$1=="deque"{print $2}')

    if [ -z "$comparisons_vector" ] || [ -z "$comparisons_deque" ] \
        || echo "$output" | grep -q BAD; then
        echo -e "${RED}Error in iteration $i:${NC} $output"
        failures=$((failures + 1))
        continue
    fi

    # Update statistics for vector
    total_comparisons_vector=$((total_comparisons_vector + comparisons_vector))
    if [ $comparisons_vector -lt $min_comparisons_vector ]; then
        min_comparisons_vector=$comparisons_vector
    fi
    if [ $comparisons_vector -gt $max_comparisons_vector ]; then
        max_comparisons_vector=$comparisons_vector
    fi

    # Update statistics for deque
    total_comparisons_deque=$((total_comparisons_deque + comparisons_deque))
    if [ $comparisons_deque -lt $min_comparisons_deque ]; then
        min_comparisons_deque=$comparisons_deque
    fi
    if [ $comparisons_deque -gt $max_comparisons_deque ]; then
        max_comparisons_deque=$comparisons_deque
    fi

    # Progress indicator
    if [ $((i % 10)) -eq 0 ]; then
        echo -e "${GREEN}Completed $i/$NUM_TESTS tests...${NC}"
    fi
done

# Calculate averages
done_tests=$((NUM_TESTS - failures))
average_vector=$(echo "scale=2; $total_comparisons_vector / $done_tests" | bc)
average_deque=$(echo "scale=2; $total_comparisons_deque / $done_tests" | bc)

# Display results
echo -e "\n${BLUE}========================================${NC}"
echo -e "${BLUE}           RESULTS${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Number of tests:      ${GREEN}$done_tests${NC} (failures: $failures)"
echo -e "Elements per test:    ${GREEN}$NUM_ELEMENTS${NC}"
echo -e "\n${BLUE}--- Vector-based sorting ---${NC}"
echo -e "Average comparisons:  ${GREEN}$average_vector${NC}"
echo -e "Minimum comparisons:  ${YELLOW}$min_comparisons_vector${NC}"
echo -e "Maximum comparisons:  ${YELLOW}$max_comparisons_vector${NC}"
echo -e "\n${BLUE}--- Deque-based sorting ---${NC}"
echo -e "Average comparisons:  ${GREEN}$average_deque${NC}"
echo -e "Minimum comparisons:  ${YELLOW}$min_comparisons_deque${NC}"
echo -e "Maximum comparisons:  ${YELLOW}$max_comparisons_deque${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "\n${YELLOW}Reference for n=21: Ford-Johnson worst case F(21) = 66${NC}"
echo -e "${YELLOW}comparisons (information-theoretic minimum: 65).${NC}"
