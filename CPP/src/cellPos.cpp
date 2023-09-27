#include "cellPos.hpp"


CellPos::CellPos(int letterPos, int numberPos) {
    if (letterPos <= 0 || numberPos <= 0) {
        this->letterPos = 0;
        this->numberPos = 0;
    } else {
        this->letterPos = letterPos;
        this->numberPos = numberPos;
    }
}

CellPos::CellPos(const std::string pos) {
    auto numIt = pos.begin();

    if (numIt == pos.end()) {
        letterPos = 0;
        numberPos = 0;
        return;
    }

    while (!std::isdigit(*numIt)) {
        ++numIt;
    }

    if (numIt == pos.begin()) {
        letterPos = 0;
        numberPos = 0;
        return;
    }
    
    // get a letter position
    auto letterIt = numIt;
    unsigned int dec = 1, res = 0;
    do {
        --letterIt;
        if ('a' > *letterIt || *letterIt > 'z') {
            letterPos = 0;
            numberPos = 0;
            return;
        }
        res += (*letterIt - 'a' + 1) * dec;
        dec *= 26;
    } while (letterIt != pos.begin());

    letterPos = res;

    // get a number position
    for (auto it = numIt; it != pos.end(); ++it) {
        if (!std::isdigit(*it)) {
            letterPos = 0;
            numberPos = 0;
            return;
        }
    }

    std::string number(numIt, pos.end());
    std::stringstream intStream(number);
    intStream >> numberPos;
}

bool CellPos::isRealPos() {
    return letterPos != 0 && numberPos != 0;
}

bool CellPos::isInSquareBoard(unsigned short size) {
    return (0 < letterPos && letterPos <= size && 0 < numberPos && numberPos <= size);
}

int CellPos::getArrayPos(unsigned short size) {
    return (letterPos - 1) * size + (numberPos - 1);
}

void CellPos::setPosition(int letterPos, int numberPos) {
    if (letterPos <= 0 || numberPos <= 0) {
        this->letterPos = 0;
        this->numberPos = 0;
    } else {
        this->letterPos = letterPos;
        this->numberPos = numberPos;
    }
}

bool CellPos::movePosition(int letterMove, int numberMove, unsigned short size) {
    int letterRes = letterPos + letterMove, numberRes = numberPos + numberMove;
    if (0 < letterRes && letterRes <= size && 0 < numberRes && numberRes <= size) {
        letterPos = letterRes;
        numberPos = numberRes;
        return 1;
    }

    return 0;
}

CellPos& CellPos::operator=(const CellPos &other) {
    if (this == &other) {
        return *this;
    }

    this->letterPos = other.letterPos;
    this->numberPos = other.numberPos;

    return *this;
}

bool CellPos::operator==(const CellPos &other) {
    return (this->letterPos == other.letterPos && this->numberPos == other.numberPos);
}

bool CellPos::operator!=(const CellPos &other) {
    return (this->letterPos != other.letterPos || this->numberPos != other.numberPos);
}
