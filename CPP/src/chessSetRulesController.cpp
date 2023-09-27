#include "chessSet.hpp"


GameStatus RulesController::getGameStatus() {
    return gameStatus;
}

void RulesController::setPriorityPiece(std::shared_ptr<Piece> &piecePtr) {
    if (piecePtr->getSide() == WHITE) {
        mainWhite = piecePtr;
    } else if (piecePtr->getSide() == BLACK) {
        mainBlack = piecePtr;
    }
}

void RulesController::deletePriorityPiece(Color pieceSide) {
    if (pieceSide == WHITE) {
        mainWhite = nullptr;
    } else if (pieceSide == BLACK) {
        mainBlack = nullptr;
    }
}

bool RulesController::isPriorityPiece(std::shared_ptr<Piece> &piecePtr) {
    return ((piecePtr->getSide() == WHITE && mainWhite->getPos() == piecePtr->getPos()) || 
    (piecePtr->getSide() == BLACK && mainBlack->getPos() == piecePtr->getPos()));
}

bool RulesController::isAttackedPriorityPiece(Board &board, Color pieceSide, MoveLog &moveLog) {
    std::shared_ptr<Piece> mainPiece = (pieceSide == WHITE) ? mainWhite : mainBlack;

    if (mainPiece == nullptr) {
        return false;
    }
 
    for (auto it = board.pieces.begin(); it != board.pieces.end(); ++it) {
        if ((*it)->getSide() != pieceSide && (*it)->isPossibleStep(board, moveLog, mainPiece->getPos())) {
            return true;
        }
    }

    return false;
}

void RulesController::excludeChecksFromPossibleSteps(CellPos pos, std::list<CellPos> &steps, Board &board, MoveLog &moveLog) {
    auto it = steps.begin();
    MoveLog newMoveLog = MoveLog(RECORDING_ON, moveLog.getLastMoveStep(), moveLog.getLastMoveColor(), moveLog.getLastMoveKind());
    Color mainPieceSide = playerTurn;

    while (it != steps.end()) {
        board.getPiecePtr(pos)->move(board, newMoveLog, *this, *it);
        board.deletePiecesFromList();

        if (isAttackedPriorityPiece(board, mainPieceSide, newMoveLog)) {
            it = steps.erase(it); 
        } else {
            ++it;
        }

        newMoveLog.goPreviousRecord(board, *this);
    }

    playerTurn = mainPieceSide;
}

void RulesController::mateAndStalemateProcessing(Board &board, MoveLog &moveLog) {
    MoveLog newMoveLog = MoveLog(RECORDING_ON, moveLog.getLastMoveStep(), moveLog.getLastMoveColor(), moveLog.getLastMoveKind());
    Color mainPieceSide = playerTurn;
    
    std::list<CellPos> firstSteps;
    for (auto it = board.pieces.begin(); it != board.pieces.end(); ++it) {
        firstSteps.push_back((*it)->getPos());
    }
    
    std::list<CellPos> secondSteps;

    for (auto firstPosIt = firstSteps.begin(); firstPosIt != firstSteps.end(); ++firstPosIt) {
        if (board.getPiecePtr(*firstPosIt)->getSide() != mainPieceSide) {
            continue;
        }
        
        board.getPiecePtr(*firstPosIt)->getPossibleSteps(board, newMoveLog, secondSteps);

        for (auto secondPosIt = secondSteps.begin(); secondPosIt != secondSteps.end(); ++secondPosIt) {
            board.getPiecePtr(*firstPosIt)->move(board, newMoveLog, *this, *secondPosIt);
            board.deletePiecesFromList();

            if (!isAttackedPriorityPiece(board, mainPieceSide, newMoveLog)) {
                newMoveLog.goPreviousRecord(board, *this);
                playerTurn = mainPieceSide;
                return;
            }

            //changePlayerTurn();
            newMoveLog.goPreviousRecord(board, *this);
        }

        secondSteps.clear();
    } 
    
    if (isAttackedPriorityPiece(board, mainPieceSide, newMoveLog)) {
        gameStatus = (mainPieceSide == WHITE) ? BLACK_WIN : WHITE_WIN;
    } else {
        gameStatus = STALEMATE;
    }

    playerTurn = mainPieceSide;
}

void RulesController::addReplacementPiece(std::shared_ptr<Piece> &piecePtr) {
    gameStatus = CHANGE_PIECE;
    replacementPiece = piecePtr;
}

void RulesController::addPieceToChoose(std::shared_ptr<Piece> piecePtr) {
    gameStatus = CHANGE_PIECE;
    piecesToChoose.push_back(piecePtr);
}

void RulesController::showPiecesToChoose(std::list<int> &kindPieces) {
    for (auto it = piecesToChoose.begin(); it != piecesToChoose.end(); ++it) {
        kindPieces.push_back((*it)->getKind() * (*it)->getSide());
    }
}

std::shared_ptr<Piece> RulesController::getPieceToChoose(PieceKind kind) {
    for (auto it = piecesToChoose.begin(); it != piecesToChoose.end(); ++it) {
        if ((*it)->getKind() == kind) {
            return *it;
        }
    }
    return nullptr;
}

std::shared_ptr<Piece> RulesController::getReplacementPiece() {
    return replacementPiece;
}

void RulesController::clearPiecesToChoose() {
    gameStatus = IN_GAME;

    piecesToChoose.clear();
    replacementPiece = nullptr;
}
