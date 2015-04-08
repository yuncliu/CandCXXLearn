#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Matrix {
    int row;
    int col;
    double** m;
};

/**
 * @brief read matrix from file
 * @param[in]   f   file path
 * @param[out]  m   matrix
 * @retval 0    success
 * @retval -1   failed
 */
int init_matrix_from_file(struct Matrix* m, const char* f);

/**
 * @brief get matrix size and check
 * @param[in]   f   file path
 * @param[out]  m   matrix
 * @retval 0    success
 * @retval -1   failed
 */
int get_matrix_size(struct Matrix* m, const char* f);

/**
 * @brief printf matrix
 */
void print_matrix(struct Matrix* m);

/**
 * @brief get maximum element of one row
 * @param[in]   m       matrix
 * @param[in]   idx     row index
 * @param[out]  value   maximum value
 * @retval  0   success
 * @retval  -1  failed
 */
int get_max_of_row(struct Matrix* m, int idx, double* value);

/**
 * @brief get minimum element of one column
 * @param[in]   m       matrix
 * @param[in]   idx     column index
 * @param[out]  value   minimum value
 * @retval  0   success
 * @retval  -1  failed
 */
int get_min_of_col(struct Matrix* m, int idx, double* value);

/**
 * @brief get saddle point and print out;
 * @param[in] m     matrix
 * @retval 0    success
 * @retval -1   failed, the matrix don't have saddle point
 */
int get_saddle(struct Matrix* m);


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("argc error!\n");
        return -1;
    }
    struct Matrix m;
    if (0 == init_matrix_from_file(&m, argv[1])) {
        print_matrix(&m);
        get_saddle(&m);
    }
    return 0;
}

int get_matrix_size(struct Matrix* m, const char* f) {
    if (NULL == m || NULL == f) {
        return -1;
    }
    FILE* fp = fopen(f, "r");
    if (NULL == fp) {
        return -1;
    }
    m->col = 0;
    m->row = 0;
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;
    int ret = 0;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
        int col = 0;
        char* tmp = line;
        m->row++;
        while ((tmp = strtok(tmp, " ")) != NULL) {
            col++;
            tmp = NULL;
        }
        if (m->col !=0 && m->col != col) {
            printf("size don't match\n");
            ret = -1;
        }
        m->col = col;
    }
    fclose(fp);
    printf("%s finished %d %d\n", __FUNCTION__, m->row, m->col);
    return ret;
}

int init_matrix_from_file(struct Matrix* m, const char* f) {
    if (0 != get_matrix_size(m, f)) {
        return -1;
    }

    m->m = (double**)malloc(sizeof(double*) * m->row);
    int ii = 0;
    for (ii = 0; ii < m->row; ++ii) {
        m->m[ii] = malloc(sizeof(double) * m->col);
    }

    FILE* fp = fopen(f, "r");
    if (NULL == fp) {
        printf("can't open file!\n");
        return -1;
    }
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;
    int i = 0;
    int j = 0;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
        char* tmp = line;
        j = 0;
        while ((tmp = strtok(tmp, " ")) != NULL)
        {
            m->m[i][j] = atof(tmp); 
            tmp = NULL;
            j++;
        }
        i++;
    }
    fclose(fp);
    return 0;
}

void print_matrix(struct Matrix* m) {
    printf("The sizeof the matrix is (%d, %d) content is:\n", m->col, m->row);
    int i = 0;
    int j = 0;
    for (i = 0; i < m->row; ++i) {
        for (j = 0; j < m->col; ++j) {
            printf("%f ", m->m[i][j]);
        }
        printf("\n");
    }
}

int get_saddle(struct Matrix* m) {
    if (NULL == m) {
        return -1;
    }

    int ret = -1;
    int i = 0;
    for (i = 0; i < m->row; ++i) {
        double v1 = 0;
        double v2 = 0;
        int max_row = get_max_of_row(m, i ,&v1);
        int max_col = get_min_of_col(m, max_row ,&v2);
        if ( max_col == i && v1 == v2) {
            printf("pos (%d, %d) value = [%f]\n", max_col + 1, max_row + 1, v1);
            ret = 0;
        }
    }
    return ret;
}

int get_max_of_row(struct Matrix* m, int idx, double* value) {
    if (NULL == m || NULL == value || idx > m->row) {
        return -1;
    }

    int pos = 0;
    int i = 0;
    double tmp = 0;
    *value = m->m[idx][0];
    for ( i = 0; i < m->col; i++) {
        tmp = m->m[idx][i];
        if (tmp > *value) {
            *value = tmp;
            pos = i;
        }
    }
    return pos;
}

int get_min_of_col(struct Matrix* m, int idx, double* value) {
    if (NULL == m || NULL == value || idx > m->col) {
        return -1;
    }

    int pos = 0;
    int i = 0;
    double tmp = 0;
    *value = m->m[0][idx];
    for ( i = 0; i < m->row; i++) {
        tmp = m->m[i][idx];
        if (tmp < *value) {
            *value = tmp;
            pos = i;
        }
    }
    return pos;
}
