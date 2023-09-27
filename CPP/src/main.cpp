#include <string>
#include <map>
#include "chessGame.hpp"


int main() {
    ChessGame game = ChessGame();
    bool isGameContinue = true;
    std::string cmd = "", pos = "";
    std::map<std::string, int> mapping;

    mapping["exit"] = 0;
    mapping["next"] = 1;
    mapping["previous"] = 2;
    mapping["show"] = 3;
    mapping["changeRecording"] = 4;
    mapping["choosePiece"] = 5;

    while (isGameContinue) {
        std::cin >> cmd;

        auto it = mapping.find(cmd);

        if (it == mapping.end()) {
            if (!game.choosePiece(cmd) && game.isPieceChosen() && game.makeStep(cmd)) {
                game.show();
            } else {
                game.showPossibleSteps();
            }
            continue;
        }

        switch (mapping.at(cmd)) {
        case 0:
            isGameContinue = false;
            break;
        case 1:
            game.nextMove();
            game.show();
            break;
        case 2:
            game.previousMove();
            game.show();
            break;
        case 3:
            game.show();
            break;
        case 4:
            std::cout << ((game.changeRecordingStatus() == RECORDING_ON) ? "recordingOn" : "recordingOff") << std::endl;
            break;
        case 5:
            int kindNum;
            std::cin >> kindNum;
            game.chooseReplacementPiece(kindNum);
            break;
        default:
            std::cout << "Unknown command" << std::endl; 
            break;
        }
    }

    return 0;
}
