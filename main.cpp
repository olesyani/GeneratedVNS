#include "GeneralVNS.h"

int main() {
    srand(time(NULL));
    std::ifstream file("/Users/lesyanikolaeva/Documents/corm37.txt");
    int rows, columns;
    file >> rows;
    file >> columns;
    Matrix matrix(rows, columns, std::move(file));
    file.close();
    Clusters solution = GeneralVNS(&matrix, rand()% std::min(rows, columns) + 1);
    for (int i = 0; i < solution.m.size(); i++) {
        std::cout << solution.m[i] << " ";
    }
    puts("");
    for (int i = 0; i < solution.p.size(); i++) {
        std::cout << solution.p[i] << " ";
    }
    puts("");
    std::cout << solution.efficacy << std::endl;
    return 0;
}
