/*���༭��*/
/*������ 2023.9.3 23:58*/
/*qq��2950171570*/
/*email��Jin0714@outlook.com  �ظ���Ե*/



#include "GlobalOfOldProgram.h"

status IsEmptyClause(HeadNode*& LIST)
{
	HeadNode* PHead = LIST;
	while (PHead != nullptr)
	{
		if (PHead->Num == 0)
			return TRUE;
		PHead = PHead->down;
	}
	return FALSE;
}

HeadNode* IsSingleClause(HeadNode* Pfind)
{
	while (Pfind != nullptr)
	{
		if (Pfind->Num == 1)
			return Pfind;
		Pfind = Pfind->down;
	}
	return nullptr;
}

HeadNode* Duplication(HeadNode*& LIST)
{
	HeadNode* oldHead = LIST;
	HeadNode* newHead = new HeadNode;
	newHead->down = nullptr;
	newHead->up = nullptr;
	newHead->Num = oldHead->Num;
	newHead->right = nullptr;
	DataNode* oldData = oldHead->right;
	DataNode* newData = new DataNode;
	newData->data = oldData->data;
	newData->next = nullptr;
	newData->pre = nullptr;
	HeadNode* PnewHead = newHead;
	PnewHead->right = newData;
	for (oldData = oldData->next; oldData != nullptr; oldData = oldData->next)
	{
		DataNode* nnewData = new DataNode;
		nnewData->data = oldData->data;
		nnewData->next = nullptr;
		nnewData->pre = newData;
		newData->next = nnewData;
		newData = newData->next;
	}
	for (oldHead = oldHead->down; oldHead != nullptr; oldHead = oldHead->down)
	{
		HeadNode* nnewHead = new HeadNode;
		nnewHead->down = nullptr;
		nnewHead->Num = oldHead->Num;
		nnewHead->up = PnewHead;
		PnewHead->down = nnewHead;
		PnewHead = PnewHead->down;
		oldData = oldHead->right;
		newData = new DataNode;
		newData->data = oldData->data;
		newData->next = nullptr;
		newData->pre = nullptr;
		PnewHead->right = newData;
		for (oldData = oldData->next; oldData != nullptr; oldData = oldData->next)
		{
			DataNode* nnewData = new DataNode;
			nnewData->data = oldData->data;
			nnewData->next = nullptr;
			nnewData->pre = newData;
			newData->next = nnewData;
			newData = newData->next;
		}
	}
	return newHead;
}

HeadNode* ADDSingleClause(HeadNode*& LIST, int Var) //���ӵĵ��Ӿ�λ�������ͷ
{
	HeadNode* AddHead = new HeadNode;
	DataNode* AddData = new DataNode;
	AddData->data = Var;
	AddData->next = nullptr;
	AddData->pre = nullptr;
	AddHead->right = AddData;
	AddHead->Num = 1;
	AddHead->down = LIST;
	AddHead->up = nullptr;
	LIST->up = AddHead;
	LIST = AddHead;
	return LIST;
}

void DeleteDataNode(int temp, HeadNode*& LIST)
{
	bool ifChange{ TRUE };
	bool iffChange{ TRUE };
	HeadNode* pHeadNode = LIST;
	while (pHeadNode != nullptr)
	{
		ifChange = TRUE;
		DataNode* rear = pHeadNode->right;
		while (rear != nullptr)
		{
			iffChange = TRUE;
			if (rear->data == temp)
			{
				pHeadNode = DeleteHeadNode(pHeadNode, LIST);
				ifChange = FALSE;
				break;
			}
			else if (abs(rear->data) == abs(temp)) //�����Ǿ���ֵ��Ȳ����ýڵ�
			{
				if (rear == pHeadNode->right)  //ͷ�ڵ�ɾ��
				{
					pHeadNode->right = rear->next;
					if (rear->next != nullptr)
						rear->next->pre = nullptr;
					DataNode* temp;
					temp = rear;
					rear = rear->next;
					delete temp;
					pHeadNode->Num--;
					iffChange = FALSE;
				}
				else//ɾ����ͨ�ڵ�
				{
					rear->pre->next = rear->next;
					if (rear->next != nullptr)
						rear->next->pre = rear->pre;
					DataNode* temp;
					temp = rear;
					rear = rear->next;
					delete temp;
					pHeadNode->Num--;
					iffChange = FALSE;
				}
			}
			if (iffChange)
				rear = rear->next;
		}
		if (ifChange)
			pHeadNode = pHeadNode->down;
	}
}

HeadNode* DeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST)
{
	if (!delGoal)
	{
		if (LIST != nullptr)
			return LIST->down;
		else
		{
			return nullptr;
		}
	}
	HeadNode* ans;
	if (delGoal == LIST)
	{
		LIST = delGoal->down;
		ans = LIST;
		if (LIST != nullptr)
			LIST->up = nullptr;
	}
	else
	{
		delGoal->up->down = delGoal->down;
		if (delGoal->down != nullptr)
			delGoal->down->up = delGoal->up;
		ans = delGoal->down;
	}
	DataNode* PData = delGoal->right;
	while (PData != nullptr)
	{
		DataNode* PPData = PData;
		PData = PData->next;
		delete PPData;
		PPData = nullptr;
	}
	delete delGoal;
	return ans;
}

void XDeleteHeadNode(HeadNode* delGoal, HeadNode*& LIST)
{
	if (!delGoal)
		return;
	if (delGoal == LIST)
	{
		LIST = delGoal->down;
		if (LIST != nullptr)
			LIST->up = nullptr;
	}
	else
	{
		delGoal->up->down = delGoal->down;
		if (delGoal->down != nullptr)
			delGoal->down->up = delGoal->up;
	}
	DataNode* temp = delGoal->right;
	delete temp;
	delete delGoal;
}

void DeleteList(HeadNode*& LIST)
{
	HeadNode* PHead = LIST;
	while (PHead != nullptr)
	{
		DataNode* PData = PHead->right;
		while (PData != nullptr)
		{
			DataNode* PPData = PData;
			PData = PData->next;
			delete PPData;
			PPData = nullptr;
		}
		HeadNode* PPHead = PHead;
		PHead = PHead->down;
		delete PPHead;
		PPHead = nullptr;
	}
}

int choose(HeadNode* head)
{
	std::unordered_map<int, int> frequencyMap;
	int maxFrequency = 0;
	int mostFrequentElement{ head->right->data };
	for (HeadNode* h = head; h != nullptr; h = h->down)
	{
		for (DataNode* d = h->right; d != nullptr; d = d->next)
		{
			frequencyMap[d->data]++;
			if (frequencyMap[d->data] > maxFrequency)
			{
				maxFrequency = frequencyMap[d->data];
				mostFrequentElement = d->data;
			}
		}
	}
	return mostFrequentElement;
}

HeadNode* FindHeadOfOne(HeadNode*& LIST, int num)
{
	HeadNode* PHead = LIST;
	while (PHead != nullptr)
	{
		DataNode* PData = PHead->right;
		while (PData != nullptr)
		{
			if (PData->data == num)
				return PHead;
			PData = PData->next;
		}
		PHead = PHead->down;
	}
	return nullptr;
}

void DeleteHeadOfOne(HeadNode*& LIST, howMany* howManyTimes, int VARNUM)
{
	for (int i{ 1 }; i <= VARNUM; i++)
	{
		if (howManyTimes[i].howM <= 1)
		{
			HeadNode* delGoal;
			do
			{
				delGoal = FindHeadOfOne(LIST, i);
				XDeleteHeadNode(delGoal, LIST);
			} while (delGoal != nullptr);
		}
	}
	delete[] howManyTimes;
}

status DPLL(HeadNode*& LIST, consequence* result, int chooseWay)
{
	//���Ӿ����
	//system("cls");
	//cout << "���ڼ���...";
	HeadNode* Pfind = LIST;
	HeadNode* SingleClause = IsSingleClause(Pfind);
	while (SingleClause != nullptr)
	{
		SingleClause->right->data > 0 ? result[abs(SingleClause->right->data) - 1].value = TRUE : result[abs(SingleClause->right->data) - 1].value = FALSE;
		int temp = SingleClause->right->data;
		XDeleteHeadNode(SingleClause, LIST);//ɾ�����Ӿ���һ��
		DeleteDataNode(temp, LIST);//ɾ����Ȼ��෴���Ľڵ�
		if (!LIST)
		{
			return TRUE;
		}
		else if (IsEmptyClause(LIST))
		{
			DeleteList(LIST);
			return FALSE;
		}
		Pfind = LIST;
		SingleClause = IsSingleClause(Pfind);//�ص�ͷ�ڵ�������м���Ƿ��е��Ӿ�
	}
	//���Ѳ���
	int Var{ 0 };
	if (chooseWay == 1)
		Var = choose(LIST);//ѡȡ��Ԫ 
	else
		Var = LIST->right->data;
	HeadNode* replica = Duplication(LIST);//���LIST�ĸ���replica
	HeadNode* temp1 = ADDSingleClause(LIST, Var);//װ�ر�Ԫ��Ϊ���Ӿ�
	bool isTrue{ TRUE };
	bool iisTrue{ TRUE };
	isTrue = DPLL(temp1, result, chooseWay);
	if (isTrue)
	{
		return TRUE;
	}
	else
	{
		HeadNode* temp2 = ADDSingleClause(replica, -Var);
		iisTrue = DPLL(temp2, result, chooseWay);
		if (iisTrue)
			return TRUE;
		else
			return FALSE;
	}
}