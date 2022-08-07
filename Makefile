all: master actuator

master:
	gcc master.c -o build/master

actuator:
	gcc actuator.c -o build/actuator
