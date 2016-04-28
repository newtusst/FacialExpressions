#include "MulticlassSVM.h"


MulticlassSVM::MulticlassSVM()
{

}

MulticlassSVM::~MulticlassSVM()
{
}


float MulticlassSVM::go(std::vector<std::vector<float>> trainingData, std::vector<float> trainingLabels, std::vector<std::vector<float>> testData, std::vector<float> testLabels)
{
	int successfullyPredicted = 0;

	cv::Mat trainingDataMat(trainingData.size(), trainingData[0].size(), CV_32FC1);
	cv::Mat trainingLabelsMat((trainingLabels.size()), 1, CV_32SC1);
	for (int i = 0; i < trainingData.size(); i++)
	{
		trainingLabelsMat.at<int>(i) = (int)trainingLabels[i];
		for (int j = 0; j < trainingData[0].size(); j++)
		{
			trainingDataMat.at<float>(i, j) = trainingData.data()[i][j];
		}
	}
	cout << "\nWould you like to load SVM.xml? y/n\n";
	std::string input;
	cin >> input;
	if (input == "y") {
		svm = StatModel::load<SVM>("SVM.xml");
	}
	if (svm == NULL)
	{
		std::cout << "Start training\n";
		//svm.load("SVM.xml");
		//if (svm.get_support_vector_count() == 0) {
		svm = SVM::create();
		svm->setType(ml::SVM::C_SVC);
		svm->setKernel(ml::SVM::RBF);
		svm->setGamma(3.375e-06);
		svm->setC(83.9666);
		svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.1));
		Ptr<ml::TrainData> tData = ml::TrainData::create(trainingDataMat, ml::SampleTypes::ROW_SAMPLE, trainingLabelsMat);
		svm->train(tData);

		//svm.train_auto(trainingDataMat, trainingLabelsMat, cv::Mat(), cv::Mat(), params);
		//params = svm.get_params();
		svm->save("SVM.xml");
	}
	//}
	FeatureExtractor FE;
	for (int i = 0; i < testLabels.size(); i++)
	{
		cv::Mat testDataMat(1, testData[i].size(), CV_32FC1, testData[i].data());
		float predictedLabel = svm->predict(testDataMat);
		std::cout << "Predicted: " << classToEmotion(predictedLabel) << "\n";
		std::cout << "Actual: " << classToEmotion(testLabels[i]) << ((predictedLabel == testLabels[i])? " << Success\n" : "\n");

		if (floor(predictedLabel) == floor(testLabels[i]))
		{
			successfullyPredicted += 1;
		}

	}
	float precision = ((float)successfullyPredicted / (testLabels.size()));
	return precision;
}

void MulticlassSVM::train(std::vector<std::vector<float>> trainingData, std::vector<float> trainingLabels, float C, float gamma)
{
	cv::Mat trainingDataMat = vectorToMat(trainingData);
	cv::Mat trainingLabelsMat((trainingLabels.size()), 1, CV_32SC1);
	for (int i = 0; i < trainingData.size(); i++)
	{
		trainingLabelsMat.at<int>(i) = (int)trainingLabels[i];
		for (int j = 0; j < trainingData[0].size(); j++)
		{
			trainingDataMat.at<float>(i, j) = trainingData.data()[i][j];
		}
	}
#ifndef DEBUG
	cout << "\nWould you like to load MulticlassSVM.xml? y/n\n";
	std::string input;
	cin >> input;
	if (input == "y") {
		svm = StatModel::load<SVM>("SVM.xml");
	}

	if (svm == NULL)
#endif // !DEBUG
	{
		std::cout << "Start training\n";
		svm = SVM::create();
		svm->setType(ml::SVM::C_SVC);
		svm->setKernel(ml::SVM::RBF);
		svm->setGamma(gamma);
		svm->setC(C);
		svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.1));
		Ptr<ml::TrainData> tData = ml::TrainData::create(trainingDataMat, ml::SampleTypes::ROW_SAMPLE, trainingLabelsMat);
		svm->trainAuto(tData);
		svm->save("SVM.xml");
		std::cout << "Finished training\n";
	}
}

void MulticlassSVM::predict(std::vector<std::vector<float>> testData, std::vector<float> testLabels, std::vector<float> &result)
{
	FeatureExtractor FE;
	for (int i = 0; i < testLabels.size(); i++)
	{
		cv::Mat testDataMat(1, testData[i].size(), CV_32FC1, testData[i].data());
		float predictedLabel = svm->predict(testDataMat);
		std::cout << "Predicted: " << classToEmotion(predictedLabel) << "\n";
		std::cout << "Actual: " << classToEmotion(testLabels[i]) << ((predictedLabel == testLabels[i]) ? " << Success\n" : "\n");

	}
}

float MulticlassSVM::predict(Mat testData)
{
		float predictedLabel = svm->predict(testData);
		std::cout << "Predicted: " << predictedLabel << "\n";
		return predictedLabel;
}