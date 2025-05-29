#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_K 15 // Максимальное количество кластеров
#define MAX_POINTS 1000 // Максимальное количество точек
#define MAX_WIDTH 50 // Ширина текстовой гистограммы
#define RERUNS 20 // Повторов для выбора лучшего результата WCSS

// Структура для хранения координат точки
typedef struct
{
    double x, y;
} Point;

// Вычисляем квадрат евклидова расстояния между двумя точками
double dist_sq(Point a, Point b) 
{
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Обновляем координаты центров кластеров как среднее всех точек внутри каждого
void update_centers(Point* data, int n, int k, int* labels, Point* cent) 
{
    int count[MAX_K] = {0};
    Point sum[MAX_K] = {0};

    // Обнуляем центры
    for (int i = 0; i < k; i++) 
    {
        cent[i].x = cent[i].y = 0;
    }

    // Суммируем координаты точек в каждом кластере
    for (int i = 0; i < n; i++) 
    {
        int c = labels[i];
        sum[c].x += data[i].x;
        sum[c].y += data[i].y;
        count[c]++;
    }

    // Вычисляем среднее, если в кластере есть точки
    for (int i = 0; i < k; i++) 
    {
        if (count[i]) {
            cent[i].x = sum[i].x / count[i];
            cent[i].y = sum[i].y / count[i];
        } else {
            // Если кластер пустой — выбираем случайную точку
            int idx = rand() % n;
            cent[i] = data[idx];
        }
    }
}

// Реализация алгоритма kmeans: возвращает число итераций до прекращения обновлений точек вокруг центроида
int kmeans(Point* data, int n, int k, int* labels, Point* centers) 
{
    // Инициализация центров случайными точками
    int used[MAX_POINTS] = {0};
    for (int i = 0; i < k;) 
    {
        int idx = rand() % n;
        if (!used[idx]) 
        {
            centers[i++] = data[idx];
            used[idx] = 1;
        }
    }

    int changed, iter = 0;
    do {
        changed = 0;
        iter++;

        // Назначаем каждой точке ближайший центр
        for (int i = 0; i < n; i++) 
        {
            int best = 0;
            double min_dist = dist_sq(data[i], centers[0]);
            for (int j = 1; j < k; j++) 
            {
                double d = dist_sq(data[i], centers[j]);
                if (d < min_dist) 
                {
                    min_dist = d;
                    best = j;
                }
            }
            // Сохраняем метку, если прозошло изменение центра
            if (labels[i] != best) 
            {
                labels[i] = best;
                changed = 1;
            }
        }

        // Обновляем координаты центров кластеров
        update_centers(data, n, k, labels, centers);

    } while (changed && iter < 100);  // Пока есть изменения и не достигнут лимит
    return iter;
}

// Вычисляем сумму квадратов расстояний внутри кластеров (WCSS)
double wcss(Point* data, int n, int* labels, Point* centers) 
{
    double total = 0.0;
    for (int i = 0; i < n; i++)
        total += dist_sq(data[i], centers[labels[i]]);
    return total;
}

// Вычисляем расстояние от точки до прямой для метода локтя
double dist_line(int x0, double y0, int x1, double y1, int x2, double y2) 
{
    double num = fabs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    double den = sqrt((y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1));
    return num / den;
}

// Отображаем WCSS-график
void graf_wcss(double* wcss) 
{
    double max = wcss[0];
    for (int i = 1; i < MAX_K; i++)
        if (wcss[i] > max) max = wcss[i];

    puts("\nWcss schedule with 'Elbow Method':");
    for (int i = 0; i < MAX_K; i++) 
    {
        int bars = (int)(wcss[i] / max * MAX_WIDTH);
        printf("k = %2d | ", i + 1);
        for (int j = 0; j < bars; j++) printf("*");
        printf("  %.2lf\n", wcss[i]);
    }
}

// Генерируем случайные точки в пределах заданного диапазона
void generate_rndm(Point* data, int n, int range) 
{
    for (int i = 0; i < n; i++) 
    {
        data[i].x = rand() % range;
        data[i].y = rand() % range;
    }
}

// Ручной ввод координат точек с дальнейшими проверками
void input(Point* data, int n) 
{
    char line[100];
    for (int i = 0; i < n; i++) 
    {
        while (1) 
        {
            printf("Point %d (x y): ", i + 1);
            if (!fgets(line, sizeof(line), stdin))
                continue;

            double x, y;
            char extra;
            int count = sscanf(line, "%lf %lf %c", &x, &y, &extra);
            if (count == 2) 
            {
                data[i].x = x;
                data[i].y = y;
                break;
            }
            puts(count == 3 ? "Too many values." : "Invalid input.");
        }
    }
}

// Печатаем точки каждого кластера 
void clusters(Point* data, int n, int* labels, int k) 
{
    for (int cluster = 0; cluster < k; cluster++) 
    {
        printf("Cluster %d:\n", cluster + 1);
        for (int i = 0; i < n; i++)
            if (labels[i] == cluster)
                printf("  (%.2lf, %.2lf)\n", data[i].x, data[i].y);
    }
}

// Печатаем центры кластеров
void print_centers(Point* cent, int k) 
{
    printf("\nCluster centroids (k = %d):\n", k);
    for (int i = 0; i < k; i++)
        printf("Center %d: (%.2lf, %.2lf)\n", i + 1, cent[i].x, cent[i].y);
}

// Сохраняем кластеры и центры в CSV-файл
void csv(Point* data, int n, int* labels, Point* cent, int k, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Cannot open file");
        return;
    }

    fprintf(f, "x,y,cluster,type\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%.2lf,%.2lf,%d,point\n", data[i].x, data[i].y, labels[i]);
    for (int i = 0; i < k; i++)
        fprintf(f, "%.2lf,%.2lf,%d,center\n", cent[i].x, cent[i].y, i);

    fclose(f);
}


int main() 
{
    srand((unsigned)time(NULL));  // Инициализация генератора случайных чисел
    printf("=== K-means with Elbow Method ===\n");

    int n, auto_mode;
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    scanf("%d", &n);
    if (n <= 0 || n > MAX_POINTS) 
    {
        puts("Invalid number of points.");
        return 1;
    }

    // Выбор режима ввода данных: ручной или случайный
    printf("Generate randomly? (1 = yes, 0 = manual input): ");
    scanf("%d", &auto_mode);
    getchar(); // Очистка буфера от лишнего \n после scanf

    // Выделение памяти под точки, метки и центры
    Point* data = malloc(sizeof(Point) * n);
    int* labels = malloc(sizeof(int) * n);
    Point* centers = malloc(sizeof(Point) * MAX_K);
    Point* tmp_centers = malloc(sizeof(Point) * MAX_K);
    double wcss_values[MAX_K];  // Массив значений WCSS для каждого k

    // Ввод или генерация данных
    if (auto_mode)
        generate_rndm(data, n, 50);
    else
        input(data, n);

    // Подбор оптимального k по методу локтя
    for (int k = 1; k <= MAX_K; k++) 
    {
        double best_wcss = 1e9;
        int best_iter = 0;

        // Несколько запусков для выбора лучшего результата по WCSS
        for (int r = 0; r < RERUNS; r++) 
        {
            int iter = kmeans(data, n, k, labels, tmp_centers);
            double current = wcss(data, n, labels, tmp_centers);
            if (current < best_wcss) 
            {
                best_wcss = current;
                best_iter = iter;
                // Сохраняем лучшие центры
                for (int i = 0; i < k; i++) 
                    centers[i] = tmp_centers[i];
            }
        }

        wcss_values[k - 1] = best_wcss;
        printf("[k=%d] best run in %d iterations.\n", k, best_iter);
    }

    // Вычислением оптимальный k (локоть) — точка с наибольшим отклонением от линии
    int best_k = 1;
    double max_d = 0.0;
    for (int k = 2; k < MAX_K; k++) 
    {
        double d = dist_line(k, wcss_values[k - 1], 1, wcss_values[0], MAX_K, wcss_values[MAX_K - 1]);
        if (d > max_d) 
        {
            max_d = d;
            best_k = k;
        }
    }

    graf_wcss(wcss_values);  // Вывод графика WCSS
    printf("\nBest k (elbow) = %d\n", best_k);

    // Повторный запуск kmeans с оптимальным k
    kmeans(data, n, best_k, labels, tmp_centers);

    // Пользовательский выбор: показать кластеры или нет
    int show;
    printf("Show clusters? (1 = yes, 0 = no): ");
    scanf("%d", &show);
    if (show) 
    {
        print_centers(tmp_centers, best_k);
        clusters(data, n, labels, best_k);
    }

    // Сохранение результатов в CSV для визуализации
    csv(data, n, labels, tmp_centers, best_k, "clusters.csv");

    free(data);
    free(labels);
    free(centers);
    free(tmp_centers);
    return 0;
}
