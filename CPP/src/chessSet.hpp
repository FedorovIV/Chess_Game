#pragma once

#include <vector>
#include <list>
#include <memory>
#include <cmath>
#include "cellPos.hpp"


enum Color { NO_COLOR = 0, WHITE = 1, BLACK = -1 };

enum PieceKind { NONE = 0, KING = 1, QUEEN, BISHOP, ROOK, KNIGHT, PAWN };

enum LogStatus { RECORDING_ON = 0, RECORDING_OFF };

enum PieceMoveStatus { REMOVAL = 0, ADDITION, REMOVAL_PRIORITY, ADDING_PRIORITY };

enum GameStatus { IN_GAME = 0, WHITE_WIN, BLACK_WIN, STALEMATE, CHANGE_PIECE };

class Piece;
class RulesController;

using piecesList = std::list<std::shared_ptr<Piece>>;
using pieceMoving = std::pair<std::shared_ptr<Piece>, PieceMoveStatus>;
using step = std::pair<CellPos, CellPos>;

// The position of the pieces on the chessboard

class Board {
    std::list<piecesList::iterator> toDeletePieces;

    public:
        const int size;
        piecesList pieces;
        std::vector<piecesList::iterator> field;

        Board(int size);

        void setPiece(std::shared_ptr<Piece> newPiece);

        std::shared_ptr<Piece> &getPiecePtr(CellPos pos);
        std::shared_ptr<Piece> &getPiecePtr(int index);

        bool isCellEmpty(CellPos pos);
        bool isCellEmpty(int index);

        void deletePiece(CellPos pos);

        // safety way to delete piece elements
        void putInDeleteList(CellPos pos);
        void deletePiecesFromList();
        void clearDeleteList();

        void replacePiece(CellPos fromPos, CellPos toPos);

        void clear();
};

// Record of moves in a chess game

struct SavedMoving_t {
    step mainPieceStep;
    Color mainPieceSide;
    PieceKind mainPieceKind;

    std::list<pieceMoving> piecesMove;

    SavedMoving_t(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind) {
        this->mainPieceStep = mainPieceStep;
        this->mainPieceSide = mainPieceSide;
        this->mainPieceKind = mainPieceKind;
    }
};

class MoveLog {
    LogStatus logStatus;

    std::list<SavedMoving_t> log;
    std::list<SavedMoving_t>::iterator lastState;

    public:
        MoveLog(LogStatus logStatus): logStatus(logStatus) {
            log.assign(1, SavedMoving_t({CellPos(), CellPos()}, NO_COLOR, NONE));
            lastState = log.begin();
        }

        MoveLog(LogStatus logStatus, step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind): logStatus(logStatus) {
            log.assign(1, SavedMoving_t(mainPieceStep, mainPieceSide, mainPieceKind));
            lastState = log.begin();
        }

        bool isLogRecordOn() { return logStatus == RECORDING_ON; }
        void turnRecordingOn();
        void turnRecordingOff();

        void createNewRecord(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind);
        void addNewMoveRecord(std::shared_ptr<Piece> &piecePtr, PieceMoveStatus status);

        bool goNextRecord(Board &board, RulesController &rulesController);
        bool goPreviousRecord(Board &board, RulesController &rulesController);

        step getLastMoveStep();
        Color getLastMoveColor();
        PieceKind getLastMoveKind();
};

// Controls the implementation of the rules of the game

class RulesController {
    GameStatus gameStatus;
    Color playerTurn;

    std::shared_ptr<Piece> replacementPiece;
    std::list<std::shared_ptr<Piece>> piecesToChoose;

    std::shared_ptr<Piece> mainWhite;
    std::shared_ptr<Piece> mainBlack;

    public:
        RulesController(): gameStatus(IN_GAME), playerTurn(WHITE) {
            mainWhite = nullptr;
            mainBlack = nullptr;
            replacementPiece = nullptr;
        }  

        GameStatus getGameStatus();

        void setPriorityPiece(std::shared_ptr<Piece> &piecePtr);
        void deletePriorityPiece(Color pieceSide);
        bool isPriorityPiece(std::shared_ptr<Piece> &piecePtr);

        bool isAttackedPriorityPiece(Board &board, Color pieceSide, MoveLog &moveLog);
        void excludeChecksFromPossibleSteps(CellPos pos, std::list<CellPos> &steps, Board &board, MoveLog &moveLog);
        void mateAndStalemateProcessing(Board &board, MoveLog &moveLog);
        
        void addReplacementPiece(std::shared_ptr<Piece> &piecePtr);
        void addPieceToChoose(std::shared_ptr<Piece> piecePtr);
        void showPiecesToChoose(std::list<int> &piecesToChoose);
        std::shared_ptr<Piece> getPieceToChoose(PieceKind kind);
        std::shared_ptr<Piece> getReplacementPiece();
        void clearPiecesToChoose();

        void changePlayerTurn() { playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR); }
        Color getPlayerTurn() { return playerTurn; }
};

// Data and structure of movement of chess pieces

class Piece {
    protected:
        CellPos position;
        Color pieceSide;
        PieceKind kind;
        bool isFirstStep;

    public:
        Piece(CellPos position, Color color) {
            this->position = position;
            pieceSide = color;
            kind = NONE;
            isFirstStep = true;
        }

        PieceKind getKind() { return this->kind; }
        Color getSide() { return this->pieceSide; }
        CellPos getPos() { return this->position; }
        bool getIsFirstStep() { return this->isFirstStep; }

        void setIsFirstStep(bool val) { this->isFirstStep = val; }
        void setPos(CellPos position) { this->position = position; }

        virtual void move(Board &board, MoveLog &moveLog, RulesController &rules, CellPos newPos);
        virtual void replace(Board &board, MoveLog &moveLog, RulesController &rules, PieceKind kind);
        virtual void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        virtual bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos);

        virtual std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Piece>(*this);
        }
};

class King: public Piece {
    public:
        King(CellPos position, Color color) : Piece(position, color) {
            this->kind = KING;
        }

        void move(Board &board, MoveLog &moveLog, RulesController &rules, CellPos newPos);
        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos);

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<King>(*this);
        }
};

class Queen: public Piece {
    protected:
        void getLinearPosSteps(std::pair<int, int> dir[4], Board &board, std::list<CellPos> &steps);
        bool isLinearPosSteps(std::pair<int, int> dir[4], Board &board, CellPos otherPos);

    public:
        Queen(CellPos position, Color color) : Piece(position, color) {
            this->kind = QUEEN;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Queen>(*this);
        }
};

class Bishop: public Queen {
    public:
        Bishop(CellPos position, Color color) : Queen(position, color) {
            this->kind = BISHOP;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Bishop>(*this);
        }
};

class Rook: public Queen { 
    public:
        Rook(CellPos position, Color color) : Queen(position, color) {
            this->kind = ROOK;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Rook>(*this);
        }
};

class Knight: public Piece {
    public:
        Knight(CellPos position, Color color) : Piece(position, color) {
            this->kind = KNIGHT;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Knight>(*this);
        }
};

class Pawn: public Piece {
    enum Direct { UP, DOWN };

    Direct direction;

    int getDirEnPassent(MoveLog &moveLog, CellPos currentPos);

    public:
        Pawn(CellPos position, Color color, bool isFirstStep) : Piece(position, color) {
            this->kind = PAWN;
            this->isFirstStep = isFirstStep;
            this->direction = (this->pieceSide == WHITE) ? UP : DOWN;
        }

        void move(Board &board, MoveLog &moveLog, RulesController &rules, CellPos newPos);
        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 
        
        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Pawn>(*this);
        }
};
