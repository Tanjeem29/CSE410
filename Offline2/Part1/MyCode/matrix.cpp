#include<iostream>
#include<vector>

using namespace std;

class MyMatrix {
    private:
    vector<vector<int>> matrix;
    int rows;
    int cols;

    public:
    MyMatrix(int rows, int cols, int initValue) {
        this->rows = rows;
        this->cols = cols;
        matrix.resize(rows);
        for (int i = 0; i < rows; i++) {
            matrix[i].resize(cols);
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = initValue;
            }
        }
    }

    int getElement(int row, int col) {
        return matrix[row][col];
    }

    void setElement(int row, int col, int value) {
        matrix[row][col] = value;
    }

    MyMatrix add(MyMatrix other) {
        MyMatrix result(rows, cols, 0);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; i++) {
                result.setElement(i, j, matrix[i][j] + other.getElement(i, j));
            }
        }
        return result;
    }

    MyMatrix multiply(MyMatrix other) {
        MyMatrix result(rows, other.cols, 0);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    result.setElement(i, j, result.getElement(i, j) + matrix[i][k] * other.getElement(k, j));
                }
            }
        }
        return result;
    }

    void printMatrix(){
        for (int i = 0; i < rows; i++) {
            cout << "[";
            for (int j = 0; j < cols; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << "]" << endl;
        }
    }

};