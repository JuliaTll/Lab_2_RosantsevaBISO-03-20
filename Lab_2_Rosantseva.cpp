#include <iostream>

using namespace std;

class Item {

private:

	int value;

	Item* previous;

public:

	Item(Item* prevItem, int newValue) {

		previous = prevItem;

		value = newValue;

	}

	Item* getPrevious() {

		return previous;

	}

	void setPrevious(Item* prevItem) {

		previous = prevItem;

	}

	int getValue() {

		return value;

	}

	void setValue(int newValue) {

		value = newValue;

	}

	int& getValueAddress() {

		return value;

	}

};

class Stack {

private:

	Item* top; // Указатель на последний элемент

	unsigned int size; // Размер стэка

public:

	long N_op;

	Stack(Stack* srcsStack) {

		size = srcsStack->getSize();

		top = nullptr;

		N_op += 3;

		if (size) {

			N_op += 8;

			Item* itemFromSource = srcsStack->top;

			Item* itemPrevious = new Item(nullptr, itemFromSource->getValue());

			top = itemPrevious;

			for (unsigned int i = size - 1; i > 0; --i) {

				N_op += 9;

				itemFromSource = itemFromSource->getPrevious();

				Item* itemCurrent = new Item(nullptr, itemFromSource->getValue());

				itemPrevious->setPrevious(itemCurrent);

				itemPrevious = itemCurrent;

			}

		}

	}

	Stack() {

		top = nullptr;

		size = 0;

		N_op += 2;

	}

	~Stack() {

		clear();

	}

	void push(int newValue) {

		N_op += 5;

		Item* newItem = new Item(top, newValue);

		top = newItem;

		++size;

	}

	int pop() {

		// checkNullPtr();

		N_op += 8;

		Item* lastItem = top;

		top = lastItem->getPrevious();

		int lastValue = lastItem->getValue();

		delete lastItem;

		--size;

		return lastValue;

	}

	int first() {

		N_op += 2;

		return top->getValue();

	}

	unsigned int getSize() {

		N_op += 1;

		return size;

	}

	void clear() {

		Item* currentItem;

		for (unsigned int i = size; i > 0; --i) {

			currentItem = top;

			top = currentItem->getPrevious();

			delete currentItem;

		}

		size = 0;

	}

	void set(int newValue, unsigned int pos) {

		N_op += 4;

		Stack temp(this);

		for (int i = 0; i < pos; i++) {

			temp.push(this->pop());

			N_op += 2;

		}

		this->top->setValue(newValue);

		for (int i = 0; i < pos; i++) {

			this->push(temp.pop());

			N_op += 2;

		}

	}

	int get(unsigned int pos) {

		// checkPos(pos);

		Stack temp(this);

		int result;

		N_op += 6;

		for (int i = 0; i < pos; i++) {

			temp.push(this->pop());

			N_op += 2;

		}

		result = first();

		for (int i = 0; i < pos; i++) {

			this->push(temp.pop());

			N_op += 2;

		}

		return result;

	}

	void swap(int index1, int index2) {

		int temp = this->get(index1);

		N_op += 1;

		set(get(index2), index1);

		set(temp, index2);

	}

	//

	void sort() {

		int n = this->getSize();

		N_op += 2;

		for (int i = 1; i < n; i++) {

			N_op += 3;

			for (int j = 0; j < n - i; j++) {

				N_op += 5;

				if (get(j) > get(j + 1)) {

					N_op += 1;

					swap(j, j + 1);

				}

			}

		}

	}

	void checkNullPtr() {

		N_op += 1;

		if (top == nullptr)

			N_op += 1;

		throw std::runtime_error("Стэк пустой!");

	}

	void checkPos(unsigned int pos) {

		N_op += 1;

		if (pos > size)

			N_op += 1;

		throw std::runtime_error("Значение pos больше чем размер стэка!");

	}

};

int main() {

	setlocale(LC_ALL, "Russian");

	Stack s = new Stack();

	clock_t start, end;

	int fullTime;

	int randomNumer;

	for (int i = 1; i <= 10; i++) {

		srand(time(0));

		for (int j = 0; j < i * 100; j++) {

			randomNumer = rand() % 100;

			s.push(randomNumer);

		}

		s.N_op = 0;

		start = clock();

		s.sort();

		end = clock();

		fullTime = (double)end - start;

		cout << "Номер сортировки: " << i << " Колличество отсортированных элементов: " << 100 * i << " Время сортировки (s): " << static_cast<double>(fullTime) / 1000000 << " Колличество операций (N_op): " << s.N_op << endl; // Шаг в 300 элементов

		s.clear();

	}

	return 0;

}