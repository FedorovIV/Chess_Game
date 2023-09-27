#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <string>


class CellPos {
    unsigned short letterPos;
    unsigned short numberPos;

    public:
        CellPos(): letterPos(0), numberPos(0) {}
        CellPos(int letterPos, int numberPos);
        CellPos(const std::string pos);

        bool isRealPos();

        bool isInSquareBoard(unsigned short size = 8);
        int getArrayPos(unsigned short size = 8);

        unsigned short getLetter() { return letterPos; }
        unsigned short getNumber() { return numberPos; }

        void setPosition(int letterPos, int numberPos);
        bool movePosition(int letterMove, int numberMove, unsigned short size = 8);

        CellPos& operator=(const CellPos &other);
        bool operator==(const CellPos &other);
        bool operator!=(const CellPos &other);
};
