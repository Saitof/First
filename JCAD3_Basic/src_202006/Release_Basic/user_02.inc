//********************************************************
//
//  ユーザ定義テクスチャ２
//
//  ※ 以下の宣言を行うユーザ定義テクスチャ群です。
//
//     object { ??????(オブジェクト名)
//       material { ??????(ユーザ定義テクスチャ名) }
//       photons {
//         target            --
//         collect on/off     |- photons の設定結果に
//         reflection on/off  |  依存
//         refraction on/off --
//       }
//       translate <?, ?, ?> --
//       rotate <?, ?, ?>     |- 環境設定の結果に依存
//       scale <?, ?, ?>     --
//     }
//
//  注意) 標準のインクルードファイルは、このファイル内で
//        宣言せずに、POV-Ray記述ファイル(.pov)内でユーザ
//        定義インクルードファイルの宣言の前で、宣言して
//        ください。
//
//********************************************************

//********************************************************
//  GEM
//********************************************************
#declare Diamond =
material { 
	texture { 
		pigment { rgbf <1,1,1,0.99> }
		finish {
			ambient 0.2
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}     
			specular 1.0
			roughness 0.01
		}
	}
	interior {
		ior 2.42
		fade_distance 20.0
		fade_power 3
//        dispersion 1.1
//        dispersion_samples 6
	}
}
#declare Emerald =
material {
	texture {
		pigment { rgbf <0.6,1.0,0.0,0.7> }
		finish {
			ambient rgb<0.1,0.08,0.02>
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.03
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 3
	}
}
#declare Ruby =
material {
	texture {
		pigment { rgbf <1,0.5,0.3,0.6> }
		finish {
			ambient rgb<0.15,0.09,0.04>
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.03
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 3
	}
}
#declare Sapphire =
material {
	texture {
		pigment { rgbf <0.6,0.4,1,0.6> }
		finish {
			ambient rgb<0.04,0.01,0.1>
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.03
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 3
	}
}
#declare Aquamarine =
material {
	texture {
		pigment { rgbf <0.7,0.85,1,0.9> }
		finish {
			ambient rgb<0.0,0.07,0.2>
			diffuse 0.4
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.15
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 1
	}
}
#declare Opal =
material {
	texture {
		pigment { crackle solid
			scale 0.5
			color_map {
				[0.3 color rgbf<1,0.3,0.3,0.9>]
				[0.4 color rgbf<0.3,1,0.3,0.9>]
				[0.6 color rgbf<0.3,0.3,1,0.9>]
				[0.6 color rgbf<0.2,0.2,0.2,0.9>]
			}
		}
		finish {
			ambient 0.1
			diffuse 0.4
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 0.8
			roughness 0.03
		}
	}
	interior {
		ior 1.6
		fade_distance 5
		fade_power 2
	}
}
#declare Pearl =
material {
	texture {
		pigment { color rgb <1,0.97,0.98> }
		finish {
			ambient rgb <0.4,0.37,0.37>
			diffuse 0.5
			reflection 0.15
		}
	}
	texture {
		pigment {color rgbf <0.98,0.98,1,0.9> }
		finish {
			ambient rgb <0.4,0.4,0.5>
			diffuse 0.5
			specular 0.5
			roughness 0.05
			irid {
				0.25
				thickness 0.17
			}
		}
	}
}
#declare B_pearl =
material {
	texture {
		pigment { color rgb <0.22,0.20,0.25> }
		finish {
			ambient rgb <0.5,0.47,0.47>
			diffuse 0.6
			reflection 0.15
		}
	}
	texture {
		pigment {color rgbf <1.0,0.97,0.98,0.9> }
		finish {
			ambient rgb <0.4,0.4,0.5>
			diffuse 0.6
			specular 0.7
			roughness 0.05
			irid {
				0.25
				thickness 0.17
			}
		}
	}
}
#declare Garnet =
material {
	texture {
		pigment { rgbf <0.7,0.1,0.1,0.6> }
		finish {
			ambient rgb<0.15,0.04,0.06>
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.03
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 3
	}
}
#declare Amethyst =
material {
	texture {
		pigment { rgbf <0.8,0.4,1.0,0.9> }
		finish {
			ambient rgb<0.12,0.04,0.2>
			diffuse 0.6
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.05
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 1
	}
}
#declare Green_gem =
material {
	texture {
		pigment { rgbf <0.4,1.0,0.0,0.95> }
		finish {
			ambient rgb<0.0,0.4,0.0>
			diffuse 0.6
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.08
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 1
	}
}
#declare Yellow_gem =
material {
	texture {
		pigment { rgbf <0.99,1.0,0.65,0.9> }
		finish {
			ambient rgb<0.4,0.4,0.2>
			diffuse 0.6
			reflection {
				0.1,1.0
				fresnel on
			}
			specular 1.0
			roughness 0.1
		}
	} 
	interior {
		ior 1.77
		fade_distance 10
		fade_power 1
	}
}
#declare Onyx =
material {
	texture {
		pigment { color Black }
		finish {
			ambient 0.2
			brilliance 5
			diffuse 0.7
			reflection 0.1
			metallic 0.1
			specular 0.5
			roughness 0.05
		}
	}
}

#declare Turquoise =
material {
	texture{
		pigment{
			crackle offset 01 scale 4
			color_map{[0.02 colour Gray10]
			          [0.02 colour rgb<0, 0.1, 0.2>]
			          [0.04 colour rgb<0, 0.8, 0.9>]
			          [1.00 colour rgb<0, 0.2, 0.3>]}
			turbulence 1
		}
        	finish {
        		ambient 0.3
        		diffuse 0.7
			brilliance 1
        		reflection 0.1
        		specular 1
        		roughness 0.05
        	}
        }
}
//********************************************************
//Other
//********************************************************
#declare Glass_01 =
material { 
	texture { 
		pigment { rgbf <1,1,1,0.85> }
		finish {
			ambient 0.15
			diffuse 0.1
			reflection {
				0.1,1.0
				fresnel on
			}     
		specular 0.8
		roughness 0.01
		}
	}
	interior {
		ior 1.5
		fade_distance 2.0
		fade_power 2
//		dispersion 1.1
//      dispersion_samples 10
	}
}
#declare Glass_mat =
material { 
	texture { 
		pigment { rgbf <1,1,1,0.6> }
		finish {
			ambient 0.4
			diffuse 0.3
			crand 0.1    
			specular 0.3
			roughness 0.5
		}
	}
	interior {
		ior 1.5
		fade_distance 2.0
		fade_power 2
//		dispersion 1.1
//      dispersion_samples 10
	}
}
#declare Tracingpaper =
material { 
	texture { 
		pigment { rgbf <1,1,1,0.7> }
		finish {
			ambient 0.5
			diffuse 0.1
			reflection 0.01
			specular 0.05
			roughness 0.1
		}
	}
}
