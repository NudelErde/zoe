//
// Created by Florian on 24.02.2021.
//

#include "Tetromino.h"
#include <string>
#include <sstream>
#include <cinttypes>
#include "zoe.h"

using namespace Zoe;

static uint8_t iPieceData[] = {
        0b0000,
        0b1111,
        0b0000,
        0b0000,

        0b0010,
        0b0010,
        0b0010,
        0b0010,

        0b0000,
        0b0000,
        0b1111,
        0b0000,

        0b0100,
        0b0100,
        0b0100,
        0b0100
};

static uint8_t jPieceData[] = {
        0b1000,
        0b1110,
        0b0000,
        0b0000,

        0b0110,
        0b0100,
        0b0100,
        0b0000,

        0b0000,
        0b1110,
        0b0010,
        0b0000,

        0b0100,
        0b0100,
        0b1100,
        0b0000
};

static uint8_t lPieceData[] = {
        0b0010,
        0b1110,
        0b0000,
        0b0000,

        0b0100,
        0b0100,
        0b0110,
        0b0000,

        0b0000,
        0b1110,
        0b1000,
        0b0000,

        0b1100,
        0b0100,
        0b0100,
        0b0000
};

static uint8_t oPieceData[] = {
        0b0110,
        0b0110,
        0b0000,
        0b0000,

        0b0110,
        0b0110,
        0b0000,
        0b0000,

        0b0110,
        0b0110,
        0b0000,
        0b0000,

        0b0110,
        0b0110,
        0b0000,
        0b0000
};

static uint8_t sPieceData[] = {
        0b0110,
        0b1100,
        0b0000,
        0b0000,

        0b0100,
        0b0110,
        0b0010,
        0b0000,

        0b0000,
        0b0110,
        0b1100,
        0b0000,

        0b1000,
        0b1100,
        0b0100,
        0b0000
};

static uint8_t tPieceData[] = {
        0b0100,
        0b1110,
        0b0000,
        0b0000,

        0b0100,
        0b0110,
        0b0100,
        0b0000,

        0b0000,
        0b1110,
        0b0100,
        0b0000,

        0b0100,
        0b1100,
        0b0100,
        0b0000
};

static uint8_t zPieceData[] = {
        0b1100,
        0b0110,
        0b0000,
        0b0000,

        0b0010,
        0b0110,
        0b0100,
        0b0000,

        0b0000,
        0b1100,
        0b0110,
        0b0000,

        0b0100,
        0b1100,
        0b1000,
        0b0000
};

static std::string buildTetrominoModeString(uint8_t modeNumber, const uint8_t data[4], const vec3& color) {
    std::stringstream ss;
    std::string modeStr = std::to_string(modeNumber);

    ss << "<Sprite visible=\"";
    ss << (modeNumber > 0 ? "false" : "true");
    ss << "\" id=\"mode";
    ss << modeStr;
    ss << "\">";

    for (uint8_t y = 0; y < 4; ++y) {
        uint8_t line = data[y];
        for (uint8_t x = 0; x < 4; ++x) {
            if ((line >> x) & 0b1u) {
                ss << R"(<Rectangle x=")";
                ss << std::to_string(x * 50);
                ss << R"(" y=")";
                ss << std::to_string(y * 50);
                ss << R"(" width="50" height="50" r=")";
                ss << color.x;
                ss << R"(" g=")";
                ss << color.y;
                ss << R"(" b=")";
                ss << color.z;
                ss << R"("/>)";
            }
        }
    }

    ss << R"(</Sprite>)";

    return ss.str();
}

static std::string buildTetrominoString(const uint8_t data[16], const vec3& color) {
    std::stringstream ss;
    ss << R"(<Sprite x="0" y="0" visible="false"> <NativeScriptComponent script="Script" id="playerTetrisScript"/>)";

    for (uint8_t i = 0; i < 4; ++i) {
        ss << buildTetrominoModeString(i, data + 4 * i, color);
    }

    ss << R"(</Sprite>)";

    return ss.str();
}

static std::string iString = buildTetrominoString(iPieceData, vec3(0, 1, 1));
static std::string jString = buildTetrominoString(jPieceData, vec3(0, 0, 1));
static std::string lString = buildTetrominoString(lPieceData, vec3(1, 0.64, 0));
static std::string oString = buildTetrominoString(oPieceData, vec3(1, 1, 0));
static std::string sString = buildTetrominoString(sPieceData, vec3(0, 1, 0));
static std::string tString = buildTetrominoString(tPieceData, vec3(1, 0, 1));
static std::string zString = buildTetrominoString(zPieceData, vec3(1, 0, 0));

const std::string& Tetromino::getIString() {
    return iString;
}
const std::string& Tetromino::getJString() {
    return jString;
}
const std::string& Tetromino::getLString() {
    return lString;
}
const std::string& Tetromino::getOString() {
    return oString;
}
const std::string& Tetromino::getSString() {
    return sString;
}
const std::string& Tetromino::getTString() {
    return tString;
}
const std::string& Tetromino::getZString() {
    return zString;
}
const std::string& Tetromino::getIndexString(const int& index) {
    switch (index) {
        case 0:
            return getIString();
        case 1:
            return getJString();
        case 2:
            return getLString();
        case 3:
            return getOString();
        case 4:
            return getSString();
        case 5:
            return getTString();
        case 6:
            return getZString();
        default:
            return getIString();
    }
}
