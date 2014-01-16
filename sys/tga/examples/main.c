

#include "ucg.h"
#include <stdlib.h>
#include <stdio.h>



ucg_t ucg;



unsigned char bitmap[2] = { 0x0f0, 0x0f0 };

ucg_int_t rule_offset = 4;
ucg_int_t ox = 64;

void prepare_picture(ucg_t *ucg)
{
  tga_init(128*2,64);
  ucg_Init(ucg, &ucg_dev_tga, ucg_ext_none, (ucg_com_fnptr)0);
  ucg_SetMaxClipRange(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 0);
  ucg_DrawBox(ucg, ox, 0, 128, 64);
}

void save_picture(ucg_t *ucg, const char *fname)
{
  tga_save("ucg_pic.tga");

  {
    char cmd[256];
    sprintf(cmd, "convert ucg_pic.tga %s.png", fname);
    system(cmd);
  }
}


void prepare_measure(ucg_t *ucg)
{
  ucg_SetMaxClipRange(ucg);
  ucg_SetColor(ucg, 0, 255, 160, 63);
  ucg_SetFont(ucg, ucg_font_6x12);
}

void pos(ucg_t *ucg, ucg_int_t x, ucg_int_t y, int is_right)
{
  char s[20];
  ucg_int_t w;
  prepare_measure(ucg);
  sprintf(s, "(%d,%d)", x-ox, y);
  w = ucg_GetStrWidth(ucg, s);
  ucg_DrawPixel(ucg, x, y);
  ucg_SetFontPosCenter(ucg);
  if (  is_right )
  {
    ucg_DrawString(ucg, x+1, y, 0, s);
  }
  else
  {
    ucg_DrawString(ucg, x+1-w, y, 0, s);
  }
}


void vrule(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len, int is_right)
{
  char s[10];
  ucg_int_t o = rule_offset;
  ucg_int_t w;
  
  prepare_measure(ucg);
  sprintf(s, "%d", len);
  ucg_SetFontPosBaseline(ucg);
  w = ucg_GetStrWidth(ucg, s);
  if ( is_right )
  {
    ucg_DrawVLine(ucg, x+o, y, len);
    ucg_DrawHLine(ucg, x-1+o, y, 3);
    ucg_DrawHLine(ucg, x-1+o, y+len-1, 3);
    ucg_SetFontPosBottom(ucg);
    ucg_DrawString(ucg, x+o-1, y + len/2 - w/2 + 1, 1, s);
  }
  else
  {
    ucg_DrawVLine(ucg, x-o, y, len);
    ucg_DrawHLine(ucg, x-1-o, y, 3);
    ucg_DrawHLine(ucg, x-1-o, y+len-1, 3);
    ucg_SetFontPosBottom(ucg);
    ucg_DrawString(ucg, x-o+1, y + len - ( len/2 - w/2 + 1), 3, s);
  }
}


/*
  pos = 1 above
  pos = 0 
  pos = -1 below
*/
void hrule(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len, int is_below)
{
  char s[10];
  ucg_int_t o = rule_offset;
  ucg_int_t w;

  if ( is_below )
  {
    ucg_SetFontPosTop(ucg);
    o  = -o;
  }
  else
  {
    ucg_SetFontPosBottom(ucg);
  }
  
  prepare_measure(ucg);
  ucg_DrawHLine(ucg, x, y-o, len);
  ucg_DrawVLine(ucg, x, y-1-o, 3);
  ucg_DrawVLine(ucg, x+len-1, y-1-o, 3);
  sprintf(s, "%d", len);
  w = ucg_GetStrWidth(ucg, s);
  ucg_DrawString(ucg, x + len/2 - w/2, y-o+1, 0, s);
}


void draw_pixel(ucg_t *ucg)
{
  prepare_picture(ucg);
  pos(ucg, ox+50, 20,0);
  ucg_SetColor(ucg, 0, 255, 255, 255);
  ucg_DrawPixel(ucg, ox+50, 20);
  save_picture(ucg, "draw_pixel");  
}

void draw_hline(ucg_t *ucg)
{
  prepare_picture(ucg);
  hrule(ucg, ox+50, 40, 45, 0);
  ucg_SetColor(ucg, 0, 255, 255, 255);
  
  ucg_DrawHLine(ucg, ox+50, 40, 45);
  pos(ucg, ox+50, 40,0);
  save_picture(ucg, "draw_hline");  
}

void draw_vline(ucg_t *ucg)
{
  prepare_picture(ucg);
  vrule(ucg, ox+50, 20, 25, 1);
  ucg_SetColor(ucg, 0, 255, 255, 255);
  ucg_DrawVLine(ucg, ox+50, 20, 25);
  pos(ucg, ox+50, 20,0);
  save_picture(ucg, "draw_vline");  
}

void set_clip_range(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFontPosBaseline(ucg);
  ucg_SetFont(ucg, ucg_font_ncenB18);
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white A */
  ucg_DrawGlyph(ucg, 50+ox, 40, 0, 'A');
  ucg_SetClipRange(ucg, 57+ox, 20, 30, 15);	/* restrict area */
  ucg_SetColor(ucg, 0, 0, 0, 127);
  ucg_DrawBox(ucg, ox, 0, 128, 64);		/* fill the restricted area with dark blue */
  ucg_SetColor(ucg, 0, 0, 0, 255);
  ucg_DrawGlyph(ucg, 50+ox, 40, 0, 'A');	/* draw light blue A */
  
  hrule(ucg, ox+57, 20, 30, 0);
  vrule(ucg, ox+57+30, 20, 15, 1);
  pos(ucg, ox+50, 40, 0);
  save_picture(ucg, "set_clip_range");  
}

void draw_text_baseline(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFont(ucg, ucg_font_ncenB18);
  ucg_SetFontPosBaseline(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 255);		/* draw blue "baseline" */
  ucg_DrawHLine(ucg, 45+ox, 30, ucg_GetStrWidth(ucg, "Ucg"));
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white "Ucg" */
  ucg_DrawString(ucg, 45+ox, 30, 0, "Ucg");
  pos(ucg, 45+ox, 30, 0);
  
  save_picture(ucg, "draw_text_baseline");  
}

void draw_text_bottom(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFont(ucg, ucg_font_ncenB18);
  ucg_SetFontPosBottom(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 255);		/* draw blue "baseline" */
  ucg_DrawHLine(ucg, 45+ox, 30, ucg_GetStrWidth(ucg, "Ucg"));
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white "Ucg" */
  ucg_DrawString(ucg, 45+ox, 30, 0, "Ucg");
  pos(ucg, 45+ox, 30, 0);
  
  save_picture(ucg, "draw_text_bottom");  
}

void draw_text_top(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFont(ucg, ucg_font_ncenB18);
  ucg_SetFontPosTop(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 255);		/* draw blue "baseline" */
  ucg_DrawHLine(ucg, 45+ox, 30, ucg_GetStrWidth(ucg, "Ucg"));
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white "Ucg" */
  ucg_DrawString(ucg, 45+ox, 30, 0, "Ucg");
  pos(ucg, 45+ox, 30, 0);
  
  save_picture(ucg, "draw_text_top");  
}

void draw_text_center(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFont(ucg, ucg_font_ncenB18);
  ucg_SetFontPosCenter(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 255);		/* draw blue "baseline" */
  ucg_DrawHLine(ucg, 45+ox, 30, ucg_GetStrWidth(ucg, "Ucg"));
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white "Ucg" */
  ucg_DrawString(ucg, 45+ox, 30, 0, "Ucg");
  pos(ucg, 45+ox, 30, 0);
  
  save_picture(ucg, "draw_text_center");  
}

void draw_text_ascent_descent(ucg_t *ucg)
{
  prepare_picture(ucg);
  
  ucg_SetFont(ucg, ucg_font_ncenB24);
  ucg_SetFontPosBaseline(ucg);
  ucg_SetColor(ucg, 0, 0, 0, 255);		/* draw blue "baseline" */
  ucg_DrawHLine(ucg, 45+ox, 30, ucg_GetStrWidth(ucg, "Ucg"));
  ucg_SetColor(ucg, 0, 255, 255, 255);		/* draw white "Ucg" */
  ucg_DrawString(ucg, 45+ox, 30, 0, "Ucg");
  pos(ucg, 45+ox, 30, 0);
  
  save_picture(ucg, "draw_text_ascent_descent");  
}


int main(void)
{
  
  tga_init(128,64);
  ucg_Init(&ucg, &ucg_dev_tga, ucg_ext_none, (ucg_com_fnptr)0);
  
  ucg_SetColor(&ucg, 0, 0, 0, 0);
  ucg_DrawBox(&ucg, 0, 0, 128, 64);
  
  
  
  ucg_SetFont(&ucg, ucg_font_ncenB18);
  
  ucg_SetColor(&ucg, 0, 0, 0, 255);
  ucg_DrawPixel(&ucg, 70,20);
  ucg_SetColor(&ucg, 0, 255, 0, 0);
 
  //ucg_SetFontPosBottom(&ucg);
  
  ucg_DrawGlyph(&ucg, 70, 20, 0, 'A');
  ucg_DrawGlyph(&ucg, 70, 20, 1, 'A');
  ucg_DrawGlyph(&ucg, 70, 20, 2, 'A');
  ucg_DrawGlyph(&ucg, 70, 20, 3, 'A');

  ucg_SetColor(&ucg, 0, 255, 255, 255);
  
  vrule(&ucg, 30, 0, 20, 1);
  vrule(&ucg, 30, 0, 20, 0);
  
  //pos(&ucg, 70, 20, 0);
  
  //hrule(&ucg, 0, 20, 70, 1);
  hrule(&ucg, 0, 20, 70, 0);
  
  
  ucg_SetColor(&ucg, 0, 255, 0, 0);
  ucg_SetColor(&ucg, 1, 0, 255, 0);
  ucg_SetColor(&ucg, 2, 255, 0, 255);
  ucg_SetColor(&ucg, 3, 0, 255, 255);

  ucg_DrawGradientLine(&ucg, 10, 40, 100, 0);

  ucg_DrawGradientBox(&ucg, 10, 43, 100, 20);


  tga_save("test.tga");
  
  draw_pixel(&ucg);
  draw_hline(&ucg);
  draw_vline(&ucg);
  set_clip_range(&ucg);
  draw_text_baseline(&ucg);
  draw_text_bottom(&ucg);
  draw_text_top(&ucg);
  draw_text_center(&ucg);
  draw_text_ascent_descent(&ucg);
  return 0;
}
