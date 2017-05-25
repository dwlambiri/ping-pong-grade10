CCFLAGS =	-O2 -g -Wall -I/usr/local/include -fmessage-length=0
CC = g++

OBJS =		d-allegro.o ping-pong-main.o

LIBS = -L/usr/local/lib -lallegro -lallegro_main -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec


TARGET =	d-allegro

$(OBJS) : d-allegro.cpp ping-pong-main.cpp
	$(CC) -c $(CFLAGS) -o d-allegro.o d-allegro.cpp
	$(CC) -c $(CFLAGS) -o ping-pong-main.o ping-pong-main.cpp

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
