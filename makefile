CC = gcc
OBJFILES = main.o motors.o sensors.o
TARGET = startcar
LDFLAGS = -lwiringPi -lpthread -lrt

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
