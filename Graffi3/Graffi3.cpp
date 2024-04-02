#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <chrono>
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

vector<int> convertToDenseMatrix(const vector<int>& values, const vector<int>& columns, const vector<int>& rows, int rowsCount, int colsCount) {
    vector<int> denseMatrix(rowsCount * colsCount, 0);
    for (size_t i = 0; i < values.size(); ++i) {
        int row = rows[i];
        int col = columns[i];
        int index = row * colsCount + col;
        denseMatrix[index] = values[i];
    }
    return denseMatrix;
}

vector<int> convertToDenseMatrixFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return {};
    }
    int rowsCount, colsCount;
    string line;
    getline(file, line);
    istringstream(line) >> rowsCount >> colsCount;
    vector<int> values, columns, rows;
    for (int i = 0; i < 3; ++i) {
        getline(file, line);
        istringstream iss(line);
        int value;
        while (iss >> value) {
            if (i == 0) {
                values.push_back(value);
            }
            else if (i == 1) {
                columns.push_back(value);
            }
            else {
                rows.push_back(value);
            }
        }
    }
    if (values.size() != columns.size() || values.size() != rows.size()) {
        cerr << "Размеры векторов значений, столбцов и строк не совпадают." << endl;
        return {};
    }
    return convertToDenseMatrix(values, columns, rows, rowsCount, colsCount);
}

vector<int> generateSparseMatrix(int rows, int cols, double sparsity) {
    vector<int> matrix(rows * cols, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            if (static_cast<double>(rand()) / RAND_MAX < sparsity) {
                continue;
            }
            else {
                matrix[index] = rand() % 100;
            }
        }
    }
    while (!isSparse(matrix, rows, cols)) {
        int randomRow = rand() % rows;
        int randomCol = rand() % cols;
        int index = randomRow * cols + randomCol;
        matrix[index] = 0;
    }
    return matrix;
}

void convertToSparseMatrix(const vector<int>& denseMatrix, int rowsCount, int colsCount, vector<int>& values, vector<int>& columns, vector<int>& rows) {
    values.clear();
    columns.clear();
    rows.clear();

    for (int row = 0; row < rowsCount; ++row) {
        for (int col = 0; col < colsCount; ++col) {
            int index = row * colsCount + col;
            if (denseMatrix[index] != 0) {
                values.push_back(denseMatrix[index]);
                columns.push_back(col);
                rows.push_back(row);
            }
        }
    }
}

void writeSparseMatrixToFile(const vector<int>& values, const vector<int>& columns, const vector<int>& rows, const string& filename, int rowws, int cols) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }
    file << rowws << " " << cols << "\n";
    for (const auto& value : values) {
        file << value << " ";
    }
    file << "\n";
    for (const auto& column : columns) {
        file << column << " ";
    }
    file << "\n";
    for (const auto& row : rows) {
        file << row << " ";
    }
    file << "\n";
    file.close();
}

vector<int> addMatrices(const vector<int>& matrix1, const vector<int>& matrix2, int rows, int cols) {
    if (matrix1.size() != matrix2.size() || matrix1.size() != rows * cols) {
        cerr << "Размеры матриц не совпадают." << endl;
        return {};
    }

    vector<int> result(rows * cols, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            result[index] = matrix1[index] + matrix2[index];
        }
    }
    return result;
}

vector<int> subtractMatrices(const vector<int>& matrix1, const vector<int>& matrix2, int rows, int cols) {
    if (matrix1.size() != matrix2.size() || matrix1.size() != rows * cols) {
        cerr << "Размеры матриц не совпадают." << endl;
        return {};
    }

    vector<int> result(rows * cols, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            result[index] = matrix1[index] - matrix2[index];
        }
    }
    return result;
}

vector<int> readFirstTwoNumbersFromFile(string filename) {
    ifstream inputFile(filename);
    string line;
    vector<int> numbers;
    if (inputFile.is_open()) {
        getline(inputFile, line); // Read the first line
        istringstream iss(line);
        int number;

        // Read the first two numbers
        if (iss >> number) {
            numbers.push_back(number);
            if (iss >> number) {
                numbers.push_back(number);
            }
        }

        inputFile.close(); // Close the file
    }
    else {
        cout << "Unable to open file" << endl;
    }

    return numbers;
}

vector<int> multiplyMatrices(const vector<int>& matrix1, const vector<int>& matrix2, int rows1, int cols1, int cols2) {
    vector<int> result(rows1 * cols2, 0);
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i * cols2 + j] += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
            }
        }
    }
    return result;
}


int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    string filename = "Test.txt";
    int rows, cols;
    double sparsity;
    vector <int> values;
    vector <int> ROW;
    vector <int> COL;
    vector <int> numbers;
    int k;
    cout << "Выберите способ запуск программы:" << endl;
    cout << "Если вы хотите создать разреженную матрицу используя входные данные. Напишите: 1" << endl;
    cout << "Если вы хотите создать разреженную матрицу используя случайную генерацию. Напишите: 2" << endl;
    cout << "Если вы хотите произвести действия над разреженной матрицой из файла и последней разреженной матрицей. Напишите: 3" << endl;
    cin >> k;
    cout << endl;
    if (k == 1) {
        cout << "Вот ваша матрица:" << endl;
        vector<int> denseMatrix = convertToDenseMatrixFromFile(filename);
        numbers = readFirstTwoNumbersFromFile("matrix.txt");
        if (isSparse(denseMatrix, numbers[0], numbers[1]) == true) {
            cout << "Данная матрица разреженная" << endl;
        }
        else {
            cout << "Данная матрица не является разреженной" << endl;
        }
        cout << endl;
        printMatrix(denseMatrix, numbers[0], numbers[1]);
        cout << endl;
        cout << "Вот зеркальная матриц:" << endl;
        auto start1 = chrono::high_resolution_clock::now();
         mirrorDiagonalColumnMajor(denseMatrix, numbers[0], numbers[1]);
         auto end1 = chrono::high_resolution_clock::now();
         auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count();
         cout << "Время выполения алгоритма: " << duration1 << " microseconds" << endl;
         if (isSparse(denseMatrix, numbers[0], numbers[1]) == true) {
             cout << "Данная матрица разреженная" << endl;
         }
         else {
             cout << "Данная матрица не является разреженной" << endl;
         }
         cout << endl;
         printMatrix(denseMatrix, numbers[0], numbers[1]);
         cout << endl;
         cout << "Эта матрица была записана в файл Test.txt" << endl;
         convertToSparseMatrix(denseMatrix, numbers[0], numbers[1], values, ROW, COL);
         writeSparseMatrixToFile(values, COL, ROW, "Test.txt", numbers[0], numbers[1]);
    }
    if (k == 2) {
        cout << "Введите количество строк:" << endl;
        cin >> rows;
        cout << "Введите количество столбцов:" << endl;
        cin >> cols;
        cout << "Введите количество нулей(например 0.5):" << endl;
        cin >> sparsity;
        vector<int> sparseMatrix = generateSparseMatrix(rows, cols, sparsity);
        cout << endl;
        if (isSparse(sparseMatrix, rows, cols) == true) {
            cout << "Данная матрица разреженная" << endl;
        }
        else {
            cout << "Данная матрица не является разреженной" << endl;
        }
        cout << "Вот ваша матрица:" << endl;
        printMatrix(sparseMatrix, rows, cols);
        cout << endl;
        if (isSparse(sparseMatrix, rows, cols) == true) {
            cout << "Данная матрица разреженная" << endl;
        }
        else {
            cout << "Данная матрица не является разреженной" << endl;
        }
        cout << "Вот зеркальная матриц:" << endl;
        auto start1 = chrono::high_resolution_clock::now();
        mirrorDiagonalColumnMajor(sparseMatrix, rows, cols);
        auto end1 = chrono::high_resolution_clock::now();
        auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count();
        cout << "Время выполения алгоритма: " << duration1 << " microseconds" << endl;
        printMatrix(sparseMatrix, rows, cols);
        cout << endl;
        cout << "Эта матрица была записана в файл Test.txt" << endl;
        convertToSparseMatrix(sparseMatrix, rows, cols, values, ROW, COL);
        writeSparseMatrixToFile(values, COL, ROW, "Test.txt", rows, cols);
    }
    if (k == 3) {
        int f;
        numbers = readFirstTwoNumbersFromFile("matrix.txt");
        vector<int> babs = readFirstTwoNumbersFromFile("Test.txt");
        vector<int> Matrix1 = convertToDenseMatrixFromFile("matrix.txt");
        vector<int> Matrix2 = convertToDenseMatrixFromFile("Test.txt");
        cout << "Если вы хотите сложить матрицу из файла matrix.txt с Test.txt. Напишите 1 " << endl;
        cout << "Если вы хотите вычесть из матрицы из файла matrix.txt матрицу из Test.txt. Напишите 2" << endl;
        cout << "Если вы хотите вычесть из матрицы из файла Test.txt матрицу из matrix.txt. Напишите 3" << endl;
        cout << "Если вы хотите умножить матрицу из файла matrix.txt на матрицу из Test.txt. Напишите 4" << endl;
        cout << "Если вы хотите умножить матрицу из файла Text.txt на матрицу из matrix.txt. Напишите 5" << endl;
        cin >> f;
        if (f == 1) {
            cout << "Матрица 1:" << endl;
            printMatrix(Matrix1, numbers[0], numbers[1]);
            cout << endl;
            cout << "Матрица 2:" << endl;
            printMatrix(Matrix2, babs[0], babs[1]);
            cout << endl;
            cout << "Результатом их сложения будет:" << endl;
            Matrix1 = addMatrices(Matrix1, Matrix2, numbers[0], numbers[1]);
            printMatrix(Matrix1, numbers[0], numbers[1]);
        }
        else if (f == 2) {
            cout << "Матрица 1:" << endl;
            printMatrix(Matrix1, numbers[0], numbers[1]);
            cout << endl;
            cout << "Матрица 2:" << endl;
            printMatrix(Matrix2, babs[0], babs[1]);
            cout << endl;
            cout << "Результатом их вычитания будет:" << endl;
            Matrix1 = subtractMatrices(Matrix1, Matrix2, numbers[0], numbers[1]);
            printMatrix(Matrix1, numbers[0], numbers[1]);
        }
        else if (f == 3) {
            cout << "Матрица 1:" << endl;
            printMatrix(Matrix2, babs[0], babs[1]);
            cout << endl;
            cout << "Матрица 2:" << endl;
            printMatrix(Matrix1, numbers[0], numbers[1]);
            cout << endl;
            cout << "Результатом их вычитания будет:" << endl;
            Matrix2 = subtractMatrices(Matrix2, Matrix1, babs[0], babs[1]);
            printMatrix(Matrix2, babs[0], babs[1]);
        }
        else if (f == 5) {
            cout << "Матрица 1:" << endl;
            printMatrix(Matrix1, numbers[0], numbers[1]);
            cout << endl;
            cout << "Матрица 2:" << endl;
            printMatrix(Matrix2, babs[0], babs[1]);
            cout << endl;
            if (numbers[1] != babs[0]) {
                cout << "Матрицы несовместимы для умножения." << endl;
            }
            else {
                vector<int> resultMatrix = multiplyMatrices(Matrix1, Matrix2, numbers[0], numbers[1], babs[1]);
                cout << "Результат умножения матриц:" << endl;
                printMatrix(resultMatrix, numbers[0], babs[1]);
            }
        }
        else if (f == 6) {
            cout << "Матрица 1:" << endl;
            printMatrix(Matrix2, babs[0], babs[1]);
            cout << endl;
            cout << "Матрица 2:" << endl;
            printMatrix(Matrix1, numbers[0], numbers[1]);
            cout << endl;
            if (babs[1] != numbers[0]) {
                cout << "Матрицы несовместимы для умножения." << endl;
            }
            else {
                vector<int> resultMatrix = multiplyMatrices(Matrix2, Matrix1, babs[0], babs[1], numbers[1]);
                cout << "Результат умножения матриц:" << endl;
                printMatrix(resultMatrix, babs[0], numbers[1]);
            }
        }
    }
    return 0;
}
