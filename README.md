# Minesweeper_AI

ABOUT
-------------------------
Implementing a Minesweeper AI agent which is able to solve the Minesweeper game.


ARCHITECTURE
-------------------------

We turned the Minesweeper board into a CSP problem where the numbered tiles that have a non-zero effective label (number on tile - number of mines found) became our uncovered domain and the covered tiles adjacent to those numbered tiles became our covered domain. We then assigned a binary value to each tile in our covered domain according to the effective label of its corresponding adjacent numbered tiles, where a value of 1 meant it was a mine, and 0 meant it was not a mine.
We recursively enumerated all possible assignments of our covered domain, backtracking whenever we ran into an inconsistent assignment. Then, for each tile in the covered domain, we checked the value of that tile in each consistent assignment. If every consistent assignment had a value of 1 (i.e. Prob(mine) = 1) for that tile, we declared it as a mine, likewise, if every assignment had it with a value of 0 (i.e. Prob(mine) = 0) we added it to our queue of tiles to uncover. Mixed values for a tile in the consistent assignments meant we would guess whether that tile was a mine or not. To ensure that our AI had sufficient time to solve each Minesweeper board, we cut off our recursion algorithm after 30 seconds and if the algorithm did not finish we then guess which tile to uncover because we only have 5 minutes to complete a world. By the end, we cut our execution time for 3000 worlds down to around an hour.

**Board size** | **Worlds complete ( out of 1000 )**:
- 5 X 5                    - 1000 
- 8 X 8                    -  875
- 16 X 16                  -  855
- 16 X 30                  -  378


### AUTHORS

- Vaanya Gupta
- John Yu
