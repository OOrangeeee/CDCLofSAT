/*���༭��*/
/*������ 2023.9.3 3:46*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#include<bits/stdc++.h>
using namespace std;
#define True 1
#define False -1
#define Unset 0
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
class lit;
class Solver;
class Clause;

//������
class lit
{
public:
    int x;
    lit() 
    { 
        this->x = 0; 
    }
    lit(int x)
    {
        this->x = (x > 0) ? (2 * x + 1) : (-2 * x);
    }
    bool sign()
    { //������ִ�����,�򷵻�
        return (this->x) & 1;
    }
    int var()
    { //�������ֶ�Ӧ�Ļ�������
        return (this->x) >> 1;
    }
    int index()
    { //������ת��Ϊ���������������ġ�С������
        return this->x;
    }
    int no()
    { // xΪ����-1��xΪż��+1��xΪ������ζ������xΪż����ζ�Ÿ�
        return this->x ^ 1;
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


//�Զ���߼�����
template <class T>
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

//����ʱ�õ�˥���������ʵ�ֺ������з�װ����������������Ĭ��Ϊ1��1��0
struct SearchParams
{
    double var_decay, clause_decay, rand_feq;
    SearchParams(double v = 1, double c = 1, double r = 0)
    {
        var_decay = v;
        clause_decay = c;
        rand_feq = r;
    }
};



/**************************
 * clause���Ķ���
 * ***************************/
class Clause
{
public:
    bool learnt = 0;//�Ƿ�Ϊѧϰ���
    double activity = 0;//��Ծ��
    mVector<lit> lits;//��̬���������������б�
    bool operator<(const Clause x)//�ж����Ļ�Ծ�ȴ�С
    {
        return activity < x.activity;
    }
    void calcReason(Solver& S, lit p, mVector<lit>& out_reason);//��ͻԭ��
    bool Clause_new(Solver& S, mVector<lit> ps, bool learnt, Clause*& out_clause);//н�ʾ�
    bool locked(Solver S);
    bool simplify(Solver& S);
    bool propagate(Solver& S, lit p);

};
