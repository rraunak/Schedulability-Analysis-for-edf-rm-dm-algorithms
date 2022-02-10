all:main.cpp 
	g++ -g -Wall -o assignment4 main.cpp 
	./assignment4 A4_test.txt report.txt

clean: 
	$(RM) assignment4 report.txt
