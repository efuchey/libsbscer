#ifndef __TSBSSimDecoder_h
#define __TSBSSimDecoder_h

/////////////////////////////////////////////////////////////////////
//
//   TSBSSimDecoder
//
/////////////////////////////////////////////////////////////////////

#include "SimDecoder.h"
#include "TSBSSimEvent.h"
#include "ha_compiledata.h"
#include <cassert>
#include <map>
#include "TSBSDBManager.h"

class THaCrateMap;

//-----------------------------------------------------------------------------
// Helper classes for making decoded event data available via global variables

class TSBSSimPMTHit : public TObject {
public:
  TSBSSimPMTHit() {}
  TSBSSimPMTHit( const TSBSSimEvent::PMTHit& hit );

  virtual void Print( const Option_t* opt="" ) const;

  Short_t  fID;          // Hit number
  Int_t    fSource;      // MC data set source (0 = signal, >0 background)
  Int_t    fType;        // GEANT particle type (1 = primary)
  Int_t    fMCtrackPID;  // GEANT particle ID (if any)
  Short_t  fOrigVolFlag; // 
  Float_t  fXPMT;        // X coordinate of the PMT in transport coordinates
  Float_t  fYPMT;        // Y coordinate of the PMT in transport coordinates
  Float_t  fNpe;         // Number of photoelectrons
  Double_t fTime;        // Arrival time at electronics
  Double_t fTDCtime1;    // TDC rising time
  Double_t fTDCtime2;    // TDC falling time
  // Digitization results for this hit
  Short_t  fDetID;       // Detector ID
  Short_t  fChannel;     // Channel number
  Short_t  fPMTrow;      // Row number: cross reference to Channel number
  Short_t  fPMTcol;      // Column number: cross reference to Channel number
  Int_t    fTDC1;      // TDC value for rising time
  Int_t    fTDC2;      // TDC value for falling time
  
  ClassDef(TSBSSimPMTHit,1) // A Monte Carlo hit at a GEM tracking chamber
};


//-----------------------------------------------------------------------------
// SoLID simulation decoder class
class TSBSSimDecoder : public Podd::SimDecoder {
 public:
  //constructor may be inputed a data file to input some of the paramaters used by SimDecoder
  //NB: if the second file path does not select a valid file, default parameters will be used.
  TSBSSimDecoder();
  virtual ~TSBSSimDecoder();
  
#if ANALYZER_VERSION_CODE >= 67072 // ANALYZER_VERSION(1,6,0)
  virtual Int_t LoadEvent( const UInt_t* evbuffer );
#else
  virtual Int_t LoadEvent( const Int_t* evbuffer );
#endif
  virtual void  Clear( Option_t* opt="" );
  virtual Int_t DefineVariables( THaAnalysisObject::EMode mode =
				 THaAnalysisObject::kDefine );
  virtual Podd::MCHitInfo GetMCHitInfo( Int_t crate, Int_t slot, Int_t chan ) const;

  TSBSSimPMTHit* GetPMTHit( Int_t i ) const {
    TObject* obj = fMCHits->UncheckedAt(i);
    assert( dynamic_cast<TSBSSimPMTHit*>(obj) );
    return static_cast<TSBSSimPMTHit*>(obj);
  }

  // Workaround for fubar THaEvData
#if ANALYZER_VERSION_CODE >= 67072  // ANALYZER_VERSION(1,6,0)
  static Int_t GetMAXSLOT() { return Decoder::MAXSLOT; }
#else
  static Int_t GetMAXSLOT() { return MAXSLOT; }
#endif

protected:
  typedef std::map<Int_t,Int_t> PMTMap_t;

  // Event-by-event data
  PMTMap_t      fPMTMap;   //! Map ROCKey -> index of corresponding strip

  // Event-by-event data
#if ANALYZER_VERSION_CODE >= 67072  // ANALYZER_VERSION(1,6,0)
  Int_t DoLoadEvent( const UInt_t* evbuffer );
#else
  Int_t DoLoadEvent( const Int_t* evbuffer );
#endif

  // void  PMTtoROC( Int_t s_plane, Int_t s_sector, Int_t s_proj, Int_t s_chan,
  //		    Int_t& crate, Int_t& slot, Int_t& chan ) const;
  Int_t PMTfromROC( Int_t crate, Int_t slot, Int_t chan ) const;
  // Int_t MakeROCKey( Int_t crate, Int_t slot, Int_t chan ) const;
  
  ClassDef(TSBSSimDecoder,0) // Decoder for simulated SoLID spectrometer data
};


#endif
