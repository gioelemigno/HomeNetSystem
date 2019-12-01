#include "label.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "fonts.h"
#include "component.hpp"

const char* Label::getText(){
  return this->text;
}

Label::Label(int rotate, uint16_t rows, uint16_t charNumber, sFONT* font, textStyle _textStyle):Component(){
  this->text="";
  this->font=font;

  if(_textStyle==textStyle_whiteOnBlack){
    this->textColor=color_white;
    this->backgroundColor=color_black;
  }
  else{ //default
    this->textColor=color_black;
    this->backgroundColor=color_white;
  }

  if(rotate==ROTATE_0 || rotate == ROTATE_180){
    this->height=rows*font->Height;
    this->width=charNumber*font->Width;
  }
  else{
    this->height=charNumber*font->Width;
    this->width=rows*font->Height;
  }
  printf("BEFORE h:%d w:%d\n", this->height, this->width);

  //must be multiple of 8 (1byte = 8 pixel)
  //From epdpaint:  width=  width % 8 ? width + 8 - (width % 8) : width;
  this->height=this->height % 8 ? this->height + 8 - (this->height % 8) : this->height;
  this->width=  this->width % 8 ? this->width + 8 - (this->width % 8) : this->width;

  printf("AFTER h:%d w:%d\n", this->height, this->width);
  this->image=(unsigned char*)malloc(this->width / 8 * this->height);

  this->SetRotate(rotate);
}

Label::~Label(){

}

void Label::setRotate(int rotate){
  if((this->GetRotate()==ROTATE_0 || this->GetRotate() == ROTATE_180) && (rotate==ROTATE_90 || rotate == ROTATE_270)){
    int tmp = this->height;
    this->height=this->width;
    this->width=tmp;
  }
  this->SetRotate(rotate);

  this->setText(this->text);
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Label::DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored) {
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];
  //  printf("DRAW CHAR : x=%d y=%d\n", x, y);
    for (j = 0; j < font->Height; j++) {
        for (i = 0; i < font->Width; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                DrawPixel(x + i, y + j, colored);

            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void Label::DrawStringAt(int x, int y, const char* text, sFONT* font, int colored) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;

    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        DrawCharAt(refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}

int Label::setText(const char* text){
  this->Clear(this->backgroundColor);

  this->DrawStringAt(0, 0, text, this->font, this->textColor);

//    !!! maybe cause a seg fault ! will change const char to STring
  this->text = text;
  return 0;
}



void  Label::setTextStyle(textStyle _textStyle){
  if(_textStyle==textStyle_whiteOnBlack){
    this->textColor=color_white;
    this->backgroundColor=color_black;
  }
  else{ //default
    this->textColor=color_black;
    this->backgroundColor=color_white;
  }
}
