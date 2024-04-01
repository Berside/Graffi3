#include <map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void mirrorDiagonalColumnMajor(vector<int>& matrix, int rows, int cols) {
    vector<int> mirroredMatrix(rows * cols, 0);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int mirroredRow = rows - 1 - j;
            int mirroredCol = cols - 1 - i;
            int index = mirroredRow * cols + mirroredCol;
            mirroredMatrix[index] = matrix[i * cols + j];
        }
    }

    matrix = mirroredMatrix;
}

void printMatrix(const vector<int>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            cout << matrix[index] << " ";
        }
        cout << endl;
    }
}

bool isSparse(const vector<int>& matrix, int rows, int cols) {
    int counter = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            if (matrix[index] == 0) {
                ++counter;
            }
        }
    }
    return (counter > (rows * cols) / 2);
}

int main() {
    vector<int> matrix = { 5,1,0,0,0,0,9,8,7};
    int rows = 3;
    int cols = 3;
    printMatrix(matrix, rows, cols);
    cout << endl;
    if (isSparse(matrix, rows, cols)) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }
    cout << endl;
    mirrorDiagonalColumnMajor(matrix, rows, cols);
    printMatrix(matrix, rows, cols);
    return 0;
}
