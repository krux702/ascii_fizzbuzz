#include "aalib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "aatest.h"

#define DWIDTH aa_imgwidth(context)
#define DHEIGHT aa_imgheight(context)
#define PI 3.1415926535897932384626433832795

aa_context *context;
aa_renderparams *params;

float v;
long m_time;

long getMicrotime()
{
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec) / 100000;
}

int main(int argc, char **argv)
{
    params = aa_getrenderparams();

    int i, y;
    char s[256];

    if (!aa_parseoptions(NULL, NULL, &argc, argv) || argc != 1) {
	printf("%s\n", aa_help);
	exit(1);
    }
    context = aa_autoinit(&aa_defparams);
    if (context == NULL) {
	printf("Can not intialize aalib\n");
	exit(2);
    }
    if (!aa_autoinitkbd(context, 0)) {
	printf("Can not intialize keyboard\n");
	aa_close(context);
	exit(3);
    }

    aa_hidecursor(context);

    for (int i = 0 ; i < 10000; i++)
    {
      m_time = getMicrotime();


      // plasma background effect
      for(int y = 0; y < aa_imgheight(context); y++)
      {
        for(int x = 0; x < aa_imgwidth(context); x++)
        {
          params->bright = 0;
          params->gamma = 2;
          params->dither = AA_NONE;
          params->randomval = 0;
          // params->dither = AA_FLOYD_S;
          // params->dither = AA_ERRORDISTRIB;
          // https://lodev.org/cgtutor/plasma.html

          /*
            int color = (int)((128 + (128 * sin((x + m_time) / 8.0)) + 128 + (128 * sin((y + m_time) / 8.0)))/2);
          */

          int color = (int)(128.0 + (128.0 * sin((x + m_time) / 16.0))
            + 128.0 + (128.0 * sin((y + m_time)/ 8.0)) + 128.0
            + (128.0 * sin(((x + m_time) + (y + m_time)) / 16.0)) + 128.0
            + 128.0 + (128.0 * sin(sqrt((double)(x * x + y * y)) / 8.0))
            / 4);

          aa_putpixel(context, x, y, color);

        }
      }

      aa_render(context, params, 0, 0, aa_scrwidth(context), aa_scrheight(context));
//      aa_fastrender(context, 0, 0, aa_scrwidth(context), aa_scrheight(context));
      aa_flush(context);
    }

    aa_close(context);
    return 0;
}

