The actual recurrence (what the code computes)

J(n) = J(n-1) + 2 · J(n-2)
       └─prev─┘   └two-back┘
Each number = the previous one, plus twice the one before that. That exact line is in your code:


int next = seq[seq.size()-1] + 2 * seq[seq.size()-2];
//              J(n-1)         +  2 *   J(n-2)
Walk it through — start from 1, 1:


1
1
3  = 1 + 2·1     ← prev 1, two-back 1
5  = 3 + 2·1     ← prev 3, two-back 1     (this is your "+2" — but two-back is still 1)
11 = 5 + 2·3     ← prev 5, two-back 3     (now two-back grew to 3 → jump of +6)
21 = 11 + 2·5    ← prev 11, two-back 5    (+10)
43 = 21 + 2·11   ← prev 21, two-back 11   (+22)
85 = 43 + 2·21   ← prev 43, two-back 21   (+42)




JACOBSTHAL numbers  →  1, 3, 5, 11, 21, 43 ...   (group boundaries: WHICH losers)
limit / window size →  3, 7, 15, 31, 63 ...      (2ᵏ − 1: HOW MANY to search)


























Using the 10-element case I verified earlier (./PmergeMe 10 3 20 15 30 25 40 8 50 45 → 3 8 10 15 20 25 30 40 45 50). After the winners are sorted:


main    = [10, 20, 30, 40, 50]
pending = [ 3, 15, 25,  8, 45]      insertion order = [1, 3, 2, 5, 4]
            b1  b2  b3  b4  b5
Insert b1 = 3 at the front for free → [3, 10, 20, 30, 40, 50], and set limit = 3.


┌─ GROUP {b3, b2}  ─────────────  limit = 3  (window = first 3 elements) ─┐

 k=1  insert b3 = 25   window = [ 3, 10, 20 | 30, 40, 50 ]   ← only first 3
                         └── 3 elems ──┘
      binary search: 10>25?no, 20>25?no  → 2 comparisons
      [ 3, 10, 20, 25, 30, 40, 50 ]

 k=2  insert b2 = 15   window = [ 3, 10, 20 | 25, 30, 40, 50 ]
                         └── 3 elems ──┘
      binary search: 10>15?no, 20>15?yes → 2 comparisons
      [ 3, 10, 15, 20, 25, 30, 40, 50 ]
└─────────────────────────────────────────────────────────────────────────┘

        ▲ order jumps 2 → 5 : new Jacobsthal group → limit = 2·3+1 = 7

┌─ GROUP {b5, b4}  ─────────────  limit = 7  (window = first 7 elements) ─┐

 k=3  insert b5 = 45   window = [ 3, 10, 15, 20, 25, 30, 40 | 50 ]  ← now first 7
                         └──────── 7 elems ────────┘
      binary search: 20>45?no, 30>45?no, 40>45?no → 3 comparisons
      [ 3, 10, 15, 20, 25, 30, 40, 45, 50 ]

 k=4  insert b4 = 8    window = [ 3, 10, 15, 20, 25, 30, 40 | 45, 50 ]
                         └──────── 7 elems ────────┘
      binary search: 20>8?yes, 10>8?yes, 3>8?no → 3 comparisons
      [ 3, 8, 10, 15, 20, 25, 30, 40, 45, 50 ]  ✅
└─────────────────────────────────────────────────────────────────────────┘



































The complete algorithm from start to finish (all pieces, in call order)
main.cpp — parse args, reject duplicates, print "Before", start the clock.
sort(data) ← recursive skeleton:
base case: size ≤ 1 → return.
createPairsAndSplit — pair up, winners→main, losers→pending, odd→end of pending. (Phase 1, descent)
sort(main) — ★ recursion on the winners, halving each level until base case. (descent continues)
reorderPending — re-link losers to their now-sorted winners. (Phase 2, ascent)
re-append oddElem if the level was odd.
insertPendingWithJacobsthal — the three-function pipeline you just studied (generateJacobsthalSequence → createInsertionOrder → bounded binarySearch inserts). (ascent)
return the finished chain to the level above.
main.cpp — stop the clock, print "After" + the two time lines; repeat with deque.
So: recursion = the descent (splitting winners out, level after level), Jacobsthal = the ascent (each level re-inserting its waiting losers). The message you quoted was a zoom-in on one Jacobsthal box; the funnel drawing from before is the frame it sits inside. Put the two together and you have the entire Ford-Johnson algorithm.

PmergeMe.cpp
great, explain the functions too, like create pairs and split, explain each and every single step from start to finish in the whole workflow and draw what happens to an example of a main and pending after everystep
Let me verify my chosen example against the real binary first — I want one where reorderPending actually shuffles (my earlier examples had already-sorted winners, which hid that step).

Bash Verify trace example 9 4 1 7 6 2 8
IN
cd /sgoinfre/mofarhat/pp09/Ford_Johnson_algorithm-main/ex02 && ./PmergeMe 9 4 1 7 6 2 8 | head -2

OUT
Before: 9 4 1 7 6 2 8
After: 1 2 4 6 7 8 9
Binary confirms 9 4 1 7 6 2 8 → 1 2 4 6 7 8 9, and my hand-trace matches it exactly. This input is perfect because the winners come out unsorted ([9,7,6]), so you'll finally see reorderPending do real work. Here is the entire workflow, every function, every step, with the state drawn after each one.

The cast (who calls whom)

main.cpp
 └── sort(data)                      ← recursive skeleton
      ├── createPairsAndSplit        ← STEP 1: make main/pending
      ├── sort(main)                 ← STEP 2: recurse on winners
      ├── reorderPending             ← STEP 3: re-link losers
      ├── (re-append oddElem)        ← STEP 4: rescue the odd one
      └── insertPendingWithJacobsthal← STEP 5: insert losers
           ├── createInsertionOrder  ←   which order?
           │    └── generateJacobsthalSequence
           └── binarySearch          ←   which slot?
Input for the whole trace: [9, 4, 1, 7, 6, 2, 8] (7 numbers, odd count).

LEVEL 0 — sort([9,4,1,7,6,2,8])
STEP 1 — createPairsAndSplit([9,4,1,7,6,2,8])

bool hasOdd = (data.size() % 2 == 1);            // 7 % 2 == 1 → true
for (size_t idx = 0; idx + 1 < data.size(); idx += 2) {   // idx = 0, 2, 4
    if (data[idx] > data[idx + 1]) {             // compare the pair
        result.main.push_back(data[idx]);        // bigger  → main
        result.pending.push_back(data[idx + 1]); // smaller → pending
    } else {
        result.main.push_back(data[idx + 1]);
        result.pending.push_back(data[idx]);
    }
}
if (hasOdd) result.pending.push_back(data.back());  // leftover 8 → end of pending
The loop walks two at a time (idx += 2), and idx + 1 < size stops it when no full pair remains:


data:    [ 9 , 4 , 1 , 7 , 6 , 2 , 8 ]
           └─┬─┘   └─┬─┘   └─┬─┘   └── leftover (no partner)
          (9,4)   (1,7)   (6,2)
          9 wins  7 wins  6 wins
          4 loses 1 loses 2 loses
STATE after step 1:


main    = [ 9 , 7 , 6 ]          ← winners (NOT sorted yet!)
pending = [ 4 , 1 , 2 , 8 ]      ← losers + odd at the end
            │   │   │   └─ odd leftover, no partner
            │   │   └───── partner of main[2]=6   (2 < 6 ✓)
            │   └───────── partner of main[1]=7   (1 < 7 ✓)
            └───────────── partner of main[0]=9   (4 < 9 ✓)
The invariant: same index = same pair, and pending[i] < main[i].

STEP 2a — stash the odd

bool isOdd = (data.size() % 2 == 1);              // true
int  oddElem = isOdd ? chains.pending.back() : 0; // oddElem = 8

oddElem = 8    (saved — because reorderPending is about to drop it)
STEP 2b — recurse: sort([9,7,6]) — descend to LEVEL 1
LEVEL 1 — sort([9,7,6])
STEP 1 (again) — createPairsAndSplit([9,7,6])

data:    [ 9 , 7 , 6 ]
           └─┬─┘   └── leftover
          (9,7)
          9 wins, 7 loses
STATE:


main    = [ 9 ]           oddElem = 6
pending = [ 7 , 6 ]
            │   └─ odd leftover
            └───── partner of 9
STEP 2 — recurse: sort([9]) — LEVEL 2: size 1 → BASE CASE, returns [9]. Recursion stops; now we climb.
STEP 3 — reorderPending(oldMain=[9], newMain=[9], oldPending=[7])

for (size_t x = 0; x < newMain.size(); ++x)       // each winner's NEW slot
    for (size_t y = 0; y < oldMain.size(); ++y)   // find its OLD slot
        if (oldMain[y] == newMain[x]) {
            newPending[x] = oldPending[y];        // carry the partner over
            break;
        }
Only one winner here, and it didn't move:


newMain[0]=9 → found at oldMain[0] → newPending[0] = oldPending[0] = 7
newPending = [ 7 ]           (note: odd 6 was NOT carried — dropped, as designed)
STEP 4 — re-append the odd: reorderedPend = [7, 6]
STEP 5 — insertPendingWithJacobsthal(main=[9], pending=[7,6])
createInsertionOrder(2): Jacobsthal seq [0,1,1,3] → groups: {1}, {}, {3→2: only 2 fits} → order [1, 2]


(A) pending[0]=7 → FRONT, free:            main = [ 7 , 9 ]

k=1: order=2 → pendIdx=1 → value 6, limit=3
     binarySearch([7,9], 6, 2): 7>6?yes→hi=-1 → pos 0
     insert at 0:                          main = [ 6 , 7 , 9 ]
LEVEL 1 returns [6, 7, 9] ⇧ climb back to level 0.

LEVEL 0 again — the climb (this is where it gets good)
We're back in the level-0 call with:


oldMain (before sorting) = [ 9 , 7 , 6 ]
sortedChain (just returned) = [ 6 , 7 , 9 ]     ← winners moved!
oldPending               = [ 4 , 1 , 2 , 8 ]
oddElem = 8
STEP 3 — reorderPending([9,7,6], [6,7,9], [4,1,2]) — the shuffle
The winners moved, so the index-link "same index = same pair" is broken. Rebuild it:


     oldMain = [ 9 , 7 , 6 ]        newMain = [ 6 , 7 , 9 ]
  oldPending = [ 4 , 1 , 2 ]

x=0: newMain[0]=6 → search oldMain… found at y=2 → newPending[0]=oldPending[2]=2
x=1: newMain[1]=7 → search oldMain… found at y=1 → newPending[1]=oldPending[1]=1
x=2: newMain[2]=9 → search oldMain… found at y=0 → newPending[2]=oldPending[0]=4
Drawn as arrows — each loser follows its winner to the winner's new position:


 BEFORE (old order)              AFTER (new order)
 main:    9    7    6            main:    6    7    9
          │    │    │                     │    │    │
 pending: 4    1    2            pending: 2    1    4
          
          9 moved slot 0→2, so 4 moved 0→2
          7 stayed  slot 1,  so 1 stayed 1
          6 moved slot 2→0, so 2 moved 2→0
STATE:


sortedChain   = [ 6 , 7 , 9 ]
reorderedPend = [ 2 , 1 , 4 ]     ← re-linked: 2<6 ✓, 1<7 ✓, 4<9 ✓
                                   (odd 8 dropped — it has no winner)
⚠️ This matching is by value (oldMain[y] == newMain[x]) — with duplicate values it could link the wrong partner. That's exactly why main.cpp rejects duplicates up front.

STEP 4 — rescue the odd: reorderedPend.push_back(8)

reorderedPend = [ 2 , 1 , 4 , 8 ]
                  b1  b2  b3  b4
STEP 5 — insertPendingWithJacobsthal([6,7,9], [2,1,4,8])
5a. generateJacobsthalSequence(4) → [0, 1, 1, 3, 5]

5b. createInsertionOrder(4):


posts (0,1) → push 1                → [1]
posts (1,1) → nothing               → [1]
posts (1,3) → push 3, 2             → [1, 3, 2]
posts (3,5) → 5>4 skip, push 4      → [1, 3, 2, 4]
5c. The insertion loop — watch main grow after every insert:


(A) b1 = pending[0] = 2 → FRONT, free (partner of smallest winner → new minimum)

    main:  [ 2 , 6 , 7 , 9 ]        limit = 3
             ▲

k=1: order=3 → pendIdx=2 → value b3 = 4      (k=1: no limit check)
     window = first 3:  [ 2 , 6 , 7 │ 9 ]
     bsearch 4: 6>4?yes→left … → pos 1
     main:  [ 2 , 4 , 6 , 7 , 9 ]
                  ▲            note: b3's winner 9 is at the window edge — the wall

k=2: order=2 (2>3? no → limit stays 3) → pendIdx=1 → value b2 = 1
     window = first 3:  [ 2 , 4 , 6 │ 7 , 9 ]
     bsearch 1: 4>1 → 2>1 → pos 0
     main:  [ 1 , 2 , 4 , 6 , 7 , 9 ]
              ▲

k=3: order=4 (4>2? YES → limit = 2·3+1 = 7) → pendIdx=3 → value b4 = 8 (the odd!)
     window = first 7 (whole chain):  [ 1 , 2 , 4 , 6 , 7 , 9 ]
     bsearch 8: 4>8?no → 7>8?no → 9>8?yes → pos 5
     main:  [ 1 , 2 , 4 , 6 , 7 , 8 , 9 ]
                                  ▲
LEVEL 0 returns [1, 2, 4, 6, 7, 8, 9] ✅ — exactly what the binary printed.

The whole journey on one page

                    [9, 4, 1, 7, 6, 2, 8]
                            │
              ┌─ STEP 1 split ─────────────────┐
              │ main=[9,7,6]  pend=[4,1,2,8]   │  odd=8 stashed
              └────────────┬───────────────────┘
                           │ STEP 2 recurse on winners
                    [9, 7, 6]
              ┌─ STEP 1 split ─────────────────┐
              │ main=[9]  pend=[7,6]           │  odd=6 stashed
              └────────────┬───────────────────┘
                           │ recurse
                        [9] ── base case, return [9]
                           │ climb ▲
              STEP 3 relink:   [7]
              STEP 4 + odd:    [7,6]
              STEP 5 insert:   [9] → [7,9] → [6,7,9]
                           │ return [6,7,9], climb ▲
              STEP 3 relink:   [4,1,2] follows winners → [2,1,4]
              STEP 4 + odd:    [2,1,4,8]
              STEP 5 insert:   [6,7,9] → [2,6,7,9] → [2,4,6,7,9]
                               → [1,2,4,6,7,9] → [1,2,4,6,7,8,9]
                           │
                    [1, 2, 4, 6, 7, 8, 9]   ✅
What each function contributed (one line each)
Function	Job	In this trace
createPairsAndSplit	pair, bigger→main, smaller→pending, odd→pending's end	[9,4,1,7,6,2,8] → main [9,7,6], pend [4,1,2,8]
sort (recursion)	sort the winners by calling itself	[9,7,6] → [6,7,9]
reorderPending	losers follow their winners to new slots	[4,1,2] → [2,1,4]
odd stash/re-append	save the partner-less element across the relink	8 dropped then re-added
generateJacobsthalSequence	fence posts	[0,1,1,3,5]
createInsertionOrder	posts → top-down groups	[1, 3,2, 4]
binarySearch	slot in the first limit elements	e.g. 4 → pos 1
insertPendingWithJacobsthal	front-insert + walk order + grow limit	[6,7,9] → [1,2,4,6,7,8,9]
That's the complete workflow — every function fired at least once, and at level 0 you saw the one thing earlier examples hid: the losers physically re-shuffling ([4,1,2] → [2,1,4]) to chase their winners. If you can redraw this page from memory with a fresh 7-number input, you own this algorithm.