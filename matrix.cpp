#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
//**********************
// Functions
int parse(int, double **);
void LUdecomp(double **array, double **L, double **U, int n);
int matrixDeterminant(double **matrixA, int n);
void inverseM(int n, double **y, double **x, double **L, double **U,
              double **b);

int main()
{
    int m; // where m = dimension of the square matrix
    double **matrixA;
    double **L;
    double **U;
    double **B;
    double **b;
    double **y;
    double **x;
    double **inverse;

    int n;    // where n represents rows
    double d; // d represents the determinant
    cout << "Enter the dimension of your square matrix: ";
    cin >> n;
    m = n;
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();
    // allocating matrices' memory
    matrixA = new double *[n];
    L = new double *[n];
    U = new double *[n];
    B = new double *[n];
    b = new double *[n];
    y = new double *[n];
    x = new double *[n];
    inverse = new double *[n];
    for (int i = 0; i < n; i++)
    {
        matrixA[i] = new double[n];
        L[i] = new double[n];
        U[i] = new double[n];
        B[i] = new double[n];
        b[i] = new double[1];
        x[i] = new double[n];
        y[i] = new double[n];
        inverse[i] = new double[n];
    }
    // r represents rows read from parse()
    int r;
    // parsing
    r = parse(n, matrixA);
    cout << "Here is the given matrix: \n";
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << matrixA[i][j] << "  ";
        }
        cout << endl;
    }
    // determinant check
    d = matrixDeterminant(matrixA, n);
    // cout << d;
    if (d == 0)
    {
        cout << "\nMatrix is singularar";
        return -1;
    }
    else
    {
        cout << "\nMatrix is non singular\n";
        LUdecomp(matrixA, L, U, n);
        // Printing results for L and U
        cout << "\nLower Matrix is::::\n";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << L[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nUpper Matrix is::::\n";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << U[i][j] << " ";
            }
            cout << endl;
        }
        // initializing identity matrix  B with values
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == j)
                    B[i][j] = 1;
                else
                    B[i][j] = 0;
            }
        }
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                b[j][0] = B[i][j];
            }
            inverseM(n, y, x, L, U, b);
            for (int k = 0; k < n; k++)
            {
                inverse[k][i] = x[k][0];
            }
        }
        cout << "\n The inverse of the matrix is: " << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << inverse[i][j] << "  ";
            }
            cout << endl;
        }
    }
    // Stop the timer
    auto stop = std::chrono::high_resolution_clock::now();
    // Compute the duration
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // Output the time taken
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    // freeing all dynamic memory
    for (int i = 0; i < n; i++)
    {
        delete[] matrixA[i];
        delete[] L[i];
        delete[] U[i];
        delete[] B[i];
        delete[] b[i];
        delete[] y[i];
    }
    delete[] matrixA;
    delete[] L;
    delete[] U;
    delete[] B;
    delete[] b;
    delete[] y;

    return 0;
}

//********************************************
// receives the size of the array as argument
int parse(int n, double **matrixA)
{
    string line;
    int i = 0;
    ifstream myfile("data.csv");
    if (!myfile)
    {
        cout << " File does not exist";
        return -1;
    }
    while (getline(myfile, line) && i < n)
    {
        string text;
        stringstream abc(line);
        int j = 0;
        while (getline(abc, text, ',') && j < n)
        {
            matrixA[i][j] = stoi(text);
            j++;
        }
        i++;
    }
    myfile.close();
    return i;
}
//*******************************************
// to find the determnant
int matrixDeterminant(double **matrixA, int n)
{
    double **temp = new double *[n];
    for (int i = 0; i < n; i++)
    {
        temp[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            temp[i][j] = matrixA[i][j];
        }
    }
    double d = 1;
    for (int i = 0; i < n; i++)
    {
        if (temp[i][i] == 0) // if pivot is 0
        {
            int j;
            for (j = i + 1; j < n; j++) // check other rows beneath
            {
                if (temp[j][i] != 0)
                {
                    break;
                }
            }
            if (j == n)
            {
                // if all rows are 0, determinant is 0
                return 0;
            }
            for (int k = i; k < n; k++)
            {
                // swap
                double temp2 = temp[i][k];
                temp[i][k] = temp[j][k];
                temp[j][k] = temp2;
            }
            d *= -1; // with a swap, sign has to change
        }
        for (int j = i + 1; j < n; j++)
        {
            int factor = temp[j][i] / temp[i][i];
            for (int k = i + 1; k < n; k++)
            {
                temp[j][k] -= factor * temp[i][k];
            }
        }
        d *= temp[i][i];
    }
    // free memory allocated for temp
    for (int i = 0; i < n; i++)
    {
        delete[] temp[i];
    }
    delete[] temp;
    return d;
}

//********************************************
// initializing L and U matrices
void LUdecomp(double **arrayA, double **L, double **U, int n)
{
    int i = 0, j = 0, k = 0;
    // initializing L with 1s at the diagonal
    // and 0s above the diagonal
    // initializing U with 0s below diagonal
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
                L[i][j] = 1;
            else if (j > i)
                L[i][j] = 0;
            else if (j < i)
                U[i][j] = 0;
        }
    }

    for (k = 0; k < n; k++)
    {
        U[k][k] = arrayA[k][k];
        for (i = k + 1; i < n; i++)
        {
            L[i][k] = arrayA[i][k] / U[k][k]; // for V
            // arrayA[i][k] =L[i][k];
            U[k][i] = arrayA[k][i];
        }
        //   A™
        for (i = k + 1; i < n; i++)
        {
            for (j = k + 1; j < n; j++)
            {
                arrayA[i][j] = arrayA[i][j] - L[i][k] * U[k][j];
            }
        }
    }
}
//********************************************
void inverseM(int n, double **y, double **x, double **L, double **U,
              double **b)
{
    // Solving LY=B for Y
    for (int i = 0; i < n; i++)
    {
        y[i][0] = b[i][0];
        for (int j = 0; j < i; j++)
        {
            y[i][0] -= L[i][j] * y[j][0];
        }
    }
    // Solving UX=Y for X
    for (int i = n - 1; i >= 0; i--)
    {
        x[i][0] = y[i][0];
        for (int j = i + 1; j < n; j++)
        {
            x[i][0] -= U[i][j] * x[j][0];
        }
        x[i][0] /= U[i][i];
    }
}
