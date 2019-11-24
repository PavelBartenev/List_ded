#include <stdio.h>
#include <stdlib.h>

typedef int type;

int const poison = -1;

struct list_d
{
	type* data;
	int* next;
	int* prev;

	int head;
	int tail;
	int next_free;
	int size;
};

list_d* list_d_constructor();

int insert_back(type, list_d*);

int insert_front(type, list_d*);

int search(list_d*, int);

int logic_insert(type, list_d*, int);

type get_back(list_d*);

type get_front(list_d*);

type logic_get(list_d*, int);

type logic_get_data(list_d*, int);

int insert_after(type, list_d*, int);

int insert_before(type, list_d*, int);

type get(list_d*, int);

type get_data(list_d*, int);

int sort(list_d*);

int change(list_d*, int, int);

int sort2(list_d*);

int main()
{
	list_d* list = list_d_constructor();

	for (int i = 0; i < 10; i++)
		insert_back(i, list);

	for (int i = 30; i > 20; i--)
		insert_front(i, list);

	logic_insert(555, list, 3);
	insert_after(999, list, 5);

	sort2(list);

	for (int i = 1; i < 23; i++)
		printf("%d ", get_data(list, i));

	printf("\n%d", logic_get(list, 0));
	printf("\n%d", logic_get(list, 5));


	return 0;
}

list_d* list_d_constructor()
{
	list_d* new_list = (list_d*)calloc(1, sizeof(list_d));

	if (!new_list)
		return (nullptr);

	new_list->data = (type*)calloc(100, sizeof(type));
	new_list->next = (int*)calloc(100, sizeof(int));
	new_list->prev = (int*)calloc(100, sizeof(int));

	if (!(new_list && new_list->data && new_list->prev && new_list->next))
		exit(-1);

	new_list->head = 0;
	new_list->tail = 0;
	new_list->next_free = 1;
	new_list->size = 0;

	for (int i = 1; i < 100; i++)
	{
		new_list->next[i] = i + 1;
		new_list->prev[i] = poison;
	}

	return new_list;
}

int insert_back(type data, list_d* list)
{
	if (!list)
		return -2;

	if (list->size == 0)
	{
		list->head = list->next_free;
		list->tail = list->next_free;

		list->next_free = list->next[list->head];
		list->next[list->head] = 0;
		list->prev[list->head] = 0;

		list->data[list->head] = data;

		list->size++;

		return 0;
	}

	int current = list->next_free;

	list->data[current] = data;

	list->next_free = list->next[current];

	list->next[current] = 0;

	list->prev[current] = list->tail;

	list->next[list->tail] = current;

	list->tail = current;

	list->size++;

	return 0;
}

int insert_front(type data, list_d* list)
{
	if (!list)
		return -3;

	if (list->size == 0)
	{
		list->head = list->next_free;
		list->tail = list->next_free;

		list->next_free = list->next[list->head];
		list->next[list->head] = 0;
		list->prev[list->head] = 0;

		list->data[list->head] = data;

		list->size++;

		return 0;
	}

	int current = list->next_free;

	list->data[current] = data;

	list->prev[current] = 0;

	list->next_free = list->next[current];

	list->next[current] = list->head;

	list->prev[list->head] = current;

	list->head = current;

	list->size++;

	return 0;
}

int search(list_d* list, int number)
{
	if ((number >= list->size) || (number < 0))
		return -4;

	int current = list->head;

	for (int i = 0; i < number; i++)
		current = list->next[current];

	return current;
}

int logic_insert(type data, list_d* list, int number)
{
	if ((number > list->size) || number < 0)
	{
		return -5;
	}

	if (number == list->size)
	{
		insert_back(data, list);

		return 0;
	}

	if (number == 0)
	{
		insert_front(data, list);

		return 0;
	}

	int previous = search(list, number - 1);

	int current = list->next_free;

	list->next_free = list->next[current];

	list->data[current] = data;

	list->prev[current] = previous;

	list->next[current] = list->next[previous];

	list->prev[list->next[current]] = current;

	list->next[previous] = current;

	list->size++;

	return 0;
}

type get_back(list_d* list)
{
	if (list->size == 0)
		return -6;

	type data = list->data[list->tail];

	list->next[list->prev[list->tail]] = 0;

	list->next[list->tail] = list->next_free;

	list->next_free = list->tail;

	list->prev[list->tail] = poison;

	list->tail = list->prev[list->tail];

	list->size--;

	return data;
}

type get_front(list_d* list)
{
	if (list->size == 0)
		return -7;

	type data = list->data[list->head];

	list->prev[list->next[list->head]] = 0;

	int current = list->head;

	list->head = list->next[current];

	list->next[current] = list->next_free;

	list->next_free = list->head;

	list->prev[current] = poison;

	list->size--;

	return data;
}

type logic_get(list_d* list, int number)
{
	if ((number < 0) || (number >= list->size))
		return -8;

	if (number == 0)
		return get_front(list);

	if (number == list->size - 1)
		return get_back(list);

	int current = search(list, number);

	type data = list->data[current];

	list->prev[list->next[current]] = list->prev[current];

	list->next[list->prev[current]] = list->next[current];

	list->next[current] = list->next_free;

	list->next_free = current;

	list->prev[current] = poison;

	list->size--;

	return data;
}

type logic_get_data(list_d* list, int number)
{
	int current = search(list, number);

	return list->data[current];
}

type get_data(list_d* list, int number)
{
	return list->data[number];
}

int insert_after(type data, list_d* list, int number)
{
	if (list->prev[number] == poison)
		return -9;

	int current = list->next_free;
	list->next_free = list->next[current];

	list->data[current] = data;

	list->prev[current] = number;
	list->next[current] = list->next[number];

	list->prev[list->next[number]] = current;
	list->next[number] = current;

	list->size++;

	return 0;
}

int insert_before(type data, list_d* list, int number)
{
	if (list->prev[number] == poison)
		return -10;

	int current = list->next_free;
	list->next_free = list->next[current];

	list->next[current] = number;
	list->prev[current] = list->prev[number];

	list->next[list->prev[number]] = current;
	list->prev[number] = current;

	list->size++;

	return 0;
}

type get(list_d* list, int number)
{
	if (list->prev[number] == poison)
		return -11;

	type data = list->data[number];

	list->prev[list->next[number]] = list->prev[number];
	list->next[list->prev[number]] = list->next[number];

	list->next[number] = list->next_free;
	list->next_free = number;

	list->prev[number] = poison;

	list->size--;

	return data;
}

int change(list_d* list, int number1, int number2)
{

	if ((list->prev[number1] == poison) || (list->prev[number2] == poison))
		return -12;

	if (number1 == number2)
		return 0;

	if (!(list->next[number1] || list->next[number2]))
		return 0;

	if (list->next[number1] == number2)
	{
		type data = list->data[number1];
		type next = list->next[number1];
		type prev = list->prev[number1];

		if (list->next[number2])
			list->prev[list->next[number2]] = number1;
		else
			list->tail = number1;

		if (list->prev[number1])
			list->next[list->prev[number1]] = number2;
		else
			list->head = number2;

		list->data[number1] = list->data[number2];
		list->next[number1] = list->next[number2];
		list->prev[number1] = number2;

		list->data[number2] = data;
		list->next[number2] = number1;
		list->prev[number2] = prev;

		return 0;
	}

	if (list->next[number2] == number1)
	{
		type data = list->data[number1];
		type next = list->next[number1];
		type prev = list->prev[number1];

		if (list->next[number1])
			list->prev[list->next[number1]] = number2;
		else
			list->tail = number2;

		if (list->prev[number2])
			list->next[list->prev[number2]] = number1;
		else
			list->head = number1;

		list->data[number1] = list->data[number2];
		list->next[number1] = number2;
		list->prev[number1] = list->prev[number2];

		list->data[number2] = data;
		list->next[number2] = next;
		list->prev[number2] = number1;

		return 0;
	}

	if (list->prev[number1])
		list->next[list->prev[number1]] = number2;
	else
		list->head = number2;
	 
	if (list->next[number1])
		list->prev[list->next[number1]] = number2;
	else
		list->tail = number2;

	if (list->prev[number2])
		list->next[list->prev[number2]] = number1;
	else
		list->head = number1;

	if (list->next[number2])
		list->prev[list->next[number2]] = number1;
	else
		list->tail = number1;

	type data = list->data[number1];
	type next = list->next[number1];
	type prev = list->prev[number1];

	list->data[number1] = list->data[number2];
	list->next[number1] = list->next[number2];
	list->prev[number1] = list->prev[number2];

	list->data[number2] = data;
	list->next[number2] = next;
	list->prev[number2] = prev;

	return 0;
	
}
/**********************************************
int sort(list_d* list)
{
	int current = list->head;

	for (int i = 1; i <= list->size; i++)
	{
		list->prev[i] = current;
		current = list->next[current];
	}

	for (int i = 1; i <= list->size; i++)
		list->next[i] = list->data[list->prev[i]];

	for (int i = 1; i <= list->size; i++)
	{
		list->data[i] = list->next[i];
		list->next[i] = i + 1;
		list->prev[i] = i - 1;
	}

	list->next[list->size] = 0;
	
	list->head = 1;
	list->tail = list->size;

	return 0;
}
**********************************************/

int sort2(list_d* list)
{
	int size = list->size;
	int max_index = list->head;
	int current_index = list->head;

	for (int i = 0; i < size; i++)
	{
		current_index = list->next[current_index];

		if (current_index > max_index) max_index = current_index;
	}

	for (int i = 1; i <= max_index; i++)
	{
		for (int j = 1; j <= max_index; j++)
		{
			if (list->prev[j] == poison)
				change(list, j, j + 1);

			else if (j > list->next[j])
				change(list, j, list->next[j]);
		}
	}

	return 0;
}


