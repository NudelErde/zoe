//
// Created by Florian on 27.05.2020.
//

#include "Font.h"
#include "../core/Console.h"
#include "api/Texture.h"
#include "GraphicsContext.h"
#include "../core/Application.h"

#include <map>
#include <cstring>

#include <cmath>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace Zoe {

static bool fontHasInit = false;

struct GlyphData {
    FT_BitmapGlyph glyph; //Only valid in loadGlyphs()
    FontCharMetric metrics;
    //unsigned int textureIndex;
    FontCharBitmapInformation charBitmapInformation;
};

struct FontData {
    FT_Face face{};
    unsigned char *source{};
    size_t sourceSize{};
    unsigned int *sourceReferences{};
    int size{};
    std::map<unsigned long, GlyphData> glyphs;
    unsigned char *textureBuffer{};
    unsigned long long textureWidth{};
    unsigned long long textureHeight{};
    std::shared_ptr<Texture> texture;
};

static FT_Library ftLibrary;

static std::vector<std::shared_ptr<FontHolder>> fontCache;

void FontHolder::init() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("Could not init FreeType library");
    } else {
        debug("Init FreeType library");
    }
    fontHasInit = true;
}

FontHolder::FontHolder(const File &file, int size) : file(file) {
    data = new FontData();


    size_t sourceSize = 0;
    std::unique_ptr<uint8_t[]> source = file.getContent(&sourceSize);

    data->sourceSize = sourceSize;
    data->source = new unsigned char[data->sourceSize];
    memcpy(data->source, source.get(), data->sourceSize);

    //FT_Open_Args args {};
    //args.flags = FT_OPEN_MEMORY;
    //args.memory_size = (FT_Long)fileSize;
    //args.memory_base = data->source;
    //err = FT_Open_Face(ftLibrary, &args, 0, &(data->face)); //TODO: read font with stream
    int err = FT_New_Memory_Face(ftLibrary, data->source, (FT_Long) data->sourceSize, 0, &(data->face));
    if (err) {
        throw std::runtime_error("Could not read font " + file.getPath() + ". Error code: " + std::to_string(err));
    }

    data->sourceReferences = new unsigned int;
    *data->sourceReferences = 1;

    FT_Set_Pixel_Sizes(data->face, 0, size);
    data->size = size;

    loadGlyphs();
}

FontHolder::FontHolder(const FontHolder &font, int size) : file(font.file) {
    data = new FontData();

    data->sourceReferences = font.data->sourceReferences;
    ++(*data->sourceReferences);

    data->source = font.data->source;
    data->sourceSize = font.data->sourceSize;

    //TODO: read font with stream
    int err = FT_New_Memory_Face(ftLibrary, data->source, (FT_Long) data->sourceSize, 0, &(data->face));
    if (err) {
        throw std::runtime_error("Could not copy font. Error Code: " + std::to_string(err));
    }

    data->size = size;
    FT_Set_Pixel_Sizes(data->face, 0, data->size);

    loadGlyphs();
}

FontHolder::~FontHolder() {
    --(data->sourceReferences);

    FT_Done_Face(data->face);

    if (data->sourceReferences == nullptr) {
        delete data->sourceReferences;
        delete[] data->source;
    }

    delete data;
}

void FontHolder::loadGlyphs() {
    unsigned int glyphIndex = 0;
    unsigned long charCode = 0;
    FT_Glyph glyph;

    unsigned int maxWidth = 0;
    unsigned int maxHeight = 0;
    unsigned int glyphCount = 0;

    loadGlyph(0, 0, &glyph, maxWidth, maxHeight, glyphCount); //Load 'missing glyph'
    for (charCode = FT_Get_First_Char(data->face, &glyphIndex);
         glyphIndex != 0;
         charCode = FT_Get_Next_Char(data->face, charCode, &glyphIndex)) {

        loadGlyph(glyphIndex, charCode, &glyph, maxWidth, maxHeight, glyphCount);
    }


    int sideLength = (int) std::ceil(std::sqrt(glyphCount));
    data->textureWidth = sideLength * maxWidth;
    data->textureHeight = sideLength * maxHeight;
    data->textureBuffer = new unsigned char[data->textureWidth * data->textureHeight];
    memset(data->textureBuffer, 0, data->textureWidth * data->textureHeight);
    unsigned int index = 0;
    for (auto &elem: data->glyphs) {
        const unsigned int rows = elem.second.glyph->bitmap.rows;
        elem.second.charBitmapInformation.width = elem.second.glyph->bitmap.width;
        elem.second.charBitmapInformation.height = elem.second.glyph->bitmap.rows;
        //elem.second.charBitmapInformation.textureIndex = index;
        elem.second.charBitmapInformation.x = (index % sideLength) * maxWidth;
        elem.second.charBitmapInformation.y = (index / sideLength) * maxHeight;
        for (unsigned int row = 0; row < rows; ++row) {
            std::memcpy(data->textureBuffer + (elem.second.charBitmapInformation.x +
                                               (elem.second.charBitmapInformation.y + row) * data->textureWidth),
                        (elem.second.glyph->bitmap.buffer + row * elem.second.charBitmapInformation.width),
                        elem.second.charBitmapInformation.width);
        }
        ++index;
    }

    data->texture = Application::getContext().getTexture(data->textureWidth, data->textureHeight, 1);
    data->texture->setData(data->textureBuffer, data->textureWidth * data->textureHeight);

    for (auto &elem : data->glyphs) {
        FT_Done_Glyph((FT_Glyph) elem.second.glyph);
    }
}

void FontHolder::loadGlyph(unsigned int glyphIndex, unsigned long charCode, void *glyph_ptr, unsigned int &maxWidth,
                           unsigned int &maxHeight, unsigned int &glyphCount) const {
    FT_Glyph &glyph = *(FT_Glyph *) glyph_ptr;
    int err;
    err = FT_Load_Glyph(data->face, glyphIndex, FT_LOAD_DEFAULT);
    if (err) {
        throw std::runtime_error(
                "Could not load Glyph: " + std::to_string(glyphIndex) + " CharCode: " + std::to_string(charCode) +
                " Error: " + std::to_string(err));
    }
    err = FT_Get_Glyph(data->face->glyph, &glyph);
    if (err) {
        throw std::runtime_error(
                "Could not get Glyph: " + std::to_string(glyphIndex) + " CharCode: " + std::to_string(charCode) +
                " Error: " + std::to_string(err));
    }

    if (glyph->format == FT_GLYPH_FORMAT_BITMAP) {
        data->glyphs[charCode].glyph = (FT_BitmapGlyph) glyph;
    } else {
        err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, 1);
        if (err) {
            warning("Could not convert glyph: ", glyphIndex, " to bitmap");
        } else {
            data->glyphs[charCode].glyph = (FT_BitmapGlyph) glyph;
        }
    }
    data->glyphs[charCode].metrics = *(FontCharMetric *) (void *) &(data->face->glyph->metrics);
    const unsigned int width = data->glyphs[charCode].glyph->bitmap.width;
    const unsigned int height = data->glyphs[charCode].glyph->bitmap.rows;
    if (width > maxWidth) {
        maxWidth = width;
    }
    if (height > maxHeight) {
        maxHeight = height;
    }
    ++glyphCount;
}

std::shared_ptr<Texture> FontHolder::getBitmap() {
    return data->texture;
}

const FontCharMetric &FontHolder::getCharMetric(unsigned long charcode) {
    if (data->glyphs.find(charcode) == data->glyphs.end())
        return data->glyphs[0].metrics;
    return data->glyphs[charcode].metrics;
}

const FontCharBitmapInformation &FontHolder::getCharBitmapInformation(unsigned long charcode) {
    if (data->glyphs.find(charcode) == data->glyphs.end())
        return data->glyphs[0].charBitmapInformation;
    return data->glyphs[charcode].charBitmapInformation;
}

const int &FontHolder::getSize() {
    return data->size;
}

unsigned int FontHolder::getTextureWidth() {
    return data->textureWidth;
}

unsigned int FontHolder::getTextureHeight() {
    return data->textureHeight;
}

const File &FontHolder::getFile() {
    return file;
}

static std::shared_ptr<FontHolder> getFontHolder(const File &file, int size) {
    for (const auto &fontHolder : fontCache) {
        if (fontHolder->getSize() == size && fontHolder->getFile() == file) {
            return fontHolder;
        }
    }
    auto val = std::make_shared<FontHolder>(file, size);
    fontCache.push_back(val);
    return val;
}

Font::Font() noexcept {
    if (!fontHasInit) {
        fontHolder = nullptr;
    } else {
        fontHolder = getFontHolder(File("virtual/zoe/display/Roboto-Regular.ttf"), 30);
    }
}

Font::Font(const File &file) {
    fontHolder = getFontHolder(file, 30);
}

Font::Font(const File &file, int size) {
    fontHolder = getFontHolder(file, size);
}

void Font::setSize(int size) {
    fontHolder = getFontHolder(fontHolder->getFile(), size);
}

std::shared_ptr<Texture> Font::getBitmap() {
    return fontHolder->getBitmap();
}

const FontCharMetric &Font::getCharMetric(unsigned long charcode) {
    return fontHolder->getCharMetric(charcode);
}

const FontCharBitmapInformation &Font::getCharBitmapInformation(unsigned long charcode) {
    return fontHolder->getCharBitmapInformation(charcode);
}

const int &Font::getSize() {
    return fontHolder->getSize();
}

unsigned int Font::getTextureHeight() {
    return fontHolder->getTextureHeight();
}

unsigned int Font::getTextureWidth() {
    return fontHolder->getTextureWidth();
}
}