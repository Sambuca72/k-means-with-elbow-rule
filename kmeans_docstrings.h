// Вычисляет квадрат евклидова расстояния между двумя точками
double dist_sq(Point a, Point b);

// Обновляет координаты центров кластеров на основе средних значений точек в каждом кластере
void update_centers(Point* data, int n, int k, int* labels, Point* cent);

// Основная реализация алгоритма k-means
// Возвращает количество итераций до сходимости
int kmeans(Point* data, int n, int k, int* labels, Point* centers);

// Вычисляет суммарное внутрикластерное расстояние (WCSS)
double wcss(Point* data, int n, int* labels, Point* centers);

// Вычисляет расстояние от заданной точки до прямой через две другие точки
// Используется для определения "локтя" на графике
double dist_line(int x0, double y0, int x1, double y1, int x2, double y2);

// Строит горизонтальный ASCII-график значений WCSS по разным значениям k
void graf_wcss(double* wcss);

// Генерирует случайные точки в диапазоне [0, range)
void generate_random(Point* data, int n, int range);

// Читает точки с ввода и проверяет правильность формата
void input(Point* data, int n);

// Выводит координаты точек, сгруппированных по их кластерам
void clusters(Point* data, int n, int* labels, int k);

// Выводит координаты центров кластеров
void print_centers(Point* cent, int k);

// Сохраняет точки и центроиды в CSV-файл для последующей визуализации
void csv(Point* data, int n, int* labels, Point* cent, int k, const char* filename);
