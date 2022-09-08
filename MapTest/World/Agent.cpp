#include "Agent.h"
#include "Astar.h"

int randnum(const int min, const int max)
{
	return (rand() % (max - min)) + min; //范围[min,max)
}

Agent::Agent()
{
	startx = starty = finishx = finishy = length_of_path = 0;
    current_step = 0;
    movement = nullptr;
}

Agent::~Agent()
{
    free(movement);
}

void Agent::Initialize(const int height, const int width, const short* map)
{
    //srand(time(nullptr)); // 用当前时间作为种子
    //cout << endl << height << " " << width << endl;
    movement = (int*)malloc(sizeof(int) * height * width * 3);

    while (1)
    {
        startx = randnum(0, height);
        starty = randnum(0, width);
        finishx = randnum(0, height);
        finishy = randnum(0, width);

        if (map[startx * width + starty] == 0 &&
            map[finishx * width + finishy] == 0 &&
            getPath(height, width, map))
            break;
    }
}

bool Agent::getPath(const int height, const int width, const short* map)
{
   // cout << endl << "start: (" << startx << "," << starty << ")" << endl
     //   << "finish: (" << finishx << "," << finishy << ")" << endl;
   /* cout << "map:" << endl;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            cout << map[row * height + col];
        }
        cout << endl;
    }*/
    

    //测试过相等的时候是正确的，不相等可能出问题
    Astar astar(height, width);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (map[row * width + col] == 1)
                // 这里坐标要反过来
                astar.setBlock({ col,row });
        }
    }

    if (astar.search(startx, starty, finishx, finishy)) {

        //cout << "entered" << endl;

        //找到通路
        list<Node*> searchPath;
        // 这里坐标要反过来
        Node* sNode = &astar.nodeMap[finishy][finishx];
        do
        {
            searchPath.push_front(sNode);
            sNode = sNode->parent;
        } while (sNode != nullptr);//起点为空指针

        //cout << "step 1 completed" << endl;

        //记录路径
        length_of_path = 0;
        for (auto& x : searchPath) {
            movement[length_of_path * num_of_props + 0] = x->x;
            movement[length_of_path * num_of_props + 1] = x->y;
            movement[length_of_path * num_of_props + 2] = 0;
            //cout << '(' << x->x << ',' << x->y << ')';
            length_of_path++;
        }
        //cout << "path recorded" << endl;

        //记录姿态
        for (int id = 0; id < length_of_path-1; id++)
        {
            for (int dir = 0; dir < 4; dir++)
            {
                if (movement[id * num_of_props + 0] + direction[dir][0] == movement[(id + 1) * num_of_props + 0] &&
                    movement[id * num_of_props + 1] + direction[dir][1] == movement[(id + 1) * num_of_props + 1])
                {
                    movement[id * num_of_props + 2] = dir;
                    break;
                }
            }
        }
        movement[(length_of_path - 1) * num_of_props + 2] = movement[(length_of_path - 2) * num_of_props + 2];
        //showPath(height, width, (short*)map);
        return true;
    }
    else
    {
        //cout << "无法到达！" << endl;
        return false;
    }
}

void Agent::showPath(const int height, const int width, const short* map)
{
    short tmp[500 * 500];
    char label[6] = { '0','1','^','v','<','>' };
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            tmp[row * width + col] = map[row * width + col];
        }
    }

    for (int id = 0; id < length_of_path; id++)
    {
        int x = movement[id * num_of_props + 0];
        int y = movement[id * num_of_props + 1];
        tmp[x * width + y] = movement[id * num_of_props + 2] + 2;
    }
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            cout << label[tmp[row * width + col]];
        }
        cout << endl;
    }
}

void Agent::setStep(const int step_of_clock)
{
    current_step = step_of_clock;
}

void Agent::getPose(int* x, int* y, int* dir)
{
    int real_step = min(current_step, length_of_path - 1);
    *x = movement[real_step * num_of_props + 0];
    *y = movement[real_step * num_of_props + 1];
    *dir = movement[real_step * num_of_props + 2];
}