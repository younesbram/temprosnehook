/*
 * CGameRules.h
 *
 *  Created on: Nov 23, 2017
 *      Author: nullifiedcat
 */

/*
 * !! READ ME BEFORE ADDING NEW MEMBERS !!
 * Padding should only be done using arrays of chars in order to keep things as simple as possible.
 *
 * If we're trying to add a new boolean member, then keep in mind that Little-endian is used,
 * meaning that the byte the boolean uses is stored at the smallest address. The remaining 3 bytes are
 * useless, and must be properly padded using "bool_pad". Do NOT include the boolean padding into a general "pad" padding array.
 * */

#pragma once

class CGameRules
{
public:
    char pad0[68];                // 0    | 68 bytes  | 68
    int m_iRoundState;            // 68   | 4 bytes   | 72
    bool m_bInWaitingForPlayers;  // 72   | 1 byte    | 73
    char bool_pad0[3];            // 73   | 3 bytes   | 76
    int m_iWinningTeam;           // 76   | 4 bytes   | 80
    char pad2[974];               // 80   | 974 bytes | 1054
    bool m_bPlayingMannVsMachine; // 1054 | 1 byte    | 1055
    char bool_pad1[3];            // 1055 | 3 bytes   | 1058
    char pad3[12];                // 1058 | 12 bytes  | 1070
    bool m_bIsUsingSpells;        // 1070 | 1 byte    | 1071
    char bool_pad2[3];            // 1071 | 3 bytes   | 1074
    char pad4[790];               // 1074 | 790 bytes | 1864
    int m_halloweenScenario;      // 1864 | 4 bytes   | 1868

    bool IsUsingSpells() const
    {
        auto tf_spells_enabled = g_ICvar->FindVar("tf_spells_enabled");
        if (tf_spells_enabled->GetBool())
            return true;

        // Hightower
        if (m_halloweenScenario == 4)
            return true;

        return m_bIsUsingSpells;
    }
} __attribute__((packed));
