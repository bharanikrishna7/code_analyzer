LIMITATIONS OF PROJECT:
-----------------------
1. This project will not work when there are multiple types (function / class / alias / typedef / struct / enum) with same name.
2. Will not understand method overloading.
3. Will give false dependencies when a token matching the entry in Final Type Table is found 
   ex : Suppose final type table has an entry {A , namespace, global, file1.cpp }
		Now if file2.cpp has a semi-expression => int A;
		
		This program will say file2.cpp depends on file1.cpp which is incorrect. This happens since
		it finds a matching token and it doesnot (no functions present) analyze what type of token it
		got.

		
FOR TESTING:
-------------
1. The test files are placed in Test folder.
2. For better understanding use VERBOSE Mode.
   Can be done by setting VERBOSE variable to true