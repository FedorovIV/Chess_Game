#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "cellPos.hpp"
#include "chess.hpp"


class ChessGame {
    Chess chess;

    CellPos currentPos;
    std::list<CellPos> possibleSteps; 

    bool isPossibleStep(CellPos pos);

    public:
        ChessGame();

        void clearChoosenPiece();

        bool makeStep(std::string str);

        bool isPieceChosen();

        bool choosePiece(std::string str);

        void chooseReplacementPiece(int kindNum);

        LogStatus changeRecordingStatus();
        void nextMove();
        void previousMove();

        void showPossibleSteps();
        void show();
};
