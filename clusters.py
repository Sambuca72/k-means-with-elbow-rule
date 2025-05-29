"""This module visualizes k-means clusters from CSV"""
import pandas as pd # Библиотека для работы с таблицами
import matplotlib.pyplot as plt # Библиотека для построения графиков

# Загружаем данные из CSV-файла, который был сгенерирован С
df = pd.read_csv("clusters.csv")

# Создаем фигуру и ось для построения графика
fig, ax = plt.subplots()

# Отображаем все точки по кластерам
for cluster_id in df['cluster'].unique():

    # Фильтруем только точки (не центроиды), принадлежащие текущему кластеру
    group = df[(df['cluster'] == cluster_id) & (df['type'] == 'point')]
    ax.scatter(group['x'], group['y'], label = f"Cluster {cluster_id + 1}", s = 15)  # Рисуем точки кластера

# Выделяем центроиды кластера красным цветом
centers = df[df['type'] == 'center']
ax.scatter(centers['x'], centers['y'], color = 'red', s = 100, marker = 'X', label = "Centroids")

# Оформление графика
plt.title(f"k = {len(centers)}") # Заголовок с количеством кластеров

# Форматирование таблицы с обозначением кластеров и центроидов
plt.legend(                                    
    loc='upper left',
    fontsize='small',
    title_fontsize='medium'
)
plt.grid(True) # Включаем сетку
plt.axis("equal") # Равномерные масштабы по осям X и Y
plt.show() # Показываем график

# Примечание: для запуска нужно установить библиотеки:
# pip install pandas matplotlib

