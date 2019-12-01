#ifndef LABEL_HPP_
#define LABEL_HPP_

#include <stdint.h>
#include "fonts.h"
#include "component.hpp"

typedef enum textStyle{
  textStyle_blackOnWhite,
  textStyle_whiteOnBlack
}textStyle;

const int color_white=0;
const int color_black=1;

class Label : public Component{

  public:
    Label(int rotate, uint16_t rows, uint16_t charNumber, sFONT* font, textStyle _textStyle);
    ~Label();

    int setText(const char* text);
    void setTextStyle(textStyle _textStyle);
    void setRotate(int rotate);

    const char* getText();
  private:

    void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
    void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored);
    int backgroundColor;
    int textColor;

    sFONT* font;
    const char* text;

};

#endif /*LABEL_HPP_*/
