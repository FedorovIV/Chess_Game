#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "chessSet.hpp"
#include "cellPos.hpp"


class Chess {
    Board board; 
    MoveLog moveLog;
    RulesController rulesController;

    public:
        Chess(int size = 8, LogStatus recordingStatus = RECORDING_ON): board(size), moveLog(recordingStatus) {
            rulesController = RulesController();
            classicStartPosition(rulesController);
        }

        int getSize() {
            return board.size;
        }

        void getPossibleSteps(CellPos pos, std::list<CellPos> &steps);
        Color getPieceSide(CellPos pos);

        void changePlayerTurn() { rulesController.changePlayerTurn(); }
        Color getPlayerTurn() { return rulesController.getPlayerTurn(); }
        GameStatus getGameStatus() { return rulesController.getGameStatus(); }

        bool classicStartPosition(RulesController &rulesController); 

        bool move(CellPos pos1, CellPos pos2);

        void changePiece(PieceKind kind);

        bool isRecordingOn();
        void setRecordingStatus(LogStatus recordingStatus);
        bool nextMove();
        bool previousMove();


        std::vector<int> getBoard();
};
