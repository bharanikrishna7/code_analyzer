#ifndef TESTEXEC_H
#define TESTEXEC_H
//////////////////////////////////////////////////////////////////////
// TestExec.h - Package for the testing and verifying the code      //
// Version: 1.0                                                     //
// Language:    C++, Visual Studio 2015                             //
// Platform:    MSI GE62 - 2QD, Windows 10                          //
// Application: Parser component, CSE687 - Object Oriented Design   //
// Author:      Venkata Chekuri, SUID : 356579351                   //
//              vbchekur@syr.edu                                    //
//////////////////////////////////////////////////////////////////////
/* 
 * Package Operations:
 * -------------------
 * Test the Code for all possible test cases to make sure everything works
 * as expected. 
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 08 Feb 2016
 * - Initial Release
 *
*/
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/ITokCollection.h"

namespace Scanner
{
	class TestExec
	{
	public:
		TestExec();
		~TestExec();
		void Test4();
		void Test5();
		void Test6();
		void Test7();
		void Test8();
		void Test9();
		void Test9Contd();
		void Test10();
	};
}
#endif