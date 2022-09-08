#pragma once
#include <vector>
#include <set>
#include <list>
#include <memory>
namespace std {
	class Node
	{
	public:
		int  x, y, f, g, h;
		Node* parent;
		bool block;

		Node(int x, int y)//构造函数
			:x(x), y(y), f(0), g(0), h(0), parent(nullptr), block(false) {}

		void calculateCost(Node* par, int x2, int y2) {//计算曼哈顿距离f，g，h
			parent = par;
			g = parent->g + 1;
			h = std::abs(x2 - x) + std::abs(y2 - y);
			f = g + h;
		}
	};


	class Astar
	{
	public:
		vector<vector<Node>> nodeMap;//地图矩阵
		list<pair<int, int>> openList;//open表
		list<pair<int, int>> closedList;//closed表

		Astar(int n, int m);//构造函数输入地图矩阵长宽
		void setBlock(initializer_list<pair<int, int>> initList);//设置障碍
		void setBlock(pair<int, int> ele);//设置障碍(重载）
		bool search(int x1, int y1, int x2, int y2);//开始搜索

	private:
		int direction[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };//用来生成节点周围上下左右的坐标

		list<pair<int, int>>::iterator findLeastNode();//查找open表中f最小的节点
	};



}