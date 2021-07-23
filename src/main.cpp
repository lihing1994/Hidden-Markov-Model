//
//  main.cpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 10/30/20.
//

#include "Preprocessing.h"
#include <chrono>
int main(int argc, const char * argv[]) {
    
    auto start = std::chrono::steady_clock::now();
    std::vector<int> sample_size;
    std::vector<std::string> sample_family_name;
    sample_size.push_back(2135);
    sample_family_name.push_back("zbot");
    
    sample_size.push_back(4359);
    sample_family_name.push_back("winwebsec");
    sample_size.push_back(1305);
    sample_family_name.push_back("zeroaccess");
    Preprocessing * proprocess = new Preprocessing(sample_size,sample_family_name,20,2);
    proprocess->pre_processing_certain_family();
    auto end = std::chrono::steady_clock::now();
    std::vector<std::pair<int,int>> testing_range = proprocess->get_testing_range();
    
    HiddenMarkovModelTester hmm_zbot_tester("zbot_A_Matrix.txt","zbot_B_Matrix.txt","zbot_Pi_Matrix.txt","zbot_observation_sequence.txt");
    hmm_zbot_tester.traning();
    //Testing the zbot model.
    hmm_zbot_tester.testing("zbot",testing_range.at(0).first,testing_range.at(0).second);
    hmm_zbot_tester.testing("zeroaccess",1,101);
    hmm_zbot_tester.testing("winwebsec", 1, 101);
    //Testing the winwebsec model.
    HiddenMarkovModelTester hmm_winwebsec_tester("winwebsec_A_matrix.txt","winwebsec_B_matrix.txt","winwebsec_Pi_matrix.txt","winwebsec_observation_sequence.txt");
    hmm_winwebsec_tester.traning();
    hmm_winwebsec_tester.testing("winwebsec", testing_range.at(1).first, testing_range.at(1).second);
    hmm_winwebsec_tester.testing("zbot", 1, 101);
    hmm_winwebsec_tester.testing("zeroaccess", 1, 101);
    //Testing the zeroaccess model.
    HiddenMarkovModelTester hmm_zeroaccess_tester("zeroaccess_A_matrix.txt","zeroaccess_B_matrix.txt","zeroaccess_Pi_matrix.txt","zeroaccess_observation_sequence.txt");
    hmm_zeroaccess_tester.traning();
    hmm_zeroaccess_tester.testing("zeroaccess", testing_range.at(2).first,testing_range.at(2).second);
    hmm_zeroaccess_tester.testing("zbot", 1, 101);
    hmm_zeroaccess_tester.testing("winwebsec", 1, 101);
    delete proprocess;
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Time to completed whole training and testing process is equal to : " << (float)elapsed_seconds.count()/60 << " min"<<std::endl;
    return 0;
}
/*
 HiddenMarkovModelTester hmm_tester("A_Matrix.txt","B_Matrix.txt","Pi_Matrix.txt","Observation_sequence.txt");
 hmm_tester.alpha_pass_tester();
 hmm_tester.beta_pass_tester();
 hmm_tester.output_alpha_matrix();
 hmm_tester.output_beta_matrix();
 
 */
