//
// Created by jacko on 30/10/2020.
//

#ifndef POLYTEST_SHAPES_H
#define POLYTEST_SHAPES_H


#include <utility>
#include "Image.h"

/**
 * Represents 2d shape.
 */
class Shape
{
    Vector2 *_vertices;
    int _verticesSize;
    unsigned char _color;

    // Returns true if given point is in this shape. Otherwise, returns false.
    bool _isPointInShape(const Vector2 &point) const;

protected:
    /**
     * Default ctor for Shape (can't be drawn)
     */
    Shape();

    /**
     * Creates a new Shape of the given color that has the given vertices.
     *
     * @param vertices Dynamically allocated array of 2d vectors (this class has ownership on this pointer).
     * @param verticesSize The size of the vertices array.
     * @param color The color of the shape (1 byte grayscale).
     */
    Shape(Vector2 *vertices, int verticesSize, unsigned char color);

    /**
     * Makes this shape a copy of the given shape with a new given color.
     *
     * @param other The shape to copy.
     * @param color The color of this shape.
     */
    Shape(const Shape &other, unsigned char color);

    /**
     * Returns this shape's array of vertices.
     *
     * @return this shape's array of vertices.
     */
    const Vector2 *getVertices() const;

    /**
     * Return's this shape's color.
     *
     * @return This shape's color.
     */
    unsigned char getColor() const;

public:
    /**
     * Destructs this shape.
     */
    virtual ~Shape();

    /**
     * Draw's this shape to the given image.
     *
     * @param img The image to draw to.
     */
    virtual void draw(Image &img) const;

    /**
     * Returns an array of pointers to Shapes that contains all rectangles that are parallel to the x and y axis.
     * Shapes can only be in non-zero color.
     * The output array (and all pointers in it) needs to be freed (either manually or with freeShapesArray).
     *
     * @param img The image to scan in.
     * @param arrSize This will be set to the size of the output array.
     * @return an array of pointers to Shapes that contains all rectangles that are parallel to the x and y axis.
     */
    static Shape **getRectanglesFromImage(const Image &img, int &arrSize);

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
    static Shape **getRectanglesAndTrianglesFromImage(const Image &img, int &arrSize);

    /**
     * Draws the given shapes to the given image.
     *
     * @param img The image to draw to.
     * @param shapes An array of shape pointers.
     * @param size The size of the shapes array.
     */
    static void drawShapesToImage(Image &img, const Shape **shapes, int size);

    /**
     * Frees the memory taken by a dynamically allocated array of dynamically allocated shape pointers.
     * Used to free array output of getRectanglesFromImage and getRectanglesAndTrianglesFromImage.
     *
     * @param shapes An array of pointers to Shapes.
     * @param size The size of the shapes array.
     */
    static void freeShapesArray(Shape **shapes, int size);
};

/**
 * Represents a 2d triangle.
 */
class Triangle : public Shape
{
public:
    /**
     * Default ctor for Triangle (can't be drawn)
     */
    Triangle();

    /**
     * Creates a new triangle according to given vertices.
     * Vertices order should be in clockwise order.
     *
     * @param a First vertex in clockwise order.
     * @param b Second vertex in clockwise order.
     * @param c Third vertex in clockwise order.
     * @param color The color of the triangle.
     */
    Triangle(const Vector2 &a, const Vector2 &b, const Vector2 &c, unsigned char color);

    /**
     * Makes this Triangle a copy of the given Triangle with a new given color.
     *
     * @param other The Triangle to copy.
     * @param color The color of this shape.
     */
    Triangle(const Triangle &other, unsigned char color);

    /**
     * Recognizes the Triangle (that is parallel to the x axis) whose top-left corner is the given location
     * and then sets innerTriangle to this triangle.
     *
     * @param img The image to scan in.
     * @param topLeft The top-left pixel of the triangle.
     * @param innerTriangle This will be set to the new Triangle object.
     */
    static void recognizeTriangle(const Image &img, const Vector2 &topLeft, Triangle &innerTriangle);
};

/**
 * Represents a 2d rectangle.
 */
class Rectangle : public Shape
{
public:
    /**
     * Default ctor for rectangle (can't be drawn)
     */
    Rectangle();

    /**
     * Creates a new Rectangle according to given vertices.
     *
     * @param topLeft The top-left pixel of the rectangle.
     * @param topRight The top-right pixel of the rectangle.
     * @param bottomRight The bottom-right pixel of the rectangle.
     * @param bottomLeft The bottom-left pixel of the rectangle.
     * @param color The color of the rectangle.
     */
    Rectangle(const Vector2 &topLeft, const Vector2 &topRight, const Vector2 &bottomRight, const Vector2 &bottomLeft,
              unsigned char color);

    /**
     * Creates a new Rectangle (parallel to the x and y axis) according to given vertices.
     * (Other new corners are calculated from the given two)
     *
     * @param topLeft The top-left pixel of the rectangle.
     * @param bottomRight The bottom-right pixel of the rectangle.
     * @param color The color of the rectangle.
     */
    Rectangle(const Vector2 &topLeft, const Vector2 &bottomRight, unsigned char color);

    /**
     * Makes this Rectangle a copy of the given Rectangle with a new given color.
     *
     * @param other The Rectangle to copy.
     * @param color The color of this shape.
     */
    Rectangle(const Rectangle &other, unsigned char color);

    /**
     * Recognizes the Rectangle (that is parallel to the x and y axis) whose top-left corner is the given location
     * and then sets rectangle to this Rectangle.
     *
     * @param img The image to scan in.
     * @param topLeft The top-left pixel of the Rectangle.
     * @param rectangle This will be set to the new Rectangle object.
     */
    static void recognizeRectangle(const Image &img, const Vector2 &topLeft, Rectangle &rectangle);

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
    static bool recognizeRectangleWithTriangle(const Image &img, const Vector2 &topLeft, Rectangle &rectangle,
                                               Triangle &innerTriangle);

};

/**
 * Represents a 2d circle.
 */
class Circle : public Shape
{
    int _radius;

public:
    /**
     * Default ctor for circle (can't be drawn)
     */
    Circle();

    /**
     * Creates a new circle according the given center point, radius and color.
     *
     * @param center 2d vector that has location of center point.
     * @param radius The circle's radius.
     * @param color The color of the circle.
     */
    Circle(const Vector2 &center, int radius, unsigned char color);

    /**
     * Makes this Circle a copy of the given Circle with a new given color.
     *
     * @param other The Circle to copy.
     * @param color The color of this shape.
     */
    Circle(const Circle &other, unsigned char color);

    /**
     * Draw's this circle to the given image.
     *
     * @param img The image to draw to.
     */
    void draw(Image &img) const override;
};

#endif //POLYTEST_SHAPES_H
