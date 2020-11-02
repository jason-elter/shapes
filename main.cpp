#include <iostream>
#include "Image.h"
#include "Shapes.h"


int main()
{
    Rectangle rect1 = Rectangle(Vector2(1, 1), Vector2(3, 2), 70);
    Rectangle rect2 = Rectangle(Vector2(3, 4), Vector2(4, 5), 140);
    Triangle triangle = Triangle(Vector2(2, 1), Vector2(3, 2), Vector2(1, 2), 210);
    const Shape *shapesOriginal[3] = {&rect1, &rect2, &triangle};

    Image img1 = Image(6, 6);
    Image img2 = Image(6, 6);
    Shape::drawShapesToImage(img1, shapesOriginal, 2);
    Shape::drawShapesToImage(img2, shapesOriginal, 3);

    std::cout << "Original images:" << std::endl << std::endl;
    std::cout << img1 << std::endl << std::endl;
    std::cout << img2 << std::endl << std::endl;

    int arrSize1, arrSize2;
    Shape **shapes1 = Shape::getRectanglesFromImage(img1, arrSize1);
    Shape **shapes2 = Shape::getRectanglesAndTrianglesFromImage(img2, arrSize2);
    Image img1Compare = Image(6, 6);
    Image img2Compare = Image(6, 6);
    Shape::drawShapesToImage(img1Compare, (const Shape **) shapes1, arrSize1);
    Shape::drawShapesToImage(img2Compare, (const Shape **) shapes2, arrSize2);

    std::cout << "New images:" << std::endl << std::endl;
    std::cout << img1Compare << std::endl << std::endl;
    std::cout << img2Compare << std::endl << std::endl;

    Shape::freeShapesArray(shapes1, arrSize1);
    Shape::freeShapesArray(shapes2, arrSize2);

    return 0;
}