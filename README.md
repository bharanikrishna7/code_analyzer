# code_analyzer
Lexical Scanner in C++

-------------
Descreption :
-------------
Code analysis consists of extracting lexical content from source code files, analyzing the code's syntax from its lexical content, and building an Abstract Syntax Tree (AST) that holds the results of our analysis. It is then fairly easy to build several backends that can do further analyses on the AST to construct code metrics, search for particular constructs, or some other interesting features of the code.


In this first project we will build and test a lexical scanner in C++ that consists of two packages:

1. Tokenizer
Extracts words, called tokens, from a stream of characters. Token boundaries are white-space characters, transitions between alphanumeric and punctuator characters, and comment and string boundaries. Certain classes of punctuator characters belong to single character or two character tokens so they require special rules for extraction.

2. SemiExpression
groups tokens into sets, each of which contain all the information needed to analyze some grammatical construct without containing extra tokens that have to be saved for subsequent analyses. SemiExpressions are determined by special terminating characters: semicolon, open brace, closed brace, newline when preceeded on the same line with '#', and colon when preceded by one of the three tokens "public", "protected", or "private".

------
Build:
------
To generate a .bat file which will run this program :-

1. To use this solution Visual Studio (2015 or newer) will be required. 
2. Run compile.bat through Visual Studio Developer Console to generate build the solution.
3. To execute the program again using Visual Studio Developer Console execute run.bat

Standard Visual Studio IDE Build :-

1. Set the default project and define appropriate variable to include the main of the program which is supposed to be executed. Also disable other main() by commenting out the defined variable.
2. Execute the program "using start without debugging"

P.S : Only the program which is being executed should be set to "Application", remaining projects should be set to "Static Library". This can be done through project properties.

----------
Changelog:
----------
v1.0 : 09/02/2016
- This is the initial release.
- It has a small bug which causes run.bat to be unable to locate files.
