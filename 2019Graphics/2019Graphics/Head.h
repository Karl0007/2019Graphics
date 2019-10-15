#pragma once
#include <Eigen/Eigen>
#include <unordered_map>
#include <unordered_set>
class Graph;
using Hash = std::unordered_map<int, Graph*>;
using Set = std::unordered_set<int>;
using Eigen::Vector2f;
using Eigen::Vector2i;
using Eigen::Vector3i;
using Eigen::Vector3f;
using Eigen::Matrix3f;
using Eigen::Matrix2i;
using Byte = unsigned char;