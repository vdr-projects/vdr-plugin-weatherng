#include <string>
#include <vector>
#include <Magick++.h>
#include <vdr/tools.h>
#include <vdr/osd.h>
using namespace Magick;

cBitmap* LoadMagick(const char *Filename, int height, int width, int colors, bool Quiet) {
	std::vector<Image> images;
	cBitmap *bmp = NULL;
	try {
		int w, h;
		std::vector<Image>::iterator it;
		readImages(&images, Filename);
		if (images.size() == 0) {
			printf("ERROR: text2skin: Couldn't load %s\n:w", Filename);
			return NULL;
		}
//		mDelay = images[0].animationDelay() * 10;
		for (it = images.begin(); it != images.end(); ++it) {
			if (colors != 0){
				(*it).opacity(OpaqueOpacity);
				(*it).backgroundColor( Color ( 0,0,0,0) );
				(*it).quantizeColorSpace( RGBColorspace );
				(*it).quantizeColors( colors );
				(*it).quantize();
			}
			 if (height != 0 || width != 0)
				(*it).sample(Geometry(width,height));
			w = (*it).columns();
			h = (*it).rows();
			if ((*it).depth() > 8) {
				printf("ERROR: text2skin: More than 8bpp images are not supported\n");
				return NULL;
			}
			bmp = new cBitmap(w, h, (*it).depth());
			printf("this image has %d colors\n", (*it).totalColors());

			const PixelPacket *pix = (*it).getConstPixels(0, 0, w, h);
			for (int iy = 0; iy < h; ++iy) {
				for (int ix = 0; ix < w; ++ix) {
					tColor col = (~(pix->opacity * 255 / MaxRGB) << 24) | ((pix->red * 255 / MaxRGB) << 16) | ((pix->green * 255 / MaxRGB) << 8) | (pix->blue * 255 / MaxRGB);
					bmp->DrawPixel(ix, iy, col);
					++pix;
				}
			}
			//mBitmaps.push_back(bmp);
		}
	} catch (Exception &e) {
			printf("ERROR: text2skin: Couldn't load %s: %s\n", Filename, e.what());
		delete bmp;
		return NULL;
	}
	return bmp;
}
