

//ransac用 -------------------------------
#define SEARCH 500 	// 直線サーチ回数
#define THRESHOLD 5  // 最小二乗の閾値
//----------------------------------------

struct Coordinate
{
	float x;
	float y;
    
};




 
//	tmp1 = coord2.getY()-coord1.getY(); //y'-y
//				↓
//	tmp1 = coord2.y-coord1.y; //y'-y


//	coord1.setX(samplePointA.getX()); //x
//				↓
//	coord1.x = samplePointA.x; //x

/********************************************
 *　関数名 gauss()
 *　2点の座標(x,y),(x',y')から直線の式を求める
 *　入力：2点の座標coord1(x,y),coord2(x',y')
 *　出力：y=Ax+BのAとB
 *********************************************/
void gauss(Coordinate coord1,Coordinate coord2, float &A, float &B, float &C, bool &xflag)
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



/*
 *	入力：座標のリスト
 *	出力：直線の係数
 *
 *
 */
void RANSAC_y_Ax_B(std::vector<Coordinate> wall, float &RANSAC_A, float &RANSAC_B, float &RANSAC_C, std::vector<Coordinate> &outliers, std::vector<Coordinate> &inliers, bool &xflag)
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
    printf("score・・・・・・・・・・・・・・%d\n",score);
    
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
    printf("outlier_SIZE・・・・・・・・・・・・・・・・・・・・・・・・・・・・・%d\n",outliers.size());
    printf("inlier_SIZE・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・%d\n",inliers.size());
    
    for(int j=0;j<inliers.size();j++){
        //   printf("inliers・・・・・・・・・・・・・・%d:[%lf,%lf]\n",j,inliers[j].getX(),inliers[j].getY());
    }
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
CvPoint p1,p2,p3,p4;
void line(double a,double b)
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
///////////////////////////////////////////////////////////////////////////////////////////////


void Map::showMap()
{
    std::vector<float> RANSAC_A(4);
    std::vector<float> RANSAC_B(4);
    std::vector<float> RANSAC_C(4);
    std::vector<Coordinate> wall,outliers, inliers;
    bool xflag;
    //float X1,X2,Y1,Y2;
    IplImage *img = 0;//図を定義する
    CvPoint create_point,obstacle_point,obstacle_point1,obstacle_point2;
    CvScalar rcolor;//色を定義する
    int i;
    
    
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
    
    /*
    //Location *create_list = new Location[num_Location1];//Location型の構造体を新規する
    //if(inFile_Location1.is_open())//ファイルから一行毎の3つの数がそれぞれLocation型の構造体の3つの変数に入れる、printする
	//{
		for(int i=0;i<create_point_list.size();i++)
		{
            inFile_Location1 >> create_list[i].number >> create_list[i].X >> create_list[i].Y;
            cout << create_list[i].number <<"  "<< create_list[i].X << "  " << create_list[i].Y << "\n";
		}
    //}
    */
    //Location *obstacle_list = new Location[num_Location2];
   
	for(int i=0;i<obstacle_point_list.size();i++)
	{
            Coordinate tmp_wall;
            //inFile_Location2 >> obstacle_list[i].number  >> obstacle_list[i].X >> obstacle_list[i].Y;
            //cout << obstacle_list[i].number <<"  "<< obstacle_list[i].X << "  " << obstacle_list[i].Y << "\n";
            tmp_wall.x = (obstacle_point_list[i].x+500)/10;
            tmp_wall.y = (obstacle_point_list[i].y+500)/10;
            wall.push_back(tmp_wall);
	}
    
    //wall+4000
    //wall/500
    std::vector<float> a(4);
    std::vector<float> b(4);

	for(int i = 0; i < 4; i++)
	{
		RANSAC_y_Ax_B(wall, RANSAC_A[i], RANSAC_B[i], RANSAC_C[i], outliers, inliers, xflag);
        //線の描画------ここから
        if (!xflag) {
            if (RANSAC_A[i]>0 && RANSAC_B[i]<0) {
                obstacle_point1.x=400;
                obstacle_point1.y=400*RANSAC_A[i]+RANSAC_B[i];
                obstacle_point2.x=(-RANSAC_B[i])/RANSAC_A[i];
                obstacle_point2.y=0;
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]>0 && RANSAC_B[i]>0){
                obstacle_point1.x=0;
                obstacle_point1.y=RANSAC_B[i];
                obstacle_point2.x=(400-RANSAC_B[i])/RANSAC_A[i];
                obstacle_point2.y=400;
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]<0 && RANSAC_B[i]>400){
                obstacle_point1.x=400;
                obstacle_point1.y=400*RANSAC_A[i]+RANSAC_B[i];
                obstacle_point2.x=(400-RANSAC_B[i])/RANSAC_A[i];
                obstacle_point2.y=400;
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
            else if (RANSAC_A[i]<0 && RANSAC_B[i]<400){
                obstacle_point1.x=0;
                obstacle_point1.y=RANSAC_B[i];
                obstacle_point2.x=(-RANSAC_B[i])/RANSAC_A[i];
                obstacle_point2.y=0;
                a[i]=RANSAC_A[i];
                b[i]=RANSAC_B[i];
            }
        
        }
        else
        {
            obstacle_point1.x=0;
            obstacle_point1.y=RANSAC_C[i];
            obstacle_point2.x=400;
            obstacle_point2.y=RANSAC_C[i];
        }
        
        //rcolor = CV_RGB (0,0,255);
        //cvLine (img, obstacle_point1, obstacle_point2, rcolor, 5, CV_AA, 0);
        
        //線の描画-----ここまで
        
        // 点の描画 ----- ここから
        CvPoint testpoint;
        rcolor = CV_RGB (255,200,0);
        for(int j=0;j<inliers.size();j++)
        {
            int x = (int)(inliers[j].x);
            int y = (int)(inliers[j].y);
            testpoint = cvPoint(x, y);
            
            cvCircle(img, testpoint, 2, rcolor, -5);
            
        }
        printf("--------------------------------------------------------|||%lf,%lf\n",a[i],b[i]);
        
        // 点の描画 ----- ここまで
        
        
        wall.clear();
		wall = outliers;
		outliers.clear();
	}
    double x,y;
        for(int i=0;i<4;i++)
    {
        int j=i+1;
        for(j;j<4;j++)
        {
            x=(b[j]-b[i])/(a[i]-a[j]);
            y=a[i]*(b[j]-b[i])/(a[i]-a[j])+b[i];
            line(x, y);
        }
    }
    
   
    rcolor = CV_RGB (200,0,200);
    cvLine (img, p1, p2, rcolor, 2, CV_AA, 0);
    cvLine (img, p2, p3, rcolor, 2, CV_AA, 0);
    cvLine (img, p3, p4, rcolor, 2, CV_AA, 0);
    cvLine (img, p4, p1, rcolor, 2, CV_AA, 0);
    
    // ランダムな円を描画する
    // **行のデーターを読み込むまで、構造体のx,yを点の座標に入れる
    /*
    for (i =0; i<num_Location1; i++) {
        create_point.x = create_list[i].X;
        create_point.y = create_list[i].Y;
        rcolor = CV_RGB (255,0,0);//点（丸）の色を設定
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    */
    /*
    for (i =0; i<num_Location2; i++) {
        obstacle_point.x = obstacle_list[i].X;
        obstacle_point.y = obstacle_list[i].Y;
        rcolor = CV_RGB (255,200,0);
        cvCircle (img, obstacle_point, 10, rcolor,-5, CV_AA, 3);
        
    }
     */
/////////////////////////////
    



    
    
    //cvNamedWindow ("testDrawing", CV_WINDOW_AUTOSIZE);
    //cvShowImage ("testDrawing", test);
    
    
    
    
////////////////////////////
    
    
    cvFlip(img, NULL,0);//図を上下回転する
    cvNamedWindow ("Drawing", CV_WINDOW_AUTOSIZE);
    
    cvShowImage ("Drawing", img);
    
    cvSaveImage("/Users/gaohonghong/Documents/pblについて/img.png",img);
    cvWaitKey (0);
    
    cvDestroyWindow ("Drawing");
    cvReleaseImage (&img);
 
    cvDestroyWindow ("testDrawing");
    cvReleaseImage (&test);
    
    return 0;
}

