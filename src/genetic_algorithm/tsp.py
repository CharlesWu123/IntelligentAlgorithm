# -*- coding: utf-8 -*-
"""
@Version: 0.1
@Author: Charles
@Time: 2022/11/21 13:17
@File: tsp.py
@Desc: 
"""
import math
import os
import sys
import time

sys.path.append(os.path.join(os.path.dirname(__file__), '../../'))
import random
import numpy as np
import matplotlib.pyplot as plt


class GeneticAlgorithm:
    def __init__(
        self,
        city_count,
        distance_city,
        improve_count,
        generations=100,
        population_size=100,
        mutation_rate=0.005,
        crossover_rate=0.8,
        retain_rate=0.3,
        live_rate=0.5,
        origin=0
    ):
        """

        :param generations: 迭代次数
        :param population_size: 种群大小
        :param mutation_rate: 变异概率
        :param crossover_rate: 交叉概率
        :param retain_rate: 设置强者的概率，种群前30%是强者，保留每代的强者
        :param live_rate: 设置弱者的存活概率
        :param origin: 起始点
        """
        self.city_count = city_count
        self.distance_city = distance_city
        self.improve_count = improve_count
        self.generations = generations
        self.population_size = population_size
        self.mutation_rate = mutation_rate
        self.crossover_rate = crossover_rate
        self.retain_rate = retain_rate
        self.live_rate = live_rate
        self.origin = origin

        self.population = []
        self.every_gen_best = []        # 存储每一代最好的
        self.result_path = []
        self.distance = None

    def run(self):
        self.init_population()
        distance, result_path = self.get_result()
        for _ in range(self.generations):
            parents = self.select()
            children = self.cross(parents)
            self.mutate(children)
            self.population = parents + children
            distance, result_path = self.get_result()
            self.every_gen_best.append(distance)
        self.distance = distance
        self.result_path = [self.origin] + result_path + [self.origin]

    def init_population(self):
        # 初始化
        init_population = [i for i in range(self.city_count)]
        init_population.remove(self.origin)
        for _ in range(self.population_size):
            # 随机生成个体
            path = init_population.copy()
            random.shuffle(path)
            path = self.improve(path)
            self.population.append(path)

    def improve(self, path):
        distance = self.get_distance(path)
        for _ in range(self.improve_count):  # 改良迭代
            u = random.randint(0, len(path) - 1)  # 在[0, len(path)-1]中随机选择交换点下标
            v = random.randint(0, len(path) - 1)
            if u != v:
                new_path = path.copy()
                new_path[u], new_path[v] = new_path[v], new_path[u]
                new_distance = self.get_distance(new_path)
                if new_distance < distance:  # 保留更优解
                    distance = new_distance
                    path = new_path.copy()
        return path

    def select(self):
        # 选择
        # 杰出选择
        # 先对适应度从大到小进行排序，选出存活的染色体，再进行随机选择，选出适应度小但是存活的个体
        graded = [[self.get_distance(path), path] for path in self.population]
        graded = [path[1] for path in sorted(graded)]
        # 选出适应性强的染色体
        retain_length = int(len(graded) * self.retain_rate)
        parents = graded[: retain_length]  # 保留适应性强的染色体
        for weak in graded[retain_length:]:
            if random.random() < self.live_rate:
                parents.append(weak)
        return parents

    def cross(self, parents):
        # 生成子代的个数，以此保证种群稳定
        children_count = self.population_size - len(parents)
        # 孩子列表
        children = []
        while len(children) < children_count:
            male_index = random.randint(0, len(parents) - 1)  # 在父母种群中随机选择父母
            female_index = random.randint(0, len(parents) - 1)
            if male_index != female_index:
                male = parents[male_index]
                female = parents[female_index]
                left = random.randint(0, len(male) - 2)  # 给定父母染色体左右两个位置坐标
                right = random.randint(left + 1, len(male) - 1)
                # 交叉片段
                gen1 = male[left: right]
                gen2 = female[left: right]
                # 通过部分匹配交叉法获得孩子
                # 将male和female中的交叉片段移到末尾
                male = male[right:] + male[:right]
                female = female[right:] + female[:right]
                child1 = male.copy()
                child2 = female.copy()

                for o in gen2:  # 移除male中存在于gen2交换片段上的基因
                    male.remove(o)
                for o in gen1:  # 移除female中存在于gen1交换片段上的基因
                    female.remove(o)

                # 直接替换child上对应的基因片段
                child1[left:right] = gen2
                child2[left:right] = gen1

                # 调整交换片段两侧的基因
                child1[right:] = male[0: len(child1) - right]  # 将原male交叉片段右侧长度对应的现male片段给child
                child1[:left] = male[len(child1) - right:]  # 将现male靠后的片段是原male的左侧片段

                child2[right:] = female[0: len(child2) - right]
                child2[:left] = female[len(child2) - right:]

                children.append(child1)
                children.append(child2)
        return children

    def mutate(self, children):
        # 变异: 随机选取两个下标交换对应的城市
        for i in range(len(children)):
            if random.random() < self.mutation_rate:  # 变异
                u = random.randint(0, self.city_count - 3)
                v = random.randint(u + 1, self.city_count - 2)
                children[i][u], children[i][v] = children[i][v], children[i][u]

    def get_result(self):
        graded = [[self.get_distance(path), path] for path in self.population]
        graded = sorted(graded)
        return graded[0][0], graded[0][1]  # 返回种群的最优解

    def get_distance(self, path):
        distance = 0
        distance += self.distance_city[self.origin][path[0]]
        for i in range(len(path)):
            if i == len(path) - 1:
                distance += self.distance_city[path[i]][self.origin]
            else:
                distance += self.distance_city[path[i]][path[i + 1]]
        return distance


def read_data():
    city_name = []
    city_position = []
    with open("demo.txt", 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for line in lines:
            line = line.split('\n')[0]
            line = line.split('\t')
            city_name.append(line[0])
            city_position.append([float(line[1]), float(line[2])])
    city_position = np.array(city_position)
    # locations = [
    #     [18, 54], [87, 76], [74, 78], [71, 71], [25, 38],
    #     [58, 35], [4, 50], [13, 40], [18, 40], [24, 42],
    #     [71, 44], [64, 60], [68, 58], [83, 69], [58, 69],
    #     [54, 62], [51, 67], [37, 84], [41, 94], [2, 99],
    #     [7, 64], [22, 60], [25, 62], [62, 32], [87, 7],
    #     [91, 38], [83, 46], [41, 26], [45, 21], [44, 35]
    # ]
    # city_name = list(map(str, range(len(locations))))
    # city_position = np.array(locations)
    return city_name, city_position


def distance_Matrix(city_name, city_position):
    city_count = len(city_name)
    distance_city = np.zeros([city_count, city_count])
    for i in range(city_count):
        for j in range(i+1, city_count):
            dis = math.sqrt((city_position[i][0] - city_position[j][0]) ** 2 + (city_position[i][1] - city_position[j][1]) ** 2)
            distance_city[i][j] = dis
            distance_city[j][i] = dis
    return city_count, distance_city


def plot(result_path, every_gen_best):
    X = []
    Y = []
    for i in result_path:
        X.append(city_position[i, 0])
        Y.append(city_position[i, 1])

    plt.figure()
    plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
    # plt.subplot(211)
    plt.plot(X, Y, '-o')
    plt.xlabel('经度')
    plt.ylabel('纬度')
    plt.title("GA_TSP")
    for i in range(len(X)):
        # xy是需要标记的坐标，xytext是对应的标签坐标
        plt.annotate(city_name[result_path[i]], xy=(X[i], Y[i]), xytext=(X[i] + 0.1, Y[i] + 0.1))
        # plt.annotate(f'({X[i]}, {Y[i]})', xy=(X[i], Y[i]), xytext=(X[i] + 0.1, Y[i] + 0.1))
    plt.show()

    plt.figure()
    # plt.subplot(212)
    plt.plot(range(len(every_gen_best)), every_gen_best)
    plt.show()


if __name__ == '__main__':
    city_name, city_position = read_data()
    city_count, distance_city = distance_Matrix(city_name, city_position)
    print(city_count)
    ga = GeneticAlgorithm(
        city_count,
        distance_city,
        improve_count=1000,
        population_size=500,
        generations=3000,
        mutation_rate=0.1,
        retain_rate=0.3,
        live_rate=0.5,
        origin=0
    )
    ga.run()
    print("最佳路径长度:", ga.distance)
    print("最佳路线：")
    for i, index in enumerate(ga.result_path):
        print(city_name[index] + "(" + str(index) + ")", end=' ')
        if i % 9 == 0:
            print()
    plot(ga.result_path, ga.every_gen_best)