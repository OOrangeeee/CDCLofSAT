/*���༭��*/
/*������ 2023.9.7 23:37*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#include<bits/stdc++.h>
using namespace std;
const int True{ 1 };
const int False{ -1 };
const int Unset{ 0 };
//�궨�庯��
#define SafeDelete(_x)   \
    {                    \
        if (_x)          \
        {                \
            delete (_x); \
            (_x) = NULL; \
        }                \
    }
#define SafeDeleteArray(_x) \
    {                       \
        if (_x)             \
        {                   \
            delete[](_x);   \
            (_x) = NULL;    \
        }                   \
    }
#define max(a, b) ((a) > (b) ? (a) : (b))
//������
class lit;
class Solver;
class Clause;

//������
class lit
{
	friend Clause;

	friend Solver;

public:
	int x;
	/// <summary>
	/// ʹ���޲������������ֺ�������ʼ��Ϊ0
	/// </summary>
	lit()
	{
		this->x = 0;//
	}
	/// <summary>
	/// ��ʼ�����֣�������ʼ��Ϊ����������Ϊż������������
	/// </summary>
	/// <param name="x"></param>
	lit(int x)
	{
		this->x = (x > 0) ? (2 * x + 1) : (-2 * x);
	}
	/// <summary>
	/// ͨ��λ�����жϷ���
	/// </summary>
	/// <returns>������1��������0</returns>
	bool sign()
	{
		return (this->x) & 1;
	}
	/// <summary>
	/// �õ���Ԫ�ı�ţ��Ⱦ���ֵ
	/// </summary>
	/// <returns>���ر�Ԫ���</returns>
	int var()
	{
		return (this->x) >> 1;
	}
	/// <summary>
	/// �������ֵ��������ȷ������ֳ�ʼ��ʱ�õ���ֵ
	/// </summary>
	/// <returns>��������2*a+1����������-2*a��aΪ��Ԫ���</returns>
	int index()
	{
		return this->x;
	}
	/// <summary>
	/// ���ص�ǰ���ֵĸ�ֵ������һ����ʽ
	/// </summary>
	/// <returns>���ص�ǰ���ֵ���һ����ʽ</returns>
	int no()
	{
		return this->x ^ 1;
	}
};

//����ʱ�õ�˥���������ʵ�ֺ������з�װ����������������Ĭ��Ϊ1��1��0
struct SearchParams
{
	double varDecrease, clause_decay, rand_feq;
	SearchParams(double v = 1, double c = 1, double r = 0)//��ʼ���ṹ���õĺ���
	{
		varDecrease = v;
		clause_decay = c;
		rand_feq = r;
	}
};

//�Զ���ɱ�����Ա�std::vector
template <class T>//���ͱ��
class mVector
{
	friend lit;
public:
	//Ĭ�ϳ�ʼ����������
	mVector()
	{
		this->length = 0;
		this->maxlength = 10;
		this->data = new T[this->maxlength]();

	};
	mVector(const mVector& v)
	{
		this->length = v.length;
		this->maxlength = v.maxlength;
		this->data = new T[this->maxlength]();
		for (int i = 0; i < this->maxlength; i++)
		{
			this->data[i] = v.data[i];
		}
	};
	~mVector()
	{
		this->length = 0;
		this->maxlength = 0;
		SafeDeleteArray(this->data);
	};
	//����һ��Ԫ�ص����
	void push_back(T element) {
		if (this->length >= this->maxlength)
		{
			int i;
			T* dataTemp = new T[this->maxlength * 2]();
			for (i = 0; i < this->maxlength; i++)
			{
				dataTemp[i] = this->data[i];
			}
			this->maxlength = this->maxlength * 2;
			SafeDeleteArray(this->data);
			this->data = dataTemp;
		}
		this->data[this->length] = element;
		this->length++;
	};
	//���vector
	void clear() {
		SafeDeleteArray(this->data);
		this->length = 0;
		this->maxlength = 10;
		this->data = new T[this->maxlength]();

	};
	void pop_back() {
		this->length--;
	}

	//���vectorԪ�ظ���
	int size() {
		return this->length;
	};
	//����[]������
	T& operator[](int i) {
		return this->data[i];
	};
	//����=������
	void resize(int i) {
		this->length = i;
	}
	mVector& operator=(const mVector& v) {
		this->length = v.length;
		this->maxlength = v.maxlength;
		SafeDeleteArray(this->data);
		this->data = new T[this->maxlength]();
		for (int i = 0; i < this->maxlength; i++)
		{
			this->data[i] = v.data[i];
		}
		return *this;
	};
private:
	T* data;    //�洢���ݵ�����
	int length;   //����Ԫ��
	int maxlength;
};


//�Զ���߼����жԱ�std::Queue
template <class T>//����
class mQueue
{
public:
	//��ʼ����������
	mQueue() : Front(NULL), rear(NULL), count(0)
	{
	}

	~mQueue()
	{
		//clear();
	}

	/*void push(const T& node)
	{
		if (Front == NULL)
			Front = rear = new QueueNode(node);
		else
		{
			QueueNode* newqueuenode = new QueueNode(node);
			rear->next = newqueuenode;
			rear = newqueuenode;
		}
		count++;
	}*/

	void push_back(const T& node)
	{
		if (Front == NULL)
			Front = rear = new QueueNode(node);
		else
		{
			QueueNode* newqueuenode = new QueueNode(node);
			rear->next = newqueuenode;
			rear = newqueuenode;
		}
		count++;
	}
	bool empty() const
	{
		return Front == NULL;
	}

	int size() const
	{
		return count;
	}

	void clear()
	{
		while (Front)
		{
			QueueNode* FrontofQueue = Front;
			Front = Front->next;
			delete FrontofQueue;
		}
		rear = NULL;
		count = 0;
	}

	void pop()
	{
		QueueNode* FrontofQueue = Front;
		Front = Front->next;
		delete FrontofQueue;
		count--;
	}

	void pop_back()
	{
		if (count == 1) {
			delete Front;
			count--;
		}
		else {
			QueueNode* t = Front;
			while (t->next != rear) {
				t = t->next;
			}
			QueueNode* rearofQueue = rear;
			t->next = NULL;
			rear = t;
			delete rearofQueue;
			count--;
		}
	}

	T& front()
	{

		return Front->data;
	}

	T front() const
	{

		return Front->data;
	}

	T Rear() const
	{
		return rear->data;
	}

	T* tolist()
	{
		T* array = new T[count];
		QueueNode* p = Front;
		for (int i = 0; i < count; i++) {
			array[i] = p->data;
			p = p->next;
		}
		return array;
	}

	//private: //Ҳ����ֱ����������listֱ�ӹ���
	struct QueueNode
	{
		T data;
		QueueNode* next;
		QueueNode(const T& Newdata, QueueNode* nextnode = NULL) : data(Newdata), next(nextnode)
		{
		}
		// QueueNode() = default;
	};
	QueueNode* Front; //��ͷָ��
	QueueNode* rear;  // ��βָ��
	int count;
};

//�Ӿ�
class Clause
{

	friend lit;

	friend Solver;

protected:
	bool learnt{ 0 };//�Ƿ�Ϊѧϰ���
	double Act{ 0 };//��Ծ��
	mVector<lit> lits;//��̬���������������б�
	/// <summary>
	/// �жϵ�ǰ���Ͳ������Ļ�Ծ�ȴ�С
	/// </summary>
	/// <param name="x">���Ƚ����</param>
	/// <returns>�����ǰС��true������false</returns>
	bool operator<(const Clause x)
	{
		return Act < x.Act;
	}
	void ReasonForLearn(Solver& S, lit p, mVector<lit>& out_reason);//��¼��ͻԭ��
	bool GetClauseInfluence(Solver& S, lit p);//�������жϲ���ֵ��һ����Ԫ
	bool IsClause(Solver S);//�жϸ��Ӿ��ܲ��ܱ����򣬵��Ӿ䲻����
	bool GetNewClause(Solver& S, mVector<lit> ps, bool learnt, Clause*& out_clause);//�����µ�ѧϰ�Ӿ�
	bool MakeClauseMoreCleaner(Solver& S);//�����Ӿ䣬ȥ���Ѹ�ֵ��Ԫ�����޵�һ��

};
