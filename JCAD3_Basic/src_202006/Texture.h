/////////////////////////////////////////////////////////////////////////////
// POV-Ray定義済みテクスチャ定義
// Texture.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/15 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___TEXTURE_H
#define ___TEXTURE_H

#define MATERIAL_CNT 12													// テクスチャ 材質数
#define GOLD_CNT     25													// 金 種類数
#define SILVER_CNT   25													// 銀 種類数
#define COPPER_CNT   25													// 銅 種類数
#define BRASS_CNT    25													// 真鍮 種類数
#define CHROME_CNT   25													// クロム 種類数
#define METAL_CNT    24													// 金属 種類数
#define GLASS1_CNT   9													// ガラス1 種類数
#define WOOD_CNT     54													// 木 種類数
#define STONE_CNT    112												// 石 種類数
#define SKY_CNT      12													// 空 種類数
#define G_OUT_CNT    10													// ガラス表面 性質数
#define G_IN1_CNT    8													// ガラス内部 性質数
#define G_COL1_CNT   94													// ガラス 色数
#define G_IN2_CNT    31													// ガラス内部 性質(屈折率)数
#define G_COL2_CNT   9													// ガラス 色(透明)数

const CString T_Material[] = {											// テクスチャ 材質
	"金(Gold)|A", "銀(Silver)|B", "銅(Copper)|C", "真鍮(Brass)|D", "クロム(Chrome)|E",
	"金属(Metal)|F", "ガラス１(Glass1)|G", "ガラス２(Glass2)|H", "ガラス３(Glass3)|I",
	"木(Wood)|J", "石(Stone)|K", "空(Sky)|L"
};

// include "metals.inc" (->golds.inc)
const CString T_Gold[] = {												// 金 種類
	"T_Gold_1A|A", "T_Gold_1B|A", "T_Gold_1C|A", "T_Gold_1D|A", "T_Gold_1E|A",
	"T_Gold_2A|A", "T_Gold_2B|A", "T_Gold_2C|A", "T_Gold_2D|A", "T_Gold_2E|A",
	"T_Gold_3A|A", "T_Gold_3B|A", "T_Gold_3C|A", "T_Gold_3D|A", "T_Gold_3E|A",
	"T_Gold_4A|A", "T_Gold_4B|A", "T_Gold_4C|A", "T_Gold_4D|A", "T_Gold_4E|A",
	"T_Gold_5A|A", "T_Gold_5B|A", "T_Gold_5C|A", "T_Gold_5D|A", "T_Gold_5E|A"
};

// include "metals.inc"
const CString T_Silver[] = {											// 銀 種類
	"T_Silver_1A|A", "T_Silver_1B|A", "T_Silver_1C|A", "T_Silver_1D|A", "T_Silver_1E|A",
	"T_Silver_2A|A", "T_Silver_2B|A", "T_Silver_2C|A", "T_Silver_2D|A", "T_Silver_2E|A",
	"T_Silver_3A|A", "T_Silver_3B|A", "T_Silver_3C|A", "T_Silver_3D|A", "T_Silver_3E|A",
	"T_Silver_4A|A", "T_Silver_4B|A", "T_Silver_4C|A", "T_Silver_4D|A", "T_Silver_4E|A",
	"T_Silver_5A|A", "T_Silver_5B|A", "T_Silver_5C|A", "T_Silver_5D|A", "T_Silver_5E|A"
};

// include "metals.inc"
const CString T_Copper[] = {											// 銅 種類
	"T_Copper_1A|A", "T_Copper_1B|A", "T_Copper_1C|A", "T_Copper_1D|A", "T_Copper_1E|A",
	"T_Copper_2A|A", "T_Copper_2B|A", "T_Copper_2C|A", "T_Copper_2D|A", "T_Copper_2E|A",
	"T_Copper_3A|A", "T_Copper_3B|A", "T_Copper_3C|A", "T_Copper_3D|A", "T_Copper_3E|A",
	"T_Copper_4A|A", "T_Copper_4B|A", "T_Copper_4C|A", "T_Copper_4D|A", "T_Copper_4E|A",
	"T_Copper_5A|A", "T_Copper_5B|A", "T_Copper_5C|A", "T_Copper_5D|A", "T_Copper_5E|A"
};

// include "metals.inc"
const CString T_Brass[] = {												// 真鍮 種類
	"T_Brass_1A|A", "T_Brass_1B|A", "T_Brass_1C|A", "T_Brass_1D|A", "T_Brass_1E|A",
	"T_Brass_2A|A", "T_Brass_2B|A", "T_Brass_2C|A", "T_Brass_2D|A", "T_Brass_2E|A",
	"T_Brass_3A|A", "T_Brass_3B|A", "T_Brass_3C|A", "T_Brass_3D|A", "T_Brass_3E|A",
	"T_Brass_4A|A", "T_Brass_4B|A", "T_Brass_4C|A", "T_Brass_4D|A", "T_Brass_4E|A",
	"T_Brass_5A|A", "T_Brass_5B|A", "T_Brass_5C|A", "T_Brass_5D|A", "T_Brass_5E|A"
};

// include "metals.inc"
const CString T_Chrome[] = {											// クロム 種類
	"T_Chrome_1A|A", "T_Chrome_1B|A", "T_Chrome_1C|A", "T_Chrome_1D|A", "T_Chrome_1E|A",
	"T_Chrome_2A|A", "T_Chrome_2B|A", "T_Chrome_2C|A", "T_Chrome_2D|A", "T_Chrome_2E|A",
	"T_Chrome_3A|A", "T_Chrome_3B|A", "T_Chrome_3C|A", "T_Chrome_3D|A", "T_Chrome_3E|A",
	"T_Chrome_4A|A", "T_Chrome_4B|A", "T_Chrome_4C|A", "T_Chrome_4D|A", "T_Chrome_4E|A",
	"T_Chrome_5A|A", "T_Chrome_5B|A", "T_Chrome_5C|A", "T_Chrome_5D|A", "T_Chrome_5E|A"
};

// include "textures.inc"
const CString T_Metal[] = {												// 金属 種類
	"Gold_Texture|A", "Gold_Nugget|A", "Silver_Texture|A", "Silver1|A", "Silver2|A",
	"Silver3|A", "Soft_Silver|A", "Copper_Texture|A", "Bronze_Texture|A",
	"Bright_Bronze|A", "New_Penny|A", "Brass_Texture|A", "Polished_Brass|A",
	"New_Brass|A", "Tinny_Brass|A", "Spun_Brass|A", "Brass_Valley|A",
	"Chrome_Texture|A", "Polished_Chrome|A", "Aluminum|A", "Brushed_Aluminum|A",
	"Rusty_Iron|A", "Rust|A", "Metal - White|C"
};

// include "textures.inc"
const CString T_Glass1[] = {											// ガラス1 種類
	"M_Glass|D", "M_Glass2|D", "M_Glass3|D", "M_Green_Glass|D", "M_Ruby_Glass|D",
	"M_Dark_Green_Glass|D", "M_Yellow_Glass|D", "M_Orange_Glass|D", "M_Vicks_Bottle_Glass|D"
};

// include "textures.inc"/"woods.inc"
const CString T_Wood[] = {												// 木 種類
	"Cherry_Wood|B", "Pine_Wood|B", "Dark_Wood|B", "Tan_Wood|B", "White_Wood|B", "Tom_Wood|B",
	"DMFWood1|B", "DMFWood2|B", "DMFWood3|B", "DMFWood4|B", "DMFWood5|B", "DMFWood6|B",
	"DMFLightOak|B", "DMFDarkOak|B", "EMBWood1|A", "Yellow_Pine|A", "Rosewood|A",
	"Sandalwood|A", "Cork|A", "T_Wood1|A", "T_Wood2|A", "T_Wood3|A", "T_Wood4|A", "T_Wood5|A",
	"T_Wood6|A", "T_Wood7|A", "T_Wood8|A", "T_Wood9|A", "T_Wood10|A", "T_Wood11|A",
	"T_Wood12|A", "T_Wood13|A", "T_Wood14|A", "T_Wood15|A", "T_Wood16|A", "T_Wood17|A",
	"T_Wood18|A", "T_Wood19|A", "T_Wood20|A", "T_Wood21|A", "T_Wood22|A", "T_Wood23|A",
	"T_Wood24|A", "T_Wood25|A", "T_Wood26|A", "T_Wood27|A", "T_Wood28|A", "T_Wood29|A",
	"T_Wood30|A", "T_Wood31|A", "T_Wood32|A", "T_Wood33|A", "T_Wood34|A", "T_Wood35|A"
};

// include "textures.inc"/"stones.inc" (->stones1.inc/stones2.inc)
const CString T_Stone[] = {												// 石 種類
	"Jade|B", "Red_Marble|B", "White_Marble|B", "Blood_Marble|B", "Blue_Agate|B",
	"Sapphire_Agate|B", "Brown_Agate|B", "Pink_Granite|B", "PinkAlabaster|A",
	"T_Grnt0|A", "T_Grnt1|A", "T_Grnt2|A", "T_Grnt3|A", "T_Grnt4|A", "T_Grnt5|A",
	"T_Grnt6|A", "T_Grnt7|A", "T_Grnt8|A", "T_Grnt9|A", "T_Grnt10|A", "T_Grnt11|A",
	"T_Grnt12|A", "T_Grnt13|A", "T_Grnt14|A", "T_Grnt15|A", "T_Grnt16|A", "T_Grnt17|A",
	"T_Grnt18|A", "T_Grnt19|A", "T_Grnt20|A", "T_Grnt21|A", "T_Grnt22|A", "T_Grnt23|A",
	"T_Grnt24|A", "T_Grnt25|A", "T_Grnt26|A", "T_Grnt27|A", "T_Grnt28|A", "T_Grnt29|A",
	"T_Grnt0a|A", "T_Grnt1a|A", "T_Grnt2a|A", "T_Grnt3a|A", "T_Grnt4a|A", "T_Grnt5a|A",
	"T_Grnt6a|A", "T_Grnt7a|A", "T_Grnt8a|A", "T_Grnt9a|A", "T_Grnt10a|A", "T_Grnt11a|A",
	"T_Grnt12a|A", "T_Grnt13a|A", "T_Grnt14a|A", "T_Grnt15a|A", "T_Grnt16a|A", "T_Grnt17a|A",
	"T_Grnt18a|A", "T_Grnt19a|A", "T_Grnt20a|A", "T_Grnt21a|A", "T_Grnt22a|A", "T_Grnt23a|A",
	"T_Grnt24a|A", "T_Crack1|A", "T_Crack2|A", "T_Crack3|A", "T_Crack4|A", "T_Stone1|A",
	"T_Stone2|A", "T_Stone3|A", "T_Stone4|A", "T_Stone5|A", "T_Stone6|A", "T_Stone7|A",
	"T_Stone8|A", "T_Stone9|A", "T_Stone10|A", "T_Stone11|A", "T_Stone12|A", "T_Stone13|A",
	"T_Stone14|A", "T_Stone15|A", "T_Stone16|A", "T_Stone17|A", "T_Stone18|A", "T_Stone19|A",
	"T_Stone20|A", "T_Stone21|A", "T_Stone22|A", "T_Stone23|A", "T_Stone24|A", "T_Stone25|A",
	"T_Stone26|A", "T_Stone27|A", "T_Stone28|A", "T_Stone29|A", "T_Stone30|A", "T_Stone31|A",
	"T_Stone32|A", "T_Stone33|A", "T_Stone34|A", "T_Stone35|A", "T_Stone36|A", "T_Stone37|A",
	"T_Stone38|A", "T_Stone39|A", "T_Stone40|A", "T_Stone41|A", "T_Stone42|A", "T_Stone43|A",
	"T_Stone44|A"
};

// include "textures.inc"
const CString T_Sky[] = {												// 空 種類
	"Blue_Sky|B", "Bright_Blue_Sky|B", "Blue_Sky2|B", "Blue_Sky3|B", "Blood_Sky|B",
	"Apocalypse|B", "Clouds|B", "FBM_Clouds|A", "Shadow_Clouds|A", "Lightening1|A",
	"Lightening2|A", "Starfield|A"
};

// include "glass.inc" (->glass_old.inc)
const CString G_Out_Prpt[] = {											// ガラス表面 性質
	"F_Glass1", "F_Glass2", "F_Glass3", "F_Glass4", "F_Glass5",
	"F_Glass6", "F_Glass7", "F_Glass8", "F_Glass9", "F_Glass10"
};

// include "glass.inc"
const CString G_In_Prpt1[] = {											// ガラス内部 性質
	"I_Glass1", "I_Glass2", "I_Glass3", "I_Glass4", "I_Glass_Dispersion1",
	"I_Glass_Dispersion2", "I_Glass_Caustics1", "I_Glass_Caustics2"
};

// include "glass.inc"
const CString G_Color1[] = {											// ガラス 色
	"Col_Winebottle", "Col_Beerbottle", "Col_Ruby", "Col_Green", "Col_Dark_Green",
	"Col_Yellow", "Col_Orange", "Col_Vicksbottle", "Col_Red_01", "Col_Red_02",
	"Col_Red_03", "Col_Red_04", "Col_Green_01", "Col_Green_02", "Col_Green_03",
	"Col_Green_04", "Col_Blue_01", "Col_Blue_02", "Col_Blue_03", "Col_Blue_04",
	"Col_Yellow_01", "Col_Yellow_02", "Col_Yellow_03", "Col_Yellow_04",
	"Col_Amethyst_01", "Col_Amethyst_02", "Col_Amethyst_03", "Col_Amethyst_04",
	"Col_Amethyst_05", "Col_Amethyst_06", "Col_Apatite_01", "Col_Apatite_02",
	"Col_Apatite_03", "Col_Apatite_04", "Col_Apatite_05", "Col_Aquamarine_01",
	"Col_Aquamarine_02", "Col_Aquamarine_03", "Col_Aquamarine_04", "Col_Aquamarine_05",
	"Col_Aquamarine_06", "Col_Azurite_01", "Col_Azurite_02", "Col_Azurite_03",
	"Col_Azurite_04", "Col_Citrine_01", "Col_Emerald_01", "Col_Emerald_02",
	"Col_Emerald_03", "Col_Emerald_04", "Col_Emerald_05", "Col_Emerald_06",
	"Col_Emerald_07", "Col_Fluorite_01", "Col_Fluorite_02", "Col_Fluorite_03",
	"Col_Fluorite_04", "Col_Fluorite_05", "Col_Fluorite_06", "Col_Fluorite_07",
	"Col_Fluorite_08", "Col_Fluorite_09", "Col_Gypsum_01", "Col_Gypsum_02",
	"Col_Gypsum_03", "Col_Gypsum_04", "Col_Gypsum_05", "Col_Gypsum_06",
	"Col_Ruby_01", "Col_Ruby_02", "Col_Ruby_03", "Col_Ruby_04", "Col_Ruby_05",
	"Col_Sapphire_01", "Col_Sapphire_02", "Col_Sapphire_03", "Col_Topaz_01",
	"Col_Topaz_02", "Col_Topaz_03", "Col_Tourmaline_01", "Col_Tourmaline_02",
	"Col_Tourmaline_03", "Col_Tourmaline_04", "Col_Tourmaline_05", "Col_Tourmaline_06",
	"Col_Amber_01", "Col_Amber_02", "Col_Amber_03", "Col_Amber_04", "Col_Amber_05",
	"Col_Amber_06", "Col_Amber_07", "Col_Amber_08", "Col_Amber_09"
};

// include "glass.inc"
const CString G_In_Prpt2[] = {											// ガラス内部 性質(屈折率)
	"ior 1.00", "ior 1.05", "ior 1.10", "ior 1.15", "ior 1.20", "ior 1.25",
	"ior 1.30", "ior 1.35", "ior 1.40", "ior 1.45", "ior 1.50", "ior 1.55",
	"ior 1.60", "ior 1.65", "ior 1.70", "ior 1.75", "ior 1.80", "ior 1.85",
	"ior 1.90", "ior 1.95", "ior 2.00", "ior 2.05", "ior 2.10", "ior 2.15",
	"ior 2.20", "ior 2.25", "ior 2.30", "ior 2.35", "ior 2.40", "ior 2.45", "ior 2.50"
};

// include "glass.inc"
const CString G_Color2[] = {											// ガラス 色(透明)
	"Col_Glass_Old", "Col_Glass_Winebottle", "Col_Glass_Beerbottle",
	"Col_Glass_Ruby", "Col_Glass_Green", "Col_Glass_Dark_Green",
	"Col_Glass_Yellow", "Col_Glass_Orange", "Col_Glass_Vicksbottle"
};

#endif ___TEXTURE_H

//==============================<EOF:Texture.h>==============================