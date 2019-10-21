#pragma once
#include <Eigen/Eigen>
#include <unordered_map>
#include <unordered_set>
#include <vector>
class Graph;
class MainWindow;
class Painter;
class Canvas;
using Hash = std::unordered_map<int, Graph*>;
using Set = std::unordered_set<int>;
using std::vector;
using Eigen::Vector2f;
using Eigen::Vector2i;
using Eigen::Vector3i;
using Eigen::Vector3f;
using Eigen::Matrix3f;
using Eigen::Matrix2i;
using Byte = unsigned char;
constexpr auto VERSION = "V0.81 -> V0.82 updating";
/*
V0.82 update :
ToDo : 
1.状态机优化 多次绘图不用重复点击 增加取消功能
2.增加复制粘贴快捷键
3.优化SetColor增加滤波功能
4.曲线采样优化 使用直线更新
Done :
*/
