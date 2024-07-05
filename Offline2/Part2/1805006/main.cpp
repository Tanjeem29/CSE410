#include <iostream>
#include <stdio.h>
#include "matrix.cpp"
using namespace std;
#include <fstream>
#include <stack>
#include "./bitmap_image.hpp"

MyMatrix eye = MyMatrix(4, 1, 1);
MyMatrix look = MyMatrix(4, 1, 1);
MyMatrix up = MyMatrix(4, 1, 1);
double fovY, aspectRatio, near, far, fovX, t_proj, r_proj;
stack<MyMatrix> matStack;
MyMatrix currTrxMat = MyMatrix(4, 4, 0);
double ***zbuffer;
double screen_width, screen_height;
double zmin, zmax;
class triangle
{
public:
    MyMatrix v1 = MyMatrix(4, 1, 1);
    MyMatrix v2 = MyMatrix(4, 1, 1);
    MyMatrix v3 = MyMatrix(4, 1, 1);
    int color[3];

    void printTriangle()
    {
        // cout<<"Triangle"<<endl;
        printf("%.7f, %.7f, %.7f\n", v1.getElement(0, 0), v1.getElement(1, 0), v1.getElement(2, 0));
        printf("%.7f, %.7f, %.7f\n", v2.getElement(0, 0), v2.getElement(1, 0), v2.getElement(2, 0));
        printf("%.7f, %.7f, %.7f\n", v3.getElement(0, 0), v3.getElement(1, 0), v3.getElement(2, 0));
        cout << endl;

        // for(int i=0;i<3;i++){
        //     cout<<v1.getElement(i,0)<<"\t";
        // }
        // cout<<endl;
        // for(int i=0;i<3;i++){
        //     cout<<v2.getElement(i,0)<<"\t";
        // }
        // cout<<endl;
        // for(int i=0;i<3;i++){
        //     cout<<v3.getElement(i,0)<<"\t";
        // }
        // cout<<endl;
    }

    void printTriangle2(FILE *fp)
    {
        if (fp == nullptr)
        {
            std::cout << "Invalid file pointer." << std::endl;
            return;
        }

        // Set the precision to 7 decimal places
        fprintf(fp, "%.7f %.7f %.7f\n", v1.getElement(0, 0), v1.getElement(1, 0), v1.getElement(2, 0));
        fprintf(fp, "%.7f %.7f %.7f\n", v2.getElement(0, 0), v2.getElement(1, 0), v2.getElement(2, 0));
        fprintf(fp, "%.7f %.7f %.7f\n", v3.getElement(0, 0), v3.getElement(1, 0), v3.getElement(2, 0));
        fprintf(fp, "\n");
        return;
    }

    void assigncolor(int r, int g, int b)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void printColor()
    {
        cout << color[0] << " " << color[1] << " " << color[2] << endl;
    }

    // COMMENT
    // bool isInside(double x, double y)
    // {
    //     double x1 = v1.getElement(0, 0);
    //     double y1 = v1.getElement(1, 0);
    //     double x2 = v2.getElement(0, 0);
    //     double y2 = v2.getElement(1, 0);
    //     double x3 = v3.getElement(0, 0);
    //     double y3 = v3.getElement(1, 0);

    //     double a = (x1 - x) * (y2 - y1) - (x2 - x1) * (y1 - y);
    //     double b = (x2 - x) * (y3 - y2) - (x3 - x2) * (y2 - y);
    //     double c = (x3 - x) * (y1 - y3) - (x1 - x3) * (y3 - y);

    //     if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
    //     {
    //         return true;
    //     }
    //     return false;
    // }
};

vector<triangle> modelTransformedTriangles;
vector<triangle> viewTransformedTriangles;
vector<triangle> projTransformedTriangles;
vector<triangle> coloredTriangles;

void viewTransformAllTriangles(MyMatrix viewMat)
{
    triangle temp;
    viewTransformedTriangles.clear();
    for (triangle t : modelTransformedTriangles)
    {
        temp.v1 = viewMat.multiply(t.v1);
        temp.v2 = viewMat.multiply(t.v2);
        temp.v3 = viewMat.multiply(t.v3);
        // temp.printTriangle();
        viewTransformedTriangles.push_back(temp);
    }
}

void projTransformAllTriangles(MyMatrix projMat)
{
    triangle temp;
    projTransformedTriangles.clear();
    for (triangle t : viewTransformedTriangles)
    {
        temp.v1 = projMat.multiply(t.v1);
        // cout<<"v1: "<<temp.v1.getElement(3,0)<<endl;
        temp.v1 = temp.v1.scalarMultiply(1.0 / temp.v1.getElement(3, 0));

        temp.v2 = projMat.multiply(t.v2);
        // cout<<"v2: "<<temp.v2.getElement(3,0)<<endl;
        temp.v2 = temp.v2.scalarMultiply(1.0 / temp.v2.getElement(3, 0));
        temp.v3 = projMat.multiply(t.v3);
        // cout<<"v3: "<<temp.v3.getElement(3,0)<<endl;
        temp.v3 = temp.v3.scalarMultiply(1.0 / temp.v3.getElement(3, 0));
        // temp.printTriangle();
        projTransformedTriangles.push_back(temp);
    }
}

void printModelTransformedTriangles()
{
    cout << "Model Transformed:" << endl;
    for (triangle t : modelTransformedTriangles)
    {
        t.printTriangle();
    }
    cout << endl;
}

void printModelTransformedTrianglesToFile(FILE *fp)
{
    // cout<<"Model Transformed:"<<endl;
    for (triangle t : modelTransformedTriangles)
    {
        t.printTriangle2(fp);
    }
    // cout<<endl;
}

void printViewTransformedTriangles()
{
    cout << "View Transformed:" << endl;
    for (triangle t : viewTransformedTriangles)
    {
        t.printTriangle();
    }
    cout << endl;
}

void printViewTransformedTrianglesToFile(FILE *fp)
{
    // cout<<"View Transformed:"<<endl;
    for (triangle t : viewTransformedTriangles)
    {
        t.printTriangle2(fp);
    }
    // cout<<endl;
}

void printProjTransformedTriangles()
{
    cout << "Projection Transformed:" << endl;
    for (triangle t : projTransformedTriangles)
    {
        t.printTriangle();
    }
    cout << endl;
}

void printProjTransformedTrianglesToFile(FILE *fp)
{
    // cout<<"Projection Transformed:"<<endl;
    for (triangle t : projTransformedTriangles)
    {
        t.printTriangle2(fp);
    }
    // cout<<endl;
}

static unsigned long int g_seed = 1;
inline int randomColor()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

int getRandomColor()
{
    return randomColor() % 256;
    // return randomColor();
}

void colorTriangles()
{
    coloredTriangles.clear();
    for (triangle t : projTransformedTriangles)
    {
        int r = getRandomColor();
        int g = getRandomColor();
        int b = getRandomColor();
        t.assigncolor(r, g, b);
        coloredTriangles.push_back(t);
    }
    return;
}

void printTriangleColors()
{
    for (triangle t : coloredTriangles)
    {
        t.printColor();
    }
}

void initZbuffer(int screen_height, int screen_width, int zm, int r, int g, int b)
{
    zbuffer = new double **[screen_height];
    for (int i = 0; i < screen_height; ++i)
    {
        zbuffer[i] = new double *[screen_width];
        for (int j = 0; j < screen_width; ++j)
        {
            zbuffer[i][j] = new double[4];
            zbuffer[i][j][3] = zm;
            zbuffer[i][j][0] = r;
            zbuffer[i][j][1] = g;
            zbuffer[i][j][2] = b;
        }
    }
}

void freeZbuffer(int screen_height, int screen_width)
{
    for (int i = 0; i < screen_height; ++i)
    {
        for (int j = 0; j < screen_width; ++j)
        {
            delete[] zbuffer[i][j];
        }
        delete[] zbuffer[i];
    }
    delete[] zbuffer;
}

int main()
{
    // cout<<"Hello World!"<<endl;
    currTrxMat.initDiagonal(1);
    // currTrxMat.printMatrix();

    // string filename = "./testCases/1/scene.txt";
    string filename1 = "./scene.txt";
    ifstream file(filename1);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return 0;
    }
    double x, y, z;
    file >> x >> y >> z;
    // cout<<x<<" "<<y<<" "<<z<<endl;;
    eye.setElement(0, 0, x);
    eye.setElement(1, 0, y);
    eye.setElement(2, 0, z);
    // eye.printMatrix();
    // cout<<endl;

    file >> x >> y >> z;
    // cout<<x<<" "<<y<<" "<<z<<endl;;
    look.setElement(0, 0, x);
    look.setElement(1, 0, y);
    look.setElement(2, 0, z);
    // look.printMatrix();
    // cout<<endl;

    file >> x >> y >> z;
    // cout<<x<<" "<<y<<" "<<z<<endl;;
    up.setElement(0, 0, x);
    up.setElement(1, 0, y);
    up.setElement(2, 0, z);
    // up.printMatrix();
    // cout<<endl;

    file >> fovY >> aspectRatio >> near >> far;
    fovY *= (M_PI / 180);
    // cout<<fovY<<" "<<aspectRatio<<" "<<near<<" "<<far<<endl;

    string cmd;
    // file>>cmd;
    // while(cmd != "end"){
    //     cout<<cmd<<endl;
    //     file>>cmd;
    // }

    modelTransformedTriangles.clear();
    while (1)
    {
        file >> cmd;
        if (cmd == "end")
        {
            break;
        }
        else if (cmd == "triangle")
        {
            // cout<<"------------"<<endl;
            // cout<<"Triangle"<<endl;
            MyMatrix p1 = MyMatrix(4, 1, 1);
            MyMatrix p2 = MyMatrix(4, 1, 1);
            MyMatrix p3 = MyMatrix(4, 1, 1);
            triangle new_tri;

            file >> x >> y >> z;
            new_tri.v1.setElement(0, 0, x);
            new_tri.v1.setElement(1, 0, y);
            new_tri.v1.setElement(2, 0, z);

            file >> x >> y >> z;
            new_tri.v2.setElement(0, 0, x);
            new_tri.v2.setElement(1, 0, y);
            new_tri.v2.setElement(2, 0, z);

            file >> x >> y >> z;
            new_tri.v3.setElement(0, 0, x);
            new_tri.v3.setElement(1, 0, y);
            new_tri.v3.setElement(2, 0, z);

            // new_tri.v1.printMatrix();
            // new_tri.v2.printMatrix();
            // new_tri.v3.printMatrix();
            // cout<<"------------"<<endl;
            // new_tri.printTriangle();
            new_tri.v1 = currTrxMat.multiply(new_tri.v1);
            new_tri.v2 = currTrxMat.multiply(new_tri.v2);
            new_tri.v3 = currTrxMat.multiply(new_tri.v3);
            // new_tri.printTriangle();
            modelTransformedTriangles.push_back(new_tri);
        }
        else if (cmd == "translate")
        {
            // cout<<"------------"<<endl;
            // cout<<"Translate"<<endl;
            MyMatrix t = MyMatrix(4, 4, 0);
            t.initDiagonal(1);
            file >> x >> y >> z;
            t.setElement(0, 3, x);
            t.setElement(1, 3, y);
            t.setElement(2, 3, z);
            // t.printMatrix();
            // cout<<"-----trans1------"<<endl;
            // currTrxMat.printMatrix();
            // cout<<"-----trans2------"<<endl;
            // currTrxMat = t.multiply(currTrxMat);
            currTrxMat = currTrxMat.multiply(t);
            // currTrxMat.printMatrix();
            // cout<<"-----trans3------"<<endl;
        }
        else if (cmd == "scale")
        {
            // cout<<"------------"<<endl;
            // cout<<"Scale"<<endl;
            MyMatrix s = MyMatrix(4, 4, 0);
            s.initDiagonal(1);
            file >> x >> y >> z;
            s.setElement(0, 0, x);
            s.setElement(1, 1, y);
            s.setElement(2, 2, z);
            // s.printMatrix();
            // cout<<"------------"<<endl;
            currTrxMat = currTrxMat.multiply(s);
        }
        else if (cmd == "rotate")
        {
            double theta;
            MyMatrix r = MyMatrix(4, 4, 0);
            r.initDiagonal(1);
            file >> theta >> x >> y >> z;
            theta = theta * M_PI / 180.0;
            // cout<<"Rotate"<<endl;
            MyMatrix i = MyMatrix(4, 1, 0);
            MyMatrix j = MyMatrix(4, 1, 0);
            MyMatrix k = MyMatrix(4, 1, 0);
            i.setElement(0, 0, 1);
            j.setElement(1, 0, 1);
            k.setElement(2, 0, 1);
            i.setElement(3, 0, 1);
            j.setElement(3, 0, 1);
            k.setElement(3, 0, 1);

            MyMatrix a = MyMatrix(4, 1, 0);
            a.setElement(0, 0, x);
            a.setElement(1, 0, y);
            a.setElement(2, 0, z);
            a.setElement(3, 0, 1);
            // cout<<"Rotate2"<<endl;
            // a = a.normalize();

            MyMatrix c1 = i.RodriguesFormula(i, a, theta);

            MyMatrix c2 = j.RodriguesFormula(j, a, theta);
            MyMatrix c3 = k.RodriguesFormula(k, a, theta);
            // cout<<"Rotate3"<<endl;
            // i.printMatrix();
            // c1.printMatrix();3
            // j.printMatrix();
            // c2.printMatrix();
            // k.printMatrix();
            // c3.printMatrix();
            MyMatrix result = MyMatrix(4, 4, 0);
            for (int i = 0; i < 4; i++)
            {
                result.setElement(i, 0, c1.getElement(i, 0));
                result.setElement(i, 1, c2.getElement(i, 0));
                result.setElement(i, 2, c3.getElement(i, 0));
            }
            result.setElement(3, 3, 1);

            // cout<<"rot1"<<endl;
            // currTrxMat.printMatrix();
            // cout<<"rot2"<<endl;
            // result.printMatrix();
            currTrxMat = currTrxMat.multiply(result);
            // cout<<"rot3"<<endl;
            // currTrxMat.printMatrix();
        }
        else if (cmd == "push")
        {
            matStack.push(currTrxMat);
            // cout<<"push"<<endl;
        }
        else if (cmd == "pop")
        {
            currTrxMat = matStack.top();
            matStack.pop();
            // cout<<"pop"<<endl;
        }
        else
        {
            cout << "Invalid command: " << cmd << endl;
        }
    }

    MyMatrix l = MyMatrix(4, 1, 1);
    l.setElement(0, 0, (look.getElement(0, 0) - eye.getElement(0, 0)));
    l.setElement(1, 0, (look.getElement(1, 0) - eye.getElement(1, 0)));
    l.setElement(2, 0, (look.getElement(2, 0) - eye.getElement(2, 0)));
    l = l.normalize();

    MyMatrix r = l.crossProduct(up);
    r = r.normalize();

    MyMatrix u = r.crossProduct(l);

    // cout<<"l:"<<endl;
    // l.printMatrix();

    // cout<<"r: "<<endl;
    // r.printMatrix();

    // cout<<"u: "<<endl;
    // u.printMatrix();

    MyMatrix T = MyMatrix(4, 4, 0);
    T.initDiagonal(1);
    T.setElement(0, 3, -eye.getElement(0, 0));
    T.setElement(1, 3, -eye.getElement(1, 0));
    T.setElement(2, 3, -eye.getElement(2, 0));
    // cout<<"T:"<<endl;
    // T.printMatrix();

    MyMatrix RotMat = MyMatrix(4, 4, 0);
    RotMat.setElement(0, 0, r.getElement(0, 0));

    RotMat.setElement(0, 1, r.getElement(1, 0));

    RotMat.setElement(0, 2, r.getElement(2, 0));

    RotMat.setElement(1, 0, u.getElement(0, 0));

    RotMat.setElement(1, 1, u.getElement(1, 0));

    RotMat.setElement(1, 2, u.getElement(2, 0));

    RotMat.setElement(2, 0, -(l.getElement(0, 0)));

    RotMat.setElement(2, 1, -(l.getElement(1, 0)));

    RotMat.setElement(2, 2, -(l.getElement(2, 0)));

    RotMat.setElement(3, 3, 1);

    // cout<<"RotMat:"<<endl;
    // RotMat.printMatrix();

    MyMatrix v = RotMat.multiply(T);

    // cout<<"V"<<endl;
    // v.printMatrix();
    // MyMatrix t = MyMatrix(4,1,1);
    // t.setElement(0,0,3);
    // t.setElement(1,0,-3);
    // t.setElement(2,0,1);

    // MyMatrix s = MyMatrix(4,1,1);
    // s.setElement(0,0,4);
    // s.setElement(1,0,9);
    // s.setElement(2,0,2);

    // MyMatrix t2 = MyMatrix(4,1,1);
    // t2.setElement(0,0,1);
    // t2.setElement(1,0,2);
    // t2.setElement(2,0,3);

    // MyMatrix s2 = MyMatrix(4,1,1);
    // s2.setElement(0,0,4);
    // s2.setElement(1,0,-5);
    // s2.setElement(2,0,6);

    // cout<<"Test"<<endl;
    // // t.crossProduct(s).printMatrix();
    // cout<<t.dotProduct(s)<<endl;
    // s.printMatrix();
    // t.printMatrix();

    // cout<<"Test2"<<endl;
    // // t2.crossProduct(s2).printMatrix();
    // cout<<t2.dotProduct(s2)<<endl;
    // s2.printMatrix();
    // t2.printMatrix();

    viewTransformAllTriangles(v);

    // printModelTransformedTriangles();
    // printViewTransformedTriangles();

    fovX = fovY * aspectRatio;
    t_proj = near * tan(fovY / 2);
    r_proj = near * tan(fovX / 2);

    MyMatrix projMat = MyMatrix(4, 4, 0);
    projMat.setElement(0, 0, near / r_proj);
    projMat.setElement(1, 1, near / t_proj);
    projMat.setElement(1, 1, near / t_proj);
    projMat.setElement(2, 2, -(far + near) / (far - near));
    projMat.setElement(2, 3, -(2 * far * near) / (far - near));
    projMat.setElement(3, 2, -1);

    // cout<<"ProjMat:"<<endl;
    // projMat.printMatrix();
    // cout<<"near: "<<near<<", far: "<<far<<", t_proj: "<<t_proj<<", r_proj: "<<r_proj<<", fovY: "<<fovY<<", fovX: "<<fovX<<", aspectRatio: "<<aspectRatio<<endl;

    projTransformAllTriangles(projMat);
    // printProjTransformedTriangles();

    FILE *fp_stage1 = std::fopen("stage1.txt", "w");
    printModelTransformedTrianglesToFile(fp_stage1);

    FILE *fp_stage2 = std::fopen("stage2.txt", "w");
    printViewTransformedTrianglesToFile(fp_stage2);

    FILE *fp_stage3 = std::fopen("stage3.txt", "w");
    printProjTransformedTrianglesToFile(fp_stage3);

    //-----------------Part2-------------------------
    // for (int i = 0; i < 100; i++)
    //     cout << getRandomColor() << endl;

    // cout << "---------------------" << endl;
    string filename2 = "./config.txt";
    ifstream file2(filename2);
    if (!file2.is_open())
    {
        cout << "Error opening file" << endl;
        return 0;
    }

    file2 >> screen_width >> screen_height;

    colorTriangles();
    // printTriangleColors();

    double uplimit = 1, downlimit = -1, leftlimit = -1, rightlimit = 1;
    double dx = (rightlimit - leftlimit) / screen_width;
    double dy = (uplimit - downlimit) / screen_height;
    double leftX = leftlimit + dx / 2, topY = uplimit - dy / 2;
    zmin = -1, zmax = 1;
    // cout << dx << " " << dy << endl;
    // cout << leftX << " " << topY << endl;

    // zbuffer->[i][j[k] ->  i-> row/y , j-> column/x  ;k-> [0] = r, [1] = g, [2] = b, [3] = z
    initZbuffer(screen_height, screen_width, zmax, 0, 0, 0);

    // triangle temp;
    // temp.v1.setElement(0, 0, 0);
    // temp.v1.setElement(1, 0, 0);
    // temp.v1.setElement(2, 0, .98);

    // temp.v2.setElement(0, 0, 0.12);
    // temp.v2.setElement(1, 0, 0);
    // temp.v2.setElement(2, 0, .98);

    // temp.v3.setElement(0, 0, 0);
    // temp.v3.setElement(1, 0, 0.12);
    // temp.v3.setElement(2, 0, 0.98);

    // temp.assigncolor(255, 0, 0);
    // COMMENT
    //  for(int i = 0; i< screen_height;i++){
    //      for(int j=0;j<screen_width;j++){
    //          if(temp.isInside(leftX + j * dx, topY - i * dy)){
    //              zbuffer[i][j][0] = temp.color[0];
    //              zbuffer[i][j][1] = temp.color[1];
    //              zbuffer[i][j][2] = temp.color[2];
    //              zbuffer[i][j][3] = temp.v1.getElement(2,0);
    //          }
    //      }
    //  }
    // int tcount = 0, tc2 = 0;
    for (triangle temp : coloredTriangles)
    {
        // tcount++;
        // tc2++;

        double planea = 0, planeb = 0, planec = 0, planed = 0;

        MyMatrix side1(4, 1, 1);
        MyMatrix side2(4, 1, 1);
        side1.setElement(0, 0, temp.v2.getElement(0, 0) - temp.v1.getElement(0, 0));
        side1.setElement(1, 0, temp.v2.getElement(1, 0) - temp.v1.getElement(1, 0));
        side1.setElement(2, 0, temp.v2.getElement(2, 0) - temp.v1.getElement(2, 0));
        side2.setElement(0, 0, temp.v3.getElement(0, 0) - temp.v1.getElement(0, 0));
        side2.setElement(1, 0, temp.v3.getElement(1, 0) - temp.v1.getElement(1, 0));
        side2.setElement(2, 0, temp.v3.getElement(2, 0) - temp.v1.getElement(2, 0));

        MyMatrix normal = side1.crossProduct(side2);
        planea = normal.getElement(0, 0);
        planeb = normal.getElement(1, 0);
        planec = normal.getElement(2, 0);
        planed = -(planea * temp.v1.getElement(0, 0) + planeb * temp.v1.getElement(1, 0) + planec * temp.v1.getElement(2, 0));

        double y1 = temp.v1.getElement(1, 0);
        double y2 = temp.v1.getElement(1, 0);
        if (temp.v2.getElement(1, 0) < y1)
            y1 = temp.v2.getElement(1, 0);
        if (temp.v2.getElement(1, 0) > y2)
            y2 = temp.v2.getElement(1, 0);

        if (temp.v3.getElement(1, 0) < y1)
            y1 = temp.v3.getElement(1, 0);
        if (temp.v3.getElement(1, 0) > y2)
            y2 = temp.v3.getElement(1, 0);

        int row1 = ceil((topY - y1) / dy);
        int row2 = floor((topY - y2) / dy);
        // cout << row1 << " " << row2 << endl;
        if (row1 > screen_height - 1)
            row1 = screen_height - 1;
        if (row2 < 0)
            row2 = 0;

        for (int i = row2; i <= row1; i++)
        {

            // lines intersecting = 1-2 and 2-3

            int flag = 0;
            MyMatrix vertex1 = temp.v1;
            MyMatrix vertex2 = temp.v2;
            MyMatrix vertex3 = temp.v3;
            double ys = topY - i * dy;

            if (temp.v1.getElement(1, 0) == temp.v2.getElement(1, 0))
            {
                flag = 1;
                vertex1 = temp.v1;
                vertex2 = temp.v3;
                vertex3 = temp.v2;
            }
            else if (temp.v2.getElement(1, 0) == temp.v3.getElement(1, 0))
            {
                flag = 1;
                vertex1 = temp.v2;
                vertex2 = temp.v1;
                vertex3 = temp.v3;
            }
            else if (temp.v3.getElement(1, 0) == temp.v1.getElement(1, 0))
            {
                flag = 1;
                vertex1 = temp.v3;
                vertex2 = temp.v2;
                vertex3 = temp.v1;
            }
            else
            {
                vertex1 = temp.v1;
                vertex2 = temp.v2;
                vertex3 = temp.v3;
                double t12 = (ys - vertex1.getElement(1, 0)) / (vertex2.getElement(1, 0) - vertex1.getElement(1, 0));
                double t23 = (ys - vertex2.getElement(1, 0)) / (vertex3.getElement(1, 0) - vertex2.getElement(1, 0));
                double t31 = (ys - vertex3.getElement(1, 0)) / (vertex1.getElement(1, 0) - vertex3.getElement(1, 0));

                if (t12 >= 0 && t12 <= 1 && t23 >= 0 && t23 <= 1)
                {
                    vertex1 = temp.v1;
                    vertex2 = temp.v2;
                    vertex3 = temp.v3;
                }
                else if (t23 >= 0 && t23 <= 1 && t31 >= 0 && t31 <= 1)
                {
                    vertex1 = temp.v2;
                    vertex2 = temp.v3;
                    vertex3 = temp.v1;
                }
                else if (t31 >= 0 && t31 <= 1 && t12 >= 0 && t12 <= 1)
                {
                    vertex1 = temp.v3;
                    vertex2 = temp.v1;
                    vertex3 = temp.v2;
                }
                else
                {
                    // cout<<"Error2"<<endl;
                    continue;
                }
            }
            double t12 = (ys - vertex1.getElement(1, 0)) / (vertex2.getElement(1, 0) - vertex1.getElement(1, 0));
            double t23 = (ys - vertex2.getElement(1, 0)) / (vertex3.getElement(1, 0) - vertex2.getElement(1, 0));
            double t31 = (ys - vertex3.getElement(1, 0)) / (vertex1.getElement(1, 0) - vertex3.getElement(1, 0));
            if (!(t12 >= 0 && t12 <= 1 && t23 >= 0 && t23 <= 1))
                continue;
            // cout<<"Error"<<endl;
            // if (t31 >= 0 && t31 <= 1)
            //     cout << "Error" << endl;

            double xa = vertex1.getElement(0, 0) + t12 * (vertex2.getElement(0, 0) - vertex1.getElement(0, 0));
            double xb = vertex2.getElement(0, 0) + t23 * (vertex3.getElement(0, 0) - vertex2.getElement(0, 0));
            if (xa > xb)
            {
                double tempd = xa;
                xa = xb;
                xb = tempd;
            }

            int col1 = round((xa - leftX) / dx);
            int col2 = round((xb - leftX) / dx);
            if (col2 > screen_width - 1)
                col2 = screen_width - 1;
            if (col1 < 0)
                col1 = 0;

            for (int j = col1; j <= col2; j++)
            {
                double x = leftX + j * dx;
                double y = topY - i * dy;
                double z = (-planed - planea * x - planeb * y) / planec;
                if (z < zbuffer[i][j][3] && z >= zmin)
                {
                    // if (tcount < 100 && tc2 == 1)
                    // {
                    //     tcount++;
                    //     cout << tcount << " " << i << " " << j << " " << z << endl;
                    // }

                    zbuffer[i][j][0] = temp.color[0];
                    zbuffer[i][j][1] = temp.color[1];
                    zbuffer[i][j][2] = temp.color[2];
                    zbuffer[i][j][3] = z;
                }
                else
                    continue;
            }
        }
    }

    FILE *fp_zbuffer = std::fopen("z_buffer.txt", "w");
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            double tempd = zbuffer[i][j][3];
            if (tempd == zmax)
                // fprintf(fp_zbuffer, "\t\t");
                continue;
            else
                fprintf(fp_zbuffer, "%.6f\t", tempd);
        }
        fprintf(fp_zbuffer, "\n");
    }

    bitmap_image image(screen_width, screen_height);
    // start from topleft
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            image.set_pixel(j, i, zbuffer[i][j][0], zbuffer[i][j][1], zbuffer[i][j][2]);
        }
    }
    image.save_image("out.bmp");
    // cout << "test.bmp saved" << endl;
    freeZbuffer(screen_height, screen_width);
    image.clear();
    // cout << "Done" << endl;
    return 0;
}
