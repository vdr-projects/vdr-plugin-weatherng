/*
 * borrowed from vdr-text2skin
 */


#ifndef X_DISPLAY_MISSING
#define X_DISPLAY_MISSING
#endif

#ifdef USE_MAGICK
#include "setup.h"
#include <Magick++.h>
using namespace Magick;
#else
#include <stddef.h>
#include <Imlib2.h>
#endif
#include <glob.h>

#include <vdr/tools.h>

#include "bitmap.h"
#include "quantize.h"



#ifdef USE_MAGICK
cBitmapCache cWeatherBitmap::mCache(1024);
#else
cBitmapCache cWeatherBitmap::mCache(1);
#endif

void cBitmapCache::DeleteObject(const tBitmapSpec &Key, cWeatherBitmap *&Data)
{
	delete Data;
}

void cBitmapCache::ResetObject(cWeatherBitmap *&Data)
{
	Data->Reset();
}


cWeatherBitmap *cWeatherBitmap::Load(const std::string &Filename, int Alpha, int height, 
                                         int width, int colors, bool Quiet) {
	tBitmapSpec spec(Filename, Alpha, height, width, colors);
	printf("checking image with spec %s_%d_%d_%d_%d..", Filename.c_str(),Alpha,height,width,colors);
	std::string fname = Filename;

	cWeatherBitmap *res = NULL;
	if (mCache.Contains(spec)) {
		res = mCache[spec];
		printf("..cache ok\n");
	} else {
		int pos;
		if ((pos = fname.find('*')) != -1) {
			glob_t gbuf;
			if (glob(fname.c_str(), 0, NULL, &gbuf) == 0){
				printf("GLOB: FOUND %s\n", gbuf.gl_pathv[0]);
				fname = gbuf.gl_pathv[0];
			} else {
				if (!Quiet)
					esyslog("weatherng-image: ERROR: No match for wildcard filename %s", 
							Filename.c_str());
				fname = "";
			} 
			globfree(&gbuf);
		}

		res = new cWeatherBitmap;
		bool result = false;


#ifdef USE_MAGICK
		result = res->LoadMagick(fname.c_str(),height,width,colors, Quiet);
#else
		result = res->LoadImlib(fname.c_str(),height,width,colors, Quiet);
#endif
	        printf("..load %sok\n", result ? "" : "not ");

		if (result) {
		  res->SetAlpha(Alpha);
		  }
		else {
		  esyslog("weatherng-image: ERROR: filename %s too short to identify format", fname.c_str());
		  DELETENULL(res);
		}	
		mCache[spec] = res;
	}
	return res;
}

bool cWeatherBitmap::Available(const std::string &Filename, int Alpha, int height, int width, 
                                 int colors)
{
	if ((int)Filename.find('*') != -1) {
		bool result = false;
		glob_t gbuf;
		if (glob(Filename.c_str(), 0, NULL, &gbuf) == 0)
			result = true;
		globfree(&gbuf);
		return result;
	} else
		return access(Filename.c_str(), F_OK) == 0;
}

cWeatherBitmap::cWeatherBitmap(void) {
	mCurrent = 0;
	mLastGet = 0;
}

cWeatherBitmap::~cWeatherBitmap() {
	for (int i = 0; i < (int)mBitmaps.size(); ++i)
		delete mBitmaps[i];
	mBitmaps.clear();
}


cBitmap &cWeatherBitmap::Get(void) {
	if (mBitmaps.size() == 1)
		return *mBitmaps[0];

/*	time_t upd;
	int diff;
	if (mLastGet == 0) {
		mLastGet = Now;
		upd = mDelay;
	} else if ((diff = Now - mLastGet) >= mDelay) {
		mCurrent = (mCurrent + 1) % mBitmaps.size();
		mLastGet = Now;
		upd = mDelay - diff > 1 ? mDelay - diff : 1;
	} else {
		upd = mDelay - diff;
	}

	if (UpdateIn == 0 || UpdateIn > (uint)upd)
		UpdateIn = upd;
*/
        mCurrent = (mCurrent + 1) % mBitmaps.size();
	return *mBitmaps[mCurrent];

}


void cWeatherBitmap::SetAlpha(int Alpha) {
	if (Alpha > 0) {
		std::vector<cBitmap*>::iterator it = mBitmaps.begin();
		for (; it != mBitmaps.end(); ++it) {
			int count;
			if ((*it)->Colors(count)) {
				for (int i = 0; i < count; ++i) {
					int alpha = (((*it)->Color(i) & 0xFF000000) >> 24) * Alpha / 255;
					(*it)->SetColor(i, ((*it)->Color(i) & 0x00FFFFFF) | (alpha << 24));
				}
			}
		}
	}
}



#ifdef USE_MAGICK
extern cWetterSetup wetterSetup;

bool cWeatherBitmap::LoadMagick(const char *Filename, int height, int width, int colors, bool Quiet) {
  std::string geometry;
  std::vector<Image> images;
  cBitmap *bmp = NULL;
  
  try {
    int w, h;
    std::vector<Image>::iterator it;
    readImages(&images, Filename);

    if(images.size() == 0)	{
      esyslog("weatherng-image: ERROR: images.size == NULL . could not load %s", Filename);
      return false;
    }

    printf("weatherng-image: load image %s\n", Filename);

    geometry = Geometry(width,height);

    for (it =images.begin(); it !=images.end(); ++it) {
      if (height != 0 || width != 0) {
        (*it).sample(geometry);
      }  

      if (colors != 0){
        (*it).magick("RGB");
	(*it).quantizeTreeDepth(wetterSetup.w_treedepth);
        (*it).opacity(OpaqueOpacity);
        (*it).backgroundColor( Color ( 0,0,0,0) );
        (*it).quantizeColorSpace( RGBColorspace );
        (*it).quantizeDither( wetterSetup.w_dither );
        (*it).quantizeColors( colors );
        (*it).quantize();
      }

      w = (*it).columns();
      h = (*it).rows();

      bmp = new cBitmap(w, h, (*it).depth());

      const PixelPacket *pix = (*it).getConstPixels(0, 0, w, h);
      for (int iy = 0; iy < h; ++iy) {
        for (int ix = 0; ix < w; ++ix) {
	  tColor col = (~(pix->opacity * 255 / MaxRGB) << 24) | ((pix->red * 255 / MaxRGB) << 16) | ((pix->green * 255 / MaxRGB) << 8) | (pix->blue * 255 / MaxRGB);
	  bmp->DrawPixel(ix, iy, col);
	  ++pix;
        }
      }
      mBitmaps.push_back(bmp);
    }
  }    

  catch (Exception &e) {
    if (!Quiet)	esyslog("weatherng-image: ERROR: Couldn't load %s: %s", Filename, e.what());
    delete bmp;
    return false;
  }
  catch (...) {
    if (!Quiet)	esyslog("weatherng-image: ERROR: Couldn't load %s: Unknown exception caught", Filename);
    delete bmp;
    return false;
  }

  return true;
}

#else
bool cWeatherBitmap::LoadImlib(const char *Filename, int height, int width, int colors, bool Quiet) {
  Imlib_Image buffer;
  int h,w;
  unsigned char * outputImage = NULL;
  unsigned int * outputPalette = NULL;
  cQuantizeWu* quantizer = new cQuantizeWu();
  cBitmap *bmp = NULL;

  printf("weatherng-image: Loading '%s' with parameters: h:'%i' w:'%i' colors:'%i'\n", Filename, height, width, colors);
  
  buffer = imlib_load_image(Filename);

  imlib_context_set_image(buffer);
  w= imlib_image_get_width();
  h= imlib_image_get_height();
  imlib_context_set_image(buffer);
	
  if (!buffer) {
    esyslog("weatherng-image: ERROR: buffer empty. ...aborting!");
    delete(quantizer);
    return false;
  }
  
  Imlib_Image image;
  image = imlib_create_cropped_scaled_image(0, 0, w, h ,width , height);

  imlib_context_set_image(buffer);
  imlib_free_image_and_decache();

  imlib_context_set_image(image);
  w= imlib_image_get_width();
  h= imlib_image_get_height();
  imlib_context_set_image(image);
	
  bmp = new cBitmap(w, h, 8);
  uint8_t *data = (uint8_t*)imlib_image_get_data_for_reading_only();

  if ( colors != 0 ){
    quantizer->Quantize(data, w* h, colors);
    outputImage = quantizer->OutputImage();
    outputPalette = quantizer->OutputPalette();
  }
  imlib_free_image();

  int pos = 0;
  for (int y = 0; y < bmp->Height(); ++y) {
    for (int x = 0; x < bmp->Width(); ++x) {
      if ( colors != 0 ) {
        bmp->DrawPixel(x, y ,  outputPalette[outputImage[y * bmp->Width() + x]] | 0xFF000000 );
	}
      else {	
	tColor col = (data[pos + 3] << 24) | (data[pos + 2] << 16) | (data[pos + 1] << 8) | data[pos + 0];
	bmp->DrawPixel(x, y, col);
	pos += 4;
      }
    }
  }
	
  mBitmaps.push_back(bmp);
  delete(quantizer);
  return true;
}

#endif
