# DSA-Assignment-1
This repositry contains my assignment of DSA. The header files were already given, and we implemented the cpp files to work the solution of the problems. There were three problems in the assignment to be solved using ADT concepts.

Overall Design
Since header files were locked without private members, I used a map-based storage pattern across all three problems. Each object pointer maps to its internal data structure, maintaining proper ADT principles while respecting the fixed interfaces.

Polynomial ADT : Used a linked list with nodes sorted by exponents. Each term stores coefficient and exponent. Operations moves in the list as addition merges two lists, multiplication uses nested loops for term pairs, derivative applies the power rule. The insertTerm method handles combining like terms and removing zeros automatically.

Text Editor ADT : Stored text as a string and cursor as integer in class. Insert uses string's insert method, delete removes from position cursor-1 to end (matching required output). Movement operations adjust cursor with bounds checking. Static counters track consecutive moves to format output correctly (only printing after second move).

UNO Game ADT : Built 76 card with no wilds, stored in vectors. Implemented card priority using numeric scoring: color match (1) > value match (2) > actions (3-5). Action cards modify game state immediately, Skip advances turn, Reverse flips direction, Draw Two forces next player to draw and skip. Fixed seed 1234 ensures reproducible shuffles.

Common Pattern was cppstatic map<const ClassName*, InternalData*> storage;

Challenges Faced
1. Working with Fixed Headers
Headers had no private members. Used static maps to associate each object with its internal state.
2. Text Editor Delete 
   started coded standard backspace (one character), but sthen delete removes everything from cursor 1 onwards. Required careful analysis of expected output.
3. Consecutive Moves in text editor
TextEditor needed to print "twice" only after second consecutive move. Used static counters to track previous editor and move count in twice ones in last as in assg. outputs.
4. Card Selection Priority
UNO required choosing best card from multiple playable options. Implemented numeric priority system where lower score is higher priority, making selection logic clear.
5. Memory Management
No destructors in some headers meant incomplete cleanup. For TextEditor, added destructor to erase map entry. For UNO and Polynomial, cleanup happens at program end so that way only.
