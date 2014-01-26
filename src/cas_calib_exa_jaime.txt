


namespace ForwardRecord {  
  const unsigned int CSectors = 16;
  const unsigned int CModules = 14;
} //end namespace ForwardRecord

namespace castor {

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
 

}


// THE FOLLOWING SHOULD GO SOMEWHERE INTO YOUR ANALYSER

  const bool isData = true;


  int irun = iEvent.id().run(); //iEvent is normally "known" to your analyser

  // *************************** CASTOR RecHits ********************************
 

  edm::Handle<CastorRecHitCollection> casrechits;
  try{ iEvent.getByLabel("castorreco",casrechits); }
  catch(...) { edm::LogWarning(" CASTOR ") << " Cannot get Castor RecHits " << std::endl; }

  int nhits = 0;
  double energyCastor = 0;

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
		 if(isData && irun>=181530 && irun<=181603) corrFactor = castor::channelFullCalibrationHI1[sec-1][mod-1]; 
         if(isData && irun>=181604 && irun<=183126) corrFactor = castor::channelFullCalibrationHI2[sec-1][mod-1]; 
         energyCastor += corrFactor * rh.energy();
      }
      nhits++;
    } // end loop castor rechits
    if (_ShowDebug && casrechits->size() > 0) edm::LogVerbatim(" CastorRecHits ") << " Castor energy: " << energyCastor << std::endl;
  }

