#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Agenda:
// Name hiding
// Copy constructor
// Move constructor
// emplace method
// assign operator overload over class hierarchy
// Methods of empty struct

struct Empty{};

class A
{
public:
    int data;
    int* numbers;

    A(int data) : data(data), numbers(new int[data]) 
    {
        for (int i(0); i < data; i++)
            numbers[i] = i+1;
    }

    A(const A& other) : data(other.data), numbers(new int[other.data])
    {
        memcpy(numbers, other.numbers, data*sizeof(int));
    }

    A(A&& other) : data(other.data), numbers(nullptr)
    {
        std::swap(numbers, other.numbers);
    }

    A& operator=(const A& other)
    {
        data = other.data;
        if (numbers != nullptr)
            delete[] numbers;
        numbers = new int[other.data];
        memcpy(numbers, other.numbers, data*sizeof(int));
        return *this;
    }

    A& operator=(A&& other)
    {
        data = other.data;
        std::swap(numbers, other.numbers);
        return *this;
    }

    // Unified assignment operator
    /*A& operator=(A other)
    {
        data = std::exchange(other.data, 0);
        std::swap(numbers, other.numbers);
        return *this;
    }*/

    int GetData(int a)
    {
        return data+a;
    }

    ~A() { delete[] numbers; }
};

class B : public A
{
    string* str = nullptr;
public:
    B(int data) : A(data), str(new string("Hello world!"s)) {}

    B(const B& other) : A(other), str(other.str) {}

    B& operator=(const B& other)
    {
        if(this != &other)
        {
            A::operator=(other);
            delete str;
            str = other.str ? new string(*other.str) : nullptr;
        }
        return *this;
    }

    int GetData()
    {
        return data;
    }
    void PrintNumbers()
    {
        for (int i(0); i < data; i++)
            cout << numbers[i] << ' ';
        cout << endl;
    }

    ~B() { delete str; }

    using A::GetData;
};


void main()
{
    B b(2);
    cout << b.GetData(2) << endl;

    B a(4);
    a = b;
    // B a = b;                    // Copy constructor called
    a.PrintNumbers();


    /*vector<B> vec {3, 6};
    vec.emplace_back(4);

    for (auto& b : vec)
        b.PrintNumbers();

    vector<int> v{7,8};
    vector<int> v2{1,2,3};
    int* seq = v.data();            // Dangling pointer after assignment

    v = v2;*/

    Empty g;
    Empty h;
    a = b;


    // Move semantic
    A aa(5);
    A bb(std::move(aa));

    A aaa(5);
    A bbb(std::move(aaa));

    cin.get();
