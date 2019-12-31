#pragma once
#include<iostream>
#include<vector>
#include <string>
#include<set>
#include<map>
#include <cassert>
#include<algorithm>
#pragma warning(disable:4996)

typedef unsigned __int64 uint64;
using namespace std;

struct S_IJ
{
	unsigned int I;
	unsigned int J;
	bool operator == (const S_IJ & obj) const //���� ��==�� ���������������� const �����ˣ�����ᱨ��
	{
		return I == obj.I && J == obj.J;//����ƥ�������������Լ��趨
	}

	bool operator<(const struct S_IJ & right)const   //����<�����
	{
		if (this->I == right.I&&this->J == right.J)     //����idȥ��
			return false;
		else
		{
			if (this->I != right.I)
			{
				return this->I < right.I;      //����
			}
			else
			{
				return this->J < right.J;
			}
		}
	}
};

//һ��������ṹ
struct S_XY
{
	double X;
	double Y;

};

//�洢���������ĸ���������
struct grid_XY
{
	vector<S_XY>vxy;
};

//��I��С��������
bool comp_I(const S_IJ &a, const S_IJ &b)
{
	if (a.I < b.I)
		return true;
	else if (a.I == b.I  && a.J < b.J)
		return true;
	else
		return false;
}


//��J��С��������
bool comp_J(const S_IJ &a, const S_IJ &b)
{
	if (a.J < b.J)
		return true;
	else if (a.J == b.J  && a.I < b.I)
		return true;
	else
		return false;
}


void multiscale_combine(vector<uint64> vOut, vector<grid_XY>&xy)
{
	C_GeodataProcess CT;
	uint64 outMc0;


	//�����
	vector<S_IJ>vij;
	vector<S_XY>vxy;
	vector<S_IJ>points_IJ;
	vector<S_XY>polygon_XY;
	vector<S_IJ>::iterator ite;

	//�����ж���װ��vector����������ż���εĶ���
	for (int i = 0; i < vOut.size(); i++)
	{
		outMc0 = vOut[i];

		CT.test_m_Corner4XY(outMc0, vxy);
		CT.m_Corner4IJ(outMc0, vij);
		for (int j = 0; j < vxy.size(); j++)
		{
			ite = find(points_IJ.begin(), points_IJ.end(), vij[j]);
			if (ite != points_IJ.end())
			{
				points_IJ.erase(ite);
			}
			else
			{
				points_IJ.push_back(vij[j]);
			}
			//points_IJ.push_back(vij[j]);
		}
	}



	//�����㰴x����
	sort(points_IJ.begin(), points_IJ.end(), comp_I);
	vector<S_IJ>sorted_I;
	for (ite = points_IJ.begin(); ite != points_IJ.end(); ite++)
	{
		sorted_I.push_back(*ite);
	}


	//�����㰴y����
	sort(points_IJ.begin(), points_IJ.end(), comp_J);
	vector<S_IJ>sorted_J;
	for (ite = points_IJ.begin(); ite != points_IJ.end(); ite++)
	{
		sorted_J.push_back(*ite);
	}


	//��ˮƽ��װ���ֵ�
	map<S_IJ, S_IJ>edges_h;
	int i = 0;
	while (i < sorted_J.size())
	{
		double curr_y = sorted_J[i].J;
		while (i < sorted_J.size() && sorted_J[i].J == curr_y)
		{
			edges_h[sorted_J[i]] = sorted_J[i + 1];
			edges_h[sorted_J[i + 1]] = sorted_J[i];
			i += 2;
		}
	}

	//����ֱ��װ���ֵ�
	map<S_IJ, S_IJ>edges_v;
	int j = 0;
	while (j < sorted_I.size())
	{
		double curr_x = sorted_I[j].I;
		while (j < sorted_I.size() && sorted_I[j].I == curr_x)
		{
			edges_v[sorted_I[j]] = sorted_I[j + 1];
			edges_v[sorted_I[j + 1]] = sorted_I[j];
			j += 2;
		}
	}


	//�õ��߽����괮
	int e = 0;
	vector<S_IJ>polygon_IJ;
	grid_XY qq;
	while (edges_h.size() > 0)
	{
		polygon_IJ.clear();
		polygon_IJ.push_back((*edges_h.begin()).first);//��ˮƽ�ߵ�һ����װ��IJ���괮
		edges_h.erase(polygon_IJ.back());
		polygon_IJ.push_back(edges_v[polygon_IJ.back()]);
		edges_v.erase(polygon_IJ.back());
		e = 1;
		while (polygon_IJ.back().I != polygon_IJ.front().I || polygon_IJ.back().J != polygon_IJ.front().J)
		{
			if (e == 0)//e=0��Ӵ�ֱ����ȡ��
			{
				polygon_IJ.push_back(edges_v[polygon_IJ.back()]);
				edges_v.erase(polygon_IJ.back());
				e = 1;
			}
			else //e=0���ˮƽ����ȡ��
			{
				polygon_IJ.push_back(edges_h[polygon_IJ.back()]);
				edges_h.erase(polygon_IJ.back());
				e = 0;
			}

		}
		for (int k = 0; k < polygon_IJ.size(); k++)
		{
			if (edges_h.find(polygon_IJ[k]) != edges_h.end())
			{
				edges_h.erase(polygon_IJ[k]);
			}
		}
		for (int k = 0; k < polygon_IJ.size(); k++)
		{
			if (edges_v.find(polygon_IJ[k]) != edges_v.end())
			{
				edges_v.erase(polygon_IJ[k]);
			}
		}

		S_XY tt;
		polygon_XY.clear();
		for (int r = 0; r < polygon_IJ.size(); r++)
		{
			//cout << polygon_IJ[r].I << "," << polygon_IJ[r].J << endl;
			CT.IJNtoBL(polygon_IJ[r].J, polygon_IJ[r].I, 31, tt.X, tt.Y);
			polygon_XY.push_back(tt);
		}
		cout << endl;
		qq.vxy = polygon_XY;
		xy.push_back(qq);
	}
}

