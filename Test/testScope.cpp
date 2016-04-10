#include <iostream>
#include<conio.h>
using namespace std;

class person
{
public:
  string name;
  int number;
  enum Color { red, green, blue };
  enum class ColorClass { red, green = 20, blue };
  enum byte : unsigned char {};
};
namespace trial {
int main()
{
    person obj;

    cout<<"Enter the Name :";
    cin>>obj.name;

    cout<<"Enter the Number :";
    cin>>obj.number;

    cout << obj.name << ": " << obj.number << endl;

    getch();
    return 0;
}

int sum(int x, int y) {
	return x+y;
}

}