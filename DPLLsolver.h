/*���༭��*/
/*������ 2023.9.3 23:33*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structure.h"
//��ؼ��Ľ���࣬��װ�˻�����������Ҫ�ĺ����ͳ�Ա����
class Solver
{
	friend Clause;
public:
	//Լ�����ݿ�
	Clause** constrs;         //�Ӿ��б�
	int constrs_size{ 0 };
	mQueue<Clause*> learnts;  //�ɳ�ͻ��������������ѧϰ���Ӿ��б�
	double cla_inc{ 1 };       //�Ӿ��Ծ������
	double cla_decay{ -1 };     //�Ӿ��Ծ��˥����
	int literalNum{ -1 };           //��������
	int clauseNum{ -1 };            //�Ӿ�����

	//����˳��
	double* activity;     //ͨ������ʽ����������������Ծ��
	double var_inc{ 1 };   //������Ծ�ȵ���������
	double var_decay{ 1 }; //������Ծ�ȵ�˥������

	//����ģ��
	mQueue<Clause*>* watches; //ÿ�����ֱ��۲��Լ���б�
	mQueue<lit> propQ;         //��������

	//����/��ֵ
	char* assigns;          //�����������ĵ�ǰ����
	mVector<lit> trail;     //��ʱ��˳�����еķ����б�
	mVector<int> trail_lim; //�ڡ�׷�ټ�¼�����������ֲ�ͬ���߲㼶�ķָ�������
	Clause** reason;       //Լ����ʾ�����ֻ�Ծֵ
	int* level;            //ÿ����������ֵ�ľ��߲㼶
	int root_level{ 0 };

	//���
	int* model;//����ģ��
	int model_size{ 0 };

	//��������
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
	//��������
	Clause* propagate()
	{
		while (!propQ.empty())
		{
			lit p = propQ.front();
			propQ.pop();
			mQueue<Clause*> tmp;
			while (!watches[p.x].empty())//ȡ�����й۲�����е�ֵ
			{
				Clause* t = watches[p.x].front();
				tmp.push_back(t);
				watches[p.x].pop();
			}
			//tmp = watches[p.x];
			//(*watches[p.x]).clear();
			while (!tmp.empty()) {//�����µĶ���
				Clause* c = tmp.front();
				tmp.pop();
				if ((*c).propagate(*this, p) == 0)
				{
					while (!tmp.empty()) {
						Clause* cl = tmp.front();
						tmp.pop();
						watches[p.x].push_back(cl);
					}
					while (!propQ.empty())
						propQ.pop();
					return c;
				}
			}
		}
		return nullptr;
	}

	bool enqueue(lit p, Clause* from)
	{
		if (this->assigns[p.var()] != Unset)
		{
			int opt;
			if (p.sign() == 0)
				opt = False;
			else
				opt = True;
			if (this->assigns[p.var()] != opt)
				return 0; //��֮��ͻ����������еĸ�ֵ
			else
				return 1; //���и�ֵ-���������
		}
		else
		{
			int x = p.var();
			if (p.sign() == 0)
			{
				assigns[p.var()] = False;
			}
			else
			{
				assigns[p.var()] = True;
			}
			level[p.var()] = decisionLevel();
			reason[p.var()] = from;
			trail.push_back(p);
			propQ.push_back(p);
			return 1;
		}
	}

	void analyze(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		int vars = nVars();
		bool* seen;
		seen = (bool*)malloc(vars * sizeof(bool*));
		for (int i = 0; i < nVars(); i++)
			seen[i] = false;
		int counter = 0;
		lit p;
		p.x = 0;
		mVector<lit> p_reason;
		out_learnt.push_back(p);
		out_btlevel = root_level;
		do
		{
			p_reason.clear();
			(*confl).calcReason(*this, p, p_reason);

			for (int j = 0; j < p_reason.size(); j++)
			{
				lit q = p_reason[j];
				if (!seen[q.var()])
				{
					seen[q.var()] = 1;
					if (level[q.var()] > root_level)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						varBumpActivity(q);
					}
					if (level[q.var()] == decisionLevel())
						counter++;
					else if (level[q.var()] > root_level)
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
				p = trail[trail.size() - 1];
				confl = reason[p.var()];
				undoOne();
			} while (!seen[p.var()]);
			counter--;
		} while (counter > 0);
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;
	}

	void record(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt = nullptr;
		int tmp = (*c).Clause_new(*this, clause, 1, learnt);
		if (learnt == nullptr)
			return;
		enqueue(clause[0], learnt);
		if (learnt != NULL)
		{
			learnts.push_back(learnt);
		}
	}

	void undoOne()
	{
		lit p = trail[trail.size() - 1];
		int x = p.var();
		assigns[x] = Unset;
		reason[x] = nullptr;
		level[x] = -1;
		activity[p.x] += var_inc;
		trail.pop_back();
	}

	bool assume(int num)
	{
		lit p;
		p.x = num;
		trail_lim.push_back(trail.size());
		return enqueue(p, nullptr);
	}

	void cancel()
	{
		int c = trail.size() - trail_lim[trail_lim.size() - 1];
		while (c)
		{
			undoOne();
			c--;
		}
		trail_lim.pop_back();
	}

	void cancelUntil(int level)
	{
		while (decisionLevel() > level)
			cancel();
	}

	int search(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		var_decay = 1 / params.var_decay;
		cla_decay = 1 / params.clause_decay;
		int conflictC = 0;
		//model.clear();
		for (int i = 0; i < literalNum + 1; i++)//��ʼ�����������
			model[i] = 0;
		model_size = 0;
		while (1)
		{
			Clause* confl = propagate();
			if (confl)
			{
				conflictC++;//��ͻ������
				if (decisionLevel() == root_level)//�жϾ��߲�
					return False;
				mVector<lit> learnt_clause;
				int backtr_level;
				analyze(confl, learnt_clause, backtr_level);//������ͻ
				cancelUntil(max(backtr_level, root_level));//����

				record(learnt_clause);//�Ӿ�ѧϰ
				if (learnt_clause.size() == 1)
				{
					level[learnt_clause[0].var()] = 0;
				}
				claDecayActivity();//�Ӿ�˥��
				varDecayActivity();//���ֻ˥��
			}
			else
			{
				if (decisionLevel() == root_level)//�жϾ��߲��Ƿ��Ǹ���
				{
					simplifyDB();//��������
				}
				if ((int)learnts.size() - (int)nAssigns() >= nof_learnt && nof_learnt >= 0)
				{
					reduceDB();
				}
				if (nAssigns() == nVars())//ȫ����ֵ
				{
					// Ѱ�Ҵ�
					for (int i = 1; i <= nVars(); i++)
					{
						model[i] = ((assigns[i] == True));
						model_size++;
					}
					cancelUntil(root_level);//���ݸ���
					return 1;
				}
				else
				{
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)//��ͻ�������ڱ�׼ֵ���ҳ�ͻ�������ڸ��㣬��������н⵫���ڱ�������
					{
						cancelUntil(root_level);//���ݸ���
						return Unset;
					}
					//Ѱ��һ����Ҫ��ֵ������
					int tmp = -1;
					double m = -1;
					lit p;

					for (int i = 1; i <= nVars(); i++)
					{
						p = lit(i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
						p = lit(-i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
					}
					if (tmp < 0)
					{
						for (int i = 1; i <= nVars(); i++)
						{
							model[i] = ((assigns[i] == True));
							model_size++;
						}
						cancelUntil(root_level);
						return 1;
					}
					assume(tmp ^ 1);
				}
			}
		}
	}

	void varBumpActivity(lit p)
	{
		if (var_decay < 0)
			return;
		if ((activity[p.x] += var_inc) > 1e100)
			varRescaleActivity();
	}
	void varRescaleActivity()
	{
		for (int i = 0; i <= literalNum * 2 + 1; i++)
		{
			activity[i] *= 1e-100;
		}
		var_inc *= 1e-100;
	}
	void varDecayActivity()
	{
		if (var_decay >= 0)
			var_inc *= var_decay;
	}
	void claDecayActivity()
	{
		cla_inc *= cla_decay;
	}
	void claRescaleActivity()
	{
		Clause** arr = learnts.tolist();

		for (int i = 0; i < learnts.size(); i++)
		{
			arr[i]->activity *= 1e-20;
			//learnts[i]->activity *= 1e-20;
			//p=p->next;
		}
		delete(arr);
		cla_inc *= 1e-20;
	}
	void claBumpActivity(Clause* c)
	{
		c->activity += cla_inc;
		if (c->activity > 1e20)
			claRescaleActivity();
	}
	void decayActivities()
	{
		varDecayActivity();
		claDecayActivity();
	}
	//���ţ������Ӿ��Ծ�ȶ��Ӿ�����
	void QuickSort(Clause**& a, int low, int high)
	{
		Clause* pivot;
		int i = low, j = high;
		if (low < high)
		{
			pivot = a[low]; //����һ��Ԫ����Ϊ����
			//���������е�һ�η���
			while (i < j)
			{
				while (i < j && a[j]->activity >= pivot->activity) //���������ҵ���һ��С���������
					j--;
				if (i < j)
				{ //�������ƶ�����ָ���λ��
					//a.set(i, a[j]);
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->activity < pivot->activity) //���������ҵ���һ�����ڵ����������
					i++;
				if (i < j)
				{ //�������ƶ�����ָ���λ��
					a[j] = a[i];
					//a.set(j, a[i]);
					j--;
				}
			}
			//a.set(i, pivot);
			a[i] = pivot; //ѭ���˳�ʱi == j�������λ��
			//�ݹ鴦��ǰ���������������
			QuickSort(a, low, i - 1);
			QuickSort(a, i + 1, high);
		}
	}

	void reduceDB()
	{
		int i, j;
		double lim = cla_inc / learnts.size();
		Clause** arr = learnts.tolist();
		QuickSort(arr, 0, learnts.size() - 1);
		int learnts_size = learnts.size();
		learnts.clear();
		for (i = j = 0; i < learnts_size / 2; i++)
		{
			if (!(*arr[i]).locked(*this))
			{
			}
			else
			{
				//learnts.set(j++, learnts[i]);
				//arr[j++] = arr[i];
				learnts.push_back(arr[i]);
			}
		}
		for (; i < learnts_size; i++)
		{
			if (!(*arr[i]).locked(*this) && (*arr[i]).activity < lim)
			{
			}
			else
			{
				//learnts.set(j++, learnts[i]);
				learnts.push_back(arr[i]);
				//arr[j++] = arr[i];
			}
		}


		delete(arr);
		//learnts.resize(j);
	}

	bool simplifyDB()//���򣬳��Զ�ÿһ���Ӿ���л��������������������ɾ��
	{
		if (propagate() != nullptr)//�ɳ�ͻ������
		{
			return 0;
		}

		Clause** cs = learnts.tolist();//�Ѷ���ת��Ϊ�б�
		int j = 0;
		learnts.clear();
		for (int i = 0; i < learnts.size(); i++)
		{
			if ((*cs[i]).simplify(*this))
			{
			}
			else
				//cs.set(j++, cs[i]);
				learnts.push_back(cs[i]);
			//cs[j++] = cs[i];
		}
		//cs.resize(j);
		delete(cs);
		j = 0;
		for (int i = 0; i < constrs_size; i++)
		{
			if ((*constrs[i]).simplify(*this)) {

			}
			else constrs[j++] = constrs[i];
		}
		constrs_size = j;
		return 1;
	}

	int solve()
	{
		SearchParams params(0.95, 0.999);
		double nof_conflicts = 100;//��ͻ����
		double nof_learnts = constrs_size / 3;//ѧϰ����
		int status = Unset;//0
		root_level = decisionLevel();//���߲�
		if (propagate())
		{
			cancelUntil(root_level);
			return False;
		}
		simplifyDB();
		while (status == Unset)
		{
			status = search((int)nof_conflicts, (int)nof_learnts, params);
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}
		cancelUntil(root_level);
		return status == True;
	}
	int read(char filename[])
	{
		FILE* f;
		char s[200];
		int k = 0, j = 0;
		if ((f = fopen(filename, "rb")) == NULL)
		{
			printf("�޷���!\n");
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
				literalNum = k;
				k = 0;
			}
			j++;
		}
		clauseNum = k;
		activity = (double*)malloc((literalNum * 2 + 2) * sizeof(double));
		assigns = (char*)malloc((literalNum + 1) * sizeof(char));//����
		reason = (Clause**)malloc((literalNum + 1) * sizeof(Clause*));//��ͻԭ��
		level = (int*)malloc((literalNum + 1) * (sizeof(int)));
		//watches.resize(2*literalNum+2);
		watches = (mQueue<Clause*>*)malloc((2 * literalNum + 2) * sizeof(mQueue<Clause*>));//�۲����
		model = (int*)malloc((literalNum + 1) * sizeof(int));
		//model.resize(literalNum + 1);
		constrs = (Clause**)malloc(clauseNum * sizeof(Clause*));
		for (int i = 0; i < 2 * literalNum + 2; i++) //�����۲����
		{
			//mVector tmp=mVector();
			//watches[i]=&tmp;
			watches[i] = mQueue<Clause*>();
			//watches[i]=(mVector*)malloc(sizeof(mVector));
		}
		//model = (int*)malloc((literalNum + 1) * sizeof(int*));
		for (int i = 0; i < literalNum * 2 + 2; i++)//��ʼ����Ծ������
			activity[i] = 0;
		for (int i = 0; i < literalNum + 1; i++)//��ʼ���������ָ��
		{
			assigns[i] = 0;
			reason[i] = nullptr;
			level[i] = -1;
			model[i] = 0;
		}
		for (int i = 0; i < clauseNum; i++)//���벢�ҳ�ʼ����Ծ�ȣ���һ����+1*var_inc
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
			constrs[i] = V;//�Ӿ����
			constrs_size++;//�Ӿ���Ŀ��һ
		}
		for (int i = 0; i < clauseNum; i++)//�������Ӿ䣬������ǰ�������ֵ�ֵ����۲����
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
		fclose(f);
		return literalNum;
	}

	void write(long long timer, int literalnum, char filename[])
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


//�������Ӿ䲢������ӵ��۲�����б�
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
			//ѡ��ڶ�����Ҫ�۲������
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

//�ж�����ǲ��ǵ�λ��䣬����ѧϰ���
bool Clause::locked(Solver S)
{
	return S.reason[this->lits[0].var()] == this;
}
//ɾ������δ����ֵ�����
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
		if (S.assigns[lits[i].var()] == opt)//�Ӿ�����
			return 1;
		if (S.assigns[lits[i].var()] == Unset)//δ����ֵ����
		{
			lits[j++] = lits[i];
		}
	}
	lits.resize(j);
	return 0;
}

bool Clause::propagate(Solver& S, lit p)
{
	//ȷ����������lits[1]
	if (lits[0].x == p.no())
	{
		lits[0].x = lits[1].x;
		lits[1].x = p.no();
	}
	//�����0���۲����Ϊ��,���Ӿ�������
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
	//Ѱ���µ��������۲�
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
	//�Ӿ��ڵ�ǰ��ֵ��Ϊ��Ԫ�Ӿ�
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
