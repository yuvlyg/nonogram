# nonogram solver

B&W nonogram solver written in C++

**Algorithm**
Find a row or column with a few options
If all options agree on certain tiles, mark those tiles
If didn't find such row/column, choose a tile and recuresively try both options, using backtracking

Puzzles from the [nonogram-db](https://github.com/mikix/nonogram-db) repository can be converted using `non_to_files.py`

