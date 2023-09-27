#include "chess.hpp"


void Chess::getPossibleSteps(CellPos pos, std::list<CellPos> &steps) {
    board.getPiecePtr(pos)->getPossibleSteps(board, moveLog, steps);

    rulesController.excludeChecksFromPossibleSteps(pos, steps, board, moveLog);
}

Color Chess::getPieceSide(CellPos pos) {
    if (!board.isCellEmpty(pos)) {
        return board.getPiecePtr(pos)->getSide();
    }

    return NO_COLOR;
}

bool Chess::classicStartPosition(RulesController &rulesController) {
    if (board.size != 8) {
        return false;
    }

    board.clear();

    for (int i = 1; i <= 8; ++i) {
        board.setPiece(std::make_shared<Pawn>(CellPos(i, 2), WHITE, true));
        board.setPiece(std::make_shared<Pawn>(CellPos(i, 7), BLACK, true));
    }

    for (int i = 0; i < 2; ++i) {
        board.setPiece(std::make_shared<Rook>(CellPos(1 + i * 7, 1), WHITE));
        board.setPiece(std::make_shared<Rook>(CellPos(1 + i * 7, 8), BLACK));
        board.setPiece(std::make_shared<Knight>(CellPos(2 + i * 5, 1), WHITE));
        board.setPiece(std::make_shared<Knight>(CellPos(2 + i * 5, 8), BLACK));
        board.setPiece(std::make_shared<Bishop>(CellPos(3 + i * 3, 1), WHITE));
        board.setPiece(std::make_shared<Bishop>(CellPos(3 + i * 3, 8), BLACK));
    }

    board.setPiece(std::make_shared<Queen>(CellPos(4, 1), WHITE));
    board.setPiece(std::make_shared<Queen>(CellPos(4, 8), BLACK));

    board.setPiece(std::make_shared<King>(CellPos(5, 1), WHITE));
    rulesController.setPriorityPiece(board.getPiecePtr(CellPos(5, 1)));
    board.setPiece(std::make_shared<King>(CellPos(5, 8), BLACK));
    rulesController.setPriorityPiece(board.getPiecePtr(CellPos(5, 8)));

    return true;
} 

bool Chess::move(CellPos pos1, CellPos pos2) {
    if (!pos1.isInSquareBoard(board.size) || !pos2.isInSquareBoard(board.size)) {
        return false;
    }

    if (!board.isCellEmpty(pos1) && pos1 != pos2 && board.getPiecePtr(pos1)->isPossibleStep(board, moveLog, pos2)) {
        board.getPiecePtr(pos1)->move(board, moveLog, rulesController, pos2);
        board.deletePiecesFromList();

        if (rulesController.getGameStatus() != CHANGE_PIECE) {
            rulesController.changePlayerTurn();
            rulesController.mateAndStalemateProcessing(board, moveLog);
        }

        return true;
    }

    return false;
}

void Chess::changePiece(PieceKind kind) {
    if (rulesController.getGameStatus() == CHANGE_PIECE) {
        rulesController.getReplacementPiece()->replace(board, moveLog, rulesController, kind);
        rulesController.clearPiecesToChoose();
        rulesController.changePlayerTurn();
        rulesController.mateAndStalemateProcessing(board, moveLog);
    }
}

bool Chess::isRecordingOn() {
    return moveLog.isLogRecordOn();
}

void Chess::setRecordingStatus(LogStatus recordingStatus) {
    if (recordingStatus == RECORDING_OFF) {
        moveLog.turnRecordingOff();
    } else {
        moveLog.turnRecordingOn();
    }
}

bool Chess::nextMove() {
    return moveLog.goNextRecord(board, rulesController);
}

bool Chess::previousMove() {
    return moveLog.goPreviousRecord(board, rulesController);
}

std::vector<int> Chess::getBoard() {
    std::vector<int> fullBoard;

    for (int i = 0; i < board.size * board.size; ++i) {
        if (board.isCellEmpty(i)) {
            fullBoard.push_back(0);
        } else {
            fullBoard.push_back(board.getPiecePtr(i)->getKind() * board.getPiecePtr(i)->getSide());
        }
    }
    return fullBoard;
}
