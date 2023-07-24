all: dz6

dz6: ioctl.c
        gcc -o ioctl ioctl.c
clean:
        rm*.o
