


void Map::point()
{
    //txtファイルから読み込む
    /*
    ifstream inFile_Location1("/Users/gaohonghong/Documents/pblについて/cluster_list.txt");//ファイルを読み込む
	if(!inFile_Location1.is_open())//ファイルが開けないとエラーになる
	{
		cout<<"could not open the file \"create_list\"\n";
		cout<<"program terminating!\n";
		exit(EXIT_FAILURE);
	}
    
    string str1;
    int num_Location1=0;//文本行数
    
    ifstream fin;//打开文本
    
    fin.open("/Users/gaohonghong/Documents/pblについて/cluster_list.txt");
    while(getline(fin,str1))//行毎で読み込む、行がなくなるまでnum_Location1を増える
        ++num_Location1;
    fin.close(); //关闭文本
    
    */
    //Location *create_list = new Location[num_Location1];//Location型の構造体を新規する
    Location *create_list_in = new Location[num_Location1];
    //if(inFile_Location1.is_open())//ファイルから一行毎の3つの数がそれぞれLocation型の構造体の3つの変数に入れる、printする
	//{
		for(int i=0;i<obstacle_point_list.size();i++)
		{
            create_list_in[i].X = (obstacle_point_list[i].X+500);
            create_list_in[i].Y = (obstacle_point_list[i].Y+500);
		}
        
    //}
    //一番近い二つの点の距離が25以上の点を削除する、残るの点をfinalに保存する
    struct Location dis_over,dis_over2;
    double dis_all = 0.0;
    int final_num=0,g1=0,g2=0;
    for (int i=0; i<num_Location1; i++) {
        int x=create_list_in[i].X;
        int y=create_list_in[i].Y;
        double dis=0;
        for (int j=0; j<num_Location1; j++)
        {
            if(j!=i)
            {
                int x1=create_list_in[j].X;
                int y1=create_list_in[j].Y;
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
        
        dis_over.X=x;
        dis_over.Y=y;
        if (dis>25) {
            dis_over.number=create_list[i].number;
            dictance.push_back(dis_over);
            
        }
        else
        {
            dis_over.number = final_num;
            final.push_back(dis_over);
            //cout << final[i].number <<"  "<< final[i].X << "  " << final[i].Y << "\n";
            final_num ++;
        }
        
    }
    
    //距離が300以内の点を一つのグループにする（2回繰り返す、最後に三つのグループにする
    int x = final[0].X;
    int y = final[0].Y;
    double dis=0;
        for (int j=1; j<final.size(); j++)
        {
            int x1=final[j].X;
            int y1=final[j].Y;
            dis=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
            dis_over2.X= x1;
            dis_over2.Y= y1;
            dis_over2.number=final[j].number;

            if(dis>300)
            {
                group_a.push_back(dis_over2);
                
            }
            else
            {
                group1.push_back(dis_over2);
            }
        }
    
        
    int x2=group_a[0].X;
    int y2=group_a[0].Y;
    double dis2=0;
    for (int j=1; j<group_a.size(); j++)
    {
        int x1=group_a[j].X;
        int y1=group_a[j].Y;
        dis=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        dis_over2.X= x1;
        dis_over2.Y= y1;
        dis_over2.number=group_a[j].number;
        
        if(dis>300)
        {
            group3.push_back(dis_over2);
        }
        else
        {
            group2.push_back(dis_over2);
        }
    }
    
    int g1_x=0,g1_y=0,g2_x=0,g2_y=0,g3_x=0,g3_y=0;
    for (int i=0; i<group1.size(); i++) {
        g1_x += group1[i].X;
        g1_y += group1[i].Y;
    }
    
    for (int i=0; i<group2.size(); i++) {
        g2_x += group2[i].X;
        g2_y += group2[i].Y;
    }
    
    for (int i=0; i<group3.size(); i++) {
        g3_x += group3[i].X;
        g3_y += group3[i].Y;
    }
    
    
    
    int i;
    IplImage *img = 0;//図を定義する
    CvPoint create_point,create_point_over;//二つの点を定義する
    CvScalar rcolor;//色を定義する
    
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

    
    
    
    
    //障害物を色分けで描画
    for (i =0; i<group1.size(); i++) {
        create_point.x = group1[i].X;
        create_point.y = group1[i].Y;
        rcolor = CV_RGB (255,0,0);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    
    
   
    for (i =0; i<group2.size(); i++) {
        create_point.x = group2[i].X;
        create_point.y = group2[i].Y;
        rcolor = CV_RGB (255,255,0);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    
    for (i =0; i<group3.size(); i++) {
        create_point.x = group3[i].X;
        create_point.y = group3[i].Y;
        rcolor = CV_RGB (0,255,255);//点（丸）の色を設定
        
        cvCircle (img, create_point, 10, rcolor,-5, CV_AA, 3);//点を描く
    }
    
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
    
    
    
    //障害物の縦、横の長さを求める
    g1_x=0,g1_y=0,g2_x=0,g2_y=0,g3_x=0,g3_y=0;
    for (int i=0; i<group1.size(); i++) {
        g1_x += abs(group1[i].X-center_g1.x);
        g1_y += abs(group1[i].Y-center_g1.y);
        
    }
    
    for (int i=0; i<group2.size(); i++) {
        g2_x += abs(group2[i].X-center_g2.x);
        g2_y += abs(group3[i].Y-center_g3.y);
    }
    
    for (int i=0; i<group3.size(); i++) {
        g3_x += abs(group3[i].X-center_g3.x);
        g3_y += abs(group3[i].Y-center_g3.y);
    }
    
    double b1_x=(g1_x/group1.size())*2;
    double b1_y=(g1_y/group1.size())*2;
    double b2_x=(g2_x/group2.size())*2;
    double b2_y=(g2_y/group2.size())*2;
    double b3_x=(g3_x/group3.size())*2;
    double b3_y=(g3_y/group3.size())*2;
    cout << b1_x<<" "<<b1_y<<"  "<<b2_x<<"  "<<b2_y<<"  "<<b3_x<<"  "<<b3_y<<endl;
    
    
    ofstream outfile("/Users/gaohonghong/Documents/pblについて/result.txt",ios::in);
    if(!outfile)
    {
        cerr<<"open result.txt error!\n";
        return 0;
    }
    outfile<<"障害物1の横、縦の長さは" << b1_x << " " << b1_y   <<endl;
    outfile<<"障害物2の横、縦の長さは" << b2_x << " " << b3_y   <<endl;
    outfile<<"障害物3の横、縦の長さは" << b3_x << " " << b3_y   <<endl;
    outfile<<"障害物1の重心は" << center_g1.x << " " << center_g1.y   <<endl;
    outfile<<"障害物2の重心は" << center_g2.x << " " << center_g2.y   <<endl;
    outfile<<"障害物3の重心は" << center_g3.x << " " << center_g3.y   <<endl;

    outfile.close();
    
    
    cvFlip(img, NULL,0);//図を上下か回転する
    cvNamedWindow ("Drawing", CV_WINDOW_AUTOSIZE);
    
    cvShowImage ("Drawing", img);
    cvSaveImage("/Users/gaohonghong/Documents/pblについて/img.png",img);
    cvWaitKey (0);
    
    cvDestroyWindow ("Drawing");
    cvReleaseImage (&img);
    
    
}