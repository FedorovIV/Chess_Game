#include "chessSet.hpp"


void MoveLog::turnRecordingOn() {
    logStatus = RECORDING_ON;
}

void MoveLog::turnRecordingOff() {
    log.assign(1, SavedMoving_t(lastState->mainPieceStep, lastState->mainPieceSide, lastState->mainPieceKind));
    lastState = log.begin();

    logStatus = RECORDING_OFF;
}

void MoveLog::createNewRecord(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind) {
    if (logStatus == RECORDING_OFF) {
        log.assign(1, SavedMoving_t(mainPieceStep, mainPieceSide, mainPieceKind));
        lastState = log.begin();
    } else {
        auto it = log.begin();
        while (it != lastState) {
            it = log.erase(it);
        }

        log.push_front(SavedMoving_t(mainPieceStep, mainPieceSide, mainPieceKind));
        lastState = log.begin();
    }
}

void MoveLog::addNewMoveRecord(std::shared_ptr<Piece> &piecePtr, PieceMoveStatus status) {
    if (logStatus == RECORDING_OFF) {
        return;
    }

    lastState->piecesMove.push_front({piecePtr->copySharedPtr(), status});
}

bool MoveLog::goNextRecord(Board &board, RulesController &rulesController) {
    if (lastState == log.begin() || logStatus == RECORDING_OFF) {
        return false;
    }

    --lastState;

    auto it = lastState->piecesMove.end();

    while (it != lastState->piecesMove.begin()) {
        --it;
        CellPos piecePos = it->first->getPos();

        if (!piecePos.isRealPos()) {
            continue;
        }

        board.deletePiece(piecePos);

        if (it->second == ADDITION || it->second == ADDING_PRIORITY) {
            board.setPiece(it->first->copySharedPtr());
        } 
        
        if (it->second == ADDING_PRIORITY) {
            rulesController.setPriorityPiece(board.getPiecePtr(piecePos));
        }

        if (it->second == REMOVAL_PRIORITY) {
            rulesController.deletePriorityPiece(it->first->getSide());
        }
    }

    rulesController.changePlayerTurn();

    return true;
}

bool MoveLog::goPreviousRecord(Board &board, RulesController &rulesController) {
    if (lastState == log.end() || std::next(lastState) == log.end() || logStatus == RECORDING_OFF) {
        return false;
    }

    for (auto it = lastState->piecesMove.begin(); it != lastState->piecesMove.end(); ++it) {
        CellPos piecePos = it->first->getPos();
        
        if (!piecePos.isRealPos()) {
            continue;
        }

        board.deletePiece(piecePos);

        if (it->second == REMOVAL || it->second == REMOVAL_PRIORITY) {
            board.setPiece(it->first->copySharedPtr());
        } 
        
        if (it->second == REMOVAL_PRIORITY) {
            rulesController.setPriorityPiece(board.getPiecePtr(piecePos));
        }

        if (it->second == ADDING_PRIORITY) {
            rulesController.deletePriorityPiece(it->first->getSide());
        }
    }
    ++lastState;

    rulesController.changePlayerTurn();

    return true;
}

step MoveLog::getLastMoveStep() {
    return lastState->mainPieceStep;
}

Color MoveLog::getLastMoveColor() {
    return lastState->mainPieceSide;
}

PieceKind MoveLog::getLastMoveKind() {
    return lastState->mainPieceKind;
}
