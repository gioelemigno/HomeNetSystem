#include "component.hpp"

#include <stdlib.h>
#include <stdio.h>

Component::Component(int width, int height){
  this->rotate = ROTATE_0;

  /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
  width=width % 8 ? width + 8 - (width % 8) : width;
  height=height % 8 ? height + 8 - (height % 8) : height;

  this->width = width;
  this->height = height;

  this->image = (unsigned char*) malloc(width / 8 * height);

  this->x=0;
  this->y=0;
}

int Component::setCoordinates(int x, int y){
  this->x = x;
  this->y = y;
  return 0;
}

void Component::getEndCoordinates(int* x, int*y){
  switch (this->rotate) {
    case ROTATE_0:
    printf("rot 0 this x=%d, this y=%d\n", this->relativeX , this->relativeY);
      *x = this->relativeX + this->width;
      *y = this->relativeY;// + this->height;
      printf("x=%d, y=%d\n", *x, *y);
    break;

    case ROTATE_90:
    printf("rot 90 this x=%d, this y=%d\n", this->relativeX , this->relativeY);
      *x = this->relativeX;
      *y = this->relativeY + this->height;
      printf("x=%d, y=%d\n", *x, *y);
    break;

    case ROTATE_180:
    printf("rot 180 this x=%d, this y=%d\n", this->relativeX , this->relativeY);
      *x = this->relativeX + this->width;
      *y = this->relativeY;// + this->height;
      printf("x=%d, y=%d\n", *x, *y);
    break;

    case ROTATE_270:
    printf("rot 270 this x=%d, this y=%d\n", this->relativeX , this->relativeY);
      *x = this->relativeX;
      *y = this->relativeY + this->height;
      printf("x=%d, y=%d\n", *x, *y);
    break;
  }
}

Component::Component() {}

Component::~Component() {
}


/**
 *  @brief: clear the image
 */
void Component::Clear(int colored) {
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            DrawAbsolutePixel(x, y, colored);
        }
    }
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void Component::DrawAbsolutePixel(int x, int y, int colored) {

    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (colored) {
            image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
        } else {
            image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
        } else {
            image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
        }
    }
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* Component::GetImage(void) {
    return this->image;
}

int Component::GetWidth(void) {
    return this->width;
}

void Component::SetWidth(int width) {
    this->width = width % 8 ? width + 8 - (width % 8) : width;
}

int Component::GetHeight(void) {
    return this->height;
}

void Component::SetHeight(int height) {
    this->height = height;
}

int Component::GetRotate(void) {
    return this->rotate;
}

void Component::SetRotate(int rotate){
    this->rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Component::DrawPixel(int x, int y, int colored) {
    int point_temp;
    if (this->rotate == ROTATE_0) {
        if(x < 0 || x >= this->width || y < 0 || y >= this->height) {
            return;
        }
        DrawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_90) {
        if(x < 0 || x >= this->height || y < 0 || y >= this->width) {
          return;
        }
        point_temp = x;
        x = this->width - y;
        y = point_temp;
        DrawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_180) {
        if(x < 0 || x >= this->width || y < 0 || y >= this->height) {
          return;
        }
        x = this->width - x;
        y = this->height - y;
        DrawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_270) {
        if(x < 0 || x >= this->height || y < 0 || y >= this->width) {
          return;
        }
        point_temp = x;
        x = y;
        y = this->height - point_temp;
        DrawAbsolutePixel(x, y, colored);
    }
}




/* END OF FILE */
