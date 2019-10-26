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
3.�Ż�SetColor�����˲�����
4.���߲����Ż� ʹ��ֱ�߸���
5.�Ż�ɫ��ѡ��
Done :
1.״̬���Ż� ��λ�ͼ�����ظ���� ����ȡ������
2.����ѡ��ͼ�� ���Ӹ��ƹ���
3.���Ӹ���ճ������
4.��ӿ�ݼ�������ʽ
*/
