//
// Created by Florian on 27.05.2020.
//

#pragma once

#include "../Core.h"
#include <memory>
#include <freetype/ftglyph.h>
#include "../File.h"

namespace Zoe{

struct FontData;
class Texture;

struct DLL_PUBLIC FontCharMetric{
    long  width;
    long  height;

    long  horiBearingX;
    long  horiBearingY;
    long  horiAdvance;

    long  vertBearingX;
    long  vertBearingY;
    long  vertAdvance;
};

struct DLL_PUBLIC FontCharBitmapInformation{
    unsigned int width;
    unsigned int height;
    unsigned int x;
    unsigned int y;
};

class DLL_PUBLIC Font {
public:
    Font() = delete;
    Font(const File& file, int size);
    Font(const Font& font);
    Font(const Font& font, int size);

    Font& operator=(const Font&);

    ~Font();

    std::shared_ptr<Texture> getBitmap();

    const FontCharMetric& getCharMetric(unsigned long charcode);
    const FontCharBitmapInformation& getCharBitmapInformation(unsigned long charcode);

    const int& getSize();

    const unsigned int &getTextureHeight();
    const unsigned int &getTextureWidth();

    static void init();

private:
    void loadGlyphs();

private:
    FontData* data;

    void loadGlyph(unsigned int glyphIndex, unsigned long charCode, void* glyph, unsigned int &maxWidth,
                   unsigned int &maxHeight, unsigned int &glyphCount) const;
};

}