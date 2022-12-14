#include "king.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

King::King(int color, int name) : Piece(color, name) {}

bool King::canMove(Board& board, Move& mv) {
  try {
  
  // normal move
  if (mv.end->isEmpty() || color != mv.end->getPiece()->getColor()) {
    // can move to neighbouring squares
    int hoz_move = dist(mv.start->getCol(), mv.end->getCol());
    int ver_move = dist(mv.start->getRow(), mv.end->getRow());
    if (!(hoz_move <= 1 && ver_move <= 1)) {
      return false;
    }
  // castling move
  // can only castle under specific conditions
  // follows fisher random chess: https://en.wikipedia.org/wiki/Fischer_random_chess#Castling_rules
  } else {
    // Rook with same color at the end square
    if (!(mv.end->getPiece()->getName() == board.ROOK && mv.end->getPiece()->getColor() == color)) {
      return false;
    // Must both king and rook be unmoved and the king currently unattacked
    } else if (has_moved || mv.end->getPiece()->getHasMoved() || board.isChecked(color)) {
      return false;
    // Must be on the same row
    } else if (mv.end->getRow() != mv.start->getRow()) {
      return false;
    }
    // Determine whether it is kingside or queenside castling
    int rook_step, king_step, rook_end, king_end;
    if (mv.start->getCol() < mv.end->getCol()) { // if rook is on the rightside
      mv.is_kingside_castling = true;
      int f_file = 'f' - 'a';
      int g_file = 'g' - 'a';
      rook_end = f_file;
      king_end = g_file;
    } else { // if rook is on the leftside
      mv.is_queenside_castling = true;
      int c_file = 'c' - 'a';
      int d_file = 'd' - 'a';
      rook_end = d_file;
      king_end = c_file;
    }
    rook_step = sign(disp(mv.end->getCol(), rook_end));
    king_step = sign(disp(mv.start->getCol(), king_end));
    // All squares between rook's initial and final squares must be vacant
    int rook_cur_col = mv.end->getCol() + rook_step;
    while (rook_cur_col != rook_end) {
      if (!board.getSquare(mv.end->getRow(), rook_cur_col)->isEmpty()) {
        return false;
      }
      rook_cur_col += rook_step;
    }
    // All squares between king's initial and final squares must be vacant AND unattacked
    int king_cur_col = mv.start->getCol() + king_step;
    auto my_shared_ptr = position->getPiece();
    while (king_cur_col != king_end) {
      if (!board.getSquare(mv.start->getRow(), king_cur_col)->isEmpty()) {
        return false;
      } else if (board.isDangerousFor(board.getSquare(mv.start->getRow(), king_cur_col), my_shared_ptr)) {
        return false;
      }
      king_cur_col += king_step;
    }
  }
  mv.is_pseudo_legal = true;
  } catch (...) {
            std::cout << "here\n";
        }
  return true;
}

vector<Move> King::listPseudoLegalMoves(Board& board) {
  vector<Move> pseudo_legal_moves;
  int col = position->getCol();
  int row = position->getRow();
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (board.inRange(row+i, col+j)) {
        Move mv = Move(position, board.getSquare(row+i, col+j));
        if (canMove(board, mv)) {
            pseudo_legal_moves.push_back(mv);
        }
      }
    }
  }
  try {
  for (int k = 0; k < board.getCols(); ++k) {
    if (!board.getSquare(row, k)->isEmpty()) {
      Move mv = Move(position, board.getSquare(row, k));
      if (canMove(board, mv)) {
          pseudo_legal_moves.push_back(mv);
      }
    }
  }
  } catch (...) {
    std::cout << "here";
  }
  return pseudo_legal_moves;
}

string King::printText() {
  if (color == WHITE) {
    return "K";
  } else {
    return "k";
  }
}


