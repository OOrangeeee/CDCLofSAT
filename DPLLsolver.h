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
	Clause** clauses;//�Ӿ��б�
	int ClauseNum{ 0 };
	mQueue<Clause*> learns;  //�ɳ�ͻ��������������ѧϰ���Ӿ��б�
	double clauseIncrease{ 1 };       //�Ӿ��Ծ������
	double clauseDecrease{ -1 };     //�Ӿ��Ծ��˥����
	int VARNUM{ -1 };           //��������
	int clauseNum{ -1 };            //�Ӿ�����
	double* Act;     //ͨ������ʽ����������������Ծ��
	double varIncrease{ 1 };   //������Ծ�ȵ���������
	double varDecrease{ 1 }; //������Ծ�ȵ�˥������
	mQueue<Clause*>* GetCheck; //ÿ�����ֱ��۲��Լ���б�
	mQueue<lit> queueForChuanBo;         //��������
	char* fuHao;          //�����������ĵ�ǰ����
	mVector<lit> timeInList;     //��ʱ��˳�����еķ����б�
	mVector<int> timeInLiseF; //�ڡ�׷�ټ�¼�����������ֲ�ͬ���߲㼶�ķָ�������
	Clause** reasonForFalse;       //��ͻԭ��
	int* level;            //ÿ����������ֵ�ľ��߲㼶
	int rootLevel{ 0 };
	int* result;//������
	int resultSize{ 0 };

	//��������
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
	//����������
	Clause* Influence()
	{
		// �����в�Ϊ��ʱ��ѭ������
		while (!queueForChuanBo.empty())
		{
			// �Ӷ��е�ǰ�˻�ȡԪ�أ���ֵ��'p'
			lit p = queueForChuanBo.front();
			queueForChuanBo.pop();

			// ������ʱ����'tmp'
			mQueue<Clause*> tmp;

			// ���Ӷ��в�Ϊ��ʱ��ѭ������
			while (!GetCheck[p.x].empty())//ȡ�����й۲�����е�ֵ
			{
				// �ӵ��Ӷ���ǰ�˻�ȡԪ�أ���ֵ��'t'
				Clause* t = GetCheck[p.x].front();
				tmp.push_back(t);
				GetCheck[p.x].pop();
			}

			// ���µĶ���'tmp'���д���
			while (!tmp.empty())
			{
				// ��'tmp'����ǰ�˻�ȡԪ�أ���ֵ��'c'
				Clause* c = tmp.front();
				tmp.pop();

				if ((*c).GetClauseInfluence(*this, p) == 0)
				{
					// ��'tmp'���в�Ϊ��ʱ��ѭ������
					while (!tmp.empty())
					{
						// ��'tmp'����ǰ�˻�ȡԪ�أ���ֵ��'cl'
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
		// ���'p'�ı����Ѿ�����ֵ
		if (this->fuHao[p.var()] != Unset)
		{
			int opt;
			// ���'p'�ķ���Ϊ0������'opt'ΪFalse
			if (p.sign() == 0)
				opt = False;
			else
				// ��������'opt'ΪTrue
				opt = True;
			// ����µĸ�ֵ'opt'��ԭ�еĸ�ֵ��ͻ������0
			if (this->fuHao[p.var()] != opt)
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
				fuHao[p.var()] = False;
			}
			else
			{
				fuHao[p.var()] = True;
			}

			// ����'p'�ı����ھ��߲���е�λ��
			level[p.var()] = getDlevel();
			reasonForFalse[p.var()] = from;
			timeInList.push_back(p);
			queueForChuanBo.push_back(p);
			return 1;
		}
	}

	void GetReason(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		// ��ȡ����������
		int vars = getVAR();
		bool* seen;
		// �����ڴ�ռ�������Ƿ񱻿����ı������
		seen = (bool*)malloc(vars * sizeof(bool*));
		// ��ʼ�����еı��Ϊfalse
		for (int i = 0; i < getVAR(); i++)
			seen[i] = false;
		// ��ʼ��������������'p'
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
				// �������'q'�ı�����û������
				if (!seen[q.var()])
				{
					// �������'q'�ı���
					seen[q.var()] = 1;
					// �������'q'�ı����ľ��߲�δ��ڸ���Σ�������'q'�ķ��ŷ�ת�������ӱ����Ļ�Ծ��
					if (level[q.var()] > rootLevel)
					{
						lit tmp;
						tmp.x = (q.x) ^ 1;
						GetVARinAct(q);
					}
					// �������'q'�ı����ľ��߲�ε��ڵ�ǰ���߲�Σ����Ӽ�����
					if (level[q.var()] == getDlevel())
						counter++;
					// �����������'q'�ı����ľ��߲�δ��ڸ���Σ�������'q'�ķ��ŷ�ת������ӵ�ѧϰ���Ӿ��У����·�����߲��
					else if (level[q.var()] > rootLevel)
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
				p = timeInList[timeInList.size() - 1];
				confl = reasonForFalse[p.var()];
				FlashBack();
			} while (!seen[p.var()]);
			counter--;
		} while (counter > 0);
		// ����һ��ѧϰ����������Ϊ����'p'�ķ�
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;
	}

	void LearnFromFalse(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt = nullptr;
		int tmp = (*c).GetNewClause(*this, clause, 1, learnt);//����ѧϰ�Ӿ�
		if (learnt == nullptr)//û�п���ѧϰ����
			return;
		Enqueue(clause[0], learnt);//��ӵ�ѧϰ�б�
		if (learnt != NULL)//
			learns.push_back(learnt);
	}

	void FlashBack()
	{
		lit p = timeInList[timeInList.size() - 1]; 
		// ��ȡ'p'�ı������
		int x = p.var(); 
		// ������'x'�ĸ�ֵ״̬��Ϊδ����
		fuHao[x] = Unset;
		// ������'x'��ԭ����Ϊnullptr
		reasonForFalse[x] = nullptr;
		// ������'x'�ľ��߲����Ϊ-1
		level[x] = -1;
		// ��������'p'�Ļ�Ծ��
		Act[p.x] += varIncrease;
		timeInList.pop_back();
	}

	//��������
	bool MakeD(int num)
	{
		// ����һ���µ�����'p'������������Ϊ���������
		lit p; 
		p.x = num; 
		timeInLiseF.push_back(timeInList.size()); 
		return Enqueue(p, nullptr); 
	}

	void FlashBackAll()
	{
		// �������ǰ���߲�εĴ�С
		int c = timeInList.size() - timeInLiseF[timeInLiseF.size() - 1];
		// ʹ��ѭ����������������߲�ε����о���
		while (c)
		{
			FlashBack();
			c--;
		}
		timeInLiseF.pop_back();
	}

	void FlashBackALLD(int level)
	{
		// ʹ��ѭ���������ߣ�ֱ����ǰ���߲�β�����Ŀ����߲��
		while (getDlevel() > level)
			FlashBackAll();
	}

	int FindAns(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// ���û�Ծ��˥������
		varDecrease = 1 / params.varDecrease;
		clauseDecrease = 1 / params.clause_decay;
		// ��ʼ����ͻ������
		int conflictC = 0;
		// ��ʼ�����������
		for (int i = 0; i < VARNUM + 1; i++)
			result[i] = 0;
		resultSize = 0;
		// ��ѭ����ʼ
		while (1)
		{
			Clause* confl = Influence();
			// ������ڳ�ͻ�Ӿ�
			if (confl)
			{
				// ��ͻ����������
				conflictC++;
				// �����ͻ�����ڸ����߲�Σ�����False
				if (getDlevel() == rootLevel)
					return False;
				// ��ʼ��ѧϰ�Ӿ�ͻ��ݲ��
				mVector<lit> learnt_clause;
				int backtr_level;
				GetReason(confl, learnt_clause, backtr_level);
				// ���ݵ����ľ��߲��
				FlashBackALLD(max(backtr_level, rootLevel));
				// ��¼ѧϰ�Ӿ�
				LearnFromFalse(learnt_clause);
				// ���ѧϰ�Ӿ�ֻ��һ�����֣���������߲��Ϊ0
				if (learnt_clause.size() == 1)
					level[learnt_clause[0].var()] = 0;
				// �Ӿ�����ֻ��˥��
				DecreaseClasuseinAct();
				DecreaseVARinAct();
			}
			else
			{
				// �����ǰ���߲��Ϊ����Σ���������
				if (getDlevel() == rootLevel)
					MakeDataBaseEasy();
				// ���ѧϰ�Ӿ�������������ƣ��������ݿ�ѹ��
				if ((int)learns.size() - (int)getFuHao() >= nof_learnt && nof_learnt >= 0)
					CleanDataBase();
				// ������б������Ѹ�ֵ���ҵ�һ����
				if (getFuHao() == getVAR())
				{
					// ��װ��
					for (int i = 1; i <= getVAR(); i++)
					{
						result[i] = ((fuHao[i] == True));
						resultSize++;
					}
					// ���ݵ������߲��
					FlashBackALLD(rootLevel);
					return 1;
				}
				else
				{
					// �����ͻ�����������ƣ����ݵ������߲�β�����Unset
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						FlashBackALLD(rootLevel);
						return Unset;
					}
					// Ѱ��һ��δ��ֵ�Ļ�Ծ����
					int tmp = -1;
					double m = -1;
					lit p;
					for (int i = 1; i <= getVAR(); i++)
					{
						// ������
						p = lit(i);
						if (m < Act[p.index()] && fuHao[p.var()] == Unset)
						{
							tmp = p.index();
							m = Act[p.index()];
						}
						// ������
						p = lit(-i);
						if (m < Act[p.index()] && fuHao[p.var()] == Unset)
						{
							tmp = p.index();
							m = Act[p.index()];
						}
					}
					if (tmp < 0)
					{
						// ����Ҳ�����Ҫ��ֵ�����֣����ҵ���һ����
						for (int i = 1; i <= getVAR(); i++)
						{
							result[i] = ((fuHao[i] == True));
							resultSize++;
						}
						// ���ݵ�����
						FlashBackALLD(rootLevel);
						return 1;
					}
					// ���ҵ�����Ҫ��ֵ�����ֽ��о���
					MakeD(tmp ^ 1);
				}
			}
		}
	}

	void GetVARinAct(lit p)
	{
		if (varDecrease < 0)
			return;
		// ���ӱ���p�Ļ��
		if ((Act[p.x] += varIncrease) > 1e100)
			// ��������Ļ�Թ��󣬵��ú�����������
			DelVARinAct();
	}

	void DelVARinAct()
	{
		// �������е����֣���С���ǵĻ��
		for (int i = 0; i <= VARNUM * 2 + 1; i++)
		{
			Act[i] *= 1e-100;
		}
		// ��С������ӵĲ���
		varIncrease *= 1e-100;
	}

	void DecreaseVARinAct()
	{
		if (varDecrease >= 0)
			varIncrease *= varDecrease;
	}

	void DecreaseClasuseinAct()
	{
		// ��С�Ӿ������ӵĲ���
		clauseIncrease *= clauseDecrease;
	}

	void ChangeLearnAct()
	{
		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** arr = learns.tolist();

		// ��������ѧϰ�����Ӿ�
		for (int i = 0; i < learns.size(); i++)
		{
			// ��С�Ӿ�Ļ��
			arr[i]->Act *= 1e-20;
			//learnts[i]->activity *= 1e-20;
			//p=p->next;
		}
		// ɾ���Ӿ�ָ������
		delete(arr);
		// ��С�Ӿ������ӵĲ���
		clauseIncrease *= 1e-20;
	}

	void IncreaseClasuseinAct(Clause* c)
	{
		// �����Ӿ�c�Ļ��
		c->Act += clauseIncrease;
		// ����Ӿ�Ļ�Թ��󣬵��ú�����������
		if (c->Act > 1e20)
			ChangeLearnAct();
	}

	void DecreaseAct()
	{
		// ���ú�����С����������ӵĲ���
		DecreaseVARinAct();
		// ���ú�����С�Ӿ������ӵĲ���
		DecreaseClasuseinAct();
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
				while (i < j && a[j]->Act >= pivot->Act) //���������ҵ���һ��С���������
					j--;
				if (i < j)
				{ //�������ƶ�����ָ���λ��
					//a.set(i, a[j]);
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->Act < pivot->Act) //���������ҵ���һ�����ڵ����������
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

	void CleanDataBase()//ɾ������ѧϰ�Ӿ�
	{
		int i, j;
		// �����Ӿ��Ե���ֵ
		double lim = clauseIncrease / learns.size();

		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** arr = learns.tolist();

		// ��������п�������
		QuickSort(arr, 0, learns.size() - 1);

		int learnts_size = learns.size();

		// ���ѧϰ�����Ӿ��б�
		learns.clear();

		// ���������ǰ�벿��
		for (i = j = 0; i < learnts_size / 2; i++)
		{
			// ����Ӿ�û�б�������������
			if (!(*arr[i]).IsClause(*this))
			{
			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learns.push_back(arr[i]);
			}
		}

		// ��������ĺ�벿��
		for (; i < learnts_size; i++)
		{
			// ����Ӿ�û�б�������������С����ֵ��������
			if (!(*arr[i]).IsClause(*this) && (*arr[i]).Act < lim)
			{
			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learns.push_back(arr[i]);
			}
		}

		// ɾ���Ӿ�ָ������
		delete(arr);
	}

	bool MakeDataBaseEasy()//���򣬳��Զ�ÿһ���Ӿ���л��������������������ɾ��
	{
		// ������ڳ�ͻ���򲻽��л���
		if (Influence() != nullptr)//�ɳ�ͻ������
		{
			return 0;
		}

		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** cs = learns.tolist();

		// ���ѧϰ�����Ӿ��б�
		learns.clear();

		int j = 0;
		// ��ÿһ��ѧϰ�����Ӿ���л���
		for (int i = 0; i < learns.size(); i++)
		{
			// ����Ӿ���Ա�����������
			if ((*cs[i]).GetCleanInClause(*this))
			{

			}
			else
			{
				// ���򣬽����Ӿ���ӻ�ѧϰ�����Ӿ��б�
				learns.push_back(cs[i]);
			}
		}

		// ɾ���Ӿ�ָ������
		delete(cs);

		j = 0;
		// ��ÿһ��Լ���Ӿ���л���
		for (int i = 0; i < ClauseNum; i++)
		{
			// ����Ӿ���Ա�����������
			if ((*clauses[i]).GetCleanInClause(*this))
			{

			}
			else
			{
				// ���򣬽����Ӿ���ӻ�Լ���Ӿ��б�
				clauses[j++] = clauses[i];
			}
		}
		// ����Լ���Ӿ������
		ClauseNum = j;

		return 1;
	}

	int GetAns()
	{
		// ��ʼ�������������������ֺ��Ӿ��˥�����
		SearchParams params(0.95, 0.999);

		// ��ʼ����ͻ�������������������ָ����������
		double nof_conflicts = 100;

		// ��ʼ��ѧϰ������һ��ʼ����ΪԼ���Ӿ�������1/3
		double nof_learnts = ClauseNum / 3;

		// ��ʼ�����״̬Ϊδ�趨��Unset��
		int status = Unset;

		// ��ȡ��ǰ�ľ��߲㼶��������Ϊ���㼶
		rootLevel = getDlevel();

		// ����ڶ���ʹ��ڳ�ͻ����ʾ�޽�
		if (Influence())
		{
			FlashBackALLD(rootLevel);
			return False;
		}

		// �����ݿ���л���
		MakeDataBaseEasy();

		// ������״̬��δȷ���������ѭ����������������
		while (status == Unset)
		{
			// �����������У���������״̬
			status = FindAns((int)nof_conflicts, (int)nof_learnts, params);

			// ÿ��������Ϻ󣬶��������ͻ������ѧϰ��������ָ����һ�ε����������ٶȽ������⣡������
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}

		// �ָ�������֮ǰ��״̬��ȡ�������ڸ��㼶֮�ϵľ���
		FlashBackALLD(rootLevel);

		// ������յ����״̬���棨True�����򷵻��棻���򣬷��ؼ٣�False��
		return status == True;
	}

	int Read(char filename[])
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
				VARNUM = k;
				k = 0;
			}
			j++;
		}
		clauseNum = k;

		// ��ʼ�����ݽṹ
		Act = (double*)malloc((VARNUM * 2 + 2) * sizeof(double));
		fuHao = (char*)malloc((VARNUM + 1) * sizeof(char)); // ����
		reasonForFalse = (Clause**)malloc((VARNUM + 1) * sizeof(Clause*)); // ��ͻԭ��
		level = (int*)malloc((VARNUM + 1) * (sizeof(int)));
		GetCheck = (mQueue<Clause*>*)malloc((2 * VARNUM + 2) * sizeof(mQueue<Clause*>)); // �۲����
		result = (int*)malloc((VARNUM + 1) * sizeof(int));
		clauses = (Clause**)malloc(clauseNum * sizeof(Clause*));

		// ��ʼ���۲����
		for (int i = 0; i < 2 * VARNUM + 2; i++)
			GetCheck[i] = mQueue<Clause*>();

		// ��ʼ����Ծ������͸������ָ��
		for (int i = 0; i < VARNUM * 2 + 2; i++)
			Act[i] = 0;
		for (int i = 0; i < VARNUM + 1; i++)
		{
			fuHao[i] = 0;
			reasonForFalse[i] = nullptr;
			level[i] = -1;
			result[i] = 0;
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
				Act[lit(tmp).no()] += varIncrease;
			}
			clauses[i] = V; // �Ӿ����
			ClauseNum++; // �Ӿ���Ŀ��һ
		}

		// �����Ӿ䣬������ǰ�������ֵ�ֵ����۲����
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

		// �ر��ļ�
		fclose(f);

		// ������������
		return VARNUM;
	}

	void Print(long long timer, int literalnum, char filename[])//д���ļ�
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


//�������Ӿ䲢������ӵ��۲�����б�
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
			//ѡ��ڶ�����Ҫ�۲������
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

//�ж�����ǲ��ǵ�λ��䣬����ѧϰ���
bool Clause::IsClause(Solver S)
{
	return S.reasonForFalse[this->lits[0].var()] == this;
}
//ɾ������δ����ֵ�����
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
		if (S.fuHao[lits[i].var()] == opt)//�Ӿ�����
			return 1;
		if (S.fuHao[lits[i].var()] == Unset)//δ����ֵ����
		{
			lits[j++] = lits[i];
		}
	}
	lits.resize(j);
	return 0;
}

bool Clause::GetClauseInfluence(Solver& S, lit p)
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
	if (S.fuHao[lits[0].var()] == opt)
	{
		S.GetCheck[p.x].push_back(this);
		return true;
	}
	//Ѱ���µ��������۲�
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
	//�Ӿ��ڵ�ǰ��ֵ��Ϊ��Ԫ�Ӿ�
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
