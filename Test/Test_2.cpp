// Loop and conditional scopes Test
#include <algorithm>
#include <cmath>
#include <string>

namespace testNamespace {
	class testClass {
	private:
		int _pointless;
	public:
		void ForLoopTest(float* x, unsigned n) {
	    	for(int i = 0; i < 5; i++)
	    	{
	    		std::cout << "hello\n";
	    	}
	    }

	    int whileLoopTest()
	    {
	    	while(true)
	    	{
	    		std::cout << "hello\n";
	    	}
	    }

	    std::string IfTest(int one, int two){
	    	if(one < two)
	    	{
	    		return "TWO";
	    	}

	    	return "ONE";
	    }

	    void DoubleLoopsTest()
	    {
	    	bool sw = true;
	    	while(true)
	    	{
	    		while(true)
	    		{
	    			sw = false;
	    			std::cout << "hello from inside\n";
	    		}
	    		sw = true;
	    		std::cout << "hello from outside\n";
	    	}
	    }
	};
}
