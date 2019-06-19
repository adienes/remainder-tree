#include <iostream>
#include <array>

using namespace std;

void test(array<int, 5> &var);

int main()
{
   	array<int, 5> arr = {1, 2, 3, 4, 5};
   	test(arr);
   	for(int i = 0; i < 5; i++){
   		cout << arr[i] << endl;
   	}
   
}

void test(array<int, 5> &var){
	for(int i = 0; i < 5; i++){
		var[i] += 1;
	}
}
