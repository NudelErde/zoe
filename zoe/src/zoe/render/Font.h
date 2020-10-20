//
// Created by Florian on 27.05.2020.
//

#pragma once

#include <memory>
#include "../core/Core.h"
#include "../core/File.h"

namespace Zoe{

struct FontData;
class Texture;

/**
 * FontCharMetric contains the information on how to draw a specified char.
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 * @see https://www.freetype.org/freetype2/docs/tutorial/metrics.png
 * @see https://www.freetype.org/freetype2/docs/tutorial/metrics2.png
 */
struct DLL_PUBLIC FontCharMetric{
    /**
     * This is the width of the glyph image's bounding box. It is independent of the layout direction.
     */
    long  width;
    /**
     * This is the height of the glyph image's bounding box. It is independent of the layout direction.
     */
    long  height;

    /**
     * For horizontal text layouts, this is the horizontal distance from the current cursor position to the leftmost border of the glyph image's bounding box.
     */
    long  horiBearingX;

    /**
     * For horizontal text layouts, this is the vertical distance from the current cursor position (on the baseline) to the topmost border of the glyph image's bounding box.
     */
    long  horiBearingY;

    /**
     * For horizontal text layouts, this is the horizontal distance to increment the pen position when the glyph is drawn as part of a string of text.
     */
    long  horiAdvance;

    /**
     * For vertical text layouts, this is the horizontal distance from the current cursor position to the leftmost border of the glyph image's bounding box.
     */
    long  vertBearingX;

    /**
     * For vertical text layouts, this is the vertical distance from the current cursor position (on the baseline) to the topmost border of the glyph image's bounding box.
     */
    long  vertBearingY;

    /**
     * For vertical text layouts, this is the vertical distance used to increment the pen position when the glyph is drawn as part of a string of text.
     */
    long  vertAdvance;
};

/**
 * Contains the information about the position and size of the char on the generated bitmap.
 */
struct DLL_PUBLIC FontCharBitmapInformation{
    /**
     * Width on the bitmap.
     */
    unsigned int width;
    /**
     * Height on the bitmap.
     */
    unsigned int height;
    /**
     * X-Coordinate of the top left corner.
     */
    unsigned int x;
    /**
     * Y-Coordinate of the top left corner.
     */
    unsigned int y;
};

/**
 * A Font provides the information to render text in a visable way.
 */
class DLL_PUBLIC Font {
public:
    /**
     * Fonts can not be empty so the default constructor is deleted.
     */
    Font() = delete;

    /**
     * Create a font with a specified file .ttf file and a specified size.
     * @param file the specified file
     * @param size the specified size
     */
    Font(const File& file, int size);

    /**
     * Copies the specified font.
     * @param font the specified font
     */
    Font(const Font& font);

    /**
     * Copies the specified font but changes the font size.
     * @param font the specified font
     * @param size the new size
     */
    Font(const Font& font, int size);

    /**
     * Copies the specified font.
     * @return a reference to this
     */
    Font& operator=(const Font&);

    /**
     * Destructs the font and releases the used resources.
     */
    ~Font();

    /**
     * Returns the generated bitmap.
     * @return the bitmap
     */
    std::shared_ptr<Texture> getBitmap();

    /**
     * Get the FontCharMetric for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharMetric
     */
    const FontCharMetric& getCharMetric(unsigned long charcode);

    /**
     * Get the FontCharBitmapInformation for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharBitmapInformation
     */
    const FontCharBitmapInformation& getCharBitmapInformation(unsigned long charcode);

    /**
     * Return the fontsize.
     * @return the fontsize
     */
    const int& getSize();

    /**
     * Get the height of the bitmap texture.
     * @return the height of the bitmap texture
     */
    const unsigned int &getTextureHeight();

    /**
     * Get the width of the bitmap texture.
     * @return the width of the bitmap texture
     */
    const unsigned int &getTextureWidth();

    /**
     * Initializes the font loading library. (FreeType2)
     */
    static void init();

private:
    void loadGlyphs();

private:
    FontData* data;

    void loadGlyph(unsigned int glyphIndex, unsigned long charCode, void* glyph, unsigned int &maxWidth,
                   unsigned int &maxHeight, unsigned int &glyphCount) const;
};

}