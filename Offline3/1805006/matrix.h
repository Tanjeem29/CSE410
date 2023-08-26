#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

class MyMatrix {
    private:
    vector<vector<double>> matrix;
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

    double getElement(int row, int col) {
        return matrix[row][col];
    }

    void setElement(int row, int col, double value) {
        matrix[row][col] = value;
    }

    MyMatrix add(MyMatrix other) {
        // cout<<"add"<<endl;
        // cout<<"This: "<<rows<<" "<<cols<<endl;  
        // cout<<"Other: "<<other.rows<<" "<<other.cols<<endl;
        MyMatrix result(rows, cols, 0);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // cout<<"i: "<<i<<" j: "<<j<<endl;
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

    void initDiagonal(double value){
        for(int i=0;i<rows;i++){
            matrix[i][i] = value;
        }
    }

    MyMatrix scalarMultiply(double value){
        MyMatrix result = MyMatrix(rows,cols,0);
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                result.setElement(i,j,matrix[i][j]*value);
            }
        }
        result.setElement(rows-1,cols-1,1);
        return result;
    }

    double dotProduct(MyMatrix other){
        double ans = 0;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                ans+=matrix[i][j]*other.getElement(i,j);
            }
        }
        if(rows == 4 && matrix[rows-1][0] == 1) ans--;
        return ans; //-1 for last element +1 extra
    }

    MyMatrix crossProduct(MyMatrix other){
        MyMatrix result(rows,cols,1);
        result.setElement(0,0,matrix[1][0]*other.getElement(2,0) - matrix[2][0]*other.getElement(1,0));
        result.setElement(1,0,matrix[2][0]*other.getElement(0,0) - matrix[0][0]*other.getElement(2,0));
        result.setElement(2,0,matrix[0][0]*other.getElement(1,0) - matrix[1][0]*other.getElement(0,0));
        return result;
    }

    MyMatrix normalize(){
        MyMatrix result(rows,cols,1);
        double length = sqrt(matrix[0][0]*matrix[0][0] + matrix[1][0]*matrix[1][0] + matrix[2][0]*matrix[2][0]);
        result.setElement(0,0,matrix[0][0]/length);
        result.setElement(1,0,matrix[1][0]/length);
        result.setElement(2,0,matrix[2][0]/length);
        return result;
    }

    MyMatrix RodriguesFormula(MyMatrix x, MyMatrix a, double radAngle){
        // cout<<"RodriguesFormula"<<endl;
        // cout<<"x: "<<endl;
        // x.printMatrix();
        // cout<<"a: "<<endl;
        // a.printMatrix();
        // cout<<"radAngle: "<<radAngle<<endl;


        MyMatrix a2 = a.normalize();
        // cout<<"a2: "<<endl;
        // a2.printMatrix();

        MyMatrix t1 = x.scalarMultiply(cos(radAngle));
        // cout<<"t1: "<<endl;
        // t1.printMatrix();
        MyMatrix t2 = a2.scalarMultiply(1-cos(radAngle)).scalarMultiply(a2.dotProduct(x));
        // cout<<"t2: "<<endl;
        // t2.printMatrix();
        MyMatrix t3 = a2.crossProduct(x).scalarMultiply(sin(radAngle));
        // cout<<"t3: "<<endl;
        // t3.printMatrix();
        // cout<<"--------------"<<endl;
        // t2.printMatrix();
        // t3.printMatrix();

        MyMatrix result = t1.add(t2).add(t3);
        // cout<<"result: "<<endl;
        // result.printMatrix();
        result.setElement(3,0,0);
        result.setElement(3,1,0);
        result.setElement(3,2,0);
        return result;
    }

    double getDeterminant(){
        if(rows == 2){
            return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
        }
        else if(rows == 3){
            return matrix[0][0]*(matrix[1][1]*matrix[2][2] - matrix[1][2]*matrix[2][1]) - matrix[0][1]*(matrix[1][0]*matrix[2][2] - matrix[1][2]*matrix[2][0]) + matrix[0][2]*(matrix[1][0]*matrix[2][1] - matrix[1][1]*matrix[2][0]);
        }
        else{
            return 0;
        }
    }

};