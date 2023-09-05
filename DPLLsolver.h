/*最后编辑：*/
/*晋晨曦 2023.9.4 21.39*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structure.h"
//最关键的解决类，封装了基本上所有需要的函数和成员参数
class Solver
{
	friend Clause;
public:
	Clause** clauses;//子句列表
	int ClauseNum{ 0 };
	mQueue<Clause*> learns;  //由冲突分析得来的用于学习的子句列表
	double clauseIncrease{ 1 };       //子句活跃度增量
	double clauseDecrease{ -1 };     //子句活跃的衰减量
	int VARNUM{ -1 };           //文字数量
	int clauseNum{ -1 };            //子句数量
	double* Act;     //通过启发式方法来衡量变量活跃度
	double varIncrease{ 1 };   //变量活跃度的增加因子
	double varDecrease{ 1 }; //变量活跃度的衰减因子
	mQueue<Clause*>* GetCheck; //每个文字被观察的约束列表
	mQueue<lit> queueForChuanBo;         //传播队列
	char* fuHao;          //按变量索引的当前分配
	mVector<lit> timeInList;     //按时间顺序排列的分配列表
	mVector<int> timeInLiseF; //在“追踪记录”中用于区分不同决策层级的分隔符索引
	Clause** reasonForFalse;       //冲突原因
	int* level;            //每个变量被赋值的决策层级
	int rootLevel{ 0 };
	int* result;//定义结果
	int resultSize{ 0 };

	//辅助函数
	int getVAR()
	{
		return this->VARNUM;
	}
	int getFuHao()
	{
		return timeInList.size();
	}
	int getClasueNum()
	{
		return this->clauseNum;
	}
	int getLearnSize()
	{
		return learns.size();
	}
	int getDlevel()
	{
		return timeInLiseF.size();
	}
	//传播化简函数
	Clause* Influence()
	{
		// 当队列不为空时，循环继续
		while (!queueForChuanBo.empty())
		{
			// 从队列的前端获取元素，赋值给'p'
			lit p = queueForChuanBo.front();
			queueForChuanBo.pop();

			// 创建临时队列'tmp'
			mQueue<Clause*> tmp;

			// 当子队列不为空时，循环继续
			while (!GetCheck[p.x].empty())//取出所有观测队列中的值
			{
				// 从的子队列前端获取元素，赋值给't'
				Clause* t = GetCheck[p.x].front();
				tmp.push_back(t);
				GetCheck[p.x].pop();
			}

			// 对新的队列'tmp'进行处理
			while (!tmp.empty())
			{
				// 从'tmp'队列前端获取元素，赋值给'c'
				Clause* c = tmp.front();
				tmp.pop();

				if ((*c).GetClauseInfluence(*this, p) == 0)
				{
					// 当'tmp'队列不为空时，循环继续
					while (!tmp.empty())
					{
						// 从'tmp'队列前端获取元素，赋值给'cl'
						Clause* cl = tmp.front();
						tmp.pop();
						GetCheck[p.x].push_back(cl);
					}
					while (!queueForChuanBo.empty())
						queueForChuanBo.pop();
					return c;
				}
			}
		}
		return nullptr;
	}

	bool Enqueue(lit p, Clause* from)
	{
		// 如果'p'的变量已经被赋值
		if (this->fuHao[p.var()] != Unset)
		{
			int opt;
			// 如果'p'的符号为0，设置'opt'为False
			if (p.sign() == 0)
				opt = False;
			else
				// 否则设置'opt'为True
				opt = True;
			// 如果新的赋值'opt'和原有的赋值冲突，返回0
			if (this->fuHao[p.var()] != opt)
				return 0;
			else
				// 如果新的赋值'opt'和原有的赋值不冲突，返回1
				return 1;
		}
		else
		{
			int x = p.var();
			// 如果'p'的符号为0，将'p'的变量赋值为False
			if (p.sign() == 0)
			{
				fuHao[p.var()] = False;
			}
			else
			{
				fuHao[p.var()] = True;
			}

			// 设置'p'的变量在决策层次中的位置
			level[p.var()] = getDlevel();
			reasonForFalse[p.var()] = from;
			timeInList.push_back(p);
			queueForChuanBo.push_back(p);
			return 1;
		}
	}

	void GetReason(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		// 获取变量的数量
		int vars = getVAR();
		bool* seen;
		// 分配内存空间给变量是否被看过的标记数组
		seen = (bool*)malloc(vars * sizeof(bool*));
		// 初始化所有的标记为false
		for (int i = 0; i < getVAR(); i++)
			seen[i] = false;
		// 初始化计数器和文字'p'
		int counter = 0;
		lit p;
		p.x = 0;
		mVector<lit> pReason;
		out_learnt.push_back(p);
		out_btlevel = rootLevel;
		do
		{
			pReason.clear();
			(*confl).ReasonForLearn(*this, p, pReason);
			for (int j = 0; j < pReason.size(); j++)
			{
				lit q = pReason[j];
				// 如果文字'q'的变量还没被看过
				if (!seen[q.var()])
				{
					// 标记文字'q'的变量
					seen[q.var()] = 1;
					// 如果文字'q'的变量的决策层次大于根层次，将文字'q'的符号反转，并增加变量的活跃度
					if (level[q.var()] > rootLevel)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						GetVARinAct(q);
					}
					// 如果文字'q'的变量的决策层次等于当前决策层次，增加计数器
					if (level[q.var()] == getDlevel())
						counter++;
					// 否则如果文字'q'的变量的决策层次大于根层次，将文字'q'的符号反转，并添加到学习的子句中，更新反向决策层次
					else if (level[q.var()] > rootLevel)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						out_learnt.push_back(tmp);
						out_btlevel = (out_btlevel > level[q.var()]) ? out_btlevel : level[q.var()];
					}
				}
			}
			// 进入循环，直到文字'p'的变量被看过
			do
			{
				p = timeInList[timeInList.size() - 1];
				confl = reasonForFalse[p.var()];
				FlashBack();
			} while (!seen[p.var()]);
			counter--;
		} while (counter > 0);
		// 将第一个学习的文字设置为文字'p'的否定
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;
	}

	void LearnFromFalse(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt = nullptr;
		int tmp = (*c).GetNewClause(*this, clause, 1, learnt);//创建学习子句
		if (learnt == nullptr)//没有可以学习的了
			return;
		Enqueue(clause[0], learnt);//添加到学习列表
		if (learnt != NULL)//
			learns.push_back(learnt);
	}

	void FlashBack()
	{
		lit p = timeInList[timeInList.size() - 1]; 
		// 获取'p'的变量编号
		int x = p.var(); 
		// 将变量'x'的赋值状态设为未设置
		fuHao[x] = Unset;
		// 将变量'x'的原因设为nullptr
		reasonForFalse[x] = nullptr;
		// 将变量'x'的决策层次设为-1
		level[x] = -1;
		// 增加文字'p'的活跃度
		Act[p.x] += varIncrease;
		timeInList.pop_back();
	}

	//创建决策
	bool MakeD(int num)
	{
		// 创建一个新的文字'p'，并将其设置为输入的数字
		lit p; 
		p.x = num; 
		timeInLiseF.push_back(timeInList.size()); 
		return Enqueue(p, nullptr); 
	}

	void FlashBackAll()
	{
		// 计算出当前决策层次的大小
		int c = timeInList.size() - timeInLiseF[timeInLiseF.size() - 1];
		// 使用循环连续撤销这个决策层次的所有决策
		while (c)
		{
			FlashBack();
			c--;
		}
		timeInLiseF.pop_back();
	}

	void FlashBackALLD(int level)
	{
		// 使用循环撤销决策，直到当前决策层次不大于目标决策层次
		while (getDlevel() > level)
			FlashBackAll();
	}

	int FindAns(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// 设置活跃度衰减因子
		varDecrease = 1 / params.varDecrease;
		clauseDecrease = 1 / params.clause_decay;
		// 初始化冲突计数器
		int conflictC = 0;
		// 初始化结果管理器
		for (int i = 0; i < VARNUM + 1; i++)
			result[i] = 0;
		resultSize = 0;
		// 主循环开始
		while (1)
		{
			Clause* confl = Influence();
			// 如果存在冲突子句
			if (confl)
			{
				// 冲突计数器增加
				conflictC++;
				// 如果冲突发生在根决策层次，返回False
				if (getDlevel() == rootLevel)
					return False;
				// 初始化学习子句和回溯层次
				mVector<lit> learnt_clause;
				int backtr_level;
				GetReason(confl, learnt_clause, backtr_level);
				// 回溯到最大的决策层次
				FlashBackALLD(max(backtr_level, rootLevel));
				// 记录学习子句
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
				// 如果学习子句的数量超过限制，进行数据库压缩
				if ((int)learns.size() - (int)getFuHao() >= nof_learnt && nof_learnt >= 0)
					CleanDataBase();
				// 如果所有变量都已赋值，找到一个解
				if (getFuHao() == getVAR())
				{
					// 组装解
					for (int i = 1; i <= getVAR(); i++)
					{
						result[i] = ((fuHao[i] == True));
						resultSize++;
					}
					// 回溯到根决策层次
					FlashBackALLD(rootLevel);
					return 1;
				}
				else
				{
					// 如果冲突数量超过限制，回溯到根决策层次并返回Unset
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						FlashBackALLD(rootLevel);
						return Unset;
					}
					// 寻找一个未赋值的活跃文字
					int tmp = -1;
					double m = -1;
					lit p;
					for (int i = 1; i <= getVAR(); i++)
					{
						// 正文字
						p = lit(i);
						if (m < Act[p.index()] && fuHao[p.var()] == Unset)
						{
							tmp = p.index();
							m = Act[p.index()];
						}
						// 负文字
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
						for (int i = 1; i <= getVAR(); i++)
						{
							result[i] = ((fuHao[i] == True));
							resultSize++;
						}
						// 回溯到根层
						FlashBackALLD(rootLevel);
						return 1;
					}
					// 对找到的需要赋值的文字进行决策
					MakeD(tmp ^ 1);
				}
			}
		}
	}

	void GetVARinAct(lit p)
	{
		if (varDecrease < 0)
			return;
		// 增加变量p的活动性
		if ((Act[p.x] += varIncrease) > 1e100)
			// 如果变量的活动性过大，调用函数进行缩放
			DelVARinAct();
	}

	void DelVARinAct()
	{
		// 遍历所有的文字，缩小他们的活动性
		for (int i = 0; i <= VARNUM * 2 + 1; i++)
		{
			Act[i] *= 1e-100;
		}
		// 缩小活动性增加的步长
		varIncrease *= 1e-100;
	}

	void DecreaseVARinAct()
	{
		if (varDecrease >= 0)
			varIncrease *= varDecrease;
	}

	void DecreaseClasuseinAct()
	{
		// 减小子句活动性增加的步长
		clauseIncrease *= clauseDecrease;
	}

	void ChangeLearnAct()
	{
		// 将所有学习到的子句转换为指针数组
		Clause** arr = learns.tolist();

		// 遍历所有学习到的子句
		for (int i = 0; i < learns.size(); i++)
		{
			// 缩小子句的活动性
			arr[i]->Act *= 1e-20;
			//learnts[i]->activity *= 1e-20;
			//p=p->next;
		}
		// 删除子句指针数组
		delete(arr);
		// 缩小子句活动性增加的步长
		clauseIncrease *= 1e-20;
	}

	void IncreaseClasuseinAct(Clause* c)
	{
		// 增加子句c的活动性
		c->Act += clauseIncrease;
		// 如果子句的活动性过大，调用函数进行缩放
		if (c->Act > 1e20)
			ChangeLearnAct();
	}

	void DecreaseAct()
	{
		// 调用函数减小变量活动性增加的步长
		DecreaseVARinAct();
		// 调用函数减小子句活动性增加的步长
		DecreaseClasuseinAct();
	}
	//快排，按照子句活跃度对子句排序
	void QuickSort(Clause**& a, int low, int high)
	{
		Clause* pivot;
		int i = low, j = high;
		if (low < high)
		{
			pivot = a[low]; //将第一个元素作为枢轴
			//接下来进行第一次分治
			while (i < j)
			{
				while (i < j && a[j]->Act >= pivot->Act) //从右往左找到第一个小于枢轴的数
					j--;
				if (i < j)
				{ //将该数移动到左指针的位置
					//a.set(i, a[j]);
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->Act < pivot->Act) //从左往右找到第一个大于等于枢轴的数
					i++;
				if (i < j)
				{ //将该数移动到右指针的位置
					a[j] = a[i];
					//a.set(j, a[i]);
					j--;
				}
			}
			//a.set(i, pivot);
			a[i] = pivot; //循环退出时i == j即枢轴的位置
			//递归处理当前枢轴的左右两部分
			QuickSort(a, low, i - 1);
			QuickSort(a, i + 1, high);
		}
	}

	void CleanDataBase()//删除部分学习子句
	{
		int i, j;
		// 计算子句活动性的阈值
		double lim = clauseIncrease / learns.size();

		// 将所有学习到的子句转换为指针数组
		Clause** arr = learns.tolist();

		// 对数组进行快速排序
		QuickSort(arr, 0, learns.size() - 1);

		int learnts_size = learns.size();

		// 清空学习到的子句列表
		learns.clear();

		// 遍历数组的前半部分
		for (i = j = 0; i < learnts_size / 2; i++)
		{
			// 如果子句没有被锁定，则跳过
			if (!(*arr[i]).IsClause(*this))
			{
			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learns.push_back(arr[i]);
			}
		}

		// 遍历数组的后半部分
		for (; i < learnts_size; i++)
		{
			// 如果子句没有被锁定并且其活动性小于阈值，则跳过
			if (!(*arr[i]).IsClause(*this) && (*arr[i]).Act < lim)
			{
			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learns.push_back(arr[i]);
			}
		}

		// 删除子句指针数组
		delete(arr);
	}

	bool MakeDataBaseEasy()//化简，尝试对每一个子句进行化简，如果化简完毕且满足就删除
	{
		// 如果存在冲突，则不进行化简
		if (Influence() != nullptr)//由冲突不化简
		{
			return 0;
		}

		// 将所有学习到的子句转换为指针数组
		Clause** cs = learns.tolist();

		// 清空学习到的子句列表
		learns.clear();

		int j = 0;
		// 对每一个学习到的子句进行化简
		for (int i = 0; i < learns.size(); i++)
		{
			// 如果子句可以被化简，则跳过
			if ((*cs[i]).GetCleanInClause(*this))
			{

			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learns.push_back(cs[i]);
			}
		}

		// 删除子句指针数组
		delete(cs);

		j = 0;
		// 对每一个约束子句进行化简
		for (int i = 0; i < ClauseNum; i++)
		{
			// 如果子句可以被化简，则跳过
			if ((*clauses[i]).GetCleanInClause(*this))
			{

			}
			else
			{
				// 否则，将该子句添加回约束子句列表
				clauses[j++] = clauses[i];
			}
		}
		// 更新约束子句的数量
		ClauseNum = j;

		return 1;
	}

	int GetAns()
	{
		// 初始化搜索参数，包括文字和子句的衰变参数
		SearchParams params(0.95, 0.999);

		// 初始化冲突参数，这个参数可用于指导搜索过程
		double nof_conflicts = 100;

		// 初始化学习参数，一开始设置为约束子句数量的1/3
		double nof_learnts = ClauseNum / 3;

		// 初始化求解状态为未设定（Unset）
		int status = Unset;

		// 获取当前的决策层级，并设置为根层级
		rootLevel = getDlevel();

		// 如果在顶层就存在冲突，表示无解
		if (Influence())
		{
			FlashBackALLD(rootLevel);
			return False;
		}

		// 对数据库进行化简
		MakeDataBaseEasy();

		// 如果求解状态尚未确定，则进入循环，持续进行搜索
		while (status == Unset)
		{
			// 在搜索过程中，会更新求解状态
			status = FindAns((int)nof_conflicts, (int)nof_learnts, params);

			// 每次搜索完毕后，都会调整冲突参数和学习参数，以指导下一次的搜索，把速度交给天意！！！！
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}

		// 恢复到搜索之前的状态，取消所有在根层级之上的决策
		FlashBackALLD(rootLevel);

		// 如果最终的求解状态是真（True），则返回真；否则，返回假（False）
		return status == True;
	}

	int Read(char filename[])
	{
		// 打开文件
		FILE* f;
		char s[200];
		int k = 0, j = 0;
		if ((f = fopen(filename, "rb")) == NULL) // 如果文件打开失败，则返回错误
		{
			printf("无法打开!\n");
			return -1;
		}

		// 跳过文件的非 'p' 行
		do
		{
			fgets(s, 200, f);
		} while (s[0] != 'p');

		// 从 'p' 行提取文字和子句的数量
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
		Act = (double*)malloc((VARNUM * 2 + 2) * sizeof(double));
		fuHao = (char*)malloc((VARNUM + 1) * sizeof(char)); // 分配
		reasonForFalse = (Clause**)malloc((VARNUM + 1) * sizeof(Clause*)); // 冲突原因
		level = (int*)malloc((VARNUM + 1) * (sizeof(int)));
		GetCheck = (mQueue<Clause*>*)malloc((2 * VARNUM + 2) * sizeof(mQueue<Clause*>)); // 观察队列
		result = (int*)malloc((VARNUM + 1) * sizeof(int));
		clauses = (Clause**)malloc(clauseNum * sizeof(Clause*));

		// 初始化观察队列
		for (int i = 0; i < 2 * VARNUM + 2; i++)
			GetCheck[i] = mQueue<Clause*>();

		// 初始化活跃度数组和各项回溯指标
		for (int i = 0; i < VARNUM * 2 + 2; i++)
			Act[i] = 0;
		for (int i = 0; i < VARNUM + 1; i++)
		{
			fuHao[i] = 0;
			reasonForFalse[i] = nullptr;
			level[i] = -1;
			result[i] = 0;
		}

		// 读取并初始化子句活跃度
		for (int i = 0; i < clauseNum; i++)
		{
			Clause* V = new Clause;
			while (1)
			{
				int tmp;
				fscanf(f, "%d", &tmp);
				if (tmp == 0)
					break;
				V->lits.push_back(lit(tmp));
				Act[lit(tmp).no()] += varIncrease;
			}
			clauses[i] = V; // 子句入表
			ClauseNum++; // 子句数目加一
		}

		// 检测各子句，并根据前两个文字的值插入观察队列
		for (int i = 0; i < clauseNum; i++)
		{
			if (clauses[i]->lits.size() > 1)
			{
				GetCheck[clauses[i]->lits[0].no()].push_back(clauses[i]);
				GetCheck[clauses[i]->lits[1].no()].push_back(clauses[i]);
			}
			else
			{
				if (MakeD(clauses[i]->lits[0].x) == 0)
					return -1;
			}
		}

		// 关闭文件
		fclose(f);

		// 返回文字数量
		return VARNUM;
	}

	void Print(long long timer, int literalnum, char filename[])//写入文件
	{
		int k = strlen(filename);
		filename[k - 1] = 's';
		filename[k - 2] = 'e';
		filename[k - 3] = 'r';
		FILE* f;
		if ((f = fopen(filename, "wb")) == NULL)
		{
			printf("write error");
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
};


//创建新子句并将其添加到观察程序列表
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
			//选择第二个需要观察的文字
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
			int tmp = ps[max_i].x;
			int t1 = ps[1].x;
			c->lits[1].x = tmp;
			c->lits[max_i].x = t1;
			//bumping
			S.IncreaseClasuseinAct(c);
			for (int i = 0; i < ps.size(); i++)
			{
				lit tmp = ps[i];
				tmp.x ^= 1;
				S.GetVARinAct(tmp);
			}
		}
		S.GetCheck[(c->lits[0]).no()].push_back(c);
		S.GetCheck[(c->lits[1]).no()].push_back(c);
		out_clause = c;
		return 1;
	}
}

//判断语句是不是单位语句，仅限学习语句
bool Clause::IsClause(Solver S)
{
	return S.reasonForFalse[this->lits[0].var()] == this;
}
//删除所有未被赋值的语句
bool Clause::GetCleanInClause(Solver& S)
{
	int j = 0;
	for (int i = 0; i < lits.size(); i++)
	{
		int opt;
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
		S.GetCheck[p.x].push_back(this);
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
			S.GetCheck[lits[1].no()].push_back(this);
			return 1;
		}
	}
	//子句在当前赋值下为单元子句
	S.GetCheck[p.x].push_back(this);
	return S.Enqueue(lits[0], this);
}

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
