/**
 *  @copyright Copyright 2019 The J-PET Gate Authors. All rights reserved.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  @file GammaPairsAnalysis.h
 */
#ifndef GammaPairsAnalysis_h
#define GammaPairsAnalysis_h
#include "../AnalysisClass/AnalysisClass.h"
#include <map>
#include <string>

/**
 * @About class: This class is an example how user can create its own analysis class. For example in this class analyse eents from pPs decay and collect statistic how pairs of gammas scattered.
 * Example output is: g1:g3 n [ FALSE = k % ] <-- this means: detected n events where one gamma scattered once and the other one scattered 3-times [ k percent of this detected hits are from the hits merging ]
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GammaPairsAnalysis : public AnalysisClass //always remember to inherit from AnalysisClass
{
 public:

  struct ScatterindKind
  {
   unsigned int nTotal = 0;// how many scattering of this kind is in data
   unsigned int nFalse = 0;// how many of this scatterings is false - this means they are from the hits merging
   std::string getStatus() const; //return status inf form "nTotal [ FALSE = 100.0 * nFalse/nTotal % ]"
   void count( bool is_true ); //increase nTotal and if is_true = false additioanly a nFalse counter
  };

  GammaPairsAnalysis();
  GammaPairsAnalysis( std::string class_name ); //This constructor is mandatory to proper registration of analysis class
  virtual ~GammaPairsAnalysis();
  virtual void noticeEvent( const Dictionaries::Event& event ); //place in this method your analysis  - reader will call this method each time for next event
  virtual void displayStatus() const; //this is not mandatory but is useful: at the end of program work you can display analysis status

  private: //some private methods used or this analysis noticeEvent method

  void countThis( unsigned int track_1_hits_number, unsigned int track_2_hits_number, bool is_true );
  bool isNotMergedTrack( const Dictionaries::Track& track ) const;

  private:

  std::map<std::string, ScatterindKind> mKinds;

  unsigned int mEventsNumber = 0;
};
//About: REGISTER_ANALYSIS_CLASS( by this parameter you will call this analysis class in command line, your class name )
REGISTER_ANALYSIS_CLASS(pPsGammasPairsFraction,GammaPairsAnalysis); //This is how analysis classes manager will know which model you want to use

#endif
