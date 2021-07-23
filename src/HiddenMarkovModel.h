//
//  HiddenMarkovModel.hpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 10/30/20.
//

#ifndef HiddenMarkovModel_hpp
#define HiddenMarkovModel_hpp
#pragma once
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cfloat>
class HiddenMarkovModel
{
    public:
        HiddenMarkovModel(std::vector<std::vector<float>> * aMatrix,std::vector<std::vector<float>> * bMatrix,std::vector<float> * piMatrix,int maxIters = 100);
        void alpha_pass(std::vector<int> * observation_sequence);
        void beta_pass(std::vector<int> * observation_sequence);
        void gammas_pass(std::vector<int>* observation_sequence);
        void re_estimation(std::vector<int>* observation_matrix);
        void training(std::vector<int>* observation_sequence);
        void testing(std::string family_name,int index_begin, int index_end);
        void set_alpha_matrix(std::vector<std::vector<float>> alpha);
        void set_beta_matrix(std::vector<std::vector<float>> beta);
        bool earily_stopping(std::vector<int> *observation_sequence);
        void print_alpha_matrix();
        void print_beta_matrix();
        void print_gammas_matrix();
        std::vector<std::vector <float>>* get_alpha_matrix();
        std::vector<std::vector <float>>* get_beta_matrix();
        double cal_log_value_for_training();
        double get_current_prob();
    private:
        void _initMatrix(std::vector<std::vector<std::vector<float>>> * ptr,int x,int y,int z);
        void _initMatrix(std::vector<std::vector<float>> * ptr,int row,int column);
        void _initMatrix(std::vector<float> * ptr,int column);
        std::vector<std::vector <float>> * _a_matrix;
        std::vector<std::vector <float>> * _b_matrix;
        std::vector<float> * _pi_matrix;
        std::vector<int> * _observation_sequence;
        std::vector<std::vector <float>> _alpha_matrix;
        std::vector<std::vector <float>> _beta_matrix;
        std::vector<std::vector <float>> _gammas_matrix;
        std::vector<std::vector<std::vector<float>>> _di_gammas_matrix;
        std::vector<float> _c_matrix;
        int _max_iters;
        int _number_of_iters;
        int _number_of_state;
        double _old_log_prob;
        int _number_of_observation;
        bool _is_alpha_pass_completed;
        bool _is_beta_pass_completed;
        bool _is_gammas_pass_completed;
};
#endif /* HiddenMarkovModel_hpp */
