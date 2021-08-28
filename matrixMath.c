#include "matrixMath.h"
#include <cmath>

matrix matScale(matrix mat, double factor)
{
    for (int i = 0; i < mat.size(); i++)
        for (int j = 0; j < mat[i].size(); j++)
            mat[i][j] *= factor;
    return mat;
}

matrix matMul(matrix mat1, matrix mat2)
{
    if (mat1[0].size() != mat2.size())
        throw "Worng Matrecies Size";
    matrix result = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    for (int i = 0; i < mat1.size(); ++i)
    {
        for (int j = 0; j < mat2[0].size(); ++j)
        {
            for (int k = 0; k < mat1[0].size(); ++k)
            {
                result[i][j] += mat1[i][k] * mat2[k][j]; //use std::insert i guess
            }
        }
    }
    return result;
}

matrix matMove(matrix mat, double x, double y, double z)
{
    for (int i = 0; i < mat.size(); i++)
    {
        mat[i][0] += x;
        mat[i][1] += y;
        mat[i][2] += z;
    }
    return mat;
}

void rotateX(matrix &point, double angle)
{
    matrix rotationMatrixX = {
        {1, 0, 0},
        {0, cos(angle), -sin(angle)},
        {0, sin(angle), cos(angle)}};
    point = matMul(rotationMatrixX, point);
}

void rotateY(matrix &point, double angle)
{
    matrix rotationMatrixY = {
        {cos(angle), 0, sin(angle)},
        {0, 1, 0},
        {-sin(angle), 0, cos(angle)}};
    point = matMul(rotationMatrixY, point);
}

void rotateZ(matrix &point, double angle)
{
    matrix rotationMatrixZ = {
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0, 0, 1}};
    point = matMul(rotationMatrixZ, point);
}

void project(matrix &point, double distance)
{
    double w = 1 / (distance - point[2][0]);
    matrix projectionMatrix =
        {{{w, 0, 0},
          {0, w, 0}}};
    point = matMul(projectionMatrix, point);
}