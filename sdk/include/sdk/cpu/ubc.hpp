/**
 * @file
 * @brief UBC (User Break Controller).
 */
#pragma once
#include <stdint.h>

/// Match condition setting register 0.
volatile uint32_t &UBC_REG_CBR0 = *reinterpret_cast<uint32_t *>(0xFF200000);
/// Match operation setting register 0.
volatile uint32_t &UBC_REG_CRR0 = *reinterpret_cast<uint32_t *>(0xFF200004);
/// Match address setting register 0.
volatile uint32_t &UBC_REG_CAR0 = *reinterpret_cast<uint32_t *>(0xFF200008);
/// Match address mask setting register 0.
volatile uint32_t &UBC_REG_CAMR0 = *reinterpret_cast<uint32_t *>(0xFF20000C);
/// Break control register.
volatile uint32_t &UBC_REG_CBCR = *reinterpret_cast<uint32_t *>(0xFF200620);

/// CBR.ID offset (bits).
const uint32_t UBC_CBR_ID = 4;
/// CBR.RW offset (bits).
const uint32_t UBC_CBR_RW = 1;
/// CBR.CE offset (bits).
const uint32_t UBC_CBR_CE = 0;

/// CRR.RESERVED offset (bits).
const uint32_t UBC_CRR_RESERVED = 13;
/// CRR.PCB offset (bits).
const uint32_t UBC_CRR_PCB = 1;
/// CRR.BIE offset (bits).
const uint32_t UBC_CRR_BIE = 0;

/// CBCR.UBDE offset (bits).
const uint32_t UBC_CBCR_UBDE = 0;
