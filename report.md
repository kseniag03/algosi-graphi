##  АиСД 2023 КДЗ №3

### Ганина Ксения Андреевна 212 (тг для вопросов: @kgnn47)
### CLion & IDLE python
### Выполнено
С++: генерация 3-х типов графов, 3 алгоритма на поиск кратчайших путей, замеры времени работы алгоритмов, сохранение результатов в файлы логов
Python: получение данных из файлов логов и построение графиков + их сохранение
### Не сделано: доп. алгоритм
________________________

## 1. Ориентация по проекту

Код со структурами, алгоритмами и генерацией можно посмотреть [здесь](https://github.com/kseniag03/algosi-graphi/blob/master/path-search/main.cpp)

Код с построением и сохранением графиков [здесь](https://github.com/kseniag03/algosi-graphi/blob/master/path-search/plots-gen-save.py)

Логи с данными для графиков можно посмотреть [здесь](https://github.com/kseniag03/algosi-graphi/tree/master/path-search/cmake-build-debug/logs)

Сами графики расположены [здесь](https://github.com/kseniag03/algosi-graphi/tree/master/path-search/cmake-build-debug/logs/plots)
________________________

## 2. Поясняю за графики <br>

### 2.1. Алгоритм Дейкстры

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Dijkstra_vertex_count.png

Как видим, алгоритм работает очень быстро на деревьях; сложность на полном графе возрастает линейно, на связном -- логарифмически

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Dijkstra_edge_count.png

Скорость работы алгоритма практически не зависит от числа рёбер в случае деревьев; полный и связный граф растут одинаково, но в полном графе больше рёбер, поэтому он продолжает расти и дальше

### 2.2. Алгоритм Форда-Беллмана

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Bellman-Ford_vertex_count.png

Тоже очень быстро работает на деревьях; в случае полного графа растёт кубически (при m == n^2) или выше, в случае связного графа -- квадратично (при m = n) или выше

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Bellman-Ford_edge_count.png

Скорость работы алгоритма снова практически не зависит от числа рёбер в случае деревьев, а полный и связный меняются примерно одинаково

### 2.3. Алгоритм Флойда-Уоршелла

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Floyd-Warshall_vertex_count.png

Работает практически идентично на полном и связном графах; на деревьях быстрее, но не так ощутимо, как два предыдущих алгоритма

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/Floyd-Warshall_edge_count.png

Уже при малом числе рёбер показывает более высокое время, чем при большом кол-ве рёбер связного или ещё большего кол-ва полного графа; медленнее всего растёт в полном графе

### 2.4. Агрегированные графики зависимости

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/aggregated_vertex_count.png

Эффективнее всего себя показывает Дейкстра на деревьях (настолько эффективно, что даже на не самом маленьком размере плоскости его график не видно...) и Дейкстра на связных <br> Менее эффективным по графику является Флойд-Уоршелл на полных и связных (однако это зависит от соотношения числа вершин и рёбер, т.к. на графике видно, что в некоторых случаях он будет работать быстрее Беллмана-Форда на полном)

https://github.com/kseniag03/algosi-graphi/blob/master/path-search/cmake-build-debug/logs/plots/aggregated_edge_count.png

Флойд-Уоршелл на деревьях очень быстро растёт при небольших значениях числа рёбер, что сильно выделяет его среди других графиков (и в принципе этот алгоритм растёт быстрее остальных, т.е. он самый неэффективный); Дейкстра же наоборот -- растёт медленнее всего на всех типах графов. Беллман-Форд где-то посередине (но линии с деревьями не видно, поэтому, возможно, он тоже весьма эффективен)
________________________

## 3. Дополнения к выводам

Итак, по графикам видим, что  работает быстрее на  данных, чем . <br>

В большинстве случаев 

