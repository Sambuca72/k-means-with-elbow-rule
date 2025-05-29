"""This module visualizes k-means clusters from CSV"""
import pandas as pd
import matplotlib.pyplot as plt

# Загрузка CSV
df = pd.read_csv("clusters.csv")

# Фигура
fig, ax = plt.subplots()

# По всем кластерам рисуем точки
for cluster_id in df['cluster'].unique():
    group = df[(df['cluster'] == cluster_id) & (df['type'] == 'point')]
    ax.scatter(group['x'], group['y'], label = f"Cluster {cluster_id + 1}", s = 15)

centers = df[df['type'] == 'center']
ax.scatter(centers['x'], centers['y'], color = 'red', s = 100, marker = 'X', label = "Centroids")

# Параметры вывода изображения
plt.title(f"k = {len(centers)}")
plt.legend (
    loc='upper left',
    fontsize='small',
    title_fontsize='medium'
)
plt.grid(True)
plt.axis("equal")
plt.show()
#pip install pandas matplotlib
