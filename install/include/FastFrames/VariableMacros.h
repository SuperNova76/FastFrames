// This file contains the macros used to define histogram templates, preventing JIT compilation when user specifies the histogram type.
#pragma once

#include "ROOT/RVec.hxx"


// The scalar case is for the case where we want to add support for TYPE but not for VECTOR_TYPE.
// This is important for BOOL, where no good implementation of std::vector<bool> is available.
#define ADD_HISTO_1D_SUPPORT_SCALAR(CodeType, CppType) \
    case VariableType::CodeType : \
        return node.Histo1D<CppType, double>(variable.histoModel1D(), \
                                            this->systematicVariable(variable, systematic), \
                                            this->systematicWeight(systematic)); \
        break;

#define ADD_HISTO_1D_SUPPORT_VECTOR(CodeType, CppType) \
    case VariableType::CodeType : \
        return node.Histo1D<CppType, double>(variable.histoModel1D(), \
                                            this->systematicVariable(variable, systematic), \
                                            this->systematicWeight(systematic)); \
        break; \
    case VariableType::VECTOR_##CodeType : /* The ## operator is used for macro token concatenation. */ \
        return node.Histo1D<std::vector<CppType>, double>(variable.histoModel1D(), \
                                            this->systematicVariable(variable, systematic), \
                                            this->systematicWeight(systematic)); \
        break; \
    case VariableType::RVEC_##CodeType : \
        return node.Histo1D<ROOT::VecOps::RVec<CppType>, double>(variable.histoModel1D(), \
                                            this->systematicVariable(variable, systematic), \
                                            this->systematicWeight(systematic)); \
        break;


#define ADD_HISTO_1D_SUPPORT_SCALAR_TRUTH(CodeType, CppType) \
    case VariableType::CodeType : \
        return node.Histo1D<CppType, double>(variable.histoModel1D(), \
                                             definition, \
                                             "weight_truth_TOTAL_"+truth->name()); \
        break;

#define ADD_HISTO_1D_SUPPORT_VECTOR_TRUTH(CodeType, CppType) \
    case VariableType::CodeType : \
        return node.Histo1D<CppType, double>(variable.histoModel1D(), \
                                             definition, \
                                             "weight_truth_TOTAL_"+truth->name()); \
        break; \
    case VariableType::VECTOR_##CodeType : \
        return node.Histo1D<std::vector<CppType>, double>(variable.histoModel1D(), \
                                                          definition, \
                                                          "weight_truth_TOTAL_"+truth->name()); \
        break; \
    case VariableType::RVEC_##CodeType : \
        return node.Histo1D<ROOT::VecOps::RVec<CppType>, double>(variable.histoModel1D(), \
                                                                 definition, \
                                                                 "weight_truth_TOTAL_"+truth->name()); \
        break;

#define NOT_JIT_2D_HISTOGRAMS
#ifndef NOT_JIT_2D_HISTOGRAMS

// Auxiliar macro to generate all the 2D histogram templates.
#define ADD_HISTO_2D_PAIR_NO_VECTOR(CodeTypeOne,CppTypeOne,CodeTypeTwo,CppTypeTwo) \
    if (type1 == VariableType::CodeTypeOne && type2 == VariableType::CodeTypeTwo) { \
        return node.Histo2D<CppTypeOne, CppTypeTwo, double>(Utils::histoModel2D(variable1, variable2), \
                                              this->systematicVariable(variable1, systematic), \
                                              this->systematicVariable(variable2, systematic), \
                                              this->systematicWeight(systematic)); \
    } \

#define ADD_HISTO_2D_PAIR(CodeTypeOne,CppTypeOne,CodeTypeTwo,CppTypeTwo) \
    if (type1 == VariableType::CodeTypeOne && type2 == VariableType::CodeTypeTwo) { \
        return node.Histo2D<CppTypeOne, CppTypeTwo, double>(Utils::histoModel2D(variable1, variable2), \
                                              this->systematicVariable(variable1, systematic), \
                                              this->systematicVariable(variable2, systematic), \
                                              this->systematicWeight(systematic)); \
    } \
    if (type1 == VariableType::CodeTypeOne && type2 == VariableType::VECTOR_##CodeTypeTwo) { \
        return node.Histo2D<CppTypeOne, std::vector<CppTypeTwo>, double>(Utils::histoModel2D(variable1, variable2), \
                                              this->systematicVariable(variable1, systematic), \
                                              this->systematicVariable(variable2, systematic), \
                                              this->systematicWeight(systematic)); \
    }\

// Support for 2D histograms, do not forget to add your YOUR_NEW_TYPE to the macro below. Template included!
#define ADD_HISTO_2D_SUPPORT_VECTOR(CodeType,CppType) \
/*  ADD_HISTO_2D_PAIR(CodeType,CppType,YOUR_NEW_TYPE,YOUR_NEW_TYPE_C++) */ \
/*  ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,YOUR_NEW_TYPE,YOUR_NEW_TYPE_C++) */ \
    ADD_HISTO_2D_PAIR(CodeType,CppType,CHAR,char) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,CHAR,char) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,INT,int) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,INT,int) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,UNSIGNED_INT,unsigned int) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,UNSIGNED_INT,unsigned int) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,LONG_INT,long long int) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,LONG_INT,long long int) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,UNSIGNED,unsigned long) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,UNSIGNED,unsigned long) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,LONG_UNSIGNED,unsigned long long) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,LONG_UNSIGNED,unsigned long long) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,FLOAT,float) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,FLOAT,float) \
    ADD_HISTO_2D_PAIR(CodeType,CppType,DOUBLE,double) \
    ADD_HISTO_2D_PAIR(VECTOR_##CodeType,std::vector<CppType>,DOUBLE,double) \
    ADD_HISTO_2D_PAIR_NO_VECTOR(CodeType,CppType,BOOL,bool) \
    ADD_HISTO_2D_PAIR_NO_VECTOR(VECTOR_##CodeType,std::vector<CppType>,BOOL,bool) \
    ADD_HISTO_2D_PAIR_NO_VECTOR(BOOL,bool,CodeType,CppType) \
    ADD_HISTO_2D_PAIR_NO_VECTOR(BOOL,bool,VECTOR_##CodeType,std::vector<CppType>)

#endif
