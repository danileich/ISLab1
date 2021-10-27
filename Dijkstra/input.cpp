#include "input.h"

Input::Input(std::string fileName, bool dma)
{
    map.diagonal_moves_allowed = dma;
    if(fileName != "")
    {
        loadInput(fileName);
        return;
    }
}

double Grid::getCost(Node current, Node neighbor)
{
    // по хорошему, тут надо учитывать также случай current=neighbor, но ожидается, что такого случая никогда не будет
    // /////////////////////////////////////////////////////////////////////////////////////////
    // диагональный переход дает нам корень из 2 (sqrt(1*1+1*1))
    // переход в cardinal дает лишь изменение 1 координаты, потому переход дает нам стоимость 1
    // /////////////////////////////////////////////////////////////////////////////////////////
    if ((current.x != neighbor.x) && (current.y != neighbor.y))
        return sqrt(2);
    else
        return 1;
}
std::list<Node> Grid::getValidMoves(Node current)
{
    std::list<Node> result;
    std::list<std::pair<int, int>> delta1 = {{0 ,1}, {1, 0}, {-1, 0}, {0, -1}};
    for(auto delta:delta1)
        if(current.x + delta.first < width-1 && current.x + delta.first >= 0 && current.y + delta.second < height-1 && current.y + delta.second >= 0)
            if(elements[current.x + delta.first][current.y + delta.second] == 0)
                result.push_back(Node(current.x + delta.first, current.y + delta.second));
    if(diagonal_moves_allowed)
    {
        // просто добавляем дополнительные значения по диагоналям в наш result
        std::list<std::pair<int, int>> delta2 = {{1, 1}, {-1, -1}, {1, -1},  {-1, 1}};
        for(auto delta:delta2)
            if(current.x + delta.first >= 0 && current.x + delta.first <= width &&  current.y + delta.second >= 0 && current.y + delta.second <= height)
                if((elements[current.x + delta.first][current.y + delta.second] == 0) && (elements[current.x + delta.first][current.y] == 0) && (elements[current.x][current.y + delta.second] == 0)) 
                {
                    result.push_back(Node(current.x + delta.first, current.y + delta.second));
                }
    }
    return result; // массив со всеми вершинами, к которым у нас есть прямой путь
}

void Input::loadInput(std::string fileName)
{
    std::ifstream in;
    in.open(fileName);
    if(!in.is_open())
    {
        std::cout<<"Error! Cannot load the file "<<fileName<<"\n";
        return;
    }
    char c;
    in>>map.height>>map.width;
    map.elements.resize(map.height, std::vector<int>(map.width, 0));
    for(int k=0; k<map.height; k++)
        for(int l=0; l<map.width; l++)
        {
            in>>c;
            if(c == '#')
                map.elements[k][l]=1;
        }
    in>>start.x>>start.y>>goal.x>>goal.y>>true_cardinal_cost>>true_diagonal_cost;
}
