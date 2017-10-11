// gcc -Wall -lm -O2 -pipe -std=c99 status.c -o status

#define _BSD_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

struct led_t {
  float r, g, b;
  int flickerSpeed;
  int flickerI;
  int dimmingSpeed;
  int dimmingI;
};

float getGradient(float input, float thresholds[], float colors[], int len) {
  for (int i = 0; i < len; i++) {
    if (input <= thresholds[i]) {
      if (i == 0) { 
        return colors[0];
      } else {
        float p = (input - thresholds[i-1]) / (thresholds[i] - thresholds[i-1]);
        return colors[i-1] * (1.0f - p) + colors[i] * p;
      }
    }
  }
  return colors[len-1];
}
void setGradient(struct led_t *led, float input, float thresholds[], 
                 float reds[], float greens[], float blues[], int len) {
  led->r = getGradient(input, thresholds, reds, len);
  led->g = getGradient(input, thresholds, greens, len);
  led->b = getGradient(input, thresholds, blues, len);
}
void setGradient2(struct led_t *led, float input,
                  float threshold1, float red1, float green1, float blue1, 
                  float threshold2, float red2, float green2, float blue2) {
  float thresholds[] = { threshold1, threshold2 };
  float reds[] = { red1, red2 };
  float greens[] = { green1, green2 };
  float blues[] = { blue1, blue2 };
  setGradient(led, input, thresholds, reds, greens, blues, 2);
}
void setGradient3(struct led_t *led, float input,
                  float threshold1, float red1, float green1, float blue1, 
                  float threshold2, float red2, float green2, float blue2,
                  float threshold3, float red3, float green3, float blue3) {
  float thresholds[] = { threshold1, threshold2, threshold3 };
  float reds[] = { red1, red2, red3 };
  float greens[] = { green1, green2, green3 };
  float blues[] = { blue1, blue2, blue3 };
  setGradient(led, input, thresholds, reds, greens, blues, 3);
}
void setGradient9(struct led_t *led, float input,
                  float threshold1, float red1, float green1, float blue1, 
                  float threshold2, float red2, float green2, float blue2,
                  float threshold3, float red3, float green3, float blue3,
                  float threshold4, float red4, float green4, float blue4,
                  float threshold5, float red5, float green5, float blue5,
                  float threshold6, float red6, float green6, float blue6,
                  float threshold7, float red7, float green7, float blue7,
                  float threshold8, float red8, float green8, float blue8,
                  float threshold9, float red9, float green9, float blue9) {
  float thresholds[] = { threshold1, threshold2, threshold3, threshold4, 
                         threshold5, threshold6, threshold7, threshold8,
                         threshold9 };
  float reds[] = { red1, red2, red3, red4, red5, red6, red7, red8, red9 };
  float greens[] = { green1, green2, green3, green4, green5, green6, green7, green8, green9 };
  float blues[] = { blue1, blue2, blue3, blue4, blue5, blue6, blue7, blue8, blue9 };
  setGradient(led, input, thresholds, reds, greens, blues, 9);
}

void setGradient2Test(const char *msg, float input,
                      float threshold1, float red1, float green1, float blue1, 
                      float threshold2, float red2, float green2, float blue2,
                      float outRed, float outGreen, float outBlue) {
  struct led_t led;
  led.r = led.g = led.b = -1.0f;
  setGradient2(&led, input, threshold1, red1, green1, blue1,
               threshold2, red2, green2, blue2);
  if (led.r != outRed || led.g != outGreen || led.b != outBlue) {
    fprintf(stderr, "%s: setGradient2Test(%.2f, %.2f, (%.2f,%.2f,%.2f)"
            ", %.2f, (%.2f,%.2f,%.2f)) = (%.2f,%.2f,%.2f) (expected: (%.2f,%.2f,%.2f))\n",
            msg, 
            input, threshold1, red1, green1, blue1,
            threshold2, red2, green2, blue2,
            led.r, led.g, led.b,
            outRed, outGreen, outBlue);
    exit(1);
  }
}
void getGradientTester() {
  setGradient2Test("1", 0.0,
                   0.4, 0.4, 0.0, 0.0,
                   0.6, 0.6, 0.0, 0.0,
                   0.4, 0.0, 0.0);
  setGradient2Test("2", 1.0,
                   0.4, 0.4, 0.0, 0.0,
                   0.6, 0.6, 0.0, 0.0,
                   0.6, 0.0, 0.0);
  setGradient2Test("3", 0.5,
                   0.4, 0.4, 0.0, 0.0,
                   0.6, 0.6, 0.0, 0.0,
                   0.5, 0.0, 0.0);
  setGradient2Test("4", 0.45,
                   0.4, 0.4, 0.0, 0.0,
                   0.6, 0.6, 0.0, 0.0,
                   0.45, 0.0, 0.0);
  setGradient2Test("4", 0.55,
                   0.4, 0.4, 0.0, 0.0,
                   0.6, 0.6, 0.0, 0.0,
                   0.55, 0.0, 0.0);
}


void setWeather(struct led_t leds[], int led) {

  FILE *json = popen("curl -s 'http://www.dmi.dk/Data4DmiDk/getData?by_hour=true&id=45008000&country=DK'", "r");
  /* FILE *json = popen("curl -s file:///home/dthomsen/Downloads/dmi.json", "r"); */
  if (NULL == json) {
    return;
  }
  char data[10000];
  if (0 == fread(data, 1, sizeof(data)-1, json)) {
    pclose(json);
    return;
  }
  pclose(json);

  char *data_today = strstr(data, "\"day1\":");
  if (NULL == data_today) {
    return;
  }

  int temp = 0;
  {
    const char weatherTempNeedle[] = "\"temp\":";
    char *dataWeatherTemp = strstr(data_today, weatherTempNeedle);
    if (NULL == dataWeatherTemp)
      return;
    temp = atoi(dataWeatherTemp + sizeof(weatherTempNeedle) - 1);
  }

  int wind = 0;
  {
    const char weatherWindNeedle[] = "\"wind_gust\":";
    char *dataWeatherWind = strstr(data_today, weatherWindNeedle);
    if (NULL == dataWeatherWind)
      return;
    wind = atoi(dataWeatherWind + sizeof(weatherWindNeedle) - 1);
  }

  float rain = 0.0f;
  {
    const char weatherRainNeedle[] = "\"precip\":";
    char *dataWeatherRain = strstr(data_today, weatherRainNeedle);
    if (NULL == dataWeatherRain)
      return;
    char rainTmpString[100] = { '\0' };
    for (int i = 0; i < 99; i++) {
      int index = sizeof(weatherRainNeedle);
      if (dataWeatherRain[index + i] == ',') {
        rainTmpString[i] = '.';
      } else if (dataWeatherRain[index + i] == '"') {
        rainTmpString[i] = '\0';
        break;
      } else {
        rainTmpString[i] = dataWeatherRain[index + i];
      }
    }
    rain = atof(rainTmpString);
  }

  /* printf("## temp=%d, wind=%d, rain=%.2f\n", temp, wind, rain); */

  setGradient3(&(leds[led]), temp,
               -2.0, 0.0, 0.0, 1.0,
               10, 1.0, 1.0, 0.0,
               25, 1.0, 0.0, 0.0);

  setGradient3(&(leds[led+1]), wind,
               5, 0.0, 1.0, 0.0,
               10, 1.0, 1.0, 0.0,
               25, 1.0, 0.0, 0.0);
  
  setGradient3(&(leds[led+2]), rain,
               0, 0.0, 1.0, 0.0,
               0.5, 1.0, 1.0, 0.0,
               4, 1.0, 0.0, 0.0);
}

static unsigned long cpusLastActive[128] = {0};
static unsigned long cpusLastIdle[128] = {0};
void setCpu(struct led_t leds[], int led) {
  char line[256];

  FILE *stat = fopen("/proc/stat", "r");
  if (NULL == stat) {
    fprintf(stderr, "Could not open '/proc/stat'\n");
    exit(1);
  }

  if (NULL == fgets(line, sizeof(line), stat)) {
    fclose(stat);
    fprintf(stderr, "Could not parse '/proc/stat'\n");
    exit(1);
  }

  int cpus = 0;
  unsigned long cpusActive[128] = {0};
  unsigned long cpusIdle[128] = {0};

  while (NULL != fgets(line, sizeof(line), stat)) {
    if (0 != strncmp("cpu", line, 3))
      break;
    int cpu;
    unsigned long user, nice, system, idle;
    sscanf(line, "cpu%d %lu %lu %lu %lu", &cpu, &user, &nice, &system, &idle);
    unsigned long active = user + nice + system;
    cpusActive[cpus] = active;
    cpusIdle[cpus] = idle;
    cpus++;
  }
  fclose(stat);

  if (cpusLastActive[0] > 0) {
    float usageSum = 0.0;
    for (int i = 0; i < cpus; i++) {
      unsigned long activeDelta = cpusActive[i] - cpusLastActive[i];
      unsigned long idleDelta = cpusIdle[i] - cpusLastIdle[i];
      float usage = 1.0 * activeDelta / (activeDelta + idleDelta);
      usageSum += usage;
    }

    setGradient9(&(leds[led]), usageSum,
                   0.0, 0.0, 0.0, 0.0, // off
                   1.0, 0.5, 0.0, 1.0, // purple
                   2.0, 0.0, 0.0, 1.0, // blue
                   3.0, 0.0, 1.0, 1.0, // cyan
                   4.0, 0.0, 1.0, 0.0, // green
                   5.0, 1.0, 1.0, 0.0, // yellow
                   6.0, 0.8, 0.4, 0.0, // orange
                   7.0, 1.0, 0.2, 0.2, // pink
                   8.0, 1.0, 0.0, 0.0); // red

  }

  for (int i = 0; i < cpus; i++) {
    cpusLastActive[i] = cpusActive[i];
    cpusLastIdle[i] = cpusIdle[i];
  }
}

void getMemSwapUsage(float *mem, float *swap) {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (meminfo == NULL) {
    fprintf(stderr, "Could not open '/proc/meminfo'\n");
    exit(1);
  }

  int memtotal, memfree, buffers, cached, swaptotal, swapfree;
  char line[256];
  while (NULL != fgets(line, 256, meminfo)) {
    sscanf(line, "MemTotal: %d", &memtotal);
    sscanf(line, "MemFree: %d", &memfree);
    sscanf(line, "Buffers: %d", &buffers);
    sscanf(line, "Cached: %d", &cached);
    sscanf(line, "SwapTotal: %d", &swaptotal);
    sscanf(line, "SwapFree: %d", &swapfree);
  }
  fclose(meminfo);

  *mem = 1.0 - (float) (memfree + buffers + cached) / memtotal;
  *swap = 1.0 - (float) swapfree / swaptotal;
}
void setMem(struct led_t leds[], int led) {
  float mem;
  float swap;
  getMemSwapUsage(&mem, &swap);

  setGradient2(&(leds[led]), mem,
               0.0, 0.0, 1.0, 0.0,
               1.0, 1.0, 0.0, 0.0);
}
void setSwap(struct led_t leds[], int led) {
  float mem;
  float swap;
  getMemSwapUsage(&mem, &swap);

  setGradient2(&(leds[led]), swap,
               0.0, 0.0, 1.0, 0.0,
               1.0, 1.0, 0.0, 0.0);
}

int getJenkinsState(const char *curl) {
  FILE *json = popen(curl, "r");
  if (NULL == json) {
    return 2;
  }
  char data[100000];
  int len;
  if (0 == (len = fread(data, 1, sizeof(data)-1, json))) {
    pclose(json);
    return 2;
  }
  pclose(json);
  data[len] = '\0';

  if (NULL != strstr(data, "\"red"))
    return 2;

  if (NULL != strstr(data, "\"aborted") || NULL != strstr(data, "\"yellow"))
    return 1;

  return 0;
}


void setJenkinsState(struct led_t leds[], int led, const char *curl) {
  int state = getJenkinsState(curl);
  if (state == 0) {
    leds[led].r = 0.0f;
    leds[led].g = 1.0f;
    leds[led].b = 0.0f;
  } else if (state == 1) {
    leds[led].r = 1.0f;
    leds[led].g = 0.5f;
    leds[led].b = 0.0f;
  } else {
    leds[led].r = 1.0f;
    leds[led].g = 0.0f;
    leds[led].b = 0.0f;
  }
}

void setJenkinsState2(struct led_t leds[], int led, const char *curl1, const char *curl2) {
  int state1 = getJenkinsState(curl1);
  int state2 = getJenkinsState(curl2);
  int state = state1 > state2 ? state1 : state2;
  if (state == 0) {
    leds[led].r = 0.0f;
    leds[led].g = 1.0f;
    leds[led].b = 0.0f;
  } else if (state == 1) {
    leds[led].r = 1.0f;
    leds[led].g = 0.5f;
    leds[led].b = 0.0f;
  } else {
    leds[led].r = 1.0f;
    leds[led].g = 0.0f;
    leds[led].b = 0.0f;
  }
}

void setSvnActivity(struct led_t leds[], int led) {
  // TODO: color based on number of added+removed lines in last 10 minutes
}

void setClock(struct led_t leds[], int ledHour, int ledMinute1, int ledMinute2) {
  time_t now = time(NULL);
  struct tm *now_tm = localtime(&now);
  int hour = now_tm->tm_hour;
  int workhour = hour % 8; // workhour is 0-7 from 8:00 to 16:00
  leds[ledHour].r = (workhour & 4) > 0 ? 1.0f : 0.0f;
  leds[ledHour].g = (workhour & 2) > 0 ? 1.0f : 0.0f;
  leds[ledHour].b = (workhour & 1) > 0 ? 1.0f : 0.0f;

  int min = now_tm->tm_min;

  leds[ledMinute1].r = (min & 32) > 0 ? 1.0f : 0.0f;
  leds[ledMinute1].g = (min & 16) > 0 ? 1.0f : 0.0f;
  leds[ledMinute1].b = (min & 8) > 0 ? 1.0f : 0.0f;
  leds[ledMinute2].r = (min & 4) > 0 ? 1.0f : 0.0f;
  leds[ledMinute2].g = (min & 2) > 0 ? 1.0f : 0.0f;
  leds[ledMinute2].b = (min & 1) > 0 ? 1.0f : 0.0f;
}

void printled(struct led_t led) {
  printf("(%.2f, %.2f, %.2f) (%d, %d) (%d, %d)\n",
         led.r, led.g, led.b, led.flickerSpeed, led.flickerI, 
         led.dimmingSpeed, led.dimmingI);
}

/* void setled(int fd, int led, unsigned char r, unsigned char g, unsigned char b) { */
/*   // data bits: l5 l4 l3 l2  l1 l0 update-leds 1 */

/*   unsigned char control = (unsigned char) (led << 2) + 0x01; */
/*   unsigned char packet[4]; */
/*   packet[0] = control; */
/*   packet[1] = r; */
/*   packet[2] = g; */
/*   packet[3] = b; */

/*   if (write(fd, packet, 4) < 0) { */
/*     perror("argh2"); */
/*     exit(2); */
/*   } */
/*   tcdrain(fd); */
/*   usleep(3000); */
/* } */

// works with usbserialneopixel_simple
void setleds_kb(int fd, unsigned char leds[]) {
  for (int i = 0; i < 16; i++) {
    if (write(fd, leds + i*3, 3) < 0) {
      perror("argh2");
      exit(2);
    }
    tcdrain(fd);
    usleep(8000);
  }
}

void setleds(int fd, struct led_t leds[]) {
  unsigned char leddata[] = {0x00, 0x00, 0x00,
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

  for (int i = 0; i < 16; i++) {
    struct led_t *led = & leds[i];
    float dimmer = 1.0f;
    if (led->dimmingSpeed > 0) {
      led->dimmingI++;
      if (led->dimmingI > led->dimmingSpeed * 2) {
        led->dimmingI = 0;
      }
      if (led->dimmingI < led->dimmingSpeed) {
        dimmer = (float) led->dimmingI / led->dimmingSpeed;
      } else {
        dimmer = (float) (led->dimmingSpeed * 2 - led->dimmingI) / led->dimmingSpeed;
      }
      if (dimmer < 0.1f)
        dimmer = 0.1f;
    }


    unsigned char r = (unsigned char) (led->r * dimmer * 60.0);
    unsigned char g = (unsigned char) (led->g * dimmer * 60.0);
    unsigned char b = (unsigned char) (led->b * dimmer * 60.0);

    if (led->flickerSpeed > 0) {
      led->flickerI++;
      if (led->flickerI > led->flickerSpeed + 1) {
        led->flickerI = 0;
      }
      if (led->flickerI >= led->flickerSpeed) {
        r = g = b = 0x00;
      }
    }
    /* setled(fd, i, r, g, b); */
    leddata[i * 3] = r;
    leddata[i * 3 + 1] = g;
    leddata[i * 3 + 2] = b;
  }

  setleds_kb(fd, leddata);
}

int main(int argc, char *argv[]) {
  struct termios term_opt;

  getGradientTester();

  /* Setup the serial port */
  /* int fd = open("/dev/ttyUSB0", O_RDWR); */
  int fd = open("/dev/ttyACM0", O_RDWR);
  if(fd < 0) {
    perror("argh");
    exit(1);
  }
  tcgetattr(fd, &term_opt);
  cfmakeraw(&term_opt);
  /* cfsetospeed(&term_opt, B38400); // B57600); */
  cfsetospeed(&term_opt, B19200); // B57600);
  tcsetattr(fd, TCSAFLUSH, &term_opt);
  struct led_t leds[16];

  for (int i = 0; i < 16; i++) {
    leds[i].r = 0.0f;
    leds[i].g = 0.0f;
    leds[i].b = 0.0f;
    leds[i].flickerSpeed = leds[i].flickerI = 0;
    leds[i].dimmingSpeed = leds[i].dimmingI = 0;
  }

  setleds(fd, leds); // turn off everything
  usleep(50000);

  /* setWeather(leds, 0); */
  /* return 0; */

  long time= 0;
  while (1) {
    if (time % 20 == 0) {
      setJenkinsState(leds, 0, "curl -s http://jenkins1/jenkins1/view/Light%20show/api/json");
      setJenkinsState(leds, 1, "curl -s http://jenkins2/jenkins2/view/1.Integration/api/json");
    }

    if (time % (60 * 10) == 0) {
      setWeather(leds, 13);
    }

    setCpu(leds, 7);
    setMem(leds, 8);
    setSwap(leds, 9);

    if (time % 5 == 0) {
      setClock(leds, 10, 11, 12); 
    }

    setleds(fd, leds);
      
    sleep(1);
    time++;
  }

  /* for (int i = 0; i < 10; i++) { */
  while (1) {
    printled(leds[0]);

    setleds(fd, leds);
    usleep(30000);
  }

}
