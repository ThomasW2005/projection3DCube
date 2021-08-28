#include <vector>

struct vec3d
{
    double x, y, z;
};

typedef std::vector<std::vector<double>> matrix;
matrix matMul(matrix mat1, matrix mat2);
matrix matScale(matrix mat, double factor);
matrix matMove(matrix mat, double x, double y, double z);
void rotateX(matrix &point, double angle);
void rotateY(matrix &point, double angle);
void rotateZ(matrix &point, double angle);
void project(matrix &point, double distance);