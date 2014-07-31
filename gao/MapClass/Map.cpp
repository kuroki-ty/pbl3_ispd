#include "Map.h"
 
//	tmp1 = coord2.getY()-coord1.getY(); //y'-y
//				↓
//	tmp1 = coord2.y-coord1.y; //y'-y


//	coord1.setX(samplePointA.getX()); //x
//				↓
//	coord1.x = samplePointA.x; //x

Map::Map()
{
    for(int i=0; i<LINE_NUM; i++)
    {
        RANSAC_A.push_back(0);
        RANSAC_B.push_back(0);
        RANSAC_C.push_back(0);
    }
}

void Map::calcLine()
{
    
    /*デバッグ用，txtファイルから読み込み*/
    //txtファイルから読み込む
    std::ifstream inFile_Location2("/Users/ispd/Workspace/Xcode/pbl3/opencv_test/opencv_test/obstacle_list.txt");
	if(!inFile_Location2.is_open())
	{
        std::cout<<"could not open the file \"obstacle_list.txt\"\n";
        std::cout<<"program terminating!\n";
		exit(EXIT_FAILURE);
	}
    
    std::string str1,str2;
    int num_Location2=0;
    std::ifstream fin;//打开文本
    
    fin.open("/Users/ispd/Workspace/Xcode/pbl3/opencv_test/opencv_test/obstacle_list.txt");
    while(getline(fin,str2))
        ++num_Location2;
    fin.close(); //关闭文本
    Location *obstacle_list = new Location[num_Location2];
    if(inFile_Location2.is_open())
    {
        for(int i=0;i<num_Location2;i++)
        {
            Coordinate tmp_wall;
            inFile_Location2 >> obstacle_list[i].number  >> obstacle_list[i].X >> obstacle_list[i].Y;
            std::cout << obstacle_list[i].number <<"  "<< obstacle_list[i].X << "  " << obstacle_list[i].Y << std::endl;
            
            tmp_wall.x = (obstacle_list[i].X+500)/10;
            tmp_wall.y = (obstacle_list[i].Y+500)/10;
            
            wall.push_back(tmp_wall);
        }
    }
    /*********************************/
    
    /*競技プログラム用，Mapが保持しているobstacle_point_listから読み込み
     
     for(int i=0;i<obstacle_point_list.size();i++)
     {
     Coordinate tmp_wall;
     tmp_wall.x = (obstacle_point_list[i].x+500)/10;
     tmp_wall.y = (obstacle_point_list[i].y+500)/10;
     wall.push_back(tmp_wall);
     }
     
     */
    
    std::vector<float> a(LINE_NUM);
    std::vector<float> b(LINE_NUM);
    
	for(int i = 0; i < LINE_NUM; i++)
	{
		RANSAC_y_Ax_B(wall, RANSAC_A[i], RANSAC_B[i], RANSAC_C[i], outliers, inliers, xflag);

        if (!xflag) {
            if (RANSAC_A[i]>0 && RANSAC_B[i]<0) {
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]>0 && RANSAC_B[i]>0){
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]<0 && RANSAC_B[i]>400){
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]<0 && RANSAC_B[i]<400){
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            
        }
        
        wall.clear();
		wall = outliers;
		outliers.clear();
	}
    
    double x,y;
    for(int i=0;i<LINE_NUM;i++)
    {
        for(int j=i+1;j<LINE_NUM;j++)
        {
            x=(b[j]-b[i])/(a[i]-a[j]);
            y=a[i]*(b[j]-b[i])/(a[i]-a[j])+b[i];
            line(x, y);
        }
    }
    //p1から順番にp配列にpush_backする
    p.push_back(p1);
    p.push_back(p2);
    p.push_back(p3);
    p.push_back(p4);
    
}

//マップを描画する
void Map::showMap()
{

    IplImage *img = 0;//図を定義する
    CvScalar rcolor;//色を定義する
    
    IplImage *test;//図を定義する
    test = cvCreateImage (cvSize (400, 400), IPL_DEPTH_8U, 3);
    cvZero (test);
    for(int x=0;x<test->height;x++){
        for(int y=0;y<test->width;y++) {
            test->imageData[test->widthStep * y + x * 3]     = 255; //B
            test->imageData[test->widthStep * y + x * 3 + 1] = 255; //G
            test->imageData[test->widthStep * y + x * 3 + 2] = 255; //R
        }
    }
    
    // 画像領域を確保し初期化する
    img = cvCreateImage (cvSize (400, 400), IPL_DEPTH_8U, 3);
    cvZero (img);
    //画像の背景を白にする
    for(int x=0;x<img->height;x++){
        for(int y=0;y<img->width;y++) {
            img->imageData[img->widthStep * y + x * 3]     = 255; //B
            img->imageData[img->widthStep * y + x * 3 + 1] = 255; //G
            img->imageData[img->widthStep * y + x * 3 + 2] = 255; //R
        }
    }


	for(int i = 0; i < LINE_NUM; i++)
	{        
        wall.clear();
		wall = outliers;
		outliers.clear();
        // 点の描画 ----- ここから
        CvPoint testpoint;
        rcolor = CV_RGB (255,200,0);
        for(int j=0;j<inliers_s[i].size();j++)
        {
            int x = (int)(inliers_s[i][j].x);
            int y = (int)(inliers_s[i][j].y);
            testpoint = cvPoint(x, y);
            
            cvCircle(img, testpoint, 2, rcolor, -5);
        }
        // 点の描画 ----- ここまで
        
        
        wall.clear();
		wall = outliers;
		outliers.clear();
	}
    
    // 線の描画 ----- ここから
    rcolor = CV_RGB (200,0,200);
    for(int i=0; i<LINE_NUM; i++)
    {
        if(i != LINE_NUM-1)
        {
            cvLine (img, p[i], p[i+1], rcolor, 2, CV_AA, 0);
        }
        else
        {
            cvLine (img, p[i], p[0], rcolor, 2, CV_AA, 0);

        }
    }
    // 線の描画 ----- ここまで
    
    cvFlip(img, NULL,0);//図を上下回転する
    cvNamedWindow ("Drawing", CV_WINDOW_AUTOSIZE);
    
    cvShowImage ("Drawing", img);
    
    cvSaveImage("/Users/ispd/Workspace/Xcode/pbl3/opencv_test/opencv_test/img.png",img);
    cvWaitKey (0);
    
    cvDestroyWindow ("Drawing");
    cvReleaseImage (&img);
 
    cvDestroyWindow ("testDrawing");
    cvReleaseImage (&test);
    
}

/********************************************
 *　関数名 gauss()
 *　2点の座標(x,y),(x',y')から直線の式を求める
 *　入力：2点の座標coord1(x,y),coord2(x',y')
 *　出力：y=Ax+BのAとB
 *********************************************/
void Map::gauss(Coordinate coord1, Coordinate coord2, float &A, float &B, float &C, bool &xflag)
{
	float coef=0.0;
    float tmp1=0.0;
	float tmp2=0.0;
	tmp1 = coord2.y-coord1.y; //y'-y
	tmp2 = coord2.x-coord1.x; //x'-x
    
    if(fabs(tmp1)<0.0000001)	// 分子が0のとき
	{
		A=0.0;
		B=(coord1.y+coord2.y)/2;  //y=B
		C=0.0;
	}
    else if(fabs(tmp2)<0.0000001)	// 分母が0のとき x=C
	{
		A=0.0;
		B=0.0;
		C=(coord1.x+coord2.y)/2; //x=C
		xflag=true;
	}
	else  // y=Ax+B
	{
		coef = tmp1/tmp2;
		A = coef;
		B = -coef*coord1.x+coord1.y;
		C = 0.0;
	}
    
}

////各直線の交点の位置関係を求める
void Map::line(double a,double b)
{
    if(0<a<400 && 0<b<400)
    {
        if(a<100)
        {
            if(b<100)
            {
                p1.x=a;
                p1.y=b;
                
            }
            else
            {
                p2.x=a;
                p2.y=b;
            }
        }
        else
        {
            if(b>100)
            {
                p3.x=a;
                p3.y=b;
                
            }
            else if(b>0)
            {
                p4.x=a;
                p4.y=b;
            }
        }
    }
}


/*  座標のリストを基に直線の係数を算出する
 *	入力：座標のリスト
 *	出力：直線の係数
 */
void Map::RANSAC_y_Ax_B(std::vector<Coordinate> wall, float &RANSAC_A, float &RANSAC_B, float &RANSAC_C, std::vector<Coordinate> &outliers, std::vector<Coordinate> &inliers, bool &xflag)
{
    
	Coordinate samplePointA;
	Coordinate samplePointB;
	Coordinate coord1;
	Coordinate coord2;
    
	float A=0.0;
	float B=0.0;
	float C=0.0;
	xflag=false;
    
	float threshold=THRESHOLD; //正解とみなす値の範囲
	float threshold_sq=threshold*threshold;
	float probA=0.0;
	float probB=0.0;
	float probC=0.0;
	bool  prob_xflag=false;
    
	int rand1,rand2;
	int localScore;
	int   score=0;
    
	for (int i=0;i<SEARCH;i++) // 点が多く含むラインを探す
	{
		rand1=rand()%wall.size();
		rand2=rand()%wall.size();
		samplePointA=wall[rand1];
		samplePointB=wall[rand2];
        
		//pointA.x *a + 1*b=pointA.y
		coord1.x=samplePointA.x; //x
		coord1.y=samplePointA.y; //y
        
		coord2.x=samplePointB.x; //x
		coord2.y=samplePointB.y; //y
		
        
		//連立方程式を解く　　solve equations
		xflag = false;
		gauss(coord1, coord2, A, B, C, xflag);
		localScore=0;
		
		Coordinate samplePoint;
		for(int j=0;j<wall.size();j++)
		{
			samplePoint=wall[j];
			double dist = 0.0;
			if(xflag) //x = Cのとき
			{
				dist = (C-samplePoint.x)*(C-samplePoint.x);
				if(dist<threshold_sq)
				{
					localScore++;
				}
			}
			else      //y=Ax+Bのとき
			{
				dist = ((A*samplePoint.x-samplePoint.y+B)*(A*samplePoint.x-samplePoint.y+B))/((A*A)+1);
                
				if(dist<threshold_sq)
				{
					localScore++;
				}
			}
            
		}
        
		if(localScore>score)
		{
			//仮パラメータをアップデート
			score=localScore;
			probA=A;
			probB=B;
			probC=C;
			prob_xflag = xflag;
		}
	}
    std::cout << "score・・・・・・・・・・・・・・" << score << std::endl;
    
	//正解の点のリスト用
	//std::vector<Coordinate> inliers; //距離d内に入っている点の座標の配列
	Coordinate inliers_buf;
	//外れ値のリスト
	Coordinate outliers_buf;
    
    
    
	double dist=0.0;
	
	inliers.clear();
	Coordinate samplePoint;
    
	for(int j=0;j<wall.size();j++)
	{
        //printf("wall[x,y]・・・・・・・・・・・・・・%d:[%lf,%lf]\n",j,wall[j].getX(),wall[j].getY());
		samplePoint=wall[j];
        
		if(prob_xflag){	// probCが0のとき、x=○○の式である
			dist = (probC-samplePoint.x)*(probC-samplePoint.x);
		}
		else
		{
			dist = ((probA*samplePoint.x-samplePoint.y+probB)*(probA*samplePoint.x-samplePoint.y+probB))/((probA*probA)+1);
		}
        //printf("dist_threshold・・・・・・・・・・・・・・%d:[%lf,%lf]\n",j,dist,threshold_sq);
        
        if(dist < threshold_sq)
        {
            //正解値リスト作成
            inliers_buf.x=wall[j].x;
            inliers_buf.y=wall[j].y;
            inliers.push_back(inliers_buf);
        }
        else
        {
            //外れ値リスト作成
            outliers_buf.x=wall[j].x;
            outliers_buf.y=wall[j].y;
            outliers.push_back(outliers_buf);
        }
        
	}
    std::cout << "outlier_SIZE・・・・・・・・・・・・・・・・・・・・・・・・・・・・・" << outliers.size() << std::endl;
    std::cout << "inlier_SIZE・・・・・・・・・・・・・・・・・・・・・・・・・・・・・" << inliers.size() << std::endl;
    
    //1ライン分のoutliersを2次元配列に格納
    std::vector<Coordinate> list;
    for(int i=0; i<outliers.size(); i++)
    {
        list.push_back(outliers[i]);
    }
    outliers_s.push_back(list);
    list.clear();
    
    //1ライン分のinliersを2次元配列に格納
    for(int i=0; i<inliers.size(); i++)
    {
        list.push_back(inliers[i]);
    }
    inliers_s.push_back(list);
    list.clear();

	if(prob_xflag)
	{
		RANSAC_A=0.0;
		RANSAC_B=0.0;
		RANSAC_C=probC;
		xflag = true;
	}
	else      // 全ての直線がx＝○○のときに使用
	{
		RANSAC_A=probA;
		RANSAC_B=probB;
		RANSAC_C=0.0;
		xflag = false;
	}
    
}

