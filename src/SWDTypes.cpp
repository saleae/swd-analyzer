#include <cassert>

#include <algorithm>
#include <iterator>

#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>

#include "SWDAnalyzer.h"
#include "SWDTypes.h"
#include "SWDUtils.h"

// DP registers

enum class DPMask : U8      // Access and version bit field
{
    DP_REG_READ  = 0b00001u,
    DP_REG_WRITE = 0b00010u,
    DP_REG_V1    = 0b00100u,
    DP_REG_V2    = 0b01000u,
    DP_REG_V3    = 0b10000u
};
U8 operator|( const DPMask a, const DPMask b )
{
    return ( static_cast<U8>( a ) | static_cast<U8>( b ) );
}
U8 operator|( const U8 a, const DPMask b )
{
    return ( a | static_cast<U8>( b ) );
}

const std::map<U8, std::vector<SWDOperation::DPRegister>> SWDOperation::DP_REGISTERS = {
    { 0x00u,
        {
            { {}, DPMask::DP_REG_READ | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 , SWDRegisters::SWDR_DP_DPIDR },
            { { 0x00u }, DPMask::DP_REG_READ | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_DPIDR },
            { { 0x01u }, DPMask::DP_REG_READ | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_DPIDR1 },
            { { 0x02u }, DPMask::DP_REG_READ | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_BASEPTR0 },
            { { 0x03u }, DPMask::DP_REG_READ | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_BASEPTR1 },
            { {}, DPMask::DP_REG_WRITE | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_ABORT }
        }
    },
    { 0x04u,
        {
            { { 0x00u }, DPMask::DP_REG_READ | DPMask::DP_REG_WRITE | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_CTRL_STAT },
            { { 0x01u }, DPMask::DP_REG_READ | DPMask::DP_REG_WRITE | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_DLCR },
            { { 0x02u }, DPMask::DP_REG_READ | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_TARGETID },
            { { 0x03u }, DPMask::DP_REG_READ | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_DLPIDR },
            { { 0x04u }, DPMask::DP_REG_READ | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_EVENTSTAT },
            { { 0x05u }, DPMask::DP_REG_WRITE | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_SELECT1 },
        }
    },
    { 0x08u,
        {
            { {}, DPMask::DP_REG_READ | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_RESEND },
            { {}, DPMask::DP_REG_WRITE | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_SELECT }
        }
    },
    { 0x0Cu,
        {
            { {}, DPMask::DP_REG_READ | DPMask::DP_REG_V1 | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_RDBUFF },
            { {}, DPMask::DP_REG_WRITE | DPMask::DP_REG_V2 | DPMask::DP_REG_V3, SWDRegisters::SWDR_DP_TARGETSEL }
        }
    },
};

const std::map<U8, SWDRegisters> SWDOperation::MEM_AP_ADI_V5 = {
    { 0x00u, SWDRegisters::SWDR_AP_CSW },
    { 0x04u, SWDRegisters::SWDR_AP_TAR },
    { 0x08u, SWDRegisters::SWDR_AP_TAR_MSW },
    { 0x0Cu, SWDRegisters::SWDR_AP_DRW },
    { 0x10u, SWDRegisters::SWDR_AP_BD0 },
    { 0x14u, SWDRegisters::SWDR_AP_BD1 },
    { 0x18u, SWDRegisters::SWDR_AP_BD2 },
    { 0x1Cu, SWDRegisters::SWDR_AP_BD3 },
    { 0x20u, SWDRegisters::SWDR_AP_MBT },
    { 0xF0u, SWDRegisters::SWDR_AP_BASE_MSW },
    { 0xF4u, SWDRegisters::SWDR_AP_CFG },
    { 0xF8u, SWDRegisters::SWDR_AP_BASE },
    { 0xFCu, SWDRegisters::SWDR_AP_IDR }
};

const std::map<U16, SWDRegisters> SWDOperation::MEM_AP_ADI_V6 = {
    { 0x000u, SWDRegisters::SWDR_AP_DAR0 },
    { 0x004u, SWDRegisters::SWDR_AP_DAR1 },
    { 0x008u, SWDRegisters::SWDR_AP_DAR2 },
    { 0x00Cu, SWDRegisters::SWDR_AP_DAR3 },
    { 0x010u, SWDRegisters::SWDR_AP_DAR4 },
    { 0x014u, SWDRegisters::SWDR_AP_DAR5 },
    { 0x018u, SWDRegisters::SWDR_AP_DAR6 },
    { 0x01Cu, SWDRegisters::SWDR_AP_DAR7 },
    { 0x020u, SWDRegisters::SWDR_AP_DAR8 },
    { 0x024u, SWDRegisters::SWDR_AP_DAR9 },
    { 0x028u, SWDRegisters::SWDR_AP_DAR10 },
    { 0x02Cu, SWDRegisters::SWDR_AP_DAR11 },
    { 0x030u, SWDRegisters::SWDR_AP_DAR12 },
    { 0x034u, SWDRegisters::SWDR_AP_DAR13 },
    { 0x038u, SWDRegisters::SWDR_AP_DAR14 },
    { 0x03Cu, SWDRegisters::SWDR_AP_DAR15 },
    { 0x040u, SWDRegisters::SWDR_AP_DAR16 },
    { 0x044u, SWDRegisters::SWDR_AP_DAR17 },
    { 0x048u, SWDRegisters::SWDR_AP_DAR18 },
    { 0x04Cu, SWDRegisters::SWDR_AP_DAR19 },
    { 0x050u, SWDRegisters::SWDR_AP_DAR20 },
    { 0x054u, SWDRegisters::SWDR_AP_DAR21 },
    { 0x058u, SWDRegisters::SWDR_AP_DAR22 },
    { 0x05Cu, SWDRegisters::SWDR_AP_DAR23 },
    { 0x060u, SWDRegisters::SWDR_AP_DAR24 },
    { 0x064u, SWDRegisters::SWDR_AP_DAR25 },
    { 0x068u, SWDRegisters::SWDR_AP_DAR26 },
    { 0x06Cu, SWDRegisters::SWDR_AP_DAR27 },
    { 0x070u, SWDRegisters::SWDR_AP_DAR28 },
    { 0x074u, SWDRegisters::SWDR_AP_DAR29 },
    { 0x078u, SWDRegisters::SWDR_AP_DAR30 },
    { 0x07Cu, SWDRegisters::SWDR_AP_DAR31 },
    { 0x080u, SWDRegisters::SWDR_AP_DAR32 },
    { 0x084u, SWDRegisters::SWDR_AP_DAR33 },
    { 0x088u, SWDRegisters::SWDR_AP_DAR34 },
    { 0x08Cu, SWDRegisters::SWDR_AP_DAR35 },
    { 0x090u, SWDRegisters::SWDR_AP_DAR36 },
    { 0x094u, SWDRegisters::SWDR_AP_DAR37 },
    { 0x098u, SWDRegisters::SWDR_AP_DAR38 },
    { 0x09Cu, SWDRegisters::SWDR_AP_DAR39 },
    { 0x0A0u, SWDRegisters::SWDR_AP_DAR40 },
    { 0x0A4u, SWDRegisters::SWDR_AP_DAR41 },
    { 0x0A8u, SWDRegisters::SWDR_AP_DAR42 },
    { 0x0ACu, SWDRegisters::SWDR_AP_DAR43 },
    { 0x0B0u, SWDRegisters::SWDR_AP_DAR44 },
    { 0x0B4u, SWDRegisters::SWDR_AP_DAR45 },
    { 0x0B8u, SWDRegisters::SWDR_AP_DAR46 },
    { 0x0BCu, SWDRegisters::SWDR_AP_DAR47 },
    { 0x0C0u, SWDRegisters::SWDR_AP_DAR48 },
    { 0x0C4u, SWDRegisters::SWDR_AP_DAR49 },
    { 0x0C8u, SWDRegisters::SWDR_AP_DAR50 },
    { 0x0CCu, SWDRegisters::SWDR_AP_DAR51 },
    { 0x0D0u, SWDRegisters::SWDR_AP_DAR52 },
    { 0x0D4u, SWDRegisters::SWDR_AP_DAR53 },
    { 0x0D8u, SWDRegisters::SWDR_AP_DAR54 },
    { 0x0DCu, SWDRegisters::SWDR_AP_DAR55 },
    { 0x0E0u, SWDRegisters::SWDR_AP_DAR56 },
    { 0x0E4u, SWDRegisters::SWDR_AP_DAR57 },
    { 0x0E8u, SWDRegisters::SWDR_AP_DAR58 },
    { 0x0ECu, SWDRegisters::SWDR_AP_DAR59 },
    { 0x0F0u, SWDRegisters::SWDR_AP_DAR60 },
    { 0x0F4u, SWDRegisters::SWDR_AP_DAR61 },
    { 0x0F8u, SWDRegisters::SWDR_AP_DAR62 },
    { 0x0FCu, SWDRegisters::SWDR_AP_DAR63 },
    { 0x100u, SWDRegisters::SWDR_AP_DAR64 },
    { 0x104u, SWDRegisters::SWDR_AP_DAR65 },
    { 0x108u, SWDRegisters::SWDR_AP_DAR66 },
    { 0x10Cu, SWDRegisters::SWDR_AP_DAR67 },
    { 0x110u, SWDRegisters::SWDR_AP_DAR68 },
    { 0x114u, SWDRegisters::SWDR_AP_DAR69 },
    { 0x118u, SWDRegisters::SWDR_AP_DAR70 },
    { 0x11Cu, SWDRegisters::SWDR_AP_DAR71 },
    { 0x120u, SWDRegisters::SWDR_AP_DAR72 },
    { 0x124u, SWDRegisters::SWDR_AP_DAR73 },
    { 0x128u, SWDRegisters::SWDR_AP_DAR74 },
    { 0x12Cu, SWDRegisters::SWDR_AP_DAR75 },
    { 0x130u, SWDRegisters::SWDR_AP_DAR76 },
    { 0x134u, SWDRegisters::SWDR_AP_DAR77 },
    { 0x138u, SWDRegisters::SWDR_AP_DAR78 },
    { 0x13Cu, SWDRegisters::SWDR_AP_DAR79 },
    { 0x140u, SWDRegisters::SWDR_AP_DAR80 },
    { 0x144u, SWDRegisters::SWDR_AP_DAR81 },
    { 0x148u, SWDRegisters::SWDR_AP_DAR82 },
    { 0x14Cu, SWDRegisters::SWDR_AP_DAR83 },
    { 0x150u, SWDRegisters::SWDR_AP_DAR84 },
    { 0x154u, SWDRegisters::SWDR_AP_DAR85 },
    { 0x158u, SWDRegisters::SWDR_AP_DAR86 },
    { 0x15Cu, SWDRegisters::SWDR_AP_DAR87 },
    { 0x160u, SWDRegisters::SWDR_AP_DAR88 },
    { 0x164u, SWDRegisters::SWDR_AP_DAR89 },
    { 0x168u, SWDRegisters::SWDR_AP_DAR90 },
    { 0x16Cu, SWDRegisters::SWDR_AP_DAR91 },
    { 0x170u, SWDRegisters::SWDR_AP_DAR92 },
    { 0x174u, SWDRegisters::SWDR_AP_DAR93 },
    { 0x178u, SWDRegisters::SWDR_AP_DAR94 },
    { 0x17Cu, SWDRegisters::SWDR_AP_DAR95 },
    { 0x180u, SWDRegisters::SWDR_AP_DAR96 },
    { 0x184u, SWDRegisters::SWDR_AP_DAR97 },
    { 0x188u, SWDRegisters::SWDR_AP_DAR98 },
    { 0x18Cu, SWDRegisters::SWDR_AP_DAR99 },
    { 0x190u, SWDRegisters::SWDR_AP_DAR100 },
    { 0x194u, SWDRegisters::SWDR_AP_DAR101 },
    { 0x198u, SWDRegisters::SWDR_AP_DAR102 },
    { 0x19Cu, SWDRegisters::SWDR_AP_DAR103 },
    { 0x1A0u, SWDRegisters::SWDR_AP_DAR104 },
    { 0x1A4u, SWDRegisters::SWDR_AP_DAR105 },
    { 0x1A8u, SWDRegisters::SWDR_AP_DAR106 },
    { 0x1ACu, SWDRegisters::SWDR_AP_DAR107 },
    { 0x1B0u, SWDRegisters::SWDR_AP_DAR108 },
    { 0x1B4u, SWDRegisters::SWDR_AP_DAR109 },
    { 0x1B8u, SWDRegisters::SWDR_AP_DAR110 },
    { 0x1BCu, SWDRegisters::SWDR_AP_DAR111 },
    { 0x1C0u, SWDRegisters::SWDR_AP_DAR112 },
    { 0x1C4u, SWDRegisters::SWDR_AP_DAR113 },
    { 0x1C8u, SWDRegisters::SWDR_AP_DAR114 },
    { 0x1CCu, SWDRegisters::SWDR_AP_DAR115 },
    { 0x1D0u, SWDRegisters::SWDR_AP_DAR116 },
    { 0x1D4u, SWDRegisters::SWDR_AP_DAR117 },
    { 0x1D8u, SWDRegisters::SWDR_AP_DAR118 },
    { 0x1DCu, SWDRegisters::SWDR_AP_DAR119 },
    { 0x1E0u, SWDRegisters::SWDR_AP_DAR120 },
    { 0x1E4u, SWDRegisters::SWDR_AP_DAR121 },
    { 0x1E8u, SWDRegisters::SWDR_AP_DAR122 },
    { 0x1ECu, SWDRegisters::SWDR_AP_DAR123 },
    { 0x1F0u, SWDRegisters::SWDR_AP_DAR124 },
    { 0x1F4u, SWDRegisters::SWDR_AP_DAR125 },
    { 0x1F8u, SWDRegisters::SWDR_AP_DAR126 },
    { 0x1FCu, SWDRegisters::SWDR_AP_DAR127 },
    { 0x200u, SWDRegisters::SWDR_AP_DAR128 },
    { 0x204u, SWDRegisters::SWDR_AP_DAR129 },
    { 0x208u, SWDRegisters::SWDR_AP_DAR130 },
    { 0x20Cu, SWDRegisters::SWDR_AP_DAR131 },
    { 0x210u, SWDRegisters::SWDR_AP_DAR132 },
    { 0x214u, SWDRegisters::SWDR_AP_DAR133 },
    { 0x218u, SWDRegisters::SWDR_AP_DAR134 },
    { 0x21Cu, SWDRegisters::SWDR_AP_DAR135 },
    { 0x220u, SWDRegisters::SWDR_AP_DAR136 },
    { 0x224u, SWDRegisters::SWDR_AP_DAR137 },
    { 0x228u, SWDRegisters::SWDR_AP_DAR138 },
    { 0x22Cu, SWDRegisters::SWDR_AP_DAR139 },
    { 0x230u, SWDRegisters::SWDR_AP_DAR140 },
    { 0x234u, SWDRegisters::SWDR_AP_DAR141 },
    { 0x238u, SWDRegisters::SWDR_AP_DAR142 },
    { 0x23Cu, SWDRegisters::SWDR_AP_DAR143 },
    { 0x240u, SWDRegisters::SWDR_AP_DAR144 },
    { 0x244u, SWDRegisters::SWDR_AP_DAR145 },
    { 0x248u, SWDRegisters::SWDR_AP_DAR146 },
    { 0x24Cu, SWDRegisters::SWDR_AP_DAR147 },
    { 0x250u, SWDRegisters::SWDR_AP_DAR148 },
    { 0x254u, SWDRegisters::SWDR_AP_DAR149 },
    { 0x258u, SWDRegisters::SWDR_AP_DAR150 },
    { 0x25Cu, SWDRegisters::SWDR_AP_DAR151 },
    { 0x260u, SWDRegisters::SWDR_AP_DAR152 },
    { 0x264u, SWDRegisters::SWDR_AP_DAR153 },
    { 0x268u, SWDRegisters::SWDR_AP_DAR154 },
    { 0x26Cu, SWDRegisters::SWDR_AP_DAR155 },
    { 0x270u, SWDRegisters::SWDR_AP_DAR156 },
    { 0x274u, SWDRegisters::SWDR_AP_DAR157 },
    { 0x278u, SWDRegisters::SWDR_AP_DAR158 },
    { 0x27Cu, SWDRegisters::SWDR_AP_DAR159 },
    { 0x280u, SWDRegisters::SWDR_AP_DAR160 },
    { 0x284u, SWDRegisters::SWDR_AP_DAR161 },
    { 0x288u, SWDRegisters::SWDR_AP_DAR162 },
    { 0x28Cu, SWDRegisters::SWDR_AP_DAR163 },
    { 0x290u, SWDRegisters::SWDR_AP_DAR164 },
    { 0x294u, SWDRegisters::SWDR_AP_DAR165 },
    { 0x298u, SWDRegisters::SWDR_AP_DAR166 },
    { 0x29Cu, SWDRegisters::SWDR_AP_DAR167 },
    { 0x2A0u, SWDRegisters::SWDR_AP_DAR168 },
    { 0x2A4u, SWDRegisters::SWDR_AP_DAR169 },
    { 0x2A8u, SWDRegisters::SWDR_AP_DAR170 },
    { 0x2ACu, SWDRegisters::SWDR_AP_DAR171 },
    { 0x2B0u, SWDRegisters::SWDR_AP_DAR172 },
    { 0x2B4u, SWDRegisters::SWDR_AP_DAR173 },
    { 0x2B8u, SWDRegisters::SWDR_AP_DAR174 },
    { 0x2BCu, SWDRegisters::SWDR_AP_DAR175 },
    { 0x2C0u, SWDRegisters::SWDR_AP_DAR176 },
    { 0x2C4u, SWDRegisters::SWDR_AP_DAR177 },
    { 0x2C8u, SWDRegisters::SWDR_AP_DAR178 },
    { 0x2CCu, SWDRegisters::SWDR_AP_DAR179 },
    { 0x2D0u, SWDRegisters::SWDR_AP_DAR180 },
    { 0x2D4u, SWDRegisters::SWDR_AP_DAR181 },
    { 0x2D8u, SWDRegisters::SWDR_AP_DAR182 },
    { 0x2DCu, SWDRegisters::SWDR_AP_DAR183 },
    { 0x2E0u, SWDRegisters::SWDR_AP_DAR184 },
    { 0x2E4u, SWDRegisters::SWDR_AP_DAR185 },
    { 0x2E8u, SWDRegisters::SWDR_AP_DAR186 },
    { 0x2ECu, SWDRegisters::SWDR_AP_DAR187 },
    { 0x2F0u, SWDRegisters::SWDR_AP_DAR188 },
    { 0x2F4u, SWDRegisters::SWDR_AP_DAR189 },
    { 0x2F8u, SWDRegisters::SWDR_AP_DAR190 },
    { 0x2FCu, SWDRegisters::SWDR_AP_DAR191 },
    { 0x300u, SWDRegisters::SWDR_AP_DAR192 },
    { 0x304u, SWDRegisters::SWDR_AP_DAR193 },
    { 0x308u, SWDRegisters::SWDR_AP_DAR194 },
    { 0x30Cu, SWDRegisters::SWDR_AP_DAR195 },
    { 0x310u, SWDRegisters::SWDR_AP_DAR196 },
    { 0x314u, SWDRegisters::SWDR_AP_DAR197 },
    { 0x318u, SWDRegisters::SWDR_AP_DAR198 },
    { 0x31Cu, SWDRegisters::SWDR_AP_DAR199 },
    { 0x320u, SWDRegisters::SWDR_AP_DAR200 },
    { 0x324u, SWDRegisters::SWDR_AP_DAR201 },
    { 0x328u, SWDRegisters::SWDR_AP_DAR202 },
    { 0x32Cu, SWDRegisters::SWDR_AP_DAR203 },
    { 0x330u, SWDRegisters::SWDR_AP_DAR204 },
    { 0x334u, SWDRegisters::SWDR_AP_DAR205 },
    { 0x338u, SWDRegisters::SWDR_AP_DAR206 },
    { 0x33Cu, SWDRegisters::SWDR_AP_DAR207 },
    { 0x340u, SWDRegisters::SWDR_AP_DAR208 },
    { 0x344u, SWDRegisters::SWDR_AP_DAR209 },
    { 0x348u, SWDRegisters::SWDR_AP_DAR210 },
    { 0x34Cu, SWDRegisters::SWDR_AP_DAR211 },
    { 0x350u, SWDRegisters::SWDR_AP_DAR212 },
    { 0x354u, SWDRegisters::SWDR_AP_DAR213 },
    { 0x358u, SWDRegisters::SWDR_AP_DAR214 },
    { 0x35Cu, SWDRegisters::SWDR_AP_DAR215 },
    { 0x360u, SWDRegisters::SWDR_AP_DAR216 },
    { 0x364u, SWDRegisters::SWDR_AP_DAR217 },
    { 0x368u, SWDRegisters::SWDR_AP_DAR218 },
    { 0x36Cu, SWDRegisters::SWDR_AP_DAR219 },
    { 0x370u, SWDRegisters::SWDR_AP_DAR220 },
    { 0x374u, SWDRegisters::SWDR_AP_DAR221 },
    { 0x378u, SWDRegisters::SWDR_AP_DAR222 },
    { 0x37Cu, SWDRegisters::SWDR_AP_DAR223 },
    { 0x380u, SWDRegisters::SWDR_AP_DAR224 },
    { 0x384u, SWDRegisters::SWDR_AP_DAR225 },
    { 0x388u, SWDRegisters::SWDR_AP_DAR226 },
    { 0x38Cu, SWDRegisters::SWDR_AP_DAR227 },
    { 0x390u, SWDRegisters::SWDR_AP_DAR228 },
    { 0x394u, SWDRegisters::SWDR_AP_DAR229 },
    { 0x398u, SWDRegisters::SWDR_AP_DAR230 },
    { 0x39Cu, SWDRegisters::SWDR_AP_DAR231 },
    { 0x3A0u, SWDRegisters::SWDR_AP_DAR232 },
    { 0x3A4u, SWDRegisters::SWDR_AP_DAR233 },
    { 0x3A8u, SWDRegisters::SWDR_AP_DAR234 },
    { 0x3ACu, SWDRegisters::SWDR_AP_DAR235 },
    { 0x3B0u, SWDRegisters::SWDR_AP_DAR236 },
    { 0x3B4u, SWDRegisters::SWDR_AP_DAR237 },
    { 0x3B8u, SWDRegisters::SWDR_AP_DAR238 },
    { 0x3BCu, SWDRegisters::SWDR_AP_DAR239 },
    { 0x3C0u, SWDRegisters::SWDR_AP_DAR240 },
    { 0x3C4u, SWDRegisters::SWDR_AP_DAR241 },
    { 0x3C8u, SWDRegisters::SWDR_AP_DAR242 },
    { 0x3CCu, SWDRegisters::SWDR_AP_DAR243 },
    { 0x3D0u, SWDRegisters::SWDR_AP_DAR244 },
    { 0x3D4u, SWDRegisters::SWDR_AP_DAR245 },
    { 0x3D8u, SWDRegisters::SWDR_AP_DAR246 },
    { 0x3DCu, SWDRegisters::SWDR_AP_DAR247 },
    { 0x3E0u, SWDRegisters::SWDR_AP_DAR248 },
    { 0x3E4u, SWDRegisters::SWDR_AP_DAR249 },
    { 0x3E8u, SWDRegisters::SWDR_AP_DAR250 },
    { 0x3ECu, SWDRegisters::SWDR_AP_DAR251 },
    { 0x3F0u, SWDRegisters::SWDR_AP_DAR252 },
    { 0x3F4u, SWDRegisters::SWDR_AP_DAR253 },
    { 0x3F8u, SWDRegisters::SWDR_AP_DAR254 },
    { 0x3FCu, SWDRegisters::SWDR_AP_DAR255 },

    { 0xD00u, SWDRegisters::SWDR_AP_CSW },
    { 0xD04u, SWDRegisters::SWDR_AP_TAR },
    { 0xD08u, SWDRegisters::SWDR_AP_TAR_MSW },
    { 0xD0Cu, SWDRegisters::SWDR_AP_DRW },
    { 0xD10u, SWDRegisters::SWDR_AP_BD0 },
    { 0xD14u, SWDRegisters::SWDR_AP_BD1 },
    { 0xD18u, SWDRegisters::SWDR_AP_BD2 },
    { 0xD1Cu, SWDRegisters::SWDR_AP_BD3 },
    { 0xD20u, SWDRegisters::SWDR_AP_MBT },
    { 0xD24u, SWDRegisters::SWDR_AP_TRR },
    { 0xD30u, SWDRegisters::SWDR_AP_T0TR },
    { 0xDE0u, SWDRegisters::SWDR_AP_CFG1 },
    { 0xDF0u, SWDRegisters::SWDR_AP_BASE_MSW },
    { 0xDF4u, SWDRegisters::SWDR_AP_CFG },
    { 0xDF8u, SWDRegisters::SWDR_AP_BASE },
    { 0xDFCu, SWDRegisters::SWDR_AP_IDR },
    { 0xF00u, SWDRegisters::SWDR_AP_ITCTRL },
    { 0xFA0u, SWDRegisters::SWDR_AP_CLAIMSET },
    { 0xFA4u, SWDRegisters::SWDR_AP_CLAIMCLR },
    { 0xFA8u, SWDRegisters::SWDR_AP_DEVAFF0 },
    { 0xFACu, SWDRegisters::SWDR_AP_DEVAFF1 },
    { 0xFB0u, SWDRegisters::SWDR_AP_LAR },
    { 0xFB4u, SWDRegisters::SWDR_AP_LSR },
    { 0xFB8u, SWDRegisters::SWDR_AP_AUTHSTATUS },
    { 0xFBCu, SWDRegisters::SWDR_AP_DEVARCH },
    { 0xFC0u, SWDRegisters::SWDR_AP_DEVID2 },
    { 0xFC4u, SWDRegisters::SWDR_AP_DEVID1 },
    { 0xFC8u, SWDRegisters::SWDR_AP_DEVID },
    { 0xFCCu, SWDRegisters::SWDR_AP_DEVTYPE },
    { 0xFD0u, SWDRegisters::SWDR_AP_PIDR4 },
    { 0xFD4u, SWDRegisters::SWDR_AP_PIDR5 },
    { 0xFD8u, SWDRegisters::SWDR_AP_PIDR6 },
    { 0xFDCu, SWDRegisters::SWDR_AP_PIDR7 },
    { 0xFE0u, SWDRegisters::SWDR_AP_PIDR0 },
    { 0xFE4u, SWDRegisters::SWDR_AP_PIDR1 },
    { 0xFE8u, SWDRegisters::SWDR_AP_PIDR2 },
    { 0xFECu, SWDRegisters::SWDR_AP_PIDR3 },
    { 0xFF0u, SWDRegisters::SWDR_AP_CIDR0 },
    { 0xFF4u, SWDRegisters::SWDR_AP_CIDR1 },
    { 0xFF8u, SWDRegisters::SWDR_AP_CIDR2 },
    { 0xFFCu, SWDRegisters::SWDR_AP_CIDR3 }
};

const std::map<SwdFrameTypes, std::string> SWDFrame::FRAME_NAMES =
{
    { SwdFrameTypes::SWD_FT_ERROR, "Erroneus bits" },
    { SwdFrameTypes::SWD_FT_IGNORED, "Ignored bits" },
    { SwdFrameTypes::SWD_FT_LINE_RESET, "SWD Line Reset" },
    { SwdFrameTypes::SWD_FT_JTAG_TO_SWD, "JTAG to SWD" },
    { SwdFrameTypes::SWD_FT_SWD_TO_JTAG, "SWD to JTAG" },
    { SwdFrameTypes::SWD_FT_IDLE_CYCLE, "SWD Idle Cycles" },
    { SwdFrameTypes::SWD_FT_JTAG_TLR, "JTAG Test-Logic-Reset" },
    { SwdFrameTypes::SWD_FT_JTAG_TO_DS, "JTAG to DS sequence" },
    { SwdFrameTypes::SWD_FT_SWD_TO_DS, "SWD to DS sequence" },
    { SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE, "DS Selection Alert sequence preamble" },
    { SwdFrameTypes::SWD_FT_DS_SEL_ALERT, "DS Selection Alert sequence" },
    { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE, "DS Activation code sequence preamble" },
    { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE, "DS Activation code sequence" },
    { SwdFrameTypes::SWD_FT_OPERATION, "SWD Transfer" }
};

const std::string SWDFrame::GetSwdFrameName( SwdFrameTypes frame )
{
    auto search = FRAME_NAMES.find( frame );
    if( search != FRAME_NAMES.end() )
    {
        return search->second;
    }
    else
    {
        return "Unknown frame";
    }
}

// ********************************************************************************

bool SWDBit::IsHigh( bool isRising ) const
{
    return ( isRising ? stateRising : stateFalling ) == BIT_HIGH;
}

S64 SWDBit::GetMinStartEnd() const
{
    S64 s = ( rising - lowStart ) / 2;
    S64 e = ( lowEnd - falling ) / 2;
    return s < e ? s : e;
}

S64 SWDBit::GetStartSample() const
{
    return rising - GetMinStartEnd() + 1;
}

S64 SWDBit::GetEndSample() const
{
    return falling + GetMinStartEnd() - 1;
}

Frame SWDBit::MakeFrame() const
{
    Frame f;

    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_BIT );
    f.mFlags = 0;
    f.mStartingSampleInclusive = GetStartSample();
    f.mEndingSampleInclusive = GetEndSample();

    f.mData1 = stateRising == BIT_HIGH ? 1 : 0;
    f.mData2 = 0;

    return f;
}

// ********************************************************************************

SWDRequestByte::SWDRequestByte( const U8 byteValue )
{
    byte = byteValue;
}

bool SWDRequestByte::IsByteValid() const
{
    // List of valid SWD request bytes
    static std::set<U8> allowedValues = { 0x81u, 0x87u, 0x8Bu, 0x8Du, 0x93u, 0x95u, 0x99u, 0x9Fu,
                                          0xA3u, 0xA5u, 0xA9u, 0xAFu, 0xB1u, 0xB7u, 0xBBu, 0xBDu };

    return allowedValues.find( byte ) != allowedValues.end();
}

bool SWDRequestByte::GetAPnDP() const
{
    return ( byte & 0x02u ) != 0u;
}

bool SWDRequestByte::GetRnW() const
{
    return ( byte & 0x04u ) != 0u;
}

U8 SWDRequestByte::GetAddr() const
{
    return ( byte & 0x18u ) >> 1u;
}

// ********************************************************************************

void SWDBaseSequnce::Clear()
{
    bits.clear();
}

void SWDBaseSequnce::AddFrames( SWDAnalyzerResults* pResults ) const
{
}

void SWDBaseSequnce::AddMarkers( SWDAnalyzerResults* pResults ) const
{
}

// ********************************************************************************

void SWDOperation::Clear()
{
    reqRnW = reqAPnDP = parityRead = dataParityOk = false;
    addr = parityRead = requestByte = reqAck = dataParity = data = 0;
    reg = SWDRegisters::SWDR_UNDEFINED;

    bits.clear();
}

void SWDOperation::AddFrames( SWDAnalyzerResults* pResults ) const
{
    static size_t numApReads = 0;
    static SWDRegisters lastRegister = SWDRegisters::SWDR_UNDEFINED;

    Frame f;

    assert( bits.size() >= 8u + GetTurnaroundNumber() + 3u );

    // request
    SWDRequestFrame req;
    req.mStartingSampleInclusive = bits[ 0u ].GetStartSample();
    req.mEndingSampleInclusive = bits[ 7u ].GetEndSample();
    req.mFlags = ( IsRead() ? SWDRequestFrame::IS_READ : 0u ) | ( reqAPnDP ? SWDRequestFrame::IS_ACCESS_PORT : 0u );
    req.SetRequestByte( requestByte );
    req.SetRegister( reg );
    req.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_REQUEST );
    pResults->AddFrame( req );

    std::deque<SWDBit>::const_iterator bi( bits.begin() + 8u );
    // turnaround
    f = bi->MakeFrame();
    f.mEndingSampleInclusive = bi[ GetTurnaroundNumber() - 1u ].GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_TURNAROUND );
    pResults->AddFrame( f );
    bi += GetTurnaroundNumber();

    // ack
    f = bi->MakeFrame();
    f.mEndingSampleInclusive = bi[ 2u ].GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_ACK );
    f.mData1 = reqAck;
    pResults->AddFrame( f );
    bi += 3u;

    // turnaround if ACK is WAIT or FAULT or OK with write operation
    if( ( reqAck == static_cast<U8>( SWDAcks::ACK_WAIT ) ) ||
        ( reqAck == static_cast<U8>( SWDAcks::ACK_FAULT ) ) ||
        ( ( reqAck == static_cast<U8>( SWDAcks::ACK_OK ) ) && !IsRead() ) )
    {
        f = bi->MakeFrame();
        f.mEndingSampleInclusive = bi[ GetTurnaroundNumber() - 1u ].GetEndSample();
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_TURNAROUND );
        pResults->AddFrame( f );
        bi += GetTurnaroundNumber();
    }
    if( bits.size() < 8u + GetTurnaroundNumber() + 3u + 32u + 1u + GetTurnaroundNumber() )
        return;

    // data
    f = bi->MakeFrame();
    f.mEndingSampleInclusive = bi[ 31u ].GetEndSample();
    if( IsRead() )
    {
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_RDATA );
    }
    else
    {
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_WDATA );
    }

    SWDRegistersUnion swdRegCouple; // Couple of previous and current SWD register
    if( IsRead() )
    {
        if( reqAPnDP )
        {
            // AP read
            if( numApReads == 0 )
            {
                // On the first AP read access, the read data that is returned is unknown. The debugger must discard this result.
                swdRegCouple.reg.prev = reg;
                swdRegCouple.reg.current = SWDRegisters::SWDR_UNDEFINED;
            }
            else
            {
                // The next AP read access, if successful, returns the result of the previous AP read.
                swdRegCouple.reg.prev = lastRegister;
                swdRegCouple.reg.current = reg;
            }
            numApReads++;
            lastRegister = reg;
        }
        else
        {
            // DP read
            if( ( numApReads != 0 ) && ( reg == SWDRegisters::SWDR_DP_RDBUFF ) )
            {
                // The debugger can then read the DP RDBUFF register to obtain the last AP read result.
                swdRegCouple.reg.prev = lastRegister;
                swdRegCouple.reg.current = reg;
                numApReads = 0;
                lastRegister = SWDRegisters::SWDR_UNDEFINED;
            }
            else
            {
                swdRegCouple.reg.prev = SWDRegisters::SWDR_UNDEFINED;
                swdRegCouple.reg.current = reg;
            }
        }
    }
    else
    {
        swdRegCouple.reg.prev = SWDRegisters::SWDR_UNDEFINED;
        swdRegCouple.reg.current = reg;
        numApReads = 0;
        lastRegister = SWDRegisters::SWDR_UNDEFINED;
    }

    f.mData1 = data;
    f.mData2 = swdRegCouple.blob;
    pResults->AddFrame( f );

    // data parity
    f = bi[ 32u ].MakeFrame();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_DATA_PARITY );
    f.mData1 = dataParity;
    f.mData2 = dataParityOk ? 1 : 0;
    pResults->AddFrame( f );

    if( IsRead() )
    {
        // add turnaround on read operation
        f = bi[ 33u ].MakeFrame();
        f.mEndingSampleInclusive = bi[ 33u + GetTurnaroundNumber() - 1u ].GetEndSample();
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_TURNAROUND );
        pResults->AddFrame( f );
    }
}

void SWDOperation::AddMarkers( SWDAnalyzerResults* pResults ) const
{
    for( std::deque<SWDBit>::const_iterator bi( bits.begin() ); bi != bits.end(); ++bi )
    {
        size_t ndx = bi - bits.begin();

        // turnaround
        if( ( (ndx >= 8u) && (ndx < 8u + GetTurnaroundNumber()) ) ||
            ( !IsRead() && ( ( ndx >= 8u + GetTurnaroundNumber() + 3u ) && ( ndx < 8u + GetTurnaroundNumber() + 3u + GetTurnaroundNumber() ) ) ) ||
            ( IsRead() && ( ( ndx >= 8u + GetTurnaroundNumber() + 3u + 32u + 1u ) && ( ndx < 8u + GetTurnaroundNumber() + 3u + 32u + 1u + GetTurnaroundNumber() ) ) )
        )
        {
            pResults->AddMarker( ( bi->falling + bi->rising ) / 2, AnalyzerResults::X, pResults->GetSettings()->mSWCLK );
        }
        else
        {
            // Data is always sampled by both ends on the rising edge.
            pResults->AddMarker( bi->rising, bi->stateRising == BIT_HIGH ? AnalyzerResults::One : AnalyzerResults::Zero,
                             pResults->GetSettings()->mSWCLK );
        }
    }
}

void SWDOperation::SetDPVer( DPVersion version )
{
    dpVer = version;
}

void SWDOperation::SetRegister( U32 selectReg )
{

    if( reqAPnDP ) // AccessPort or DebugPort?
    {
        reg = SWDRegisters::SWDR_AP_RAZ_WI;
        if( dpVer < DPVersion::DP_V3 )
        {
            // MEM-AP ADIv5
            U8 apBankSel = U8( selectReg & 0xF0u );
            U8 apRegOffset = apBankSel | addr;
            const auto apReg = MEM_AP_ADI_V5.find( apRegOffset );
            if( apReg != MEM_AP_ADI_V5.end() )
            {
                reg = apReg->second;
            }
        }
        else
        {
            // MEM-AP ADIv6
            U16 apRegOffset = U16( selectReg & 0xFF0u );
            apRegOffset |= addr;
            const auto apReg = MEM_AP_ADI_V6.find( apRegOffset );
            if( apReg != MEM_AP_ADI_V6.end() )
            {
                reg = apReg->second;
            }
        }
    }
    else
    {
        reg = SWDRegisters::SWDR_UNDEFINED;
        U8 dpBankSel = selectReg & 0x0Fu;

        const auto dpReg = DP_REGISTERS.find( addr );
        if( dpReg != DP_REGISTERS.end() )
        {
            for( const auto& dpRegRecord : dpReg->second )
            {
                if( dpRegRecord.Bank.empty() || ( dpRegRecord.Bank.find( dpBankSel ) != dpRegRecord.Bank.end() ) )
                {
                    if( ( reqRnW && ( ( dpRegRecord.Access & static_cast<U8>( DPMask::DP_REG_READ ) ) != 0u ) ) ||
                        ( !reqRnW && ( ( dpRegRecord.Access & static_cast<U8>( DPMask::DP_REG_WRITE ) ) != 0u ) ) )
                    {
                        if( ( dpVer == DPVersion::DP_V0 ) ||
                            ( ( dpVer == DPVersion::DP_V1 ) && ( ( dpRegRecord.Access & static_cast<U8>( DPMask::DP_REG_V1 ) ) != 0u ) ) ||
                            ( ( dpVer == DPVersion::DP_V2 ) && ( ( dpRegRecord.Access & static_cast<U8>( DPMask::DP_REG_V2 ) ) != 0u ) ) ||
                            ( ( dpVer == DPVersion::DP_V3 ) && ( ( dpRegRecord.Access & static_cast<U8>( DPMask::DP_REG_V3 ) ) != 0u ) ) )
                        {
                            reg = dpRegRecord.Register;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool SWDOperation::IsRead() const
{
    return reqRnW;
}

void SWDOperation::SetTurnaroundNumber( U8 num )
{
    turnaround = num;
}

size_t SWDOperation::GetTurnaroundNumber() const
{
    return turnaround;
}

void SWDOperation::SetOrunDetect( bool enable )
{
    orundetect = enable;
}

bool SWDOperation::GetOrunDetect() const
{
    return orundetect;
}


// ********************************************************************************

void SWDLineReset::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // line reset
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_LINE_RESET );
    f.mData1 = bits.size();
    pResults->AddFrame( f );
}

// ********************************************************************************

void JTAGToSWD::Clear()
{
    deprecated = false;
    data = 0;
    bits.clear();
}

void JTAGToSWD::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TO_SWD );
    f.mData1 = data;
    f.mFlags = deprecated ? 1 : 0;
    pResults->AddFrame( f );
}

// ********************************************************************************

void SWDToJTAG::Clear()
{
    deprecated = false;
    data = 0;
    bits.clear();
}

void SWDToJTAG::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_SWD_TO_JTAG );
    f.mData1 = data;
    f.mFlags = deprecated ? 1 : 0;
    pResults->AddFrame( f );
}

// ********************************************************************************

void SWDErrorBits::Clear()
{
    protocol = DebugProtocol::DPROTOCOL_UNKNOWN;
    bits.clear();
}

void SWDErrorBits::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // Erroneus bits
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    if( ( protocol == DebugProtocol::DPROTOCOL_UNKNOWN ) || ( protocol == DebugProtocol::DPROTOCOL_SWD ) )
    {
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_ERROR );
    }
    else
    {
        f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_IGNORED );
    }
    f.mData1 = bits.size();
    f.mData2 = static_cast<U64>( protocol );
    pResults->AddFrame( f );
}

void SWDErrorBits::SetProtocol( const DebugProtocol newProtocol )
{
    protocol = newProtocol;
}

// ********************************************************************************

void SWDIdleCycles::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // Idle cycle bits
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_IDLE_CYCLE );
    f.mData1 = bits.size();
    pResults->AddFrame( f );
}

// ********************************************************************************

void JTAGTlr::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // JTAG test logic reset
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TLR );
    f.mData1 = bits.size();
    pResults->AddFrame( f );
}

// ********************************************************************************

void JTAGToDS::Clear()
{
    data = 0;
    bits.clear();
}

void JTAGToDS::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TO_DS );
    f.mData1 = data;
    pResults->AddFrame( f );
}

// ********************************************************************************

void SWDToDS::Clear()
{
    data = 0;
    bits.clear();
}

void SWDToDS::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_SWD_TO_DS );
    f.mData1 = data;
    pResults->AddFrame( f );
}

// ********************************************************************************

void DSSelectionAlertPreamble::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // DS Selection Alert sequence preamble
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE );
    f.mData1 = bits.size();
    pResults->AddFrame( f );
}

// ********************************************************************************

void DSSelectionAlert::Clear()
{
    hi64BitData = 0;
    lo64BitData = 0;
    bits.clear();
}

void DSSelectionAlert::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // DS Selection Alert sequence
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_DS_SEL_ALERT );
    f.mData1 = lo64BitData;
    f.mData2 = hi64BitData;
    pResults->AddFrame( f );
}

// ********************************************************************************

void DSActivationCodePreamble::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // DS Activation Code sequence preamble
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE );
    f.mData1 = bits.size();
    pResults->AddFrame( f );
}

// ********************************************************************************

void DSActivationCode::Clear()
{
    data = 0;
    bits.clear();
}

void DSActivationCode::AddFrames( SWDAnalyzerResults* pResults ) const
{
    Frame f;

    // DS Activation code
    f.mStartingSampleInclusive = bits.front().GetStartSample();
    f.mEndingSampleInclusive = bits.back().GetEndSample();
    f.mType = static_cast<U8>( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE );
    f.mData1 = data;
    pResults->AddFrame( f );
}

// ********************************************************************************

void SWDRequestFrame::SetRequestByte( U8 requestByte )
{
    mData1 = requestByte;
}

U8 SWDRequestFrame::GetAddr() const
{
    return static_cast<U8>( ( mData1 >> 1 ) & 0xc );
}

bool SWDRequestFrame::IsRead() const
{
    return ( mFlags & IS_READ ) != 0;
}

bool SWDRequestFrame::IsAccessPort() const
{
    return ( mFlags & IS_ACCESS_PORT ) != 0;
}

bool SWDRequestFrame::IsDebugPort() const
{
    return !IsAccessPort();
}

void SWDRequestFrame::SetRegister( SWDRegisters reg )
{
    mData2 = static_cast<U64>(reg);
}

SWDRegisters SWDRequestFrame::GetRegister() const
{
    return SWDRegisters( mData2 );
}

std::string SWDRequestFrame::GetRegisterName() const
{
    return ::GetRegisterName( GetRegister() );
}

// ********************************************************************************

const U16 SWDParser::SEQUENCE_JTAG_SERIAL = 0x0000u;         // 0b0000_0000_0000 transmitted LSB first, JTAG-Serial
const U8 SWDParser::SEQUENCE_SW_DP = 0x1Au;            // 0b0001_1010 transmitted LSB first, ARM CoreSight SW-DP
const U8 SWDParser::SEQUENCE_JTAG_DP = 0x0Au;                  // 0b0000_1010 transmitted LSB first, ARM CoreSight JTAG-DP

SWDBit SWDParser::ParseBit()
{
    SWDBit rbit;

    assert( mSWCLK->GetBitState() == BIT_LOW );

    rbit.lowStart = mSWCLK->GetSampleNumber();

    // sample the rising edge 1 sample before the the actual
    mSWCLK->AdvanceToAbsPosition( mSWCLK->GetSampleOfNextEdge() - 1 );
    mSWDIO->AdvanceToAbsPosition( mSWCLK->GetSampleNumber() );
    rbit.rising = mSWCLK->GetSampleNumber();
    rbit.stateRising = mSWDIO->GetBitState();
    mSWCLK->AdvanceToNextEdge();
    mSWDIO->AdvanceToAbsPosition( mSWCLK->GetSampleNumber() );

    /*
    // go to the rising eggde
    mSWCLK->AdvanceToNextEdge();
    mSWDIO->AdvanceToAbsPosition(mSWCLK->GetSampleNumber());

    rbit.rising = mSWCLK->GetSampleNumber();
    rbit.state_rising = mSWDIO->GetBitState();
    */

    // go to the falling edge
    mSWCLK->AdvanceToNextEdge();
    mSWDIO->AdvanceToAbsPosition( mSWCLK->GetSampleNumber() );

    rbit.falling = mSWCLK->GetSampleNumber();
    rbit.stateFalling = mSWDIO->GetBitState();

    rbit.lowEnd = mSWCLK->GetSampleOfNextEdge();

    return rbit;
}

void SWDParser::CopyBits( std::deque<SWDBit>& destination, const size_t numBits )
{
    // clear destination before populating
    destination.clear();
    if( numBits > mBitsBuffer.size() / 2 )
    {
        // more than half of the buffer needs to be copied, so it will be faster to do the following
        // 1) copy the tail of the buffer into a temporary object, starting from the next after the given bit number
        std::deque<SWDBit> mBitsBufferTail( mBitsBuffer.begin() + numBits, mBitsBuffer.end() );
        // 2) strip tail of buffer starting from the next after the given bit number
        mBitsBuffer.erase( mBitsBuffer.begin() + numBits, mBitsBuffer.end() );
        // 3) give the bits to the destination object by swap with mBitsBuffer
        std::swap( destination, mBitsBuffer );
        // 4) keep tail of buffer because that one is probably next operation's start bit
        std::swap( mBitsBuffer, mBitsBufferTail );
    }
    else
    {
        // less than half of the buffer needs to be copied, so it will be faster to do the following
        // 1) copy the buffer head up to the given bit into a temporary object
        std::deque<SWDBit> mBitsBufferHead( mBitsBuffer.begin(), mBitsBuffer.begin() + numBits );
        // 2) strip head up to the given bit
        mBitsBuffer.erase( mBitsBuffer.begin(), mBitsBuffer.begin() + numBits );
        // 3) give the bits to the destination object by swap with the temporary object
        std::swap( destination, mBitsBufferHead );
    }
}

bool SWDParser::IsAtLeast( const size_t numBits, const BitState bit )
{
    bool result = true;
    size_t bitCnt = 0;

    // we need at least num_bits with a given value
    while( bitCnt < numBits )
    {
        if( bitCnt >= mBitsBuffer.size() )
        {
            mBitsBuffer.push_back( ParseBit() );
        }

        // we can't have a bit other than given value
        if( mBitsBuffer[ bitCnt ].IsHigh() != ( bit == BIT_HIGH ) )
        {
            // Not enough bits with given value
            result = false;
            break;
        };
        bitCnt++;
    }

    return result;
}

size_t SWDParser::BitCount( const BitState bit, const size_t startingFromBit )
{
    size_t bitCnt = 0;

    // make sure that the startinng bit is placed in buffer
    BufferBits( startingFromBit );
    // Process all bits with given value
    while( true )
    {
        const size_t bitIndex = startingFromBit + bitCnt;
        if( bitIndex >= mBitsBuffer.size() )
        {
            mBitsBuffer.push_back( ParseBit() );
        }

        if( mBitsBuffer[ bitIndex ].IsHigh() != ( bit == BIT_HIGH ) )
        {
            // the bit at bitIndex does not match to the given value
            break;
        }
        bitCnt++;
    }

    return bitCnt;
}

bool SWDParser::IsU8Sequence( const U8* sequence, const size_t bitCnt )
{
    bool sequenceMatched = true;

    // make sure that the required bit count are placed in buffer
    BufferBits( bitCnt );
    // Check for the given sequence value, transmitted LSB first
    for( size_t sequenceCnt = 0u; sequenceCnt < bitCnt; sequenceCnt++ )
    {
        size_t index = sequenceCnt / ( sizeof( U8 ) * 8u );
        size_t shift = sequenceCnt % ( sizeof( U8 ) * 8u );
        if( mBitsBuffer[ sequenceCnt ].IsHigh() != ( ( ( sequence[ index ] >> shift ) & 0b1 ) == 0b1 ) )
        {
            sequenceMatched = false;
            break;
        }
    }

    return sequenceMatched;
}

bool SWDParser::IsU16Sequence( const U16* sequence, const size_t bitCnt )
{
    bool sequenceMatched = true;

    // make sure that the required bit count are placed in buffer
    BufferBits( bitCnt );
    // Check for the given sequence value, transmitted LSB first
    for( size_t sequenceCnt = 0u; sequenceCnt < bitCnt; sequenceCnt++ )
    {
        size_t index = sequenceCnt / ( sizeof( U16 ) * 8u );
        size_t shift = sequenceCnt % ( sizeof( U16 ) * 8u );
        if( mBitsBuffer[ sequenceCnt ].IsHigh() != ( ( ( sequence[ index ] >> shift ) & 0b1 ) == 0b1 ) )
        {
            sequenceMatched = false;
            break;
        }
    }

    return sequenceMatched;
}

bool SWDParser::IsU32Sequence( const U32* sequence, const size_t bitCnt )
{
    bool sequenceMatched = true;

    // make sure that the required bit count are placed in buffer
    BufferBits( bitCnt );
    // Check for the given sequence value, transmitted LSB first
    for( size_t sequenceCnt = 0u; sequenceCnt < bitCnt; sequenceCnt++ )
    {
        size_t index = sequenceCnt / ( sizeof( U32 ) * 8u );
        size_t shift = sequenceCnt % ( sizeof( U32 ) * 8u );
        if( mBitsBuffer[ sequenceCnt ].IsHigh() != ( ( ( sequence[ index ] >> shift ) & 0b1 ) == 0b1 ) )
        {
            sequenceMatched = false;
            break;
        }
    }

    return sequenceMatched;
}

bool SWDParser::IsU64Sequence( const U64* sequence, const size_t bitCnt )
{
    bool sequenceMatched = true;

    // make sure that the required bit count are placed in buffer
    BufferBits( bitCnt );
    // Check for the given sequence value, transmitted LSB first
    for( size_t sequenceCnt = 0u; sequenceCnt < bitCnt; sequenceCnt++ )
    {
        size_t index = sequenceCnt / ( sizeof( U64 ) * 8u );
        size_t shift = sequenceCnt % ( sizeof( U64 ) * 8u );
        if( mBitsBuffer[ sequenceCnt ].IsHigh() != ( ( ( sequence[ index ] >> shift ) & 0b1 ) == 0b1 ) )
        {
            sequenceMatched = false;
            break;
        }
    }

    return sequenceMatched;
}

SWDParser::SWDParser()
    : mSWDIO( 0 ),
      mSWCLK( 0 ),
      mAnalyzer(),
      mLastFrameType( SwdFrameTypes::SWD_FT_LINE_RESET ),
      mCurrentProtocol( DebugProtocol::DPROTOCOL_UNKNOWN ),
      mDPVersion( DPVersion::DP_V0 ),
      mTran(),
      mReset(),
      mJtagToSwd(),
      mSwdToJtag(),
      mErrorBits(),
      mIdleCycles(),
      mJtagToDs(),
      mSwdToDs(),
      mDsSelectionAlert(),
      mDsActivationCode(),
      mSelectRegister()

{
}

void SWDParser::Setup( AnalyzerChannelData* pSWDIO, AnalyzerChannelData* pSWCLK, SWDAnalyzer* pAnalyzer )
{
    mSWDIO = pSWDIO;
    mSWCLK = pSWCLK;

    mAnalyzer = pAnalyzer;

    // skip the SWCLK high
    if( mSWCLK->GetBitState() == BIT_HIGH )
    {
        mSWCLK->AdvanceToNextEdge();
        mSWDIO->AdvanceToAbsPosition( mSWCLK->GetSampleNumber() );
    }
}

void SWDParser::Clear()
{
    mBitsBuffer.clear();
    mSelectRegister = 0;
    mDPVersion = DPVersion::DP_V0;
    mLastFrameType = SwdFrameTypes::SWD_FT_LINE_RESET;
    mCurrentProtocol = DebugProtocol::DPROTOCOL_UNKNOWN;
}

void SWDParser::BufferBits( const size_t numBits )
{
    while( mBitsBuffer.size() < numBits )
        mBitsBuffer.push_back( ParseBit() );
}

SwdFrameTypes SWDParser::GetLastFrameType() const
{
    return mLastFrameType;
}

DebugProtocol SWDParser::GetCurrentProtocol() const
{
    return mCurrentProtocol;
}

void SWDParser::SetCurrentProtocol( DebugProtocol protocol )
{
    mCurrentProtocol = protocol;
}

void SWDParser::SetLastFrameType( SwdFrameTypes frame )
{
    mLastFrameType = frame;
}

void SWDParser::SetDPVersion( DPVersion version )
{
    mDPVersion = version;
}

DPVersion SWDParser::GetDPVersion() const
{
    return mDPVersion;
}

void SWDParser::SetNumTurnarounds( U8 num )
{
    mTran.SetTurnaroundNumber( num );
}

void SWDParser::SetOverrunDetection( bool enabled )
{
    mTran.SetOrunDetect( enabled );
}

void SWDParser::SetSelectRegister( U32 value )
{
    mSelectRegister = value;
}


bool SWDParser::IsOperation()
{
    mTran.Clear();

    // read enough bits so that we don't have to worry of subscripts out of range
    // 8bit request + turnaround bit(s) + three bits ACK = 12 bits in case 1 turnaround bit
    size_t headerLength = 8u + mTran.GetTurnaroundNumber() + 3u;
    BufferBits( headerLength );

    // turn the request bits into a byte
    U8 reqByte = 0u;
    for( size_t cnt = 0u; cnt < 8u; ++cnt )
    {
        reqByte >>= 1u;
        reqByte |= ( mBitsBuffer[ cnt ].IsHigh() ? 0x80u : 0u );
    }
    mTran.requestByte = reqByte;
    bool isTargetSel = ( reqByte == 0x99u ); // During the response phase of a write to the TARGETSEL register, the target does not drive the line.
                                             // So ACK bits shouldn't be analysed

    SWDRequestByte requestByte(reqByte);

    // are the request's constant bits (start, stop, park and parity) wrong?
    if( !requestByte.IsByteValid() )
    {
        return false;
    }

    // get the indivitual bits
    mTran.reqAPnDP = requestByte.GetAPnDP();
    mTran.reqRnW = requestByte.GetRnW();
    mTran.addr = requestByte.GetAddr();

    // Set the actual register in this operation based on the data from the request
    // and the previous select register state.
    mTran.SetDPVer( mDPVersion );
    mTran.SetRegister( mSelectRegister );

    // get the ACK value
    mTran.reqAck = ( mBitsBuffer[ 8u + mTran.GetTurnaroundNumber() + 0u ].stateRising == BIT_HIGH ? ( 1u << 0u ) : 0u ) +
                ( mBitsBuffer[ 8u + mTran.GetTurnaroundNumber() + 1u ].stateRising == BIT_HIGH ? ( 1u << 1u ) : 0u ) +
                ( mBitsBuffer[ 8u + mTran.GetTurnaroundNumber() + 2u ].stateRising == BIT_HIGH ? ( 1u << 2u ) : 0u );

    // handling non-OK response if Overrun detection is not enabled
    if( !mTran.orundetect && !isTargetSel && ( mTran.reqAck != static_cast<U8>( SWDAcks::ACK_OK ) ) )
    {
        size_t badTranLength = 8u + mTran.GetTurnaroundNumber() + 3u;
        if( ( mTran.reqAck == static_cast<U8>( SWDAcks::ACK_WAIT ) ) || ( mTran.reqAck == static_cast<U8>( SWDAcks::ACK_FAULT ) ) )
        {
            // 8bit request + turnaround bit(s) + three bits ACK + turnaround bit(s) = 13 bits in case 1 turnaround bit
            // read turnaround bit(s) after ACK
            badTranLength += mTran.GetTurnaroundNumber();
            // read one more bit that is turnaround after ACK
            BufferBits( badTranLength );
        }
        // give the bits to the tran object
        CopyBits( mTran.bits, badTranLength );

        return true;
    }

    // for read operation:  8bit request + turnaround bit(s) + three bits ACK
    //                      + 32bit data + parity + turnaround bit(s) = 46 bits in case 1 turnaround bit
    // for write operation: 8bit request + turnaround bit(s) + three bits ACK
    //                      + turnaround bit(s) + 32bit data + parity = 46 bits in case 1 turnaround bit
    size_t tranLength = 8u + mTran.GetTurnaroundNumber() + 3u + 32u + 1u + mTran.GetTurnaroundNumber();

    BufferBits( tranLength );

    std::deque<SWDBit>::iterator bi( mBitsBuffer.begin() + headerLength );
    // turnaround if write operation
    if( !mTran.IsRead() )
    {
        // jump over turnaround bit if it is write operation
        bi += mTran.GetTurnaroundNumber();
    }

    // read the data
    mTran.data = 0u;
    U8 check = 0u;
    size_t ndx;
    for( ndx = 0u; ndx < 32u; ndx++ )
    {
        mTran.data >>= 1u;

        if( bi[ ndx ].IsHigh() )
        {
            mTran.data |= 0x80000000u;
            ++check;
        }
    }

    // data parity
    mTran.dataParity = bi[ ndx ].IsHigh() ? 1u : 0u;

    mTran.dataParityOk = ( mTran.dataParity == ( check & 1u ) );

    if( !mTran.orundetect && !mTran.dataParityOk )
        return false;

    // give the bits to the tran object
    CopyBits( mTran.bits, tranLength );

    return true;
}

bool SWDParser::IsLineReset()
{
    const size_t minimumBitCnt = 50; // Required at least 50 bits
    mReset.Clear();                  // Clear the bits

    // we need at least 50 bits with a value of 1
    if( !IsAtLeast( minimumBitCnt, BIT_HIGH ) )
    {
        // Not enough bits with value 1
        return false;
    };

    // Get number of continuous 1
    size_t lineResetCnt = minimumBitCnt + BitCount( BIT_HIGH, minimumBitCnt );

    // Buffer 7 more bits
    BufferBits( lineResetCnt + 7u );

    U16 lastBits = 0u;
    // get bits in the offset range from -5 to +7 around lineResetCnt
    for( size_t i = lineResetCnt - 5u; i < lineResetCnt + 7u; ++i )
    {
        lastBits <<= 1u;
        if( mBitsBuffer[ i ].IsHigh() )
        {
            lastBits |= 1u;
        }
    }

    // Check if the last bits "1" is not the beginning of the next swd request
    for( size_t i = 0; i < 5u; ++i )
    {
        U8 reqByte = ( lastBits >> ( 4u - i ) );
        SWDRequestByte requestByte( reqByte );
        if( requestByte.IsByteValid() )
        {
            // Detected a valid SWD request butted to SWD reset sequence
            lineResetCnt -= ( 5u - i );
            break;
        }
    }

    // give the bits to the reset object
    CopyBits( mReset.bits, lineResetCnt );

    return true;
}

bool SWDParser::IsJtagToSwd()
{
    mJtagToSwd.Clear();
    const U16 sequence = 0xE79E;           // 0xE79E, transmitted LSB first
    const U16 sequenceDeprecated = 0xEDB6; // 0xEDB6, transmitted LSB first

    // Check for 0xE79E value, transmitted LSB first
    bool sequenceMatched = IsU16Sequence( &sequence );

    if( sequenceMatched )
    {
        mJtagToSwd.data = sequence;
    }
    else
    {
        // Check for deprecated 0xEDB6 value, transmitted LSB first
        sequenceMatched = IsU16Sequence( &sequenceDeprecated );

        if( sequenceMatched )
        {
            mJtagToSwd.deprecated = true;
            mJtagToSwd.data = sequenceDeprecated;
        }
        else
        {
            return false;
        }
    }

    // give the bits to the reset object
    CopyBits( mJtagToSwd.bits, 16u );

    return true;
}

bool SWDParser::IsSwdToJtag()
{
    mSwdToJtag.Clear();
    const U16 sequence = 0xE73C;           // 0xE73C, transmitted LSB first
    const U16 sequenceDeprecated = 0xAEAE; // 0xAEAE, transmitted LSB first

    // Check for 0xE73C value, transmitted LSB first
    bool sequenceMatched = IsU16Sequence( &sequence );

    if( sequenceMatched )
    {
        mSwdToJtag.data = sequence;
    }
    else
    {
        // Check for deprecated 0xAEAE value, transmitted LSB first
        sequenceMatched = IsU16Sequence( &sequenceDeprecated );

        if( sequenceMatched )
        {
            mSwdToJtag.deprecated = true;
            mSwdToJtag.data = sequenceDeprecated;
        }
        else
        {
            return false;
        }
    }

    // give the bits to the reset object
    CopyBits( mSwdToJtag.bits, 16u );

    return true;
}

bool SWDParser::IsIdleCycles()
{
    mIdleCycles.Clear(); // Clear the bits

    // Get number of continuous 0
    size_t idleCyclesCnt = BitCount( BIT_LOW, 0 );
    if( idleCyclesCnt == 0 )
    {
        // No idle cycles found
        return false;
    }
    // give the bits to the idleCycles object
    CopyBits( mIdleCycles.bits, idleCyclesCnt );

    return true;
}

bool SWDParser::IsJtagTlr()
{
    const size_t minimumBitCnt = 5u; // Required at least 5 bits
    mJtagTlr.Clear();                // Clear the bits

    // we need at least 5 bits with a value of 1
    if( !IsAtLeast( minimumBitCnt, BIT_HIGH ) )
    {
        // Not enough bits with value 1
        return false;
    };

    // Get number of continuous 1
    size_t highBitCnt = minimumBitCnt + BitCount( BIT_HIGH, minimumBitCnt );
    // give the bits to the JTAG Test Logic Reset object
    CopyBits( mJtagTlr.bits, highBitCnt );

    return true;
}

bool SWDParser::IsJtagToDs()
{
    mJtagToDs.Clear();
    const U32 sequence = 0x33BBBBBA; // 0x33BBBBBA, 31 bits transmitted LSB first

    bool sequenceMatched = IsU32Sequence( &sequence, 31u );

    if( sequenceMatched )
    {
        mJtagToDs.data = sequence;
    }
    else
    {
        return false;
    }
    // give the bits to the JTAG-to-DS object
    CopyBits( mJtagToDs.bits, 31u );

    return true;
}

bool SWDParser::IsSwdToDs()
{
    mSwdToDs.Clear();
    const U16 sequence = 0xE3BC; // 0xE3BC, transmitted LSB first

    // Check for 0xE73C value, transmitted LSB first
    bool sequenceMatched = IsU16Sequence( &sequence );

    if( sequenceMatched )
    {
        mSwdToDs.data = sequence;
    }
    else
    {
        return false;
    }

    // give the bits to the SWD-to-DS object
    CopyBits( mSwdToDs.bits, 16u );

    return true;
}

bool SWDParser::IsDsSelectionAlertPreamble()
{
    const size_t minimumBitCnt = 8u;   // Required at least 8 bits
    mDsSelectionAlertPreamble.Clear(); // Clear the bits

    // we need at least 8 bits with a value of 1
    if( !IsAtLeast( minimumBitCnt, BIT_HIGH ) )
    {
        // Not enough bits with value 1
        return false;
    };

    // Get number of continuous 1
    size_t highBitCnt = minimumBitCnt + BitCount( BIT_HIGH, minimumBitCnt );
    // give the bits to the Selection Alert sequence preamble object
    CopyBits( mDsSelectionAlertPreamble.bits, highBitCnt );

    return true;
}

bool SWDParser::IsDsSelectionAlert()
{
    mDsSelectionAlert.Clear();
    const U64 sequence[ 2u ] = {
        0x86852D956209F392u, // Low 64 bits transmitted LSB first
        0x19BC0EA2E3DDAFE9u  // Hihg 64 bits transmitted LSB first
    };

    bool sequenceMatched = IsU64Sequence( sequence, 128u );

    if( sequenceMatched )
    {
        mDsSelectionAlert.hi64BitData = sequence[ 1u ];
        mDsSelectionAlert.lo64BitData = sequence[ 0u ];
    }
    else
    {
        return false;
    }
    // give the bits to the DS Selection Alert sequence object
    CopyBits( mDsSelectionAlert.bits, 128u );

    return true;
}

bool SWDParser::IsDsActivationCodePreamble()
{
    mDsActivationCodePreamble.Clear();
    const U8 sequence = 0x00; // 0x00, 4 cycles with SWDIOTMS LOW

    // Check for 0x00 value, transmitted LSB first
    bool sequenceMatched = IsU8Sequence( &sequence, 4u );

    if( !sequenceMatched )
    {
        return false;
    }

    // give the bits to the SWD-to-DS object
    CopyBits( mDsActivationCodePreamble.bits, 4u );

    return true;
}

bool SWDParser::IsDsActivationCode()
{
    mDsActivationCode.Clear();
    size_t sequenceLength = 0;

    bool sequenceMatched = IsU16Sequence( &SEQUENCE_JTAG_SERIAL, 12u );

    if( sequenceMatched )
    {
        mDsActivationCode.data = SEQUENCE_JTAG_SERIAL;
        sequenceLength = 12u;
    }
    else
    {
        sequenceMatched = IsU8Sequence( &SEQUENCE_SW_DP );
        if( sequenceMatched )
        {
            mDsActivationCode.data = SEQUENCE_SW_DP;
            sequenceLength = 8u;
        }
        else
        {
            sequenceMatched = IsU8Sequence( &SEQUENCE_JTAG_DP );
            if( sequenceMatched )
            {
                mDsActivationCode.data = SEQUENCE_JTAG_DP;
                sequenceLength = 8u;
            }
            else
            {
                return false;
            }
        }
    }
    // give the bits to the DS Activation Code sequence object
    CopyBits( mDsActivationCode.bits, sequenceLength );

    return true;
}


const SWDBaseSequnce& SWDParser::GetLineReset() const
{
    return mReset;
}
const SWDBaseSequnce& SWDParser::GetJtagToSwd() const
{
    return mJtagToSwd;
}
const SWDBaseSequnce& SWDParser::GetSwdToJtag() const
{
    return mSwdToJtag;
}
const SWDBaseSequnce& SWDParser::GetOperation() const
{
    return mTran;
}
const SWDBaseSequnce& SWDParser::GetIdleCycles() const
{
    return mIdleCycles;
}
const SWDBaseSequnce& SWDParser::GetJtagTlr() const
{
    return mJtagTlr;
}
const SWDBaseSequnce& SWDParser::GetJtagToDs() const
{
    return mJtagToDs;
}
const SWDBaseSequnce& SWDParser::GetSwdToDs() const
{
    return mSwdToDs;
}
const SWDBaseSequnce& SWDParser::GetDsSelectionAlertPreamble() const
{
    return mDsSelectionAlertPreamble;
}
const SWDBaseSequnce& SWDParser::GetDsSelectionAlert() const
{
    return mDsSelectionAlert;
}
const SWDBaseSequnce& SWDParser::GetDsActivationCodePreamble() const
{
    return mDsActivationCodePreamble;
}
const SWDBaseSequnce& SWDParser::GetDsActivationCode() const
{
    return mDsActivationCode;
}
SWDErrorBits& SWDParser::GetErrorBits()
{
    return mErrorBits;
}

void SWDParser::SetLineReset()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_LINE_RESET;

    // Forcing DP registers that are dependent on the SWD line reset.
    // DLCR.TURNROUND <- 0; Set number of turnaround cycles to 1
    mTran.SetTurnaroundNumber( 1u );
    // SELECT.DPBANKSEL <- 0;
    mSelectRegister &= 0xFFFFFFF0u;
}
void SWDParser::SetJtagToSwd()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_JTAG_TO_SWD;
    mCurrentProtocol = DebugProtocol::DPROTOCOL_SWD;
}
void SWDParser::SetSwdToJtag()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_SWD_TO_JTAG;
    mCurrentProtocol = DebugProtocol::DPROTOCOL_JTAG;
}
void SWDParser::SetOperation()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_OPERATION;

    // Make further decision based on ACK
    if( mTran.reqAck == static_cast<U8>( SWDAcks::ACK_OK ) )
    {
        mCurrentProtocol = DebugProtocol::DPROTOCOL_SWD;
        switch( mTran.reg )
        {
            case SWDRegisters::SWDR_DP_DPIDR:
                // if this is read DPIDR, capture the Version of the DP architecture implemented
                switch( ( mTran.data >> 12u ) & 0x0fu )
                {
                case 0x01u:
                    mDPVersion = DPVersion::DP_V1;
                    break;
                case 0x02u:
                    mDPVersion = DPVersion::DP_V2;
                    break;
                case 0x03u:
                    mDPVersion = DPVersion::DP_V3;
                    break;
                default:
                    mDPVersion = DPVersion::DP_V0;
                    break;
                }
                break;
            case SWDRegisters::SWDR_DP_CTRL_STAT:
                // Capture ORUNDETECT bit of DP CTRL/STAT register
                mTran.SetOrunDetect( ( mTran.data & 0x00000001u ) != 0u );
                break;
            case SWDRegisters::SWDR_DP_SELECT:
                // if this is a SELECT register write, remember the value
                mSelectRegister = mTran.data;
                break;
            case SWDRegisters::SWDR_DP_DLCR:
                // if this is a DLCR register read or write, update number of turnaround cycles
                mTran.SetTurnaroundNumber( ( ( mTran.data >> 8u ) & 0x03u ) + 1u );
                break;
            default:
                break;
        }
    }
    else
    {
        mCurrentProtocol = DebugProtocol::DPROTOCOL_UNKNOWN;
    }
}

void SWDParser::SetIdleCycles()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_IDLE_CYCLE;
}
void SWDParser::SetJtagTlr()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_JTAG_TLR;
}
void SWDParser::SetJtagToDs()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_JTAG_TO_DS;
    mCurrentProtocol = DebugProtocol::DPROTOCOL_DORMANT;
}
void SWDParser::SetSwdToDs()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_SWD_TO_DS;
    mCurrentProtocol = DebugProtocol::DPROTOCOL_DORMANT;
}
void SWDParser::SetDsSelectionAlertPreamble()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE;
}
void SWDParser::SetDsSelectionAlert()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_DS_SEL_ALERT;
}
void SWDParser::SetDsActivationCodePreamble()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE;
}
void SWDParser::SetDsActivationCode()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE;
    switch(mDsActivationCode.data)
    {
        case SEQUENCE_JTAG_SERIAL:
        case SEQUENCE_JTAG_DP:
            mCurrentProtocol = DebugProtocol::DPROTOCOL_JTAG;
            break;
        case SEQUENCE_SW_DP:
            mCurrentProtocol = DebugProtocol::DPROTOCOL_SWD;
            break;
        default:
            mCurrentProtocol = DebugProtocol::DPROTOCOL_UNKNOWN;
            break;
    }
}

void SWDParser::SetErrorBits()
{
    mLastFrameType = SwdFrameTypes::SWD_FT_ERROR;
}

SWDBit SWDParser::PopFrontBit()
{
    assert( !mBitsBuffer.empty() );

    SWDBit retVal( mBitsBuffer.front() );

    // shift the elements by 1
    std::copy( mBitsBuffer.begin() + 1, mBitsBuffer.end(), mBitsBuffer.begin() );
    mBitsBuffer.pop_back();

    return retVal;
}
