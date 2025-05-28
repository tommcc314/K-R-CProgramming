#include <stdio.h>

#define EMPTY 0
#define SUCCESS 1
#define NOPROGRESS 0
#define ERROR -1
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

int board[9][9] = { /*Original board state. Zeros indicate empty cells*/
  {0,0,0,0,6,9,4,8,0},
  {0,0,3,1,0,0,0,0,0},
  {0,0,0,0,4,7,0,0,0},
  {6,0,0,0,0,4,0,0,0},
  {0,0,0,0,0,3,0,0,7},
  {5,7,0,0,0,0,0,0,0},
  {0,1,0,7,0,0,8,9,0},
  {0,0,7,0,3,0,6,5,4},
  {0,0,9,0,0,8,7,0,0}
};
int candidates[9][9][9]; /*Binary array for every cell indicating candidate existance*/
int rows[9][9], cols[9][9], boxes[9][9]; /* Binary array indicating whether a number exists in a particular row, col or 3x3 box*/

/*  Searches for an int, up to a specified number of elements 
    Returns 1 if found, 0 if not */
int find(int target, int* ptr, int n) {
  while (n--)
    if (target == *ptr++)
      return 1;
  return 0;
}

/*  Return Box ID for a given cell*/
int boxnum(int r, int c) {
  return 3 * (r / 3) + c / 3;
}

/*  Prints the board state*/
void printboard(int board[9][9]) {
  for (int i = 0; i < 9; ++i) {
    if (i > 0 && i % 3 == 0)
      printf("-----------\n");
    for (int j = 0; j < 9; ++j) {
      if (j > 0 && j % 3 == 0)
        printf("|");
      printf("%d", board[i][j]);
    }
    printf("\n");
  }
}

/*  Checks if board is solved
    Returns 1 if solved, 0 if not solved */
int issolved(int board[9][9]) {
  int ret = 1;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (!board[i][j])
        ret = 0;
  return ret;
}

/*  Fills specified cell with value and updates secondary data structures accordingly.*/
void fillcell(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9], int i, int j, int val) {
  rows[i][val - 1] = cols[j][val - 1] = boxes[boxnum(i,j)][val - 1] = 1;
  for (int k = 0; k < 9; ++k)
    candidates[k][j][val - 1] = candidates[i][k][val - 1] = 0;
  for (int a = i - i % 3; a < 3 + i - i % 3; ++a)
    for (int b = j - j % 3; b < 3 + j - j % 3; ++b)
      candidates[a][b][val - 1] = 0;
  printf("Filling cell (%d,%d) with %d\n", i, j, val);
  board[i][j] = val;
}

/*  Compute candidates for a given cell, and updates the cell value if only 1 candidate is found (Last Digit)
    Returns 1 if candidates have been updated, 0 if no candidates were updated, and -1 if the cell has no candidates*/
int getcandidates(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9], int i, int j, int debug) {
  int ret = NOPROGRESS, numCandidates = 0, lastDigit;
  for (int k = 0; k < 9; ++k)
    if (rows[i][k] || cols[j][k] || boxes[boxnum(i,j)][k]) {
      if (candidates[i][j][k])
        ret = SUCCESS;
      candidates[i][j][k] = 0;
    }
  for (int k = 0; k < 9; ++k)
    if (candidates[i][j][k]) {
      numCandidates++;
      lastDigit = k + 1;
    }  
  if (!numCandidates) {
    if (debug)
      printf("ERROR: cell (%d,%d) has no candidates\n", i, j);
    return ERROR;
  }
  if (numCandidates == 1) {
    fillcell(board, candidates, rows, cols, boxes, i, j, lastDigit);
    ret = SUCCESS;
  }
  return ret;
}

/*  Finds and fills naked singles (when a cell can only contain one number)
    Returns 1 if at least one naked single is found, 0 if not, and -1 if getcandidates finds an invalid state*/
int nakedsingle(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9], int debug) {
  printf("In naked single");
  int ret = NOPROGRESS, curret;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY) {
        if ((curret = getcandidates(board, candidates, rows, cols, boxes, i, j, debug)) == ERROR)
          return curret;
        ret = max(ret, curret);
      }
  printf("returning %d", ret);
  return ret;
}

/*  Finds and fills hidden singles (when a number can only go in one spot in row/col/box)
    Returns 1 if at least one hidden single is found, 0 if not*/
int hiddensingle(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9]) {
  printf("In hiddensingle");
  int rowcandcounts[9][9][3] = {0}, colcandcounts[9][9][3] = {0}, boxcandcounts[9][9][3] = {0}, ret = NOPROGRESS;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j) {
      if (board[i][j] == EMPTY)
        for (int k = 0; k < 9; ++k)
          if (candidates[i][j][k]) {
            rowcandcounts[i][k][0]++;
            rowcandcounts[i][k][1] = i, rowcandcounts[i][k][2] = j;
          }
      if (board[j][i] == EMPTY)
        for (int k = 0; k < 9; ++k)
          if (candidates[j][i][k]) {
            colcandcounts[j][k][0]++;
            colcandcounts[j][k][1] = j, colcandcounts[j][k][2] = i;
          }
    }
  for (int box = 0; box < 9; ++box)
    for (int i = 3 * (box / 3); i < 3 + 3 * (box / 3); ++i)
      for (int j = 3 * (box % 3); j < 3 + 3 * (box % 3); ++j)
        if (board[i][j] == EMPTY)
          for (int k = 0; k < 9; ++k)
            if (candidates[i][j][k]) {
              boxcandcounts[box][k][0]++;
              boxcandcounts[box][k][1] = i, boxcandcounts[box][k][2] = j;
            }
  for (int i = 0; i < 9; ++i)
    for (int k = 0; k < 9; ++k) {
      if (rowcandcounts[i][k][0] == 1) {
        fillcell(board, candidates, rows, cols, boxes, rowcandcounts[i][k][1], rowcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
        
      if (colcandcounts[i][k][0] == 1) {
        fillcell(board, candidates, rows, cols, boxes, colcandcounts[i][k][1], colcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
      if (boxcandcounts[i][k][0] == 1) {
        fillcell(board, candidates, rows, cols, boxes, boxcandcounts[i][k][1], boxcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
    }
  printf("returning %d", ret);
  return ret;
}

/*  Finds and fills locked candidates (when a number can only go in one row/col in a box)
    Returns 1 if at least one useful candidate elimination is found, 0 if not, -1 if invalid state */
int lockedcandidate(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9], int debug) {
  int ret = NOPROGRESS;
  for (int i = 0; i < 9; ++i) {
    int rowcandcounts[9][3] = {0};
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY)
        for (int k = 0; k < 9; ++k)
          if (candidates[i][j][k])
            rowcandcounts[k][j / 3]++;
    for (int k = 0; k < 9; ++k) {
      int numBoxes = 0, onlyBox;
      for (int b = 0; b < 3; ++b)
        if (rowcandcounts[9][b] > 0) {
          numBoxes++;
          onlyBox = b;
        }
      if (numBoxes == 1)
        for (int tempi = 3 * (i / 3); tempi < 3 + 3 * (i / 3); ++tempi) {
          if (tempi == i) continue;
          for (int j = 3 * (onlyBox / 3); j < 3 + 3 * (onlyBox / 3); ++j) {
            if (board[tempi][j] == EMPTY && candidates[tempi][j][k]) {
              candidates[tempi][j][k] = 0;
              if (getcandidates(board, candidates, rows, cols, boxes, tempi, j, debug) == ERROR)
                return ERROR;
              ret = SUCCESS;
            }
            candidates[tempi][j][k] = 0;
          }
        }
    }
  }
  for (int j = 0; j < 9; ++j) {
    int colcandcounts[9][3] = {0};
    for (int i = 0; i < 9; ++i)
      if (board[i][j] == EMPTY)
        for (int k = 0; k < 9; ++k)
          if (candidates[i][j][k])
            colcandcounts[k][i / 3]++;
    for (int k = 0; k < 9; ++k) {
      int numBoxes = 0, onlyBox;
      for (int b = 0; b < 3; ++b)
        if (colcandcounts[9][b] > 0) {
          numBoxes++;
          onlyBox = b;
        }
      if (numBoxes == 1)
        for (int tempj = 3 * (j / 3); tempj < 3 + 3 * (j / 3); ++tempj) {
          if (tempj == j) continue;
          for (int i = 3 * (onlyBox / 3); i < 3 + 3 * (onlyBox / 3); ++i) {
            if (board[i][tempj] == EMPTY && candidates[i][tempj][k]) {
              candidates[i][tempj][k] = 0;
              if (getcandidates(board, candidates, rows, cols, boxes, i, tempj, debug) == ERROR)
                return ERROR;
              ret = SUCCESS;
            }
          }
        }
    }
  }
  for (int b = 0; b < 9; ++b) {
    int boxrowcandcounts[9][3] = {0}, boxcolcandcounts[9][3] = {0};
    for (int i = 3 * (b / 3); i < 3 + 3 * (b / 3); ++i)
      for (int j = 3 * (b % 3); j < 3 + 3 * (b % 3); ++j)
        if (board[i][j] == EMPTY)
          for (int k = 0; k < 9; ++k)
            if (candidates[i][j][k]) {
              boxrowcandcounts[k][i - 3 * (b / 3)]++;
              boxcolcandcounts[k][j - 3 * (b % 3)]++;
            }
    for (int k = 0; k < 9; ++k) {
      int numRows = 0, onlyRow, numCols = 0, onlyCol;
      for (int i = 0; i < 3; ++i)
        if (boxrowcandcounts[9][i] > 0) {
          numRows++;
          onlyRow = i;
        }
      if (numRows == 1)
        for (int j = 0; j < 9; ++j) {
          if (j >= 3 * (b % 3) && j < 3 + 3 * (b % 3)) continue;
          int i = 3 * (b / 3) + onlyRow;
          if (board[i][j] == EMPTY && candidates[i][j][k]) {
            candidates[i][j][k] = 0;
            if (getcandidates(board, candidates, rows, cols, boxes, i, j, debug) == ERROR)
              return ERROR;
            ret = SUCCESS;
          }
        }
      for (int j = 0; j < 3; ++j)
        if (boxcolcandcounts[9][j] > 0) {
          numCols++;
          onlyCol = j;
        }
      if (numCols == 1)
        for (int i = 0; i < 9; ++i) {
          if (i >= 3 * (b / 3) && i < 3 + 3 * (b / 3)) continue;
          int j = 3 * (b % 3) + onlyCol;
          if (board[i][j] == EMPTY && candidates[i][j][k]) {
            candidates[i][j][k] = 0;
            if (getcandidates(board, candidates, rows, cols, boxes, i, j, debug) == ERROR)
              return ERROR;
            ret = SUCCESS;
          }
        }
    }
  }
  return ret;
}

/*  Finds and fills naked doubles (when two cells have the same two candidates in a row/col/box)
    Returns 1 if at least one useful candidate elimination is found, 0 if not, -1 if invalid state */
int nakeddouble(int board[9][9], int candidates[9][9][9], int rows[9][9], int cols[9][9], int boxes[9][9], int debug) {
  int ret = NOPROGRESS, pairidx[2], pairval[2];
  for (int i = 0; i < 9; ++i) {
    for (int j1 = 0; j1 < 9; ++j1) {
      if (board[i][j1] == EMPTY)
        for (int j2 = j1 + 1; j2 < 9; ++j2) {
          int isPair = 1, numCandidates = 0;
          if (board[i][j2] == EMPTY)
            for (int k = 0; k < 9; ++k)
              if (candidates[i][j1][k]) {
                if (numCandidates < 2)
                  pairval[numCandidates] = k;
                numCandidates++;
              }
              if (candidates[i][j1][k] != candidates[i][j2][k])
                isPair = 0;
          if (isPair && numCandidates == 2) {
            pairidx[0] = j1, pairidx[1] = j2;
            for (int j = 0; j < 9; ++j) {
              if (board[i][j] == EMPTY && j != pairidx[0] && j != pairidx[1]) {
                if (candidates[i][j][pairval[0]]) {
                  candidates[i][j][pairval[0]] = 0;
                  ret = SUCCESS;
                }
                
                if (getcandidates(board, candidates, rows, cols, boxes, i, j, debug) == ERROR)
                  return ERROR;
              }

            }
          }
        }
    }
  }
}

/*  Initializes the board state and candidates
    Returns 1 on success, -1 if invalid state */
int init() {
  int ret;
  printf("In Init");
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      for (int k = 0; k < 9; ++k)
        candidates[i][j][k] = 1;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] != EMPTY) {
        rows[i][board[i][j] - 1] = 1;
        cols[j][board[i][j] - 1] = 1;
        boxes[boxnum(i,j)][board[i][j] - 1] = 1;
      }
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY)
        if (getcandidates(board, candidates, rows, cols, boxes, i, j, 1) == ERROR)
          return ERROR;
  return SUCCESS;
}

/*  The parent routine for all other solving functions.
    Returns 1 if solved, -1 if not */
int solve() {
  int ret = SUCCESS, curret;
  if (init() == ERROR)
    return ERROR;
  while (ret == SUCCESS) {
    if (issolved(board))
      return SUCCESS;
    ret = NOPROGRESS;
    ret = max(ret, hiddensingle(board, candidates, rows, cols, boxes));
    if ((curret = nakedsingle(board, candidates, rows, cols, boxes, 1)) == -1)
      return curret;
    ret = max(ret, curret);
  }
  return ERROR;
}

int main(int argc, char *argv[]) {
  int ret = solve();
  if (ret == SUCCESS)
    printf("Solved!\n");
  else
    printf("Unable to solve. Last board state:\n");
  printboard(board);
  return 0;
}