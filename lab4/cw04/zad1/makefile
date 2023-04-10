all:
	make fork
	make exec
	make report

fork: 
	gcc -Wall assignment1fork.c -o assignment1fork.exe

exec: 
	gcc -Wall assignment1exec.c -o assignment1exec.exe

report:
	echo "./assignment1fork.exe ignore" >> report.txt
	./assignment1fork.exe ignore >> report.txt
	echo "" >> report.txt
	echo "./assignment1fork.exe handler" >> report.txt
	./assignment1fork.exe handler >> report.txt
	echo "" >> report.txt
	echo "./assignment1fork.exe mask" >> report.txt
	./assignment1fork.exe mask >> report.txt
	echo "" >> report.txt
	echo "./assignment1fork.exe pending" >> report.txt
	./assignment1fork.exe pending >> report.txt
	echo "" >> report.txt
	echo "-----------------------------" >> report.txt
	echo "" >> report.txt
	echo "./assignment1exec.exe ignore" >> report.txt
	./assignment1exec.exe ignore >> report.txt
	echo "" >> report.txt
	echo "./assignment1exec.exe mask" >> report.txt
	./assignment1exec.exe mask >> report.txt
	echo "" >> report.txt
	echo "./assignment1exec.exe pending" >> report.txt
	./assignment1exec.exe pending >> report.txt
	echo "" >> report.txt

clean:
	rm -f assignment1fork.exe assignment1exec.exe
	rm -f report.txt