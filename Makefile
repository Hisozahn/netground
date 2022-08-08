all: master actuator

master:
	gcc master.c parser.c -o build/master -g3

actuator:
	gcc actuator.c parser.c -o build/actuator -g3
