//
// Created by jacko on 30/10/2020.
//

#ifndef POLYTEST_IMAGE_H
#define POLYTEST_IMAGE_H


#include <ostream>


#define ERROR_IMAGE_DIM "ERROR: Location vectors given to image don't fit the image requirements."


/**
 * Exception for problems with location input in Image.
 */
class ImageDimException : public std::exception
{
public:
    const char *what() const noexcept override
    {
        return ERROR_IMAGE_DIM;
    }
};

/**
 * 2 dimensional vector.
 */
struct Vector2
{
    int x, y;

    /**
     * Creates a new 2d vector with the given coordinates.
     *
     * @param a The x coordinate - defaults to 0.
     * @param b The y coordinate - defaults to 0.
     */
    explicit Vector2(int a = 0, int b = 0) : x(a), y(b)
    {}

};

/**
 * Class representing a 2d grayscale image.
 */
class Image
{
    int _height, _width;
    unsigned char **_matrix;

    // frees the memory taken by the image data.
    void _freeMatrix();

    // copies the given image data to this object.
    void _copyMatrix(int height, int width, const unsigned char *const *otherMatrix);

public:
    /**
     * Creates a new grayscale image of the given parameters.
     *
     * @param height The image height in pixels.
     * @param width The image width in pixels.
     * @param color The color to set all pixels to - defaults to 0 (black).
     */
    Image(int height, int width, unsigned char color = 0) noexcept;

    /**
     * Creates a new grayscale image this is a copy of the given matrix..
     *
     * @param height The image height in pixels.
     * @param width The image width in pixels.
     * @param otherMatrix The matrix to copy the image data from.
     */
    Image(int height, int width, const unsigned char **otherMatrix) noexcept;

    /**
     * Copy ctor for image.
     *
     * @param otherImage The image to copy.
     */
    Image(const Image &otherImage);

    /**
     * Assign this image to be a copy of the given one.
     *
     * @param otherImage  The image to copy.
     * @return This image after the copy procedure.
     */
    Image &operator=(const Image &otherImage);

    /**
     * Destructs the image.
     */
    ~Image() noexcept;

    /**
     * Returns the image's width.
     *
     * @return The image's width.
     */
    int getWidth() const;

    /**
     * Returns the image's height.
     *
     * @return The image's height.
     */
    int getHeight() const;

    /**
     * Draws a pixel of the given color at the given location.
     * Throws exception if location is out of image bounds.
     *
     * @param location 2d vector representing image location.
     * @param color The color to draw (1 byte grayscale).
     */
    void drawPixel(const Vector2 &location, unsigned char color);

    /**
     * Draws a horizontal line of teh given color from the start location to the given x coordinate.
     *
     * @param start 2d vector representing start location.
     * @param xFinish The last x coordinate of the line.
     * @param color The color to draw (1 byte grayscale).
     */
    void drawHorizontalLine(const Vector2 &start, int xFinish, unsigned char color);

    /**
     * Return true if the given pixel is in the image bounds. Otherwise, returns false.
     *
     * @param x The x coordinate of the pixel.
     * @param y The y coordinate of the pixel.
     * @return true if the given pixel is in the image bounds. Otherwise, returns false.
     */
    bool isPixelValid(int x, int y) const;

    /**
    * Return true if the given pixel is in the image bounds. Otherwise, returns false.
    *
    * @param location 2d vector representing image location.
    * @return true if the given pixel is in the image bounds. Otherwise, returns false.
    */
    bool isPixelValid(const Vector2 &location) const;

    /**
     * Returns the intensity value of the given pixel.
     * Throws exception if location is out of image bounds.
     *
     *
     * @param x The x coordinate of the pixel.
     * @param y The y coordinate of the pixel.
     * @return The intensity value of the given pixel.
     */
    unsigned char getPixel(int x, int y) const;

    /**
     * Returns the intensity value of the given pixel.
     * Throws exception if location is out of image bounds.
     *
     *
     * @param location 2d vector representing image location.
     * @return The intensity value of the given pixel.
     */
    unsigned char getPixel(const Vector2 &location) const;

    /**
     * Prints the image to the output stream (as integer matrix).
     *
     * @param os The output stream to send to.
     * @param img The image to print.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Image &img) noexcept;
};


#endif //POLYTEST_IMAGE_H
