#include <iostream>
#include <fstream>
#include <algorithm>

#include <AnalyzerHelpers.h>

#include "SWDAnalyzerResults.h"
#include "SWDAnalyzer.h"
#include "SWDAnalyzerSettings.h"
#include "SWDUtils.h"

SWDAnalyzerResults::SWDAnalyzerResults(SWDAnalyzer* analyzer, SWDAnalyzerSettings* settings)
:	mSettings(settings),
	mAnalyzer(analyzer)
{}

SWDAnalyzerResults::~SWDAnalyzerResults()
{}

double SWDAnalyzerResults::GetSampleTime(S64 sample) const
{
	return (sample - mAnalyzer->GetTriggerSample()) / double(mAnalyzer->GetSampleRate());
}

std::string SWDAnalyzerResults::GetSampleTimeStr(S64 sample) const
{
	char time_str[128];
	AnalyzerHelpers::GetTimeString(sample, mAnalyzer->GetTriggerSample(), mAnalyzer->GetSampleRate(), time_str, sizeof(time_str));

	// remove trailing zeros
	int l = strlen(time_str);
	if (l > 7)
		time_str[l - 7] = '\0';

	return time_str;
}

void SWDAnalyzerResults::GetBubbleText(const Frame& f, DisplayBase display_base, std::vector<std::string>& results)
{
	results.clear();

	std::string result;

	if (f.mType == SWDFT_Request)
	{
		SWDRequestFrame& req((SWDRequestFrame&) f);

		std::string addr_str(int2str_sal(req.GetAddr(), display_base, 4));
		std::string reg_name(req.GetRegisterName());

		results.push_back(std::string("Request ") + (req.IsAccessPort() ? " AccessPort" : " DebugPort")
									+ (req.IsRead() ? " Read" : " Write")
									+ " " + reg_name);

		results.push_back(int2str_sal(req.mData2, display_base));
		results.push_back("rq");
		results.push_back("req");
		results.push_back("request");
		results.push_back(std::string("request ")
									+ (req.IsAccessPort() ? "AP" : "DP")
									+ (req.IsRead() ? " R" : " W")
									+ " " + reg_name);

		results.push_back(std::string("Request ")
									+ (req.IsAccessPort() ? "AccessPort" : "DebugPort")
									+ (req.IsRead() ? " Read" : " Write")
									+ " " + reg_name);

	} else if (f.mType == SWDFT_LineReset) {

		results.push_back("Line Reset " + int2str(f.mData1) + " bits");
		results.push_back("rst");
		results.push_back("reset");
		results.push_back("Line Reset");

	} else if (f.mType == SWDFT_Turnaround) {

		results.push_back("Turnaround");
		results.push_back("T");
		results.push_back("trn");
		results.push_back("turn");

	} else if (f.mType == SWDFT_ACK) {

		if (f.mData1 == ACK_OK)
		{
			results.push_back("ACK OK");
			results.push_back("OK");
		} else if (f.mData1 == ACK_WAIT) {
			results.push_back("ACK WAIT");
			results.push_back("WAIT");
		} else if (f.mData1 == ACK_FAULT) {
			results.push_back("ACK FAULT");
			results.push_back("FAULT");
		} else {
			results.push_back("ACK <unknown> probably disconnected");
			results.push_back("ACK <unknown>");
			results.push_back("disc");
		}

		results.push_back("ACK");

	} else if (f.mType == SWDFT_WData) {

		std::string data_str(int2str_sal(f.mData1, display_base, 32));
		SWDRegisters reg(SWDRegisters(f.mData2));
		std::string reg_name(GetRegisterName(reg));
		std::string reg_value(GetRegisterValueDesc(reg, U32(f.mData1), display_base));

		if (!reg_value.empty())
			results.push_back("WData " + data_str + " reg " + reg_name + " bits " + reg_value);
		results.push_back("WData " + data_str + " reg " + reg_name);
		results.push_back("WData");
		results.push_back("WData " + data_str);

	} else if (f.mType == SWDFT_DataParity) {

		results.push_back(std::string("Data parity") + (f.mData2 ? "ok" : "NOT OK"));
		results.push_back(f.mData1 ? "1" : "0");
		results.push_back("prty");
		results.push_back("Parity");

	} else if (f.mType == SWDFT_TrailingBits) {

		results.push_back("Trailing bits");
		results.push_back("Trail");

	} else {

		std::string msg;

		switch (f.mType)
		{
		case SWDFT_Bit:				msg = "bit " + int2str(f.mData2); break;
		case SWDFT_WData:			msg = "data"; break;
		case SWDFT_DataParity:		msg = "dprty"; break;
		case SWDFT_Error:			msg = "err"; break;
		case SWDFT_Request:			msg = "request"; break;
		}

		results.push_back(msg);
	}
}

void SWDAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
	ClearResultStrings();
	Frame f = GetFrame(frame_index);

	std::vector<std::string> results;
	GetBubbleText(f, display_base, results);

	for (std::vector<std::string>::iterator ri(results.begin()); ri != results.end(); ++ri)
		AddResultString(ri->c_str());
}

#define EXP_RECORD_FIELDS	9

void SaveRecord(std::vector<std::string>& rec, std::ofstream& of)
{
	if (rec.empty())
		return;

	while (rec.size() < EXP_RECORD_FIELDS)
		rec.push_back("");

	for (std::vector<std::string>::iterator ri(rec.begin()); ri != rec.end(); ++ri)
	{
		if (ri != rec.begin())
			of << "\t";

		of << *ri;
	}

	of << std::endl;

	rec.clear();
}

void SWDAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
	std::ofstream of(file, std::ios::out);

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	of << "Time\tType\tR/W\tAP/DP\tRegister\tRequest byte\tACK\tWData\tWData details" << std::endl;

	Frame f;
	const U64 num_frames = GetNumFrames();
	std::vector<std::string> record;
	for (U64 fcnt = 0; fcnt < num_frames; fcnt++)
	{
		// get the frame
		f = GetFrame(fcnt);

		if (f.mType == SWDFT_LineReset)
		{
			SaveRecord(record, of);

			record.push_back(GetSampleTimeStr(f.mStartingSampleInclusive));
			record.push_back("Line reset");
			SaveRecord(record, of);
		} else if (f.mType == SWDFT_Request) {

			SaveRecord(record, of);

			SWDRequestFrame& req((SWDRequestFrame&) f);
			record.push_back(GetSampleTimeStr(f.mStartingSampleInclusive));
			record.push_back("Operation");
			record.push_back(req.IsRead() ? "read" : "write");
			record.push_back(req.IsAccessPort() ? "AccessPort" : "DebugPort");
			record.push_back(req.GetRegisterName());
			record.push_back(int2str_sal(req.mData1, display_base, 8));

		} else if (f.mType == SWDFT_ACK) {

			if (f.mData1 == ACK_OK)
				record.push_back("OK");
			else if (f.mData1 == ACK_WAIT)
				record.push_back("WAIT");
			else if (f.mData1 == ACK_FAULT)
				record.push_back("FAULT");
			else
				record.push_back("<disc>");
			
		} else if (f.mType == SWDFT_WData) {

			record.push_back(int2str_sal(f.mData1, display_base, 32));

			SWDRegisters reg(SWDRegisters(f.mData2));
			record.push_back(GetRegisterValueDesc(reg, U32(f.mData1), display_base));

			SaveRecord(record, of);
		}

		if (UpdateExportProgressAndCheckForCancel(fcnt, num_frames))
			return;
	}

	UpdateExportProgressAndCheckForCancel(num_frames, num_frames);
}

void SWDAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
	ClearTabularText();

	std::vector<std::string> results;
	Frame f = GetFrame(frame_index);
	GetBubbleText(f, display_base, results);

	if (!results.empty())
		AddTabularText(results.front().c_str());
}

void SWDAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
	ClearResultStrings();
	AddResultString("not supported");
}

void SWDAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
	ClearResultStrings();
	AddResultString("not supported");
}
