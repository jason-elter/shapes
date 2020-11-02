#include <algorithm>
#include "Image.h"


// frees the memory taken by the image data.
void Image::_freeMatrix()
{
    for (int i = 0; i < _height; ++i)
    {
        delete[] _matrix[i];
    }
    delete[] _matrix;
}

// copies the given image data to this object.
void Image::_copyMatrix(int height, int width, const unsigned char *const *otherMatrix)
{
    _height = height;
    _width = width;
    _matrix = new unsigned char *[_height];
    for (int i = 0; i < _height; ++i)
    {
        _matrix[i] = new unsigned char[_width];
        unsigned char *row = _matrix[i];
        const unsigned char *otherRow = otherMatrix[i];
        for (int j = 0; j < _width; ++j)
        {
            row[j] = otherRow[j];
        }
    }
}


/**
 * Creates a new grayscale image of the given parameters.
 *
 * @param height The image height in pixels.
 * @param width The image width in pixels.
 * @param color The color to set all pixels to - defaults to 0 (black).
 */
Image::Image(int height, int width, unsigned char color) noexcept: _height(height), _width(width)
{
    _matrix = new unsigned char *[_height];
    for (int i = 0; i < _height; ++i)
    {
        _matrix[i] = new unsigned char[_width];
        unsigned char *row = _matrix[i];
        for (int j = 0; j < _width; ++j)
        {
            row[j] = color;
        }
    }
}

/**
 * Creates a new grayscale image this is a copy of the given matrix..
 *
 * @param height The image height in pixels.
 * @param width The image width in pixels.
 * @param otherMatrix The matrix to copy the image data from.
 */
Image::Image(int height, int width, const unsigned char **otherMatrix) noexcept
{
    _copyMatrix(height, width, otherMatrix);
}

/**
 * Destructs the image.
 */
Image::~Image() noexcept
{
    _freeMatrix();
}

/**
 * Draws a pixel of the given color at the given location.
 * Throws exception if location is out of image bounds.
 *
 * @param location 2d vector representing image location.
 * @param color The color to draw (1 byte grayscale).
 */
void Image::drawPixel(const Vector2 &location, unsigned char color)
{
    if (location.x < 0 || location.x >= _width || location.y < 0 || location.y >= _height)
    {
        throw ImageDimException();
    }

    _matrix[location.y][location.x] = color;
}

/**
 * Draws a horizontal line of teh given color from the start location to the given x coordinate.
 *
 * @param start 2d vector representing start location.
 * @param xFinish The last x coordinate of the line.
 * @param color The color to draw (1 byte grayscale).
 */
void Image::drawHorizontalLine(const Vector2 &start, int xFinish, unsigned char color)
{
    if (start.x < 0 || start.y < 0 || start.y >= _height || xFinish >= _width || start.x > xFinish)
    {
        throw ImageDimException();
    }

    unsigned char *row = _matrix[start.y];
    for (int x = start.x; x <= xFinish; ++x)
    {
        row[x] = color;
    }
}

/**
 * Return true if the given pixel is in the image bounds. Otherwise, returns false.
 *
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @return true if the given pixel is in the image bounds. Otherwise, returns false.
 */
bool Image::isPixelValid(int x, int y) const
{
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

/**
* Return true if the given pixel is in the image bounds. Otherwise, returns false.
*
* @param location 2d vector representing image location.
* @return true if the given pixel is in the image bounds. Otherwise, returns false.
*/
bool Image::isPixelValid(const Vector2 &location) const
{
    return isPixelValid(location.x, location.y);
}

/**
 * Returns the intensity value of the given pixel.
 * Throws exception if location is out of image bounds.
 *
 *
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @return The intensity value of the given pixel.
 */
unsigned char Image::getPixel(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height)
    {
        throw ImageDimException();
    }

    return _matrix[y][x];
}

/**
 * Returns the intensity value of the given pixel.
 * Throws exception if location is out of image bounds.
 *
 *
 * @param location 2d vector representing image location.
 * @return The intensity value of the given pixel.
 */
unsigned char Image::getPixel(const Vector2 &location) const
{
    return getPixel(location.x, location.y);
}

/**
 * Prints the image to the output stream (as integer matrix).
 *
 * @param os The output stream to send to.
 * @param img The image to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const Image &img) noexcept
{
    int lastColumn = img._width - 1;
    for (int i = 0; i < img._height; i++)
    {
        unsigned char *row = img._matrix[i];
        for (int j = 0; j < lastColumn; j++)
        {
            os << (int) row[j] << " ";
        }
        os << (int) row[lastColumn] << std::endl;
    }
    return os;
}

/**
 * Copy ctor for image.
 *
 * @param otherImage The image to copy.
 */
Image::Image(const Image &otherImage)
{
    _copyMatrix(otherImage._height, otherImage._width, otherImage._matrix);
}

/**
 * Assign this image to be a copy of the given one.
 *
 * @param otherImage  The image to copy.
 * @return This image after the copy procedure.
 */
Image &Image::operator=(const Image &otherImage)
{
    if (this != &otherImage)
    {
        _freeMatrix();
        _copyMatrix(otherImage._height, otherImage._width, otherImage._matrix);
    }
    return *this;
}

/**
 * Returns the image's width.
 *
 * @return The image's width.
 */
int Image::getWidth() const
{
    return _width;
}

/**
 * Returns the image's height.
 *
 * @return The image's height.
 */
int Image::getHeight() const
{
    return _height;
}
