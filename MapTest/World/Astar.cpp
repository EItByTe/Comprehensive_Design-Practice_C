#include "Astar.h"
#include <algorithm>
using namespace std;

Astar::Astar(int n, int m)
{
	nodeMap.resize(m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			nodeMap[i].push_back(Node(j, i));
		}
	}
}

void Astar::setBlock(initializer_list<pair<int, int>> initList)
{
	for (auto& ele : initList) {
		nodeMap[ele.first][ele.second].block = true;
	}
}

void Astar::setBlock(pair<int, int> ele) //设置障碍(重载）
{
	nodeMap[ele.first][ele.second].block = true;
}

bool Astar::search(int x1, int y1, int x2, int y2)
{
	openList.push_back({ x1,y1 });//首先将起点输入open表

	while (!openList.empty()) {
		auto currentIter = findLeastNode();//从open表中取出f最小的一个节点
		int cx = currentIter->first, cy = currentIter->second;//current的坐标
		auto &currentNode = nodeMap[cy][cx];
		openList.erase(currentIter);//将当前取出的节点从open表中移除
		closedList.push_back({ cx,cy });//加入closed表中

		if (cx == x2 && cy == y2)
			return true;//取出的节点是终点返回true

		for (auto& d : direction) {//对于上下左右四个方向均尝试探索一次
			int nx = d[0] + cx, ny = d[1] + cy;//nx代表next（探索节点）的x坐标
			if (nx>=nodeMap[0].size() || nx < 0 ||//next节点无意义则跳过
				ny>=nodeMap.size() || ny < 0 )
				continue;
			auto &next = nodeMap[ny][nx];//有意义则找到该节点
			//用来定义STL函数find_if()的Pred的Lambta表达式，定义了如何算相等
			auto preLambta = [&](const pair<int,int>& a) {return nx == a.first && ny == a.second; };
			//若探索节点不为block并且不在closed表中
			if (!next.block && find_if(closedList.begin(), closedList.end(), preLambta) == closedList.end()) {
				//若探索节点不在open表中
				if (find_if(openList.begin(), openList.end(), preLambta) == openList.end()) {
					next.calculateCost(&currentNode, x2, y2);
					openList.push_back({nx,ny});
				}
				else {//若探索节点在open表中
					//当前节点的g+1还小于探索节点的g则重新计算open表中探索节点的g并改父亲节点为当前节点
					if (currentNode.g+1  < next.g) {
						next.calculateCost(&currentNode, x2, y2);
					}
				}
			}
		}
	}
	return false;
}

list<pair<int,int>>::iterator Astar::findLeastNode()
{
	auto result = openList.begin();
	for (auto i = openList.begin(); i != openList.end();i++) {
		if (nodeMap[result->second][result->first].f > nodeMap[i->second][i->first].f) {
			result = i;
		}
	}
	return result;
}

