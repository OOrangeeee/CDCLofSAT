/*���༭��*/
/*������ 2023.9.8 0:00*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structure.h"
//��ؼ��Ľ���࣬��װ�˻�����������Ҫ�ĺ����ͳ�Ա����
class Solver
{

	friend Clause;

	friend lit;

private:

	Clause** clauses;//��ʼ�Ӿ��б�
	int ClauseNum{ 0 };//��ʼ�Ӿ���Ŀ
	mQueue<Clause*> learns;//�ɳ�ͻ��������������ѧϰ���Ӿ��б�
	double clauseIncrease{ 1 };//�Ӿ��Ծ������
	double clauseDecrease{ -1 };//�Ӿ��Ծ��˥����
	int VARNUM{ -1 };//��������
	int clauseNum{ -1 };//�Ӿ�����
	double* Act;//ͨ������ʽ����������������Ծ��
	double varIncrease{ 1 };//������Ծ�ȵ���������
	double varDecrease{ 1 };//������Ծ�ȵ�˥������
	mQueue<Clause*>* getWatch;//�����������ݽṹ���Ӿ��������У�ÿ���Ӿ��ǰ����ֵ�ķ�����Ϊָ��
	mQueue<lit> queueForChuanBo;//��������һ�������Ƶı�Ԫ
	char* fuHao;//Ŀǰ���ڱ�Ԫ�ĸ�ֵ
	mVector<lit> timeInList;//��
	mVector<int> timeInLiseF; //���в�ͬ���߼���ķָ���������Ҳ����ÿ�����ߵ��ڼ����ĸ�λ��
	Clause** reasonForFalse;//�ƶϳ�ÿ����Ԫ��ֵ��ԭ���Ӿ䣬Ҳ���Ǽ��еı�Ԫ��ֵԭ��
	int* level;//���ڵľ��߲���
	int rootLevel{ 0 };
	int* result;//������
	int resultSize{ 0 };




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//�����ú���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




public:
	/// <summary>
	/// ͨ����ε���findans�������ҵ���
	/// </summary>
	/// <returns>�����Ƿ��н�</returns>
	int GetAns()
	{
		SearchParams params(0.95, 0.999);// ��ʼ�������������������ֺ��Ӿ��˥�����
		double nof_conflicts{ 100 };// ��ͻ�Ӿ���������
		double nof_learnts = { ClauseNum / 3.0 };//  ѧϰ�Ӿ���������
		int status{ Unset };
		rootLevel = getDlevel();
		if (Influence())// ����ڶ���ʹ��ڳ�ͻ����ʾ�޽�
		{
			FlashBackALLD(rootLevel);
			return False;
		}
		MakeDataBaseEasy();//���Ӿ����л���
		while (status == Unset)
		{
			status = FindAns((int)nof_conflicts, (int)nof_learnts, params);
			// ÿ��������Ϻ󣬶��������ͻ�Ӿ��������޺�ѧϰ�Ӿ���������
			nof_learnts *= 1.1;
			nof_conflicts *= 1.5;
		}
		// �ָ�������֮ǰ��״̬��ȡ�������ڸ��㼶֮�ϵľ���
		FlashBackALLD(rootLevel);
		return status == True;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//��������
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




private:
	/// <summary>
	/// ����Ҫ�������������������Ӿ��в��ϴ������������ߣ��жϳ�ͻ��ѧϰ�Ӿ䣬ֱ��ȷ���޽�����н�
	/// </summary>
	/// <param name="nof_conflicts">��ͻ����</param>
	/// <param name="nof_learnt">ѧϰ�Ӿ�����</param>
	/// <param name="params">��������</param>
	/// <returns>�����Ƿ��ҵ����</returns>
	int FindAns(int nof_conflicts, int nof_learnt, SearchParams params)
	{
		// ���û�Ծ��˥������
		varDecrease = 1 / params.varDecrease;
		clauseDecrease = 1 / params.clause_decay;
		int conflictC{ 0 };
		for (int i{ 0 }; i < VARNUM + 1; i++)
			result[i] = 0;
		resultSize = 0;
		while (1)
		{
			Clause* confl = Influence();//�Ӿ䴫������
			// ������ڳ�ͻ�Ӿ�
			if (confl)
			{
				conflictC++;
				// �����ͻ�����ڸ����߲�Σ�����False
				if (getDlevel() == rootLevel)
					return False;
				mVector<lit> learnt_clause;
				int backtr_level;
				//�õ�ѧϰ�Ӿ��е�����
				GetReason(confl, learnt_clause, backtr_level);
				// ���ݵ����ľ��߲��
				FlashBackALLD(max(backtr_level, rootLevel));
				// �������ּ�¼ѧϰ�Ӿ�
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
				// ���ѧϰ�Ӿ�������������ƣ����ݻ�Ծ��ɾ������ѧϰ�Ӿ�
				if ((int)learns.size() - (int)getFuHao() >= nof_learnt && nof_learnt >= 0)
					CleanDataBase();
				// ������б������Ѹ�ֵ���ҵ�һ����
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
					// �����ͻ�����������ƣ������¿�ʼ
					if (conflictC >= nof_conflicts && nof_conflicts >= 0)
					{
						FlashBackALLD(rootLevel);
						return Unset;
					}
					// Ѱ��һ��δ��ֵ�Ļ�Ծ����
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
						// ����Ҳ�����Ҫ��ֵ�����֣����ҵ���һ����
						for (int i{ 1 }; i <= getVAR(); i++)
						{
							result[i] = ((fuHao[i] == True));
							resultSize++;
						}
						FlashBackALLD(rootLevel);
						return 1;
					}
					// ���ҵ�����Ҫ��ֵ�����ֽ��о���
					MakeD(tmp ^ 1);
				}
			}
		}
	}

	/// <summary>
	/// ����queueForChuanBo�����е�ֵ�Ա�Ԫ���и�ֵ�������ݸñ�Ԫ��ֵ�����Ӿ䣬Ѱ�ҳ�ͻ
	/// </summary>
	/// <returns>���ص�ǰ���߲�</returns>
	Clause* Influence()
	{
		//���е�ȫ������ֵ��Ԫ������ֵ
		while (!queueForChuanBo.empty())
		{
			lit p{ queueForChuanBo.front() };
			queueForChuanBo.pop();
			mQueue<Clause*> tmp;
			while (!getWatch[p.x].empty())//���ݵ�ǰ��Ԫ��ֵȡ�������������Ԫ�ķ����Ԫ��ص��Ӿ侫ѡ������
			{
				Clause* t{ getWatch[p.x].front() };
				tmp.push_back(t);
				getWatch[p.x].pop();
			}
			while (!tmp.empty())
			{
				Clause* c{ tmp.front() };
				tmp.pop();
				if ((*c).GetClauseInfluence(*this, p) == 0)//�����������0����ζ�Ż�һ�����Ӿ��е�Ψһ��Ԫ�ĸ�ֵʧ�ܣ�Ҳ���Ǹ�ֵ��Ψһ����ζ�ŷ�����ͻ
				{
					while (!tmp.empty())//���ٽ���ʣ�µĲ��裬��δ������Ӿ��������ӻ�ȥ
					{
						Clause* cl{ tmp.front() };
						tmp.pop();
						getWatch[p.x].push_back(cl);
					}
					while (!queueForChuanBo.empty())
						queueForChuanBo.pop();
					return c;//���س�ͻ���
				}
			}
		}
		return nullptr;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//��ȡ����
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



public:
	/// <summary>
	/// ��ȡ�ļ�����ʼ���������ݽṹ
	/// </summary>
	/// <param name="filename[]">�ļ���</param>
	/// <returns>������������</returns>
	int Read(char filename[])
	{
		FILE* f{ nullptr };
		char s[200];
		int k{ 0 };
		int j{ 0 };
		if ((f = fopen(filename, "rb")) == NULL)
		{
			cout << "�ļ���ʧ��\n";
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
		// ��ʼ�����ݽṹ
		Act = (double*)malloc((VARNUM * 2 + 2) * sizeof(double));//��Ԫ��Ծ��
		fuHao = (char*)malloc((VARNUM + 1) * sizeof(char));//��ǰ��Ԫ��ֵ���
		reasonForFalse = (Clause**)malloc((VARNUM + 1) * sizeof(Clause*));//��ͻԭ���Ӿ��б�
		level = (int*)malloc((VARNUM + 1) * (sizeof(int)));//ÿ����Ԫ�ľ��߲�
		getWatch = (mQueue<Clause*>*)malloc((2 * VARNUM + 2) * sizeof(mQueue<Clause*>));//�������ݽṹ�еĹ۲���У�ÿ��Ԫ�ض����Ӿ�ָ�����
		result = (int*)malloc((VARNUM + 1) * sizeof(int));//��
		clauses = (Clause**)malloc(clauseNum * sizeof(Clause*));//�Ӿ��б�
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
		for (int i{ 0 }; i < clauseNum; i++)//��ÿ���Ӿ��ǰ����λ����Ϊ���ָ�룬Ҫ��������λ��ֻ���������δ��ֵ���ٵ����ֶ��õ�����ȥ�����ұ�֤��һ��Ϊ�����δ��ֵ����ֻ�е�һ��δ��ֵ��ʱ������ѡ�������Ϊ���Ӿ䣬�鿴�Ƿ��г�ͻ���߸�ֵΪ��
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
				if (MakeD(clauses[i]->lits[0].x) == 0)//����ǵ��Ӿ�ͽ������ߣ���������ʧ�ܾ��޽�
					return -1;
			}
		}
		fclose(f);
		return VARNUM;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//���򲿷ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




private:
	/// <summary>
	/// ���������Ӿ�
	/// </summary>
	/// <returns>�����Ƿ񻯼�ɹ�</returns>
	bool MakeDataBaseEasy()//���򣬳��Զ�ÿһ���Ӿ���л��������������������ɾ��
	{
		// ������ڳ�ͻ���򲻽��л���
		if (Influence() != nullptr)//�ɳ�ͻ������
			return 0;
		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** cs{ learns.tolist() };
		int length{ 0 };
		length = learns.size();
		learns.clear();
		// ��ÿһ��ѧϰ�����Ӿ���л���
		for (int i{ 0 }; i < length; i++)
		{
			if ((*cs[i]).MakeClauseMoreCleaner(*this))//�����Ӿ䣬������ɾȥ���������������ɾȥ�Ӿ䣬Ҳ�������������б�Ĳ���
			{

			}
			else
				learns.push_back(cs[i]);
		}
		delete(cs);
		int j{ 0 };
		// ��ÿһ��Լ���Ӿ���л���
		for (int i = 0; i < ClauseNum; i++)
		{
			if ((*clauses[i]).MakeClauseMoreCleaner(*this))//�����Ӿ䣬������ɾȥ���������������ɾȥ�Ӿ䣬Ҳ�������������б�Ĳ���
			{

			}
			else
				clauses[j++] = clauses[i];
		}
		ClauseNum = j;
		return 1;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//���߲��ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// ��������
	/// </summary>
	bool MakeD(int num)
	{
		lit p;
		p.x = num;
		timeInLiseF.push_back(timeInList.size());//���������ľ��߼��뵽���߼���ָ���������
		return Enqueue(p, nullptr);//�����߼��봫������
	}

	/// <summary>
	/// ��������ָ�ֵΪ�����κμ��뼣�������������Ԫ���Ӿ�Ҳ���뼣
	/// </summary>
	/// <param name="p">ѡ��</param>
	/// <param name="from">ѡ��</param>
	/// <returns>�����Ƿ��ͻ</returns>
	bool Enqueue(lit p, Clause* from)
	{
		if (this->fuHao[p.var()] != Unset)
		{
			int opt{ 0 };
			if (p.sign() == 0)
				opt = False;
			else
				opt = True;
			if (this->fuHao[p.var()] != opt)//��ֵ��Ψһ����ͻ��
				return 0;
			else//��ֵΨһ������ͻ
				return 1;
		}
		else
		{
			int x{ p.var() };
			//��������ֵΪ��
			if (p.sign() == 0)
				fuHao[p.var()] = False;
			else
				fuHao[p.var()] = True;

			// ����'p'�ı����ھ��߲���е�λ��
			level[p.var()] = getDlevel();
			reasonForFalse[p.var()] = from;
			timeInList.push_back(p);
			queueForChuanBo.push_back(p);
			return 1;
		}
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//���ݲ��ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// �������һ����
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
	/// ������ǰ���߲�����о���
	/// </summary>
	void FlashBackAll()
	{
		// �������ǰ���߲�εĴ�С
		int c{ timeInList.size() - timeInLiseF[timeInLiseF.size() - 1] };
		while (c)
		{
			FlashBack();
			c--;
		}
		timeInLiseF.pop_back();
	}

	/// <summary>
	/// �������м�ֱ��Ŀ����
	/// </summary>
	void FlashBackALLD(int level)
	{
		while (getDlevel() > level)
			FlashBackAll();
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//�Ӿ�ѧϰ���ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// ������ͻ����ã������ҵ�ѧϰ�Ӿ��е�����
	/// </summary>
	/// <param name="confl">��ͻ�������Ӿ�</param>
	/// <param name="out_learnt">ѧϰ�Ӿ������ֵ�����</param>
	/// <param name="out_btlevel">���Ļ���Ŀ���</param>
	void GetReason(Clause* confl, mVector<lit>& out_learnt, int& out_btlevel)
	{
		int vars{ getVAR() };
		bool* seen{ nullptr };//��Ǳ�Ԫ�Ƿ񱻴����
		seen = (bool*)malloc(vars * sizeof(bool*));
		for (int i{ 0 }; i < getVAR(); i++)
			seen[i] = false;
		int counter{ 0 };
		lit p;
		p.x = 0;
		mVector<lit> pReason;//��ǰ��Ԫ��ֵ��ԭ���Ӿ�����ǳ�ʼ������ͻ���Ӿ�
		out_learnt.push_back(p);
		out_btlevel = rootLevel;
		do
		{
			pReason.clear();
			(*confl).ReasonForLearn(*this, p, pReason);//��õ�ǰ��Ԫ��ֵ��ԭ���Ӿ�����ǳ�ʼ������ͻ���Ӿ�
			for (int j{ 0 }; j < pReason.size(); j++)
			{
				lit q{ pReason[j] };

				if (!seen[q.var()])
				{
					seen[q.var()] = 1;
					if (level[q.var()] > rootLevel)//���߲㳬�����㣬��Ծ������
						GetVARinAct(q);
					if (level[q.var()] == getDlevel())
						counter++;
					// �������'q'�ı����ľ��߲�δ��ڸ���Σ�������'q'�ķ��ŷ�ת������ӵ�ѧϰ���Ӿ��У����·�����߲�Σ�˵���˾���ȥ�ҵ�һ��Ψһ�̺��и�
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
			} while (!seen[p.var()]);//���ݼ������������뵱ǰ��ͻ�޹صı�Ԫ��ֵ
			counter--;
		} while (counter > 0);
		lit tmp;
		tmp.x = p.no();
		out_learnt[0] = tmp;//���ʣ�µ�һ���ڳ�ͻ�����ľ��߲�ı�Ԫ�������Ψһ�̺��㣬Ҳ���ǵ�һ��Ψһ�̺��и�ķֽ���
	}

	/// <summary>
	/// ����ѧϰ�Ӿ䲢����ѧϰ�б�����ѧϰ�Ӿ�ĵ�һ����Ҳ���ǵ�һ��Ψһ�̺�����봫�����н�����һ�ָ�ֵ
	/// </summary>
	/// <param name="clause">ѧϰ�Ӿ������б�</param>
	void LearnFromFalse(mVector<lit> clause)
	{
		Clause* c = new Clause;
		Clause* learnt{ nullptr };
		int tmp = (*c).GetNewClause(*this, clause, 1, learnt);//����ѧϰ�Ӿ�
		if (learnt == nullptr)
			return;
		Enqueue(clause[0], learnt);//��ӵ�ѧϰ�б�
		if (learnt != NULL)
			learns.push_back(learnt);
	}

	/// <summary>
	/// ���ݻ�Ծ��ɾ������ѧϰ�Ӿ�
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
			if (!(*arr[i]).IsClause(*this))//����Ӿ�ĵ�һ����Ԫ��������Ӿ��Ѿ���ֵ��������������ζ������Ӿ�Ϊ�棬��Ϊ�����������ݽṹ��һ����Ԫһ�㲻����Ϊ��
			{
			}
			else
				learns.push_back(arr[i]);
		}
		for (; i < learnts_size; i++)//�Ӻ�벿��ɾ��
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
	//��Ծ�ȹ����ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// �����ܻ�Ծ��
	/// </summary>
	void DecreaseAct()
	{
		// ���ú�����С����������ӵĲ���
		DecreaseVARinAct();
		// ���ú�����С�Ӿ������ӵĲ���
		DecreaseClasuseinAct();
	}

	/// <summary>
	/// ��߱����Ļ�Ծ�ȣ�����������ֵ
	/// </summary>
	/// <param name="p">����ߵı�Ԫ</param>
	void GetVARinAct(lit p)
	{
		if (varDecrease < 0)
			return;
		// ���ӱ���p�Ļ��
		if ((Act[p.x] += varIncrease) > 1e100)
			// ��������Ļ�Թ��󣬵��ú�����������
			DelVARinAct();
	}

	/// <summary>
	/// �����Ӿ�c�Ļ�Ծ�ȣ�����������ֵ
	/// </summary>
	/// <param name="p">����ߵı�Ԫ</param>
	void IncreaseClasuseinAct(Clause* c)
	{
		// �����Ӿ�c�Ļ��
		c->Act += clauseIncrease;
		// ����Ӿ�Ļ�Թ��󣬵��ú�����������
		if (c->Act > 1e20)
			ChangeLearnAct();
	}

	/// <summary>
	/// �������б�Ԫ�Ļ�Ծ��
	/// </summary>
	void DelVARinAct()
	{
		for (int i{ 0 }; i <= VARNUM * 2 + 1; i++)
			Act[i] *= 1e-100;
		// ��С������ӵĲ���
		varIncrease *= 1e-100;
	}

	/// <summary>
	/// ����ѧϰ�Ӿ��Ծ��
	/// </summary>
	void ChangeLearnAct()
	{
		// ������ѧϰ�����Ӿ�ת��Ϊָ������
		Clause** arr = learns.tolist();
		for (int i{ 0 }; i < learns.size(); i++)
			arr[i]->Act *= 1e-20;
		delete(arr);
		clauseIncrease *= 1e-20;
	}

	/// <summary>
	/// ���ٱ�Ԫ��Ծ����������
	/// </summary>
	void DecreaseVARinAct()
	{
		if (varDecrease >= 0)
			varIncrease *= varDecrease;
	}

	/// <summary>
	/// �����Ӿ��Ծ����������
	/// </summary>
	void DecreaseClasuseinAct()
	{
		// ��С�Ӿ������ӵĲ���
		clauseIncrease *= clauseDecrease;
	}




	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//�������ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




	/// <summary>
	/// ������������
	/// </summary>
	int getVAR()
	{
		return this->VARNUM;
	}

	/// <summary>
	/// ���ؼ��ĳ���
	/// </summary>
	/// <returns>���ؼ��ĳ���</returns>
	int getFuHao()
	{
		return timeInList.size();
	}

	/// <summary>
	/// �����Ӿ�����
	/// </summary>
	/// <returns>�����Ӿ�����</returns>
	int getClasueNum()
	{
		return this->clauseNum;
	}

	/// <summary>
	/// ����ѧϰ�Ӿ�����
	/// </summary>
	/// <returns>����ѧϰ�Ӿ�����</returns>
	int getLearnSize()
	{
		return learns.size();
	}

	/// <summary>
	/// ���ص�ǰ���߲�
	/// </summary>
	/// <returns>���ص�ǰ���߲�</returns>
	int getDlevel()
	{
		return timeInLiseF.size();
	}

	/// <summary>
	/// �����Ӿ��Ծ�Ƚ��п���
	/// </summary>
	/// <param name="a">�Ӿ��б�</param>
	/// <param name="low">��ָ��</param>
	/// <param name="high">��ָ��</param>
	void QuickSort(Clause**& a, int low, int high)
	{
		Clause* pivot = nullptr;
		int i{ low };
		int j{ high };
		if (low < high)
		{
			pivot = a[low]; //����һ��Ԫ����Ϊ����
			//���������е�һ�η���
			while (i < j)
			{
				while (i < j && a[j]->Act >= pivot->Act) //���������ҵ���һ��С���������
					j--;
				if (i < j)
				{
					a[i] = a[j];
					i++;
				}
				while (i < j && a[i]->Act < pivot->Act) //���������ҵ���һ�����ڵ����������
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
	//������ֺ���
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





public:
	/// <summary>
	/// д��𰸵�res�ļ�
	/// </summary>
	/// <param name="timer">ʱ��</param>
	/// <param name="literalnum">�Ӿ���Ŀ</param>
	/// <param name="filename[]">�ļ���</param>
	void Print(long long timer, int literalnum, char filename[])
	{
		int k{ (int)strlen(filename) };
		filename[k - 1] = 's';
		filename[k - 2] = 'e';
		filename[k - 3] = 'r';
		FILE* f{ nullptr };
		if ((f = fopen(filename, "wb")) == NULL)
		{
			cout << "д��ʧ��\n";
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
	/// չʾ��ȡ��cnf�ļ�
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
/// �����µ�ѧϰ�Ӿ�
/// </summary>
/// <param name="S">�����</param>
/// <param name="ps">��������</param>
/// <param name="learnt">�Ƿ���ѧϰ�Ӿ�</param>
/// <param name="out_clause">ѧϰ�Ӿ�ָ��</param>
/// <returns>�����Ƿ�ɹ�</returns>
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
/// �ж��Ӿ��Ƿ�Ϊ�棬����ѧϰ�Ӿ�
/// <param name="S">�����</param>
/// </summary>
/// <returns>�����Ƿ������Ӿ�</returns>
bool Clause::IsClause(Solver S)
{
	return S.reasonForFalse[this->lits[0].var()] == this;
}

/// <summary>
/// �����Ӿ��е����֣�δ��ֵ�������ˣ�����Ӿ�����ľͷ���1������ɾ���Ӿ�
/// </summary>
/// <param name="S">�����</param>
/// <returns>�����Ƿ�Ϊ��</returns>
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

/// <summary>
/// �ƶ��۲�ָ�룬���߿������Ϊ�����۲������������������ݽṹ�ĺ���
/// </summary>
/// <param name="S">�����</param>
/// <param name="p">��ǰ����ֵ�ı�Ԫ</param>
/// <returns>�����Ƿ�ɹ�</returns>
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
		S.getWatch[p.x].push_back(this);
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
			S.getWatch[lits[1].no()].push_back(this);
			return 1;
		}
	}
	//�Ӿ��ڵ�ǰ��ֵ��Ϊ��Ԫ�Ӿ�
	S.getWatch[p.x].push_back(this);
	return S.Enqueue(lits[0], this);
}

/// <summary>
/// �õ���ͻ�����ߵõ���ǰ��Ԫ��ԭ�����
/// </summary>
/// /// <param name="S">�����</param>
/// <param name="p">��ǰ���еı�Ԫ</param>
/// <param name="out_reason">����λ��</param>
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
