import os
import matplotlib.pyplot as plt
import numpy as np

# Путь к папке с файлами (переписать по необходимости)
folder_path = "D:\\Contest_check\\algosi\\chw03\\algosi-graphi\\path-search\\cmake-build-debug\\logs"

# Создание папки для сохранения графиков
plots_folder = os.path.join(folder_path, "plots")
os.makedirs(plots_folder, exist_ok=True)

# Словарь для хранения данных по алгоритмам и типам графов
algorithm_data = {}

# Парсинг файлов
for file_name in os.listdir(folder_path):
    file_path = os.path.join(folder_path, file_name)

    if not os.path.isfile(file_path):
        continue
    
    with open(file_path, 'r') as file:
        file_data = file.read()
        
        # Извлечение информации из файла
        graph_type = file_data.split("Graph Type: ")[1].split("\n")[0]
        vertex_count = int(file_data.split("Vertex Count: ")[1].split("\n")[0])
        edge_count = int(file_data.split("Edge Count: ")[1].split("\n")[0])
        algorithm = file_data.split("Algorithm: ")[1].split("\n")[0]
        duration = int(file_data.split("Duration: ")[1].split(" nanoseconds")[0])
        
        # Добавление данных в словари
        if algorithm not in algorithm_data:
            algorithm_data[algorithm] = {"vertex_count": [], "edge_count": [], "duration": [], "graph_type": []}
        
        algorithm_data[algorithm]["vertex_count"].append(vertex_count)
        algorithm_data[algorithm]["edge_count"].append(edge_count)
        algorithm_data[algorithm]["duration"].append(duration)
        algorithm_data[algorithm]["graph_type"].append(graph_type)

# Построение графиков по каждому алгоритму и сохранение
for algorithm, data in algorithm_data.items():
    # Уникальные типы графов для данного алгоритма
    graph_types = set(data["graph_type"])

    # Создание графика для каждого алгоритма
    plt.figure(figsize=(20, 20))

    for graph_type in graph_types:
        # Фильтрация данных по типу графа
        indices = [i for i, gt in enumerate(data["graph_type"]) if gt == graph_type]
        vertex_count_filtered = [data["vertex_count"][i] for i in indices]
        duration_filtered = [data["duration"][i] for i in indices]

        # Время работы от числа вершин
        plt.plot(vertex_count_filtered, duration_filtered, marker='o', label=graph_type)

    plt.xlabel("Число вершин")
    plt.ylabel("Время работы (наносекунды)")
    plt.title(f"Алгоритм: {algorithm}\nЗависимость времени работы от числа вершин")
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(plots_folder, f"{algorithm}_vertex_count.png"))
    plt.close()

    # Создание графика для каждого алгоритма
    plt.figure(figsize=(20, 20))

    for graph_type in graph_types:
        # Фильтрация данных по типу графа
        indices = [i for i, gt in enumerate(data["graph_type"]) if gt == graph_type]
        edge_count_filtered = [data["edge_count"][i] for i in indices]
        duration_filtered = [data["duration"][i] for i in indices]

        # Время работы от числа ребер
        plt.plot(edge_count_filtered, duration_filtered, marker='o', label=graph_type)

    plt.xlabel("Число ребер")
    plt.ylabel("Время работы (наносекунды)")
    plt.title(f"Алгоритм: {algorithm}\nЗависимость времени работы от числа ребер")
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(plots_folder, f"{algorithm}_edge_count.png"))
    plt.close()

# Создание агрегированного графика времени работы от числа вершин
plt.figure(figsize=(20, 20))
for algorithm, data in algorithm_data.items():
    for graph_type in set(data["graph_type"]):
        indices = [i for i, gt in enumerate(data["graph_type"]) if gt == graph_type]
        vertex_count_filtered = [data["vertex_count"][i] for i in indices]
        duration_filtered = [data["duration"][i] for i in indices]

        plt.plot(vertex_count_filtered, duration_filtered, marker='o', label=f"{algorithm} ({graph_type})")

plt.xlabel("Число вершин")
plt.ylabel("Время работы (наносекунды)")
plt.title("Агрегированная зависимость времени работы от числа вершин")
plt.legend()
plt.grid(True)
plt.savefig(os.path.join(plots_folder, "aggregated_vertex_count.png"))
plt.close()

# Создание агрегированного графика времени работы от числа ребер
plt.figure(figsize=(20, 20))
for algorithm, data in algorithm_data.items():
    for graph_type in set(data["graph_type"]):
        indices = [i for i, gt in enumerate(data["graph_type"]) if gt == graph_type]
        edge_count_filtered = [data["edge_count"][i] for i in indices]
        duration_filtered = [data["duration"][i] for i in indices]

        plt.plot(edge_count_filtered, duration_filtered, marker='o', label=f"{algorithm} ({graph_type})")

plt.xlabel("Число ребер")
plt.ylabel("Время работы (наносекунды)")
plt.title("Агрегированная зависимость времени работы от числа ребер")
plt.legend()
plt.grid(True)
plt.savefig(os.path.join(plots_folder, "aggregated_edge_count.png"))
plt.close()
