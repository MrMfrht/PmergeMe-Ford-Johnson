# PmergeMe - Ford-Johnson Merge-Insertion Sort

A C++98 implementation of the **Ford-Johnson merge-insertion sort** algorithm using two STL containers (`std::vector` and `std::deque`), built as part of **42's CPP Module 09 (ex02)**.

The algorithm achieves a near-optimal number of comparisons by combining recursive pair-wise sorting with Jacobsthal-number-guided binary insertion.

## Build

```bash
make        # builds the PmergeMe executable
make re     # clean rebuild
make clean  # remove object files
make fclean # remove object files and executable
```

Requires a C++ compiler with C++98 support (tested with `c++` / `clang++` / `g++`).

## Usage

```bash
./PmergeMe [positive integers ...]
```

Pass any sequence of **unique positive integers** as arguments. The program sorts them using both containers and prints timing results.

```
$ ./PmergeMe 9 4 1 7 6 2 8
Before: 9 4 1 7 6 2 8
After: 1 2 4 6 7 8 9
Time to process a range of 7 elements with std::vector : 5.00000 us
Time to process a range of 7 elements with std::deque  : 4.00000 us
```

### Input rules

- All values must be **positive integers** (> 0).
- **Duplicates are not allowed** (the pairing/relinking step matches partners by value).
- Non-numeric or zero/negative inputs are rejected with an error message.

### Generating random input

```bash
# Linux/macOS — 3000 unique integers in [1, 100000]
./PmergeMe $(shuf -i 1-100000 -n 3000 | tr '\n' ' ')
```

## How the Algorithm Works

The Ford-Johnson algorithm minimises the total number of element comparisons needed to sort a sequence. It proceeds in two phases — a **recursive descent** (pairing and splitting) and an **ascent** (reinserting losers using Jacobsthal-ordered binary search).

### Call graph

```
sort(data)                         // recursive skeleton
 |-- createPairsAndSplit(data)     // pair elements, winners -> main, losers -> pending
 |-- sort(main)                    // recurse on the winners (halves the problem)
 |-- reorderPending(...)           // re-link losers to follow their now-sorted winners
 |-- (re-append odd element)       // rescue the unpaired element if count was odd
 +-- insertPendingWithJacobsthal() // insert losers into the sorted main chain
      |-- createInsertionOrder()   //   Jacobsthal-based insertion order
      |    +-- generateJacobsthalSequence()
      +-- binarySearch()           //   bounded binary search for each insertion
```

### Step-by-step walkthrough (input: `9 4 1 7 6 2 8`)

**1. Pair and split** — walk the input two at a time; the larger goes to `main`, the smaller to `pending`. An odd leftover is stashed separately.

```
data:    [9, 4, 1, 7, 6, 2, 8]
pairs:   (9,4) (1,7) (6,2)  leftover: 8
main   = [9, 7, 6]      (winners)
pending = [4, 1, 2, 8]   (losers + odd)
```

**2. Recurse on winners** — `sort([9, 7, 6])` applies the same algorithm until the base case (size <= 1), returning `[6, 7, 9]`.

**3. Reorder pending** — since the winners moved during the recursive sort, their paired losers must follow. Each loser is relocated to match its winner's new position.

```
Before:  main=[9,7,6]  pending=[4,1,2]
After:   main=[6,7,9]  pending=[2,1,4]   (2<6, 1<7, 4<9 -- pairs preserved)
```

**4. Re-append the odd element** — the stashed `8` is added to the end of pending: `[2, 1, 4, 8]`.

**5. Insert pending with Jacobsthal order** — pending elements are binary-inserted into the main chain in an order derived from Jacobsthal numbers (`J(n) = J(n-1) + 2*J(n-2)`), which keeps each binary search within a power-of-two window, minimising comparisons.

```
Insertion order: [1, 3, 2, 4]

Insert b1=2 at front      -> [2, 6, 7, 9]          limit=3
Insert b3=4, window=3     -> [2, 4, 6, 7, 9]       2 comparisons
Insert b2=1, window=3     -> [1, 2, 4, 6, 7, 9]    2 comparisons
Insert b4=8, window=7     -> [1, 2, 4, 6, 7, 8, 9] 3 comparisons
```

### Why Jacobsthal numbers?

The Jacobsthal sequence (`1, 1, 3, 5, 11, 21, 43, ...`) defines group boundaries for the insertion order. Within each group, elements are inserted in **descending** order against a search window of size `2^k - 1`. This ensures every binary search fits within a power-of-two range, keeping the comparison count at the theoretical minimum (`ceil(log2(window))` per element).

## Project Structure

```
.
|-- main.cpp                       # Entry point: parse, time, print
|-- Makefile                       # Build system
|-- lib/
|   |-- vector/PmergeMe.hpp        # Vector-based sorter class
|   +-- deque/PmergeMe.hpp         # Deque-based sorter class
|-- src/
|   |-- vector/
|   |   |-- PmergeMe.cpp           # Vector sort implementation
|   |   +-- Jacobsthal.cpp         # Jacobsthal sequence (vector)
|   +-- deque/
|       |-- PmergeMe.cpp           # Deque sort implementation
|       +-- Jacobsthal.cpp         # Jacobsthal sequence (deque)
|-- extra/
|   |-- comparison_counter.cpp     # Benchmark: counts comparisons
|   |-- jacobsthal_generator.cpp   # Utility: prints Jacobsthal sequences
|   +-- Makefile                   # Builds jacobsthal_generator
|-- test_average.sh                # Automated benchmark script
|-- jacobsthal_numbers.md          # Detailed algorithm walkthrough with examples
+-- LICENSE                        # MIT
```

## Extra Tools

### Comparison counter

Counts the exact number of comparisons performed by each container's sort, and validates correctness.

```bash
# Build (from project root)
c++ -Wall -Wextra -Werror -std=c++98 -I. extra/comparison_counter.cpp \
    src/vector/PmergeMe.cpp src/vector/Jacobsthal.cpp \
    src/deque/PmergeMe.cpp  src/deque/Jacobsthal.cpp \
    -o extra/comparison_counter

# Run
./extra/comparison_counter 9 4 1 7 6 2 8
# Output: vector 15 OK
#         deque  15 OK
```

### Jacobsthal generator

Prints the Jacobsthal sequence and the resulting insertion order for a given pending size.

```bash
cd extra && make && ./jacobsthal_generator 21
```

### Benchmark script

Runs 100 iterations with random unique inputs, checks correctness, and reports comparison statistics.

```bash
bash test_average.sh
```

The script is configurable via variables at the top: `NUM_TESTS`, `NUM_ELEMENTS`, `MAX_VALUE`.

Reference for `n=21`: Ford-Johnson worst case is **66 comparisons** (information-theoretic minimum: 65).

## License

[MIT](LICENSE) - Moussa Farhat
