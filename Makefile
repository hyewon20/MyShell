CC = gcc

FILES = mypwd myenv myls mycat myhead mytail mysort mycmp mycp mymv mymkdir myrmdir myrm
TARGET = myshell

all: $(FILES)
all: $(TARGET)

clean:
	rm -f $(FILES) *~
	rm -f $(TARGET) *~
