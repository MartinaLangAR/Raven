//train FFN 
//!-----------------input.csv and label.csv currently should be in the same directory as this script------------//
#define MLPACK_ENABLE_ANN_SERIALIZATION
#include "utils.hpp"
#include <iostream>
#include <mlpack.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <ensmallen.hpp>


int EPOCHS_PER_ITERATION = 10000; 
int ITERATION_MAX = 5;
bool retraining = false;

int main(){
	//mlpack version////////////////////////////////////////////////////
	int current_iteration = 0;
	std::vector<int> indices;
	arma::mat predictionTmp;
	double learning_rate = 0.1;
	mlpack::FFN<mlpack::MeanSquaredError, mlpack::RandomInitialization> model;
	
	///RETRAINING ONLY!
	//load model
	if (retraining){
		mlpack::data::Load("models/model_cam2_0_in6_4_3_2_reinforce.xml", "model", model);
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
		//model.Add<mlpack::LeakyReLU>();
		
		model.Add<mlpack::ann::Linear>(4);
		//model.Add<mlpack::Sigmoid>();
		//model.Add<mlpack::LeakyReLU>();
		//model.Add<mlpack::ann::Linear>(3);
		//model.Add<mlpack::LeakyReLU>();
		//model.Add<mlpack::Sigmoid>();
		model.Add<mlpack::ann::Linear>(2);
		//model.Add<mlpack::LeakyReLU>();
		std::cout << "Initialized Model..." << std::endl;
	}
	///Fresh training part ends here
    while (current_iteration < ITERATION_MAX and trainData.n_cols > 10) {
		ens::Adam opt(learning_rate, trainData.n_cols, 0.9, 0.999, 1e-8, 0, 1e-8, true, true);
		std::cout << "Started training iteration " << current_iteration << std::endl;
		std::cout << "Training Data size is " << size(trainData) << std::endl;
		//training loop
		for (int i=0; i<EPOCHS_PER_ITERATION; i++){
			model.Train(trainData, labelData); //, opt);
			//std::cout << "Training Loop: " << i << std::endl;
		}
		std::cout << "training iteration "<< current_iteration << "  completed" << std::endl;
		
		std::cout << "starting prediction" << std::endl;
		model.Predict(trainData, predictionTmp);
		std::cout << "predicting labels completed" << std::endl;
		
		arma::mat::const_row_col_iterator it = predictionTmp.begin_row_col();
		arma::mat::const_row_col_iterator it_end = predictionTmp.end_row_col();
		
		double acc_error = 0.0;
		for (; it != it_end; ++it){
				//std::cout << "predictionTmp hast the same dimensions as LabelData " << predictionTmp.n_rows << ", " << predictionTmp.n_cols << " == " << labelData.n_rows << ", " << labelData.n_cols << std::endl;
				double error = std::abs(predictionTmp(it.row(), it.col()) - labelData(it.row(), it.col()));
				acc_error += error;
				if (error > 0.02 and (indices.size() == 0 or indices.back() != it.col())){ //only keep the bad ones
					indices.push_back(it.col());
				}
				
		}
		std::cout << "average error is : " << acc_error/(2 * labelData.n_cols) << std::endl;
		std::cout << "num of errorous tries : " << indices.size() << "\n"<< std::endl;
		trainData = trainData.cols(arma::conv_to<arma::uvec>::from(indices));
		labelData = labelData.cols(arma::conv_to<arma::uvec>::from(indices));
		indices.clear();
		
		learning_rate /= 10;
		current_iteration++;
	}
	
	//Test again
	mlpack::data::Load("input_cam2_short.csv", trainData, true);
	mlpack::data::Load("label_cam2_short.csv", labelData, true);
			
	std::cout << "starting prediction" << std::endl;
	model.Predict(trainData, predictionTmp);
	std::cout << "predicting labels completed" << std::endl;
	
	arma::mat::const_row_col_iterator it = predictionTmp.begin_row_col();
	arma::mat::const_row_col_iterator it_end = predictionTmp.end_row_col();
	double acc_error = 0.0;
	for (; it != it_end; ++it){
			//std::cout << "predictionTmp hast the same dimensions as LabelData " << predictionTmp.n_rows << ", " << predictionTmp.n_cols << " == " << labelData.n_rows << ", " << labelData.n_cols << std::endl;
			double error = std::abs(predictionTmp(it.row(), it.col()) - labelData(it.row(), it.col()));
			acc_error += error;
				
	}
	std::cout << "resulting error is : " << acc_error/(2 * labelData.n_cols) << std::endl;
    
    //export model
    std::string model_name = "models/model_cam2_6_4_2_it10000_976samp.xml";
	mlpack::data::Save(model_name, "model", model, true);
	std::cout << "Model was written to : " << model_name << std::endl; 
    
   return 0;
}
