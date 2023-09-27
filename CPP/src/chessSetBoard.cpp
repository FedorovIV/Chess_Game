#include "chessSet.hpp"


Board::Board(int size): size(size) {
    field.assign(size * size, pieces.end());
}

void Board::setPiece(std::shared_ptr<Piece> newPiece) {
    CellPos pos = newPiece->getPos();

    if (!pos.isInSquareBoard(size)) {
        return;
    }

    pieces.push_back(newPiece);
    field.at(pos.getArrayPos(size)) = --pieces.end();
}

void Board::deletePiece(CellPos pos) {
    if (isCellEmpty(pos)) {
        return;
    }

    pieces.erase(field.at(pos.getArrayPos(size)));
    field.at(pos.getArrayPos(size)) = pieces.end();
}

void Board::putInDeleteList(CellPos pos) {
    if (isCellEmpty(pos)) {
        return;
    }

    toDeletePieces.push_back(field.at(pos.getArrayPos(size)));
    field.at(pos.getArrayPos(size)) = pieces.end();
}

void Board::deletePiecesFromList() {
    for (auto it = toDeletePieces.begin(); it != toDeletePieces.end(); ++it) {
        pieces.erase(*it);
    }
    toDeletePieces.clear();
}

void Board::clearDeleteList() {
    toDeletePieces.clear();
}

std::shared_ptr<Piece> &Board::getPiecePtr(CellPos pos) {
    return *field.at(pos.getArrayPos(size));
}

std::shared_ptr<Piece> &Board::getPiecePtr(int index) {
    return *field.at(index);
}

bool Board::isCellEmpty(CellPos pos) {
    return field.at(pos.getArrayPos(size)) == pieces.end();
}

bool Board::isCellEmpty(int index) {
    return field.at(index) == pieces.end();
}

void Board::replacePiece(CellPos fromPos, CellPos toPos) {
    field.at(toPos.getArrayPos(size)) = field.at(fromPos.getArrayPos(size));
    field.at(fromPos.getArrayPos(size)) = pieces.end();
}

void Board::clear() {
    field.assign(size * size, pieces.end());
    pieces.clear();
}
