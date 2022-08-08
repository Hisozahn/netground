all: master actuator

master:
	gcc master.c helpers.c -o build/master -g3

actuator:
	gcc actuator.c helpers.c -o build/actuator -g3
