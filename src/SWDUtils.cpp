#include <AnalyzerHelpers.h>

#include "SWDUtils.h"
#include "SWDTypes.h"

#include <map>

const std::map<SWDRegisters, std::string> REGISTER_NAMES
{
    { SWDRegisters::SWDR_UNDEFINED, "??" },
    // DPv1
    { SWDRegisters::SWDR_DP_ABORT, "ABORT" },
    { SWDRegisters::SWDR_DP_DPIDR, "DPIDR" },
    { SWDRegisters::SWDR_DP_CTRL_STAT, "CTRL/STAT" },
    { SWDRegisters::SWDR_DP_SELECT, "SELECT" },
    { SWDRegisters::SWDR_DP_RDBUFF, "RDBUFF" },
    { SWDRegisters::SWDR_DP_DLCR, "DLCR" },
    { SWDRegisters::SWDR_DP_RESEND, "RESEND" },
    // DPv2
    { SWDRegisters::SWDR_DP_TARGETID, "TARGETID" },
    { SWDRegisters::SWDR_DP_DLPIDR, "DLPIDR" },
    { SWDRegisters::SWDR_DP_EVENTSTAT, "EVENTSTAT" },
    { SWDRegisters::SWDR_DP_TARGETSEL, "TARGETSEL" },
    // DPv3
    { SWDRegisters::SWDR_DP_DPIDR1, "DPIDR1" },
    { SWDRegisters::SWDR_DP_BASEPTR0, "BASEPTR0" },
    { SWDRegisters::SWDR_DP_BASEPTR1, "BASEPTR1" },
    { SWDRegisters::SWDR_DP_SELECT1, "SELECT1" },
    // MEM-AP ADIv5
    { SWDRegisters::SWDR_AP_CSW, "CSW" },
    { SWDRegisters::SWDR_AP_TAR, "TAR" },
    { SWDRegisters::SWDR_AP_TAR_MSW, "TAR_MSW" },
    { SWDRegisters::SWDR_AP_DRW, "DRW" },
    { SWDRegisters::SWDR_AP_BD0, "BD0" },
    { SWDRegisters::SWDR_AP_BD1, "BD1" },
    { SWDRegisters::SWDR_AP_BD2, "BD2" },
    { SWDRegisters::SWDR_AP_BD3, "BD3" },
    { SWDRegisters::SWDR_AP_MBT, "MBT" },
    { SWDRegisters::SWDR_AP_BASE_MSW, "BASE_MSW" },
    { SWDRegisters::SWDR_AP_CFG, "CFG" },
    { SWDRegisters::SWDR_AP_BASE, "BASE" },
    { SWDRegisters::SWDR_AP_RAZ_WI, "RAZ_WI" },
    { SWDRegisters::SWDR_AP_IDR, "IDR" },
    // MEM-APv2 ADIv6
    { SWDRegisters::SWDR_AP_DAR0, "DAR0" },
    { SWDRegisters::SWDR_AP_DAR1, "DAR1" },
    { SWDRegisters::SWDR_AP_DAR2, "DAR2" },
    { SWDRegisters::SWDR_AP_DAR3, "DAR3" },
    { SWDRegisters::SWDR_AP_DAR4, "DAR4" },
    { SWDRegisters::SWDR_AP_DAR5, "DAR5" },
    { SWDRegisters::SWDR_AP_DAR6, "DAR6" },
    { SWDRegisters::SWDR_AP_DAR7, "DAR7" },
    { SWDRegisters::SWDR_AP_DAR8, "DAR8" },
    { SWDRegisters::SWDR_AP_DAR9, "DAR9" },
    { SWDRegisters::SWDR_AP_DAR10, "DAR10" },
    { SWDRegisters::SWDR_AP_DAR11, "DAR11" },
    { SWDRegisters::SWDR_AP_DAR12, "DAR12" },
    { SWDRegisters::SWDR_AP_DAR13, "DAR13" },
    { SWDRegisters::SWDR_AP_DAR14, "DAR14" },
    { SWDRegisters::SWDR_AP_DAR15, "DAR15" },
    { SWDRegisters::SWDR_AP_DAR16, "DAR16" },
    { SWDRegisters::SWDR_AP_DAR17, "DAR17" },
    { SWDRegisters::SWDR_AP_DAR18, "DAR18" },
    { SWDRegisters::SWDR_AP_DAR19, "DAR19" },
    { SWDRegisters::SWDR_AP_DAR20, "DAR20" },
    { SWDRegisters::SWDR_AP_DAR21, "DAR21" },
    { SWDRegisters::SWDR_AP_DAR22, "DAR22" },
    { SWDRegisters::SWDR_AP_DAR23, "DAR23" },
    { SWDRegisters::SWDR_AP_DAR24, "DAR24" },
    { SWDRegisters::SWDR_AP_DAR25, "DAR25" },
    { SWDRegisters::SWDR_AP_DAR26, "DAR26" },
    { SWDRegisters::SWDR_AP_DAR27, "DAR27" },
    { SWDRegisters::SWDR_AP_DAR28, "DAR28" },
    { SWDRegisters::SWDR_AP_DAR29, "DAR29" },
    { SWDRegisters::SWDR_AP_DAR30, "DAR30" },
    { SWDRegisters::SWDR_AP_DAR31, "DAR31" },
    { SWDRegisters::SWDR_AP_DAR32, "DAR32" },
    { SWDRegisters::SWDR_AP_DAR33, "DAR33" },
    { SWDRegisters::SWDR_AP_DAR34, "DAR34" },
    { SWDRegisters::SWDR_AP_DAR35, "DAR35" },
    { SWDRegisters::SWDR_AP_DAR36, "DAR36" },
    { SWDRegisters::SWDR_AP_DAR37, "DAR37" },
    { SWDRegisters::SWDR_AP_DAR38, "DAR38" },
    { SWDRegisters::SWDR_AP_DAR39, "DAR39" },
    { SWDRegisters::SWDR_AP_DAR40, "DAR40" },
    { SWDRegisters::SWDR_AP_DAR41, "DAR41" },
    { SWDRegisters::SWDR_AP_DAR42, "DAR42" },
    { SWDRegisters::SWDR_AP_DAR43, "DAR43" },
    { SWDRegisters::SWDR_AP_DAR44, "DAR44" },
    { SWDRegisters::SWDR_AP_DAR45, "DAR45" },
    { SWDRegisters::SWDR_AP_DAR46, "DAR46" },
    { SWDRegisters::SWDR_AP_DAR47, "DAR47" },
    { SWDRegisters::SWDR_AP_DAR48, "DAR48" },
    { SWDRegisters::SWDR_AP_DAR49, "DAR49" },
    { SWDRegisters::SWDR_AP_DAR50, "DAR50" },
    { SWDRegisters::SWDR_AP_DAR51, "DAR51" },
    { SWDRegisters::SWDR_AP_DAR52, "DAR52" },
    { SWDRegisters::SWDR_AP_DAR53, "DAR53" },
    { SWDRegisters::SWDR_AP_DAR54, "DAR54" },
    { SWDRegisters::SWDR_AP_DAR55, "DAR55" },
    { SWDRegisters::SWDR_AP_DAR56, "DAR56" },
    { SWDRegisters::SWDR_AP_DAR57, "DAR57" },
    { SWDRegisters::SWDR_AP_DAR58, "DAR58" },
    { SWDRegisters::SWDR_AP_DAR59, "DAR59" },
    { SWDRegisters::SWDR_AP_DAR60, "DAR60" },
    { SWDRegisters::SWDR_AP_DAR61, "DAR61" },
    { SWDRegisters::SWDR_AP_DAR62, "DAR62" },
    { SWDRegisters::SWDR_AP_DAR63, "DAR63" },
    { SWDRegisters::SWDR_AP_DAR64, "DAR64" },
    { SWDRegisters::SWDR_AP_DAR65, "DAR65" },
    { SWDRegisters::SWDR_AP_DAR66, "DAR66" },
    { SWDRegisters::SWDR_AP_DAR67, "DAR67" },
    { SWDRegisters::SWDR_AP_DAR68, "DAR68" },
    { SWDRegisters::SWDR_AP_DAR69, "DAR69" },
    { SWDRegisters::SWDR_AP_DAR70, "DAR70" },
    { SWDRegisters::SWDR_AP_DAR71, "DAR71" },
    { SWDRegisters::SWDR_AP_DAR72, "DAR72" },
    { SWDRegisters::SWDR_AP_DAR73, "DAR73" },
    { SWDRegisters::SWDR_AP_DAR74, "DAR74" },
    { SWDRegisters::SWDR_AP_DAR75, "DAR75" },
    { SWDRegisters::SWDR_AP_DAR76, "DAR76" },
    { SWDRegisters::SWDR_AP_DAR77, "DAR77" },
    { SWDRegisters::SWDR_AP_DAR78, "DAR78" },
    { SWDRegisters::SWDR_AP_DAR79, "DAR79" },
    { SWDRegisters::SWDR_AP_DAR80, "DAR80" },
    { SWDRegisters::SWDR_AP_DAR81, "DAR81" },
    { SWDRegisters::SWDR_AP_DAR82, "DAR82" },
    { SWDRegisters::SWDR_AP_DAR83, "DAR83" },
    { SWDRegisters::SWDR_AP_DAR84, "DAR84" },
    { SWDRegisters::SWDR_AP_DAR85, "DAR85" },
    { SWDRegisters::SWDR_AP_DAR86, "DAR86" },
    { SWDRegisters::SWDR_AP_DAR87, "DAR87" },
    { SWDRegisters::SWDR_AP_DAR88, "DAR88" },
    { SWDRegisters::SWDR_AP_DAR89, "DAR89" },
    { SWDRegisters::SWDR_AP_DAR90, "DAR90" },
    { SWDRegisters::SWDR_AP_DAR91, "DAR91" },
    { SWDRegisters::SWDR_AP_DAR92, "DAR92" },
    { SWDRegisters::SWDR_AP_DAR93, "DAR93" },
    { SWDRegisters::SWDR_AP_DAR94, "DAR94" },
    { SWDRegisters::SWDR_AP_DAR95, "DAR95" },
    { SWDRegisters::SWDR_AP_DAR96, "DAR96" },
    { SWDRegisters::SWDR_AP_DAR97, "DAR97" },
    { SWDRegisters::SWDR_AP_DAR98, "DAR98" },
    { SWDRegisters::SWDR_AP_DAR99, "DAR99" },
    { SWDRegisters::SWDR_AP_DAR100, "DAR100" },
    { SWDRegisters::SWDR_AP_DAR101, "DAR101" },
    { SWDRegisters::SWDR_AP_DAR102, "DAR102" },
    { SWDRegisters::SWDR_AP_DAR103, "DAR103" },
    { SWDRegisters::SWDR_AP_DAR104, "DAR104" },
    { SWDRegisters::SWDR_AP_DAR105, "DAR105" },
    { SWDRegisters::SWDR_AP_DAR106, "DAR106" },
    { SWDRegisters::SWDR_AP_DAR107, "DAR107" },
    { SWDRegisters::SWDR_AP_DAR108, "DAR108" },
    { SWDRegisters::SWDR_AP_DAR109, "DAR109" },
    { SWDRegisters::SWDR_AP_DAR110, "DAR110" },
    { SWDRegisters::SWDR_AP_DAR111, "DAR111" },
    { SWDRegisters::SWDR_AP_DAR112, "DAR112" },
    { SWDRegisters::SWDR_AP_DAR113, "DAR113" },
    { SWDRegisters::SWDR_AP_DAR114, "DAR114" },
    { SWDRegisters::SWDR_AP_DAR115, "DAR115" },
    { SWDRegisters::SWDR_AP_DAR116, "DAR116" },
    { SWDRegisters::SWDR_AP_DAR117, "DAR117" },
    { SWDRegisters::SWDR_AP_DAR118, "DAR118" },
    { SWDRegisters::SWDR_AP_DAR119, "DAR119" },
    { SWDRegisters::SWDR_AP_DAR120, "DAR120" },
    { SWDRegisters::SWDR_AP_DAR121, "DAR121" },
    { SWDRegisters::SWDR_AP_DAR122, "DAR122" },
    { SWDRegisters::SWDR_AP_DAR123, "DAR123" },
    { SWDRegisters::SWDR_AP_DAR124, "DAR124" },
    { SWDRegisters::SWDR_AP_DAR125, "DAR125" },
    { SWDRegisters::SWDR_AP_DAR126, "DAR126" },
    { SWDRegisters::SWDR_AP_DAR127, "DAR127" },
    { SWDRegisters::SWDR_AP_DAR128, "DAR128" },
    { SWDRegisters::SWDR_AP_DAR129, "DAR129" },
    { SWDRegisters::SWDR_AP_DAR130, "DAR130" },
    { SWDRegisters::SWDR_AP_DAR131, "DAR131" },
    { SWDRegisters::SWDR_AP_DAR132, "DAR132" },
    { SWDRegisters::SWDR_AP_DAR133, "DAR133" },
    { SWDRegisters::SWDR_AP_DAR134, "DAR134" },
    { SWDRegisters::SWDR_AP_DAR135, "DAR135" },
    { SWDRegisters::SWDR_AP_DAR136, "DAR136" },
    { SWDRegisters::SWDR_AP_DAR137, "DAR137" },
    { SWDRegisters::SWDR_AP_DAR138, "DAR138" },
    { SWDRegisters::SWDR_AP_DAR139, "DAR139" },
    { SWDRegisters::SWDR_AP_DAR140, "DAR140" },
    { SWDRegisters::SWDR_AP_DAR141, "DAR141" },
    { SWDRegisters::SWDR_AP_DAR142, "DAR142" },
    { SWDRegisters::SWDR_AP_DAR143, "DAR143" },
    { SWDRegisters::SWDR_AP_DAR144, "DAR144" },
    { SWDRegisters::SWDR_AP_DAR145, "DAR145" },
    { SWDRegisters::SWDR_AP_DAR146, "DAR146" },
    { SWDRegisters::SWDR_AP_DAR147, "DAR147" },
    { SWDRegisters::SWDR_AP_DAR148, "DAR148" },
    { SWDRegisters::SWDR_AP_DAR149, "DAR149" },
    { SWDRegisters::SWDR_AP_DAR150, "DAR150" },
    { SWDRegisters::SWDR_AP_DAR151, "DAR151" },
    { SWDRegisters::SWDR_AP_DAR152, "DAR152" },
    { SWDRegisters::SWDR_AP_DAR153, "DAR153" },
    { SWDRegisters::SWDR_AP_DAR154, "DAR154" },
    { SWDRegisters::SWDR_AP_DAR155, "DAR155" },
    { SWDRegisters::SWDR_AP_DAR156, "DAR156" },
    { SWDRegisters::SWDR_AP_DAR157, "DAR157" },
    { SWDRegisters::SWDR_AP_DAR158, "DAR158" },
    { SWDRegisters::SWDR_AP_DAR159, "DAR159" },
    { SWDRegisters::SWDR_AP_DAR160, "DAR160" },
    { SWDRegisters::SWDR_AP_DAR161, "DAR161" },
    { SWDRegisters::SWDR_AP_DAR162, "DAR162" },
    { SWDRegisters::SWDR_AP_DAR163, "DAR163" },
    { SWDRegisters::SWDR_AP_DAR164, "DAR164" },
    { SWDRegisters::SWDR_AP_DAR165, "DAR165" },
    { SWDRegisters::SWDR_AP_DAR166, "DAR166" },
    { SWDRegisters::SWDR_AP_DAR167, "DAR167" },
    { SWDRegisters::SWDR_AP_DAR168, "DAR168" },
    { SWDRegisters::SWDR_AP_DAR169, "DAR169" },
    { SWDRegisters::SWDR_AP_DAR170, "DAR170" },
    { SWDRegisters::SWDR_AP_DAR171, "DAR171" },
    { SWDRegisters::SWDR_AP_DAR172, "DAR172" },
    { SWDRegisters::SWDR_AP_DAR173, "DAR173" },
    { SWDRegisters::SWDR_AP_DAR174, "DAR174" },
    { SWDRegisters::SWDR_AP_DAR175, "DAR175" },
    { SWDRegisters::SWDR_AP_DAR176, "DAR176" },
    { SWDRegisters::SWDR_AP_DAR177, "DAR177" },
    { SWDRegisters::SWDR_AP_DAR178, "DAR178" },
    { SWDRegisters::SWDR_AP_DAR179, "DAR179" },
    { SWDRegisters::SWDR_AP_DAR180, "DAR180" },
    { SWDRegisters::SWDR_AP_DAR181, "DAR181" },
    { SWDRegisters::SWDR_AP_DAR182, "DAR182" },
    { SWDRegisters::SWDR_AP_DAR183, "DAR183" },
    { SWDRegisters::SWDR_AP_DAR184, "DAR184" },
    { SWDRegisters::SWDR_AP_DAR185, "DAR185" },
    { SWDRegisters::SWDR_AP_DAR186, "DAR186" },
    { SWDRegisters::SWDR_AP_DAR187, "DAR187" },
    { SWDRegisters::SWDR_AP_DAR188, "DAR188" },
    { SWDRegisters::SWDR_AP_DAR189, "DAR189" },
    { SWDRegisters::SWDR_AP_DAR190, "DAR190" },
    { SWDRegisters::SWDR_AP_DAR191, "DAR191" },
    { SWDRegisters::SWDR_AP_DAR192, "DAR192" },
    { SWDRegisters::SWDR_AP_DAR193, "DAR193" },
    { SWDRegisters::SWDR_AP_DAR194, "DAR194" },
    { SWDRegisters::SWDR_AP_DAR195, "DAR195" },
    { SWDRegisters::SWDR_AP_DAR196, "DAR196" },
    { SWDRegisters::SWDR_AP_DAR197, "DAR197" },
    { SWDRegisters::SWDR_AP_DAR198, "DAR198" },
    { SWDRegisters::SWDR_AP_DAR199, "DAR199" },
    { SWDRegisters::SWDR_AP_DAR200, "DAR200" },
    { SWDRegisters::SWDR_AP_DAR201, "DAR201" },
    { SWDRegisters::SWDR_AP_DAR202, "DAR202" },
    { SWDRegisters::SWDR_AP_DAR203, "DAR203" },
    { SWDRegisters::SWDR_AP_DAR204, "DAR204" },
    { SWDRegisters::SWDR_AP_DAR205, "DAR205" },
    { SWDRegisters::SWDR_AP_DAR206, "DAR206" },
    { SWDRegisters::SWDR_AP_DAR207, "DAR207" },
    { SWDRegisters::SWDR_AP_DAR208, "DAR208" },
    { SWDRegisters::SWDR_AP_DAR209, "DAR209" },
    { SWDRegisters::SWDR_AP_DAR210, "DAR210" },
    { SWDRegisters::SWDR_AP_DAR211, "DAR211" },
    { SWDRegisters::SWDR_AP_DAR212, "DAR212" },
    { SWDRegisters::SWDR_AP_DAR213, "DAR213" },
    { SWDRegisters::SWDR_AP_DAR214, "DAR214" },
    { SWDRegisters::SWDR_AP_DAR215, "DAR215" },
    { SWDRegisters::SWDR_AP_DAR216, "DAR216" },
    { SWDRegisters::SWDR_AP_DAR217, "DAR217" },
    { SWDRegisters::SWDR_AP_DAR218, "DAR218" },
    { SWDRegisters::SWDR_AP_DAR219, "DAR219" },
    { SWDRegisters::SWDR_AP_DAR220, "DAR220" },
    { SWDRegisters::SWDR_AP_DAR221, "DAR221" },
    { SWDRegisters::SWDR_AP_DAR222, "DAR222" },
    { SWDRegisters::SWDR_AP_DAR223, "DAR223" },
    { SWDRegisters::SWDR_AP_DAR224, "DAR224" },
    { SWDRegisters::SWDR_AP_DAR225, "DAR225" },
    { SWDRegisters::SWDR_AP_DAR226, "DAR226" },
    { SWDRegisters::SWDR_AP_DAR227, "DAR227" },
    { SWDRegisters::SWDR_AP_DAR228, "DAR228" },
    { SWDRegisters::SWDR_AP_DAR229, "DAR229" },
    { SWDRegisters::SWDR_AP_DAR230, "DAR230" },
    { SWDRegisters::SWDR_AP_DAR231, "DAR231" },
    { SWDRegisters::SWDR_AP_DAR232, "DAR232" },
    { SWDRegisters::SWDR_AP_DAR233, "DAR233" },
    { SWDRegisters::SWDR_AP_DAR234, "DAR234" },
    { SWDRegisters::SWDR_AP_DAR235, "DAR235" },
    { SWDRegisters::SWDR_AP_DAR236, "DAR236" },
    { SWDRegisters::SWDR_AP_DAR237, "DAR237" },
    { SWDRegisters::SWDR_AP_DAR238, "DAR238" },
    { SWDRegisters::SWDR_AP_DAR239, "DAR239" },
    { SWDRegisters::SWDR_AP_DAR240, "DAR240" },
    { SWDRegisters::SWDR_AP_DAR241, "DAR241" },
    { SWDRegisters::SWDR_AP_DAR242, "DAR242" },
    { SWDRegisters::SWDR_AP_DAR243, "DAR243" },
    { SWDRegisters::SWDR_AP_DAR244, "DAR244" },
    { SWDRegisters::SWDR_AP_DAR245, "DAR245" },
    { SWDRegisters::SWDR_AP_DAR246, "DAR246" },
    { SWDRegisters::SWDR_AP_DAR247, "DAR247" },
    { SWDRegisters::SWDR_AP_DAR248, "DAR248" },
    { SWDRegisters::SWDR_AP_DAR249, "DAR249" },
    { SWDRegisters::SWDR_AP_DAR250, "DAR250" },
    { SWDRegisters::SWDR_AP_DAR251, "DAR251" },
    { SWDRegisters::SWDR_AP_DAR252, "DAR252" },
    { SWDRegisters::SWDR_AP_DAR253, "DAR253" },
    { SWDRegisters::SWDR_AP_DAR254, "DAR254" },
    { SWDRegisters::SWDR_AP_DAR255, "DAR255" },
    { SWDRegisters::SWDR_AP_TRR, "TRR" },
    { SWDRegisters::SWDR_AP_T0TR, "T0TR" },
    { SWDRegisters::SWDR_AP_CFG1, "CFG1" },
    { SWDRegisters::SWDR_AP_ITCTRL, "ITCTRL" },
    { SWDRegisters::SWDR_AP_CLAIMSET, "CLAIMSET" },
    { SWDRegisters::SWDR_AP_CLAIMCLR, "CLAIMCLR" },
    { SWDRegisters::SWDR_AP_DEVAFF0, "DEVAFF0" },
    { SWDRegisters::SWDR_AP_DEVAFF1, "DEVAFF1" },
    { SWDRegisters::SWDR_AP_LAR, "LAR" },
    { SWDRegisters::SWDR_AP_LSR, "LSR" },
    { SWDRegisters::SWDR_AP_AUTHSTATUS, "AUTHSTATUS" },
    { SWDRegisters::SWDR_AP_DEVARCH, "DEVARCH" },
    { SWDRegisters::SWDR_AP_DEVID2, "DEVID2" },
    { SWDRegisters::SWDR_AP_DEVID1, "DEVID1" },
    { SWDRegisters::SWDR_AP_DEVID, "DEVID" },
    { SWDRegisters::SWDR_AP_DEVTYPE, "DEVTYPE" },
    { SWDRegisters::SWDR_AP_PIDR4, "PIDR4" },
    { SWDRegisters::SWDR_AP_PIDR5, "PIDR5" },
    { SWDRegisters::SWDR_AP_PIDR6, "PIDR6" },
    { SWDRegisters::SWDR_AP_PIDR7, "PIDR7" },
    { SWDRegisters::SWDR_AP_PIDR0, "PIDR0" },
    { SWDRegisters::SWDR_AP_PIDR1, "PIDR1" },
    { SWDRegisters::SWDR_AP_PIDR2, "PIDR2" },
    { SWDRegisters::SWDR_AP_PIDR3, "PIDR3" },
    { SWDRegisters::SWDR_AP_CIDR0, "CIDR0" },
    { SWDRegisters::SWDR_AP_CIDR1, "CIDR1" },
    { SWDRegisters::SWDR_AP_CIDR2, "CIDR2" },
    { SWDRegisters::SWDR_AP_CIDR3, "CIDR3" }
};

std::string GetRegisterName( SWDRegisters reg )
{
    auto search = REGISTER_NAMES.find( reg );
    if( search != REGISTER_NAMES.end() )
    {
        return search->second;
    }
    else
    {
        return REGISTER_NAMES.at( SWDRegisters::SWDR_UNDEFINED );
    }
}

std::string GetRegisterValueDesc( SWDRegisters reg, U32 val, DisplayBase displayBase, DPVersion version )
{
    std::string retVal;

    switch( reg )
    {
        case SWDRegisters::SWDR_DP_DPIDR:
            retVal = std::string( "DESIGNER=" ) + Int2StrSal( val & 0x0fffu, displayBase, 12u );
            retVal += std::string( ", VERSION=" ) + Int2StrSal( ( val >> 12u ) & 0x0fu, displayBase, 4u );
            retVal += std::string( ", MIN=" ) + ( ( ( ( val >> 16u ) & 0x01u ) == 0 ) ? "YES" : "NO" );
            retVal += std::string( ", PARTNO=" ) + Int2StrSal( ( val >> 20u ) & 0xffu, displayBase, 8u );
            retVal += std::string( ", REVISION=" ) + Int2StrSal( ( val >> 28u ) & 0x0fu, displayBase, 4u );
            break;

        case SWDRegisters::SWDR_DP_ABORT:
            retVal = std::string( "ORUNERRCLR=" ) + ( ( val & 16u ) ? "1" : "0" );
            retVal += std::string( ", WDERRCLR=" ) + ( ( val & 8u ) ? "1" : "0" );
            retVal += std::string( ", STKERRCLR=" ) + ( ( val & 4u ) ? "1" : "0" );
            retVal += std::string( ", STKCMPCLR=" ) + ( ( val & 2u ) ? "1" : "0" );
            retVal += std::string( ", DAPABORT=" ) + ( ( val & 1u ) ? "1" : "0" );
            break;

        case SWDRegisters::SWDR_DP_CTRL_STAT:
            retVal = std::string( "CSYSPWRUPACK=" ) + ( ( val & ( 1u << 31u ) ) ? "1" : "0" );
            retVal += std::string( ", CSYSPWRUPREQ=" ) + ( ( val & ( 1u << 30u ) ) ? "1" : "0" );
            retVal += std::string( ", CDBGPWRUPACK=" ) + ( ( val & ( 1u << 29u ) ) ? "1" : "0" );
            retVal += std::string( ", CDBGPWRUPREQ=" ) + ( ( val & ( 1u << 28u ) ) ? "1" : "0" );
            retVal += std::string( ", CDBGRSTACK=" ) + ( ( val & ( 1u << 27u ) ) ? "1" : "0" );
            retVal += std::string( ", CDBGRSTREQ=" ) + ( ( val & ( 1u << 26u ) ) ? "1" : "0" );
            if( version >= DPVersion::DP_V3 )
            {
                // available starting from DPv3
                retVal += std::string( ", ERRMODE=" ) + ( ( val & ( 1u << 24u ) ) ? "1" : "0" );
            }
            retVal += ", TRNCNT=" + Int2StrSal( ( val >> 12u ) & 0x0fffu, displayBase, 12u );
            retVal += ", MASKLANE=" + Int2StrSal( ( val >> 8u ) & 0x0fu, displayBase, 4u );
            retVal += std::string( ", WDATAERR=" ) + ( ( val & ( 1u << 7u ) ) ? "1" : "0" );
            retVal += std::string( ", READOK=" ) + ( ( val & ( 1u << 6u ) ) ? "1" : "0" );
            retVal += std::string( ", STICKYERR=" ) + ( ( val & ( 1u << 5u ) ) ? "1" : "0" );
            retVal += std::string( ", STICKYCMP=" ) + ( ( val & ( 1u << 4u ) ) ? "1" : "0" );
            retVal += ", TRNMODE=";
            switch( ( val >> 2u ) & 3u )
            {
                case 0u:
                    retVal += "Normal";
                    break;
                case 1u:
                    retVal += "Pushed verify";
                    break;
                case 2u:
                    retVal += "Pushed compare";
                    break;
                case 3u:
                    retVal += "Reserved";
                    break;
                }
            retVal += std::string( ", STICKYORUN=" ) + ( ( val & ( 1u << 1u ) ) ? "1" : "0" );
            retVal += std::string( ", ORUNDETECT=" ) + ( ( val & ( 1u << 0u ) ) ? "1" : "0" );
            break;

        case SWDRegisters::SWDR_DP_DLCR:
            retVal = "TURNAROUND=" + Int2StrSal( ( val >> 8u ) & 3u, displayBase, 2u );
            retVal += " (" + Int2StrSal( ( ( val >> 8u ) & 3u ) + 1u, Decimal, 2u ) + " data period(s))";
            if( version == DPVersion::DP_V1 )
            {
                retVal += ", WIREMODE=" + Int2StrSal( ( val >> 4u ) & 0x0f, displayBase, 4u ) + " ";
                switch( ( val >> 6u ) & 3u )
                {
                    case 0u:
                        retVal += "Asynchronous";
                        break;
                    case 1u:
                        retVal += "Synchronous";
                        break;
                    default:
                        retVal += "Reserved";
                        break;
                }
                retVal += ", PRESCALER=" + Int2StrSal( val & 3u, displayBase, 2u );
            }
            break;

        case SWDRegisters::SWDR_DP_TARGETID:
            if( version >= DPVersion::DP_V2 )
            {
                // available starting from DPv2
                retVal = std::string( "TREVISION=" ) + Int2StrSal( ( val >> 28u ) & 0xfu, displayBase );
                retVal += ", TPARTNO=" + Int2StrSal( ( val >> 12u ) & 0xffffu, displayBase );
                retVal += ", TDESIGNER=" + Int2StrSal( ( val >> 1u ) & 0x7ffu, displayBase );
            }
            break;

        case SWDRegisters::SWDR_DP_DLPIDR:
            if( version >= DPVersion::DP_V2 )
            {
                // available starting from DPv2
                retVal = std::string( "TINSTANCE=" ) + Int2StrSal( ( val >> 28u ) & 0xfu, displayBase );
                retVal += ", PROTVSN=" + Int2StrSal( val & 0x0fu, displayBase ) + " ";
                retVal += ( ( val & 0x0fu ) == 1u ) ? "(SWD protocol version 2)" : "(Reserved)";
            }
            break;

        case SWDRegisters::SWDR_DP_BASEPTR0:
            if( version == DPVersion::DP_V3 )
            {
                retVal = "PTR=" + Int2StrSal( ( val >> 12u ) & 0xfffffu, displayBase );
                retVal += std::string( ", VALID=" ) + ( ( val & 1u ) ? "1" : "0" );
            }
            break;

        case SWDRegisters::SWDR_DP_EVENTSTAT:
            if( version >= DPVersion::DP_V2 )
            {
                // available starting from DPv2
                retVal = std::string( "EA=" ) + ( ( val & 1u ) ? "1 (There is no event requiring attention)" : "0 (An event requires attention)" );
            }
            break;

            // case SWDR_DP_RESEND:     break;      // just raw data

        case SWDRegisters::SWDR_DP_SELECT:
            if( version < DPVersion::DP_V3 )
            {
                retVal = "APSEL=" + Int2StrSal( ( val >> 24u ) & 0xffu, displayBase );
                retVal += ", APBANKSEL=" + Int2StrSal( ( val >> 4u ) & 0x0fu, displayBase, 4u );
            }
            else
            {
                retVal = "APBANKADDR=" + Int2StrSal( val & 0xFFFFFFF0u, displayBase );
            }
            retVal += ", DPBANKSEL=" + Int2StrSal( val & 0xFu, displayBase, 4u );
            break;

        // case SWDR_DP_RDBUFF:     break;      // just raw data
        // case SWDR_DP_ROUTESEL:       break;      // just raw data

        case SWDRegisters::SWDR_DP_DPIDR1:
            retVal = std::string( "ERRMODE=" ) + ( ( val & ( 1u << 7u ) ) ? "1" : "0" );
            retVal += ", ASIZE=" + Int2StrSal( val & 0x7Fu, displayBase, 7u );
            break;

        // AP
        case SWDRegisters::SWDR_AP_IDR:
            retVal = "Revision=" + Int2StrSal( val >> 28u, displayBase, 4u );
            retVal += ", JEP-106 continuation=" + Int2StrSal( ( val >> 24u ) & 0x0fu, displayBase, 4u );
            retVal += ", JEP-106 identity=" + Int2StrSal( ( val >> 17u ) & 0x7fu, displayBase, 7u );
            retVal +=
                std::string( ", Class=" ) + ( ( val & ( 1u << 16u ) ) ? "This AP is a Memory Acces Port" : "This AP is not a Memory Acces Port" );
            retVal += ", AP Identfication=" + Int2StrSal( val & 0xffu, displayBase );
            break;
        case SWDRegisters::SWDR_AP_CSW:
            retVal = std::string( "DbgSwEnable=" ) + ( ( val & ( 1u << 31u ) ) ? "1" : "0" );
            retVal += ", Prot=" + Int2StrSal( ( val >> 24u ) & 0x7fu, displayBase, 7 );
            if( version < DPVersion::DP_V3 )
            {
                retVal += std::string( ", SPIDEN=" ) + ( ( val & ( 1u << 23u ) ) ? "1" : "0" );
            }
            else
            {
                retVal += std::string( ", SDeviceEn=" ) + ( ( val & ( 1u << 23u ) ) ? "1" : "0" );
                retVal += ", RMEEN=" + Int2StrSal( ( val >> 21u ) & 0x03u, displayBase, 2u );
                retVal += std::string( ", ERRSTOP=" ) + ( ( val & ( 1u << 17u ) ) ? "1" : "0" );
                retVal += std::string( ", ERRNPASS=" ) + ( ( val & ( 1u << 16u ) ) ? "1" : "0" );
                retVal += std::string( ", MTE=" ) + ( ( val & ( 1u << 15u ) ) ? "1" : "0" );
            }
            retVal += ", Type=" + Int2StrSal( ( val >> 12u ) & 0x0fu, displayBase, 4u );
            retVal += ", Mode=" + Int2StrSal( ( val >> 8u ) & 0x0fu, displayBase, 4u );
            retVal += std::string( ", TrInProg=" ) + ( ( val & ( 1u << 7u ) ) ? "1" : "0" );
            retVal += std::string( ", DeviceEn=" ) + ( ( val & ( 1u << 6u ) ) ? "1" : "0" );
            retVal += ", AddrInc=";
            switch( ( val >> 4u ) & 0x3u )
            {
                case 0u:
                    retVal += "Auto-increment off";
                    break;
                case 1u:
                    retVal += "Increment single";
                    break;
                case 2u:
                    retVal += "Increment packed";
                    break;
                default:
                    retVal += "Reserved";
                    break;
            }
            retVal += ", Size=";
            switch( val & 0x7u )
            {
                case 0u:
                    retVal += "Byte (8 bits)";
                    break;
                case 1u:
                    retVal += "Halfword (16 bits)";
                    break;
                case 2u:
                    retVal += "Word (32 bits)";
                    break;
                case 3u:
                    retVal += "Doubleword (64-bits)";
                    break;
                case 4u:
                    retVal += "128-bits";
                    break;
                case 5u:
                    retVal += "256-bits";
                    break;
                default:
                    retVal += "Reserved";
                    break;
            }
            break;
        // case SWDR_AP_TAR:            break;      // these are just raw data
        // case SWDR_AP_DRW:            break;
        // case SWDR_AP_BD0:            break;
        // case SWDR_AP_BD1:            break;
        // case SWDR_AP_BD2:            break;
        // case SWDR_AP_BD3:            break;
        case SWDRegisters::SWDR_AP_CFG:
            if( version >= DPVersion::DP_V3 )
            {
                retVal = "TARINC=" + Int2StrSal( ( val >> 16u ) & 0x0fu, displayBase, 4u );
                retVal += ", ERR=" + Int2StrSal( ( val >> 8u ) & 0x0fu, displayBase, 4u );
                retVal += ", DARSIZE=" + Int2StrSal( ( val >> 4u ) & 0x0fu, displayBase, 4u );
                retVal += std::string( ", RME=" ) + ( ( val & ( 1u << 3u ) ) ? "1" : "0" );
                retVal += ", ";
            }
            else
            {
                retVal = "";
            }
            if( version >= DPVersion::DP_V2 )
            {
                retVal += std::string( "LD=" ) + ( ( val & ( 1u << 2u ) ) ? "1" : "0" );
                retVal += std::string( ", LA=" ) + ( ( val & ( 1u << 1u ) ) ? "1" : "0" );
                retVal += ", ";
            }
            if( val & 1u )
                retVal += "Big-endian";
            else
                retVal += "Little-endian";

            break;
        case SWDRegisters::SWDR_AP_BASE:
            retVal = "BASEADDR=" + Int2StrSal( val >> 12u, displayBase, 20u );
            retVal += std::string( ", Format=" ) + ( ( val & ( 1u << 1u ) ) ? "1" : "0" );
            retVal += ", Entry present=";
            if( val & 1u )
                retVal += "Debug entry present";
            else
                retVal += "No debug entry present";

            break;
        default:
            break;
    }

    return retVal;
}

std::string Int2Str( const U8 i )
{
    char numberStr[ 8 ];
    AnalyzerHelpers::GetNumberString( i, Decimal, 8u, numberStr, sizeof( numberStr ) );
    return numberStr;
}

std::string Int2StrSal( const U64 i, DisplayBase base, const U32 maxBits )
{
    char numberStr[ 256 ];
    AnalyzerHelpers::GetNumberString( i, base, maxBits, numberStr, sizeof( numberStr ) );
    return numberStr;
}
