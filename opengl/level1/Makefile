###############################################################################
# Example makefile for portaudiov19 + fftw3 + openGL projects.
# ihsan@kehribar.me wrote this file.
###############################################################################
CFLAGS = `pkg-config --cflags gl glew portaudio-2.0 fftw3`
LIBS = `pkg-config --libs glew portaudio-2.0 fftw3`

ifeq ($(shell uname), Darwin)
	LIBS += -framework GLUT -framework OpenGL
else
	LIBS += `pkg-config --libs glut gl`
endif

TARGET = test

.PHONY: clean

$(TARGET):
	g++ $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f *.o *~ ./$(TARGET) 