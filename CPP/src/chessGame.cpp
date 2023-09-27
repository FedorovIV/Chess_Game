#include "chessGame.hpp"
#include <cmath>


ChessGame::ChessGame(): chess(), currentPos() { }

bool ChessGame::isPossibleStep(CellPos pos) {
    for (auto posIt = possibleSteps.begin(); posIt != possibleSteps.end(); ++posIt) {
        if (*posIt == pos) {
            return true;
        }
    }

    return false;
}

void ChessGame::clearChoosenPiece() {
    currentPos.setPosition(0, 0);
    possibleSteps.clear();
}

bool ChessGame::makeStep(std::string str) {
    CellPos pos = CellPos(str);

    if (isPossibleStep(pos) && chess.move(currentPos, pos)) {
        clearChoosenPiece();
        return true;
    }

    clearChoosenPiece();
    return false;
}

bool ChessGame::isPieceChosen() {
    return currentPos.isRealPos();
}

bool ChessGame::choosePiece(std::string str) {
    CellPos pos = CellPos(str);

    if (!pos.isInSquareBoard(chess.getSize())) {
        clearChoosenPiece(); 
        return false;
    }

    if (!isPossibleStep(pos) && chess.getPieceSide(pos) == chess.getPlayerTurn()) {
        if (currentPos != pos) {
            clearChoosenPiece(); 
            chess.getPossibleSteps(pos, possibleSteps);
            currentPos = pos;
        }
        return true;
    }

    return false;
}

void ChessGame::showPossibleSteps() {
    std::cout << "PossibleSteps " << possibleSteps.size() << std::endl;

    for (auto it = possibleSteps.begin(); it != possibleSteps.end(); ++it) {
        std::cout << char('a' + it->getLetter() - 1) << it->getNumber() << " ";
    }
    std::cout << std::endl;
}

void ChessGame::chooseReplacementPiece(int kindNum) {
    PieceKind kind = NONE;

    switch (kindNum) {
    case 2:
        kind = QUEEN;
        break;
    case 3:
        kind = BISHOP;
        break;
    case 4:
        kind = ROOK;
        break;
    default:
        kind = KNIGHT;
        break;
    }
    
    chess.changePiece(kind);
}

LogStatus ChessGame::changeRecordingStatus() {
    LogStatus newStatus = chess.isRecordingOn() ? RECORDING_OFF : RECORDING_ON;
    chess.setRecordingStatus(newStatus);
    return newStatus;
}

void ChessGame::nextMove() {
    clearChoosenPiece();
    chess.nextMove();
}

void ChessGame::previousMove() {
    clearChoosenPiece();
    chess.previousMove();
}

void ChessGame::show() {
    std::vector<int> fullBoard = chess.getBoard();

    std::cout << "Board " << ((chess.getPlayerTurn() == WHITE) ? "WHITE" : "BLACK") << std::endl;

    for (int i = 0; i < chess.getSize(); ++i) {
        for (int j = 0; j < chess.getSize(); ++j) {
            std::cout << std::setfill(' ') << std::setw(2) << fullBoard[j * chess.getSize() + i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << ".Status ";

    switch (chess.getGameStatus())
    {
    case WHITE_WIN:
        std::cout << "WHITE_WIN" << std::endl;
        break;
    case BLACK_WIN:
        std::cout << "BLACK_WIN" << std::endl;
        break;
    case STALEMATE:
        std::cout << "STALEMATE" << std::endl;
        break;
    case CHANGE_PIECE:
        std::cout << "CHANGE_PIECE" << std::endl;
        break;
    default:
        std::cout << "IN_GAME" << std::endl;
        break;
    }
}
