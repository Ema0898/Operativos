#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>

enum color_class{
    RED,GREEN,BLUE
} ;

int main(int argc,char **argv)
{

  MagickBooleanType
    status;

  MagickPixelPacket
    pixel;

  MagickWand
    *contrast_wand,
    *image_wand;

  PixelIterator
    *iterator;

  PixelWand
    **contrast_pixels,
    **pixels;

  register ssize_t
    x;

  size_t
    width;

  ssize_t
    y;

  if (argc != 2)
    {
      (void) fprintf(stdout,"Error: %s Es requerida una imagen\n",argv[0]);
      exit(0);
    }
  /*
    Read an image.
  */
  MagickWandGenesis();
  image_wand=NewMagickWand();
  status=MagickReadImage(image_wand,argv[1]);
  if (status == MagickFalse)
    //ThrowWandException(image_wand);
    printf("Errror\n");
  contrast_wand=CloneMagickWand(image_wand);

    /*
    Classify in RGB.
  */

    Image *im =GetImageFromMagickWand(image_wand);
    if(im==NULL)
        return -1;

    PixelWand **row;
    size_t row_width;

    PixelIterator *iter= NewPixelIterator(image_wand);
    MagickPixelPacket pixelPack;

    double red=0,green=0,blue=0;
    for (size_t i=0; i< im->rows; ++i){

        row=PixelGetNextIteratorRow(iter, &row_width);

        for (size_t j=0; j< row_width; ++j ){
            PixelGetMagickColor(row[j],&pixelPack);
            red +=pixelPack.red;
            green +=pixelPack.green;
            blue +=pixelPack.blue;
        }
    }

    iter=DestroyPixelIterator(iter);

    if( red >= green && red >= blue)
        //return RED;
        printf("Red\n");
    if(green >= red && green >= blue)
        //return GREEN;
        printf("Green\n");
    if(blue >= red && blue >= green)
        //return BLUE;
        printf("Blue\n");

}    
