#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>

#define MAX_CLUSTERS (5)

int
main (int argc, char **argv)
{
  CvScalar color_tab[MAX_CLUSTERS] =
    { CV_RGB (255, 0, 0), CV_RGB (0, 255, 0), CV_RGB (100, 100, 255), CV_RGB (255, 0, 255), CV_RGB (255, 255, 0) };
  IplImage *img = cvCreateImage (cvSize (500, 500), IPL_DEPTH_8U, 3);
  CvRNG rng = cvRNG (time (NULL));
  CvPoint ipt;

  while (1) {
    int c;
    int k, cluster_count = cvRandInt (&rng) % MAX_CLUSTERS + 1;
    int i, sample_count = cvRandInt (&rng) % 1000 + MAX_CLUSTERS;
    CvMat *points = cvCreateMat (sample_count, 1, CV_32FC2);
    CvMat *clusters = cvCreateMat (sample_count, 1, CV_32SC1);

    // (1)複数のガウシアンから成るランダムサンプルを生成する
    for (k = 0; k < cluster_count; k++) {
      CvPoint center;
      CvMat point_chunk;
      center.x = cvRandInt (&rng) % img->width;
      center.y = cvRandInt (&rng) % img->height;
      cvGetRows (points, &point_chunk, k * sample_count / cluster_count,
                 k == cluster_count - 1 ? sample_count : (k + 1) * sample_count / cluster_count, 1);
      cvRandArr (&rng, &point_chunk, CV_RAND_NORMAL,
                 cvScalar (center.x, center.y, 0, 0), cvScalar (img->width * 0.1, img->height * 0.1, 0, 0));
    }

    // (2)ランダムサンプルをシャッフルする
    for (i = 0; i < sample_count / 2; i++) {
      CvPoint2D32f *pt1 = (CvPoint2D32f *) points->data.fl + cvRandInt (&rng) % sample_count;
      CvPoint2D32f *pt2 = (CvPoint2D32f *) points->data.fl + cvRandInt (&rng) % sample_count;
      CvPoint2D32f temp;
      CV_SWAP (*pt1, *pt2, temp);
    }

    // (3)K-menas法によるクラスタリング
    cvKMeans2 (points, cluster_count, clusters, cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0));

    // (4)クラスタ毎に色を変えてサンプルを描画する
    cvZero (img);
    for (i = 0; i < sample_count; i++) {
      int cluster_idx = clusters->data.i[i];
      ipt.x = (int) points->data.fl[i * 2];
      ipt.y = (int) points->data.fl[i * 2 + 1];
      cvCircle (img, ipt, 2, color_tab[cluster_idx], CV_FILLED, CV_AA, 0);
    }

    cvReleaseMat (&points);
    cvReleaseMat (&clusters);

    // (5)画像を表示，"Esc"キーが押されたときに終了
    cvNamedWindow ("Clusters", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Clusters", img);
    c = cvWaitKey (0);
    if (c == '\x1b')
      break;

    cvReleaseMat (&clusters);
    cvReleaseMat (&points);
  }

  cvDestroyWindow ("Clusters");
  cvReleaseImage (&img);

  return 0;
}
