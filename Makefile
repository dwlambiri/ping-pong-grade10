CCFLAGS =	-O2 -DDARIUS -g -Wall -I/usr/local/include -fmessage-length=0
CC = g++

OBJS =		pong.o

LIBS = -L/usr/local/lib -lallegro -lallegro_main -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec


TARGET =	pong

$(OBJS) : d-allegro.cpp ping-pong-main.cpp config-manager.cpp d-allegro.h pong.cpp write-results.cpp
	$(CC) -c $(CCFLAGS) -o pong.o pong.cpp

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
