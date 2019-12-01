#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3
// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     0

class Component {
  public:
    Component(int width, int height);
    ~Component();

    void Clear(int colored);

    int  GetWidth(void);
    void SetWidth(int width);

    int  GetHeight(void);
    void SetHeight(int height);

    int  GetRotate(void);
    void SetRotate(int rotate);

    unsigned char* GetImage(void);

    void DrawAbsolutePixel(int x, int y, int colored);
    void DrawPixel(int x, int y, int colored);

    void getEndCoordinates(int* x, int*y);
    int setCoordinates(int x, int y);

    int x;
    int y;

    int relativeX;
    int relativeY;
  protected:
    Component();

  //  int x;
  //  int y;


    unsigned char* image;
    int width;
    int height;
    int rotate;
};


#endif/*COMPONENT_HPP_*/
