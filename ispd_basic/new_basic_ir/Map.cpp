//#include "Map.h"

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
	inliers_maxi = 0;
	inliers_maxj = 0;
}

void Map::calc2Line()
{
    
    /*デバッグ用，txtファイルから読み込み
     
     //txtファイルから読み込む
     std::ifstream inFile_Location2("./test/obstacle_list.txt");
     if(!inFile_Location2.is_open())
     {
     std::cout<<"could not open the file \"obstacle_list.txt\"\n";
     std::cout<<"program terminating!\n";
     exit(EXIT_FAILURE);
     }
     
     std::string str1,str2;
     int num_Location2=0;
     std::ifstream fin;//打开文本
     
     fin.open("./test/obstacle_list.txt");
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
     
     tmp_wall.x = (obstacle_list[i].X+1000)/10;
     tmp_wall.y = (obstacle_list[i].Y+1000)/10;
     
     wall.push_back(tmp_wall);
     }
     }
     */
    /*********************************/
    
    /*競技プログラム用，Mapが保持しているobstacle_point_listから読み込み*/
    for(int i=0;i<this->wall_point_list2.size();i++)
    {
        Coordinate tmp_wall;
        tmp_wall.x = (this->wall_point_list2[i].x+1000)/10;
        tmp_wall.y = (this->wall_point_list2[i].y+1000)/10;
        wall.push_back(tmp_wall);
    }
    /*********************************/    
    
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
    /*
     for(int i=0;i<LINE_NUM;i++)
     {
     for(int j=i+1;j<LINE_NUM;j++)
     {
     x=(b[j]-b[i])/(a[i]-a[j]);
     y=a[i]*(b[j]-b[i])/(a[i]-a[j])+b[i];
     line(x, y);
     }
     }
    */
    
    //p1から順番にp配列にpush_backする
    /*
    x=(b[1]-b[0])/(a[0]-a[1]);
    y=a[0]*(b[1]-b[0])/(a[0]-a[1])+b[0];
    line(x, y);
    
    
    p2.x=30;
    p2.y=30;
    p3.x=inliers[inliers_max].x;
    p3.y=inliers[inliers_max].y;
    
    
    
    double k=0.0,m=0.0;
    double y1=p3.y-p2.y;
    double x1=p3.x-p2.x;
    k=y1/x1;
    m=p1.y-k*p1.x;
    
    p4.y=(p1.y/k)-(m/k);
    p4.x=k*p1.x+m;
    */

    x=(b[1]-b[0])/(a[0]-a[1]);
    y=a[0]*(b[1]-b[0])/(a[0]-a[1])+b[0];
    line(x, y);
    p.push_back(p1);

    float max = 0;
    //右壁の一番yが大きい点を選択
    for (int i=0;i<inliers_s.size();i++)
    {
	    for (int j=0;j<inliers_s[i].size();j++)
	    {
	        if (inliers_s[i][j].y>max) 
			{
	            max = inliers_s[i][j].y;
	            inliers_maxi=i;
	            inliers_maxj=j;
	        }
		}
    }
    p2.x=inliers_s[inliers_maxi][inliers_maxj].x;
    p2.y=inliers_s[inliers_maxi][inliers_maxj].y;

	float min = FLT_MAX;
    for (int i=0;i<inliers_s.size();i++)
    {
	    for (int j=0;j<inliers_s[i].size();j++)
	    {
	        if (inliers_s[i][j].x<min) 
			{
	            min = inliers_s[i][j].x;
	            inliers_maxi=i;
	            inliers_maxj=j;
	        }
		}
    }
    p4.x=inliers_s[inliers_maxi][inliers_maxj].x-40;
    p4.y=inliers_s[inliers_maxi][inliers_maxj].y;
    
    //p4.x=216.5;
    //p4.y=70;
    
    std::cout << "p2:" << p2.x <<"	"<<p2.y<<std::endl;
    double c3=0,c4=0,a3=0,a4=0;
 	double r3=abs(RANSAC_A[0]-0);
    double r4=abs(RANSAC_A[1]-0);
    if (r3>r4)
    {
        
        a3=RANSAC_A[0];
        a4=RANSAC_A[1];
    }
    else
    {
        a4=RANSAC_A[0];
        a3=RANSAC_A[1];
            
    }
    c3=p4.y-a3*p4.x;
    c4=p2.y-a4*p2.x;
        
    p3.x=(c3-c4)/(a4-a3);
    p3.y=a4*p3.x+c4;
    
    
    p.push_back(p2);
    p.push_back(p3);
    p.push_back(p4);
    
    
}
//離散している障害物点を削除、残りの点をgroup1,2,3の中に保存する



void Map::dividePoint()
{
    
#if 0
    /*デバッグ用，txtファイルから読み込み*/
    //txtファイルから読み込む
    std::ifstream inFile_Location2("/Users/ispd/Workspace/Xcode/pbl3/gao_map/gao_map/cluster_list.txt");
	if(!inFile_Location2.is_open())
	{
        std::cout<<"could not open the file \"cluster_list.txt\"\n";
        std::cout<<"program terminating!\n";
		exit(EXIT_FAILURE);
	}
    
    std::string str1,str2;
    int num_Location2=0;
    std::ifstream fin;//打开文本
    
    fin.open("/Users/ispd/Workspace/Xcode/pbl3/gao_map/gao_map/cluster_list.txt");
    while(getline(fin,str2))
        ++num_Location2;
    fin.close(); //关闭文本
    Location *cluster_list = new Location[num_Location2];
    if(inFile_Location2.is_open())
    {
        for(int i=0;i<num_Location2;i++)
        {
            Coordinate tmp_cluster;
            inFile_Location2 >> cluster_list[i].number  >> cluster_list[i].X >> cluster_list[i].Y;
            std::cout << cluster_list[i].number <<"  "<< cluster_list[i].X << "  " << cluster_list[i].Y << std::endl;
            
            tmp_cluster.x = (cluster_list[i].X+500);
            tmp_cluster.y = (cluster_list[i].Y+500);
            
            obstacle_point_list2.push_back(tmp_cluster);
        }
    }
    
#endif
    /*********************************/
    
    /*競技プログラム用，Mapが保持しているobstacle_point_listから読み込み*/
    
    struct Coordinate ob;
    for(int i=0;i<obstacle_point_list.size();i++)
    {
        ob.x = (obstacle_point_list[i].x+1000)/10;
        ob.y = (obstacle_point_list[i].y+1000)/10;
        obstacle_point_list2.push_back(ob);
    }
    
    /***********************************/
    
    
    //一番近い二つの点の距離が25以上の点を削除する、残るの点をfinalに保存する
    struct Coordinate dis_over,dis_over2;
    double dis_all = 0.0;
    int final_num=0,g1=0,g2=0;
    for (int i=0; i<obstacle_point_list2.size(); i++) {
        int x=obstacle_point_list2[i].x;
        int y=obstacle_point_list2[i].y;
        double dis=0;
        for (int j=0; j<obstacle_point_list2.size(); j++)
        {
            if(j!=i)
            {
                int x1=obstacle_point_list2[j].x;
                int y1=obstacle_point_list2[j].y;
                if((i==0&&j==1)||(i!=0&&j==0))
                {
                    dis=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
                }
                else
                {
                    
                    dis_all=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
                }
                if((i==0&&j==1)||(i!=0&&j==0))
                {
                    dis_all=dis;
                }
                else if(dis_all<dis)
                {
                    dis=dis_all;
                }
            }
        }
        
        dis_over.x=x;
        dis_over.y=y;
        if (dis>25) {
            //dis_over.number=create_list[i].number;
            distance.push_back(dis_over);
            
        }
        else
        {
            //dis_over.number = final_num;
            final.push_back(dis_over);
            //cout << final[i].number <<"  "<< final[i].x << "  " << final[i].y << "\n";
            final_num ++;
        }
        
    }
    
    //距離が300以内の点を一つのグループにする（2回繰り返す、最後に三つのグループにする
    int x = final[0].x;
    int y = final[0].y;
    double dis=0;
    for (int j=1; j<final.size(); j++)
    {
        int x1=final[j].x;
        int y1=final[j].y;
        dis=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
        dis_over2.x= x1;
        dis_over2.y= y1;
        //dis_over2.number=final[j].number;
        
        if(dis>30)
        {
            group_a.push_back(dis_over2);
            
        }
        else
        {
            group1.push_back(dis_over2);
        }
    }
    
    
    int x2=group_a[0].x;
    int y2=group_a[0].y;
    double dis2=0;
    
    std::cout << "group_aの配列サイズ:" << group_a.size() << std::endl;
    
    for (int j=1; j<group_a.size(); j++)
    {
        int x1=group_a[j].x;
        int y1=group_a[j].y;
        dis=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        dis_over2.x= x1;
        dis_over2.y= y1;
        //dis_over2.number=group_a[j].number;
        
        if(dis>30)
        {
            group3.push_back(dis_over2);
        }
        else
        {
            group2.push_back(dis_over2);
        }
    }
    
    
}

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
    img = cvCreateImage (cvSize (600, 600), IPL_DEPTH_8U, 3);
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
    
     for(int i=0; i<LINE_NUM4; i++)
     {
     	if(i != LINE_NUM4-1)
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
    //cvNamedWindow ("Drawing", CV_WINDOW_AUTOSIZE);
    
    //cvShowImage ("Drawing", img);
    
    cvSaveImage("./test/img.png",img);
    //cvWaitKey (0);
    
  //  cvDestroyWindow ("Drawing");
    cvReleaseImage (&img);
    
   // cvDestroyWindow ("testDrawing");
    cvReleaseImage (&test);
    
}


//マップを描画する
void Map::showMap2()
{

    /*********************************/
    
    /*競技プログラム用，Mapが保持しているobstacle_point_listから読み込み*/
    
    struct Coordinate ob;
    for(int i=0;i<obstacle_point_list.size();i++)
    {
        ob.x = (obstacle_point_list[i].x+1000)/10;
        ob.y = (obstacle_point_list[i].y+1000)/10;
        obstacle_point_list2.push_back(ob);
    }
    
    /***********************************/

    /*********************************/
    
    /*競技プログラム用，Mapが保持しているobstacle_point_listから読み込み*/
    std::vector<Coordinate> create_point_list2;
    for(int i=0;i<create_point_list.size();i++)
    {
        ob.x = (create_point_list[i].x+1000)/10;
        ob.y = (create_point_list[i].y+1000)/10;
        create_point_list2.push_back(ob);
    }
    
    /***********************************/

    
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
    img = cvCreateImage (cvSize (600, 600), IPL_DEPTH_8U, 3);
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
        rcolor = CV_RGB (0,0,255);
	// 障害物の描画
		CvPoint testpoint2;
        for(int i=0;i<obstacle_point_list2.size();i++)
        {
            int x = (int)(obstacle_point_list2[i].x);
            int y = (int)(obstacle_point_list2[i].y);
            testpoint2 = cvPoint(x, y);
            
            cvCircle(img, testpoint2, 2, rcolor, -5);
        }
	// create座標値の描画
        rcolor = CV_RGB (255,0,0);
        for(int i=0;i<create_point_list2.size();i++)
        {
            int x = (int)(create_point_list2[i].x);
            int y = (int)(create_point_list2[i].y);
            testpoint2 = cvPoint(x, y);
            
            //cvCircle(img, testpoint2, 2, rcolor, -5);
        }

    // 線の描画 ----- ここから
    rcolor = CV_RGB (200,0,200);
    for(int i=0; i<LINE_NUM4; i++)
    {
        if(i != LINE_NUM4-1)
        {
            cvLine (img, p[i], p[i+1], rcolor, 2, CV_AA, 0);
        }
        else
        {
            cvLine (img, p[i], p[0], rcolor, 2, CV_AA, 0);
            
        }
    }
    
    // 線の描画 ----- ここまで
    
    //壁の長さ,角度の計算とoutputーーーーここから
    
    double tate=0.0, yoko=0.0;
    yoko=sqrt(abs(p1.x-p4.x)*abs(p1.x-p4.x)+abs(p1.y-p4.y)*abs(p1.y-p4.y));
    tate=sqrt(abs(p1.x-p2.x)*abs(p1.x-p2.x)+abs(p1.y-p2.y)*abs(p1.y-p2.y));
    
    std::cout << "縦の長さは ：" << tate << std::endl;
    std::cout << "横の長さは ：" << yoko << std::endl;
    
    double k1 ;
    double k2 ;
    double k3 ;
    double k4 ;
    double c,c2,d,d2,e,e2,f,f2;
	c=p2.y-p1.y;
	d=p3.y-p2.y;
	e=p4.y-p3.y;
	f=p1.y-p4.y;

	c2=p2.x-p1.x;
	d2=p3.x-p2.x;
	e2=p4.x-p3.x;
	f2=p1.x-p4.x;

	k1=c/c2;
	k2=d/d2;
	k3=e/e2;
	k4=f/f2;
    double tan_1 = (abs(k1-k2))/(1+k1*k2);
    double ang_1 = atan(tan_1)*double(180)/pi;
    double tan_2 = (abs(k2-k3))/(1+k2*k3);
    double ang_2 =180-( atan(tan_2)*double(180)/pi);
    double tan_3 = (abs(k3-k4))/(1+k3*k4);
    double ang_3 = atan(tan_3)*double(180)/pi;
    double tan_4 = (abs(k4-k1))/(1+k4*k1);
    double ang_4 =180-( atan(tan_4)*double(180)/pi);
    
    //printf("$$$$$$$$   %f  %f    %f   %f   $$$$$$",ang_1,ang_2,ang_3,ang_4);
    
    //printf("$$$$$$$$   %f  %f    %f   %f   $$$$$$",ang_1,ang_2,ang_3,ang_4);

	float xmax = 0;
	float xmin = FLT_MAX;
	float ymax = 0;
	float ymin = FLT_MAX;

	float tmpx=0;
	float tmpy=0;

	float ob_gx;
	float ob_gy;
    for(int i=0;i<obstacle_point_list2.size();i++)
    {
		if( obstacle_point_list2[i].x > xmax)
		{
			xmax = obstacle_point_list2[i].x;
		}
		if( obstacle_point_list2[i].x < xmin)
		{
			xmin = obstacle_point_list2[i].x;
		}
		if( obstacle_point_list2[i].y > ymax)
		{
			ymax = obstacle_point_list2[i].y;
		}
		if( obstacle_point_list2[i].y < ymin)
		{
			ymin = obstacle_point_list2[i].y;
		}
		tmpx += obstacle_point_list2[i].x;
		tmpy += obstacle_point_list2[i].y;
    }

	float ob_yoko;
	float ob_tate;

	ob_yoko = xmax-xmin;
	ob_tate = ymax-ymin;

	ob_gx = tmpx / obstacle_point_list2.size(); 	
	ob_gy = tmpy / obstacle_point_list2.size();

    
    
    std::ofstream outfile("./result.txt",std::ios::in);
    if(!outfile)
    {
        std::cerr<<"open result.txt error!\n";
        exit(EXIT_FAILURE);
    }
    outfile<<"各角の角度は　" << ang_1 << "°, " << ang_2  << "°, " << ang_3 << "°, " << ang_4 << "°" << std::endl;
    outfile<<"壁の縦の長さは " << tate*10 << "[mm]" << std::endl;
    outfile<<"壁の横の長さは　" << yoko*10 << "[mm]" << std::endl;
    outfile<<"縮尺は1/10　" << std::endl;

    outfile<<"障害物の縦の長さは " << ob_tate*10 << "[mm]" << std::endl;
    outfile<<"障害物の横の長さは　" << ob_yoko*10 << "[mm]" << std::endl;
    outfile<<"障害物の重心位置 (x, y)=" << "("<< ob_gx*10 << "," << ob_gy*10 <<")" << std::endl;

    
    //壁の長さ、角度の計算とoutputーーーーここまで
    
#if 0    
    //障害物の色分け、重心、長さの計算とoutputーーーーーここから
    int g1_x=0,g1_y=0,g2_x=0,g2_y=0,g3_x=0,g3_y=0;
    for (int i=0; i<group1.size(); i++) {
        g1_x += group1[i].x;
        g1_y += group1[i].y;
    }
    std::cout << "----------------1-----------------" << std::endl;
    for (int i=0; i<group2.size(); i++) {
        g2_x += group2[i].x;
        g2_y += group2[i].y;
    }
    std::cout << "----------------2-----------------" << std::endl;    
    for (int i=0; i<group3.size(); i++) {
        g3_x += group3[i].x;
        g3_y += group3[i].y;
    }
     std::cout << "----------------3-----------------" << std::endl;   
    //障害物を色分けで描画
    CvPoint create_point;//障害物の点
    std::cout << "----------------4-----------------" << std::endl;
    for (int i =0; i<group1.size(); i++) {
        create_point.x = group1[i].x;
        create_point.y = group1[i].y;
        rcolor = CV_RGB (255,0,0);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    std::cout << "----------------5-----------------" << std::endl;    
    
    
    for (int i =0; i<group2.size(); i++) {
        create_point.x = group2[i].x;
        create_point.y = group2[i].y;
        rcolor = CV_RGB (255,255,0);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    std::cout << "----------------6-----------------" << std::endl;    
    for (int i =0; i<group3.size(); i++) {
        create_point.x = group3[i].x;
        create_point.y = group3[i].y;
        rcolor = CV_RGB (0,255,255);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    std::cout << "----------------7-----------------" << std::endl;    
    //-----各障害物の重心を求めるーーーー//
    CvPoint center_g1,center_g2,center_g3;
    
    center_g1.x=g1_x/group1.size();
    center_g1.y=g1_y/group1.size();
    
    center_g2.x=g2_x/group2.size();
    center_g2.y=g2_y/group2.size();
    
    center_g3.x=g3_x/group3.size();
    center_g3.y=g3_y/group3.size();
    
    
    rcolor = CV_RGB (22,22,222);
    cvCircle (img, center_g1, 10, rcolor,-5, CV_AA, 3);//点を描く
    cvCircle (img, center_g2, 10, rcolor,-5, CV_AA, 3);//点を描く
    cvCircle (img, center_g3, 10, rcolor,-5, CV_AA, 3);//点を描く
    //------重心を描くはここまで
    
    
    
    //障害物の縦、横の長さを求める（すべての点と重心の距離の和
    g1_x=0,g1_y=0,g2_x=0,g2_y=0,g3_x=0,g3_y=0;
    for (int i=0; i<group1.size(); i++) {
        g1_x += abs(group1[i].x-center_g1.x);
        g1_y += abs(group1[i].y-center_g1.y);
        
    }
    std::cout << "----------------8-----------------" << std::endl;    
    for (int i=0; i<group2.size(); i++) {
        g2_x += abs(group2[i].x-center_g2.x);
        g2_y += abs(group3[i].y-center_g3.y);
    }
    std::cout << "----------------9-----------------" << std::endl;    
    for (int i=0; i<group3.size(); i++) {
        g3_x += abs(group3[i].x-center_g3.x);
        g3_y += abs(group3[i].y-center_g3.y);
    }
    std::cout << "----------------10-----------------" << std::endl;    
    double b1_x=(g1_x/group1.size())*4;
    double b1_y=(g1_y/group1.size())*4;
    double b2_x=(g2_x/group2.size())*4;
    double b2_y=(g2_y/group2.size())*4;
    double b3_x=(g3_x/group3.size())*4;
    double b3_y=(g3_y/group3.size())*4;
    //cout << b1_x<<" "<<b1_y<<"  "<<b2_x<<"  "<<b2_y<<"  "<<b3_x<<"  "<<b3_y<<endl;
    
    outfile<<"障害物1の横、縦の長さは" << b1_x << " " << b1_y   << std::endl;
    outfile<<"障害物2の横、縦の長さは" << b2_x << " " << b3_y   << std::endl;
    outfile<<"障害物3の横、縦の長さは" << b3_x << " " << b3_y   << std::endl;
    outfile<<"障害物1の重心は" << center_g1.x << " " << center_g1.y   << std::endl;
    outfile<<"障害物2の重心は" << center_g2.x << " " << center_g2.y   << std::endl;
    outfile<<"障害物3の重心は" << center_g3.x << " " << center_g3.y   << std::endl;
    
    outfile.close();
    
    
    //障害物の色分け、重心、長さの計算とoutputーーーーーここまで
#endif    
    cvFlip(img, NULL,0);//図を上下回転する
    cvNamedWindow ("Drawing", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Drawing", img);
    cvSaveImage("./img.png",img);
    //cvWaitKey (0);
 
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
    /*
     
     if(a<400 && b<400 && a>0 && b>0)
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
     */
    
    if (a>0 && a<600 &&b>0 &&b<600) {
        if (a>300 ) {
            p1.x=a;
            p1.y=b;
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
