// Viterbi_model_xx.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include "iostream"
using namespace std;

class Viterbi
{
private:
	float **MaxScore;
	int **PrevPos;
	vector<vector<int>> Backdate;
	int m; //列，m个词
	int n; //行，n个最长候选
	
public:
	Viterbi(int row, int col); //初始化MaxScore，PrevPos
	void CalViterbi(float **EmissionPro,float ***TransitionPro); //计算维特比
	vector<vector<int>> BackdateCoordinate(); //回溯
	void CoordinateToVector(int r, int c, vector<vector<int>> &vec); //将坐标放入vec中
	void getLastMax(float *maxscore, int *posend); //得到最后一列的最大值及其位置
};

Viterbi::Viterbi(int row,int col)
{
	m = col;
	n = row;
	MaxScore = new float*[n];
	PrevPos = new int*[n];
	for (int a = 0; a < n; a++)
	{
		MaxScore[a] = new float[m];
		PrevPos[a] = new int[m];
	}

	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < m; i++)
		{
			MaxScore[j][i] = 0;
			PrevPos[j][i] = -1;
		}
	}
}

void Viterbi::CalViterbi(float **EmissionPro, float ***TransitionPro)
{
	int i = 0, j = 0, k = 0;
	for (; i < m; i++)
	{
		if (i == 0)
		{
			for (j = 0; j < n; j++)
			{
				MaxScore[j][0] = EmissionPro[j][0] + TransitionPro[0][j][0];
			}
		}
		else
		{
			for (j = 0; j < n; j++)
			{
				float max = 0;
				int postemp = -1;
				for (k = 0; k < n; k++)
				{
					float mtemp = EmissionPro[j][i] + TransitionPro[i][j][k] + MaxScore[k][i - 1];
					if (mtemp > max)
					{
						max = mtemp;
						postemp = k;
					}
				}
				MaxScore[j][i] = max;
				PrevPos[j][i] = postemp;
			}
		}
	}
	cout << "MaxScore:" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << MaxScore[i][j] << " ";
		cout << endl;
	}
	cout << "PrevPro:" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << PrevPos[i][j] << " ";
		cout << endl;
	}


}

vector<vector<int>> Viterbi::BackdateCoordinate()
{
	float maxscore = 0;
	int posend = -1;
	getLastMax(&maxscore, &posend);
	cout << "maxscore:" << maxscore << ",posend:" << posend << endl;
	CoordinateToVector(posend, m-1, Backdate);
	int backdate_pos = PrevPos[posend][m-1];
	for (int i = m - 2; i > 0; --i)
	{
		CoordinateToVector(backdate_pos, i, Backdate);
		backdate_pos = PrevPos[backdate_pos][i];
		if (backdate_pos == -1)
		{
			cout << "Back to the first word!" << endl;
			break;
		}
	}
	CoordinateToVector(backdate_pos, 0, Backdate);
	reverse(Backdate.begin(), Backdate.end());
	return Backdate;
}

void Viterbi::CoordinateToVector(int r, int c, vector<vector<int>> &vec)
{
	vector<int> tempvec;
	tempvec.push_back(r);
	tempvec.push_back(c);
	vec.push_back(tempvec);
}

void Viterbi::getLastMax(float *maxscore, int *posend)
{
	int i = m - 1;
	*maxscore = MaxScore[0][i];
	for (int j = 1; j < n; j++)
	{
		if (MaxScore[j][i] > *maxscore)
		{
			*maxscore = MaxScore[j][i];
			*posend = j;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	int m = 3;
	int n = 3;

	cout << "Input EmissionPro:" << endl;
	//分配内存
	float **EmissionPro;
	EmissionPro = new float*[n];
	for (int i = 0; i < n; i++)
		EmissionPro[i] = new float[m];

	for (int j = 0; j < n; j++)
		for (int k = 0; k < m; k++)
			cin >> EmissionPro[j][k];

	cout << "Input TransitionPro:" << endl;
	//分配内存
	float ***TransitionPro;
	TransitionPro = new float**[m];
	for (int i = 0; i < m; i++)
	{
		TransitionPro[i] = new float*[n];
		for (int j = 0; j < n; j++)
			TransitionPro[i][j] = new float[n];
	}

	for (int i = 0; i < m;i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				cin >> TransitionPro[i][j][k];

	Viterbi vit(n,m);
	vit.CalViterbi(EmissionPro, TransitionPro);
	vector<vector<int>> backdate = vit.BackdateCoordinate();

	cout << "The coordinate is :" << endl;
	vector<vector<int>>::iterator vviter;
	vector<int>::iterator viter;
	for (vviter = backdate.begin(); vviter != backdate.end(); vviter++)
	{
		cout << "(";
		for (viter = (*vviter).begin(); viter != (*vviter).end(); viter++)
			cout << *viter << ",";
		cout << ")" << endl;
	}

	system("pause");
	return 0;
}

//测试用例
//float EmissionPro[3][3] = { { 0.8, 0.3,0.1 }, { 0.2, 0.2,0.2 }, { 0.4, 0.6,0.3 } };
//float TransitionPro[3][3][3] = { 0.1, 0, 0, 0.2, 0, 0, 0.3, 0, 0, 0.2, 0.3, 0.2, 0.1, 0.2, 0.3, 0.8, 0.2, 0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1 };
