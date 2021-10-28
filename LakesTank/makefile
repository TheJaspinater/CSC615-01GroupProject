CC = gcc
CFLAGS = -Wall -lpthread
OBJ1 = test

run:
	rm -rf $(OBJ1)
	rm -rf *.o
	$(CC) $(CFLAGS)  driveTrain.c driveTrain.h softPwm.c softPwm.h sensors.c sensors.h gpioController.c gpioController.h main.c -o $(OBJ1)
	./$(OBJ1)
clean:
	rm -rf $(OBJ1)
	rm -rf *.o