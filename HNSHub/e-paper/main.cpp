
#include <stdio.h>
#include <stdlib.h>
#include "epd2in7.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "label.hpp"
#include "frame.hpp"
#include "component.hpp"
#define COLORED      1
#define UNCOLORED    0

int main(){
  Epd epd;
  if (epd.Init() != 0) {
    printf("e-Paper init failed\n");
    return -1;
  }
  epd.ClearFrame();
/*
  unsigned char* frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);

  Frame window1 = Frame(&epd, frame_buffer, epd.width, epd.height);
  window1.Clear(UNCOLORED);
  window1.SetRotate(ROTATE_270);
  window1.DrawStringAt(0, 0, "Window1", &Font16, COLORED);
*/
  Frame window2 = Frame(&epd);
  window2.Clear(UNCOLORED);
  window2.SetRotate(ROTATE_270);
  window2.DrawStringAt(0, 0, "Window2", &Font16, COLORED);

//  epd.DisplayFrame(window1.GetImage());
//  epd.DisplayFrame(window2.GetImage());

  Label lab = Label(ROTATE_0, 1,  4, &Font20, textStyle_blackOnWhite);
  lab.setText("CIAO");
  lab.setRotate(ROTATE_270);

  window2.add(&lab, 0, 0);

  Label lab2 = Label(ROTATE_0, 1,  1, &Font20, textStyle_blackOnWhite);
  lab2.setRotate(ROTATE_270);
  lab2.setText("1");
  int x,y;
  lab.getEndCoordinates(&x,&y);
  window2.add(&lab2, x, y);

  //lab.setCoordinates(0,0);
  //window2.refresh(&lab);
window2.refresh();
char*
for(int i=0; i<10; i++){
  lab2.setText("1");
  window2.refresh(&lab2);
}

  //Component com = Component(40,70);

}
/*
int main(){
  Epd epd;
  if (epd.Init() != 0) {
    printf("e-Paper init failed\n");
    return -1;
  }
  epd.ClearFrame();


  Label lab = Label(&epd, ROTATE_270, 1,  4, &Font20, textStyle_blackOnWhite);
  //lab.setRotate(ROTATE_270);
  lab.setText("CIAO");
  lab.setCoordinates(0,0);
  lab.refresh();

  Label lab1 = Label(&epd, ROTATE_270, 1,  4, &Font16, textStyle_blackOnWhite);
  //lab.setRotate(ROTATE_270);
  lab1.setText("432p");
  lab1.setCoordinates(50,50);
  lab1.refresh();

  Label lab2 = Label(&epd, ROTATE_270, 1,  5, &Font12, textStyle_blackOnWhite);
  //lab.setRotate(ROTATE_270);
  lab2.setText("15:20");
  lab2.setCoordinates(0,80);
  lab2.refresh();

  return 0;

  int width = Font20.Height;//Font20.Width * 4;
  int height = Font20.Width * 4;//Font20.Height;

  //int width = Font20.Width * 4;
  //int height = Font20.Height;

  unsigned char* labelMem = (unsigned char*) malloc(width / 8 * height);
  Paint label = Paint(labelMem, width, height);
  label.SetRotate(ROTATE_270);

  label.Clear(UNCOLORED);
  label.DrawStringAt(0,0, "Aplq", &Font20, COLORED);

  int cx = 0;
  int cy = 0;

  int x = 0+cx;
  int y = 264-label.GetHeight()-cy;

  int w=label.GetWidth();
  int h=label.GetHeight();

  epd.TransmitPartialData(label.GetImage(), x, y, w,h);
  epd.RefreshPartial(x,y,w,h);

  label.Clear(UNCOLORED);
  label.DrawStringAt(0,0, "CIAO", &Font20, COLORED);
  epd.TransmitPartialData(label.GetImage(), x, y, w,h);
  epd.RefreshPartial(x,y,w,h);

}
*/
