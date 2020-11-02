//
// Created by Florian on 27.05.2020.
//

#pragma once

#include <memory>
#include "../core/Core.h"
#include "../core/File.h"

namespace Zoe {

struct FontData;

class Texture;

/**
 * FontCharMetric contains the information on how to draw a specified char.
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 * @see https://www.freetype.org/freetype2/docs/tutorial/metrics.png
 * @see https://www.freetype.org/freetype2/docs/tutorial/metrics2.png
 */
struct DLL_PUBLIC FontCharMetric {
    /**
     * This is the width of the glyph image's bounding box. It is independent of the layout direction.
     */
    long width;
    /**
     * This is the height of the glyph image's bounding box. It is independent of the layout direction.
     */
    long height;

    /**
     * For horizontal text layouts, this is the horizontal distance from the current cursor position to the leftmost border of the glyph image's bounding box.
     */
    long horiBearingX;

    /**
     * For horizontal text layouts, this is the vertical distance from the current cursor position (on the baseline) to the topmost border of the glyph image's bounding box.
     */
    long horiBearingY;

    /**
     * For horizontal text layouts, this is the horizontal distance to increment the pen position when the glyph is drawn as part of a string of text.
     */
    long horiAdvance;

    /**
     * For vertical text layouts, this is the horizontal distance from the current cursor position to the leftmost border of the glyph image's bounding box.
     */
    long vertBearingX;

    /**
     * For vertical text layouts, this is the vertical distance from the current cursor position (on the baseline) to the topmost border of the glyph image's bounding box.
     */
    long vertBearingY;

    /**
     * For vertical text layouts, this is the vertical distance used to increment the pen position when the glyph is drawn as part of a string of text.
     */
    long vertAdvance;
};

/**
 * Contains the information about the position and size of the char on the generated bitmap.
 */
struct DLL_PUBLIC FontCharBitmapInformation {
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
 * A FontHolder holds the font data and provides the information to render text in a visible way.
 */
class DLL_PUBLIC FontHolder {
public:
    /**
     * Fonts can not be empty so the default constructor is deleted.
     */
    FontHolder() = delete;

    /**
     * Create a font with a specified file .ttf file and a specified size.
     * @param file the specified file
     * @param size the specified size
     */
    FontHolder(const File &file, int size);

    /**
     * FontHolder should not be copied.
     */
    FontHolder(const FontHolder &) = delete;

    /**
     * Copies the specified font but changes the font size.
     * @param font the specified font
     * @param size the new size
     */
    FontHolder(const FontHolder &font, int size);

    /**
     * FontHolder should not be copied.
     * @return a reference to this
     */
    FontHolder &operator=(const FontHolder &) = delete;

    /**
     * Destructs the font and releases the used resources.
     */
    ~FontHolder();

    /**
     * Returns the generated bitmap.
     * @return the bitmap
     */
    [[nodiscard]] std::shared_ptr<Texture> getBitmap();

    /**
     * Get the FontCharMetric for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharMetric
     */
    [[nodiscard]] const FontCharMetric &getCharMetric(unsigned long charcode);

    /**
     * Get the FontCharBitmapInformation for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharBitmapInformation
     */
    [[nodiscard]] const FontCharBitmapInformation &getCharBitmapInformation(unsigned long charcode);

    /**
     * Return the fontsize.
     * @return the fontsize
     */
    [[nodiscard]] const int &getSize();

    /**
     * Get the height of the bitmap texture.
     * @return the height of the bitmap texture
     */
    [[nodiscard]] unsigned int getTextureHeight();

    /**
     * Get the width of the bitmap texture.
     * @return the width of the bitmap texture
     */
    [[nodiscard]] unsigned int getTextureWidth();

    /**
     * Returns the file used to create this object.
     * @return the font file
     */
    [[nodiscard]] const File &getFile();

    /**
     * Initializes the font loading library. (FreeType2)
     */
    static void init();

private:
    void loadGlyphs();

private:
    FontData *data;
    File file;

    void loadGlyph(unsigned int glyphIndex, unsigned long charCode, void *glyph, unsigned int &maxWidth,
                   unsigned int &maxHeight, unsigned int &glyphCount) const;
};

/**
 * A Font provides the information to render text in a visible way.
 */
class Font {
public:

    /**
     * Creates an empty font. Defaults to the default font if the font library is initialized.
     */
    Font() noexcept;

    /**
     * Creates a Font from a specified file.
     * @param file the specified file
     */
    explicit Font(const File &file);


    /**
     * Creates a Font from a specified file with a specified size
     * @param file the specified file
     * @param size the specified size
     */
    Font(const File &file, int size);

    /**
     * Resize this Font.
     * @param size the new size
     */
    void setSize(int size);

    /**
     * Returns the generated bitmap.
     * @return the bitmap
     */
    [[nodiscard]] std::shared_ptr<Texture> getBitmap();

    /**
     * Get the FontCharMetric for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharMetric
     */
    [[nodiscard]] const FontCharMetric &getCharMetric(unsigned long charcode);

    /**
     * Get the FontCharBitmapInformation for a specified charcode.
     * @param charcode the specified charcode
     * @return the FontCharBitmapInformation
     */
    [[nodiscard]] const FontCharBitmapInformation &getCharBitmapInformation(unsigned long charcode);

    /**
     * Return the fontsize.
     * @return the fontsize
     */
    [[nodiscard]] const int &getSize();

    /**
     * Get the height of the bitmap texture.
     * @return the height of the bitmap texture
     */
    [[nodiscard]] unsigned int getTextureHeight();

    /**
     * Get the width of the bitmap texture.
     * @return the width of the bitmap texture
     */
    [[nodiscard]] unsigned int getTextureWidth();

private:
    std::shared_ptr<FontHolder> fontHolder;
};

}