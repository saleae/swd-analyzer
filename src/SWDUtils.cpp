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

const std::map<SWDRegisters, std::vector<RegisterFieldDescription>> REGISTER_FIELDS = 
{
    // DP registers
    { SWDRegisters::SWDR_DP_DPIDR,
      { { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 31u, 28u, "REVISION", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 27u, 20u, "PARTNO", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 16u, 16u, "MIN", { { 0u, "YES" }, { 1u, "NO" } } },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 15u, 12u, "VERSION", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 11u, 1u, "DESIGNER", {} } }
    },
    { SWDRegisters::SWDR_DP_ABORT,
      { { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 4u, 4u, "ORUNERRCLR", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 3u, 3u, "WDERRCLR", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 2u, 2u, "STKERRCLR", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 1u, 1u, "STKCMPCLR", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 0u, 0u, "DAPABORT", {} } }
    },
    { SWDRegisters::SWDR_DP_CTRL_STAT,
      { { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 31u, 31u, "CSYSPWRUPACK", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 30u, 30u, "CSYSPWRUPREQ", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 29u, 29u, "CDBGPWRUPACK", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 28u, 28u, "CDBGPWRUPREQ", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 27u, 27u, "CDBGRSTACK", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 26u, 26u, "CDBGRSTREQ", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 24u, 24u, "ERRMODE", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 23u, 12u, "TRNCNT", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 11u, 8u, "MASKLANE", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 7u, 7u, "WDATAERR", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 6u, 6u, "READOK", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 5u, 5u, "STICKYERR", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 4u, 4u, "STICKYCMP", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 3u, 2u, "TRNMODE", { { 0u, "Normal" }, { 1u, "Pushed verify" }, { 2u, "Pushed compare" }, { 3u, "Reserved" } } },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 1u, 1u, "STICKYCMP", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 0u, 0u, "ORUNDETECT", {} } }
    },
    { SWDRegisters::SWDR_DP_DLCR,
      { { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 9u, 8u, "TURNROUND", { { 0u, "1 data period" }, { 1u, "2 data periods" }, { 2u, "3 data periods" }, { 3u, "4 data periods" } } },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1, 7u, 6u, "WIREMODE", { { 0u, "Asynchronous" }, { 1u, "Synchronous" }, { 2u, "Reserved" }, { 3u, "Reserved" } } },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1, 2u, 0u, "PRESCALER", {} } }
    },
    { SWDRegisters::SWDR_DP_TARGETID,
      { { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 31u, 28u, "TREVISION", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 27u, 12u, "TPARTNO", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 11u, 1u, "TDESIGNER", {} } }
    },
    { SWDRegisters::SWDR_DP_DLPIDR,
      { { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 31u, 28u, "TINSTANCE", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 3u, 0u, "PROTVSN", { { 1u, "SWD protocol version 2" } } } }
    },
    { SWDRegisters::SWDR_DP_BASEPTR0,
      { { RegMask::REG_READ | RegMask::REG_V3, 31u, 12u, "PTR", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 0u, 0u, "VALID", {} } }
    },
    { SWDRegisters::SWDR_DP_DLPIDR,
      { { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 0u, 0u, "EA", { { 0u, "An event requires attention" }, { 1u, "There is no event requiring attention" } } } }
    },
    { SWDRegisters::SWDR_DP_SELECT,
      { { RegMask::REG_WRITE | RegMask::REG_V3, 31u, 4u, "APADDR", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2, 31u, 24u, "APSEL", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2, 7u, 4u, "APBANKSEL", {} },
        { RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 3u, 0u, "DPBANKSEL", {} } }
    },
    { SWDRegisters::SWDR_DP_DPIDR1,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 7u, "ERRMODE", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 6u, 0u, "ASIZE", {} } } 
    },
    // AP registers
    { SWDRegisters::SWDR_AP_CSW,
      { { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 31u, 31u, "DbgSwEnable", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 30u, 24u, "Prot", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2, 23u, 23u, "SPIDEN", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 23u, 23u, "SDeviceEn", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 22u, 21u, "RMEEN", { { 0u, "Realm and Root accesses are disabled" }, { 1u, "Realm access is enabled. Root access is disabled" }, { 3u, "Realm access is enabled. Root access is enabled" } } },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 17u, 17u, "ERRSTOP", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 16u, 16u, "ERRNPASS", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V2 | RegMask::REG_V3, 15u, 12u, "Type", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 11u, 8u, "Mode", { { 0u, "Basic mode" }, { 1u, "Barrier support enabled" } } },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 7u, 7u, "TrInProg", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 6u, 6u, "DeviceEn", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 5u, 4u, "AddrInc", { { 0u, "Auto-increment disabled" }, { 1u, "Increment-single" }, { 2u, "Increment-packed" } } },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 2u, 0u, "Size",
          { { 0u, "Byte (8-bits)" }, 
            { 1u, "Halfword (16-bits)" },
            { 2u, "Word (32-bits)" },
            { 3u, "Doubleword (64-bits)" },
            { 4u, "128-bits" },
            { 5u, "256-bits" } } } } 
    },
    { SWDRegisters::SWDR_AP_TRR,
      { { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 0u, 0u, "ERR", {} } }
    },
    { SWDRegisters::SWDR_AP_T0TR,
      { { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 31u, 28u, "T7", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 27u, 24u, "T6", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 23u, 20u, "T5", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 19u, 16u, "T4", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 15u, 12u, "T3", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 11u, 8u, "T2", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 7u, 4u, "T1", {} },
        { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 3u, 0u, "T0", {} } }
    },
    { SWDRegisters::SWDR_AP_CFG1,
      { { RegMask::REG_READ | RegMask::REG_V3, 8u, 4u, "TAG0GRAN", { { 0u, "Memory tagging granule is 16 bytes" } } },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "TAG0SIZE", { { 0u, "Memory Tagging Extension not implemented" }, { 4u, "Tag size is 4-bits" } } } }
    },
    { SWDRegisters::SWDR_AP_CFG, 
      { { RegMask::REG_READ | RegMask::REG_V3, 19u, 16u, "TARINC", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 11u, 8u, "ERR", { { 0u, "Error response handling 0" }, { 1u, "Error response handling 1" } } },
        { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "DARSIZE", { { 0x0u, " DAR0-DAR255 are not implemented" }, { 0xAu, " DAR0-DAR255 are implemented" } } },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 3u, "RME", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 2u, 2u, "LD", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 1u, 1u, "LA", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 0u, 0u, "BE", {} }
      } 
    },
    { SWDRegisters::SWDR_AP_BASE, 
      { { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 31u, 12u, "BASEADDR", {} }, 
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 0u, 0u, "P", { { 0u, "No debug entry present" }, { 1u, "Debug entry present" } } } }
    },
    { SWDRegisters::SWDR_AP_IDR,
      { { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 31u, 28u, "REVISION", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 27u, 24u, "JEP-106 continuation code", {} },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 23u, 17u, "JEP-106 identity code", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 16u, 13u, "CLASS", { { 0u, "No defined class" }, { 8u, "MEM-AP" } } },
        { RegMask::REG_READ | RegMask::REG_V1 , 16u, 16u, "CLASS", { { 0u, "not a MEM-AP" }, { 1u, "MEM-AP" } } },
        { RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, 7u, 4u, "VARIANT", {} },
        { RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, 3u, 0u, "TYPE",
          { { 0u, "JTAG connection" },
            { 1u, "AMBA AHB3 bus" },
            { 2u, "AMBA APB2 or APB3 bus" },
            { 4u, "AMBA AXI3 or AXI4 bus, with optional ACE-Lite support" },
            { 5u, "AMBA AHB5 bus" },
            { 6u, "AMBA APB4 and APB5 bus" },
            { 7u, "AMBA AXI5 bus" },
            { 8u, "AMBA AHB5 with enhanced HPROT" } } }
      }
    },
    { SWDRegisters::SWDR_AP_ITCTRL,
      { { RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V3, 0u, 0u, "IME", {} } }
    },
    { SWDRegisters::SWDR_AP_AUTHSTATUS,
      { { RegMask::REG_READ | RegMask::REG_V3, 27u, 26u, "RTNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 25u, 24u, "RTID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 23u, 22u, "SUNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 21u, 20u, "SUID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 19u, 18u, "NSUNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 17u, 16u, "NSUID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 15u, 14u, "RLNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 13u, 12u, "RLID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 11u, 10u, "HNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 9u, 8u, "HID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 7u, 6u, "SNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 5u, 4u, "SID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 2u, "NSNID", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 1u, 0u, "NSID", {} } }
    },
    { SWDRegisters::SWDR_AP_DEVARCH,
      { { RegMask::REG_READ | RegMask::REG_V3, 31u, 21u, "ARCHITECT", { { 0x23B, "Arm" } } },
        { RegMask::REG_READ | RegMask::REG_V3, 20u, 20u, "PRESENT", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 19u, 16u, "REVISION", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 15u, 0u, "ARCHID", { { 0x0A17, "MEM-AP" } } } } 
    },
    { SWDRegisters::SWDR_AP_DEVTYPE,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "SUB", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "MAJOR", {} } } 
    },
    { SWDRegisters::SWDR_AP_PIDR4,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "SIZE", {} }, 
      { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "DES_2", {} } }
    },
    { SWDRegisters::SWDR_AP_PIDR0,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 0u, "PART_0", {} } }
    },
    { SWDRegisters::SWDR_AP_PIDR1,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "DES_0", {} }, 
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "PART_1", {} } }
    },
    { SWDRegisters::SWDR_AP_PIDR2,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "REVISION", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 3u, "JEDEC", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 2u, 0u, "DES_1", {} } }
    },
    { SWDRegisters::SWDR_AP_PIDR3,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "REVAND", {} }, 
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "CMOD", {} } }
    },
    { SWDRegisters::SWDR_AP_CIDR0,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 0u, "PRMBL_0", {} } }
    },
    { SWDRegisters::SWDR_AP_CIDR1,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 4u, "CLASS", {} },
        { RegMask::REG_READ | RegMask::REG_V3, 3u, 0u, "PRMBL_1", {} } }
    },
    { SWDRegisters::SWDR_AP_CIDR2, 
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 0u, "PRMBL_2", {} } }
    },
    { SWDRegisters::SWDR_AP_CIDR3,
      { { RegMask::REG_READ | RegMask::REG_V3, 7u, 0u, "PRMBL_3", {} } }
    }
};

bool IsApReg( SWDRegisters reg )
{
    return ( ( reg >= SWDRegisters::SWDR_AP_CSW ) && ( reg <= SWDRegisters::SWDR_AP_CIDR3 ) );
}

bool IsApDataReg( SWDRegisters reg )
{
    return ( ( reg == SWDRegisters::SWDR_AP_DRW ) ||
        ( ( reg >= SWDRegisters::SWDR_AP_BD0 ) && ( reg <= SWDRegisters::SWDR_AP_BD3 ) ) ||
        ( ( reg >= SWDRegisters::SWDR_AP_DAR0 ) && ( reg <= SWDRegisters::SWDR_AP_DAR255 ) ) );
}

U32 GetMemAddr( const SWDRegisters dataReg, const U32 tar )
{
    U32 memAddr = 0;
    if( dataReg == SWDRegisters::SWDR_AP_DRW )
    {
        memAddr = tar;
    }
    else if( ( dataReg >= SWDRegisters::SWDR_AP_BD0 ) && ( dataReg <= SWDRegisters::SWDR_AP_BD3 ) )
    {
        memAddr = tar + ( ( static_cast<U16>( dataReg ) - static_cast<U16>( SWDRegisters::SWDR_AP_BD0 ) ) * 4u );
    }
    else if( ( dataReg >= SWDRegisters::SWDR_AP_DAR0 ) && ( dataReg <= SWDRegisters::SWDR_AP_DAR255 ) )
    {
        memAddr = tar + ( ( static_cast<U16>( dataReg ) - static_cast<U16>( SWDRegisters::SWDR_AP_DAR0 ) ) * 4u );
    }
    return memAddr;
}

std::string GetReadRegisterValueDesc( const SWDRegistersUnion& swdRegCouple, const U32 data, const DisplayBase displayBase,
                                      const DPVersion version )
{
    std::string regValue;
    regValue.reserve( 256u );
    if( IsApReg( swdRegCouple.reg.current ) )
    {
        if( swdRegCouple.reg.prev == SWDRegisters::SWDR_UNDEFINED )
        {
            // First AP Read
            regValue = "Ignored AP first read data";
        }
        else
        {
            // Next AP Reads
            regValue = "APREG=" + GetRegisterName( swdRegCouple.reg.prev );
            // Data Read/Write register
            if( IsApDataReg( swdRegCouple.reg.prev ) )
            {
                U32 memAddr = GetMemAddr( swdRegCouple.reg.prev, swdRegCouple.reg.memAddr );
                regValue += ", MEMADDR=" + Int2StrSal( memAddr, displayBase, 32 );
            }
            std::string regValueDesc = GetRegisterValueDesc( swdRegCouple.reg.prev, data, displayBase, version, true );
            if( !regValueDesc.empty() )
            {
                regValue += ", " + regValueDesc;
            }
        }
    }
    else
    {
        // DP Read
        if( swdRegCouple.reg.current == SWDRegisters::SWDR_DP_RDBUFF )
        {
            // DP Read of READBUFF
            if( swdRegCouple.reg.prev != SWDRegisters::SWDR_UNDEFINED )
            {
                // AP reg on prev operation
                regValue = "APREG=" + GetRegisterName( swdRegCouple.reg.prev );
                // Data Read/Write register
                if( IsApDataReg( swdRegCouple.reg.prev ) )
                {
                    U32 memAddr = GetMemAddr( swdRegCouple.reg.prev, swdRegCouple.reg.memAddr );
                    regValue += ", MEMADDR=" + Int2StrSal( memAddr, displayBase, 32 );
                }
                std::string regValueDesc = GetRegisterValueDesc( swdRegCouple.reg.prev, data, displayBase, version, true );
                if( !regValueDesc.empty() )
                {
                    regValue += ", " + regValueDesc;
                }
            }
        }
        else
        {
            // DP Read except READBUFF
            regValue = GetRegisterValueDesc( swdRegCouple.reg.current, data, displayBase, version, true );
        }
    }

    return regValue;
}

std::string GetWriteRegisterValueDesc( const SWDRegistersUnion& swdRegCouple, const U32 data, const DisplayBase displayBase,
                                       const DPVersion version )
{
    std::string regValue;
    regValue.reserve( 256u );
    if( IsApReg( swdRegCouple.reg.current ) )
    {
        // AP Write
        regValue = "APREG=" + GetRegisterName( swdRegCouple.reg.current );
        // Data Read/Write register
        if( IsApDataReg( swdRegCouple.reg.current ) )
        {
            U32 memAddr = GetMemAddr( swdRegCouple.reg.current, swdRegCouple.reg.memAddr );
            regValue += ", MEMADDR=" + Int2StrSal( memAddr, displayBase, 32 );
        }
        std::string regValueDesc =
            GetRegisterValueDesc( swdRegCouple.reg.current, data, displayBase, version, false );
        if( !regValueDesc.empty() )
        {
            regValue += ", " + regValueDesc;
        }
    }
    else
    {
        // DP Write
        regValue = GetRegisterValueDesc( swdRegCouple.reg.current, data, displayBase, version, false );
    }
    return regValue;
}

std::string GetRegisterName( const SWDRegisters reg )
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

std::string GetRegisterValueDesc( const SWDRegisters reg, const U32 val, const DisplayBase displayBase, const DPVersion version, const bool isRead )
{
    U8 access = static_cast<U8>( isRead ? RegMask::REG_READ : RegMask::REG_WRITE );
    switch( version )
    {
        case DPVersion::DP_V1:
            access = access | RegMask::REG_V1;
            break;
        case DPVersion::DP_V2:
            access = access | RegMask::REG_V2;
            break;
        case DPVersion::DP_V3:
            access = access | RegMask::REG_V3;
            break;
        default:
            break;
    }
    std::string valueDesc;
    auto search = REGISTER_FIELDS.find( reg );
    if( search != REGISTER_FIELDS.end() )
    {
        valueDesc.reserve( 256u );
        for( const auto& field : search->second )
        {
            if((field.Access & access) == access)
            {
                std::string fieldDesc = field.GetStrFieldValue( val, displayBase );
                if( !fieldDesc.empty() )
                {
                    if( !valueDesc.empty() )
                    {
                        valueDesc += ", ";
                    }
                }
                valueDesc += fieldDesc;
            }
        }
    }
    return valueDesc;
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

const U32 RegisterFieldDescription::GetFieldValue( const U32 regValue ) const
{
    const U32 mask = BitMaskRange<U32>( FiledMSB + 1, FiledLSB );
    return ( regValue & mask ) >> FiledLSB;
}

std::string RegisterFieldDescription::GetStrFieldValue( const U32 regValue, DisplayBase base ) const
{
    const U32 fieldValue = GetFieldValue( regValue );
    std::string resultStr;
    resultStr.reserve( 256u );
    resultStr = FieldName + "=";
    if( FiledLSB == FiledMSB )
    {
        // Single bit
        resultStr += ( fieldValue == 0 ) ? "0" : "1";
    }
    else
    {
        // Bit range
        resultStr += Int2StrSal( fieldValue, base, FiledMSB - FiledLSB + 1 );
    }
    auto search = ValuesDesc.find( fieldValue );
    if( search != ValuesDesc.end() )
    {
        resultStr += " (" + search->second + ")";
    }

    return resultStr;
}
