#include "GeneralVNS.h"

void Clusters::clustering(int n) {
    clustersNum = n;
    for (int i = 0; i < input_matrix.rows_number; i++) {
        m.push_back(i % n + 1);
    }
    for (int i = 0; i < input_matrix.columns_number; i++) {
        p.push_back(i % n + 1);
    }
    groupingEfficacy();
}

void Clusters::groupingEfficacy() {
    float number_one = 0;
    float number_zero = 0;
    for (int i = 0; i < input_matrix.rows_number; i++) {
        for (int j = 0; j < input_matrix.columns_number; j++) {
            if (m[i] == p[j]) {
                if (input_matrix.matrix[i][j] == 1)
                    number_one++;
                else
                    number_zero++;
            }
        }
    }
    efficacy = number_one / (input_matrix.ones_number + number_zero);
}

bool is_good(Clusters init) {
    std::vector<int> clusters_arr(init.clustersNum, 0);
    for (int i = 0; i < init.m.size(); i++) {
        for (int j = 0; j < init.p.size(); j++) {
            if (init.m[i] == init.p[j])
              clusters_arr[init.m[i] - 1] = 1;
        }
    }
    if (std::find(clusters_arr.begin(), clusters_arr.end(), 0) != clusters_arr.end())
        return true;
    return false;
}

Clusters moveColumns(Clusters init) {
    Clusters result = init;
    for (int i = 0; i < result.p.size(); i++) {
        for (int j = 0; j < result.p.size(); j++) {
            Clusters new_result = result;
            int temp = new_result.p[i];
            new_result.p[i] = new_result.p[j];
            new_result.p[j] = temp;
            new_result.groupingEfficacy();
            if (result.efficacy < new_result.efficacy && is_good(new_result)) {
                result = new_result;
            }
        }
    }
    result.groupingEfficacy();
    return result;
}

Clusters moveRows(Clusters init) {
    Clusters result = init;
    for (int i = 0; i < result.m.size(); i++) {
        for (int j = 0; j < result.m.size(); j++) {
            Clusters new_result = result;
            int temp = new_result.m[i];
            new_result.m[i] = new_result.m[j];
            new_result.m[j] = temp;
            new_result.groupingEfficacy();
            if (result.efficacy < new_result.efficacy && is_good(new_result)) {
                result = new_result;
            }
        }
    }
    result.groupingEfficacy();
    return result;
}

Clusters merge(Clusters init) {
    Clusters result = init;
    int n = 0, boolean = 0;
    while ((n < 1000)&&(boolean == 0)&&(init.clustersNum > 1)) {
        int f_cluster = rand() % init.clustersNum + 1;
        int s_cluster = rand() % init.clustersNum + 1;
        if (f_cluster == s_cluster) {
            while (f_cluster != s_cluster) {
                s_cluster = rand() % init.clustersNum + 1;
            }
        }
        for (int i = 0; i < result.m.size(); i++) {
            if (result.m[i] == s_cluster) {
                result.m[i] = f_cluster;
            }
            if (result.m[i] == init.clustersNum) {
                result.m[i] = s_cluster;
            }
        }
        for (int i = 0; i < result.p.size(); i++) {
            if (result.p[i] == s_cluster) {
                result.p[i] = f_cluster;
            }
            if (result.p[i] == init.clustersNum) {
                result.p[i] = s_cluster;
            }
        }
        boolean = is_good(result);
        n++;
    }
    if (result.efficacy != init.efficacy)
        result.clustersNum--;
    result.groupingEfficacy();
    return result;
}

Clusters division(Clusters init) {
    Clusters result = init;
    int n = 0, boolean = 0;
    while ((n < 1000)&&(boolean == 0)) {
        result = init;
        int new_cluster = init.clustersNum + 1;
        for (int i = 0; i < rand()%init.m.size() / 2; i++) {
            int rand_m = rand()%init.m.size();
            result.m[rand_m] = new_cluster;
        }
        for (int i = 0; i < rand()%init.p.size() / 2; i++) {
            int rand_p = rand()%init.p.size();
            result.p[rand_p] = new_cluster;
        }
        boolean = is_good(result);
        n++;
    }
    if (result.efficacy != init.efficacy)
        result.clustersNum++;
    result.groupingEfficacy();
    return result;
}

Clusters shaking(Clusters prev_sol, int i) {
    Clusters result = prev_sol;
    switch (i) {
        case 0:
            result = division(result);
            break;
        case 1:
            result = merge(result);
            break;
    }
    return result;
}

Clusters vnd(Clusters prev_sol, int i) {
    Clusters result = prev_sol;
    switch (i) {
        case 0:
            result = moveColumns(result);
            break;
        case 1:
            result = moveRows(result);
            break;
    }
    return result;
}

Clusters GeneralVNS(Matrix* matrix, int k) {
    int j_max = 100, j = 0;
    int sf_num = 0, vnd_num = 0;
    Clusters init_solution(*matrix);
    init_solution.clustering(k);
    while (j <= j_max) {
        j++;
        sf_num = 0;
        while (sf_num < 2) {
            Clusters new_sol = shaking(init_solution, sf_num);
            vnd_num = 0;
            while (vnd_num < 2) {
                Clusters new_sol_second = vnd(new_sol, vnd_num);
                vnd_num++;
                if (new_sol_second.efficacy > new_sol.efficacy) {
                    new_sol = new_sol_second;
                    vnd_num = 0;
                }
            }
            sf_num++;
            if (new_sol.efficacy > init_solution.efficacy) {
                init_solution = new_sol;
                sf_num = 0;
                j = 0;
            }
        }
    }
    return init_solution;
}
