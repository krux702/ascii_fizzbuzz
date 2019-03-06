#include "aalib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "aatest.h"

#define DWIDTH aa_imgwidth(context)
#define DHEIGHT aa_imgheight(context)
#define PI 3.1415926535897932384626433832795
#define SCROLLER_HEIGHT 20

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
    int ticks = 0;

    int i, y;
    char s[256];
    float p_color_percent = 0;
    float p_color_goal = 1000;

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

    int n = 0;

    // init scroller buffer
    int scroller[aa_imgwidth(context)+1][SCROLLER_HEIGHT];
    for(int x = 0; x < aa_imgwidth(context)+1; x++)
    {
      for(int y = 0; y < SCROLLER_HEIGHT; y++)
      {
        scroller[x][y] = 0;
      }
    }

    // sample scroller data
    for(int x = 0; x < aa_imgwidth(context); x++)
    {
      n++;
      if(n > 7)
      {
        n = 0;
      }
      scroller[x][0] = 1;
      scroller[x][1] = 1;
      scroller[x][2] = 128;
      scroller[x][3] = 255;
      scroller[x][4] = 128;
      scroller[x][5] = 1;
      scroller[x][6] = 1;

      scroller[x][7] = 0;
      scroller[x][8] = 0;
      scroller[x][9] = 0;
      scroller[x][10] = 0;
      scroller[x][11] = 0;
      scroller[x][12] = 0;
      scroller[x][13] = 0;

      scroller[x][7+n] = 255;

      scroller[x][14] = 1;
      scroller[x][15] = 1;
      scroller[x][16] = 128;
      scroller[x][17] = 255;
      scroller[x][18] = 128;
      scroller[x][19] = 1;
      scroller[x][20] = 1;
    }


    aa_hidecursor(context);

    while(1)
    {
      ticks++;

      if(p_color_percent < p_color_goal)
      {
        p_color_percent++;
      }
      else if(p_color_percent > p_color_goal)
      {
        p_color_percent--;
      }

      m_time = getMicrotime();


      // plasma background effect
      for(int y = 0; y < aa_imgheight(context); y++)
      {
        for(int x = 0; x < aa_imgwidth(context); x++)
        {
          params->bright = -1;
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
          color = (int)((float)color * (p_color_percent / 1000));

          aa_putpixel(context, x, y, color);

        }
      }

      // update scroll buffer once every three frames
      if(ticks % 3 == 0)
      {


        for(int x = 0; x < aa_imgwidth(context); x++)
        {
          for(int y = 0; y <= 20; y++)
          {
            scroller[x][y] = scroller[x+1][y];
          }
        }
      }


      // sin wave
      for(int x = 0; x < aa_imgwidth(context); x++)
      {
        int w = (int)( aa_imgheight(context)/2 - 10
                       + (7 * sin((x + m_time) / 33.0)) 
                       + (4 * sin((x - (m_time*1.2)) / 72.0)) );
        for(int y = 0; y < 20; y++)
        {
          if(scroller[x][y])
          {
            aa_putpixel(context, x, w + y, scroller[x][y]);
          }
        }
      }

      aa_render(context, params, 0, 0, aa_scrwidth(context), aa_scrheight(context));
//      aa_fastrender(context, 0, 0, aa_scrwidth(context), aa_scrheight(context));
//      aa_printf(context, 0, 0, AA_SPECIAL, "percent %f",p_color_percent);
      aa_flush(context);
    }

    aa_close(context);
    return 0;
}

