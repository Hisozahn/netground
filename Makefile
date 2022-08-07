all: master actuator

master:
	gcc master.c -o build/master -g3

actuator:
	gcc actuator.c -o build/actuator -g3
