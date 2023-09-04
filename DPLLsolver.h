/*���༭��*/
/*������ 2023.9.4 21.39*/
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
	//����������
	Clause* propagate()
	{
		// ��'propQ'���в�Ϊ��ʱ��ѭ������
		while (!propQ.empty())
		{
			// ��'propQ'���е�ǰ�˻�ȡԪ�أ���ֵ��'p'
			lit p = propQ.front();
			propQ.pop();

			// ������ʱ����'tmp'
			mQueue<Clause*> tmp;

			// ��'watches'���Ӷ��в�Ϊ��ʱ��ѭ������
			while (!watches[p.x].empty())//ȡ�����й۲�����е�ֵ
			{
				// ��'watches'���Ӷ���ǰ�˻�ȡԪ�أ���ֵ��'t'
				Clause* t = watches[p.x].front();
				tmp.push_back(t);
				watches[p.x].pop();
			}

			// ���µĶ���'tmp'���д���
			while (!tmp.empty())
			{
				// ��'tmp'����ǰ�˻�ȡԪ�أ���ֵ��'c'
				Clause* c = tmp.front();
				tmp.pop();

				// ���'c'��'propagate'��������0��ִ����������
				if ((*c).propagate(*this, p) == 0)
				{
					// ��'tmp'���в�Ϊ��ʱ��ѭ������
					while (!tmp.empty())
					{
						// ��'tmp'����ǰ�˻�ȡԪ�أ���ֵ��'cl'
						Clause* cl = tmp.front();
						tmp.pop();

						// ��'cl'��ӵ�'watches'���Ӷ�����
						watches[p.x].push_back(cl);
					}
					// ���'propQ'����
					while (!propQ.empty())
						propQ.pop();
					return c;
				}
			}
		}

		// ���ִ�е����˵��'propQ'�����Ѿ�Ϊ�գ����ؿ�ָ��
		return nullptr;
	}

	bool enqueue(lit p, Clause* from)
	{
		// ���'p'�ı����Ѿ�����ֵ
		if (this->assigns[p.var()] != Unset)
		{
			int opt;
			// ���'p'�ķ���Ϊ0������'opt'ΪFalse
			if (p.sign() == 0)
				opt = False;
			else
				// ��������'opt'ΪTrue
				opt = True;
			// ����µĸ�ֵ'opt'��ԭ�еĸ�ֵ��ͻ������0
			if (this->assigns[p.var()] != opt)
				return 0;
			else
				// ����µĸ�ֵ'opt'��ԭ�еĸ�ֵ����ͻ������1
				return 1;
		}
		else
		{
			int x = p.var();
			// ���'p'�ķ���Ϊ0����'p'�ı�����ֵΪFalse
			if (p.sign() == 0)
			{
				assigns[p.var()] = False;
			}
			else
			{
				assigns[p.var()] = True;
			}

			// ����'p'�ı����ھ��߲���е�λ��
			level[p.var()] = decisionLevel();
			reason[p.var()] = from;
			// ��'p'��ӵ�'trail'����
			trail.push_back(p);
			// ��'p'��ӵ�'propQ'����
			propQ.push_back(p);
			return 1;
		}
	}

	void analyze(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		// ��ȡ����������
		int vars = nVars();
		bool* seen;
		// �����ڴ�ռ�������Ƿ񱻿����ı������
		seen = (bool*)malloc(vars * sizeof(bool*));
		// ��ʼ�����еı��Ϊfalse
		for (int i = 0; i < nVars(); i++)
			seen[i] = false;
		// ��ʼ��������������'p'
		int counter = 0;
		lit p;
		p.x = 0;
		// ����һ������'p'ԭ�������'p_reason'
		mVector<lit> p_reason;
		out_learnt.push_back(p);
		out_btlevel = root_level;

		// ����ѭ����ֱ��������Ϊ0
		do
		{
			// ���'p_reason'������������'confl'�Ӿ��ԭ��
			p_reason.clear();
			(*confl).calcReason(*this, p, p_reason);

			// ����'p_reason'�����е�����Ԫ��
			for (int j = 0; j < p_reason.size(); j++)
			{
				lit q = p_reason[j];
				// �������'q'�ı�����û������
				if (!seen[q.var()])
				{
					// �������'q'�ı���
					seen[q.var()] = 1;
					// �������'q'�ı����ľ��߲�δ��ڸ���Σ�������'q'�ķ��ŷ�ת�������ӱ����Ļ�Ծ��
					if (level[q.var()] > root_level)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						varBumpActivity(q);
					}
					// �������'q'�ı����ľ��߲�ε��ڵ�ǰ���߲�Σ����Ӽ�����
					if (level[q.var()] == decisionLevel())
						counter++;
					// �����������'q'�ı����ľ��߲�δ��ڸ���Σ�������'q'�ķ��ŷ�ת������ӵ�ѧϰ���Ӿ��У����·�����߲��
					else if (level[q.var()] > root_level)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						out_learnt.push_back(tmp);
						out_btlevel = (out_btlevel > level[q.var()]) ? out_btlevel : level[q.var()];
					}
				}
			}
			// ����ѭ����ֱ������'p'�ı���������
			do
			{
				// ��'trail'������ĩβ��ȡ����'p'������ȡ����'p'��ԭ��
				p = trail[trail.size() - 1];
				confl = reason[p.var()];
				// ������һ���ľ���
				undoOne();
			} while (!seen[p.var()]);
			counter--;
		} while (counter > 0);
		// ����һ��ѧϰ����������Ϊ����'p'�ķ�
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;
	}

	void record(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt = nullptr;
		int tmp = (*c).Clause_new(*this, clause, 1, learnt);//����ѧϰ�Ӿ�
		if (learnt == nullptr)//û�п���ѧϰ����
			return;
		enqueue(clause[0], learnt);//��ӵ�ѧϰ�б�
		if (learnt != NULL)//
			learnts.push_back(learnt);
	}

	void undoOne()
	{
		// ��'trail'������ĩβ��ȡ����'p'
		lit p = trail[trail.size() - 1]; 
		// ��ȡ'p'�ı������
		int x = p.var(); 
		// ������'x'�ĸ�ֵ״̬��Ϊδ����
		assigns[x] = Unset;
		// ������'x'��ԭ����Ϊnullptr
		reason[x] = nullptr;
		// ������'x'�ľ��߲����Ϊ-1
		level[x] = -1;
		// ��������'p'�Ļ�Ծ��
		activity[p.x] += var_inc;
		// ��'trail'�������Ƴ����һ��Ԫ��
		trail.pop_back();
	}

	bool assume(int num)
	{
		// ����һ���µ�����'p'������������Ϊ���������
		lit p; 
		p.x = num; 
		// ����ǰ'trail'�����Ĵ�С���뵽'trail_lim'�����У������������Ǿ��߲�εĿ�ʼ
		trail_lim.push_back(trail.size()); 
		// ������'p'���뵽'trail'�����У�����������ԭ��Ϊnullptr����Ϊ����һ���µľ��ߣ�����������ĳ���Ӿ���Ƶ��ó���
		return enqueue(p, nullptr); 
	}

	void cancel()
	{
		// �������ǰ���߲�εĴ�С����'trail'������'trail_lim'�������һ��Ԫ�صĲ�
		int c = trail.size() - trail_lim[trail_lim.size() - 1];
		// ʹ��ѭ����������������߲�ε����о���
		while (c)
		{
			// ����'undoOne'�����������һ������
			undoOne();
			c--;
		}
		// ��'trail_lim'�������Ƴ����һ��Ԫ�أ�������������ľ��߲�εĿ�ʼλ��
		trail_lim.pop_back();
	}

	void cancelUntil(int level)
	{
		// ʹ��ѭ���������ߣ�ֱ����ǰ���߲�β�����Ŀ����߲��
		while (decisionLevel() > level)
			// ����'cancel'�����������ϲ�ľ���
			cancel();
	}

	int search(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// ���û�Ծ��˥������
		var_decay = 1 / params.var_decay;
		cla_decay = 1 / params.clause_decay;
		// ��ʼ����ͻ������
		int conflictC = 0;
		// ��ʼ�����������
		for (int i = 0; i < literalNum + 1; i++)
			model[i] = 0;
		model_size = 0;
		// ��ѭ����ʼ
		while (1)
		{
			// ����'propagate'���������Ƶ�������ȡ��ͻ�Ӿ�
			Clause* confl = propagate();
			// ������ڳ�ͻ�Ӿ�
			if (confl)
			{
				// ��ͻ����������
				conflictC++;
				// �����ͻ�����ڸ����߲�Σ�����False
				if (decisionLevel() == root_level)
					return False;
				// ��ʼ��ѧϰ�Ӿ�ͻ��ݲ��
				mVector<lit> learnt_clause;
				int backtr_level;
				// ����'analyze'����������ͻ����ȡѧϰ�Ӿ�ͻ��ݲ��
				analyze(confl, learnt_clause, backtr_level);
				// ���ݵ����ľ��߲��
				cancelUntil(max(backtr_level, root_level));
				// ��¼ѧϰ�Ӿ�
				record(learnt_clause);
				// ���ѧϰ�Ӿ�ֻ��һ�����֣���������߲��Ϊ0
				if (learnt_clause.size() == 1)
					level[learnt_clause[0].var()] = 0;
				// �Ӿ�����ֻ��˥��
				claDecayActivity();
				varDecayActivity();
			}
			else
			{
				// �����ǰ���߲��Ϊ����Σ���������
				if (decisionLevel() == root_level)
					simplifyDB();
				// ���ѧϰ�Ӿ�������������ƣ��������ݿ�ѹ��
				if ((int)learnts.size() - (int)nAssigns() >= nof_learnt && nof_learnt >= 0)
					reduceDB();
				// ������б������Ѹ�ֵ���ҵ�һ����
				if (nAssigns() == nVars())
				{
					// ��װ��
					for (int i = 1; i <= nVars(); i++)
					{
						model[i] = ((assigns[i] == True));
						model_size++;
					}
					// ���ݵ������߲��
					cancelUntil(root_level);
					return 1;
				}
				else
				{
					// �����ͻ�����������ƣ����ݵ������߲�β�����Unset
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						cancelUntil(root_level);
						return Unset;
					}
					// Ѱ��һ��δ��ֵ�Ļ�Ծ����
					int tmp = -1;
					double m = -1;
					lit p;
					for (int i = 1; i <= nVars(); i++)
					{
						// ������
						p = lit(i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
						// ������
						p = lit(-i);
						if (m < activity[p.index()] && assigns[p.var()] == Unset)
						{
							tmp = p.index();
							m = activity[p.index()];
						}
					}
					if (tmp < 0)
					{
						// ����Ҳ�����Ҫ��ֵ�����֣����ҵ���һ����
						for (int i = 1; i <= nVars(); i++)
						{
							model[i] = ((assigns[i] == True));
							model_size++;
						}
						// ���ݵ�����
						cancelUntil(root_level);
						return 1;
					}
					// ���ҵ�����Ҫ��ֵ�����ֽ��о���
					assume(tmp ^ 1);
				}
			}
		}
	}

	void varBumpActivity(lit p)
	{
		// ���var_decayС��0����ֱ�ӷ���
		if (var_decay < 0)
			return;
		// ���ӱ���p�Ļ��
		if ((activity[p.x] += var_inc) > 1e100)
			// ��������Ļ�Թ��󣬵��ú�����������
			varRescaleActivity();
	}

	void varRescaleActivity()
	{
		// �������е����֣���С���ǵĻ��
		for (int i = 0; i <= literalNum * 2 + 1; i++)
		{
			activity[i] *= 1e-100;
		}
		// ��С������ӵĲ���
		var_inc *= 1e-100;
	}

	void varDecayActivity()
	{
		// ���var_decay���ڵ���0����С������ӵĲ���
		if (var_decay >= 0)
			var_inc *= var_decay;
	}

	void claDecayActivity()
	{
		// ��С�Ӿ������ӵĲ���
		cla_inc *= cla_decay;
	}

	void claRescaleActivity()
	{
		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** arr = learnts.tolist();

		// ��������ѧϰ�����Ӿ�
		for (int i = 0; i < learnts.size(); i++)
		{
			// ��С�Ӿ�Ļ��
			arr[i]->activity *= 1e-20;
			//learnts[i]->activity *= 1e-20;
			//p=p->next;
		}
		// ɾ���Ӿ�ָ������
		delete(arr);
		// ��С�Ӿ������ӵĲ���
		cla_inc *= 1e-20;
	}

	void claBumpActivity(Clause* c)
	{
		// �����Ӿ�c�Ļ��
		c->activity += cla_inc;
		// ����Ӿ�Ļ�Թ��󣬵��ú�����������
		if (c->activity > 1e20)
			claRescaleActivity();
	}

	void decayActivities()
	{
		// ���ú�����С����������ӵĲ���
		varDecayActivity();
		// ���ú�����С�Ӿ������ӵĲ���
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

	void reduceDB()//ɾ������ѧϰ�Ӿ�
	{
		int i, j;
		// �����Ӿ��Ե���ֵ
		double lim = cla_inc / learnts.size();

		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** arr = learnts.tolist();

		// ��������п�������
		QuickSort(arr, 0, learnts.size() - 1);

		int learnts_size = learnts.size();

		// ���ѧϰ�����Ӿ��б�
		learnts.clear();

		// ���������ǰ�벿��
		for (i = j = 0; i < learnts_size / 2; i++)
		{
			// ����Ӿ�û�б�������������
			if (!(*arr[i]).locked(*this))
			{
			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learnts.push_back(arr[i]);
			}
		}

		// ��������ĺ�벿��
		for (; i < learnts_size; i++)
		{
			// ����Ӿ�û�б�������������С����ֵ��������
			if (!(*arr[i]).locked(*this) && (*arr[i]).activity < lim)
			{
			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learnts.push_back(arr[i]);
			}
		}

		// ɾ���Ӿ�ָ������
		delete(arr);
	}

	bool simplifyDB()//���򣬳��Զ�ÿһ���Ӿ���л��������������������ɾ��
	{
		// ������ڳ�ͻ���򲻽��л���
		if (propagate() != nullptr)//�ɳ�ͻ������
		{
			return 0;
		}

		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** cs = learnts.tolist();

		// ���ѧϰ�����Ӿ��б�
		learnts.clear();

		int j = 0;
		// ��ÿһ��ѧϰ�����Ӿ���л���
		for (int i = 0; i < learnts.size(); i++)
		{
			// ����Ӿ���Ա�����������
			if ((*cs[i]).simplify(*this))
			{

			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learnts.push_back(cs[i]);
			}
		}

		// ɾ���Ӿ�ָ������
		delete(cs);

		j = 0;
		// ��ÿһ��Լ���Ӿ���л���
		for (int i = 0; i < constrs_size; i++)
		{
			// ����Ӿ���Ա�����������
			if ((*constrs[i]).simplify(*this))
			{

			}
			else
			{
				// ���򣬽����Ӿ���ӻ�Լ���Ӿ��б�
				constrs[j++] = constrs[i];
			}
		}
		// ����Լ���Ӿ������
		constrs_size = j;

		return 1;
	}

	int solve()
	{
		// ��ʼ�������������������ֺ��Ӿ��˥�����
		SearchParams params(0.95, 0.999);

		// ��ʼ����ͻ�������������������ָ����������
		double nof_conflicts = 100;

		// ��ʼ��ѧϰ������һ��ʼ����ΪԼ���Ӿ�������1/3
		double nof_learnts = constrs_size / 3;

		// ��ʼ�����״̬Ϊδ�趨��Unset��
		int status = Unset;

		// ��ȡ��ǰ�ľ��߲㼶��������Ϊ���㼶
		root_level = decisionLevel();

		// ����ڶ���ʹ��ڳ�ͻ����ʾ�޽�
		if (propagate())
		{
			cancelUntil(root_level);
			return False;
		}

		// �����ݿ���л���
		simplifyDB();

		// ������״̬��δȷ���������ѭ����������������
		while (status == Unset)
		{
			// �����������У���������״̬
			status = search((int)nof_conflicts, (int)nof_learnts, params);

			// ÿ��������Ϻ󣬶��������ͻ������ѧϰ��������ָ����һ�ε����������ٶȽ������⣡������
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}

		// �ָ�������֮ǰ��״̬��ȡ�������ڸ��㼶֮�ϵľ���
		cancelUntil(root_level);

		// ������յ����״̬���棨True�����򷵻��棻���򣬷��ؼ٣�False��
		return status == True;
	}

	int read(char filename[])
	{
		// ���ļ�
		FILE* f;
		char s[200];
		int k = 0, j = 0;
		if ((f = fopen(filename, "rb")) == NULL) // ����ļ���ʧ�ܣ��򷵻ش���
		{
			printf("�޷���!\n");
			return -1;
		}

		// �����ļ��ķ� 'p' ��
		do
		{
			fgets(s, 200, f);
		} while (s[0] != 'p');

		// �� 'p' ����ȡ���ֺ��Ӿ������
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

		// ��ʼ�����ݽṹ
		activity = (double*)malloc((literalNum * 2 + 2) * sizeof(double));
		assigns = (char*)malloc((literalNum + 1) * sizeof(char)); // ����
		reason = (Clause**)malloc((literalNum + 1) * sizeof(Clause*)); // ��ͻԭ��
		level = (int*)malloc((literalNum + 1) * (sizeof(int)));
		watches = (mQueue<Clause*>*)malloc((2 * literalNum + 2) * sizeof(mQueue<Clause*>)); // �۲����
		model = (int*)malloc((literalNum + 1) * sizeof(int));
		constrs = (Clause**)malloc(clauseNum * sizeof(Clause*));

		// ��ʼ���۲����
		for (int i = 0; i < 2 * literalNum + 2; i++)
			watches[i] = mQueue<Clause*>();

		// ��ʼ����Ծ������͸������ָ��
		for (int i = 0; i < literalNum * 2 + 2; i++)
			activity[i] = 0;
		for (int i = 0; i < literalNum + 1; i++)
		{
			assigns[i] = 0;
			reason[i] = nullptr;
			level[i] = -1;
			model[i] = 0;
		}

		// ��ȡ����ʼ���Ӿ��Ծ��
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
			constrs[i] = V; // �Ӿ����
			constrs_size++; // �Ӿ���Ŀ��һ
		}

		// �����Ӿ䣬������ǰ�������ֵ�ֵ����۲����
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

		// �ر��ļ�
		fclose(f);

		// ������������
		return literalNum;
	}

	void write(long long timer, int literalnum, char filename[])//д���ļ�
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
