//********************************************************
//
//  ���[�U��`�e�N�X�`���P
//
//  �� �ȉ��̐錾���s�����[�U��`�e�N�X�`���Q�ł��B
//
//     object { ??????(�I�u�W�F�N�g��)
//       material { ??????(���[�U��`�e�N�X�`����) }
//       translate <?, ?, ?> --
//       rotate <?, ?, ?>     |- ���ݒ�̌��ʂɈˑ�
//       scale <?, ?, ?>     --
//     }
//
//  ����) �W���̃C���N���[�h�t�@�C���́A���̃t�@�C������
//        �錾�����ɁAPOV-Ray�L�q�t�@�C��(pov)���Ń��[�U
//        ��`�C���N���[�h�t�@�C���̐錾�̑O�ŁA�錾����
//        ���������B
//
//********************************************************

//********************************************************
//  METAL
//********************************************************
#declare Gold =
material {
	texture {
		pigment {color rgb <1.00,0.91,0.60> }
		finish {
			ambient rgb <0.0074,0.0068,0.0044>
			brilliance 20
			diffuse 0.7
			reflection rgb <0.63,0.49,0.17>
			metallic
			specular 0.8
			roughness 0.015
		}
	}
}
#declare Gold64 =
material {
	texture {
		pigment {color rgb <1.00,0.96,0.56> }
		finish {
			ambient rgb <0.074,0.070,0.042>
			brilliance 40
			diffuse 0.7
			reflection rgb <0.74,0.70,0.42>
			metallic
			specular 0.8
			roughness 0.015
		}
	}
}
#declare Silver =
material {
	texture {
		pigment { color rgb <0.96,0.98,1.00> }
		finish {
			ambient 0.08
			brilliance 40
			diffuse 0.7
			reflection rgb <0.78,0.78,0.80>
			metallic
			specular 0.85
			roughness 0.01
		}
	}
}
#declare Copper =
material {
	texture {
		pigment {color rgb <1.00,0.92,0.84> }
		finish {
			ambient rgb <0.078,0.068,0.060>
			brilliance 40
			diffuse 0.7
			reflection rgb <0.78,0.68,0.60>
			metallic
			specular 0.8
			roughness 0.015
		}
	}
}
#declare Gold_mat =
material {
	texture {
		pigment {color rgb <1.00,0.93,0.5> }
		finish {
			ambient rgb <0.3,0.26,0.1>
			brilliance 0.3
			diffuse 0.2
			reflection rgb <0.12,0.1,0.03>
			specular 1.0
			roughness 0.8
			crand 0.8
		}
	}
}
#declare Gold64_mat =
material {
	texture {
		pigment {color rgb <1.00,0.97,0.48> }
		finish {
			ambient rgb <0.3,0.29,0.1>
			brilliance 0.3
			diffuse 0.2
			reflection rgb <0.25,0.24,0.05>
			specular 1.0
			roughness 0.8
			crand 0.8
		}
	}
}
#declare Silver_mat =
material {
	texture {
		pigment { color rgb <0.90,0.92,0.94> }
		finish {
			ambient 0.3
			brilliance 0.3
			diffuse 0.2
			reflection 0.10
			specular 1.0
			roughness 0.8
			crand 0.8
		}
	}
}
#declare Blackmetal =
material {
	texture {
		pigment { color Black }
		finish {
			ambient 0.08
			brilliance 40
			diffuse 0.7
			reflection 0.25
			metallic 0.0
			specular 0.6
			roughness 0.01
		}
	}
}
#declare Mokume01 =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.5 color rgb<0.8,0.3,0.1>][0.5 White]}
			scale 1
			rotate <90,0,0>
		}
		finish {
			ambient 0.05
			brilliance 20
			diffuse 0.5
			reflection 0.7
			metallic
			specular 0.8
			roughness 0.3
		}
	}
}
#declare Mokume02 =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.5 White][0.5 Black]}
			scale 1
			rotate <90,0,0>
		}
		finish {
			ambient 0.05
			brilliance 20
			diffuse 0.5
			reflection 0.7
			metallic
			specular 0.8
			roughness 0.3
		}
	}
}
#declare Mokume03 =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.2 color rgb<0.8,0.3,0.1>][0.2 White][0.8 White][0.8 Black]}
			scale 1.3
			rotate <90,0,0>
		}
		finish {
			ambient 0.05
			brilliance 20
			diffuse 0.5
			reflection 0.7
			metallic
			specular 0.8
			roughness 0.3
		}
	}
}
#declare Mokume01_mat =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.5 color rgb<0.8,0.3,0.1>][0.5 White]}
			rotate <90,0,0>
			scale 1
		}
		finish {
			ambient 0.3
			brilliance 0.3
			diffuse 0.4
			reflection 0.25
			specular 0.8
			roughness 0.6
			crand 0.12
		}
	}
}
#declare Mokume02_mat =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.5 White][0.5 Black]}
			scale 1
			rotate <90,0,0>
		}
		finish {
			ambient 0.3
			brilliance 0.3
			diffuse 0.4
			reflection 0.25
			specular 0.8
			roughness 0.6
			crand 0.12
		}
	}
}
#declare Mokume03_mat =
material {
	texture {
		pigment {
			agate
			agate_turb 0.7
			color_map {[0.2 color rgb<0.8,0.3,0.1>][0.2 White][0.8 White][0.8 Black]}
			scale 1.3
			rotate <90,0,0>
		}
		finish {
			ambient 0.3
			brilliance 0.3
			diffuse 0.4
			reflection 0.25
			specular 0.8
			roughness 0.6
			crand 0.12
		}
	}
}
			
//********************************************************
//  Other
//********************************************************
#declare Ceramic =
material { 
	texture { 
		pigment { rgb <1,1,1> }
		finish {
			ambient 0.3
			diffuse 0.3
			reflection {
				0.1,1.0
				fresnel on
			}     
			specular 0.8
			roughness 0.005
		}
	}
}