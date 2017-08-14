// gcc -Wall -lm -O2 -pipe -std=c99 rainbow.c -o rainbow
#define _BSD_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

// parameters value range [0..1]
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ) {
  int i;
  float f, p, q, t;

  h = h * 360;
  
  if( s == 0 ) {
    // achromatic (grey)
    *r = *g = *b = v;
    return;
  }
  
  h /= 60;			// sector 0 to 5
  i = (int) h;
  f = h - i;			// factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );
  
  switch( i ) {
  case 0:
    *r = v;
    *g = t;
    *b = p;
    break;
  case 1:
    *r = q;
    *g = v;
    *b = p;
    break;
  case 2:
    *r = p;
    *g = v;
    *b = t;
    break;
  case 3:
    *r = p;
    *g = q;
    *b = v;
    break;
  case 4:
    *r = t;
    *g = p;
    *b = v;
    break;
  default:		// case 5:
    *r = v;
    *g = p;
    *b = q;
    break;
  }
}

void setleds(int fd, unsigned char leds[]) {
  for (int i = 0; i < 16; i++) {
    if (write(fd, leds + i*3, 3) < 0) {
      perror("argh2");
      exit(2);
    }
    tcdrain(fd);
    usleep(8000);
  }
}

int main(int argc, char *argv[]) {
  struct termios term_opt;
  int fd = open("/dev/ttyACM0", O_RDWR);
  if(fd < 0) {
    perror("argh");
    return -1;
  }
  tcgetattr(fd, &term_opt);
  cfmakeraw(&term_opt);
  cfsetispeed(&term_opt, B19200);
  cfsetospeed(&term_opt, B19200);
  tcsetattr(fd, TCSAFLUSH, &term_opt);

  unsigned char test[] = {0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,

                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,

                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,

                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00 };

  float h, s, v;
  float r, g, b;

  h = 0.0;
  s = 1.0;
  v = 1.0;

  while (1) {
    for (h = 0.01; h <= 0.99; h += 0.008) {
      HSVtoRGB(&r, &g, &b, h, s, v);

      for (int i = 15; i > 0; i--) {
        test[i*3] = test[(i-1)*3];
        test[i*3+1] = test[(i-1)*3+1];
        test[i*3+2] = test[(i-1)*3+2];
      }

      test[0] = (unsigned char)(g * 255);
      test[1] = (unsigned char)(r * 255);
      test[2] = (unsigned char)(b * 255);
      setleds(fd, test);
    }
  }
}
