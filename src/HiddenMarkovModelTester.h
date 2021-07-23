//
//  HiddenMarkovModelTester.hpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 11/1/20.
//

#ifndef HiddenMarkovModelTester_hpp
#define HiddenMarkovModelTester_hpp
#include <map>
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "HiddenMarkovModel.h"
class HiddenMarkovModelTester
{
    public:
        HiddenMarkovModelTester(std::string a_matrix_path, std::string b_matrix_path, std::string pi_matrix_path, std::string observation_sequence_path);
        ~HiddenMarkovModelTester();
        void print_matrix(std::vector<std::vector<float>> temp_matrix);
        void print_matrix(std::vector<float> temp_matrix);
        void print_matrix(std::vector<int> temp_matrix);
        void alpha_pass_tester();
        void beta_pass_tester();
        void gammas_pass_tester();
        void re_estimation();
        void output_alpha_matrix();
        void output_beta_matrix();
        void get_a_matrix_size();
        void get_b_matrix_size();
        void get_pi_matri_size();
        void output_a_matrix();
        void output_b_matrix();
        void output_pi_matrix();
        void traning();
        void testing(std::string family_name,int index_begin, int index_end);
    private:
        std::string _a_matrix_path;
        std::string _b_matrix_path;
        std::string _pi_matrix_path;
        std::string _observation_sequence_path;
        std::vector<std::vector<float>> _a_matrix;
        std::vector<std::vector<float>> _b_matrix;
        std::vector<float> _pi_matrix;
        std::vector<int> _observation_sequence;
        HiddenMarkovModel * _hmm;
        std::vector<std::vector<float>> _get_float_matrix_from_txt_file(std::string path);
        std::vector<float> _get_float_one_by_one_matrix_from_txt_file(std::string path);
        std::vector<int> _get_int_one_by_one_matrix_from_txt_file(std::string path);
        
};
#endif /* HiddenMarkovModelTester_hpp */
