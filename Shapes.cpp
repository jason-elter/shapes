//
// Created by jacko on 30/10/2020.
//

#include <list>
#include <iostream>
#include "Shapes.h"


static const int BACKGROUND = 0;

/**
 * Default ctor for Shape (can't be drawn)
 */
Shape::Shape() : _vertices(nullptr), _verticesSize(0), _color(0)
{}

/**
 * Creates a new Shape of the given color that has the given vertices.
 *
 * @param vertices Dynamically allocated array of 2d vectors (this class has ownership on this pointer).
 * @param verticesSize The size of the vertices array.
 * @param color The color of the shape (1 byte grayscale).
 */
Shape::Shape(Vector2 *vertices, int verticesSize, unsigned char color) : _vertices(vertices),
                                                                         _verticesSize(verticesSize),
                                                                         _color(color)
{}

/**
 * Makes this shape a copy of the given shape with a new given color.
 *
 * @param other The shape to copy.
 * @param color The color of this shape.
 */
Shape::Shape(const Shape &other, unsigned char color) : _vertices(new Vector2[other._verticesSize]),
                                                        _verticesSize(other._verticesSize), _color(color)
{
    for (int i = 0; i < _verticesSize; ++i)
    {
        _vertices[i] = other._vertices[i];
    }
}

/**
 * Destructs this shape.
 */
Shape::~Shape()
{
    if (_verticesSize > 0)
    {
        delete[] _vertices;
    }
}

/**
 * Returns this shape's array of vertices.
 *
 * @return this shape's array of vertices.
 */
const Vector2 *Shape::getVertices() const
{
    return _vertices;
}

/**
 * Return's this shape's color.
 *
 * @return This shape's color.
 */
unsigned char Shape::getColor() const
{
    return _color;
}

// Sets the bounding box that the shape is in.
static void setBoundingBox(int &minX, int &minY, int &maxX, int &maxY, const Vector2 *vertices, int size)
{
    minX = maxX = vertices[0].x;
    minY = maxY = vertices[0].y;
    for (int i = 1; i < size; ++i)
    {
        const Vector2 &current = vertices[i];
        if (current.x < minX)
        {
            minX = current.x;
        }
        else if (current.x > maxX)
        {
            maxX = current.x;
        }

        if (current.y < minY)
        {
            minY = current.y;
        }
        else if (current.y > maxY)
        {
            maxY = current.y;
        }
    }
}

// Returns true if given point is in half space created by the two other vectors. Otherwise, returns false.
static bool isPointInHalfSpace(const Vector2 &a, const Vector2 &b, const Vector2 &point)
{
    return ((point.x - a.x) * (b.y - a.y) - (point.y - a.y) * (b.x - a.x)) <= 0;
}

// Returns true if given point is in this shape. Otherwise, returns false.
bool Shape::_isPointInShape(const Vector2 &point) const
{
    for (int i = 1; i < _verticesSize; ++i)
    {
        if (!isPointInHalfSpace(_vertices[i - 1], _vertices[i], point))
        {
            return false;
        }
    }
    return isPointInHalfSpace(_vertices[_verticesSize - 1], _vertices[0], point);
}

/**
 * Draw's this shape to the given image.
 *
 * @param img The image to draw to.
 */
void Shape::draw(Image &img) const
{
    int minX, minY, maxX, maxY;
    setBoundingBox(minX, minY, maxX, maxY, _vertices, _verticesSize);

    for (Vector2 currentPoint(minX, minY); currentPoint.y <= maxY; ++currentPoint.y)
    {
        for (; currentPoint.x <= maxX; ++currentPoint.x)
        {
            if (_isPointInShape(currentPoint))
            {
                img.drawPixel(currentPoint, _color);
            }
        }
        currentPoint.x = minX;
    }
}

/**
 * Returns an array of pointers to Shapes that contains all rectangles that are parallel to the x and y axis.
 * Shapes can only be in non-zero color.
 * The output array (and all pointers in it) needs to be freed (either manually or with freeShapesArray).
 *
 * @param img The image to scan in.
 * @param arrSize This will be set to the size of the output array.
 * @return an array of pointers to Shapes that contains all rectangles that are parallel to the x and y axis.
 */
Shape **Shape::getRectanglesFromImage(const Image &img, int &arrSize)
{
    std::list<Shape *> rectangles;
    Image tempImg(img);
    int width = img.getWidth();
    int height = img.getWidth();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (tempImg.getPixel(x, y) != BACKGROUND)
            {
                Rectangle *newRect;
                Rectangle::recognizeRectangle(tempImg, Vector2(x, y), &newRect);
                rectangles.push_front(newRect);
                Rectangle(*newRect, BACKGROUND).draw(tempImg);
            }
        }
    }


    arrSize = rectangles.size();
    auto **shapesArray = new Shape *[arrSize];
    std::copy(rectangles.begin(), rectangles.end(), shapesArray);
    return shapesArray;
}

/**
 * Returns an array of pointers to Shapes that contains all rectangles (that are parallel to the x and y axis)
 * and all triangles that are in the rectangles (that are parallel to the x axis).
 * Each rectangle can have either 0 or 1 triangles in them.
 * Shapes can only be in non-zero color.
 * The output array (and all pointers in it) needs to be freed (either manually or with freeShapesArray).
 *
 * @param img The image to scan in.
 * @param arrSize This will be set to the size of the output array.
 * @return an array of pointers to Shapes that contains all rectangles and triangles.
 */
Shape **Shape::getRectanglesAndTrianglesFromImage(const Image &img, int &arrSize)
{
    std::list<Shape *> shapes;
    Image tempImg(img);
    int width = img.getWidth();
    int height = img.getWidth();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (tempImg.getPixel(x, y) != BACKGROUND)
            {
                Rectangle *newRect;
                Triangle *newTriangle;
                if (Rectangle::recognizeRectangleWithTriangle(tempImg, Vector2(x, y), &newRect, &newTriangle))
                {
                    // Triangle was found.
                    shapes.push_back(newTriangle);
                }
                shapes.push_front(newRect);
                Rectangle(*newRect, BACKGROUND).draw(tempImg);
            }
        }
    }

    arrSize = shapes.size();
    auto **shapesArray = new Shape *[arrSize];
    std::copy(shapes.begin(), shapes.end(), shapesArray);
    return shapesArray;
}

/**
 * Draws the given shapes to the given image.
 *
 * @param img The image to draw to.
 * @param shapes An array of shape pointers.
 * @param size The size of the shapes array.
 */
void Shape::drawShapesToImage(Image &img, const Shape **shapes, int size)
{
    for (int i = 0; i < size; ++i)
    {
        shapes[i]->draw(img);
    }
}

/**
 * Frees the memory taken by a dynamically allocated array of dynamically allocated shape pointers.
 * Used to free array output of getRectanglesFromImage and getRectanglesAndTrianglesFromImage.
 *
 * @param shapes An array of pointers to Shapes.
 * @param size The size of the shapes array.
 */
void Shape::freeShapesArray(Shape **shapes, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete shapes[i];
    }
    delete[] shapes;
}

/**
 * Creates a new triangle according to given vertices.
 * Vertices order should be in clockwise order.
 *
 * @param a First vertex in clockwise order.
 * @param b Second vertex in clockwise order.
 * @param c Third vertex in clockwise order.
 * @param color The color of the triangle.
 */
Triangle::Triangle(const Vector2 &a, const Vector2 &b, const Vector2 &c, unsigned char color) : Shape(
        new Vector2[3]{a, b, c}, 3, color)
{}

// Sets bottomLeft to the bottom-left pixel of the triangle whose top-left pixel is given.
static void setTriangleBottomLeft(const Image &img, const Vector2 &topLeft, Vector2 &bottomLeft, unsigned char color)
{
    int x = topLeft.x;
    int y = topLeft.y;

    while (true)
    {
        if (img.isPixelValid(x - 1, y + 1) && (img.getPixel(x - 1, y + 1) == color))
        {
            // go diagonally left.
            x--;
            y++;
        }
        else if (img.isPixelValid(x, y + 1) && (img.getPixel(x, y + 1) == color))
        {
            // go straight down.
            y++;
        }
        else if (img.isPixelValid(x + 1, y + 1) && (img.getPixel(x + 1, y + 1) == color))
        {
            // go diagonally right.
            x++;
            y++;
        }
        else
        {
            // Can't go down anymore.
            break;
        }
    }

    // Now go as left as possible.
    while (img.isPixelValid(x - 1, y) && (img.getPixel(x - 1, y) == color))
    {
        x--;
    }

    bottomLeft = Vector2(x, y);
}

// Returns the horizontal length of a triangle starting from leftPoint location.
static int getTriangleHorizontalLength(const Image &img, const Vector2 &leftPoint, unsigned char color)
{
    int x = leftPoint.x;
    while (img.isPixelValid(x + 1, leftPoint.y) && (img.getPixel(x + 1, leftPoint.y) == color))
    {
        x++;
    }
    return x - leftPoint.x;
}

/**
 * Recognizes the Triangle (that is parallel to the x axis) whose top-left corner is the given location
 * and then sets innerTriangle to this triangle.
 *
 * @param img The image to scan in.
 * @param topLeft The top-left pixel of the triangle.
 * @param innerTriangle This will be set to the new Triangle object.
 */
void Triangle::recognizeTriangle(const Image &img, const Vector2 &topLeft, Triangle **innerTriangle)
{
    unsigned char color = img.getPixel(topLeft);
    Vector2 bottomLeft;
    setTriangleBottomLeft(img, topLeft, bottomLeft, color);
    int topLength = getTriangleHorizontalLength(img, topLeft, color);
    int bottomLength = getTriangleHorizontalLength(img, bottomLeft, color);

    Vector2 first, second, third;
    if (topLength > bottomLength)
    {
        // Triangle has parallel line on the top and single point on the bottom.
        first = topLeft;
        second = Vector2(topLeft.x + topLength, topLeft.y);
        third = Vector2(bottomLeft.x + (bottomLength / 2), topLeft.y); // get middle point (in case of 3+ pixels).
    }
    else
    {
        // Triangle has parallel line on the bottom and single point on the top.
        first = Vector2(topLeft.x + (bottomLength / 2), topLeft.y); // get middle point (in case of 3+ pixels).
        second = Vector2(bottomLeft.x + bottomLength, bottomLeft.y);
        third = bottomLeft;
    }

    *innerTriangle = new Triangle(first, second, third, color);
}

/**
 * Makes this Triangle a copy of the given Triangle with a new given color.
 *
 * @param other The Triangle to copy.
 * @param color The color of this shape.
 */
Triangle::Triangle(const Triangle &other, unsigned char color) : Shape(other, color)
{}

/**
 * Default ctor for Triangle (can't be drawn)
 */
Triangle::Triangle() : Shape()
{}

/**
 * Creates a new Rectangle according to given vertices.
 *
 * @param topLeft The top-left pixel of the rectangle.
 * @param topRight The top-right pixel of the rectangle.
 * @param bottomRight The bottom-right pixel of the rectangle.
 * @param bottomLeft The bottom-left pixel of the rectangle.
 * @param color The color of the rectangle.
 */
Rectangle::Rectangle(const Vector2 &topLeft, const Vector2 &topRight, const Vector2 &bottomRight,
                     const Vector2 &bottomLeft, unsigned char color) : Shape(
        new Vector2[4]{topLeft, topRight, bottomRight, bottomLeft}, 4, color)
{}

/**
 * Creates a new Rectangle (parallel to the x and y axis) according to given vertices.
 * (Other new corners are calculated from the given two)
 *
 * @param topLeft The top-left pixel of the rectangle.
 * @param bottomRight The bottom-right pixel of the rectangle.
 * @param color The color of the rectangle.
 */
Rectangle::Rectangle(const Vector2 &topLeft, const Vector2 &bottomRight, unsigned char color) : Rectangle(
        topLeft, Vector2(bottomRight.x, topLeft.y), bottomRight, Vector2(topLeft.x, bottomRight.y), color)
{}

// Sets bottomRight to the bottom-right pixel of the rectangle that contains the location start.
static void setBottomRightRectangleCorner(const Image &img, const Vector2 &start, Vector2 &bottomRight)
{
    int x = start.x + 1;
    int y = start.y + 1;

    while (img.isPixelValid(x, y) && img.getPixel(x, y) != BACKGROUND)
    {
        // go diagonally right.
        x++;
        y++;
    }

    if (img.isPixelValid(x - 1, y) && img.getPixel(x - 1, y) != BACKGROUND)
    {
        // We need to keep going down.
        x--; // Fix x because it's out of shape by 1.
        while (img.isPixelValid(x, ++y) && img.getPixel(x, y) != BACKGROUND);
        y--; // Fix y because it's out of shape by 1.
    }
    else if (img.isPixelValid(x, y - 1) && img.getPixel(x, y - 1) != BACKGROUND)
    {
        // We need to keep going right.
        y--; // Fix y because it's out of shape by 1.
        while (img.isPixelValid(++x, y) && img.getPixel(x, y) != BACKGROUND);
        x--; // Fix x because it's out of shape by 1.
    }
    else
    {
        // Found corner.
        x--; // Fix x because it's out of shape by 1.
        y--; // Fix y because it's out of shape by 1.
    }

    bottomRight = Vector2(x, y);
}

/**
 * Recognizes the Rectangle (that is parallel to the x and y axis) whose top-left corner is the given location
 * and then sets rectangle to this Rectangle.
 *
 * @param img The image to scan in.
 * @param topLeft The top-left pixel of the Rectangle.
 * @param rectangle This will be set to the new Rectangle object.
 */
void Rectangle::recognizeRectangle(const Image &img, const Vector2 &topLeft, Rectangle **rectangle)
{
    unsigned char color = img.getPixel(topLeft);
    Vector2 bottomRight;
    setBottomRightRectangleCorner(img, topLeft, bottomRight);
    *rectangle = new Rectangle(topLeft, bottomRight, color);
}

/**
 * Recognizes the Rectangle (that is parallel to the x and y axis) whose top-left corner is the given location
 * and then sets rectangle to this Rectangle.
 * Also Recognizes the Triangle (that is parallel to the x axis) if one is in the Rectangle
 * and then sets innerTriangle to this triangle.
 * Returns true if Triangle was found. Otherwise, returns false.
 *
 * @param img The image to scan in.
 * @param topLeft The top-left pixel of the Rectangle.
 * @param rectangle This will be set to the new Rectangle object.
 * @param innerTriangle This will be set to the new Triangle object (if found in Rectangle).
 * @return true if Triangle was found. Otherwise, returns false.
 */
bool Rectangle::recognizeRectangleWithTriangle(const Image &img, const Vector2 &topLeft, Rectangle **rectangle,
                                               Triangle **innerTriangle)
{
    unsigned char color = img.getPixel(topLeft);
    Vector2 bottomRight;
    setBottomRightRectangleCorner(img, topLeft, bottomRight);
    *rectangle =  new Rectangle(topLeft, bottomRight, color);

    for (int y = topLeft.y; y <= bottomRight.y; ++y)
    {
        for (int x = topLeft.x; x <= bottomRight.x; ++x)
        {
            if (img.getPixel(x, y) != color)
            {
                Triangle::recognizeTriangle(img, Vector2(x, y), innerTriangle);
                return true;
            }
        }
    }
    return false;
}

/**
 * Makes this Rectangle a copy of the given Rectangle with a new given color.
 *
 * @param other The Rectangle to copy.
 * @param color The color of this shape.
 */
Rectangle::Rectangle(const Rectangle &other, unsigned char color) : Shape(other, color)
{}

/**
 * Default ctor for rectangle (can't be drawn)
 */
Rectangle::Rectangle() : Shape()
{}

/**
 * Default ctor for circle (can't be drawn)
 */
Circle::Circle() : Shape(), _radius(-1)
{}

/**
 * Creates a new circle according the given center point, radius and color.
 *
 * @param center 2d vector that has location of center point.
 * @param radius The circle's radius.
 * @param color The color of the circle.
 */
Circle::Circle(const Vector2 &center, int radius, unsigned char color) : Shape(new Vector2[1]{center}, 1, color),
                                                                         _radius(radius)
{}

// Draws 8 sections of the circle and the lines in-between.
static void drawCirclePart(Image &img, const Vector2 &center, const Vector2 &currentPart, unsigned char color)
{
    // find locations of current 8 eighths of circle.
    int xLeft = center.x - currentPart.x;
    int xRight = center.x + currentPart.x;
    int xDown = center.x - currentPart.y;
    int xUp = center.x + currentPart.y;
    int yDown = center.y - currentPart.y;
    int yUp = center.y + currentPart.y;
    int yLeft = center.y - currentPart.x;
    int yRight = center.y + currentPart.x;

    // Draw the eighths and lines in-between.
    img.drawHorizontalLine(Vector2(xLeft, yUp), xRight, color);
    img.drawHorizontalLine(Vector2(xLeft, yDown), xRight, color);
    img.drawHorizontalLine(Vector2(xDown, yRight), xUp, color);
    img.drawHorizontalLine(Vector2(xDown, yLeft), xUp, color);
}

/**
 * Draw's this circle to the given image.
 *
 * @param img The image to draw to.
 */
void Circle::draw(Image &img) const
{
    if (_radius == -1)
    {
        return;
    }

    // This function is based on Bresenham's circle drawing algorithm.
    const Vector2 &center = getVertices()[0];
    unsigned char color = getColor();

    Vector2 currentPart(0, _radius);
    int decision = 3 - (2 * _radius);
    drawCirclePart(img, center, currentPart, color);

    while (currentPart.y >= currentPart.x)
    {
        currentPart.x++;
        if (decision > 0)
        {
            currentPart.y--;
            decision += 4 * (currentPart.x - currentPart.y) + 10;
        }
        else
        {
            decision += (4 * currentPart.x) + 6;
        }
        drawCirclePart(img, center, currentPart, color);
    }
}

/**
 * Makes this Circle a copy of the given Circle with a new given color.
 *
 * @param other The Circle to copy.
 * @param color The color of this shape.
 */
Circle::Circle(const Circle &other, unsigned char color) : Shape(other, color), _radius(other._radius)
{}
