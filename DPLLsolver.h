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
	//约束数据库
	Clause** constrs;         //子句列表
	int constrs_size{ 0 };
	mQueue<Clause*> learnts;  //由冲突分析得来的用于学习的子句列表
	double cla_inc{ 1 };       //子句活跃度增量
	double cla_decay{ -1 };     //子句活跃的衰减量
	int literalNum{ -1 };           //文字数量
	int clauseNum{ -1 };            //子句数量

	//变量顺序
	double* activity;     //通过启发式方法来衡量变量活跃度
	double var_inc{ 1 };   //变量活跃度的增加因子
	double var_decay{ 1 }; //变量活跃度的衰减因子

	//传播模块
	mQueue<Clause*>* watches; //每个文字被观察的约束列表
	mQueue<lit> propQ;         //传播队列

	//分配/赋值
	char* assigns;          //按变量索引的当前分配
	mVector<lit> trail;     //按时间顺序排列的分配列表
	mVector<int> trail_lim; //在“追踪记录”中用于区分不同决策层级的分隔符索引
	Clause** reason;       //约束表示的文字活跃值
	int* level;            //每个变量被赋值的决策层级
	int root_level{ 0 };

	//结果
	int* model;//定义模型
	int model_size{ 0 };

	//辅助函数
	int nVars()
	{
		return this->literalNum;
	}
	int nAssigns()
	{
		return trail.size();
	}
	int nConstraints()
	{
		return this->clauseNum;
	}
	int nLearnts()
	{
		return learnts.size();
	}
	int decisionLevel()
	{
		return trail_lim.size();
	}
	//传播化简函数
	Clause* propagate()
	{
		// 当'propQ'队列不为空时，循环继续
		while (!propQ.empty())
		{
			// 从'propQ'队列的前端获取元素，赋值给'p'
			lit p = propQ.front();
			propQ.pop();

			// 创建临时队列'tmp'
			mQueue<Clause*> tmp;

			// 当'watches'的子队列不为空时，循环继续
			while (!watches[p.x].empty())//取出所有观测队列中的值
			{
				// 从'watches'的子队列前端获取元素，赋值给't'
				Clause* t = watches[p.x].front();
				tmp.push_back(t);
				watches[p.x].pop();
			}

			// 对新的队列'tmp'进行处理
			while (!tmp.empty())
			{
				// 从'tmp'队列前端获取元素，赋值给'c'
				Clause* c = tmp.front();
				tmp.pop();

				// 如果'c'的'propagate'方法返回0，执行下面的语句
				if ((*c).propagate(*this, p) == 0)
				{
					// 当'tmp'队列不为空时，循环继续
					while (!tmp.empty())
					{
						// 从'tmp'队列前端获取元素，赋值给'cl'
						Clause* cl = tmp.front();
						tmp.pop();

						// 将'cl'添加到'watches'的子队列中
						watches[p.x].push_back(cl);
					}
					// 清空'propQ'队列
					while (!propQ.empty())
						propQ.pop();
					return c;
				}
			}
		}

		// 如果执行到这里，说明'propQ'队列已经为空，返回空指针
		return nullptr;
	}

	bool enqueue(lit p, Clause* from)
	{
		// 如果'p'的变量已经被赋值
		if (this->assigns[p.var()] != Unset)
		{
			int opt;
			// 如果'p'的符号为0，设置'opt'为False
			if (p.sign() == 0)
				opt = False;
			else
				// 否则设置'opt'为True
				opt = True;
			// 如果新的赋值'opt'和原有的赋值冲突，返回0
			if (this->assigns[p.var()] != opt)
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
				assigns[p.var()] = False;
			}
			else
			{
				assigns[p.var()] = True;
			}

			// 设置'p'的变量在决策层次中的位置
			level[p.var()] = decisionLevel();
			reason[p.var()] = from;
			// 将'p'添加到'trail'队列
			trail.push_back(p);
			// 将'p'添加到'propQ'队列
			propQ.push_back(p);
			return 1;
		}
	}

	void analyze(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		// 获取变量的数量
		int vars = nVars();
		bool* seen;
		// 分配内存空间给变量是否被看过的标记数组
		seen = (bool*)malloc(vars * sizeof(bool*));
		// 初始化所有的标记为false
		for (int i = 0; i < nVars(); i++)
			seen[i] = false;
		// 初始化计数器和文字'p'
		int counter = 0;
		lit p;
		p.x = 0;
		// 创建一个包含'p'原因的向量'p_reason'
		mVector<lit> p_reason;
		out_learnt.push_back(p);
		out_btlevel = root_level;

		// 进入循环，直到计数器为0
		do
		{
			// 清空'p_reason'向量，并计算'confl'子句的原因
			p_reason.clear();
			(*confl).calcReason(*this, p, p_reason);

			// 遍历'p_reason'向量中的所有元素
			for (int j = 0; j < p_reason.size(); j++)
			{
				lit q = p_reason[j];
				// 如果文字'q'的变量还没被看过
				if (!seen[q.var()])
				{
					// 标记文字'q'的变量
					seen[q.var()] = 1;
					// 如果文字'q'的变量的决策层次大于根层次，将文字'q'的符号反转，并增加变量的活跃度
					if (level[q.var()] > root_level)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						varBumpActivity(q);
					}
					// 如果文字'q'的变量的决策层次等于当前决策层次，增加计数器
					if (level[q.var()] == decisionLevel())
						counter++;
					// 否则如果文字'q'的变量的决策层次大于根层次，将文字'q'的符号反转，并添加到学习的子句中，更新反向决策层次
					else if (level[q.var()] > root_level)
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
				// 从'trail'向量的末尾获取文字'p'，并获取文字'p'的原因
				p = trail[trail.size() - 1];
				confl = reason[p.var()];
				// 撤销上一步的决策
				undoOne();
			} while (!seen[p.var()]);
			counter--;
		} while (counter > 0);
		// 将第一个学习的文字设置为文字'p'的否定
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;
	}

	void record(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt = nullptr;
		int tmp = (*c).Clause_new(*this, clause, 1, learnt);//创建学习子句
		if (learnt == nullptr)//没有可以学习的了
			return;
		enqueue(clause[0], learnt);//添加到学习列表
		if (learnt != NULL)//
			learnts.push_back(learnt);
	}

	void undoOne()
	{
		// 从'trail'向量的末尾获取文字'p'
		lit p = trail[trail.size() - 1]; 
		// 获取'p'的变量编号
		int x = p.var(); 
		// 将变量'x'的赋值状态设为未设置
		assigns[x] = Unset;
		// 将变量'x'的原因设为nullptr
		reason[x] = nullptr;
		// 将变量'x'的决策层次设为-1
		level[x] = -1;
		// 增加文字'p'的活跃度
		activity[p.x] += var_inc;
		// 从'trail'向量中移除最后一个元素
		trail.pop_back();
	}

	bool assume(int num)
	{
		// 创建一个新的文字'p'，并将其设置为输入的数字
		lit p; 
		p.x = num; 
		// 将当前'trail'向量的大小加入到'trail_lim'向量中，这可以用来标记决策层次的开始
		trail_lim.push_back(trail.size()); 
		// 将文字'p'加入到'trail'向量中，并设置它的原因为nullptr（因为这是一个新的决策，而不是由于某个子句的推导得出）
		return enqueue(p, nullptr); 
	}

	void cancel()
	{
		// 计算出当前决策层次的大小，即'trail'向量与'trail_lim'向量最后一个元素的差
		int c = trail.size() - trail_lim[trail_lim.size() - 1];
		// 使用循环连续撤销这个决策层次的所有决策
		while (c)
		{
			// 调用'undoOne'函数撤销最后一个决策
			undoOne();
			c--;
		}
		// 从'trail_lim'向量中移除最后一个元素，即这个被撤销的决策层次的开始位置
		trail_lim.pop_back();
	}

	void cancelUntil(int level)
	{
		// 使用循环撤销决策，直到当前决策层次不大于目标决策层次
		while (decisionLevel() > level)
			// 调用'cancel'函数撤销最上层的决策
			cancel();
	}

	int search(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// 设置活跃度衰减因子
		var_decay = 1 / params.var_decay;
		cla_decay = 1 / params.clause_decay;
		// 初始化冲突计数器
		int conflictC = 0;
		// 初始化结果管理器
		for (int i = 0; i < literalNum + 1; i++)
			model[i] = 0;
		model_size = 0;
		// 主循环开始
		while (1)
		{
			// 调用'propagate'函数进行推导，并获取冲突子句
			Clause* confl = propagate();
			// 如果存在冲突子句
			if (confl)
			{
				// 冲突计数器增加
				conflictC++;
				// 如果冲突发生在根决策层次，返回False
				if (decisionLevel() == root_level)
					return False;
				// 初始化学习子句和回溯层次
				mVector<lit> learnt_clause;
				int backtr_level;
				// 调用'analyze'函数分析冲突，获取学习子句和回溯层次
				analyze(confl, learnt_clause, backtr_level);
				// 回溯到最大的决策层次
				cancelUntil(max(backtr_level, root_level));
				// 记录学习子句
				record(learnt_clause);
				// 如果学习子句只有一个文字，设置其决策层次为0
				if (learnt_clause.size() == 1)
					level[learnt_clause[0].var()] = 0;
				// 子句和文字活动量衰减
				claDecayActivity();
				varDecayActivity();
			}
			else
			{
				// 如果当前决策层次为根层次，化简数据
				if (decisionLevel() == root_level)
					simplifyDB();
				// 如果学习子句的数量超过限制，进行数据库压缩
				if ((int)learnts.size() - (int)nAssigns() >= nof_learnt && nof_learnt >= 0)
					reduceDB();
				// 如果所有变量都已赋值，找到一个解
				if (nAssigns() == nVars())
				{
					// 组装解
					for (int i = 1; i <= nVars(); i++)
					{
						model[i] = ((assigns[i] == True));
						model_size++;
					}
					// 回溯到根决策层次
					cancelUntil(root_level);
					return 1;
				}
				else
				{
					// 如果冲突数量超过限制，回溯到根决策层次并返回Unset
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						cancelUntil(root_level);
						return Unset;
					}
					// 寻找一个未赋值的活跃文字
					int tmp = -1;
					double m = -1;
					lit p;
					for (int i = 1; i <= nVars(); i++)
					{
						// 正文字
						p = lit(i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
						// 负文字
						p = lit(-i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
					}
					if (tmp < 0)
					{
						// 如果找不到需要赋值的文字，则找到了一个解
						for (int i = 1; i <= nVars(); i++)
						{
							model[i] = ((assigns[i] == True));
							model_size++;
						}
						// 回溯到根层
						cancelUntil(root_level);
						return 1;
					}
					// 对找到的需要赋值的文字进行决策
					assume(tmp ^ 1);
				}
			}
		}
	}

	void varBumpActivity(lit p)
	{
		// 如果var_decay小于0，则直接返回
		if (var_decay < 0)
			return;
		// 增加变量p的活动性
		if ((activity[p.x] += var_inc) > 1e100)
			// 如果变量的活动性过大，调用函数进行缩放
			varRescaleActivity();
	}

	void varRescaleActivity()
	{
		// 遍历所有的文字，缩小他们的活动性
		for (int i = 0; i <= literalNum * 2 + 1; i++)
		{
			activity[i] *= 1e-100;
		}
		// 缩小活动性增加的步长
		var_inc *= 1e-100;
	}

	void varDecayActivity()
	{
		// 如果var_decay大于等于0，减小活动性增加的步长
		if (var_decay >= 0)
			var_inc *= var_decay;
	}

	void claDecayActivity()
	{
		// 减小子句活动性增加的步长
		cla_inc *= cla_decay;
	}

	void claRescaleActivity()
	{
		// 将所有学习到的子句转换为指针数组
		Clause** arr = learnts.tolist();

		// 遍历所有学习到的子句
		for (int i = 0; i < learnts.size(); i++)
		{
			// 缩小子句的活动性
			arr[i]->activity *= 1e-20;
			//learnts[i]->activity *= 1e-20;
			//p=p->next;
		}
		// 删除子句指针数组
		delete(arr);
		// 缩小子句活动性增加的步长
		cla_inc *= 1e-20;
	}

	void claBumpActivity(Clause* c)
	{
		// 增加子句c的活动性
		c->activity += cla_inc;
		// 如果子句的活动性过大，调用函数进行缩放
		if (c->activity > 1e20)
			claRescaleActivity();
	}

	void decayActivities()
	{
		// 调用函数减小变量活动性增加的步长
		varDecayActivity();
		// 调用函数减小子句活动性增加的步长
		claDecayActivity();
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
				while (i < j && a[j]->activity >= pivot->activity) //从右往左找到第一个小于枢轴的数
					j--;
				if (i < j)
				{ //将该数移动到左指针的位置
					//a.set(i, a[j]);
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->activity < pivot->activity) //从左往右找到第一个大于等于枢轴的数
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

	void reduceDB()//删除部分学习子句
	{
		int i, j;
		// 计算子句活动性的阈值
		double lim = cla_inc / learnts.size();

		// 将所有学习到的子句转换为指针数组
		Clause** arr = learnts.tolist();

		// 对数组进行快速排序
		QuickSort(arr, 0, learnts.size() - 1);

		int learnts_size = learnts.size();

		// 清空学习到的子句列表
		learnts.clear();

		// 遍历数组的前半部分
		for (i = j = 0; i < learnts_size / 2; i++)
		{
			// 如果子句没有被锁定，则跳过
			if (!(*arr[i]).locked(*this))
			{
			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learnts.push_back(arr[i]);
			}
		}

		// 遍历数组的后半部分
		for (; i < learnts_size; i++)
		{
			// 如果子句没有被锁定并且其活动性小于阈值，则跳过
			if (!(*arr[i]).locked(*this) && (*arr[i]).activity < lim)
			{
			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learnts.push_back(arr[i]);
			}
		}

		// 删除子句指针数组
		delete(arr);
	}

	bool simplifyDB()//化简，尝试对每一个子句进行化简，如果化简完毕且满足就删除
	{
		// 如果存在冲突，则不进行化简
		if (propagate() != nullptr)//由冲突不化简
		{
			return 0;
		}

		// 将所有学习到的子句转换为指针数组
		Clause** cs = learnts.tolist();

		// 清空学习到的子句列表
		learnts.clear();

		int j = 0;
		// 对每一个学习到的子句进行化简
		for (int i = 0; i < learnts.size(); i++)
		{
			// 如果子句可以被化简，则跳过
			if ((*cs[i]).simplify(*this))
			{

			}
			else
			{
				// 否则，将该子句添加回学习到的子句列表
				learnts.push_back(cs[i]);
			}
		}

		// 删除子句指针数组
		delete(cs);

		j = 0;
		// 对每一个约束子句进行化简
		for (int i = 0; i < constrs_size; i++)
		{
			// 如果子句可以被化简，则跳过
			if ((*constrs[i]).simplify(*this))
			{

			}
			else
			{
				// 否则，将该子句添加回约束子句列表
				constrs[j++] = constrs[i];
			}
		}
		// 更新约束子句的数量
		constrs_size = j;

		return 1;
	}

	int solve()
	{
		// 初始化搜索参数，包括文字和子句的衰变参数
		SearchParams params(0.95, 0.999);

		// 初始化冲突参数，这个参数可用于指导搜索过程
		double nof_conflicts = 100;

		// 初始化学习参数，一开始设置为约束子句数量的1/3
		double nof_learnts = constrs_size / 3;

		// 初始化求解状态为未设定（Unset）
		int status = Unset;

		// 获取当前的决策层级，并设置为根层级
		root_level = decisionLevel();

		// 如果在顶层就存在冲突，表示无解
		if (propagate())
		{
			cancelUntil(root_level);
			return False;
		}

		// 对数据库进行化简
		simplifyDB();

		// 如果求解状态尚未确定，则进入循环，持续进行搜索
		while (status == Unset)
		{
			// 在搜索过程中，会更新求解状态
			status = search((int)nof_conflicts, (int)nof_learnts, params);

			// 每次搜索完毕后，都会调整冲突参数和学习参数，以指导下一次的搜索，把速度交给天意！！！！
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}

		// 恢复到搜索之前的状态，取消所有在根层级之上的决策
		cancelUntil(root_level);

		// 如果最终的求解状态是真（True），则返回真；否则，返回假（False）
		return status == True;
	}

	int read(char filename[])
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
				literalNum = k;
				k = 0;
			}
			j++;
		}
		clauseNum = k;

		// 初始化数据结构
		activity = (double*)malloc((literalNum * 2 + 2) * sizeof(double));
		assigns = (char*)malloc((literalNum + 1) * sizeof(char)); // 分配
		reason = (Clause**)malloc((literalNum + 1) * sizeof(Clause*)); // 冲突原因
		level = (int*)malloc((literalNum + 1) * (sizeof(int)));
		watches = (mQueue<Clause*>*)malloc((2 * literalNum + 2) * sizeof(mQueue<Clause*>)); // 观察队列
		model = (int*)malloc((literalNum + 1) * sizeof(int));
		constrs = (Clause**)malloc(clauseNum * sizeof(Clause*));

		// 初始化观察队列
		for (int i = 0; i < 2 * literalNum + 2; i++)
			watches[i] = mQueue<Clause*>();

		// 初始化活跃度数组和各项回溯指标
		for (int i = 0; i < literalNum * 2 + 2; i++)
			activity[i] = 0;
		for (int i = 0; i < literalNum + 1; i++)
		{
			assigns[i] = 0;
			reason[i] = nullptr;
			level[i] = -1;
			model[i] = 0;
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
				activity[lit(tmp).no()] += var_inc;
			}
			constrs[i] = V; // 子句入表
			constrs_size++; // 子句数目加一
		}

		// 检测各子句，并根据前两个文字的值插入观察队列
		for (int i = 0; i < clauseNum; i++)
		{
			if (constrs[i]->lits.size() > 1)
			{
				watches[constrs[i]->lits[0].no()].push_back(constrs[i]);
				watches[constrs[i]->lits[1].no()].push_back(constrs[i]);
			}
			else
			{
				if (assume(constrs[i]->lits[0].x) == 0)
					return -1;
			}
		}

		// 关闭文件
		fclose(f);

		// 返回文字数量
		return literalNum;
	}

	void write(long long timer, int literalnum, char filename[])//写入文件
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
		fprintf(f, "s %d\n", model_size == literalnum);
		if (model_size == literalnum)
		{
			fprintf(f, "v ");
			for (int i = 1; i <= literalnum; i++)
			{
				fprintf(f, "%d ", (model[i] == 0) ? (-(i)) : (i));
			}
			fprintf(f, "\n");
		}
		fprintf(f, "t %lld", timer);
		fclose(f);
	}
};


//创建新子句并将其添加到观察程序列表
bool Clause::Clause_new(Solver& S, mVector<lit> ps, bool learnt, Clause*& out_clause)
{
	out_clause = NULL;
	if (!learnt)
	{
		for (int i = 0; i < ps.size(); i++)
		{
			if (S.assigns[ps[i].var()] == True)
				return 1;
		}
	}
	if (ps.size() == 0)
		return 0;
	else if (ps.size() == 1)
	{
		Clause* tmp = new Clause;
		return S.enqueue(ps[0], tmp);
	}
	else
	{
		Clause* c = new Clause;
		c->lits = ps;
		c->learnt = learnt;
		c->activity = 0;
		if (learnt)
		{
			//选择第二个需要观察的文字
			double mx = -1;
			int max_i = 0;
			for (int i = 0; i < ps.size(); i++)
			{
				if (S.activity[ps[i].x] > mx)
				{
					mx = S.activity[ps[i].x];
					max_i = i;
				}
			}
			int tmp = ps[max_i].x;
			int t1 = ps[1].x;
			c->lits[1].x = tmp;
			c->lits[max_i].x = t1;
			//bumping
			S.claBumpActivity(c);
			for (int i = 0; i < ps.size(); i++)
			{
				lit tmp = ps[i];
				tmp.x ^= 1;
				S.varBumpActivity(tmp);
			}
		}
		S.watches[(c->lits[0]).no()].push_back(c);
		S.watches[(c->lits[1]).no()].push_back(c);
		out_clause = c;
		return 1;
	}
}

//判断语句是不是单位语句，仅限学习语句
bool Clause::locked(Solver S)
{
	return S.reason[this->lits[0].var()] == this;
}
//删除所有未被赋值的语句
bool Clause::simplify(Solver& S)
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
		if (S.assigns[lits[i].var()] == opt)//子句满足
			return 1;
		if (S.assigns[lits[i].var()] == Unset)//未被赋值则保留
		{
			lits[j++] = lits[i];
		}
	}
	lits.resize(j);
	return 0;
}

bool Clause::propagate(Solver& S, lit p)
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
	if (S.assigns[lits[0].var()] == opt)
	{
		S.watches[p.x].push_back(this);
		return true;
	}
	//寻找新的文字来观察
	for (int i = 2; i < lits.size(); i++)
	{
		if (lits[i].sign() == 0)
			opt = True;
		else
			opt = False;
		if (S.assigns[lits[i].var()] != opt)
		{
			int tm = lits[1].x;
			lits[1].x = lits[i].x;
			lits[i].x = tm;
			S.watches[lits[1].no()].push_back(this);
			return 1;
		}
	}
	//子句在当前赋值下为单元子句
	S.watches[p.x].push_back(this);
	return S.enqueue(lits[0], this);
}

void Clause::calcReason(Solver& S, lit p, mVector<lit>& out_reason)
{
	int i = 1;
	if (S.assigns[p.var()] == Unset)
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
		S.claBumpActivity(this);
}
