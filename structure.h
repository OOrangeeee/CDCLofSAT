/*最后编辑：*/
/*晋晨曦 2023.9.7 23:37*/
/*qq：2950171570*/
/*email：Jin0714@outlook.com  回复随缘*/



#include<bits/stdc++.h>
using namespace std;
const int True{ 1 };
const int False{ -1 };
const int Unset{ 0 };
//宏定义函数
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
//类声明
class lit;
class Solver;
class Clause;

//文字类
class lit
{
	friend Clause;

	friend Solver;

public:
	int x;
	/// <summary>
	/// 使用无参数的生成文字函数，初始化为0
	/// </summary>
	lit()
	{
		this->x = 0;//
	}
	/// <summary>
	/// 初始化文字，正数初始化为奇数，负数为偶数，用作索引
	/// </summary>
	/// <param name="x"></param>
	lit(int x)
	{
		this->x = (x > 0) ? (2 * x + 1) : (-2 * x);
	}
	/// <summary>
	/// 通过位运算判断符号
	/// </summary>
	/// <returns>正返回1，负返回0</returns>
	bool sign()
	{
		return (this->x) & 1;
	}
	/// <summary>
	/// 得到变元的编号，既绝对值
	/// </summary>
	/// <returns>返回变元编号</returns>
	int var()
	{
		return (this->x) >> 1;
	}
	/// <summary>
	/// 返回文字的索引，既返回文字初始化时得到的值
	/// </summary>
	/// <returns>正数返回2*a+1，负数返回-2*a，a为变元编号</returns>
	int index()
	{
		return this->x;
	}
	/// <summary>
	/// 返回当前文字的负值，既另一个形式
	/// </summary>
	/// <returns>返回当前文字的另一个形式</returns>
	int no()
	{
		return this->x ^ 1;
	}
};

//搜索时用的衰变参数。对实现函数进行封装，如果不传入参数则默认为1，1，0
struct SearchParams
{
	double varDecrease, clause_decay, rand_feq;
	SearchParams(double v = 1, double c = 1, double r = 0)//初始化结构体用的函数
	{
		varDecrease = v;
		clause_decay = c;
		rand_feq = r;
	}
};

//自定义可变数组对标std::vector
template <class T>//泛型编程
class mVector
{
	friend lit;
public:
	//默认初始化创建函数
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
	//插入一个元素到最后
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
	//清空vector
	void clear() {
		SafeDeleteArray(this->data);
		this->length = 0;
		this->maxlength = 10;
		this->data = new T[this->maxlength]();

	};
	void pop_back() {
		this->length--;
	}

	//获得vector元素个数
	int size() {
		return this->length;
	};
	//重载[]操作符
	T& operator[](int i) {
		return this->data[i];
	};
	//重载=操作符
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
	T* data;    //存储数据的数组
	int length;   //数组元素
	int maxlength;
};


//自定义高级队列对标std::Queue
template <class T>//泛型
class mQueue
{
public:
	//初始化创建队列
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

	//private: //也可以直接用来链表list直接构造
	struct QueueNode
	{
		T data;
		QueueNode* next;
		QueueNode(const T& Newdata, QueueNode* nextnode = NULL) : data(Newdata), next(nextnode)
		{
		}
		// QueueNode() = default;
	};
	QueueNode* Front; //队头指针
	QueueNode* rear;  // 队尾指针
	int count;
};

//子句
class Clause
{

	friend lit;

	friend Solver;

protected:
	bool learnt{ 0 };//是否为学习语句
	double Act{ 0 };//活跃度
	mVector<lit> lits;//动态数组做出的文字列表
	/// <summary>
	/// 判断当前语句和参数语句的活跃度大小
	/// </summary>
	/// <param name="x">被比较语句</param>
	/// <returns>如果当前小就true，否则false</returns>
	bool operator<(const Clause x)
	{
		return Act < x.Act;
	}
	void ReasonForLearn(Solver& S, lit p, mVector<lit>& out_reason);//记录冲突原因
	bool GetClauseInfluence(Solver& S, lit p);//传播，判断并赋值下一个变元
	bool IsClause(Solver S);//判断该子句能不能被化简，单子句不化简
	bool GetNewClause(Solver& S, mVector<lit> ps, bool learnt, Clause*& out_clause);//生成新的学习子句
	bool MakeClauseMoreCleaner(Solver& S);//化简子句，去掉已赋值变元，仅限第一层

};
