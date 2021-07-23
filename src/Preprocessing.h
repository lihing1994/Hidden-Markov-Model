//
//  Preprocessing.hpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 11/19/20.
//

#ifndef Preprocessing_hpp
#define Preprocessing_hpp
#include "HiddenMarkovModelTester.h"
#include <time.h>
class Preprocessing
{
    public:
        Preprocessing(std::vector<int>sample_size,std::vector<std::string> family_name,int _unique_optcode_threshold =40,int number_of_states = 2);
        void pre_processing_certain_family();
        std::pair<int,int> get_traning_sample_range();
        std::pair<int,int> get_testing_sample_range();
        std::vector<std::string> get_observation_sequence_path();
        int get_unique_optocode_threshold();
        std::vector<std::string> generate_a_matrix(int hyperparameter_number_of_states);
        std::vector<std::pair<int, int>> get_testing_range();
    private:
        //variables
        std::vector<int> _each_family_sample_size;
        std::vector<std::string> _each_family_name;
        std::pair<int,int> traning_sample_range;
        std::pair<int,int> testing_sample_range;
        std::vector<std::pair<int, int>> testing_range;
        int _unique_optcode_threshold;
        int _number_of_states;
        //Private Helper function. Only be access within the class.
        std::vector<std::pair<std::string,int>> _temp_sort_map_zbot;
        std::vector<std::pair<std::string,int>> map_sort(std::map<std::string, int> m);
        std::vector<std::pair<std::string,int>> read_certain_sample(std::string family_name,int given_family_sample_size);
        void output_traning_sample_replace_by_assign_char(std::string family_name,std::vector<std::pair<std::string,int>> sort_map,int sample_size,int first_n);
        int associate_first_n_opcode(std::vector<std::pair<std::string,int>> sort_map,std::string opcode,int first_n);
        void print_sort_map(std::vector<std::pair<std::string,int>> temp);
        std::pair<int,int> random_select_traning_sample(int family_index,int random_selected_size);
        void output_obsevation_sequence(int family_index, std::pair<int, int> range);
        void output_pi_matrix();
        void output_a_matrix();
        void output_b_matrix();
};
#endif /* Preprocessing_hpp */
