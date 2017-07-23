#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <unistd.h>


CvHaarClassifierCascade *cascade;
CvMemStorage *storage;
int totalTime = 10;
void detect1(IplImage *img);
void detect2(IplImage *img);
double val1 = 0;
double val2 = 0;
double ratio1 = 0;
double ratio2 = 0;



int main(int argc, char** argv)
{
while(1){
  std::cout << "Signal 1 : Red " <<   std::endl;
  std::cout << "Signal 2 : Red " <<  std::endl;
  std::cout << "\n\n\n" <<  std::endl;
  std::cout << "Analyzing traffic" <<  std::endl;
  
  CvCapture *capture1;
  IplImage  *frame;
  int input_resize_percent = 100;
  cascade = (CvHaarClassifierCascade*) cvLoad("cars3.xml", 0, 0, 0);
  
 
/*****************************************Cam feed 1 starts*****************************/  
  storage = cvCreateMemStorage(0);
  capture1 = cvCaptureFromAVI("video1.mp4");
  assert(cascade && storage && capture1);
  //cvNamedWindow("video", 1);
  IplImage* frame1 = cvQueryFrame(capture1);
  frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);
  const int KEY_SPACE  = 32;
  const int KEY_ESC    = 27;
  int key = 0;
  do
  {
    frame1 = cvQueryFrame(capture1);
    if(!frame1)
      break;
    cvResize(frame1, frame);
    detect1(frame);
    key = cvWaitKey(10);
  }while(1); 
  //std::cout << "Final count 1:" << val1 <<  std::endl;//Debug
  cvDestroyAllWindows();
 
  
/*****************************************Cam feed 1 ends*****************************/  


/*****************************************Cam feed 2 starts*****************************/  
  storage = cvCreateMemStorage(0);
  capture1 = cvCaptureFromAVI("video2.mp4");
  assert(cascade && storage && capture1);
  //cvNamedWindow("video", 1); 
  frame1 = cvQueryFrame(capture1);
  frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);
  key = 0;
  do
  {
    frame1 = cvQueryFrame(capture1);
    if(!frame1)
      break;
    cvResize(frame1, frame);
    detect2(frame);
    key = cvWaitKey(10);
  }while(1);
  //std::cout << "Final count 2:" << val2 <<  std::endl;//Debug
  cvDestroyAllWindows();
  cvReleaseImage(&frame);
  cvReleaseCapture(&capture1);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&storage);

/*****************************************Cam feed 2 ends*****************************/  


if(val1 == 0 || val2==0)
{
std::cout << "Entered if" <<  std::endl;
ratio1 = totalTime * (1/2);
ratio2 = totalTime * (1/2);	
}
else
{
int tot = val1 + val2;
ratio1 = (val1 / tot );
ratio2 = (val2 / tot );

}

std::cout << "Value 1 (Signal 1 traffic):" << val1 <<  std::endl;
std::cout << "Value 2 (Signal 2 traffic):" << val2 <<  std::endl;
   
//std::cout << "Ratio 1 (Signal 1 traffic):" << ratio1 <<  std::endl;
//std::cout << "Ratio 2 (Signal 2 traffic):" << ratio2 <<  std::endl;

std::cout << "\n\n\n" <<  std::endl;

std::cout << "Signal 1 : Green " <<  std::endl;
std::cout << "Signal 2 : Red " <<  std::endl;
sleep(ratio1*totalTime);
std::cout << "\n\n\n" <<  std::endl;
std::cout << "Signal 1 : Red " <<   std::endl;
std::cout << "Signal 2 : Green " <<  std::endl;
sleep(ratio2*totalTime);
std::cout << "\n\n\n" <<  std::endl;
}
return 0;
}

void detect1(IplImage *img)
{
  CvSize img_size = cvGetSize(img);
  CvSeq *object = cvHaarDetectObjects(
    img,
    cascade,
    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(0,0),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );

  //std::cout << "Total: " << object->total << " cars" << std::endl;//Debug
  val1 = object->total;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(255, 0, 0), 2, 8, 0);
  }
  //cvShowImage("video", img);
}

void detect2(IplImage *img)
{
  CvSize img_size = cvGetSize(img);
  CvSeq *object = cvHaarDetectObjects(
    img,
    cascade,
    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(0,0),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );
  //std::cout << "Total: " << object->total << " cars" << std::endl; //Debug
  val2 = object->total;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(255, 0, 0), 2, 8, 0);
  }
  //cvShowImage("video", img);
}
