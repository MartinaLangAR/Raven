//train FFN 
//!-----------------input.csv and label.csv currently should be in the same directory as this script------------//
#define MLPACK_ENABLE_ANN_SERIALIZATION
#include "utils.hpp"
#include <iostream>
#include <mlpack.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/ffn.hpp>


int EPOCHS = 20000000; 
bool retraining = true;

int main(){
	//mlpack version////////////////////////////////////////////////////
	
	mlpack::FFN<mlpack::MeanSquaredError, mlpack::RandomInitialization> model;
	
	///RETRAINING ONLY!
	//load model
	if (retraining){
		mlpack::data::Load("models/model_cam2_0_short_0.xml", "model", model);
		std::cout << "Loaded model for retraining" << std::endl;
	}
	
	/// Retraining part ends here
	
	//load training set data
	arma::mat trainData;
	arma::mat labelData;
	mlpack::data::Load("input_cam2_short.csv", trainData, true);
	mlpack::data::Load("label_cam2_short.csv", labelData, true);
	
	std::cout << "Loaded data" << std::endl;
	
	
	///Fresh training ONLY!
	if (not retraining) {
		//init and design network (comment out if you want to retrain)
		model.Add<mlpack::ann::Linear>(6);
		model.Add<mlpack::LeakyReLU>();
		model.Add<mlpack::ann::Linear>(4);
		model.Add<mlpack::LeakyReLU>();
		model.Add<mlpack::ann::Linear>(3);
		model.Add<mlpack::LeakyReLU>();
		model.Add<mlpack::ann::Linear>(2);
		model.Add<mlpack::Sigmoid>();
		std::cout << "Initialized Model..." << std::endl;
	}
	///Fresh training part ends here

    std::cout << "Started training... " << std::endl;
	//training loop
	for (int i=0; i<EPOCHS; i++){
		model.Train(trainData, labelData);
    }
    
    std::cout << "training completed" << std::endl;
	
	//load test set data
	arma::mat testData;
	arma::mat testLabelData;
	mlpack::data::Load("input_cam2_short.csv", testData, true);
	mlpack::data::Load("label_cam2_short.csv", testLabelData, true);
	
	std::cout << "loaded test data" << std::endl;
	
	arma::mat predictionTmp;
	
	//test loop
	arma::mat::const_row_col_iterator ite = testData.begin_row_col();
	arma::mat::const_row_col_iterator ite_end = testData.end_row_col();
    

	std::cout << "starting prediction" << std::endl;
	
	model.Predict(testData, predictionTmp);
    std::cout << "predicting labels for test set completed" << std::endl;
	
	arma::mat::const_row_col_iterator it = predictionTmp.begin_row_col();
	arma::mat::const_row_col_iterator it_end = predictionTmp.end_row_col();
    
    double acc_error = 0.0;
    for (; it != it_end; ++it){
		//std::cout << *it << " | " << testLabelData(it.row(), it.col()) << std::endl;
			acc_error += std::abs(predictionTmp(it.row(), it.col()) - testLabelData(it.row(), it.col()));
	}
	std::cout << "average error is : " << acc_error/testLabelData.n_cols << std::endl;

    
    //export model
    std::string model_name = "models/model_cam2_0_short_1.xml";
	mlpack::data::Save(model_name, "model", model, true);
	std::cout << "Model was written to : " << model_name << std::endl; 
    
   return 0;
}
