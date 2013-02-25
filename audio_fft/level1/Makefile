###############################################################################
# Example makefile for portaudiov19 + fftw3 projects.
# ihsan@kehribar.me wrote this file.
###############################################################################
CFLAGS = `pkg-config --cflags portaudio-2.0 fftw3`
LIBS = `pkg-config --libs portaudio-2.0 fftw3`

TARGET = test

.PHONY: clean

$(TARGET):
	g++ $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f *.o *~ ./$(TARGET) 