

#ifndef __MAIN_H__
#define __MAIN_H__

void init(void);
void keyb(unsigned char key, int x, int y);
void disp(void);
void reshape(int w, int h);
void passive_motion(int x, int y);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void refresh_display(void);

#define NUM_POINTS 8 /* Ensure multiple of 3 to avoid trouble with bezier */

/* functions to draw stuff on screen (draw.c) */

void draw_points(void);
void draw_linear(void);
void draw_catmull_rom(void);
void draw_bezier(void);

#endif

