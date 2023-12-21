#ifndef SWD_ANALYZER_RESULTS_H
#define SWD_ANALYZER_RESULTS_H

#include <AnalyzerResults.h>

class SWDAnalyzer;
class SWDAnalyzerSettings;

class SWDAnalyzerResults : public AnalyzerResults
{
  public:
    SWDAnalyzerResults( SWDAnalyzer* analyzer, SWDAnalyzerSettings* settings );
    virtual ~SWDAnalyzerResults();

    virtual void GenerateBubbleText( U64 frameIndex, Channel& channel, DisplayBase displayBase ) override;
    virtual void GenerateExportFile( const char* file, DisplayBase displayBase, U32 exportTypeUserId ) override;

    virtual void GenerateFrameTabularText( U64 frameIndex, DisplayBase displayBase ) override;
    virtual void GeneratePacketTabularText( U64 packetId, DisplayBase displayBase ) override;
    virtual void GenerateTransactionTabularText( U64 transactionId, DisplayBase displayBase ) override;

    double GetSampleTime( S64 sample ) const;
    std::string GetSampleTimeStr( S64 sample ) const;
    SWDAnalyzerSettings* GetSettings() const;

  protected: // functions
    void GetBubbleText( const Frame& f, DisplayBase displayBase, std::vector<std::string>& results );

  protected: // vars
    SWDAnalyzerSettings* mSettings;
    SWDAnalyzer* mAnalyzer;
};

#endif // SWD_ANALYZER_RESULTS_H
