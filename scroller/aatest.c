#include "aalib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "aatest.h"

#define DWIDTH aa_imgwidth(context)
#define DHEIGHT aa_imgheight(context)
#define PI 3.1415926535897932384626433832795
#define SCROLLER_HEIGHT 22
#define FONT_HEIGHT 21
#define FONT_WIDTH 23
#define FONT_OFFSET 460

aa_context *context;
aa_renderparams *params;

float v;
long m_time;
unsigned char debug_data;
unsigned char message_scroll[] = "0123456789 abcdefghijklmnopqrstuvwxyz !@#%^&*()-=+,.?'\"    ";
int messsage_scroll_index;

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

  float p_color_percent = 0;
  float p_color_goal = 1000;

  if (!aa_parseoptions(NULL, NULL, &argc, argv) || argc != 1)
  {
    printf("%s\n", aa_help);
    exit(1);
  }
  context = aa_autoinit(&aa_defparams);
  if (context == NULL)
  {
    printf("Can not intialize aalib\n");
    exit(2);
  }
  if (!aa_autoinitkbd(context, 0))
  {
    printf("Can not intialize keyboard\n");
    aa_close(context);
    exit(3);
  }

  aa_hidecursor(context);

  // init scroller buffer
  int scroller[aa_imgwidth(context)+1][SCROLLER_HEIGHT+1];
  for(int x = 0; x < aa_imgwidth(context)+1; x++)
  {
    for(int y = 0; y <= SCROLLER_HEIGHT + 1; y++)
    {
      scroller[x][y] = 0;
    }
  }

  int f_column = 0;
  int message_scroll_index = 0;
  char cur_char = message_scroll[message_scroll_index];

  // main loop
  while(1)
  {
    ticks++;
    m_time = getMicrotime();

    if(p_color_percent < p_color_goal)
    {
      p_color_percent++;
    }
    else if(p_color_percent > p_color_goal)
    {
      p_color_percent--;
    }

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
      if(f_column > FONT_WIDTH)
      {
        f_column = 0;
        message_scroll_index++;
        if(message_scroll_index > sizeof(message_scroll))
        {
          message_scroll_index = 0;
        }
        cur_char = message_scroll[message_scroll_index];
      }

      for(int y = 0; y <= FONT_HEIGHT; y++)
      {
        scroller[aa_imgwidth(context)][y] = fontdata[f_column + (y * FONT_WIDTH) + (char_offset[cur_char] * FONT_OFFSET)];
      }

      f_column++;

      for(int x = 0; x < aa_imgwidth(context); x++)
      {
        for(int y = 0; y <= SCROLLER_HEIGHT; y++)
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
      for(int y = 0; y <= SCROLLER_HEIGHT; y++)
      {
        if(scroller[x][y])
        {
          aa_putpixel(context, x, w + y, scroller[x][y]);
        }
      }
    }

    aa_render(context, params, 0, 0, aa_scrwidth(context), aa_scrheight(context));
    // aa_fastrender(context, 0, 0, aa_scrwidth(context), aa_scrheight(context));
    // aa_printf(context, 0, 0, AA_SPECIAL, "char %d  ",debug_data);
    aa_flush(context);
  }

  aa_close(context);
  return 0;
}
