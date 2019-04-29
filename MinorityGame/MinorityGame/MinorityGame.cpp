// MinorityGame.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <random>
#include <math.h>
using namespace std;


#define THRESHOLD 60
#define POPULATION 100
#define TRIALS 1000
random_device rd;
default_random_engine generator{rd()};
uniform_real_distribution<double> distribution(0, 1);


bool go(vector<int>& history, int S_Idx , double prob,int memLength,bool verse) {

	double p = 0;
	if (verse) {
		p = distribution(generator);
		if (p < 0.1) {
			verse = true;
		}
		else {
			verse = false;
		}
	}
	switch (S_Idx)
	{
	case 0:/* go by a fixed prob*/
		p = distribution(generator);
		if (p <= prob)
			return true;
		else
			return false;
	case 1:/* go if last week is not crowded*/
		if (history.size() > 0) {
			if (history.back() > THRESHOLD)
				return verse?true:false;
			else
				return verse ? false: true;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}
	case 2:/* go if last week is crowded*/
		if (history.size() > 0) {
			if (history.back() <= THRESHOLD)
				return verse ? true : false;
			else
				return verse ? false : true;;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}
	case 3:/* depend on all the past history*/
		if (history.size() > 0) {
			int cnt = 0;
			for (int h : history) {
				cnt += h;
			}
			if (cnt/history.size() <= THRESHOLD)
				return verse ? false :true;
			else
				return verse ? true : false;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}
	case 4:/* depend on all the past history*/
		if (history.size() > 0) {
			int cnt = 0;
			for (int h : history) {
				cnt += h;
			}
			if (cnt / history.size() <= THRESHOLD)
				return verse ? true : false;
			else
				return verse ? false : true;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}

	case 5:/* depend on limited history*/
		if (history.size() > 0) {
			int cnt = 0;
			for (int i = 0; i<history.size() && i<memLength;i++) {
				cnt += history[history.size() -1 - i];

			}
			if (cnt / (memLength<=history.size()?memLength: history.size()) <= THRESHOLD)
				return verse ? false : true;
			else
				return verse ? true : false;;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}
	case 6:/* depend on limited history*/
		if (history.size() > 0) {
			int cnt = 0;
			for (int i = 0; i < history.size() && i < memLength; i++) {
				cnt += history[history.size() - 1 - i];

			}
			if (cnt / (memLength <= history.size() ? memLength : history.size()) <= THRESHOLD)
				return verse ? true : false;
			else
				return verse ? false : true;;
		}
		else {
			p = distribution(generator);
			if (p <= prob)
				return true;
			else
				return false;
		}
	}

 }




class Drunkard {
public:
	double prob = 0;
	int memLength = 0;
	bool verse = false;
	int S_Idx = 0;
	int Score = 0;
	Drunkard(double p, int l, bool v , int s) {
		prob = p;
		memLength = l;
		verse = v;
		S_Idx = s;
	}
	
};

bool Greater(Drunkard& a, Drunkard& b) {
	return a.Score > b.Score;
}



int main()
{
	vector<int> history;
	vector<Drunkard> Drunkards;
	double p = 0; 
	uniform_int_distribution<int> dis(3, 20);
	int l = 0;
	bool v = false;
	uniform_int_distribution<int> dis_S(0,6);
	uniform_int_distribution<int> vdeci(0,1);
	int Sta = 0;
	for (int i = 0; i < POPULATION; i++) {
		p = distribution(generator);
		l = dis(generator);
		v = vdeci(generator);
		Sta = dis_S(generator);
		Drunkard d(p,l,v,1);//strategy setup
		Drunkards.push_back(d);
		cout << Drunkards[i].S_Idx << endl;
	}
	

	for (int j = 0; j < TRIALS; j++) {
		int cnt = 0;
		vector<int> peopleWhoWent;
		vector<int> peopleWhoHome;
		for (int i = 0; i < POPULATION; i++) {
			Drunkard d = Drunkards[i];
			if (go(history, d.S_Idx, d.prob, d.memLength, d.verse)) {
				cnt += 1;
				peopleWhoWent.push_back(i);
			}
			else {
				peopleWhoHome.push_back(i);
			}
		}
		history.push_back(cnt);
		if (cnt > THRESHOLD) {//over crowded
			for (int i : peopleWhoWent) {
				Drunkards[i].Score += (-1);
			}
			for (int i : peopleWhoHome) {
				Drunkards[i].Score += 0;
			}
		}
		else {
			for (int i : peopleWhoWent) {
				Drunkards[i].Score += 1;
			}
			for (int i : peopleWhoHome) {
				Drunkards[i].Score += 0;
			}
		}
	}

	sort(Drunkards.begin(), Drunkards.end(), Greater);
	for (int i = 0; i < TRIALS; i++) {
		cout << i << " : " << history[i] << endl;
	}

	for (int i = 0; i < 20; i++) {
		cout << Drunkards[i].S_Idx<<" "<<Drunkards[i].verse <<" "<< Drunkards[i].Score << endl;
	}

	vector<int> strategyPoint(13,0);
	vector<int> count(13, 0);
	for (Drunkard d : Drunkards) {
		if (d.S_Idx == 0) {
			strategyPoint[0] += d.Score;
			count[0] += 1;
		}
		else if (d.verse == 1) {
			strategyPoint[d.S_Idx*2] += d.Score;
			count[d.S_Idx * 2] += 1;
		}
		else{
			strategyPoint[d.S_Idx * 2 - 1] += d.Score;
			count[d.S_Idx * 2 - 1] += 1;
		}
	}
	int s = 0;
	int ver = 0;
	for (int i = 0; i < 13; i++) {
		if (count[i] == 0) {
			cout << "Noone use strategy " << i << endl;
			continue;
		}
		else
			cout << i << "th strategy :  " << strategyPoint[i] / count[i] << endl;
	}

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
