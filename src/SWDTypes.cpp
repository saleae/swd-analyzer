#include <cassert>

#include <algorithm>
#include <iterator>

#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>

#include "SWDAnalyzer.h"
#include "SWDTypes.h"
#include "SWDUtils.h"

// DP registers

const std::map<U8, std::vector<ADIState::DPRegister>> ADIState::DP_REGISTERS = {
    { 0x00u,
        {
            { {}, RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 , SWDRegisters::SWDR_DP_DPIDR },
            { { 0x00u }, RegMask::REG_READ | RegMask::REG_V3, SWDRegisters::SWDR_DP_DPIDR },
            { { 0x01u }, RegMask::REG_READ | RegMask::REG_V3, SWDRegisters::SWDR_DP_DPIDR1 },
            { { 0x02u }, RegMask::REG_READ | RegMask::REG_V3, SWDRegisters::SWDR_DP_BASEPTR0 },
            { { 0x03u }, RegMask::REG_READ | RegMask::REG_V3, SWDRegisters::SWDR_DP_BASEPTR1 },
            { {}, RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_ABORT }
        }
    },
    { 0x04u,
        {
            { { 0x00u }, RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_CTRL_STAT },
            { { 0x01u }, RegMask::REG_READ | RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_DLCR },
            { { 0x02u }, RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_TARGETID },
            { { 0x03u }, RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_DLPIDR },
            { { 0x04u }, RegMask::REG_READ | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_EVENTSTAT },
            { { 0x05u }, RegMask::REG_WRITE | RegMask::REG_V3, SWDRegisters::SWDR_DP_SELECT1 },
        }
    },
    { 0x08u,
        {
            { {}, RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_RESEND },
            { {}, RegMask::REG_WRITE | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_SELECT }
        }
    },
    { 0x0Cu,
        {
            { {}, RegMask::REG_READ | RegMask::REG_V1 | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_RDBUFF },
            { {}, RegMask::REG_WRITE | RegMask::REG_V2 | RegMask::REG_V3, SWDRegisters::SWDR_DP_TARGETSEL }
        }
    },
};

U8 operator|( const RegMask a, const RegMask b )
{
    return ( static_cast<U8>( a ) | static_cast<U8>( b ) );
}
U8 operator|( const U8 a, const RegMask b )
{
    return ( a | static_cast<U8>( b ) );
}

SeqCmpResult BestMach( const SeqCmpResult a, const SeqCmpResult b )
{
    return ( ( static_cast<U8>( a ) >= static_cast<U8>( b ) ) ? a : b );
}

const std::map<U8, SWDRegisters> ADIState::MEM_AP_ADI_V5 = {
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

const std::map<U16, SWDRegisters> ADIState::MEM_AP_ADI_V6 = {
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

const std::string UNKNOWN_FRAME_STRING = "Unknown frame";  
const std::string& SWDFrame::GetSwdFrameName( SwdFrameTypes frame )
{
    auto search = FRAME_NAMES.find( frame );
    if( search != FRAME_NAMES.end() )
    {
        return search->second;
    }
    else
    {
        return UNKNOWN_FRAME_STRING;
    }
}

const std::map<SwdFrameTypes, std::string> SWDFrame::FRAMEV2_NAMES = {
    { SwdFrameTypes::SWD_FT_ERROR, "ERROR" },
    { SwdFrameTypes::SWD_FT_IGNORED, "IGNORED" },
    { SwdFrameTypes::SWD_FT_LINE_RESET, "LINE_RESET" },
    { SwdFrameTypes::SWD_FT_JTAG_TO_SWD, "JTAG_TO_SWD" },
    { SwdFrameTypes::SWD_FT_SWD_TO_JTAG, "SWD_TO_JTAG" },
    { SwdFrameTypes::SWD_FT_IDLE_CYCLE, "IDLE_CYCLE" },
    { SwdFrameTypes::SWD_FT_JTAG_TLR, "JTAG_TLR" },
    { SwdFrameTypes::SWD_FT_JTAG_TO_DS, "JTAG_TO_DS" },
    { SwdFrameTypes::SWD_FT_SWD_TO_DS, "SWD_TO_DS" },
    { SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE, "DS_SEL_ALERT_PREAMBLE" },
    { SwdFrameTypes::SWD_FT_DS_SEL_ALERT, "DS_SEL_ALERT" },
    { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE, "DS_ACTIVATION_CODE_PREAMBLE" },
    { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE, "DS_ACTIVATION_CODE" }
};

const std::string UNKNOWN_FRAMEV2_STRING = "UNKNOWN";
const std::string& SWDFrame::GetSwdFrameV2Name( SwdFrameTypes frame )
{
    auto search = FRAMEV2_NAMES.find( frame );
    if( search != FRAMEV2_NAMES.end() )
    {
        return search->second;
    }
    else
    {
        return UNKNOWN_FRAMEV2_STRING;
    }
}

bool operator==( const U8 a, const SWDAcks b )
{
    return ( a == static_cast<const U8>(b) );
}

bool operator!=(const U8 a, const SWDAcks b)
{
    return !( a == b );
}

const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> SWDOperation::OP_REQ_AND_ACK_SIZE = {
    // DLCR.TURNROUND == 0 (1 cycle)
    { 
      { OpBitRange::OP_REQUEST_RANGE, OpItemSize::OP_REQUEST_SIZE  },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_1_TURNAROUND_SIZE },
      { OpBitRange::OP_ACK_RANGE, OpItemSize::OP_ACK_SIZE }
    },
    // DLCR.TURNROUND == 1 (2 cycle)
    { 
      { OpBitRange::OP_REQUEST_RANGE, OpItemSize::OP_REQUEST_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_2_TURNAROUND_SIZE },
      { OpBitRange::OP_ACK_RANGE, OpItemSize::OP_ACK_SIZE }
    },
    // DLCR.TURNROUND == 2 (3 cycle)
    {
      { OpBitRange::OP_REQUEST_RANGE, OpItemSize::OP_REQUEST_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_3_TURNAROUND_SIZE },
      { OpBitRange::OP_ACK_RANGE, OpItemSize::OP_ACK_SIZE }
    },
    // DLCR.TURNROUND == 3 (4 cycle)
    { 
      { OpBitRange::OP_REQUEST_RANGE, OpItemSize::OP_REQUEST_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_4_TURNAROUND_SIZE },
      { OpBitRange::OP_ACK_RANGE, OpItemSize::OP_ACK_SIZE } 
    }
};

const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> SWDOperation::OP_RDATA_SIZE = {
    // DLCR.TURNROUND == 0 (1 cycle)
    { 
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_1_TURNAROUND_SIZE }
    },
    // DLCR.TURNROUND == 1 (2 cycle)
    { 
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_2_TURNAROUND_SIZE }
    },
    // DLCR.TURNROUND == 2 (3 cycle)
    {
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_3_TURNAROUND_SIZE }
    },
    // DLCR.TURNROUND == 3 (4 cycle)
    {
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE },
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_4_TURNAROUND_SIZE }
    }
};

const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> SWDOperation::OP_WDATA_SIZE = {
    // DLCR.TURNROUND == 0 (1 cycle)
    { 
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_1_TURNAROUND_SIZE },
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE }
    },
    // DLCR.TURNROUND == 1 (2 cycle)
    {
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_2_TURNAROUND_SIZE },
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE }
    },
    // DLCR.TURNROUND == 2 (3 cycle)
    {
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_3_TURNAROUND_SIZE },
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE }
    },
    // DLCR.TURNROUND == 3 (4 cycle)
    {
      { OpBitRange::OP_TURNAROUND_RANGE, OpItemSize::OP_4_TURNAROUND_SIZE },
      { OpBitRange::OP_DATA_RANGE, OpItemSize::OP_DATA_SIZE },
      { OpBitRange::OP_DATA_PARITY_RANGE, OpItemSize::OP_DATA_PARITY_SIZE }
    }
};

const std::vector<U16> JTAG_TO_SWD_SEQUENCE = { 0xE79Eu };               // 0xE79E, transmitted LSB first
const std::vector<U16> JTAG_TO_SWD_DEPRECATED_SEQUENCE = { 0xEDB6u };    // 0xEDB6, transmitted LSB first
const std::vector<U16> SWD_TO_JTAG_SEQUENCE = { 0xE73Cu };               // 0xE73C, transmitted LSB first
const std::vector<U16> SWD_TO_JTAG_DEPRECATED_SEQUENCE = { 0xAEAEu };    // 0xAEAE, transmitted LSB first
const std::vector<U32> JTAG_TO_DS_SEQUENCE = { 0x33BBBBBAu };            // 0x33BBBBBA, 31 bits transmitted LSB first
const std::vector<U16> SWD_TO_DS_SEQUENCE = { 0xE3BCu };                 // 0xE3BC, transmitted LSB first
const std::vector<U64> DS_SELECTION_ALERT_SEQUENCE = { 0x86852D956209F392u, 0x19BC0EA2E3DDAFE9u }; // 0x86852D956209F392 0x19BC0EA2E3DDAFE9 transmitted LSB first
const std::vector<U8> DS_ACTIVATION_CODE_PREAMBLE = { 0x00u };           // 0x00, 4 cycles with SWDIOTMS LOW
const std::vector<U16> DS_ACTIVATION_CODE_JTAG_SERIAL = { 0x0000u };     // 0b0000_0000_0000 transmitted LSB first, JTAG-Serial
const std::vector<U8> DS_ACTIVATION_CODE_SW_DP = { 0x1Au };              // 0b0001_1010 transmitted LSB first, ARM CoreSight SW-DP
const std::vector<U8> DS_ACTIVATION_CODE_JTAG_DP = { 0x0Au };            // 0b0000_1010 transmitted LSB first, ARM CoreSight JTAG-DP

UintSequence patternJtagToSwd( JTAG_TO_SWD_SEQUENCE );
UintSequence patternJtagToSwdDeprecated( JTAG_TO_SWD_DEPRECATED_SEQUENCE );
UintSequence patternSwdToJtag( SWD_TO_JTAG_SEQUENCE );
UintSequence patternSwdToJtagDeprecated( SWD_TO_JTAG_DEPRECATED_SEQUENCE );
UintSequence patternJtagToDs( JTAG_TO_DS_SEQUENCE, 31u );
UintSequence patternSwdToDs( SWD_TO_DS_SEQUENCE );
UintSequence patternDsSelectionAlert( DS_SELECTION_ALERT_SEQUENCE );
UintSequence patternDsActivationCodePreamble( DS_ACTIVATION_CODE_PREAMBLE, 4u );
UintSequence patternDsActivationCodeJtagSerial( DS_ACTIVATION_CODE_JTAG_SERIAL, 12u );
UintSequence patternDsActivationCodeSwDp( DS_ACTIVATION_CODE_SW_DP );
UintSequence patternDsActivationCodeJtagDp( DS_ACTIVATION_CODE_JTAG_DP );

// SWD request byte of DP TARGETSEL write shifted to LSB zero bit position
const std::vector<U8> opSwdReq_0x99 = { 0x99u >> 1u }; // 0b1001_1001 >> 1 = 0b0100_1100
UintSequence patternSwdReq_0x99( opSwdReq_0x99, 8u - 1u );
std::vector<std::reference_wrapper<UintSequence<U8>>> patternSwdReq = 
{
    patternSwdReq_0x99, // Line reset sequence without followed idele cycles but with followed by a DP TARGETSEL write
};


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

// ********************************************************************************

SWDRequestByte::SWDRequestByte( const U8 byteValue ) : byte( byteValue )
{
}

// ********************************************************************************

void SWDBaseSequence::Clear()
{
    startSample = 0;
    endSample = 0;
    numberCheckedBits = 0u;
    seqCheckResult = SeqCmpResult::SEQ_UNKNOWN;
    bits.clear();
}

void SWDBaseSequence::AddMarkers( SWDAnalyzerResults* pResults ) const
{
}

SWDBaseSequence::SWDBaseSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                                  const std::set<SwdFrameTypes>& previousFrames,
                                ADIState& adiStateRef )
    : seqFrameType( frameType ),
      allowedDebugProtocols( debugProtocols ),
      allowedPreviousFrames( previousFrames ),
      bits(),
      startSample( 0 ),
      endSample( 0 ),
      numberCheckedBits( 0u ),
      seqCheckResult( SeqCmpResult::SEQ_UNKNOWN ),
      adiState( adiStateRef )
{
}

void SWDBaseSequence::UpdateBitInfo()
{
    startSample = bits.front().GetStartSample();
    endSample = bits.back().GetEndSample();
}

bool SWDBaseSequence::BreakMatch( const std::deque<SWDBit>& bits )
{
    if( bits.empty() )
    {
        // Nothing to compare
        seqCheckResult = SeqCmpResult::SEQ_UNKNOWN;
        return true;
    }
    if( bits.front().GetStartSample() != startSample )
    {
        // New secuence detected
        CaptureNewSequence( bits );
    }
    if( seqCheckResult == SeqCmpResult::SEQ_MATCH_COMPLETELY )
    {
        // The operation has been already checked but result has not been consumed yet
        return true;
    }
    if( ( numberCheckedBits != 0u ) && ( seqCheckResult == SeqCmpResult::SEQ_MISMATCH ) )
    {
        // The previously checked bits did not match, so there is no point in comparing the next bits.
        return true;
    }
    return false;
}

void SWDBaseSequence::CaptureNewSequence( const std::deque<SWDBit>& bits )
{
    Clear(); // Clear the bits
    startSample = bits.front().GetStartSample();
}

void SWDBaseSequence::AddFrame( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive, const S64 endingSampleInclusive,
                                const U64 data1, const U64 data2, const SwdFrameTypes type, const U8 flags ) const
{
    Frame frame;
    frame.mStartingSampleInclusive = startingSampleInclusive;
    frame.mEndingSampleInclusive = endingSampleInclusive;
    frame.mData1 = data1;
    frame.mData2 = data2;
    frame.mType = static_cast<U8>( type );
    frame.mFlags = flags;
    pResults->AddFrame( frame );
}

void SWDBaseSequence::AddFrameV2SimpleSequence( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive,
                                                const S64 endingSampleInclusive, const SwdFrameTypes frameType,
                                                const std::size_t bitCount ) const
{
    FrameV2 frameV2;
    frameV2.AddString( "type", SWDFrame::GetSwdFrameV2Name( frameType ).c_str() );
    frameV2.AddInteger( "cycles", bitCount );
    pResults->AddFrameV2( frameV2, "sequence", startingSampleInclusive, endingSampleInclusive );
}

void SWDBaseSequence::AddFrameV2DataSequence( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive,
                                              const S64 endingSampleInclusive, const SwdFrameTypes frameType, const std::size_t bitCount,
                                              const std::vector<U8>& bytes ) const
{
    FrameV2 frameV2;
	frameV2.AddString( "type", SWDFrame::GetSwdFrameV2Name( frameType ).c_str() );
	frameV2.AddInteger( "cycles", bitCount );
    frameV2.AddByteArray( "data", bytes.data(), bytes.size() );
	pResults->AddFrameV2( frameV2, "sequence", startingSampleInclusive, endingSampleInclusive );
}

const std::size_t SWDBaseSequence::GetNumberCheckedBits() const
{
    return numberCheckedBits;
}

const bool SWDBaseSequence::Empty() const
{
    return bits.empty();
}

const bool SWDBaseSequence::IsDebugProtocolAllowed( DebugProtocol protocol ) const
{
    return ( allowedDebugProtocols.find( protocol ) != allowedDebugProtocols.end() );
}

const bool SWDBaseSequence::IsPreviousFrameTypeAllowed( SwdFrameTypes frameType ) const
{
    return ( allowedPreviousFrames.empty() || ( allowedPreviousFrames.find( frameType ) != allowedPreviousFrames.end() ) );
}

void SWDBaseSequence::UpdateAdiState()
{
    adiState.SetLastFrameType( seqFrameType );
}

SWDFixedLengthSequence::SWDFixedLengthSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                                                const std::set<SwdFrameTypes>& previousFrames,
                                              ADIState& adiStateRef )
    : SWDBaseSequence( frameType, debugProtocols, previousFrames, adiStateRef )
{
}

SWDVariableLengthSequence::SWDVariableLengthSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                                                      const std::set<SwdFrameTypes>& previousFrames,
                                                    ADIState& adiStateRef )
    : SWDBaseSequence( frameType, debugProtocols, previousFrames, adiStateRef ), bitCount( 0u )
{
}

void SWDVariableLengthSequence::UpdateBitInfo()
{
    SWDBaseSequence::UpdateBitInfo();
    bitCount = bits.size();
}

// ********************************************************************************

SWDOperation::SWDOperation( ADIState& adiStateRef )
    : SWDFixedLengthSequence( SwdFrameTypes::SWD_FT_OPERATION, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD },
                             { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_IDLE_CYCLE, SwdFrameTypes::SWD_FT_OPERATION,
                               SwdFrameTypes::SWD_FT_ERROR }, 
                             adiStateRef ),
      reqAPnDP( false ),
      reqRnW( false ),
      reqA23Addr( 0u ),
      reqParity( 0u ),
      reqByte( 0u ),
      reqAck( 0u ),
      data( 0 ),
      calculatedDataParity( 0u ),
      dataParity( 0u ),
      dataParityOk( false ),
      opReqAndAckItemSize( &( OP_REQ_AND_ACK_SIZE.front() ) ),
      opRdataItemSize( &( OP_RDATA_SIZE.front() ) ),
      opWdataItemSize( &( OP_WDATA_SIZE.front() ) ),
      opAckOffset( 0u ),
      opDataOffset( 0u ),
      opReqAndAckSize( 0u )
{
}

void SWDOperation::Clear()
{
    SWDBaseSequence::Clear();
    reqRnW = false;
    reqAPnDP = false;
    dataParityOk = false;
    reqA23Addr = 0u;
    reqParity = 0u;
    reqByte = 0u;
    reqAck =0u;
    adiState.SetIgnoreAck( adiState.GetOverrunDetection() );
    calculatedDataParity = 0u;
    dataParity = 0u;
    data = 0;
    adiState.SetRegister( SWDRegisters::SWDR_UNDEFINED );
}

void SWDOperation::AddFrames( SWDAnalyzerResults* pResults )
{
    const std::size_t trnNr = adiState.GetTurnaroundNumber();
    assert( bits.size() >=
            static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) + trnNr + static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE) );

    // request
    std::deque<SWDBit>::const_iterator bi( bits.begin() );
    auto frameEndSample = bi[ static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) - 1u ].GetEndSample();
    const SWDRegisters reg = adiState.GetRegister();
    AddFrame( pResults, startSample, frameEndSample, reqByte, static_cast<U64>( reg ), SwdFrameTypes::SWD_FT_REQUEST,
                 static_cast<U8>( reqRnW ? SWDRequestFrame::RQ_FLAG::IS_READ : SWDRequestFrame::RQ_FLAG::IS_NONE ) |
                     static_cast<U8>( reqAPnDP ? SWDRequestFrame::RQ_FLAG::IS_ACCESS_PORT : SWDRequestFrame::RQ_FLAG::IS_NONE ) );
    bi += static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE );

    // turnaround
    auto frameStartSample = bi[ 0u ].GetStartSample();
    frameEndSample = bi[ trnNr - 1u ].GetEndSample();
    AddFrame( pResults, frameStartSample, frameEndSample, bi[ 0u ].IsHigh() ? 1u : 0u, 0u, SwdFrameTypes::SWD_FT_TURNAROUND, 0u );
    bi += trnNr;

    // ack
    frameStartSample = bi[ 0u ].GetStartSample();
    frameEndSample = bi[ static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE ) - 1u ].GetEndSample();
    AddFrame( pResults, frameStartSample, frameEndSample, reqAck, 0u, SwdFrameTypes::SWD_FT_ACK, 0u );
    bi += static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE );

    // turnaround if ACK is WAIT or FAULT or OK with write operation
    if( ( reqAck == SWDAcks::ACK_WAIT ) || ( reqAck == SWDAcks::ACK_FAULT ) || ( ( reqAck == SWDAcks::ACK_OK ) && ( !reqRnW ) ) )
    {
        frameStartSample = bi[ 0u ].GetStartSample();
        frameEndSample = bi[ trnNr - 1u ].GetEndSample();
        AddFrame( pResults, frameStartSample, frameEndSample, bi[ 0u ].IsHigh() ? 1u : 0u, 0u, SwdFrameTypes::SWD_FT_TURNAROUND, 0u );
        bi += trnNr;
    }

    // Prepare FrameV2 data.
    FrameV2 frameV2;
    frameV2.AddBoolean( "RnW", reqRnW );
    frameV2.AddBoolean( "APnDP", reqAPnDP );
    frameV2.AddString( "reg", GetRegisterName( reg ).c_str() );
    frameV2.AddByte( "ack", reqAck );

    if( bits.size() < static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) + trnNr +
                          static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE ) + static_cast<std::size_t>( OpItemSize::OP_DATA_SIZE ) +
                          static_cast<std::size_t>( OpItemSize::OP_DATA_PARITY_SIZE ) + trnNr )
    {
        pResults->AddFrameV2( frameV2, "transfer", startSample, endSample );
        return;
    }

    // data
    frameStartSample = bi[ 0u ].GetStartSample();
    frameEndSample = bi[ static_cast<std::size_t>( OpItemSize::OP_DATA_SIZE ) - 1u ].GetEndSample();
    SWDRegistersUnion swdRegCouple{}; // Couple of previous and current SWD register
    if( reqRnW )
    {
        if( reqAPnDP )
        {
            // AP read
            if( adiState.GetNumApReads() == 0u )
            {
                // On the first AP read access, the read data that is returned is unknown. The debugger must discard this result.
                swdRegCouple.reg.prev = SWDRegisters::SWDR_UNDEFINED;
                swdRegCouple.reg.current = reg; 
            }
            else
            {
                // The next AP read access, if successful, returns the result of the previous AP read.
                swdRegCouple.reg.prev = adiState.GetLastReadRegister();
                swdRegCouple.reg.current = reg;
                swdRegCouple.reg.memAddr = adiState.GetTar();
            }
            adiState.IncrementNumApReads();
        }
        else
        {
            // DP read
            if( ( adiState.GetNumApReads() != 0 ) && ( reg == SWDRegisters::SWDR_DP_RDBUFF ) )
            {
                // The debugger can then read the DP RDBUFF register to obtain the last AP read result.
                swdRegCouple.reg.prev = adiState.GetLastReadRegister();
                swdRegCouple.reg.current = reg;
                swdRegCouple.reg.memAddr = adiState.GetTar();
                adiState.ClearNumApReads();
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
        swdRegCouple.reg.memAddr = adiState.GetTar();
        adiState.ClearNumApReads();
    }
    AddFrame( pResults, frameStartSample, frameEndSample, data, swdRegCouple.blob,
                 reqRnW ? SwdFrameTypes::SWD_FT_RDATA : SwdFrameTypes::SWD_FT_WDATA, 0u );
    bi += static_cast<std::size_t>( OpItemSize::OP_DATA_SIZE );

    // data parity
    frameStartSample = bi[ 0u ].GetStartSample();
    frameEndSample = bi[ static_cast<std::size_t>( OpItemSize::OP_DATA_PARITY_SIZE ) - 1u ].GetEndSample();
    AddFrame( pResults, frameStartSample, frameEndSample, dataParity, dataParityOk ? 1u : 0u, SwdFrameTypes::SWD_FT_DATA_PARITY, 0u );
    bi += static_cast<std::size_t>( OpItemSize::OP_DATA_PARITY_SIZE );

    if( reqRnW )
    {
        // add turnaround on read operation
        frameStartSample = bi[ 0u ].GetStartSample();
        frameEndSample = bi[ trnNr - 1u ].GetEndSample();
        AddFrame( pResults, frameStartSample, frameEndSample, bi[ 0u ].IsHigh() ? 1u : 0u, 0u, SwdFrameTypes::SWD_FT_TURNAROUND, 0u );
    }

    // New FrameV2 code.
    std::vector<U8> bytes = ToVectorU8( data );
    frameV2.AddByteArray( "data", bytes.data(), bytes.size() );
    // Add an effective AP register that refers to data
    SWDRegisters effectiveApReg = SWDRegisters::SWDR_UNDEFINED; 
    if( reqRnW )
    {
        // Read operation
        if( reqAPnDP )
        {
            if( swdRegCouple.reg.current != SWDRegisters::SWDR_UNDEFINED )
            {
                effectiveApReg = swdRegCouple.reg.prev;
            }
        }
        else
        {
            if( reg == SWDRegisters::SWDR_DP_RDBUFF )
            {
                effectiveApReg = swdRegCouple.reg.prev;
            }
        }
    }
    else
    {
        // Write operation
        if( reqAPnDP )
        {
            effectiveApReg = swdRegCouple.reg.current;
        }
    }
    if( effectiveApReg != SWDRegisters::SWDR_UNDEFINED )
    {
        frameV2.AddString( "apreg", ::GetRegisterName( effectiveApReg ).c_str() );
        // Data Read/Write register
        if( IsApDataReg( effectiveApReg ) )
        {
            std::vector<U8> memAddrBytes = ToVectorU8( GetMemAddr( effectiveApReg, swdRegCouple.reg.memAddr ) );
            frameV2.AddByteArray( "memaddr", memAddrBytes.data(), memAddrBytes.size() );
        }
    }
    pResults->AddFrameV2( frameV2, "transfer", startSample, endSample );
}

void SWDOperation::AddMarkers( SWDAnalyzerResults* pResults ) const
{
    const size_t trnNr = adiState.GetTurnaroundNumber();
    for( std::deque<SWDBit>::const_iterator bi( bits.begin() ); bi != bits.end(); ++bi )
    {
        size_t ndx = bi - bits.begin();

        // turnaround
        if( ( ( ndx >= 8u ) && ( ndx < 8u + trnNr ) ) ||
            ( (!reqRnW) && ( ( ndx >= 8u + trnNr + 3u ) && ( ndx < 8u + trnNr + 3u + trnNr ) ) ) ||
            ( reqRnW && ( ( ndx >= 8u + trnNr + 3u + 32u + 1u ) && ( ndx < 8u + trnNr + 3u + 32u + 1u + trnNr ) ) )
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

void ADIState::DetermineRegister( bool reqAPnDP, bool reqRnW, U8 reqA23Addr )
{
//    SWDRegisters reg;
    if( reqAPnDP ) // AccessPort or DebugPort?
    {
        // MEM-AP Registers
        if (mDPVersion < DPVersion::DP_V3)
        {
            // MEM-AP ADIv5
            const U8 apRegOffset = static_cast<U8>((mSelect & 0xF0u) | reqA23Addr);
            const auto apReg = MEM_AP_ADI_V5.find(apRegOffset);
            mRegister = (apReg != MEM_AP_ADI_V5.end()) ? apReg->second : SWDRegisters::SWDR_AP_RAZ_WI;
        }
        else
        {
            // MEM-AP ADIv6
            const U16 apRegOffset = static_cast<U16>((mSelect & 0xFF0u) | reqA23Addr);
            const auto apReg = MEM_AP_ADI_V6.find(apRegOffset);
            mRegister = (apReg != MEM_AP_ADI_V6.end()) ? apReg->second : SWDRegisters::SWDR_AP_RAZ_WI;
        }
    }
    else
    {
        // DP Registers
        mRegister = SWDRegisters::SWDR_UNDEFINED;
        const U8 dpBankSel = static_cast<U8>( mSelect & 0x0Fu );

        const auto dpReg = DP_REGISTERS.find( reqA23Addr );
        if( dpReg != DP_REGISTERS.end() )
        {
            for( const auto& dpRegRecord : dpReg->second )
            {
                if( dpRegRecord.Bank.empty() || ( dpRegRecord.Bank.find( dpBankSel ) != dpRegRecord.Bank.end() ) )
                {
                    if( ( reqRnW && ( ( dpRegRecord.Access & static_cast<U8>( RegMask::REG_READ ) ) != 0u ) ) ||
                        ( !reqRnW && ( ( dpRegRecord.Access & static_cast<U8>( RegMask::REG_WRITE ) ) != 0u ) ) )
                    {
                        if( ( mDPVersion == DPVersion::DP_V0 ) ||
                            ( ( mDPVersion == DPVersion::DP_V1 ) && ( ( dpRegRecord.Access & static_cast<U8>( RegMask::REG_V1 ) ) != 0u ) ) ||
                            ( ( mDPVersion == DPVersion::DP_V2 ) && ( ( dpRegRecord.Access & static_cast<U8>( RegMask::REG_V2 ) ) != 0u ) ) ||
                            ( ( mDPVersion == DPVersion::DP_V3 ) && ( ( dpRegRecord.Access & static_cast<U8>( RegMask::REG_V3 ) ) != 0u ) ) )
                        {
                            mRegister = dpRegRecord.Register;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void ADIState::SetAddrInc( U8 num )
{
    switch( num )
    {
        case static_cast<U8>(CswAddrInc::CSW_ADDRINC_DISABLED):
            mCswAddrInc = CswAddrInc::CSW_ADDRINC_DISABLED;
            break;
        case static_cast<U8>( CswAddrInc::CSW_ADDRINC_SINGLE ):
            mCswAddrInc = CswAddrInc::CSW_ADDRINC_SINGLE;
            break;
        case static_cast<U8>( CswAddrInc::CSW_ADDRINC_PACKED ):
            mCswAddrInc = CswAddrInc::CSW_ADDRINC_PACKED;
            break;
        default:
            mCswAddrInc = CswAddrInc::CSW_ADDRINC_RESERVED;
            break;
    }
}

void ADIState::SetDataSize( U8 num )
{
    switch( num )
    {
        case static_cast<U8>( CswSize::CSW_SIZE_8_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_8_BIT;
            break;
        case static_cast<U8>( CswSize::CSW_SIZE_16_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_16_BIT;
            break;
        case static_cast<U8>( CswSize::CSW_SIZE_32_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_32_BIT;
            break;
        case static_cast<U8>( CswSize::CSW_SIZE_64_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_64_BIT;
            break;
        case static_cast<U8>( CswSize::CSW_SIZE_128_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_128_BIT;
            break;
        case static_cast<U8>( CswSize::CSW_SIZE_256_BIT ):
            mCswDataSize = CswSize::CSW_SIZE_256_BIT;
            break;
        default:
            mCswDataSize = CswSize::CSW_SIZE_RESERVED;
            break;
    }
}

void ADIState::IncrementTar()
{
    if( mCswAddrInc == CswAddrInc::CSW_ADDRINC_SINGLE )
    {
        mTar += ( 1u << static_cast<U8>( mCswDataSize ) );
    }
    else if( mCswAddrInc == CswAddrInc::CSW_ADDRINC_PACKED )
    {
        mTar += 4u;
    }
}

void ADIState::SetSelect( U32 select )
{
	mSelect = select;
}

U32 ADIState::GetSelect() const
{
    return mSelect;
}

void ADIState::SetCurrentProtocol( DebugProtocol protocol )
{
	mCurrentProtocol = protocol;
}

DebugProtocol ADIState::GetCurrentProtocol() const
{
    return mCurrentProtocol;
}

void ADIState::SetLastFrameType( SwdFrameTypes frameType )
{
	mLastFrameType = frameType;
}

SwdFrameTypes ADIState::GetLastFrameType() const
{
    return mLastFrameType;
}


// ********************************************************************************

SWDLineReset::SWDLineReset( ADIState& adiStateRef )
    : SWDVariableLengthSequence( SwdFrameTypes::SWD_FT_LINE_RESET, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD }, {},
                                 adiStateRef ),
      firstLowBitOffset( 0u )
{
}

void SWDLineReset::Clear()
{
    SWDBaseSequence::Clear();
    firstLowBitOffset = 0u;
}

void SWDLineReset::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    AddFrame( pResults, startSample, endSample, bitCount, 0u, seqFrameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, seqFrameType, bitCount );
}

// ********************************************************************************

JTAGToSWD::JTAGToSWD( ADIState& adiStateRef )
    : SwitchSequenceWithDeprecate( SwdFrameTypes::SWD_FT_JTAG_TO_SWD, adiStateRef, patternJtagToSwd, patternJtagToSwdDeprecated )
{
}

// ********************************************************************************

SWDToJTAG::SWDToJTAG( ADIState& adiStateRef )
    : SwitchSequenceWithDeprecate( SwdFrameTypes::SWD_FT_SWD_TO_JTAG, adiStateRef, patternSwdToJtag, patternSwdToJtagDeprecated )
{
}

// ********************************************************************************

SWDErrorBits::SWDErrorBits( ADIState& adiStateRef )
    : SWDVariableLengthSequence( SwdFrameTypes::SWD_FT_ERROR, {}, {}, adiStateRef ), protocol( DebugProtocol::DPROTOCOL_UNKNOWN )
{
}

void SWDErrorBits::Clear()
{
    SWDBaseSequence::Clear();
    protocol = DebugProtocol::DPROTOCOL_UNKNOWN;
}

void SWDErrorBits::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    SwdFrameTypes frameType = ( ( protocol == DebugProtocol::DPROTOCOL_UNKNOWN ) || ( protocol == DebugProtocol::DPROTOCOL_SWD ) )
                                  ? SwdFrameTypes::SWD_FT_ERROR
                                  : SwdFrameTypes::SWD_FT_IGNORED;
    AddFrame( pResults, startSample, endSample, bitCount, 0u, frameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, frameType, bitCount );
}

void SWDErrorBits::SetProtocol( const DebugProtocol newProtocol )
{
    protocol = newProtocol;
}

void SWDErrorBits::PushBackBit( SWDBit&& bit )
{
	bits.push_back( std::move( bit ) );
}

// ********************************************************************************

SWDIdleCycles::SWDIdleCycles( ADIState& adiStateRef )
    : PlainBitSequence( SwdFrameTypes::SWD_FT_IDLE_CYCLE, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD },
                        { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_OPERATION, SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE }, adiStateRef, false, 1u )
{
}

void SWDIdleCycles::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    AddFrame( pResults, startSample, endSample, bitCount, 0u, seqFrameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, seqFrameType, bitCount );
}

// ********************************************************************************

JTAGTlr::JTAGTlr( ADIState& adiStateRef )
    : PlainBitSequence( SwdFrameTypes::SWD_FT_JTAG_TLR,
                        { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_JTAG, DebugProtocol::DPROTOCOL_SWD }, {}, adiStateRef,
                        true, 5u )
{
}

void JTAGTlr::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    AddFrame( pResults, startSample, endSample, bitCount, 0u, seqFrameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, seqFrameType, bitCount );
}

// ********************************************************************************

JTAGToDS::JTAGToDS( ADIState& adiStateRef )
    : SwitchSequence( SwdFrameTypes::SWD_FT_JTAG_TO_DS,
                      { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
                      { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_JTAG_TLR }, adiStateRef, patternJtagToDs )
{
}

// ********************************************************************************

SWDToDS::SWDToDS( ADIState& adiStateRef )
    : SwitchSequence( SwdFrameTypes::SWD_FT_SWD_TO_DS,
                      { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
                      { SwdFrameTypes::SWD_FT_LINE_RESET }, adiStateRef, patternSwdToDs )
{
}

// ********************************************************************************

DSSelectionAlertPreamble::DSSelectionAlertPreamble( ADIState& adiStateRef )
    : PlainBitSequence( SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
                        {}, adiStateRef, true, 8u )
{
}

void DSSelectionAlertPreamble::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    AddFrame( pResults, startSample, endSample, bitCount, 0u, seqFrameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, seqFrameType, bitCount );
}

// ********************************************************************************

DSSelectionAlert::DSSelectionAlert( ADIState& adiStateRef )
    : SwitchSequence( SwdFrameTypes::SWD_FT_DS_SEL_ALERT, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT }, {},
                      adiStateRef, patternDsSelectionAlert )
{
}

void DSSelectionAlert::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    auto hi64BitData = GetData( 1u );
    auto lo64BitData = GetData( 0u );
    AddFrame( pResults, startSample, endSample, hi64BitData, lo64BitData, seqFrameType, 0u );

    // FrameV2
    std::vector<U8> bytes = ToVectorU8( lo64BitData );
    std::vector<U8> bytesHi = ToVectorU8( hi64BitData );
    bytes.insert( bytes.end(), bytesHi.begin(), bytesHi.end() );
    AddFrameV2DataSequence( pResults, startSample, endSample, seqFrameType, numberCheckedBits, bytes );
}

// ********************************************************************************

DSActivationCodePreamble::DSActivationCodePreamble( ADIState& adiStateRef )
    : SwitchSequence( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE,
                      { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT }, { SwdFrameTypes::SWD_FT_DS_SEL_ALERT },
                      adiStateRef, patternDsActivationCodePreamble )
{
}

void DSActivationCodePreamble::AddFrames( SWDAnalyzerResults* pResults )
{
    const std::size_t bitCount = bits.size();

    // Legacy Frame
    AddFrame( pResults, startSample, endSample, bitCount, 0u, seqFrameType, 0u );

    // FrameV2
    AddFrameV2SimpleSequence( pResults, startSample, endSample, seqFrameType, bitCount );
}

// ********************************************************************************

DSActivationCode::DSActivationCode( ADIState& adiStateRef )
    : SwitchSequenceActivationCode( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE, adiStateRef, patternDsActivationCodeJtagSerial,
                                    patternDsActivationCodeSwDp, patternDsActivationCodeJtagDp )
{
}

void DSActivationCode::AddFrames( SWDAnalyzerResults* pResults )
{
    // Legacy Frame
    U16 data = GetData();
    AddFrame( pResults, startSample, endSample, data, 0u, seqFrameType, 0u );

    // FrameV2
    std::vector<U8> bytes;
    if( numberCheckedBits == 8u )
    {
        bytes.push_back( static_cast<U8>( data ) );
    }
    else
    {
        bytes = ToVectorU8( data );
    }
    AddFrameV2DataSequence( pResults, startSample, endSample, seqFrameType, numberCheckedBits, bytes );
}

// ********************************************************************************

bool SWDRequestFrame::IsRead() const
{
    return ( mFlags & static_cast<U8>( RQ_FLAG::IS_READ ) ) != 0;
}

bool SWDRequestFrame::IsAccessPort() const
{
    return ( mFlags & static_cast<U8>( RQ_FLAG::IS_ACCESS_PORT ) ) != 0;
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

    // go to the falling edge
    mSWCLK->AdvanceToNextEdge();
    mSWDIO->AdvanceToAbsPosition( mSWCLK->GetSampleNumber() );

    rbit.falling = mSWCLK->GetSampleNumber();
    rbit.stateFalling = mSWDIO->GetBitState();

    rbit.lowEnd = mSWCLK->GetSampleOfNextEdge();

    return rbit;
}

void SWDBaseSequence::CopyBits( std::deque<SWDBit>& srcBits )
{
    // clear destination before populating
    bits.clear();
    if( numberCheckedBits > srcBits.size() / 2 )
    {
        // more than half of the buffer needs to be copied, so it will be faster to do the following
        // 1) copy the tail of the buffer into a temporary object, starting from the next after the given bit number
        std::deque<SWDBit> mBitsBufferTail( srcBits.begin() + numberCheckedBits, srcBits.end() );
        // 2) strip tail of buffer starting from the next after the given bit number
        srcBits.erase( srcBits.begin() + numberCheckedBits, srcBits.end() );
        // 3) give the bits to the destination object by swap with srcBits
        std::swap( bits, srcBits );
        // 4) keep tail of buffer because that one is probably next operation's start bit
        std::swap( srcBits, mBitsBufferTail );
    }
    else
    {
        // less than half of the buffer needs to be copied, so it will be faster to do the following
        // 1) copy the buffer head up to the given bit into a temporary object
        std::deque<SWDBit> mBitsBufferHead( srcBits.begin(), srcBits.begin() + numberCheckedBits );
        // 2) strip head up to the given bit
        srcBits.erase( srcBits.begin(), srcBits.begin() + numberCheckedBits );
        // 3) give the bits to the destination object by swap with the temporary object
        std::swap( bits, mBitsBufferHead );
    }
    UpdateBitInfo();
}

void SWDParser::CopyBits( SWDBaseSequence& sequence )
{
    sequence.CopyBits( mBitsBuffer );
}

SWDParser::SWDParser()
    : mSWDIO( 0 ),
      mSWCLK( 0 ),
      mAnalyzer(),
      mAdiState(),
      mTran( mAdiState ),
      mReset( mAdiState ),
      mJtagToSwd( mAdiState ),
      mSwdToJtag( mAdiState ),
      mErrorBits( mAdiState ),
      mIdleCycles( mAdiState ),
      mJtagTlr( mAdiState ),
      mJtagToDs( mAdiState ),
      mSwdToDs( mAdiState ),
      mDsSelectionAlertPreamble( mAdiState ),
      mDsSelectionAlert( mAdiState ),
      mDsActivationCodePreamble( mAdiState ),
      mDsActivationCode( mAdiState ),
      mSequences( {
          std::ref(mReset),                    // 50,... - SWD line reset. At least 50 SWCLKTCK cycles with SWDIOTMS HIGH.
          std::ref(mJtagToSwd),                // 16 - JTAG-to-SWD select sequence
          std::ref(mSwdToJtag),                // 16 - SWD-to-JTAG select sequence
          std::ref(mSwdToDs),                  // 16 - SWD-to-DS select sequence
          std::ref(mJtagToDs),                 // 31 - JTAG-to-DS select sequence
          std::ref(mTran),                     // 12,64 - SWD read/write operation
          std::ref(mJtagTlr),                  // 5,... - Enters to JTAG Test-Logic-Reset state
          std::ref(mDsSelectionAlertPreamble), // 8,... - Selection Alert preamble. At least 8 SWCLKTCK cycles with SWDIOTMS HIGH.
          std::ref(mDsSelectionAlert),         // 128 - Selection Alert sequence
          std::ref(mDsActivationCodePreamble), // 4 - Activation Code preamble. 4 SWCLKTCK cycles with SWDIOTMS LOW
          std::ref(mDsActivationCode),         // 8,12 - Activation Code sequence
          std::ref(mIdleCycles)                // 1,... - SWD idle cycles. SWCLKTCK cycles with SWDIOTMS LOW
      } )
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
    mAdiState.SetSelect( 0u );
    mAdiState.SetDPVersion( DPVersion::DP_V0 );
    mAdiState.SetLastFrameType( SwdFrameTypes::SWD_FT_LINE_RESET );
    mAdiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_UNKNOWN );
}

bool SWDParser::IsBufferEmpty() const
{
    return mBitsBuffer.empty();
}

void SWDParser::AddBitToBuffer()
{
    mBitsBuffer.push_back( ParseBit() );
}

SwdFrameTypes SWDParser::GetLastFrameType() const
{
    return mAdiState.GetLastFrameType();
}

DebugProtocol SWDParser::GetCurrentProtocol() const
{
    return mAdiState.GetCurrentProtocol();
}

void SWDParser::SetCurrentProtocol( DebugProtocol protocol )
{
    mAdiState.SetCurrentProtocol( protocol );
}

void SWDParser::SetLastFrameType( SwdFrameTypes frame )
{
    mAdiState.SetLastFrameType( frame );
}

void SWDParser::SetDPVersion( DPVersion version )
{
    mAdiState.SetDPVersion( version );
}

void SWDParser::SetNumTurnarounds( U8 num )
{
	mAdiState.SetTurnaroundNumber( num );
}

DPVersion SWDParser::GetDPVersion() const
{
    return mAdiState.GetDPVersion();
}

void SWDParser::SetOverrunDetection( bool enabled )
{
    mAdiState.SetOverrunDetection( enabled );
}

void SWDParser::SetSelectRegister( U32 value )
{
    mAdiState.SetSelect( value );
}

OpBitRange SWDOperation::GetOpBitRange( const std::vector<std::pair<OpBitRange, OpItemSize>>* opItemSize, const std::size_t bitIdx ) const
{
    OpBitRange bitRange = OpBitRange::OP_UNDEFINED_RANGE;
    std::size_t startIdx = 0;
    for( const auto& range : *opItemSize )
    {
        const std::size_t rangeSize = static_cast<std::size_t>( range.second );
        if( ( bitIdx >= startIdx ) && ( bitIdx < ( startIdx + rangeSize ) ) )
        {
            bitRange = range.first;
            break;
        }
        startIdx += rangeSize;
    }
    return bitRange;
}

std::size_t SWDOperation::GetOpBitRangeFirstBitIdx( const std::size_t bitIdx ) const
{
    const std::vector<const std::vector<std::pair<OpBitRange, OpItemSize>>*> opItemSizes = {
        opReqAndAckItemSize,
        ( reqRnW ? opRdataItemSize : opWdataItemSize )
    };

    std::size_t startIdx = 0;
    for( const auto& itemSizes : opItemSizes )
    {
        for( const auto& range : *itemSizes )
        {
            const std::size_t rangeSize = static_cast<std::size_t>( range.second );
            if( ( bitIdx >= startIdx ) && ( bitIdx < ( startIdx + rangeSize ) ) )
            {
                return startIdx;
            }
            startIdx += rangeSize;
        }
    }
    return startIdx;
}


void SWDOperation::GetAndParseOperationRequest( const std::deque<SWDBit>& bits )
{
    for( ; ( numberCheckedBits < static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) ) && ( numberCheckedBits < bits.size() ) && ( seqCheckResult != SeqCmpResult::SEQ_MISMATCH ); ++numberCheckedBits )
    {
        const bool isBitHigh = bits[ numberCheckedBits ].IsHigh();
        const U8 bit = isBitHigh ? 1u : 0u;
        reqByte |= ( bit << numberCheckedBits );
        switch (numberCheckedBits)
        {
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_START ):
                // Start: A single start bit, with value 0b1
                seqCheckResult = isBitHigh ? SeqCmpResult::SEQ_MATCH_PARTIALLY : SeqCmpResult::SEQ_MISMATCH;
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_AP_N_DP ):
                // APnDP: A single bit, indicating whether the Debug Port or the Access Port Access register is to be accessed. 
                //        This bit is 0b0 for a DPACC access, or 0b1 for an APACC access.
                reqAPnDP = isBitHigh;
                reqParity = bit;
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_R_N_W ):
                // RnW: A single bit, indicating whether the access is a read or a write. This bit is 0b0 for a write access, or 
                //      0b1 for a read access.
                reqRnW = isBitHigh;
                reqParity += bit;
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_A2 ):
                // A[2]: This bit, giving the A[3:2] address field for the DP or AP register Address.
                reqA23Addr = (bit << 2u);
                reqParity += bit;
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_A3 ):
                // A[3]: This bit, giving the A[3:2] address field for the DP or AP register Address.
                reqA23Addr |= (bit << 3u);
                reqParity += bit;
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_PARITY ):
                // Parity: A single parity bit for the preceding packet. The parity check is made over the four bits APnDP, RnW and A[2:3]:
                //          If the number of bits with a value of 0b1 is odd, the parity bit is set to 0b1.
                //          If the number of bits with a value of 0b1 is even, the parity bit is set to 0b0.
                if( bit != ( reqParity & 1u ) )
                {
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_STOP ):
                // Stop: A single stop bit. In the synchronous SWD protocol, this bit is always 0b0.
                if( bit != 0u )
                {
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
                break;
            case static_cast<std::size_t>( OP_REQUEST_BIT::OP_REQ_PARK ):
                // Park: A single bit. The host must drive the Park bit HIGH to park the line before tristating it for the 
                //       turnaround period, to ensure that the line is read as HIGH by the target,
                //       which is required because the pull - up on the SWD interface is weak.The target reads this bit as 0b1.
                if( bit != 1u )
                {
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
                else
                {
                    // Check for Write to DP register 0xC, TARGETSEL, where the data indicates the selected target. The target response must be ignored. 
                    if( !reqAPnDP && !reqRnW && ( ( reqA23Addr & 0x0Cu ) == 0x0Cu ) )
                    {
                        adiState.SetIgnoreAck( true );
                    }
                    // Update data offset
                    if (reqRnW)
                    {
                        // RDATA offset
                        opDataOffset = opReqAndAckSize;
                    }
                    else
                    {
                        // WDATA offset
                        opDataOffset = opReqAndAckSize + adiState.GetTurnaroundNumber();
                    }
                    // Set the actual register in this operation based on the data from the request and the previous select register state.
                    adiState.DetermineRegister( reqAPnDP, reqRnW, reqA23Addr );
                }
                break;
            default:
                break;
        }
    }
}

void SWDOperation::GetAndParseOperationTurnaround( const std::deque<SWDBit>& bits )
{
    const std::size_t turnaroundNumber = adiState.GetTurnaroundNumber();
    std::size_t turnaroundBitIdx = numberCheckedBits - GetOpBitRangeFirstBitIdx( numberCheckedBits );
    for( ; ( turnaroundBitIdx < turnaroundNumber ) && ( numberCheckedBits < bits.size() ); ++numberCheckedBits, ++turnaroundBitIdx )
    {
        // Ignore turaround bits
    }
    if( numberCheckedBits > opReqAndAckSize )
    {
        // Check ACK against WAIT and FAULT responces on second turnaround
        if( turnaroundBitIdx == turnaroundNumber )
        {
            if( adiState.GetIgnoreAck() || ( reqAck == SWDAcks::ACK_OK ) )
            {
                if( reqRnW )
                {
                    // This is the last bit on successful read operation
                    seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
                }
            }
            else
            {
                // Stop further comparing on WAIT or FAULT rssponces
                seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
            }
        }
    }
}

void SWDOperation::GetAndParseOperationAck( const std::deque<SWDBit>& bits )
{
    auto ackBitIdx = numberCheckedBits - GetOpBitRangeFirstBitIdx( numberCheckedBits );
    for( ; ( ackBitIdx < static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE ) ) && ( numberCheckedBits < bits.size() );
         ++numberCheckedBits, ++ackBitIdx )
    {
        const bool isBitHigh = bits[ numberCheckedBits ].IsHigh();
        const U8 bit = isBitHigh ? 1u : 0u;
        reqAck |= ( bit << ackBitIdx );
    }
    // Chechk ACK against invalid responces
    if( ackBitIdx == static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE ) )
    {
        bool ignoreAck = adiState.GetIgnoreAck();
        if( !ignoreAck )
        {
            if( ( reqAck == 0u ) || ( ( reqAck & ( reqAck - 1u ) ) != 0u ) )
            {
                // Stop further comparing due to invalid ACK
                seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
            }
        }
        if( ignoreAck || ( reqAck == SWDAcks::ACK_OK ) )
        {
            // Update Data offset
            opDataOffset = opReqAndAckSize + ( reqRnW ? 0u : adiState.GetTurnaroundNumber() );
        }
    }
}

void SWDOperation::GetAndParseOperationData( const std::deque<SWDBit>& bits )
{
    auto dataBitIdx = numberCheckedBits - GetOpBitRangeFirstBitIdx( numberCheckedBits );
    for( ; ( dataBitIdx < static_cast<std::size_t>( OpItemSize::OP_DATA_SIZE ) ) && ( numberCheckedBits < bits.size() ); ++numberCheckedBits, ++dataBitIdx )
    {
        const bool isBitHigh = bits[ numberCheckedBits ].IsHigh();
        const U32 bit = isBitHigh ? 1u : 0u;
        data |= ( bit << dataBitIdx );
        calculatedDataParity += bit;
    }
}

void SWDOperation::GetAndParseOperationDataParity( const std::deque<SWDBit>& bits )
{
    const bool isBitHigh = bits[ numberCheckedBits ].IsHigh();
    const U8 bit = isBitHigh ? 1u : 0u;
    dataParity = isBitHigh ? 1u : 0u;
    dataParityOk = ( dataParity == ( calculatedDataParity & 1u ) );
    if( !reqRnW )
    {
        // This is the last bit on successful write operation
        seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
    }
    numberCheckedBits++;
}

SeqCmpResult SWDOperation::Match( const std::deque<SWDBit>& bits )
{
    if( BreakMatch( bits ) )
    {
        return seqCheckResult;
    }
    // Loop through the unchecked bits.
    for( ; ( numberCheckedBits < bits.size() ) && ( seqCheckResult != SeqCmpResult::SEQ_MISMATCH ); )
    {
        // Determine which range a bit belongs to
        OpBitRange bitRange{ OpBitRange::OP_UNDEFINED_RANGE };
        if( numberCheckedBits < opReqAndAckSize )
        {
            bitRange = GetOpBitRange( opReqAndAckItemSize, numberCheckedBits );
        }
        else
        {
            if( adiState.GetIgnoreAck() || ( reqAck == SWDAcks::ACK_OK ) )
            {
                // Data transfers (WDATA and RDATA)
                if( reqRnW )
                {
                    bitRange = GetOpBitRange( opRdataItemSize, numberCheckedBits - opReqAndAckSize );
                }
                else
                {
                    bitRange = GetOpBitRange( opWdataItemSize, numberCheckedBits - opReqAndAckSize );
                }
            }
            else
            {
                // Turnarond bits after WAIT or FAULT rssponces
                bitRange = OpBitRange::OP_TURNAROUND_RANGE;
            }
        }
        switch( bitRange )
        {
        case OpBitRange::OP_REQUEST_RANGE:
            GetAndParseOperationRequest( bits );
            break;
        case OpBitRange::OP_TURNAROUND_RANGE:
            GetAndParseOperationTurnaround( bits );
            break;
        case OpBitRange::OP_ACK_RANGE:
            GetAndParseOperationAck( bits );
            break;
        case OpBitRange::OP_DATA_RANGE:
            GetAndParseOperationData( bits );
            break;
        case OpBitRange::OP_DATA_PARITY_RANGE:
            GetAndParseOperationDataParity( bits );
            break;
        default:
            break;
        }
    }

    return seqCheckResult;
}

SeqCmpResult SWDLineReset::Match(const std::deque<SWDBit>& bits)
{
    constexpr size_t minimumBitCnt = 50u; // Required at least 50 bits
    if( BreakMatch( bits ) )
    {
        return seqCheckResult;
    }
    // Loop through the unchecked bits.
    for( ; ( numberCheckedBits < bits.size() ) && ( seqCheckResult != SeqCmpResult::SEQ_MISMATCH ); ++numberCheckedBits )
    {
        if( firstLowBitOffset == 0 )
        {
            if( bits[ numberCheckedBits ].IsHigh() )
            {
                // The bit is high
                if( numberCheckedBits == 0u )
                {
                    // The first bit
                    seqCheckResult = SeqCmpResult::SEQ_MATCH_PARTIALLY;
                }
            }
            else
            {
                if( numberCheckedBits >= minimumBitCnt )
                {
                    // The first low bit is at least 50 bits from the start
                    firstLowBitOffset = numberCheckedBits;
                }
                else
                {
                    // The first low bit is less than 50 bits from the start
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
            }
        }
        else
        {
            // The bits after the first low bit will be checked against joined SWD transaction request
            SeqCmpResult cmpJoinedOpResults = SeqCmpResult::SEQ_UNKNOWN;
            for( const auto& pattern : patternSwdReq )
            {
                SeqCmpResult cmpJoinedOpResult = pattern.get().Match( bits, firstLowBitOffset );
                if( cmpJoinedOpResult == SeqCmpResult::SEQ_MATCH_COMPLETELY )
                {
                    if( ( ( numberCheckedBits - firstLowBitOffset + 1u ) == pattern.get().GetBitCount() ) &&
                        ( numberCheckedBits >= minimumBitCnt + static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) ) )
                    {
                        seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
                        numberCheckedBits -= ( static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) -
                                 1u ); // Tha last 8 bits are part of the joined SWD transaction request
                        return seqCheckResult;
                    }
                    else
                    {
                        cmpJoinedOpResult = SeqCmpResult::SEQ_MISMATCH;
                    }
                }
                cmpJoinedOpResults = BestMach( cmpJoinedOpResults, cmpJoinedOpResult );
            }
            if( cmpJoinedOpResults == SeqCmpResult::SEQ_MISMATCH )
            {
                // The bits after the first low bit do not match any joined SWD transaction request fall back to the clean SWD reset
                // sequence
                seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
                numberCheckedBits = firstLowBitOffset;
                return seqCheckResult;
            }
        }
    }
    return seqCheckResult;
}

SeqCmpResult SWDParser::Match( SWDBaseSequence& sequence )
{
    return sequence.Match( mBitsBuffer );
}

std::vector<std::reference_wrapper<SWDBaseSequence>>& SWDParser::GetSequences()
{
    return mSequences;
}

SWDErrorBits& SWDParser::GetErrorBits()
{
    return mErrorBits;
}

void SWDLineReset::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_UNKNOWN );

    // Forcing DP registers that are dependent on the SWD line reset.
    // DLCR.TURNROUND <- 0; Set number of turnaround cycles to 1
    adiState.SetTurnaroundNumber( 1u );
    // SELECT.DPBANKSEL <- 0;
    adiState.SetSelect( adiState.GetSelect() & 0xFFFFFFF0u );
}
void JTAGToSWD::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_SWD );
}
void SWDToJTAG::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_JTAG );
}
void SWDOperation::UpdateAdiState() 
{
    SWDBaseSequence::UpdateAdiState();

    // Make further decision based on ACK
    if( reqAck == SWDAcks::ACK_OK )
    {
        // If ACK is OK, then the transaction is successful
        const SWDRegisters reg = adiState.GetRegister();
        switch( reg )
        {
            case SWDRegisters::SWDR_DP_DPIDR:
                // if this is read DPIDR, capture the Version of the DP architecture implemented.
                switch( ( data >> 12u ) & 0x0fu )
                {
                case 0x01u:
                    adiState.SetDPVersion( DPVersion::DP_V1 );
                    break;
                case 0x02u:
                    adiState.SetDPVersion( DPVersion::DP_V2 );
                    break;
                case 0x03u:
                    adiState.SetDPVersion( DPVersion::DP_V3 );
                    break;
                default:
                    adiState.SetDPVersion( DPVersion::DP_V0 );
                    break;
                }
                break;
            case SWDRegisters::SWDR_DP_CTRL_STAT:
                // Capture the ORUNDETECT bit of the DP CTRL/STAT register when reading from or writing to it.
                adiState.SetOverrunDetection( ( data & 0x00000001u ) != 0u );
                break;
            case SWDRegisters::SWDR_DP_SELECT:
                // if this is a SELECT register write, remember the value
                adiState.SetSelect( data );
                break;
            case SWDRegisters::SWDR_DP_DLCR:
                // if this is a DLCR register read or write, update number of turnaround cycles
                adiState.SetTurnaroundNumber( ( ( data >> 8u ) & 0x03u ) + 1u );
                break;
            default:
                if( reqRnW )
                {
                    // Read op
                    if( reqAPnDP || ( reg == SWDRegisters::SWDR_DP_RDBUFF ) )
                    {
                        CapureApRegData( adiState.GetLastReadRegister() );
                    }
                }
                else
                {
                    // Write op
                    CapureApRegData( reg );
                }
                break;
        }
        // Update previous AP read operation register
        if( reqRnW )
        {
            if( reqAPnDP )
            {
                adiState.SetLastReadRegister( reg );
            }
            else
            {
                if( reg == SWDRegisters::SWDR_DP_RDBUFF )
                {
                    adiState.SetLastReadRegister( SWDRegisters::SWDR_UNDEFINED );
                }
            }
        }
        else
        {
            adiState.SetLastReadRegister( SWDRegisters::SWDR_UNDEFINED );
        }
    }
    if( ( reqAck == 0u ) || ( ( reqAck & ( reqAck - 1u ) ) != 0u ) )
    {
        // Set current protocol to unknown if ACK is invalid
        adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_UNKNOWN );
    }
    else
    {
        // Set current protocol to SWD if ACK is valid
        adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_SWD );
    }
}

void SWDOperation::CapureApRegData( SWDRegisters reg )
{
    switch( reg )
    {
    case SWDRegisters::SWDR_AP_CSW:
        // Capture address auto-increment and packing mode, MEM-AP access size
        adiState.SetAddrInc( ( data >> 4u ) & 0x03u );
        adiState.SetDataSize( data & 0x07u );
        break;
    case SWDRegisters::SWDR_AP_TAR:
        // Capture TAR, Transfer Address Register
        adiState.SetTar( data );
        break;
    case SWDRegisters::SWDR_AP_DRW:
        // Increment TAR per CSW.AddrInc
        adiState.IncrementTar();
        break;
    default:
        break;
    }
}

void SWDOperation::CaptureNewSequence( const std::deque<SWDBit>& bits )
{
    SWDBaseSequence::CaptureNewSequence( bits );
    const std::size_t turnaroundNumber = adiState.GetTurnaroundNumber();
    const std::size_t turnaroundNumberIdx = turnaroundNumber - 1u;
    opReqAndAckItemSize = &( OP_REQ_AND_ACK_SIZE.at( turnaroundNumberIdx ) );
    opRdataItemSize = &( OP_RDATA_SIZE.at( turnaroundNumberIdx ) );
    opWdataItemSize = &( OP_WDATA_SIZE.at( turnaroundNumberIdx ) );
    opAckOffset = static_cast<std::size_t>( OpItemSize::OP_REQUEST_SIZE ) + turnaroundNumber;
    opReqAndAckSize = opAckOffset + static_cast<std::size_t>( OpItemSize::OP_ACK_SIZE );
}


void JTAGToDS::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_DORMANT );
}
void SWDToDS::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_DORMANT );
}
void DSActivationCode::UpdateAdiState()
{
    SWDBaseSequence::UpdateAdiState();
    if( ( resultIdx == ActivationCodeID::ACTIVATION_CODE_1 ) || ( resultIdx == ActivationCodeID::ACTIVATION_CODE_3 ) )
	{
		adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_JTAG );
	}
    else if( resultIdx == ActivationCodeID::ACTIVATION_CODE_2 )
	{
		adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_SWD );
	}
	else
	{
		adiState.SetCurrentProtocol( DebugProtocol::DPROTOCOL_UNKNOWN );
	}
}

SWDBit SWDParser::PopFrontBit()
{
    assert( !mBitsBuffer.empty() );

    SWDBit retVal( mBitsBuffer.front() );

    // shift the elements by 1
    mBitsBuffer.pop_front();

    return retVal;
}

void ADIState::SetIgnoreAck( bool ignoreAck )
{
    mIgnoreAck = ignoreAck;
}

bool ADIState::GetIgnoreAck() const
{
    return mIgnoreAck;
}

void ADIState::SetDPVersion( DPVersion version )
{
	mDPVersion = version;
}

DPVersion ADIState::GetDPVersion() const
{
    return mDPVersion;
}

void ADIState::SetRegister( SWDRegisters reg )
{
    mRegister = reg;
}

SWDRegisters ADIState::GetRegister() const
{
    return mRegister;
}

void ADIState::SetTurnaroundNumber( U8 num )
{
    mTurnaroundNumber = num;
}

U8 ADIState::GetTurnaroundNumber() const
{
    return mTurnaroundNumber;
}

void ADIState::SetOverrunDetection( bool enable )
{
    mOrundetection = enable;
}

bool ADIState::GetOverrunDetection() const
{
    return mOrundetection;
}

void ADIState::IncrementNumApReads()
{
    ++mNumApReads;
}

void ADIState::ClearNumApReads()
{
	mNumApReads = 0u;
}

size_t ADIState::GetNumApReads() const
{
    return mNumApReads;
}

void ADIState::SetLastReadRegister( SWDRegisters reg )
{
    mLastReadRegister = reg;
}

SWDRegisters ADIState::GetLastReadRegister() const
{
    return mLastReadRegister;
}

void ADIState::SetTar( U32 tar )
{
	mTar = tar;
}

U32 ADIState::GetTar() const
{
    return mTar;
}


