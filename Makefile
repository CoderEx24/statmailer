main:
	gcc -O2 -o statmailer -lquickmail main.c stats.c


debug:
	gcc -g -o statmailer -lquickmail main.c stats.c


clean:
	rm statmailer 
