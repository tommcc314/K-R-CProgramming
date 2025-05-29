#include <stdio.h>
#include <string.h>

#define EMPTY 0
#define SUCCESS 1
#define NOPROGRESS 0
#define ERROR -1
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

int board[9][9] = { /*Original board state. Zeros indicate empty cells*/
{0,0,0,0,0,0,4,0,9},
{0,8,0,0,3,0,5,6,0},
{0,5,6,0,0,0,0,0,7},
{3,9,0,0,0,0,0,0,0},
{0,0,8,0,0,0,1,0,0},
{0,0,2,9,0,0,0,0,5},
{0,0,0,0,4,0,0,1,0},
{9,0,1,6,0,2,0,0,0},
{0,0,3,0,9,0,0,0,2}
};
int solve(int board[9][9], int candidates[9][9][9], int depth);
int candidates[9][9][9]; /*Binary array for every cell indicating candidate existance*/

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

/*  Fills specified cell with value and updates candidates accordingly.*/
void fillcell(int board[9][9], int candidates[9][9][9], int i, int j, int val) {
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
int getcandidates(int board[9][9], int candidates[9][9][9], int i, int j) {
  int ret = NOPROGRESS, numCandidates = 0, lastDigit;
  if (board[i][j])
    return ret;
  for (int k = 0; k < 9; ++k)
    if (candidates[i][j][k]) {
      numCandidates++;
      lastDigit = k + 1;
    }  
  if (!numCandidates) {
    printf("ERROR: cell (%d,%d) has no candidates\n", i, j);
    return ERROR;
  }
  if (numCandidates == 1) {
    fillcell(board, candidates, i, j, lastDigit);
    ret = SUCCESS;
  }
  return ret;
}

/*  Finds and fills naked singles (when a cell can only contain one number)
    Returns 1 if at least one naked single is found, 0 if not, and -1 if getcandidates finds an invalid state*/
int nakedsingle(int board[9][9], int candidates[9][9][9]) {
  printf("In naked single\n");
  int ret = NOPROGRESS, curret;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY) {
        if ((curret = getcandidates(board, candidates, i, j)) == ERROR)
          return curret;
        ret = max(ret, curret);
      }
  printf("returning %d\n", ret);
  return ret;
}

/*  Finds and fills hidden singles (when a number can only go in one spot in row/col/box)
    Returns 1 if at least one hidden single is found, 0 if not*/
int hiddensingle(int board[9][9], int candidates[9][9][9]) {
  printf("In hiddensingle\n");
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
        fillcell(board, candidates, rowcandcounts[i][k][1], rowcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
        
      if (colcandcounts[i][k][0] == 1) {
        fillcell(board, candidates, colcandcounts[i][k][1], colcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
      if (boxcandcounts[i][k][0] == 1) {
        fillcell(board, candidates, boxcandcounts[i][k][1], boxcandcounts[i][k][2], k + 1);
        ret = SUCCESS;
      }
    }
  printf("returning %d\n", ret);
  return ret;
}

/*  Finds and fills locked candidates (when a number can only go in one row/col in a box)
    Returns 1 if at least one useful candidate elimination is found, 0 if not, -1 if invalid state */
int lockedcandidate(int board[9][9], int candidates[9][9][9]) {
  printf("In Locked candidate\n");
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
        if (rowcandcounts[k][b] > 0) {
          numBoxes++;
          onlyBox = b;
        }
      if (numBoxes == 1)
        for (int tempi = 3 * (i / 3); tempi < 3 + 3 * (i / 3); ++tempi) {
          if (tempi == i) continue;
          for (int j = 3 * onlyBox; j < 3 + 3 * onlyBox; ++j) {
            if (board[tempi][j] == EMPTY && candidates[tempi][j][k]) {
              printf("Removing canididate %d from (%d,%d) because it can only be in box %d for row %d\n", k + 1, tempi, j, onlyBox, i);
              candidates[tempi][j][k] = 0;
              if (getcandidates(board, candidates, tempi, j) == ERROR)
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
        if (colcandcounts[k][b] > 0) {
          numBoxes++;
          onlyBox = b;
        }
      if (numBoxes == 1)
        for (int tempj = 3 * (j / 3); tempj < 3 + 3 * (j / 3); ++tempj) {
          if (tempj == j) continue;
          for (int i = 3 * onlyBox; i < 3 + 3 * onlyBox; ++i) {
            if (board[i][tempj] == EMPTY && candidates[i][tempj][k]) {
              candidates[i][tempj][k] = 0;
              printf("Removing canididate %d from (%d,%d) because it can only be in box %d for col %d\n", k + 1, i, tempj, onlyBox, j);
              if (getcandidates(board, candidates, i, tempj) == ERROR)
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
        if (boxrowcandcounts[k][i] > 0) {
          numRows++;
          onlyRow = i;
        }
      if (numRows == 1)
        for (int j = 0; j < 9; ++j) {
          if (j >= 3 * (b % 3) && j < 3 + 3 * (b % 3)) continue;
          int i = 3 * (b / 3) + onlyRow;
          if (board[i][j] == EMPTY && candidates[i][j][k]) {
            printf("Removing canididate %d from (%d,%d) because it can only be in row %d for box %d\n", k + 1, i, j, onlyRow, b);
            candidates[i][j][k] = 0;
            if (getcandidates(board, candidates, i, j) == ERROR)
              return ERROR;
            ret = SUCCESS;
          }
        }
      for (int j = 0; j < 3; ++j)
        if (boxcolcandcounts[k][j] > 0) {
          numCols++;
          onlyCol = j;
        }
      if (numCols == 1)
        for (int i = 0; i < 9; ++i) {
          if (i >= 3 * (b / 3) && i < 3 + 3 * (b / 3)) continue;
          int j = 3 * (b % 3) + onlyCol;
          if (board[i][j] == EMPTY && candidates[i][j][k]) {
            printf("Removing canididate %d from (%d,%d) because it can only be in col %d for box %d\n", k + 1, i, j, onlyCol, b);
            candidates[i][j][k] = 0;
            if (getcandidates(board, candidates, i, j) == ERROR)
              return ERROR;
            ret = SUCCESS;
          }
        }
    }
  }
  printf("Returning %d\n", ret);
  return ret;
}

/*  Finds and fills naked doubles (when two cells have the same two candidates in a row/col/box)
    Returns 1 if at least one useful candidate elimination is found, 0 if not, -1 if invalid state */
int nakeddouble(int board[9][9], int candidates[9][9][9]) {
  printf("In Nakeddouble\n");
  int ret = NOPROGRESS, pairval[2];
  for (int i = 0; i < 9; ++i)
    for (int j1 = 0; j1 < 9; ++j1)
      for (int j2 = j1 + 1; j2 < 9; ++j2) 
        if (board[i][j1] == EMPTY && board[i][j2] == EMPTY) {
          int numCandidates = 0;
          for (int k = 0; k < 9; ++k)
            if (candidates[i][j1][k] || candidates[i][j2][k]) {
              if (numCandidates < 2)
                pairval[numCandidates] = k;
              numCandidates++;
            }
          if (numCandidates == 2)
            for (int j = 0; j < 9; ++j) {
              if (board[i][j] == EMPTY && j != j1 && j != j2) {
                for (int k = 0; k < 2; ++k) {
                  if (candidates[i][j][pairval[k]]) {
                    printf("Removing canididate %d from (%d,%d) because of %d,%d pair\n", pairval[k] + 1, i, j, pairval[0] + 1, pairval[1] + 1);
                    candidates[i][j][pairval[k]] = 0;
                    ret = SUCCESS;
                  }
                  if (getcandidates(board, candidates, i, j) == ERROR)
                    return ERROR;
                }
              }
            }
        }
  for (int j = 0; j < 9; ++j)
    for (int i1 = 0; i1 < 9; ++i1)
      for (int i2 = i1 + 1; i2 < 9; ++i2) 
        if (board[i1][j] == EMPTY && board[i2][j] == EMPTY) {
          int numCandidates = 0;
          for (int k = 0; k < 9; ++k)
            if (candidates[i1][j][k] || candidates[i2][j][k]) {
              if (numCandidates < 2)
                pairval[numCandidates] = k;
              numCandidates++;
            }
          if (numCandidates == 2)
            for (int i = 0; i < 9; ++i) {
              if (board[i][j] == EMPTY && i != i1 && i != i2) {
                for (int k = 0; k < 2; ++k) {
                  if (candidates[i][j][pairval[k]]) {
                    printf("Removing canididate %d from (%d,%d) because of %d,%d pair\n", pairval[k] + 1, i, j, pairval[0] + 1, pairval[1] + 1);
                    candidates[i][j][pairval[k]] = 0;
                    ret = SUCCESS;
                  }
                  if (getcandidates(board, candidates, i, j) == ERROR)
                    return ERROR;
                }
              }
            }
        }
  for (int b = 0; b < 9; ++b)
    for (int a1 = 0; a1 < 9; ++a1) {
      int i1 = 3 * (b / 3) + a1 / 3, j1 = 3 * (b % 3) + a1 % 3;
      for (int a2 = a1 + 1; a2 < 9; ++a2) {
        int i2 = 3 * (b / 3) + a2 / 3, j2 = 3 * (b % 3) + a2 % 3;
        if (board[i1][j1] == EMPTY && board[i2][j2] == EMPTY) {
          int numCandidates = 0;
          for (int k = 0; k < 9; ++k)
            if (candidates[i1][j1][k] || candidates[i2][j2][k]) {
              if (numCandidates < 2)
                pairval[numCandidates] = k;
              numCandidates++;
            }
          if (numCandidates == 2)
            for (int a = 0; a < 9; ++a) {
              int i = 3 * (b / 3) + a / 3, j = 3 * (b % 3) + a % 3;
              if (board[i][j] == EMPTY && a != a1 && a != a2) {
                for (int k = 0; k < 2; ++k) {
                  if (candidates[i][j][pairval[k]]) {
                    printf("Removing canididate %d from (%d,%d) because of %d,%d pair. a1=%d,a2=%d,a=%d\n", pairval[k] + 1, i, j, pairval[0] + 1, pairval[1] + 1, a1, a2, a);
                    candidates[i][j][pairval[k]] = 0;
                    ret = SUCCESS;
                  }
                  if (getcandidates(board, candidates, i, j) == ERROR)
                    return ERROR;
                }
              }
            }
        }
      }
    }
  printf("Returning %d\n", ret);
  return ret;
}

/*  Finds and fills naked triples (when three cells have the same two candidates in a row/col/box)
    Returns 1 if at least one useful candidate elimination is found, 0 if not, -1 if invalid state */
int nakedtriple(int board[9][9], int candidates[9][9][9]) {
  printf("In naked triple\n");
  int ret = NOPROGRESS, tripleval[3];
  for (int i = 0; i < 9; ++i)
    for (int j1 = 0; j1 < 9; ++j1)
      for (int j2 = j1 + 1; j2 < 9; ++j2) 
        for (int j3 = j2 + 1; j3 < 9; ++j3)
          if (board[i][j1] == EMPTY && board[i][j2] == EMPTY && board[i][j3] == EMPTY) {
            int numCandidates = 0;
            for (int k = 0; k < 9; ++k)
              if (candidates[i][j1][k] || candidates[i][j2][k] || candidates[i][j3][k]) {
                if (numCandidates < 3)
                  tripleval[numCandidates] = k;
                numCandidates++;
              }
            if (numCandidates == 3) {
              for (int j = 0; j < 9; ++j)
                if (board[i][j] == EMPTY && j != j1 && j != j2 && j != j3)
                  for (int k = 0; k < 3; ++k) {
                    if (candidates[i][j][tripleval[k]]) {
                      printf("Removing canididate %d from (%d,%d) because of %d,%d,%d triple. \n", tripleval[k] + 1, i, j, tripleval[0] + 1, tripleval[1] + 1, tripleval[2] + 1);
                      candidates[i][j][tripleval[k]] = 0;
                      ret = SUCCESS;
                    }
                    if (getcandidates(board, candidates, i, j) == ERROR)
                      return ERROR;
                  }
            }
          }
  for (int j = 0; j < 9; ++j)
    for (int i1 = 0; i1 < 9; ++i1)
      for (int i2 = i1 + 1; i2 < 9; ++i2) 
        for (int i3 = i2 + 1; i3 < 9; ++i3)
          if (board[i1][j] == EMPTY && board[i2][j] == EMPTY && board[i3][j] == EMPTY) {
            int numCandidates = 0;
            for (int k = 0; k < 9; ++k)
              if (candidates[i1][j][k] || candidates[i2][j][k] || candidates[i3][j][k]) {
                if (numCandidates < 3)
                  tripleval[numCandidates] = k;
                numCandidates++;
              }
            if (numCandidates == 3) {
              for (int i = 0; i < 9; ++i)
                if (board[i][j] == EMPTY && i != i1 && i != i2 && i != i3)
                  for (int k = 0; k < 3; ++k) {
                    if (candidates[i][j][tripleval[k]]) {
                      printf("Removing canididate %d from (%d,%d) because of %d,%d,%d triple for %d,%d,%d\n", tripleval[k] + 1, i, j, tripleval[0] + 1, tripleval[1] + 1, tripleval[2] + 1, i1, i2, i3);
                      candidates[i][j][tripleval[k]] = 0;
                      ret = SUCCESS;
                    }
                    if (getcandidates(board, candidates, i, j) == ERROR)
                      return ERROR;
                  }
            }
          }
  for (int b = 0; b < 9; ++b)
    for (int a1 = 0; a1 < 9; ++a1) {
      int i1 = 3 * (b / 3) + a1 / 3, j1 = 3 * (b % 3) + a1 % 3;
      for (int a2 = a1 + 1; a2 < 9; ++a2) {
        int i2 = 3 * (b / 3) + a2 / 3, j2 = 3 * (b % 3) + a2 % 3;
        for (int a3 = a2 + 1; a3 < 9; ++a3) {
          int i3 = 3 * (b / 3) + a3 / 3, j3 = 3 * (b % 3) + a3 % 3;
          if (board[i1][j1] == EMPTY && board[i2][j2] == EMPTY && board[i3][j3] == EMPTY) {
            int numCandidates = 0;
            for (int k = 0; k < 9; ++k)
              if (candidates[i1][j1][k] || candidates[i2][j2][k] || candidates[i3][j3][k]) {
                if (numCandidates < 3)
                  tripleval[numCandidates] = k;
                numCandidates++;
              }
            if (numCandidates == 3) {
              for (int a = 0; a < 9; ++a) {
                int i = 3 * (b / 3) + a / 3, j = 3 * (b % 3) + a % 3;
                if (board[i][j] == EMPTY && a != a1 && a != a2 && a != a3)
                  for (int k = 0; k < 3; ++k) {
                    if (candidates[i][j][tripleval[k]]) {
                      printf("Removing canididate %d from (%d,%d) because of %d,%d,%d triple\n", tripleval[k] + 1, i, j, tripleval[0] + 1, tripleval[1] + 1, tripleval[2] + 1);
                      candidates[i][j][tripleval[k]] = 0;
                      ret = SUCCESS;
                    }
                    if (getcandidates(board, candidates, i, j) == ERROR)
                      return ERROR;
                  }
              }
            }
          }
        }  
      }
    }
  printf("Returning %d\n", ret);
  return ret;
}

/*  Attempts to solve using a forcing chain: Set a cell with 2 candidates to one of the values, then verify
    Returns 1 if a cell is forced, 0 if not */
int chain(int board[9][9], int candidates[9][9][9], int depth) {
  int newboard[9][9], newcandidates[9][9][9], ret;
  printf("In chain\n");
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY) {
        int currcands[2], numCandidates = 0;
        for (int k = 0; k < 9; ++k)
          if (candidates[i][j][k]) {
            if (numCandidates < 2)
              currcands[numCandidates] = k;
            numCandidates++;
          }
        if (numCandidates == 2) {
          memcpy(newboard, board, 9*9*sizeof(int));
          memcpy(newcandidates, candidates, 9*9*9*sizeof(int));
          fillcell(newboard, newcandidates, i, j, currcands[0] + 1);
          if ((ret = solve(newboard, newcandidates, depth)) == ERROR) {
            fillcell(board, candidates, i, j, currcands[1] + 1);
            return SUCCESS;
          }
          if (ret == SUCCESS) {
            memcpy(board, newboard, 9*9*sizeof(int));
            memcpy(candidates, newcandidates, 9*9*9*sizeof(int));
            return SUCCESS;
          }
          memcpy(newboard, board, 9*9*sizeof(int));
          memcpy(newcandidates, candidates, 9*9*9*sizeof(int));
          fillcell(newboard, newcandidates, i, j, currcands[1] + 1);
          if ((ret = solve(newboard, newcandidates, depth)) == ERROR) {
            fillcell(board, candidates, i, j, currcands[0] + 1);
            return SUCCESS;
          }
          if (ret == SUCCESS) {
            memcpy(board, newboard, 9*9*sizeof(int));
            memcpy(candidates, newcandidates, 9*9*9*sizeof(int));
            return SUCCESS;
          }
        }
      }
  printf("Chain could not find anything.\n");
  return NOPROGRESS;
}

/*  Initializes the board state and candidates
    Returns 1 on success, -1 if invalid state */
int init() {
  int ret;
  printf("In Init\n");
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      for (int k = 0; k < 9; ++k)
        candidates[i][j][k] = 1;
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] != EMPTY)
        fillcell(board, candidates, i, j, board[i][j]);
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j] == EMPTY)
        if (getcandidates(board, candidates, i, j) == ERROR)
          return ERROR;
  return SUCCESS;
}

/*  The solving routine which calls all other solving functions.
    Returns 1 if solved, 0 if not, -1 if invalid board state */
int solve(int board[9][9], int candidates[9][9][9], int depth) {
  int ret = SUCCESS, curret;
  while (ret == SUCCESS) {
    if (issolved(board))
      return SUCCESS;
    ret = NOPROGRESS;
    if ((curret = nakeddouble(board, candidates)) == ERROR)
      return curret;
    ret = max(ret, curret);
    if ((curret = lockedcandidate(board, candidates)) == ERROR)
      return curret;
    ret = max(ret, curret);
    if ((curret = nakedtriple(board, candidates)) == ERROR)
      return curret;
    ret = max(ret, curret);
    ret = max(ret, hiddensingle(board, candidates));
    if ((curret = nakedsingle(board, candidates)) == ERROR)
      return curret;
    ret = max(ret, curret);
    if (ret == NOPROGRESS && depth < 1)
      ret = max(ret, chain(board, candidates, depth + 1));
  }
  return NOPROGRESS;
}

int main(int argc, char *argv[]) {
  if (init() == ERROR)
    printf("Invalid beginning board state!\n");
  printf("Cell (3,2) has 1: %d, 5: %d", candidates[3][2][0], candidates[3][2][4]);
  int ret = solve(board, candidates, 0);
  if (ret == SUCCESS)
    printf("Solved!\n");
  else
    printf("Unable to solve. Last board state:\n");
  printboard(board);
  return 0;
}