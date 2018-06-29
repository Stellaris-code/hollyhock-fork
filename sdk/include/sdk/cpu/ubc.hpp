#pragma once
#include <stdint.h>

#define UBC_REG_CBR0 (*((uint32_t *) 0xFF200000))
#define UBC_REG_CRR0 (*((uint32_t *) 0xFF200004))
#define UBC_REG_CAR0 (*((uint32_t *) 0xFF200008))
#define UBC_REG_CAMR0 (*((uint32_t *) 0xFF20000C))
#define UBC_REG_CBCR (*((uint32_t *) 0xFF200620))

#define UBC_CBR_ID (4)
#define UBC_CBR_RW (1)
#define UBC_CBR_CE (0)

#define UBC_CRR_RESERVED (13)
#define UBC_CRR_PCB (1)
#define UBC_CRR_BIE (0)

#define UBC_CBCR_UBDE (0)
