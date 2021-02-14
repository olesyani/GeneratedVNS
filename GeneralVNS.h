#ifndef GeneralVNS_h
#define GeneralVNS_h

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <stdlib.h>

class Matrix {
    friend class Clusters;
private:
    int rows_number = 0;
    int columns_number = 0;
    int ones_number = 0;
    std::vector<std::vector <char>> matrix;
public:
    Matrix(int r, int c, std::ifstream file) : rows_number{r}, columns_number{c} {
        matrix.assign(r, std::vector<char>(c, 0));
        std::string str;
        int i = 0;
        int column_index;
        getline(file, str);
        while (getline(file, str)) {
            int index = str.find(" ");
            str.erase(0, index + 1);
            while (str != "") {
                index = str.find(" ");
                if (index != -1) {
                    column_index = atoi(str.substr(0, index).c_str());
                    str.erase(0, index + 1);
                } else {
                    column_index = atoi(str.substr(0).c_str());
                    str.erase();
                }
                matrix[i][column_index - 1] = 1;
                ones_number++;
            }
            i++;
        }
    }
};

class Clusters {
    friend class Matrix;
private:
    Matrix input_matrix;
public:
    std::vector<int> m;
    std::vector<int> p;
    int clustersNum = 0;
    float efficacy = 0;
    explicit Clusters(Matrix matrix) : input_matrix{matrix} {}
    void clustering(int n);
    void groupingEfficacy();
};

bool is_good(Clusters init);
Clusters moveColumns(Clusters init);
Clusters moveRows(Clusters init);
Clusters merge(Clusters init);
Clusters division(Clusters init);
Clusters shaking(Clusters prev_sol, int i);
Clusters vnd(Clusters prev_sol, int i);
Clusters GeneralVNS(Matrix* matrix, int k);

#endif /* GeneralVNS_h */
