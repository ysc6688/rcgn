/*
 * author :  Shucheng Yin
 * stu_id :  SA14011016
 * e-mail :  ysc6688@mail.ustc.edu.cn
 *  date  :  2014-11-22
 *  file  :  Image.h
 *  dscp  :  The declaration of class Image
 */

#include "../include/Image.h"
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

const char res_dir[] = "../res/";
const char out_file[] = "recognition.data";
const char xml_file[] = "train_out.xml";
const int OFFSET = 7;
const int VECTOR_SIZE = 16;

bool read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses )
{
    const int M = 1024;
    char buf[M+2];

    Mat el_ptr(1, var_count, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen( filename.c_str(), "rt" );
    if( !f )
    {
        cout << "Could not read the database " << filename << endl;
        return false;
    }

    for(;;)
    {
        char* ptr;
        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
            break;
        responses.push_back((int)buf[0]);
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
        }
        if( i < var_count )
            break;
        _data->push_back(el_ptr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << filename << " is loaded.\n";

    return true;
}

bool build_svm_classifier( const string& data_filename,
                      const string& filename_to_save)
{
    int i;
    Mat data;
    Mat responses;
    bool ok = read_num_class_data( data_filename, VECTOR_SIZE, &data, &responses );
    if( !ok )
        return ok;
    int nsamples_all = data.rows;
    int ntrain_samples = (int)(nsamples_all*0.8);

    Mat train_data = data.rowRange(0,ntrain_samples);
    Mat test_data  = data.rowRange(ntrain_samples,nsamples_all);
    Mat train_response = responses.rowRange(0,ntrain_samples);
    Mat test_response = responses.rowRange(ntrain_samples,nsamples_all);

    cout << "Training the classifier ...\n";
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

     // Train the SVM
    CvSVM SVM;
    SVM.train(train_data, train_response, Mat(), Mat(), params);
    SVM.save(filename_to_save.c_str());

    cout << "Begin to test the classifier ..." << endl;
    int right = 0;
    for(i=0;i<nsamples_all - ntrain_samples;i++)
    {
    	Mat sample = test_data.row(i);
    	if(SVM.predict(sample)  == test_response.at<int>(i) )
    		right++;
    }
    cout << "The correct rate of the " << nsamples_all - ntrain_samples << " test cases is: " << right*100.0 / (nsamples_all-ntrain_samples)  << "%"<< endl;

		return true;
}

int predict(const string& sample)
{
	int i;
	char buf[80],*ptr;
	CvSVM SVM;
	SVM.load(xml_file);

	Mat sample_mat = Mat(1,VECTOR_SIZE,CV_32F);
	strcpy(buf,sample.c_str());
	ptr = buf;
	for (i = 0; i < VECTOR_SIZE; ++i)
	{
		int n = 0;
		sscanf( ptr, "%f%n", &sample_mat.at<float>(i), &n );
		ptr += n + 1;
	}

	return SVM.predict(sample_mat);
}

int main(int argc,char *argv[])
{
	int i,divide_fail = 0;
	char full_name[256];
	fstream out;
	std::vector<string> data;
	DIR *pDir;
	struct dirent *pFile;
	std::vector<string> name;
	std::vector<int> count,result;
	string test_vector;

	if (argc == 1)
	{
		std::cerr << "No file to read!" << std::endl;
		exit(0);
	}

	if (!strcmp(argv[1],"--data"))
	{	
		remove(out_file);
		out.open(out_file,ios::out | ios::app);
		if ((pDir=opendir(res_dir))==NULL)
		{
			cerr << "Can't open dir " << res_dir << " !" << endl;
			exit(0);
		}

		while( (pFile=readdir(pDir)) != NULL )
		{
			Image img;
			char *pName = pFile->d_name;
			if (!strcmp(pName,".") || !strcmp(pName,".."))
				continue;
			strcpy(full_name,"../res/");
			strcat(full_name,pName);
			img.LoadImg(full_name);;
			img.toGray();
			img.Binarization();
			img.NaiveRemoveNoise(1);
			img.ContoursRemoveNoise(5.0f);
			data.clear();
			img.FloodFillDivide(data,10,full_name+OFFSET,0);
	
			if (data.size() != 4)
			{
				name.push_back(string(full_name+7));
				count.push_back(data.size());
				divide_fail++;
				continue;
			}

			for (i = 0; i < 4; ++i)
				out << data.at(i) << std::endl;
		}
	
		out.close();
		std::cout << divide_fail << " cases failed when divided!" << std::endl;
		for (i = 0; i < (int)name.size(); ++i)
		{
			std::cout << name.at(i) << "   "  << count.at(i) << std::endl;
		}
		return 0;
	}
	else if( !strcmp(argv[1],"--train") )
	{
		build_svm_classifier(out_file,xml_file);
	}
	else if( !strcmp(argv[1],"-d") )
	{
		Image img;
		img.LoadImg(argv[2]);
		img.toGray();
		img.Binarization();
		img.NaiveRemoveNoise(1);
		img.ContoursRemoveNoise(5.0f);
		img.FloodFillDivide(data,10,argv[2],0);
	
		std::cout << "***********************************" << std::endl;
		std::cout << "The charactor vectors are as follows:" << std::endl;
		for (i = 0; i < (int)data.size(); ++i)
		{
			test_vector = data.at(i).substr(2,data.at(i).size());
			std::cout << test_vector << std::endl;
			result.push_back(predict(test_vector));
		}
		std::cout << "***********************************" << std::endl;
		cout << "The final charactors recognized is :" << endl;
		for (i = 0; i < (int)data.size(); ++i)
		{
			int ch = result.at(i);
			if (ch <= 57)
				cout << (char)(ch-48+'0') << " ";
			else
				cout << (char)(ch-65+'A') << " ";
		}
		cout << endl;
	}
	else {
		Image img;
		img.LoadImg(argv[1]);
		img.toGray();
		img.Binarization();
		img.NaiveRemoveNoise(1);
		img.ContoursRemoveNoise(5.0f);
		img.ShowInWindow("ordinary");
		img.FloodFillDivide(data,10,argv[1],1);
	
		std::cout << "***********************************" << std::endl;
		std::cout << "The charactor vectors are as follows:" << std::endl;
		for (i = 0; i < (int)data.size(); ++i)
		{
			test_vector = data.at(i).substr(2,data.at(i).size());
			std::cout << test_vector << std::endl;
			result.push_back(predict(test_vector));
		}
		std::cout << "***********************************" << std::endl;
		cout << "The final charactors recognized is :" << endl;
		for (i = 0; i < (int)data.size(); ++i)
		{
			int ch = result.at(i);
			if (ch <= 57)
				cout << (char)(ch-48+'0') << " ";
			else
				cout << (char)(ch-65+'A') << " ";
		}
		cout << endl;
	}

	return 0;
}
