// -*- C++ -*-
//
// Package:    RHAnalyser
// Class:      RHAnalyser
// 
/**\class RHAnalyser RHAnalyser.cc Analysis/RHAnalyser/src/RHAnalyser.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Igor Katkov,32 4-A19,+41227676358,
//         Created:  Wed Jan 16 14:14:19 CET 2013
// $Id$
//
//

/***************************/
/**********!!!**************/
// 2011 pp-data
// #define OLD_428_DATA 
// 2011 PbPb-data
// #define OLD_44X_DATA
// Any kind of calibrated DATA
//#define CALIBRATED_DATA 
// for now just a switch to enable checks of saturation flag in the recent SW (can be data or MC)
#define NEW_53X
#define THIS_IS_MC
/**********!!!**************/
/**************************/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <sstream>

//geometry
//
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"

//calorimeters
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
//#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// castor
//#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/CastorObjects/interface/CastorDbRecord.h"
#include "CalibFormats/CastorObjects/interface/CastorDbService.h"

//tfileservice
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//vertex
// #include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// ROOT
#include "TROOT.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
//#include "TTree.h"
//#include "TFile.h"
#include "TLorentzVector.h"
//#include "DataFormats/Math/interface/LorentzVector.h"
#include "TText.h"

namespace ForwardRecord {
  
  const uint nbEtaBins = 16;
  const double Eta_Bin_Edges[nbEtaBins+1] = {
    -5.19, 
    -4.89, 
    -4.54, 
    -4.19, 
    -3.84, 
    -3.49,
    -3.14,
    -2.65,
    0.00,
    2.65, 
    3.14, 
    3.49, 
    3.84, 
    4.19, 
    4.54, 
    4.89, 
    5.19
  };

  const unsigned int CSectors = 16;
  const unsigned int CModules = 14;
#ifdef OLD_428_DATA
  const double absCasEscaleFactor = 0.015; 
#else
  const double absCasEscaleFactor = 0.020; //0.015 // 0.020 +/-0.005 //0.025 <-> 1.49475628341732545e+02/3.38945322418397409e+04 * 7.82961248095532028e+02/1.33096168279108213e+02; //~0.025
#endif

} //end namespace ForwardRecord

namespace castor {

  //E-map swap: s5m10<->s5m12, s6m10<->s6m12, s7m10<->s7m12
  //using for Katerina's value's (some correction values seem too big (for me big is all >3), some are zeros)
  const bool channelQuality[ForwardRecord::CSectors][ForwardRecord::CModules] =             // sector
    //mod 1   2     3     4     5      6    7     8     9    10     11    12   13    14
    {{true ,true ,true ,false,true ,true ,false,true ,true ,true ,true ,true ,true ,true }, // 1
     {true ,true ,true ,true ,true ,true ,false,true ,true ,true ,true ,true ,true ,true }, // 2
     {true ,true ,true ,true ,true ,true ,false,false,false,true ,false,true ,true ,true }, // 3 //s3m9 ? 
     {true ,true ,true ,true ,true ,true ,false,false,false,true ,false,true ,false,true }, // 4
     {true ,false,true ,true ,true ,true ,false,false,false,false,true ,false,true ,true }, // 5
     {true ,true ,true ,true ,true ,true ,false,false,false,false,true ,false,true ,true }, // 6 //s6m9 ?
     {true ,true ,true ,true ,true ,true ,false,true ,true ,false,false,false,false,false}, // 7 //s7m10-14katerina?
     {true ,true ,true ,true ,true ,true ,false,false,true ,false,false,false,false,false}, // 8 //s8m10-14katerina?
     {true ,true ,true ,true ,true ,true ,false,true ,true ,true ,true ,true ,false,true }, // 9 //s9m13?
     {true ,true ,true ,true ,true ,true ,false,true ,true ,true ,true ,true ,true ,true }, // 10
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,true }, // 11 //s11m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,true }, // 12 //s12m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,false,false,false,false,true ,false,true ,false,true }, // 13 //s13m11katerina?(was also there before) //m9s13-SNP//m13s13-SNP
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,true }, // 14 //s14m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,true ,false,false,true ,false,true ,true ,true ,true }, // 15
     {true ,true ,true ,true ,true ,false,false,false,true ,true ,true ,true ,true ,true }};// 16 //m8s16-strange-noise-peak(SNP),closer look needed

#ifdef OLD_428_DATA
  //prompt-reco rechits most likely include already intercalibration (but likely not the em-factor 2); why do you try to apply it once again?
  const double channelIntercalibCorr[ForwardRecord::CSectors][ForwardRecord::CModules] =
    {{     1.3701 ,    1.4709 ,    0.6117 ,    0.8972 ,    0.4640 ,    0.1848 ,    9.7444 ,    0.3365 ,    0.2572 ,    0.4714 ,    0.3240 ,    0.1967 ,    0.3084 ,    0.1645  },
     {     1.2265 ,    1.4897 ,    0.7495 ,    0.7318 ,    0.3955 ,    0.3050 ,    5.5303 ,    2.0929 ,    0.2538 ,    0.4026 ,    0.4191 ,    0.2082 ,    0.3169 ,    0.1957  },
     {     1.0166 ,    1.2438 ,    0.8025 ,    1.9026 ,    0.2362 ,    0.3031 ,   17.5698 ,    0.0000 ,    0.8806 ,    0.3689 ,    0.5344 ,    0.4912 ,    0.3583 ,    0.3073  },
     {     0.9056 ,    0.9166 ,    0.8326 ,    1.1412 ,    0.3070 ,    0.4375 ,   16.5410 ,    0.0000 ,    0.0000 ,    0.6522 ,    0.4035 ,    0.2690 ,    0.1471 ,    0.2754  },
     {     0.9935 ,    1.2291 ,    0.9083 ,    1.0373 ,    0.3749 ,    0.5002 ,    4.2422 ,    0.0000 ,    0.0000 ,    4.1239 ,    0.3159 ,    0.7048 ,    0.5351 ,    0.4954  },
     {     0.7776 ,    1.0104 ,    0.7956 ,    1.4532 ,    0.2361 ,    0.4289 ,   44.9370 ,    0.0000 ,    0.7804 ,    0.5092 ,    0.2327 ,    0.8089 ,    0.6899 ,    0.5420  },
     {     0.8596 ,    1.1765 ,    1.1511 ,    0.8430 ,    0.6682 ,    1.8405 ,    0.0000 ,    0.4535 ,    0.6307 ,    0.5989 ,    0.6247 ,    0.0000 ,    0.7020 ,    0.4745  },
     {     1.2867 ,    1.2193 ,    1.3185 ,    1.2524 ,    0.5472 ,    0.2482 ,   32.2495 ,    0.3334 ,    0.5916 ,    0.7465 ,    0.6092 ,    0.3063 ,    0.7467 ,    0.2268  },
     {     1.0147 ,    1.2313 ,    0.9716 ,    1.0000 ,    0.2104 ,    1.1529 ,   20.1270 ,    0.2144 ,    0.4552 ,    0.3849 ,    0.2506 ,    0.3448 ,    0.3437 ,    0.4677  },
     {     0.8099 ,    1.1048 ,    1.5190 ,    1.3555 ,    0.2286 ,    1.2479 ,   69.4715 ,    0.2426 ,    0.4942 ,    0.7211 ,    0.2442 ,    0.4468 ,    0.4218 ,    0.3345  },
     {     1.6824 ,    2.3471 ,    1.5481 ,    0.9906 ,    0.2119 ,    1.0717 ,   17.8984 ,   44.3873 ,    0.5349 ,    0.5173 ,   12.8141 ,    0.5538 ,    0.7995 ,    0.4327  },
     {     1.7215 ,    1.2024 ,    1.1115 ,    1.2943 ,    0.4238 ,    1.4140 ,    0.0000 ,    0.0000 ,    0.9465 ,    0.5723 ,   13.4895 ,    0.4727 ,    0.2425 ,    0.3038  },
     {     0.4439 ,    1.3415 ,    0.9335 ,    0.6067 ,    0.1940 ,    2.4494 ,    0.0000 ,    0.0000 ,    0.0000 ,    0.4774 ,   22.2232 ,    0.4806 ,    0.0000 ,    0.3125  },
     {     0.5853 ,    1.4652 ,    0.9517 ,    1.1912 ,    0.2749 ,    0.5013 ,    0.0000 ,    0.0000 ,    0.5274 ,    0.2801 ,   10.3582 ,    0.3733 ,    0.4314 ,    0.3029  },
     {     1.1327 ,    1.4487 ,    0.9568 ,    0.7324 ,    0.3609 ,    0.2590 ,    0.0000 ,    0.0000 ,    0.2626 ,    0.2741 ,    0.2826 ,    0.3635 ,    0.3436 ,    0.3950  },
     {     1.6316 ,    1.0836 ,    1.1930 ,    0.7702 ,    0.2002 ,    2.0732 ,   51.0338 ,    0.0000 ,    0.2069 ,    0.4475 ,    0.2795 ,    0.2044 ,    0.3467 ,    0.2486  }};

  const double channelEMCorr[ForwardRecord::CSectors][ForwardRecord::CModules] =
    {{     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  },
     {     0.5000 ,    0.5000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000 ,    1.0000  }};
#elif defined OLD_44X_DATA
//for the next two tables you have problems with swapped channels in the e-map: tables assume corrected e-map, so get rid of the channels in the bad channel list above:
// s5m10<->s5m12, s6m10<->s6m12, s7m10<->s7m12
//for runs >=181530 && <=181603 
  const double channelFullCalibrationHI1[ForwardRecord::CSectors][ForwardRecord::CModules] =
//       mod1            2            3            4            5            6            7            8            9           10           11           12           13           14
{{     0.18495,     0.11952,     0.37649,     0.14166,     0.25474,     0.46429,     0.39233,     0.22713,     0.10552,     0.11956,     0.09858,     0.11220,     0.09621,     0.14243}, // s 1
 {     0.10485,     0.13385,     0.31328,     0.38071,     0.21649,     0.52951,     0.28841,     0.39027,     0.12423,     0.18388,     0.12229,     0.12109,     0.09886,     0.17016}, // s 2
 {     0.22034,     0.12004,     0.33844,     0.11442,     0.48180,     1.20719,     0.00000,     0.00000,     0.18567,     0.09967,     0.09804,     0.11879,     0.07494,     0.18650}, // s 3
 {     0.18419,     0.04722,     0.18544,     0.12960,     0.23316,     0.65600,     0.73333,     0.00000,     0.00000,     0.13856,     0.08191,     0.14840,     0.09660,     0.17110}, // s 4
 {     0.18037,     0.19413,     0.25994,     0.11838,     0.12625,     0.09974,     0.45542,    -0.00000,     0.00000,     0.00456,     0.03951,     0.14642,     0.02530,     0.05922}, // s 5
 {     0.16242,     0.14450,     0.25352,     0.12882,     0.14926,     0.14460,     0.00000,    -0.00000,     0.03382,     0.06135,     0.03623,     0.01288,     0.02831,     0.06928}, // s 6
 {     0.15914,     0.18249,     0.09443,     0.16489,     0.14572,     0.03201,    -0.00000,     0.31665,     0.03391,     0.00000,     0.04154,     0.01179,     0.02974,     0.06963}, // s 7
 {     0.13352,     0.09071,     0.15396,     0.11714,     0.21436,     0.02758,     0.00000,     0.25105,     0.02596,     0.03165,     0.04870,     0.03363,     0.04201,     0.06358}, // s 8
 {     0.13137,     0.12946,     0.20657,     0.15690,     0.83175,     0.03946,     0.00000,     0.33162,     0.08513,     0.12195,     0.19210,     0.12288,     0.14428,     0.10421}, // s 9
 {     0.15101,     0.17270,     0.19651,     0.16710,     0.80184,     0.11002,     0.00000,     0.32877,     0.11603,     0.11513,     0.21953,     0.13693,     0.17226,     0.12786}, // s10
 {     0.12553,     0.11964,     0.18151,     0.17484,     0.45408,     0.08196,     0.00000,     0.00000,     0.10772,     0.13814,     0.10941,     0.10977,     0.12545,     0.12008}, // s11
 {     0.10688,     0.11094,     0.19901,     0.17168,     0.78103,     0.17369,     0.00000,     0.00000,     0.12530,     0.10031,     0.18125,     0.11565,     0.15497,     0.11406}, // s12
 {     0.24803,     0.12523,     0.37815,     0.23653,     0.66186,     0.26966,     0.00000,     0.00000,     0.00000,     0.10463,     0.12428,     0.09990,     0.00000,     0.22826}, // s13
 {     0.25662,     0.13909,     0.23652,     0.34411,     0.74027,     0.22282,     0.00000,    -0.00000,     0.11054,     0.12363,     0.07838,     0.10696,     0.12513,     0.19703}, // s14
 {     0.18672,     0.18555,     0.10587,     0.28700,     0.41646,     0.32892,     0.00000,    -0.00000,     0.11059,     0.10379,     0.07880,     0.09307,     0.07529,     0.14927}, // s15
 {     0.12822,     0.14803,     0.39631,     0.26655,     0.52847,     0.26153,     0.00000,     0.00000,     0.11174,     0.13799,     0.08952,     0.10944,     0.10081,     0.13628}};// s16
 
//for runs >=181604 && <=183126
const double channelFullCalibrationHI2[ForwardRecord::CSectors][ForwardRecord::CModules] =
//       mod1            2            3            4            5            6            7            8            9           10           11           12           13           14
{{     0.18612,     0.10041,     0.37486,     0.14133,     0.35172,     0.45087,     0.45185,     0.23070,     0.10587,     0.12015,     0.09898,     0.11312,     0.09582,     0.14146}, // s 1
 {     0.10558,     0.11418,     0.31181,     0.37948,     0.29684,     0.50754,     0.26382,     0.39027,     0.12463,     0.18184,     0.12276,     0.12070,     0.09808,     0.17082}, // s 2
 {     0.13348,     0.23710,     0.33720,     0.11479,     0.65876,     1.19037,     0.00000,     0.00000,     0.18567,     0.09959,     0.09725,     0.11940,     0.07485,     0.18480}, // s 3
 {     0.11197,     0.09410,     0.18472,     0.13004,     0.32342,     0.64023,     0.79802,     0.00000,     0.00000,     0.13842,     0.08109,     0.14751,     0.09701,     0.17055}, // s 4
 {     0.18037,     0.19315,     0.26181,     0.11838,     0.12443,     0.09550,     0.42714,    -0.00000,     0.00000,     0.00453,     0.03928,     0.14855,     0.02519,     0.05896}, // s 5
 {     0.16242,     0.14371,     0.25176,     0.12882,     0.14858,     0.14341,     0.00000,     0.00000,     0.03441,     0.06172,     0.03642,     0.01288,     0.02819,     0.06895}, // s 6
 {     0.15984,     0.18156,     0.09443,     0.16489,     0.14635,     0.03236,    -0.00000,     0.31709,     0.03423,     0.00000,     0.04186,     0.01183,     0.02967,     0.06978}, // s 7
 {     0.13352,     0.09120,     0.15396,     0.11817,     0.21436,     0.02681,     0.00000,     0.25228,     0.02596,     0.03186,     0.04895,     0.03386,     0.04219,     0.06402}, // s 8
 {     0.13137,     0.12946,     0.20821,     0.15740,     0.83508,     0.03929,     0.00000,     0.33018,     0.08482,     0.12195,     0.19326,     0.12375,     0.14428,     0.10421}, // s 9
 {     0.15002,     0.17161,     0.19651,     0.16651,     0.80009,     0.10986,     0.00000,     0.32754,     0.11560,     0.11430,     0.21953,     0.13597,     0.17065,     0.12786}, // s10
 {     0.12262,     0.11985,     0.18113,     0.17484,     0.45191,     0.08110,     0.00000,     0.00000,     0.10772,     0.13719,     0.10941,     0.10901,     0.12485,     0.12008}, // s11
 {     0.10444,     0.11119,     0.19946,     0.17121,     0.77631,     0.17539,     0.00000,     0.00000,     0.12573,     0.10031,     0.18125,     0.11565,     0.15423,     0.11327}, // s12
 {     0.24803,     0.12486,     0.37815,     0.23653,     0.65000,     0.26966,     0.00000,     0.00000,     0.00000,     0.10494,     0.12428,     0.09921,     0.00000,     0.22941}, // s13
 {     0.25662,     0.13984,     0.23652,     0.34276,     0.73554,     0.21843,    -0.00000,     0.00000,     0.11054,     0.12353,     0.07846,     0.10611,     0.12425,     0.19604}, // s14
 {     0.18672,     0.18755,     0.10629,     0.28618,     0.41646,     0.31653,     0.00000,    -0.00000,     0.11059,     0.10390,     0.07926,     0.09334,     0.07547,     0.15018}, // s15
 {     0.12877,     0.14950,     0.39765,     0.26655,     0.53047,     0.25053,     0.00000,     0.00000,     0.11174,     0.13815,     0.08934,     0.10944,     0.10101,     0.13737}};// s16
#else
  // Katerina's values using halo muon data (w/o TOTEM), already scaled by s9m4
  const double channelGainQE[ForwardRecord::CSectors][ForwardRecord::CModules] =                                                                                // sector
    //mod 1          2          3          4           5          6          7         8          9          10         11          12        13           14
    {{  0.7510,    0.8700,    2.7370,    0.0000,    0.3630,    0.6430,    0.0000,    0.3100,    0.2120,    0.2740,    0.3030,    0.1690,    0.2650,    0.1550}, //1
     {  0.6190,    0.6160,    1.8130,    0.8690,    0.1820,    0.6280,    0.0000,    0.5070,    0.1680,    0.2910,    0.3380,    0.1460,    0.2490,    0.1250}, //2
     {  1.0700,    0.6510,    1.4250,    0.7660,    0.3040,    0.1930,    8.2170,   13.2900,    0.4650,    0.2350,    0.0000,    0.2950,    0.3430,    0.3510}, //3
     {  0.5310,    0.3300,    0.8910,    0.8260,    0.1170,    0.3300,    0.0000,    0.0000,    0.0000,    0.6390,    0.0000,    0.3170,    0.0000,    0.4580}, //4
     {  0.6120,    0.0000,    1.3410,    0.7020,    0.1560,    0.5690,    0.8360,    0.0000,    0.0000,    0.5230,    0.2360,    0.3290,    0.3990,    0.3420}, //5
     {  1.3130,    0.4870,    1.4000,    0.6320,    0.1990,    0.7950,    1.2090,    0.0000,    0.5100,    0.7060,    0.2330,    0.2800,    0.4830,    0.4410}, //6
     {  0.4160,    0.2820,    1.0430,    0.3130,    0.1140,    0.0860,  250.6690,    0.1950,    0.4200,    6.9160,    3.4790,    1.5110,    4.8590,    3.5340}, //7
     {  0.3420,    0.2950,    1.1980,    1.4030,    0.2130,    1.0730,    0.0000,    0.2060,    0.6350,   27.2690,    9.4210,    3.3400,    3.4880,    1.0100}, //8
     {  0.3030,    0.8460,    1.4120,    1.0000,    0.2180,    0.8830,    0.0000,    0.1320,    0.1950,    0.2490,    0.2250,    0.2270,    0.2990,    0.2780}, //9
     {  0.9040,    1.4030,    2.6580,    1.1900,    0.2350,    1.5570,    0.0000,    0.3160,    0.1990,    0.3100,    0.1790,    0.2510,    0.2510,    0.2520}, //10
     {  1.0160,    0.9930,    1.6950,    0.8870,    0.2850,    0.6230,    0.0000,   10.0790,    0.3730,    0.2440,    9.6350,    0.5240,    0.6990,    0.3790}, //11
     {  1.1690,    1.1300,    2.1400,    1.3920,    0.2630,    1.2470,    0.0000,    0.0000,    0.5670,    0.3030,   99.3510,    0.3510,    0.1980,    0.3560}, //12
     {  0.9160,    1.2700,    1.6430,    0.8070,    0.2310,    2.3020,    0.0000,    0.0000,    0.3230,    0.2910,    0.0000,    0.3430,    0.1280,    0.3080}, //13
     {  0.6010,    0.9840,    2.1400,    0.8210,    0.1770,    1.0970,    0.0000,    0.0000,    0.2030,    0.2920,   16.6350,    0.3020,    0.3510,    0.3680}, //14
     {  0.7590,    1.3650,    2.9620,    1.1740,    0.3800,    2.3370,    0.0000,  517.2540,    0.2690,    0.0000,    0.1940,    0.2740,    0.2800,    0.4100}, //15
     {  0.7420,    0.9720,    2.4600,    0.9240,    0.2200,    0.1630,    3.9070,    0.1970,    0.2700,    0.2580,    0.1510,    0.1340,    0.2790,    0.2620}};//16
#endif

}

//
// class declaration
//

class RHAnalyser : public edm::EDAnalyzer {
public:
  explicit RHAnalyser(const edm::ParameterSet&);
  ~RHAnalyser();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  // ----------member data ---------------------------

  struct MonVariables
  {
 
    uint runNb ;
    uint evtNb ;
    uint bxNb ;
    uint orbitNb ;
    uint lumiNb ;     

    uint trkNb1;
    uint hfNb;
    uint hfmNb4;
    double hfmE;
    uint hfpNb4;
    double hfpE;
    uint vtxNb;
    uint vtxNbfake;
    uint vtx1TrkNb;
    double vtx1Z;
    double vtx1Zerr;
    double vtx1Y;
    double vtx1X;

    int nbCasRecHits;
    double casRecHitEnergy[224];
    int    casRecHitIphi[224];
    int    casRecHitIdepth[224];
    int    casRecHitSaturation[224];
    int    casRecHitReSaturation[224];

   /*
      int nbZDCRecHits;
      double zdcRecHitEnergy[18];
      int    zdcRecHitIside[18];
      int    zdcRecHitIsection [18];
      int    zdcRecHitIchannel[18];
      int    zdcRecHitSaturation[18];

      int nbZDCDigis;
      int nbZDCDigiTs;
      double zdcDigiEnergyFC[18][10];
      int    zdcDigiEnergyADC[18][10];
      int    zdcDigiIside[18];
      int    zdcDigiIsection[18];
      int    zdcDigiIchannel[18];

    */
    
  };

  edm::Service<TFileService> fs_;

  const CaloGeometry* geo;

  bool _ShowDebug;
  edm::InputTag _VtxSrc;
  edm::InputTag _TrkSrc;
  edm::InputTag _GenPartSrc;
  edm::InputTag _PFSrc;

  TTree* rhtree_;

  MonVariables treeVariables_;

  TProfile* energy_vs_eta_reco_;
  TProfile* et_vs_eta_reco_;
  TProfile* towet_vs_eta_reco_;
  TH2D* tow_ene_map_;
  TH2D* tow_occup_map_;
  TProfile* pfet_vs_eta_reco_;
  TProfile* genet_vs_eta_reco_;
  std::vector<TH1D*> etaBinEnergies_;
  std::vector<TH1D*> etaBinEts_;
  std::vector<TH1D*> etaBinTowEts_;
  std::vector<TH1D*> etaBinPFEts_;
  std::vector<TH1D*> etaBinGenEts_;
  std::vector<TH1D*> etaBinEnergies1trk_;
  std::vector<TH1D*> etaBinEnergies1trkL_;
  std::vector<TH1D*> etaBinEnergies1trkS_;
  std::vector<TH1D*> etaBinEnergies1trk4_;
  std::vector<TH1D*> etaBinEnergiesBX_;
  std::vector<TH1D*> etaBinEnergiesNSD_;
  TH1D* hf_resp_accum_;
  TH1D* hf_resp_occup_accum_;
  std::vector<double> runningSampleEnergySum_;
  std::vector<int> runningSamplesNo_;

  TH1D* track_no_;
  TH1D* track_pt_;
  TH1D* track_eta_;
  TH1D* track_phi_;
  TH1D* vtx_no_;
  TH1D* vtx_nofk_;
  TH1D* vtx1_notrk_;
  TH1D* vtx1_x_;
  TH1D* vtx1_y_;
  TH1D* vtx1_z_;
  TH1D* vtx1_zerr_;
  TH1D* cas_etot_;
  TH1D* cas_etot1114_;
  TH1D* cas_etot15_;
  TH1D* cas_etotbx_;
  TH1D* cas_etot1114bx_;
  TProfile* cas_zprofile_;
  TProfile* cas_phiprofile_;
  TProfile* cas_phiprofile15_;
  TProfile* cas_phiprofile1114_;
  TProfile* cas_chprofile_;

  TH1D* zdc_chno_satflg_;
  TH1D* zdcm_etot_;
  TH1D* zdcp_etot_;
  TH2D* zdcm_em_vs_tot_;
  TH2D* zdcp_em_vs_tot_;
  TH2D* trkno_vs_hfme_;
  TH2D* hfme_vs_case_;
  TH2D* case_vs_zdcme_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RHAnalyser::RHAnalyser(const edm::ParameterSet& iConfig) :
  _ShowDebug ( iConfig.getUntrackedParameter<bool>("showDebug",true) ),
  _VtxSrc ( iConfig.getUntrackedParameter<edm::InputTag>("vtxSrc",edm::InputTag("offlinePrimaryVertices")) ), 
  //pp: "offlinePrimaryVertices"//HI: "hiSelectedVertex"
  _TrkSrc ( iConfig.getUntrackedParameter<edm::InputTag>("trkSrc",edm::InputTag("generalTracks")) ),
  //pp: "generalTracks" //OldHI44X: "hiSelectedTracks" //NewHI53X: "hiGeneralTracks"
  _GenPartSrc ( iConfig.getUntrackedParameter<edm::InputTag>("genPartSrc",edm::InputTag("genParticles")) ),
  //pp && pPb: "genParticles" //HI: "hiGenParticles"
  _PFSrc ( iConfig.getUntrackedParameter<edm::InputTag>("pfSrc",edm::InputTag("particleFlow")) ),
  //pp && pPb: "particleFlow" //HI: "particleFlowTmp"
  runningSampleEnergySum_(6,0.0),
  runningSamplesNo_(6,0) 
{
  //now do what ever initialization is needed

  rhtree_ = fs_->make<TTree>("RHTree","RHTree"); 

  rhtree_->Branch("runNb",&treeVariables_.runNb,"runNb/i"); //
  rhtree_->Branch("evtNb",&treeVariables_.evtNb,"evtNb/i"); //
  rhtree_->Branch("bxNb",&treeVariables_.bxNb,"bxNb/i"); //
  rhtree_->Branch("orbitNb",&treeVariables_.orbitNb,"orbitNb/i"); //
  rhtree_->Branch("lumiNb",&treeVariables_.lumiNb,"lumiNb/i"); //

  rhtree_->Branch("trkNb1",&treeVariables_.trkNb1,"trkNb1/i");
  rhtree_->Branch("hfNb",&treeVariables_.hfNb,"hfNb/i");
  rhtree_->Branch("hfmNb4",&treeVariables_.hfmNb4,"hfmNb4/i");
  rhtree_->Branch("hfmE",&treeVariables_.hfmE,"hfmE/D");
  rhtree_->Branch("hfpNb4",&treeVariables_.hfpNb4,"hfpNb4/i");
  rhtree_->Branch("hfpE",&treeVariables_.hfpE,"hfpE/D");
  rhtree_->Branch("vtxNb",&treeVariables_.vtxNb,"vtxNb/i");
  rhtree_->Branch("vtxNbfake",&treeVariables_.vtxNbfake,"vtxNbfake/i");
  rhtree_->Branch("vtx1TrkNb",&treeVariables_.vtx1TrkNb,"vtx1TrkNb/i");
  rhtree_->Branch("vtx1Z",&treeVariables_.vtx1Z,"vtx1Z/D");
  rhtree_->Branch("vtx1Zerr",&treeVariables_.vtx1Zerr,"vtx1Zerr/D");
  rhtree_->Branch("vtx1Y",&treeVariables_.vtx1Y,"vtx1Y/D");
  rhtree_->Branch("vtx1X",&treeVariables_.vtx1X,"vtx1X/D");

  rhtree_->Branch("nbCasRecHits",&treeVariables_.nbCasRecHits,"nbCasRecHits/i");
  rhtree_->Branch("casRecHitEnergy",treeVariables_.casRecHitEnergy,"casRecHitEnergy[nbCasRecHits]/D");
  rhtree_->Branch("casRecHitIphi",treeVariables_.casRecHitIphi,"casRecHitIphi[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitIdepth",treeVariables_.casRecHitIdepth,"casRecHitIdepth[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitSaturation",treeVariables_.casRecHitSaturation,"casRecHitSaturation[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitReSaturation",treeVariables_.casRecHitReSaturation,"casRecHitReSaturation[nbCasRecHits]/I");

  /*
    rhtree_->Branch("nbZDCRecHits",&treeVariables_.nbZDCRecHits,"nbZDCRecHits/i");
    rhtree_->Branch("zdcRecHitEnergy",treeVariables_.zdcRecHitEnergy,"zdcRecHitEnergy[nbZDCRecHits]/D");
    rhtree_->Branch("zdcRecHitIside",treeVariables_.zdcRecHitIside,"zdcRecHitIside[nbZDCRecHits]/I");
    rhtree_->Branch("zdcRecHitIsection",treeVariables_.zdcRecHitIsection,"zdcRecHitIsection[nbZDCRecHits]/I");
    rhtree_->Branch("zdcRecHitIchannel",treeVariables_.zdcRecHitIchannel,"zdcRecHitIchannel[nbZDCRecHits]/I");
    rhtree_->Branch("zdcRecHitSaturation",treeVariables_.zdcRecHitSaturation,"zdcRecHitSaturation[nbZDCRecHits]/I");

    rhtree_->Branch("nbZDCDigis",&treeVariables_.nbZDCDigis,"nbZDCDigis/i");
    rhtree_->Branch("nbZDCDigiTs",&treeVariables_.nbZDCDigiTs,"nbZDCDigiTs/i");
    rhtree_->Branch("zdcDigiIside",treeVariables_.zdcDigiIside,"zdcDigiIside[nbZDCDigis]/I");
    rhtree_->Branch("zdcDigiIsection",treeVariables_.zdcDigiIsection,"zdcDigiIsection[nbZDCDigis]/I");
    rhtree_->Branch("zdcDigiIchannel",treeVariables_.zdcDigiIchannel,"zdcDigiIchannel[nbZDCDigis]/I");
    rhtree_->Branch("zdcDigiEnergyFC",treeVariables_.zdcDigiEnergyFC,"zdcDigiEnergyFC[nbZDCDigis][nbZDCDigiTs]/D");
    rhtree_->Branch("zdcDigiEnergyADC",treeVariables_.zdcDigiEnergyADC,"zdcDigiEnergyADC[nbZDCDigis][nbZDCDigiTs]/I");
  
  */

  energy_vs_eta_reco_ = fs_->make<TProfile>("energy_vs_eta_reco","Detector-level <Energy> vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges);
  energy_vs_eta_reco_->Sumw2();
  energy_vs_eta_reco_->SetXTitle("#eta");
  energy_vs_eta_reco_->SetYTitle("<E>/#Delta#eta");
  et_vs_eta_reco_ = fs_->make<TProfile>("et_vs_eta_reco","Detector-level <Et> vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges);
  et_vs_eta_reco_->Sumw2();
  et_vs_eta_reco_->SetXTitle("#eta");
  et_vs_eta_reco_->SetYTitle("<Et>/#Delta#eta");
  towet_vs_eta_reco_ = fs_->make<TProfile>("towet_vs_eta_reco","Detector-level tower <Et> vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges);
  towet_vs_eta_reco_->Sumw2();
  towet_vs_eta_reco_->SetXTitle("#eta");
  towet_vs_eta_reco_->SetYTitle("<Et>/#Delta#eta");
  tow_ene_map_ = fs_->make<TH2D>("tow_ene_map","ieta-iphi map of tower energies",199,-99.5,99.5,100,-0.5,99.5);
  tow_ene_map_->SetXTitle("i#eta");
  tow_ene_map_->SetYTitle("i#phi");
  tow_occup_map_=fs_->make<TH2D>("tow_occup_map","Number of towers above threshold as an ieta-iphi map",199,-99.5,99.5,100,-0.5,99.5);
  tow_occup_map_->SetXTitle("i#eta");
  tow_occup_map_->SetYTitle("i#phi");
  pfet_vs_eta_reco_ = fs_->make<TProfile>("pfet_vs_eta_reco","Detector-level PF <Et> vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges);
  pfet_vs_eta_reco_->Sumw2();
  pfet_vs_eta_reco_->SetXTitle("#eta");
  pfet_vs_eta_reco_->SetYTitle("<Et>/#Delta#eta");  
  genet_vs_eta_reco_ = fs_->make<TProfile>("genet_vs_eta_reco","Particle-level <Et> vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges);
  genet_vs_eta_reco_->Sumw2();
  genet_vs_eta_reco_->SetXTitle("#eta");
  genet_vs_eta_reco_->SetYTitle("<Et>/#Delta#eta");  
  etaBinEnergies_.reserve(ForwardRecord::nbEtaBins);
  etaBinEts_.reserve(ForwardRecord::nbEtaBins);
  etaBinTowEts_.reserve(ForwardRecord::nbEtaBins);
  etaBinPFEts_.reserve(ForwardRecord::nbEtaBins);
  etaBinGenEts_.reserve(ForwardRecord::nbEtaBins);
  etaBinEnergies1trk_.reserve(ForwardRecord::nbEtaBins);
  etaBinEnergies1trkL_.reserve(ForwardRecord::nbEtaBins);
  etaBinEnergies1trkS_.reserve(ForwardRecord::nbEtaBins);
  etaBinEnergiesBX_.reserve(ForwardRecord::nbEtaBins);
  etaBinEnergiesNSD_.reserve(ForwardRecord::nbEtaBins);
  for(unsigned int ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++){
    TH1D* theHisto = fs_->make<TH1D>(Form("energy_reco_%d",ibin),
				     Form("Detector-level Energy in #eta %g:%g", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				     96000,-12000.0,36000.0);//2020,-136.50,13635.00);
    theHisto->Sumw2();
    theHisto->SetYTitle("Entries");
    theHisto->SetXTitle("Energy");
    theHisto->GetYaxis()->SetTitleOffset(1.7);
    theHisto->GetYaxis()->SetNoExponent();
    theHisto->GetXaxis()->SetNdivisions(505);
    etaBinEnergies_.push_back(theHisto);
    TH1D* theHistoAlike = fs_->make<TH1D>(Form("et_reco_%d",ibin),
					  Form("Detector-level Et in #eta %g:%g", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
					  96000,-12000.0,36000.0);
    etaBinEts_.push_back(theHistoAlike);
    TH1D* theHistoAlike1 = fs_->make<TH1D>(Form("towet_reco_%d",ibin),
					   Form("Detector-level tower Et in #eta %g:%g", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
					   96000,-12000.0,36000.0);
    etaBinTowEts_.push_back(theHistoAlike1);
    TH1D* theHistoAlike2 = fs_->make<TH1D>(Form("pfet_reco_%d",ibin),
					   Form("Detector-level PF Et in #eta %g:%g", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
					   96000,-12000.0,36000.0);
    etaBinPFEts_.push_back(theHistoAlike2);    
    TH1D* theHistoAlike3 = fs_->make<TH1D>(Form("genet_reco_%d",ibin),
					   Form("Particle-level Et in #eta %g:%g", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
					   96000,-12000.0,36000.0);
    etaBinGenEts_.push_back(theHistoAlike3);    
    TH1D* theHisto1 = fs_->make<TH1D>(Form("energy_reco_%d_1trk",ibin),
				      Form("Detector-level Energy in #eta %g:%g (1trk)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    //(TH1D*)theHisto->Clone(Form("energy_reco_%d_1trk",ibin)); 
    theHisto1->Sumw2();
    etaBinEnergies1trk_.push_back(theHisto1);
    TH1D* theHisto2 = fs_->make<TH1D>(Form("energy_reco_%d_1trkL",ibin),
				      Form("Detector-level Energy in #eta %g:%g (1trk&L)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    //(TH1D*)theHisto->Clone(Form("energy_reco_%d_1trkL",ibin));
    theHisto2->Sumw2();
    etaBinEnergies1trkL_.push_back(theHisto2);
    TH1D* theHisto3 = fs_->make<TH1D>(Form("energy_reco_%d_1trkS",ibin),
				      Form("Detector-level Energy in #eta %g:%g (1trk&S)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    //(TH1D*)theHisto->Clone(Form("energy_reco_%d_1trkS",ibin));
    theHisto3->Sumw2();
    etaBinEnergies1trkS_.push_back(theHisto3);
    TH1D* theHisto4 = fs_->make<TH1D>(Form("energy_reco_%d_1trk4",ibin),
				      Form("Detector-level Energy in #eta %g:%g (1trk&4)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    //(TH1D*)theHisto->Clone(Form("energy_reco_%d_1trk4",ibin));
    theHisto4->Sumw2();
    etaBinEnergies1trk4_.push_back(theHisto4);
    TH1D* theHisto5 = fs_->make<TH1D>(Form("energy_reco_%d_bx",ibin),
				      Form("Detector-level Energy in #eta %g:%g (bx)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    theHisto5->Sumw2();
    etaBinEnergiesBX_.push_back(theHisto5);
    TH1D* theHisto6 = fs_->make<TH1D>(Form("energy_reco_%d_nsd",ibin),
				      Form("Detector-level Energy in #eta %g:%g (NSD)", ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]),
				      96000,-12000.0,36000.0);
    theHisto6->Sumw2();
    etaBinEnergiesNSD_.push_back(theHisto6);
  }  

  hf_resp_accum_ = fs_->make<TH1D>("hf_resp_accum","HF sum of responses", 6 , 0.5, 6.5); //
  hf_resp_accum_->Sumw2();
  hf_resp_accum_->GetXaxis()->SetBinLabel(1,"RHe34");
  hf_resp_accum_->GetXaxis()->SetBinLabel(2,"RHe45");
  hf_resp_accum_->GetXaxis()->SetBinLabel(3,"RHet34");
  hf_resp_accum_->GetXaxis()->SetBinLabel(4,"RHet45");
  hf_resp_accum_->GetXaxis()->SetBinLabel(5,"TowEt34");
  hf_resp_accum_->GetXaxis()->SetBinLabel(6,"TowEt45");

  hf_resp_occup_accum_ = fs_->make<TH1D>("hf_resp_occup_accum","HF sum of occupancies", 6 , 0.5, 6.5); //
  hf_resp_occup_accum_->Sumw2();
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(1,"RHe34");
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(2,"RHe45");
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(3,"RHet34");
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(4,"RHet45");
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(5,"TowEt34");
  hf_resp_occup_accum_->GetXaxis()->SetBinLabel(6,"TowEt45");

  //high quality track pt histo!!!
  track_no_ = fs_->make<TH1D>("track_no","High purity tracks with p_{t} > 1 GeV",300,-0.5,299.5);
  track_no_->Sumw2();
  track_no_->SetXTitle("Number of tracks");
  track_pt_ = fs_->make<TH1D>("track_pt","High purity track p_{t}",220,-1.0,10.0);
  track_pt_->Sumw2();  
  track_pt_->SetXTitle("p_{t} (GeV)");
  track_eta_ = fs_->make<TH1D>("track_eta","High purity track #eta",15,-3.0,3.0);
  track_eta_->Sumw2();
  track_eta_->SetXTitle("#eta (GeV)");
  track_phi_ = fs_->make<TH1D>("track_phi","High purity track #phi",50,-Geom::pi(),Geom::pi());
  track_phi_->Sumw2();
  track_phi_->SetXTitle("#phi (GeV)");
  //vertex (on top of tree variables)
  vtx_no_ = fs_->make<TH1D>("vtx_no","Size of vertex collection",10,-0.5,9.5);
  vtx_no_->Sumw2();
  vtx_no_->SetXTitle("Size of collection");
  vtx_nofk_ = fs_->make<TH1D>("vtx_nofk","Number of fake vertices",10,-0.5,9.5);
  vtx_nofk_->Sumw2();
  vtx_nofk_->SetXTitle("Number of fakes");
  vtx1_notrk_ = fs_->make<TH1D>("vtx1_notrk","Number of tracks attached to vertex",300,-0.5,299.5);
  vtx1_notrk_->Sumw2();
  vtx1_notrk_->SetXTitle("Number of attached tracks");
  vtx1_x_ = fs_->make<TH1D>("Vertex x-position of 1st vertex if not fake","",100,-0.5,0.5);
  vtx1_x_->Sumw2();
  vtx1_x_->SetXTitle("V1_{x} (mm)");
  vtx1_y_ = fs_->make<TH1D>("vtx1_y","Vertex y-position of 1st vertex if not fake",100,-0.5,0.5);
  vtx1_y_->Sumw2();
  vtx1_y_->SetXTitle("V1_{y} (mm)");
  vtx1_z_ = fs_->make<TH1D>("vtx1_z","Vertex z-position of 1st vertex if not fake",100,-50.0,50.0);
  vtx1_z_->Sumw2();
  vtx1_z_->SetXTitle("V1_{z} (mm)");
  vtx1_zerr_ = fs_->make<TH1D>("vtx1_zerr","Vertex z-position-error of 1st vertex if not fake",100,0.0,0.1);
  vtx1_zerr_->Sumw2();
  vtx1_zerr_->SetXTitle("#Delta V1_{z} (mm)");

  cas_etot_ = fs_->make<TH1D>("cas_etot","Total energy of Castor calibrated RecHits",168000,-12000.0,72000.0);//96000,-12000.0,36000.0);
  cas_etot_->Sumw2();
  cas_etot_->SetXTitle("E_{tot} (GeV)");
  cas_etot15_ = fs_->make<TH1D>("cas_etot15","Total energy of Castor calibrated RecHits in m1-5",96000,-12000.0,36000.0);
  cas_etot15_->Sumw2();
  cas_etot15_->SetXTitle("E_{m1-5} (GeV)");
  cas_etot1114_ = fs_->make<TH1D>("cas_etot1114","Total energy of Castor calibrated RecHits in m11-14",96000,-12000.0,36000.0);
  cas_etot1114_->Sumw2();
  cas_etot1114_->SetXTitle("E_{m11-14} (GeV)");
  cas_etotbx_ = fs_->make<TH1D>("cas_etotbx","Total energy of Castor calibrated RecHits (selected BX)",96000,-12000.0,36000.0);
  cas_etotbx_->Sumw2();
  cas_etotbx_->SetXTitle("E_{tot} (GeV)");
  cas_etot1114bx_ = fs_->make<TH1D>("cas_etot1114bx","Total energy of Castor calibrated RecHits in m11-14 (selected BX)",96000,-12000.0,36000.0);
  cas_etot1114bx_->Sumw2();
  cas_etot1114bx_->SetXTitle("E_{m11-14} (GeV)");
  cas_zprofile_ = fs_->make<TProfile>("cas_zprofile","Profile of Castor intercalibrated RecHit responses along z",14,0.5,14.5);
  cas_zprofile_->SetXTitle("Longitudinal module number");
  cas_phiprofile_ = fs_->make<TProfile>("cas_phiprofile","Profile of Castor intercalibrated RecHit responses over #phi ",16,0.5,16.5);
  cas_phiprofile_->SetXTitle("Azimuthal sector number");
  cas_phiprofile15_ = fs_->make<TProfile>("cas_phiprofile15","Profile of Castor intercalibrated RecHit responses over #phi in m1-5",16,0.5,16.5);
  cas_phiprofile15_->SetXTitle("Azimuthal sector number");
  cas_phiprofile1114_ = fs_->make<TProfile>("cas_phiprofile1114","Profile of Castor intercalibrated RecHit responses over #phi in m11-14",16,0.5,16.5);
  cas_phiprofile1114_->SetXTitle("Azimuthal sector number");
  cas_chprofile_ = fs_->make<TProfile>("cas_chprofile","Castor intercalibrated RecHit responses in all channels",225,-0.5,224.5);
  cas_chprofile_->SetXTitle("Channel number");

  //zdcetotp/m //zdcemvshadp/m //zdcnhits //zdcsatflag //trk vs hf //hf vs cas //cas vs zdc
  zdc_chno_satflg_ = fs_->make<TH1D>("zdc_chno_satflg","No of ZDC RecHits (0..positive), Saturation Flag (-1..negative)",41,-20.5,20.5);
  zdc_chno_satflg_->Sumw2();
  zdcm_etot_ = fs_->make<TH1D>("zdcm_etot","Energy in ZDCm",5500,-110000.0,110000.0);
  zdcm_etot_->Sumw2();
  zdcm_etot_->SetXTitle("E^{-}_{tot} (GeV)");
  zdcp_etot_ = fs_->make<TH1D>("zdcp_etot","Energy in ZDCp",5500,-110000.0,110000.0);
  zdcp_etot_->Sumw2();  
  zdcp_etot_->SetXTitle("E^{+}_{tot} (GeV)");
  zdcm_em_vs_tot_ = fs_->make<TH2D>("zdcm_em_vs_tot","ZDCM EM vs (EM + HAD)",5500,-110000.0,110000.0,5500,-110000.0,110000.0);
  zdcm_em_vs_tot_->SetXTitle("(EM + HAD)");
  zdcm_em_vs_tot_->SetYTitle("EM");
  zdcp_em_vs_tot_ = fs_->make<TH2D>("zdcp_em_vs_tot","ZDCP EM vs (EM + HAD)",5500,-110000.0,110000.0,5500,-110000.0,110000.0);
  zdcp_em_vs_tot_->SetXTitle("(EM + HAD)");
  zdcp_em_vs_tot_->SetYTitle("EM");
  trkno_vs_hfme_ = fs_->make<TH2D>( "trkno_vs_hfme", "No of tracks (pt>1) vs HFM Energy", 5500,-110000.0,110000.0, 1500,-0.5,1499.5);
  trkno_vs_hfme_->SetYTitle("N_{trk}");
  trkno_vs_hfme_->SetXTitle("E^{-}_{tot} (GeV)");
  hfme_vs_case_ = fs_->make<TH2D>("hfme_vs_case", "HFM Energy vs Castor Energy", 5500,-110000.0,110000.0, 5500,-110000.0,110000.0);
  hfme_vs_case_->SetYTitle("E^{-}_{tot} (GeV)");
  hfme_vs_case_->SetXTitle("E_{tot} (GeV)");
  case_vs_zdcme_ = fs_->make<TH2D>("case_vs_zdcme", "Castor Energy vs ZDCM Energy", 5500,-110000.0,110000.0, 5500,-110000.0,110000.0);
  case_vs_zdcme_->SetYTitle("E_{tot} (GeV)");
  case_vs_zdcme_->SetXTitle("E^{-}_{tot} (GeV)");

}


RHAnalyser::~RHAnalyser()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
RHAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  bool isData = true;
#ifdef THIS_IS_MC
  isData = false;
#endif


#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  
  treeVariables_.runNb = iEvent.id().run() ;
  treeVariables_.evtNb = iEvent.id().event() ;
  treeVariables_.bxNb = iEvent.bunchCrossing() ;
  treeVariables_.orbitNb = iEvent.orbitNumber() ;
  treeVariables_.lumiNb = iEvent.luminosityBlock() ;

  uint bx = iEvent.bunchCrossing() ;

  // *************************** CASTOR RecHits ********************************
 

  edm::Handle<CastorRecHitCollection> casrechits;
  try{ iEvent.getByLabel("castorreco",casrechits); }
  catch(...) { edm::LogWarning(" CASTOR ") << " Cannot get Castor RecHits " << std::endl; }

  int nhits = 0;
  double energyCastor = 0;
  double energyCastor15 = 0;
  double energyCastor1114 = 0;
  treeVariables_.nbCasRecHits = 0; 
  std::vector<double> sectorEnergyCastor(16,0.0);
  std::vector<double> sectorEnergyCastor15(16,0.0);
  std::vector<double> sectorEnergyCastor1114(16,0.0);
  std::vector<double> moduleEnergyCastor(14,0.0);
  if(casrechits.failedToGet()!=0 || !casrechits.isValid()) {
    edm::LogWarning(" CASTOR ") << " Cannot read CastorRecHitCollection" << std::endl;
  } else {
    if (_ShowDebug) edm::LogVerbatim(" CastorRecHits ") << " CastorRecHitCollection size: " << casrechits->size() << std::endl;
    for(size_t i1 = 0; i1 < casrechits->size(); ++i1) {
      const CastorRecHit & rh = (*casrechits)[i1];
      HcalCastorDetId castorid = rh.id();
      int sec = castorid.sector();
      int mod = castorid.module();
      int ich = 14*(sec-1)+(mod-1);
      double corrFactor = 1.0;
      if(ich >= 0 && ich <= 223 && castor::channelQuality[sec-1][mod-1]) {
        if(isData) {
#ifdef OLD_428_DATA
          corrFactor = castor::channelIntercalibCorr[sec-1][mod-1] * castor::channelEMCorr[sec-1][mod-1] * ForwardRecord::absCasEscaleFactor;
#elif defined OLD_44X_DATA
	  if( iEvent.id().run()>=181530 && iEvent.id().run()<=181603) corrFactor = castor::channelFullCalibrationHI1[sec-1][mod-1];
          if( iEvent.id().run()>=181604 && iEvent.id().run()<=183126) corrFactor = castor::channelFullCalibrationHI2[sec-1][mod-1];           
#elif defined CALIBRATED_DATA
          corrFactor = 1.0;
#else
          corrFactor = castor::channelGainQE[sec-1][mod-1] * ForwardRecord::absCasEscaleFactor;
#endif
        }
        energyCastor += corrFactor * rh.energy();
        sectorEnergyCastor[sec-1] += corrFactor * rh.energy();
        if(mod>0 && mod<6) sectorEnergyCastor15[sec-1] += corrFactor * rh.energy();
        if(mod>10 && mod<15) sectorEnergyCastor1114[sec-1] += corrFactor * rh.energy();
        moduleEnergyCastor[mod-1] += corrFactor * rh.energy();    
        if(mod>0 && mod<6) energyCastor15 += corrFactor * rh.energy();
        if(mod>10 && mod<15) energyCastor1114 += corrFactor * rh.energy();
        cas_chprofile_->Fill(static_cast<double>(ich),corrFactor * rh.energy());
      }
      if (nhits  < 224) {
        treeVariables_.nbCasRecHits = nhits + 1 ;
	treeVariables_.casRecHitEnergy[nhits] = corrFactor * rh.energy();
	treeVariables_.casRecHitIphi[nhits] = castorid.sector();
	treeVariables_.casRecHitIdepth[nhits] = castorid.module();
#ifdef NEW_53X
        treeVariables_.casRecHitSaturation[nhits] = static_cast<int>( rh.flagField(HcalCaloFlagLabels::ADCSaturationBit) );
        treeVariables_.casRecHitReSaturation[nhits] = static_cast<int>( rh.flagField(HcalCaloFlagLabels::UserDefinedBit0) );
#else
        treeVariables_.casRecHitSaturation[nhits] = -1;
        treeVariables_.casRecHitReSaturation[nhits] = -1;
#endif
      }
      nhits++;
    } // end loop castor rechits
    if (_ShowDebug && casrechits->size() > 0) edm::LogVerbatim(" CastorRecHits ") << " Castor energy: " << energyCastor << std::endl;
  }

  cas_etot_->Fill(energyCastor);
  cas_etot15_->Fill(energyCastor15);
  cas_etot1114_->Fill(energyCastor1114);
  for(uint ibin = 0; ibin < 16; ibin++) {
    cas_phiprofile_->Fill(static_cast<double>(ibin+1),sectorEnergyCastor[ibin]);
    cas_phiprofile15_->Fill(static_cast<double>(ibin+1),sectorEnergyCastor15[ibin]);
    cas_phiprofile1114_->Fill(static_cast<double>(ibin+1),sectorEnergyCastor1114[ibin]); 
  }
  for(uint ibin = 0; ibin < 14; ibin++) cas_zprofile_->Fill(static_cast<double>(ibin+1),moduleEnergyCastor[ibin]);

  // *********************************** ZDC RecHits ******************************************

  edm::Handle <ZDCRecHitCollection> zdcrechits_h; 
  try{ 
    iEvent.getByLabel("zdcreco", zdcrechits_h); }
  catch(...) { 
    edm::LogWarning(" ZDC ") << " Cannot get ZDC RecHits " << std::endl;
  }

  const ZDCRecHitCollection *zdcrechits = zdcrechits_h.failedToGet()? 0 : &*zdcrechits_h;
   
  int nZhits = 0; 
  int nFlags = 0;
  double energyZDCzcas = 0.0;
  double energyEmZDCzcas = 0.0;
  double energyZDCzcasnot = 0.0;
  double energyEmZDCzcasnot = 0.0;
  if (zdcrechits) {
    if (_ShowDebug) edm::LogVerbatim("ZDCRecHits") << " ZDCRecHitCollection size: " << zdcrechits->size() << std::endl;
    for( size_t i1 = 0; i1<zdcrechits->size(); ++i1 ){
      const ZDCRecHit & zdcrechit = (*zdcrechits)[i1]; //if(zside==-1) Detector=ZDCM; if(section==1) SubDetector=EM; 2=HAD
      int iside = zdcrechit.id().zside(); 
      int isect = zdcrechit.id().section();
      if (iside > 0) { //"+" and "-" were likely confused for ZDC
         energyZDCzcas += zdcrechit.energy();
         if (isect == 1) energyEmZDCzcas += zdcrechit.energy();
      }
      if (iside < 0) {
         energyZDCzcasnot += zdcrechit.energy();
         if (isect == 1) energyEmZDCzcasnot += zdcrechit.energy();
      }
      /*if (nZhits < 18) {
         treeVariables_.nbZDCRecHits = nZhits + 1;
         treeVariables_.zdcRecHitEnergy[nZhits] = zdcrechit.energy();
         treeVariables_.zdcRecHitIside[nZhits] = zdcrechit.id().zside();
         treeVariables_.zdcRecHitIsection[nZhits] = zdcrechit.id().section();
         treeVariables_.zdcRecHitIchannel[nZhits] = zdcrechit.id().channel();
         //#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
         treeVariables_.zdcRecHitSaturation[nZhits] = static_cast<int>( zdcrechit.flagField(HcalCaloFlagLabels::ADCSaturationBit) );
      }*/
      nFlags += static_cast<int>( zdcrechit.flagField(HcalCaloFlagLabels::ADCSaturationBit) );
      nZhits++;
    } // enf of loop zdc rechits
    if (_ShowDebug && zdcrechits->size() > 0) edm::LogVerbatim(" ZDCRecHits ") << " ZDC energy (castor side): " << energyZDCzcas << std::endl;
  }
  else { edm::LogVerbatim("ZDCRecHits") << " Empty ZDCRecHitCollection" << std::endl; }

  zdc_chno_satflg_->Fill(nZhits);
  zdc_chno_satflg_->Fill(-(nFlags+1));
  zdcm_etot_->Fill(energyZDCzcas); 
  zdcp_etot_->Fill(energyZDCzcasnot);
  zdcm_em_vs_tot_->Fill(energyZDCzcas,energyEmZDCzcas);
  zdcp_em_vs_tot_->Fill(energyZDCzcasnot,energyEmZDCzcasnot);

  /*
  // *********************************     ZDC Digis ****************************

  edm::Handle<ZDCDigiCollection> zdc_digi_h;
  try{ iEvent.getByType(zdc_digi_h); }
  catch(...) { edm::LogWarning("ZDC ") << " Cannot get ZDC Digi " << std::endl; }

  const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

  int nZdigi = 0;
  if( zdc_digi ) {
  if( zdc_digi->size() > 0 ) {
  if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " ZDCDigiCollection  size: " << zdc_digi->size() << std::endl;
  for ( ZDCDigiCollection::const_iterator j=zdc_digi->begin(); j != zdc_digi->end(); ++j ) {
  double energyZDCDigi = 0;
  const ZDCDataFrame& digi = (const ZDCDataFrame)(*j);
  if (nZdigi < 18) {
  treeVariables_.nbZDCDigis = nZdigi + 1;   
  treeVariables_.zdcDigiIside[nZdigi] = digi.id().zside();
  treeVariables_.zdcDigiIsection[nZdigi] = digi.id().section();
  treeVariables_.zdcDigiIchannel[nZdigi] = digi.id().channel();
  //CapIdTs0 = digi.sample(0).capid();
  int lastTS = digi.size() <= 10 ? digi.size() : 10;
  treeVariables_.nbZDCDigiTs = lastTS;
  for(int ts = 0; ts < lastTS; ts++){
  double energy = 2.6*digi.sample(ts).nominal_fC();     //2.6*digi[ts].nominal_fC();
  energyZDCDigi =+ energy;
  treeVariables_.zdcDigiEnergyFC[nZdigi][ts] = energy;
  treeVariables_.zdcDigiEnergyADC[nZdigi][ts] = digi.sample(ts).adc();
  } //end of loop zdc digi time slices
  }
  nZdigi++;
  if (_ShowDebug) edm::LogVerbatim(" ZDCDigi ") << " ZDC energy (all ts FC): " << energyZDCDigi << " TS#: " << treeVariables_.nbZDCDigiTs << std::endl;
  } // end loop zdc digis

  } else {
  if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " ZDCDigiCollection pointer not empty; but nothing in it: " << zdc_digi->size()  << std::endl;
  }
  } else {
  if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " Empty ZDCDigiCollection pointer "  << std::endl;
  }

  */

  // *************************** Geometry ********************************

  edm::ESHandle<CaloGeometry> pG;
  try { iSetup.get<CaloGeometryRecord>().get(pG); }
  catch(...) { edm::LogWarning(" GEOMETRY ") << " Cannot get CaloGeometryRecord " << std::endl; }
  if (pG.isValid()) { geo = pG.product();
  } else { geo = NULL; }

  if (!geo) {
    edm::LogError(" GEOMETRY ") << " Cannot proceed without CaloGeometryRecord " << std::endl;
    exit(1);
  }

  // *********************************  Tracks ***************************************

  edm::Handle<reco::TrackCollection> trackHandle;
  try{
    //iEvent.getByLabel("generalTracks", trackHandle); //generalTracks hiSelectedTracks hiGeneralTracks _Src
    iEvent.getByLabel(_TrkSrc, trackHandle); 
  } catch(...) {
    edm::LogWarning(" TRACKS ") << " Cannot get Tracks " << std::endl;
  }

  bool isOneTrack = false;
  uint trkNo = 0;
  if (trackHandle.isValid() && trackHandle.failedToGet()==0) {
    if (_ShowDebug) edm::LogVerbatim("Tracks") << " Size of tracks: " << trackHandle->size() << std::endl;
    //h_ntrk->Fill(static_cast<double>(trackHandle->size()));
    for (unsigned int j = 0; j < trackHandle->size(); ++j) {
      const reco::Track& trackData = (*trackHandle)[j];
      bool highPurity = trackData.quality(reco::Track::highPurity);
      double trk_pt = trackData.pt();
      double trk_eta = trackData.eta(); 
      double trk_phi = trackData.phi();
      if (highPurity && trk_pt > 1.0) { isOneTrack = true; trkNo++; }
      if (highPurity) track_pt_->Fill( trk_pt );
      if (highPurity) track_eta_->Fill( trk_eta );
      if (highPurity) track_phi_->Fill( trk_phi );
    }
  } else {
    edm::LogWarning(" generalTracks ") << " Cannot read TRACKS " << std::endl;
  }// end tracks
  track_no_->Fill(static_cast<double>(trkNo));
  

  // *********************************     HF RecHits ****************************


  edm::Handle < HFRecHitCollection > hfrechits_h;
  try{ iEvent.getByLabel("hfreco", hfrechits_h); }
  catch(...) { edm::LogWarning(" HF ") << " Cannot get HF RecHits " << std::endl;  }

  const HFRecHitCollection *hfrechits = hfrechits_h.failedToGet()? 0 : &*hfrechits_h;

  uint hfNo = 0;
  double hfpE = 0.0;
  uint hfpNo4 = 0;
  double hfmE = 0.0;
  uint hfmNo4 = 0;
  std::vector<double> etaBinEnergies(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies1trk(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies1trkL(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies1trkS(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies1trk4(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergiesBX(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergiesNSD(ForwardRecord::nbEtaBins,0.0);
  if (hfrechits) { 
    hfNo = hfrechits->size();
    if (_ShowDebug) edm::LogVerbatim("HFRecHits") << " HFRecHitCollection size: " << hfrechits->size() << std::endl;
    for(size_t i1 = 0; i1 < hfrechits->size(); ++i1){
      const HFRecHit & hfrh = (*hfrechits)[i1];
      HcalDetId hfid = hfrh.id();
      //std::cout << hfid << std::endl;
      //hfShortThreshold_ && hit.id().depth() != 1
      //hfLongThreshold_ && hit.id().depth() == 1
      double eta = geo->getPosition(hfid).eta();
      //double phi = geo->getPosition(hfid).phi();
      if (fabs(eta) < 4.0) {
	runningSampleEnergySum_[0] += hfrh.energy(); 
	(runningSamplesNo_[0])++;
	runningSampleEnergySum_[2] += hfrh.energy()/cosh(eta);
	(runningSamplesNo_[2])++;
      }
      if (fabs(eta) > 4.0) {
	runningSampleEnergySum_[1] += hfrh.energy(); 
	(runningSamplesNo_[1])++;
	runningSampleEnergySum_[3] += hfrh.energy()/cosh(eta);
	(runningSamplesNo_[3])++;
      }
      if (eta > 0.0) {
	hfpE += hfrh.energy();
	if (hfrh.energy() > 4.0) hfpNo4++;
      } else {
	hfmE += hfrh.energy();
	if (hfrh.energy() > 4.0) hfmNo4++;
      }
      const uint ibin = energy_vs_eta_reco_->FindBin( eta ); 
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinEnergies[ibin-1] += hfrh.energy();
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinEts[ibin-1] += hfrh.energy()/cosh(eta); 
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinEnergies1trk[ibin-1] += hfrh.energy();
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && hfid.depth() == 1) etaBinEnergies1trkL[ibin-1] += hfrh.energy();
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && hfid.depth() != 1) etaBinEnergies1trkS[ibin-1] += hfrh.energy(); 
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && hfrh.energy() > 4.0) etaBinEnergies1trk4[ibin-1] += hfrh.energy();
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinEnergiesBX[ibin-1] += hfrh.energy();
      if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinEnergiesNSD[ibin-1] += hfrh.energy();
      if (_ShowDebug) edm::LogVerbatim("HFRecHits") << " hfid: " << hfid << " eta: " << eta << " ibin: " << ibin << " energy: " << hfrh.energy() << std::endl;     
    }
  }

  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergies_[ibin]->Fill(etaBinEnergies[ibin]);
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEts_[ibin]->Fill(etaBinEts[ibin]);
  if (isOneTrack) {
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergies1trk_[ibin]->Fill(etaBinEnergies1trk[ibin]);
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergies1trkL_[ibin]->Fill(etaBinEnergies1trkL[ibin]);
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergies1trkS_[ibin]->Fill(etaBinEnergies1trkS[ibin]);
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergies1trk4_[ibin]->Fill(etaBinEnergies1trk4[ibin]);
  }
  if ( 
      bx == 66 ||
      bx == 183 ||
      bx == 262 ||
      bx == 838 ||
      bx == 960 ||
      bx == 1075 ||
      bx == 1154 ||
      bx == 1854 ||
      bx == 1969 ||
      bx == 2048 ||
      bx == 2536 ||
      bx == 2748 ||
      bx == 2855 ||
      bx == 2934
       ) {
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergiesBX_[ibin]->Fill(etaBinEnergiesBX[ibin]);
    cas_etotbx_->Fill(energyCastor);
    cas_etot1114bx_->Fill(energyCastor1114);
  }
  if (hfpNo4 > 0 && hfmNo4 > 0) {
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinEnergiesNSD_[ibin]->Fill(etaBinEnergiesNSD[ibin]);
  }

  // *********************************     All (not only HF) Towers   ****************************

  edm::Handle<CaloTowerCollection> towers_h;
  try{ iEvent.getByLabel("towerMaker", towers_h); }
  catch(...) { edm::LogWarning("CaloTowers ") << " Cannot get CaloTowers " << std::endl;  }

  const CaloTowerCollection *towers = towers_h.failedToGet()? 0 : &*towers_h;

  std::vector<double> etaBinTowEts(ForwardRecord::nbEtaBins,0.0); 
  if (towers) {
    for(unsigned int i = 0; i < towers->size(); ++i){
      const CaloTower & hit= (*towers)[i];
      if(hit.energy() > 3.0) { //above thresholds
        tow_ene_map_->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()),hit.energy());
        tow_occup_map_->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()));
	if((hit.ieta() > 29 || hit.ieta() < -29) && fabs(hit.eta())<4.0) { //HF eta<4
	  runningSampleEnergySum_[4] += hit.et();
	  (runningSamplesNo_[4])++;
	}
	if((hit.ieta() > 29 || hit.ieta() < -29) && fabs(hit.eta())>4.0) { //HF eta>4
	  runningSampleEnergySum_[5] += hit.et();
	  (runningSamplesNo_[5])++;
	}
	const uint ibin = energy_vs_eta_reco_->FindBin( hit.eta() );     
	if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinTowEts[ibin-1] += hit.et();
      }
    }
  }
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinTowEts_[ibin]->Fill(etaBinTowEts[ibin]);
  
  // ************************ PF Candidates (for now: all candidates without "noise" cuts)   ****************************
  // https://github.com/CmsHI/cmssw/blob/forest_53X_04/RecoHI/HiJetAlgos/plugins/ParticleTowerProducer.cc
  /*
   edm::Handle<reco::PFCandidateCollection> inputsHandle; //particleFlowTmp
   iEvent.getByLabel(src_, inputsHandle);
   for(reco::PFCandidateCollection::const_iterator ci = inputsHandle->begin(); ci!=inputsHandle->end(); ++ci) {
   const reco::PFCandidate& particle = *ci;}
  */
  
  edm::Handle<reco::PFCandidateCollection> pfCandidates_h;
  try{ iEvent.getByLabel(_PFSrc,pfCandidates_h); } //NO particle flow for PbPb !!! //PFTowers //particleFlow //particleFlowTmp
  catch(...) { edm::LogWarning("PFCands ") << " Cannot get PFCands " << std::endl; }
  
  const reco::PFCandidateCollection *pfCandidateColl = pfCandidates_h.failedToGet()? 0 : &(*pfCandidates_h);
    
  std::vector<double> etaBinPFEts(ForwardRecord::nbEtaBins,0.0);
  if (pfCandidateColl) {
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    for(unsigned icand=0;icand<pfCandidateColl->size(); icand++) {
      const reco::PFCandidate pfCandidate = pfCandidateColl->at(icand);
      //pfCandidate.pt();//pfCandidate.particleId();//pfCandidate.eta();//pfCandidate.phi();
      double et = pfCandidate.pt();
      double eta = pfCandidate.eta();
      /*double energy = et*cosh(eta);		*/
      /*int id = pfCandidate.particleId();*/
      bool add_particle = true; //true;/*false;*/
      //X=0,           // undefined
      //h=1,           // charged hadron
      //e=2,           // electron 
      //mu=3,          // muon 
      //gamma=4,       // photon
      //h0=5,          // neutral hadron
      //h_HF=6,        // HF tower identified as a hadron
      //egamma_HF=7    // HF tower identified as an EM particle		
      /*if(id != 4 && id != 5 && id != 6 && id != 7) add_particle = true;
      if(eta > 0.0 && eta < 1.4 && id == 4 && energy > 0.4) add_particle = true;
      if(eta > 0.0 && eta < 1.4 && id == 5 && energy > 2.0) add_particle = true;
      if(eta > 1.4 && eta < 3.2 && id == 4 && energy > 1.8) add_particle = true;
      if(eta > 1.4 && eta < 3.2 && id == 5 && energy > 2.9) add_particle = true;
      if(eta > 3.2 && eta < 5.0 && id == 6 && energy > 4.0) add_particle = true;
      if(eta > 3.2 && eta < 5.0 && id == 7 && energy > 4.0) add_particle = true;*/
      if(add_particle) {
	const uint ibin = energy_vs_eta_reco_->FindBin( eta );		
	if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinPFEts[ibin-1] += et;
      }	
    }
  }
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinPFEts_[ibin]->Fill(etaBinPFEts[ibin]);
  
  // ************************ GenParticles ****************************
  
  if(!isData){
    edm::Handle<reco::GenParticleCollection> genParticles;
    try { iEvent.getByLabel(_GenPartSrc,genParticles); } //hiGenParticles //genParticles
    catch (...) { edm::LogWarning(" GenPart ") << "No GenParticles found!" << std::endl; }
      
    const reco::GenParticleCollection* genColl= genParticles.failedToGet()? 0 : &(*genParticles);
      
    std::vector<double> etaBinGenEts(ForwardRecord::nbEtaBins,0.0);
    if (genColl) {
      for(unsigned igen=0;igen<genColl->size(); igen++) {
	const reco::GenParticle gen = genColl->at(igen);
	int status = gen.status();
	int id = gen.pdgId();			 
	if (status != 1) continue;
	if (abs(id) == 12 || //muon + neutrinos //muons in case of particle flow???
	    abs(id) == 14 ||
	    abs(id) == 16 ||
	    abs(id) == 13) continue;
	//double energy = gen.energy();
	double et = gen.pt();
	double eta = gen.eta();
	//double rap = 0.5*ln((energy()+pz())/(energy()-pz()));//charge(); px();py();pz();mass();et();
	const uint ibin = energy_vs_eta_reco_->FindBin( eta );		
	if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) etaBinGenEts[ibin-1] += et;             
      }
    }
    for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) etaBinGenEts_[ibin]->Fill(etaBinGenEts[ibin]);
  }


  // ********************************* Vertex **************************** 

  uint vtxNo = 0;
  uint vtxNofake = 0;
  uint vtxTrkNo = 0;
  double vtxZ = 0.0;
  double vtxZerr = 0.0;
  double vtxY = 0.0;
  double vtxX = 0.0;
  edm::Handle<reco::VertexCollection> vertices;
  //try { iEvent.getByLabel("offlinePrimaryVertices", vertices); } //"hiSelectedVertex"
  try { iEvent.getByLabel(_VtxSrc, vertices); }
  catch (...) { edm::LogWarning(" VTX ") << "No Vertices found!" << std::endl; }

  //python config selection: !isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2
  if ( vertices->size() > 0 ) {
    vtxNo = vertices->size();
    for (unsigned int i=0; i<vertices->size(); i++) if(vertices->at(i).isFake()) vtxNofake++;
    if (!vertices->at(0).isFake()) {
      const reco::Vertex&  pv = vertices->at(0);
      //double ndof = pv.normalizedChi2();
      //double zpos = fabs(pv.z());
      //double rho = sqrt(pv.x()*pv.x()+pv.y()*pv.y()); 
      //if(ndof > 4.0 && zpos <= 24.0 && rho <= 2.0) vtxNo++;
      //if(i==0 && ndof > 4.0 && rho <= 2.0) 
      vtxZ = pv.z();
      vtxZerr = pv.zError();
      vtxY = pv.y();
      vtxX = pv.x();
      //if(i==0 && ndof > 4.0 && rho <= 2.0 && zpos <= 24.0) 
      vtxTrkNo = pv.tracksSize();
    }
  }
  
  // ******************************

  trkno_vs_hfme_->Fill(hfmE,static_cast<double>(trkNo));
  hfme_vs_case_->Fill(energyCastor,hfmE);
  case_vs_zdcme_->Fill(energyZDCzcas,energyCastor);

  // ******************************

  treeVariables_.trkNb1   = trkNo;
  treeVariables_.hfNb     = hfNo; 
  treeVariables_.hfpE     = hfpE;
  treeVariables_.hfpNb4   = hfpNo4;  
  treeVariables_.hfmE     = hfmE;
  treeVariables_.hfmNb4   = hfmNo4;
  treeVariables_.vtxNb    = vtxNo;
  vtx_no_->Fill(static_cast<double>(vtxNo));
  treeVariables_.vtxNbfake= vtxNofake;
  vtx_nofk_->Fill(static_cast<double>(vtxNofake));
  treeVariables_.vtx1TrkNb= vtxTrkNo;
  vtx1_notrk_->Fill(static_cast<double>(vtxTrkNo));
  treeVariables_.vtx1Z    = vtxZ;
  vtx1_z_->Fill(vtxZ); 
  treeVariables_.vtx1Zerr    = vtxZerr;
  vtx1_zerr_->Fill(vtxZerr);
  treeVariables_.vtx1Y    = vtxY;
  vtx1_y_->Fill(vtxY);
  treeVariables_.vtx1X    = vtxX;
  vtx1_x_->Fill(vtxX);

  rhtree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
RHAnalyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RHAnalyser::endJob() 
{

  if (_ShowDebug) edm::LogVerbatim(" OUT!!! ") << " Reached endJob ! " << std::endl;
  tow_ene_map_->Divide(tow_occup_map_);
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) {
    double width = fabs(ForwardRecord::Eta_Bin_Edges[ibin+1]-ForwardRecord::Eta_Bin_Edges[ibin]);
    double bincenter = (ForwardRecord::Eta_Bin_Edges[ibin+1]+ForwardRecord::Eta_Bin_Edges[ibin])/2.0;
    if (_ShowDebug) edm::LogVerbatim(" OUT!!! ") << "MEAN-e: " << etaBinEnergies_[ibin]->GetMean(1) << "MEAN-et: " << etaBinEts_[ibin]->GetMean(1) << " Width: " << width << std::endl;
    if(width > 1e-9) {
      energy_vs_eta_reco_->Fill(bincenter,etaBinEnergies_[ibin]->GetMean(1)/width);
      //energy_vs_eta_reco_->SetBinError(ibin+1,etaBinEnergies_[ibin]->GetMeanError(1)/width);
      et_vs_eta_reco_->Fill(bincenter,etaBinEts_[ibin]->GetMean(1)/width);
      //et_vs_eta_reco_->SetBinError(ibin+1,etaBinEts_[ibin]->GetMeanError(1)/width);       
      towet_vs_eta_reco_->Fill(bincenter,etaBinTowEts_[ibin]->GetMean(1)/width);
      //towet_vs_eta_reco_->SetBinError(ibin+1,etaBinTowEts_[ibin]->GetMeanError(1)/width);   
      pfet_vs_eta_reco_->Fill(bincenter,etaBinPFEts_[ibin]->GetMean(1)/width);
      //pfet_vs_eta_reco_->SetBinError(ibin+1,etaBinPFEts_[ibin]->GetMeanError(1)/width);   
      genet_vs_eta_reco_->Fill(bincenter,etaBinGenEts_[ibin]->GetMean(1)/width);
    }
    //if(width > 1e-9) energy_vs_eta_reco_->SetBinError(ibin+1,etaBinEnergies_[ibin]->GetRMS(1)/width);
  }
  for(int i1 = 0; i1 < 6; i1++) {
    /*hf_resp_->SetBinContent(i1+1, //do not do the division here: will merge many files later
      runningSampleEnergySum_[i1] / static_cast<double>(runningSamplesNo_[i1])
      );*/
    hf_resp_accum_->SetBinContent(i1+1, runningSampleEnergySum_[i1] );   
    hf_resp_occup_accum_->SetBinContent(i1+1, static_cast<double>(runningSamplesNo_[i1]) ); 
  }

}

// ------------ method called when starting to processes a run  ------------
void 
RHAnalyser::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
RHAnalyser::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
RHAnalyser::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
RHAnalyser::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RHAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RHAnalyser);
