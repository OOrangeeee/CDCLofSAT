/*最后编辑：*/
/*晋晨曦 2023.9.8 0:00*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structure.h"
//最关键的解决类，封装了基本上所有需要的函数和成员参数
class Solver
{

	friend Clause;

	friend lit;

private:

	Clause** clauses;//初始子句列表
	int ClauseNum{ 0 };//初始子句数目
	mQueue<Clause*> learns;//由冲突分析得来的用于学习的子句列表
	double clauseIncrease{ 1 };//子句活跃度增量
	double clauseDecrease{ -1 };//子句活跃的衰减量
	int VARNUM{ -1 };//文字数量
	int clauseNum{ -1 };//子句数量
	double* Act;//通过启发式方法来衡量变量活跃度
	double varIncrease{ 1 };//变量活跃度的增加因子
	double varDecrease{ 1 };//变量活跃度的衰减因子
	mQueue<Clause*>* getWatch;//用于胀缩数据结构的子句索引队列，每个子句的前两个值的反面作为指针
	mQueue<lit> queueForChuanBo;//储存了下一个被复制的变元
	char* fuHao;//目前对于变元的赋值
	mVector<lit> timeInList;//迹
	mVector<int> timeInLiseF; //迹中不同决策级别的分隔符索引，也就是每个决策点在迹的哪个位置
	Clause** reasonForFalse;//推断出每个变元真值的原因子句，也就是迹中的变元赋值原因
	int* level;//现在的决策层数
	int rootLevel{ 0 };
	int* result;//定义结果
	int resultSize{ 0 };




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//主调用函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




public:
	/// <summary>
	/// 通过多次调用findans函数来找到答案
	/// </summary>
	/// <returns>返回是否有解</returns>
	int GetAns()
	{
		SearchParams params(0.95, 0.999);// 初始化搜索参数，包括文字和子句的衰变参数
		double nof_conflicts{ 100 };// 冲突子句数量上限
		double nof_learnts = { ClauseNum / 3.0 };//  学习子句数量上限
		int status{ Unset };
		rootLevel = getDlevel();
		if (Influence())// 如果在顶层就存在冲突，表示无解
		{
			FlashBackALLD(rootLevel);
			return False;
		}
		MakeDataBaseEasy();//对子句库进行化简
		while (status == Unset)
		{
			status = FindAns((int)nof_conflicts, (int)nof_learnts, params);
			// 每次搜索完毕后，都会调整冲突子句数量上限和学习子句数量上限
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}
		// 恢复到搜索之前的状态，取消所有在根层级之上的决策
		FlashBackALLD(rootLevel);
		return status == True;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//主处理函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




private:
	/// <summary>
	/// 最重要的搜索函数，用于在子句中不断传播化简，做决策，判断冲突，学习子句，直到确定无解或者有解
	/// </summary>
	/// <param name="nof_conflicts">冲突上限</param>
	/// <param name="nof_learnt">学习子句上限</param>
	/// <param name="params">搜索参数</param>
	/// <returns>返回是否找到结果</returns>
	int FindAns(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// 设置活跃度衰减因子
		varDecrease = 1 / params.varDecrease;
		clauseDecrease = 1 / params.clause_decay;
		int conflictC{ 0 };
		for (int i{ 0 }; i < VARNUM + 1; i++)
			result[i] = 0;
		resultSize = 0;
		while (1)
		{
			Clause* confl = Influence();//子句传播化简
			// 如果存在冲突子句
			if (confl)
			{
				conflictC++;
				// 如果冲突发生在根决策层次，返回False
				if (getDlevel() == rootLevel)
					return False;
				mVector<lit> learnt_clause;
				int backtr_level;
				//得到学习子句中的文字
				GetReason(confl, learnt_clause, backtr_level);
				// 回溯到最大的决策层次
				FlashBackALLD(max(backtr_level, rootLevel));
				// 根据文字记录学习子句
				LearnFromFalse(learnt_clause);
				// 如果学习子句只有一个文字，设置其决策层次为0
				if (learnt_clause.size() == 1)
					level[learnt_clause[0].var()] = 0;
				// 子句和文字活动量衰减
				DecreaseClasuseinAct();
				DecreaseVARinAct();
			}
			else
			{
				// 如果当前决策层次为根层次，化简数据
				if (getDlevel() == rootLevel)
					MakeDataBaseEasy();
				// 如果学习子句的数量超过限制，根据活跃度删除部分学习子句
				if ((int)learns.size() - (int)getFuHao() >= nof_learnt && nof_learnt >= 0)
					CleanDataBase();
				// 如果所有变量都已赋值，找到一个解
				if (getFuHao() == getVAR())
				{
					for (int i{ 1 }; i <= getVAR(); i++)
					{
						result[i] = ((fuHao[i] == True));
						resultSize++;
					}
					FlashBackALLD(rootLevel);
					return 1;
				}
				else
				{
					// 如果冲突数量超过限制，则重新开始
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						FlashBackALLD(rootLevel);
						return Unset;
					}
					// 寻找一个未赋值的活跃文字
					int tmp{ -1 };
					double m{ -1 };
					lit p;
					for (int i{ 1 }; i <= getVAR(); i++)
					{
						p = lit(i);
						if (m < Act[p.index()] && fuHao[p.var()] == Unset)
						{
							tmp = p.index();
							m = Act[p.index()];
						}
						p = lit(-i);
						if (m < Act[p.index()] && fuHao[p.var()] == Unset)
						{
							tmp = p.index();
							m = Act[p.index()];
						}
					}
					if (tmp < 0)
					{
						// 如果找不到需要赋值的文字，则找到了一个解
						for (int i{ 1 }; i <= getVAR(); i++)
						{
							result[i] = ((fuHao[i] == True));
							resultSize++;
						}
						FlashBackALLD(rootLevel);
						return 1;
					}
					// 对找到的需要赋值的文字进行决策
					MakeD(tmp ^ 1);
				}
			}
		}
	}

	/// <summary>
	/// 根据queueForChuanBo队列中的值对变元进行赋值，并根据该变元赋值化简子句，寻找冲突
	/// </summary>
	/// <returns>返回当前决策层</returns>
	Clause* Influence()
	{
		//运行到全部待赋值变元都被赋值
		while (!queueForChuanBo.empty())
		{
			lit p{ queueForChuanBo.front() };
			queueForChuanBo.pop();
			mQueue<Clause*> tmp;
			while (!getWatch[p.x].empty())//根据当前变元的值取出所有与这个变元的反面变元相关的子句精选化简处理
			{
				Clause* t{ getWatch[p.x].front() };
				tmp.push_back(t);
				getWatch[p.x].pop();
			}
			while (!tmp.empty())
			{
				Clause* c{ tmp.front() };
				tmp.pop();
				if ((*c).GetClauseInfluence(*this, p) == 0)//这个函数返回0，意味着会一个单子句中的唯一变元的赋值失败，也就是赋值不唯一，意味着发生冲突
				{
					while (!tmp.empty())//不再进行剩下的步骤，将未处理的子句重新链接回去
					{
						Clause* cl{ tmp.front() };
						tmp.pop();
						getWatch[p.x].push_back(cl);
					}
					while (!queueForChuanBo.empty())
						queueForChuanBo.pop();
					return c;//返回冲突语句
				}
			}
		}
		return nullptr;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//读取函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



public:
	/// <summary>
	/// 读取文件并初始化各类数据结构
	/// </summary>
	/// <param name="filename[]">文件名</param>
	/// <returns>返回文字数量</returns>
	int Read(char filename[])
	{
		FILE* f{ nullptr };
		char s[200];
		int k{ 0 };
		int j{ 0 };
		if ((f = fopen(filename, "rb")) == NULL)
		{
			cout << "文件打开失败\n";
			return -1;
		}
		do
		{
			fgets(s, 200, f);
		} while (s[0] != 'p');
		while (s[j] != '\0')
		{
			if (s[j] - '0' <= 9 && s[j] - '0' >= 0)
				k = k * 10 + (s[j] - '0');
			if (s[j] == ' ')
			{
				VARNUM = k;
				k = 0;
			}
			j++;
		}
		clauseNum = k;
		// 初始化数据结构
		Act = (double*)malloc((VARNUM * 2 + 2) * sizeof(double));//变元活跃度
		fuHao = (char*)malloc((VARNUM + 1) * sizeof(char));//当前变元赋值情况
		reasonForFalse = (Clause**)malloc((VARNUM + 1) * sizeof(Clause*));//冲突原因子句列表
		level = (int*)malloc((VARNUM + 1) * (sizeof(int)));//每个变元的决策层
		getWatch = (mQueue<Clause*>*)malloc((2 * VARNUM + 2) * sizeof(mQueue<Clause*>));//涨缩数据结构中的观察队列，每个元素都是子句指针队列
		result = (int*)malloc((VARNUM + 1) * sizeof(int));//答案
		clauses = (Clause**)malloc(clauseNum * sizeof(Clause*));//子句列表
		for (int i{ 0 }; i < 2 * VARNUM + 2; i++)
			getWatch[i] = mQueue<Clause*>();
		for (int i{ 0 }; i < VARNUM * 2 + 2; i++)
			Act[i] = 0;
		for (int i{ 0 }; i < VARNUM + 1; i++)
		{
			fuHao[i] = 0;
			reasonForFalse[i] = nullptr;
			level[i] = -1;
			result[i] = 0;
		}
		for (int i{ 0 }; i < clauseNum; i++)
		{
			Clause* V = new Clause;
			while (1)
			{
				int tmp;
				fscanf(f, "%d", &tmp);
				if (tmp == 0)
					break;
				V->lits.push_back(lit(tmp));
				//int x = lit(tmp).no();
				Act[lit(tmp).no()] += varIncrease;
			}
			clauses[i] = V;
			ClauseNum++;
		}
		for (int i{ 0 }; i < clauseNum; i++)//由每个子句的前两个位置作为检测指针，要求这两个位置只能是真或者未赋值，假的文字都得到后面去，并且保证第一个为真或者未赋值，当只有第一个未赋值的时候我们选择这个作为单子句，查看是否有冲突或者赋值为真
		{
			if (clauses[i]->lits.size() > 1)
			{
				int x = clauses[i]->lits[0].no();
				int y = clauses[i]->lits[1].no();
				getWatch[clauses[i]->lits[0].no()].push_back(clauses[i]);
				getWatch[clauses[i]->lits[1].no()].push_back(clauses[i]);
			}
			else
			{
				if (MakeD(clauses[i]->lits[0].x) == 0)//如果是单子句就建立决策，建立决策失败就无解
					return -1;
			}
		}
		fclose(f);
		return VARNUM;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//化简部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




private:
	/// <summary>
	/// 化简所有子句
	/// </summary>
	/// <returns>返回是否化简成功</returns>
	bool MakeDataBaseEasy()//化简，尝试对每一个子句进行化简，如果化简完毕且满足就删除
	{
		// 如果存在冲突，则不进行化简
		if (Influence() != nullptr)//由冲突不化简
			return 0;
		// 将所有学习到的子句转换为指针数组
		Clause** cs{ learns.tolist() };
		int length{ 0 };
		length = learns.size();
		learns.clear();
		// 对每一个学习到的子句进行化简
		for (int i{ 0 }; i < length; i++)
		{
			if ((*cs[i]).MakeClauseMoreCleaner(*this))//化简子句，假文字删去，如果有真文字则删去子句，也就是跳过加入列表的步骤
			{

			}
			else
				learns.push_back(cs[i]);
		}
		delete(cs);
		int j{ 0 };
		// 对每一个约束子句进行化简
		for (int i = 0; i < ClauseNum; i++)
		{
			if ((*clauses[i]).MakeClauseMoreCleaner(*this))//化简子句，假文字删去，如果有真文字则删去子句，也就是跳过加入列表的步骤
			{

			}
			else
				clauses[j++] = clauses[i];
		}
		ClauseNum = j;
		return 1;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//决策部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// 创建决策
	/// </summary>
	bool MakeD(int num)
	{
		lit p;
		p.x = num;
		timeInLiseF.push_back(timeInList.size());//将现在做的决策加入到决策级别分隔符索引中
		return Enqueue(p, nullptr);//将决策加入传播队列
	}

	/// <summary>
	/// 将这个文字赋值为正，任何加入迹，将决定这个变元的子句也加入迹
	/// </summary>
	/// <param name="p">选择</param>
	/// <param name="from">选择</param>
	/// <returns>返回是否冲突</returns>
	bool Enqueue(lit p, Clause* from)
	{
		if (this->fuHao[p.var()] != Unset)
		{
			int opt{ 0 };
			if (p.sign() == 0)
				opt = False;
			else
				opt = True;
			if (this->fuHao[p.var()] != opt)//赋值不唯一，冲突！
				return 0;
			else//赋值唯一，不冲突
				return 1;
		}
		else
		{
			int x{ p.var() };
			//将变量赋值为真
			if (p.sign() == 0)
				fuHao[p.var()] = False;
			else
				fuHao[p.var()] = True;

			// 设置'p'的变量在决策层次中的位置
			level[p.var()] = getDlevel();
			reasonForFalse[p.var()] = from;
			timeInList.push_back(p);
			queueForChuanBo.push_back(p);
			return 1;
		}
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//回溯部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// 回溯最后一个迹
	/// </summary>
	void FlashBack()
	{
		lit p{ timeInList[timeInList.size() - 1] };
		int x{ p.var() };
		fuHao[x] = Unset;
		reasonForFalse[x] = nullptr;
		level[x] = -1;
		Act[p.x] += varIncrease;
		timeInList.pop_back();
	}

	/// <summary>
	/// 撤销当前决策层的所有决定
	/// </summary>
	void FlashBackAll()
	{
		// 计算出当前决策层次的大小
		int c{ timeInList.size() - timeInLiseF[timeInLiseF.size() - 1] };
		while (c)
		{
			FlashBack();
			c--;
		}
		timeInLiseF.pop_back();
	}

	/// <summary>
	/// 撤销所有迹直到目标层次
	/// </summary>
	void FlashBackALLD(int level)
	{
		while (getDlevel() > level)
			FlashBackAll();
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//子句学习部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// 发生冲突后调用，用于找到学习子句中的文字
	/// </summary>
	/// <param name="confl">冲突发生的子句</param>
	/// <param name="out_learnt">学习子句中文字的数组</param>
	/// <param name="out_btlevel">最后的回溯目标层</param>
	void GetReason(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		int vars{ getVAR() };
		bool* seen{ nullptr };//标记变元是否被处理过
		seen = (bool*)malloc(vars * sizeof(bool*));
		for (int i{ 0 }; i < getVAR(); i++)
			seen[i] = false;
		int counter{ 0 };
		lit p;
		p.x = 0;
		mVector<lit> pReason;//当前变元赋值的原因子句或者是初始发生冲突的子句
		out_learnt.push_back(p);
		out_btlevel = rootLevel;
		do
		{
			pReason.clear();
			(*confl).ReasonForLearn(*this, p, pReason);//获得当前变元赋值的原因子句或者是初始发生冲突的子句
			for (int j{ 0 }; j < pReason.size(); j++)
			{
				lit q{ pReason[j] };

				if (!seen[q.var()])
				{
					seen[q.var()] = 1;
					if (level[q.var()] > rootLevel)//决策层超过根层，活跃度增加
						GetVARinAct(q);
					if (level[q.var()] == getDlevel())
						counter++;
					// 如果文字'q'的变量的决策层次大于根层次，将文字'q'的符号反转，并添加到学习的子句中，更新反向决策层次，说白了就是去找第一个唯一蕴含切割
					else if (level[q.var()] > rootLevel)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						out_learnt.push_back(tmp);
						out_btlevel = (out_btlevel > level[q.var()]) ? out_btlevel : level[q.var()];
					}
				}
			}
			do
			{
				p = timeInList[timeInList.size() - 1];
				confl = reasonForFalse[p.var()];
				FlashBack();
			} while (!seen[p.var()]);//回溯迹，并且跳过与当前冲突无关的变元赋值
			counter--;
		} while (counter > 0);
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;//最后剩下的一个在冲突发生的决策层的变元就是最大唯一蕴含点，也就是第一个唯一蕴含切割的分界线
	}

	/// <summary>
	/// 创建学习子句并加入学习列表，并将学习子句的第一个，也就是第一个唯一蕴含点放入传播队列进行下一轮赋值
	/// </summary>
	/// <param name="clause">学习子句文字列表</param>
	void LearnFromFalse(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt{ nullptr };
		int tmp = (*c).GetNewClause(*this, clause, 1, learnt);//创建学习子句
		if (learnt == nullptr)
			return;
		Enqueue(clause[0], learnt);//添加到学习列表
		if (learnt != NULL)
			learns.push_back(learnt);
	}

	/// <summary>
	/// 依据活跃度删除部分学习子句
	/// </summary>
	void CleanDataBase()
	{
		int i{ 0 };
		int j{ 0 };
		double lim{ clauseIncrease / learns.size() };
		Clause** arr{ learns.tolist() };
		QuickSort(arr, 0, learns.size() - 1);
		int learnts_size{ learns.size() };
		learns.clear();
		for (i = 0, j = 0; i < learnts_size / 2; i++)
		{
			if (!(*arr[i]).IsClause(*this))//如果子句的第一个变元由于这个子句已经赋值，则跳过，这意味着这个子句为真，因为根据涨缩数据结构第一个变元一般不可能为假
			{
			}
			else
				learns.push_back(arr[i]);
		}
		for (; i < learnts_size; i++)//从后半部分删除
		{
			if (!(*arr[i]).IsClause(*this) && (*arr[i]).Act < lim)
			{
			}
			else
				learns.push_back(arr[i]);
		}
		delete(arr);
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//活跃度管理部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// 减少总活跃度
	/// </summary>
	void DecreaseAct()
	{
		// 调用函数减小变量活动性增加的步长
		DecreaseVARinAct();
		// 调用函数减小子句活动性增加的步长
		DecreaseClasuseinAct();
	}

	/// <summary>
	/// 提高变量的活跃度，但不超过阈值
	/// </summary>
	/// <param name="p">被提高的变元</param>
	void GetVARinAct(lit p)
	{
		if (varDecrease < 0)
			return;
		// 增加变量p的活动性
		if ((Act[p.x] += varIncrease) > 1e100)
			// 如果变量的活动性过大，调用函数进行缩放
			DelVARinAct();
	}

	/// <summary>
	/// 增加子句c的活跃度，但不超过阈值
	/// </summary>
	/// <param name="p">被提高的变元</param>
	void IncreaseClasuseinAct(Clause* c)
	{
		// 增加子句c的活动性
		c->Act += clauseIncrease;
		// 如果子句的活动性过大，调用函数进行缩放
		if (c->Act > 1e20)
			ChangeLearnAct();
	}

	/// <summary>
	/// 降低所有变元的活跃度
	/// </summary>
	void DelVARinAct()
	{
		for (int i{ 0 }; i <= VARNUM * 2 + 1; i++)
			Act[i] *= 1e-100;
		// 缩小活动性增加的步长
		varIncrease *= 1e-100;
	}

	/// <summary>
	/// 降低学习子句活跃度
	/// </summary>
	void ChangeLearnAct()
	{
		// 将所有学习到的子句转换为指针数组
		Clause** arr = learns.tolist();
		for (int i{ 0 }; i < learns.size(); i++)
			arr[i]->Act *= 1e-20;
		delete(arr);
		clauseIncrease *= 1e-20;
	}

	/// <summary>
	/// 减少变元活跃度增长因子
	/// </summary>
	void DecreaseVARinAct()
	{
		if (varDecrease >= 0)
			varIncrease *= varDecrease;
	}

	/// <summary>
	/// 减少子句活跃度增长因子
	/// </summary>
	void DecreaseClasuseinAct()
	{
		// 减小子句活动性增加的步长
		clauseIncrease *= clauseDecrease;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//辅助部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// 返回文字数量
	/// </summary>
	int getVAR()
	{
		return this->VARNUM;
	}

	/// <summary>
	/// 返回迹的长度
	/// </summary>
	/// <returns>返回迹的长度</returns>
	int getFuHao()
	{
		return timeInList.size();
	}

	/// <summary>
	/// 返回子句数量
	/// </summary>
	/// <returns>返回子句数量</returns>
	int getClasueNum()
	{
		return this->clauseNum;
	}

	/// <summary>
	/// 返回学习子句数量
	/// </summary>
	/// <returns>返回学习子句数量</returns>
	int getLearnSize()
	{
		return learns.size();
	}

	/// <summary>
	/// 返回当前决策层
	/// </summary>
	/// <returns>返回当前决策层</returns>
	int getDlevel()
	{
		return timeInLiseF.size();
	}

	/// <summary>
	/// 根据子句活跃度进行快排
	/// </summary>
	/// <param name="a">子句列表</param>
	/// <param name="low">左指针</param>
	/// <param name="high">右指针</param>
	void QuickSort(Clause**& a, int low, int high)
	{
		Clause* pivot = nullptr;
		int i{ low };
		int j{ high };
		if (low < high)
		{
			pivot = a[low]; //将第一个元素作为枢轴
			//接下来进行第一次分治
			while (i < j)
			{
				while (i < j && a[j]->Act >= pivot->Act) //从右往左找到第一个小于枢轴的数
					j--;
				if (i < j)
				{
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->Act < pivot->Act) //从左往右找到第一个大于等于枢轴的数
					i++;
				if (i < j)
				{
					a[j] = a[i];
					j--;
				}
			}
			a[i] = pivot;
			QuickSort(a, low, i - 1);
			QuickSort(a, i + 1, high);
		}
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//输出部分函数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





public:
	/// <summary>
	/// 写入答案到res文件
	/// </summary>
	/// <param name="timer">时间</param>
	/// <param name="literalnum">子句数目</param>
	/// <param name="filename[]">文件名</param>
	void Print(long long timer, int literalnum, char filename[])
	{
		int k{ (int)strlen(filename) };
		filename[k - 1] = 's';
		filename[k - 2] = 'e';
		filename[k - 3] = 'r';
		FILE* f{ nullptr };
		if ((f = fopen(filename, "wb")) == NULL)
		{
			cout << "写入失败\n";
			return;
		}
		fprintf(f, "s %d\n", resultSize == literalnum);
		if (resultSize == literalnum)
		{
			fprintf(f, "v ");
			for (int i = 1; i <= literalnum; i++)
			{
				fprintf(f, "%d ", (result[i] == 0) ? (-(i)) : (i));
			}
			fprintf(f, "\n");
		}
		fprintf(f, "t %lld", timer);
		fclose(f);
	}

	/// <summary>
	/// 展示读取的cnf文件
	/// </summary>
	void ShowCNF()
	{
		cout << "\n----------------------\n";
		cout << "p cnf " << VARNUM << " " << clauseNum << endl;
		for (int i{ 0 }; i < ClauseNum; i++)
		{
			for (int j{ 0 }; j < clauses[i]->lits.size(); j++)
			{
				/*int fuHao = clauses[i]->lits[j].sign();
				int num= clauses[i]->lits[j].var();*/
				cout << ((clauses[i]->lits[j].sign()) ? clauses[i]->lits[j].var() : 0 - clauses[i]->lits[j].var()) << " ";
			}
			cout << "0" << endl;
		}
		cout << "\n----------------------\n";
	}
};

/// <summary>
/// 创建新的学习子句
/// </summary>
/// <param name="S">解决器</param>
/// <param name="ps">文字数组</param>
/// <param name="learnt">是否是学习子句</param>
/// <param name="out_clause">学习子句指针</param>
/// <returns>创建是否成功</returns>
bool Clause::GetNewClause(Solver& S, mVector<lit> ps, bool learnt, Clause*& out_clause)
{
	out_clause = NULL;
	if (!learnt)
	{
		for (int i = 0; i < ps.size(); i++)
		{
			if (S.fuHao[ps[i].var()] == True)
				return 1;
		}
	}
	if (ps.size() == 0)
		return 0;
	else if (ps.size() == 1)
	{
		Clause* tmp = new Clause;
		return S.Enqueue(ps[0], tmp);
	}
	else
	{
		Clause* c = new Clause;
		c->lits = ps;
		c->learnt = learnt;
		c->Act = 0;
		if (learnt)
		{
			double mx = -1;
			int max_i = 0;
			for (int i = 0; i < ps.size(); i++)
			{
				if (S.Act[ps[i].x] > mx)
				{
					mx = S.Act[ps[i].x];
					max_i = i;
				}
			}
			int tmp{ ps[max_i].x };
			int t1{ ps[1].x };
			c->lits[1].x = tmp;
			c->lits[max_i].x = t1;
			S.IncreaseClasuseinAct(c);
			for (int i = 0; i < ps.size(); i++)
			{
				lit tmp = ps[i];
				tmp.x ^= 1;
				S.GetVARinAct(tmp);
			}
		}
		S.getWatch[(c->lits[0]).no()].push_back(c);
		S.getWatch[(c->lits[1]).no()].push_back(c);
		out_clause = c;
		return 1;
	}
}

/// <summary>
/// 判断子句是否为真，仅限学习子句
/// <param name="S">解决器</param>
/// </summary>
/// <returns>返回是否是真子句</returns>
bool Clause::IsClause(Solver S)
{
	return S.reasonForFalse[this->lits[0].var()] == this;
}

/// <summary>
/// 化简子句中的文字，未赋值的留下了，如果子句是真的就返回1，导致删除子句
/// </summary>
/// <param name="S">解决器</param>
/// <returns>返回是否为真</returns>
bool Clause::MakeClauseMoreCleaner(Solver& S)
{
	int j{ 0 };
	for (int i = 0; i < lits.size(); i++)
	{
		int opt{ 0 };
		if (lits[i].sign() == 0)
		{
			opt = False;
		}
		else
			opt = True;
		if (S.fuHao[lits[i].var()] == opt)//子句满足
			return 1;
		if (S.fuHao[lits[i].var()] == Unset)//未被赋值则保留
		{
			lits[j++] = lits[i];
		}
	}
	lits.resize(j);
	return 0;
}

/// <summary>
/// 移动观察指针，或者可以理解为更换观察索引，这是涨缩数据结构的核心
/// </summary>
/// <param name="S">解决器</param>
/// <param name="p">当前被赋值的变元</param>
/// <returns>返回是否成功</returns>
bool Clause::GetClauseInfluence(Solver& S, lit p)
{
	//确保假文字是lits[1]
	if (lits[0].x == p.no())
	{
		lits[0].x = lits[1].x;
		lits[1].x = p.no();
	}
	//如果第0个观察对象为真,则子句已满足
	int opt;
	if (lits[0].sign() == 1)
		opt = True;
	else
		opt = False;
	if (S.fuHao[lits[0].var()] == opt)
	{
		S.getWatch[p.x].push_back(this);
		return true;
	}
	//寻找新的文字来观察
	for (int i = 2; i < lits.size(); i++)
	{
		if (lits[i].sign() == 0)
			opt = True;
		else
			opt = False;
		if (S.fuHao[lits[i].var()] != opt)
		{
			int tm = lits[1].x;
			lits[1].x = lits[i].x;
			lits[i].x = tm;
			S.getWatch[lits[1].no()].push_back(this);
			return 1;
		}
	}
	//子句在当前赋值下为单元子句
	S.getWatch[p.x].push_back(this);
	return S.Enqueue(lits[0], this);
}

/// <summary>
/// 得到冲突语句或者得到当前变元的原因语句
/// </summary>
/// /// <param name="S">解决器</param>
/// <param name="p">当前迹中的变元</param>
/// <param name="out_reason">储存位置</param>
void Clause::ReasonForLearn(Solver& S, lit p, mVector<lit>& out_reason)
{
	int i = 1;
	if (S.fuHao[p.var()] == Unset)
	{
		i = 0;
	}
	for (; i < lits.size(); i++)
	{
		lit tmp;
		tmp.x = lits[i].no();
		out_reason.push_back(tmp);
	}
	if (learnt)
		S.IncreaseClasuseinAct(this);
}
