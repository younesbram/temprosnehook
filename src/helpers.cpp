/*
 * helpers.cpp
 *
 *  Created on: Oct 8, 2016
 *      Author: nullifiedcat
 */

#include "common.hpp"
#include "DetourHook.hpp"
#include "settings/Bool.hpp"
#include "MiscTemporary.hpp"
#include "PlayerTools.hpp"
#include "Ragdolls.hpp"
#include "WeaponData.hpp"

static settings::Boolean tcm{ "debug.tcm", "true" };
static settings::Boolean should_correct_punch{ "debug.correct-punch", "true" };

std::vector<ConVar *> &RegisteredVarsList()
{
    static std::vector<ConVar *> list{};
    return list;
}

std::vector<ConCommand *> &RegisteredCommandsList()
{
    static std::vector<ConCommand *> list{};
    return list;
}

void BeginConVars()
{
    logging::Info("Begin ConVars");
    if (!std::ifstream("tf/cfg/betrayals.cfg"))
    {
        std::ofstream cfg_betrayals("tf/cfg/betrayals.cfg");
        cfg_betrayals.close();
    }

    if (!std::ifstream("tf/cfg/cat_autoexec_textmode.cfg"))
    {
        std::ofstream cfg_autoexec_textmode("tf/cfg/cat_autoexec_textmode.cfg", std::ios::out | std::ios::trunc);
        if (cfg_autoexec_textmode.good())
            cfg_autoexec_textmode << "// Put your custom Rosnehook settings in this "
                                     "file\n// This script will be executed EACH TIME "
                                     "YOU INJECT TEXTMODE Rosnehook\n"
                                     "sv_cheats 1\n"
                                     "engine_no_focus_sleep 0\n"
                                     "hud_fastswitch 1\n"
                                     "tf_medigun_autoheal 1\n"
                                     "cat_fixvac\n"
                                     "fps_max 30\n"
                                     "cat_ipc_connect";

        cfg_autoexec_textmode.close();
    }

    if (!std::ifstream("tf/cfg/trusted.cfg"))
    {
        std::ofstream cfg_trusted("tf/cfg/trusted.cfg", std::ios::out | std::ios::trunc);
        if (cfg_trusted.good())
            cfg_trusted << "// trusted ppl\n"
                            "cat_pl_add_id 1231699043 FRIEND";

        cfg_trusted.close();
    }

    if (!std::ifstream("tf/cfg/cheaters.cfg")) // undone
    {
        std::ofstream cfg_trusted("tf/cfg/cheaters.cfg", std::ios::out | std::ios::trunc);
        if (cfg_cheaters.good())
            cfg_cheaters << "// cheaters for auto requeue\n"
                            "cat_pl_add_id 366175960 CHEATER\n"
                            "cat_pl_add_id 127644507 CHEATER\n"
                            "cat_pl_add_id 127644507 CHEATER\n"
                            "cat_pl_add_id 300435 CHEATER\n"
                            "cat_pl_add_id 426937 CHEATER\n"
                            "cat_pl_add_id 687805 CHEATER\n"
                            "cat_pl_add_id 1547677 CHEATER\n"
                            "cat_pl_add_id 1549463 CHEATER\n"
                            "cat_pl_add_id 1602884 CHEATER\n"
                            "cat_pl_add_id 1691706 CHEATER\n"
                            "cat_pl_add_id 1867592 CHEATER\n"
                            "cat_pl_add_id 2964179 CHEATER\n"
                            "cat_pl_add_id 3084891 CHEATER\n"
                            "cat_pl_add_id 3298857 CHEATER\n"
                            "cat_pl_add_id 3516622 CHEATER\n"
                            "cat_pl_add_id 4396173 CHEATER\n"
                            "cat_pl_add_id 5877833 CHEATER\n"
                            "cat_pl_add_id 6222253 CHEATER\n"
                            "cat_pl_add_id 6407180 CHEATER\n"
                            "cat_pl_add_id 6781195 CHEATER\n"
                            "cat_pl_add_id 7161997 CHEATER\n"
                            "cat_pl_add_id 9089568 CHEATER\n"
                            "cat_pl_add_id 9177145 CHEATER\n"
                            "cat_pl_add_id 9691648 CHEATER\n"
                            "cat_pl_add_id 11094100 CHEATER\n"
                            "cat_pl_add_id 11703300 CHEATER\n"
                            "cat_pl_add_id 15099480 CHEATER\n"
                            "cat_pl_add_id 15240372 CHEATER\n"
                            "cat_pl_add_id 15849520 CHEATER\n"
                            "cat_pl_add_id 15894196 CHEATER\n"
                            "cat_pl_add_id 16944372 CHEATER\n"
                            "cat_pl_add_id 24811131 CHEATER\n"
                            "cat_pl_add_id 28133832 CHEATER\n"
                            "cat_pl_add_id 32868238 CHEATER\n"
                            "cat_pl_add_id 34069118 CHEATER\n"
                            "cat_pl_add_id 40823423 CHEATER\n"
                            "cat_pl_add_id 41080530 CHEATER\n"
                            "cat_pl_add_id 41436789 CHEATER\n"
                            "cat_pl_add_id 46045047 CHEATER\n"
                            "cat_pl_add_id 48698969 CHEATER\n"
                            "cat_pl_add_id 50344049 CHEATER\n"
                            "cat_pl_add_id 52100042 CHEATER\n"
                            "cat_pl_add_id 53174147 CHEATER\n"
                            "cat_pl_add_id 55880231 CHEATER\n"
                            "cat_pl_add_id 56195273 CHEATER\n"
                            "cat_pl_add_id 56850276 CHEATER\n"
                            "cat_pl_add_id 57667768 CHEATER\n"
                            "cat_pl_add_id 61235695 CHEATER\n"
                            "cat_pl_add_id 61438565 CHEATER\n"
                            "cat_pl_add_id 62770104 CHEATER\n"
                            "cat_pl_add_id 63087379 CHEATER\n"
                            "cat_pl_add_id 64467201 CHEATER\n"
                            "cat_pl_add_id 64589713 CHEATER\n"
                            "cat_pl_add_id 65602717 CHEATER\n"
                            "cat_pl_add_id 66478947 CHEATER\n"
                            "cat_pl_add_id 66728419 CHEATER\n"
                            "cat_pl_add_id 67413236 CHEATER\n"
                            "cat_pl_add_id 73369985 CHEATER\n"
                            "cat_pl_add_id 74082796 CHEATER\n"
                            "cat_pl_add_id 81967999 CHEATER\n"
                            "cat_pl_add_id 83816164 CHEATER\n"
                            "cat_pl_add_id 86618449 CHEATER\n"
                            "cat_pl_add_id 94578106 CHEATER\n"
                            "cat_pl_add_id 94964633 CHEATER\n"
                            "cat_pl_add_id 96389784 CHEATER\n"
                            "cat_pl_add_id 98856943 CHEATER\n"
                            "cat_pl_add_id 100118019 CHEATER\n"
                            "cat_pl_add_id 100803189 CHEATER\n"
                            "cat_pl_add_id 101902710 CHEATER\n"
                            "cat_pl_add_id 102438035 CHEATER\n"
                            "cat_pl_add_id 102613249 CHEATER\n"
                            "cat_pl_add_id 104019511 CHEATER\n"
                            "cat_pl_add_id 105126002 CHEATER\n"
                            "cat_pl_add_id 105224474 CHEATER\n"
                            "cat_pl_add_id 106612944 CHEATER\n"
                            "cat_pl_add_id 108154968 CHEATER\n"
                            "cat_pl_add_id 111320042 CHEATER\n"
                            "cat_pl_add_id 111727295 CHEATER\n"
                            "cat_pl_add_id 111943220 CHEATER\n"
                            "cat_pl_add_id 113922202 CHEATER\n"
                            "cat_pl_add_id 118019931 CHEATER\n"
                            "cat_pl_add_id 121623564 CHEATER\n"
                            "cat_pl_add_id 122745208 CHEATER\n"
                            "cat_pl_add_id 123070901 CHEATER\n"
                            "cat_pl_add_id 123081271 CHEATER\n"
                            "cat_pl_add_id 125970549 CHEATER\n"
                            "cat_pl_add_id 125981603 CHEATER\n"
                            "cat_pl_add_id 127388610 CHEATER\n"
                            "cat_pl_add_id 127644507 CHEATER\n"
                            "cat_pl_add_id 129372990 CHEATER\n"
                            "cat_pl_add_id 131030460 CHEATER\n"
                            "cat_pl_add_id 131654210 CHEATER\n"
                            "cat_pl_add_id 136608102 CHEATER\n"
                            "cat_pl_add_id 136698600 CHEATER\n"
                            "cat_pl_add_id 139099285 CHEATER\n"
                            "cat_pl_add_id 141618071 CHEATER\n"
                            "cat_pl_add_id 146872269 CHEATER\n"
                            "cat_pl_add_id 147131832 CHEATER\n"
                            "cat_pl_add_id 147831332 CHEATER\n"
                            "cat_pl_add_id 149499803 CHEATER\n"
                            "cat_pl_add_id 151729549 CHEATER\n"
                            "cat_pl_add_id 153698433 CHEATER\n"
                            "cat_pl_add_id 157077584 CHEATER\n"
                            "cat_pl_add_id 157117661 CHEATER\n"
                            "cat_pl_add_id 157388780 CHEATER\n"
                            "cat_pl_add_id 158568181 CHEATER\n"
                            "cat_pl_add_id 162692528 CHEATER\n"
                            "cat_pl_add_id 163126411 CHEATER\n"
                            "cat_pl_add_id 164120700 CHEATER\n"
                            "cat_pl_add_id 165126781 CHEATER\n"
                            "cat_pl_add_id 165537643 CHEATER\n"
                            "cat_pl_add_id 166361110 CHEATER\n"
                            "cat_pl_add_id 168138588 CHEATER\n"
                            "cat_pl_add_id 170224967 CHEATER\n"
                            "cat_pl_add_id 170552789 CHEATER\n"
                            "cat_pl_add_id 173444279 CHEATER\n"
                            "cat_pl_add_id 175690057 CHEATER\n"
                            "cat_pl_add_id 176909551 CHEATER\n"
                            "cat_pl_add_id 177545963 CHEATER\n"
                            "cat_pl_add_id 181850103 CHEATER\n"
                            "cat_pl_add_id 183952210 CHEATER\n"
                            "cat_pl_add_id 188949814 CHEATER\n"
                            "cat_pl_add_id 190435171 CHEATER\n"
                            "cat_pl_add_id 191554052 CHEATER\n"
                            "cat_pl_add_id 195110334 CHEATER\n"
                            "cat_pl_add_id 196286112 CHEATER\n"
                            "cat_pl_add_id 197950089 CHEATER\n"
                            "cat_pl_add_id 198514829 CHEATER\n"
                            "cat_pl_add_id 199110399 CHEATER\n"
                            "cat_pl_add_id 199528004 CHEATER\n"
                            "cat_pl_add_id 202996424 CHEATER\n"
                            "cat_pl_add_id 203331916 CHEATER\n"
                            "cat_pl_add_id 203342971 CHEATER\n"
                            "cat_pl_add_id 204202480 CHEATER\n"
                            "cat_pl_add_id 204817014 CHEATER\n"
                            "cat_pl_add_id 205876590 CHEATER\n"
                            "cat_pl_add_id 207507426 CHEATER\n"
                            "cat_pl_add_id 207572859 CHEATER\n"
                            "cat_pl_add_id 208901157 CHEATER\n"
                            "cat_pl_add_id 209751215 CHEATER\n"
                            "cat_pl_add_id 211205495 CHEATER\n"
                            "cat_pl_add_id 211777966 CHEATER\n"
                            "cat_pl_add_id 215459509 CHEATER\n"
                            "cat_pl_add_id 220132395 CHEATER\n"
                            "cat_pl_add_id 224670169 CHEATER\n"
                            "cat_pl_add_id 228833498 CHEATER\n"
                            "cat_pl_add_id 229125209 CHEATER\n"
                            "cat_pl_add_id 229748250 CHEATER\n"
                            "cat_pl_add_id 230288982 CHEATER\n"
                            "cat_pl_add_id 230686998 CHEATER\n"
                            "cat_pl_add_id 232163794 CHEATER\n"
                            "cat_pl_add_id 234568402 CHEATER\n"
                            "cat_pl_add_id 236327626 CHEATER\n"
                            "cat_pl_add_id 238045463 CHEATER\n"
                            "cat_pl_add_id 238416754 CHEATER\n"
                            "cat_pl_add_id 239042645 CHEATER\n"
                            "cat_pl_add_id 239349517 CHEATER\n"
                            "cat_pl_add_id 239896129 CHEATER\n"
                            "cat_pl_add_id 247893525 CHEATER\n"
                            "cat_pl_add_id 249194567 CHEATER\n"
                            "cat_pl_add_id 253656939 CHEATER\n"
                            "cat_pl_add_id 253844483 CHEATER\n"
                            "cat_pl_add_id 254522807 CHEATER\n"
                            "cat_pl_add_id 256908184 CHEATER\n"
                            "cat_pl_add_id 259962249 CHEATER\n"
                            "cat_pl_add_id 276155336 CHEATER\n"
                            "cat_pl_add_id 276349374 CHEATER\n"
                            "cat_pl_add_id 277904552 CHEATER\n"
                            "cat_pl_add_id 278196044 CHEATER\n"
                            "cat_pl_add_id 280969350 CHEATER\n"
                            "cat_pl_add_id 282470170 CHEATER\n"
                            "cat_pl_add_id 282926090 CHEATER\n"
                            "cat_pl_add_id 286181282 CHEATER\n"
                            "cat_pl_add_id 288563606 CHEATER\n"
                            "cat_pl_add_id 288728546 CHEATER\n"
                            "cat_pl_add_id 293224509 CHEATER\n"
                            "cat_pl_add_id 295280537 CHEATER\n"
                            "cat_pl_add_id 295499753 CHEATER\n"
                            "cat_pl_add_id 295813087 CHEATER\n"
                            "cat_pl_add_id 296273550 CHEATER\n"
                            "cat_pl_add_id 296627298 CHEATER\n"
                            "cat_pl_add_id 297889732 CHEATER\n"
                            "cat_pl_add_id 297948573 CHEATER\n"
                            "cat_pl_add_id 298381931 CHEATER\n"
                            "cat_pl_add_id 298538125 CHEATER\n"
                            "cat_pl_add_id 298786441 CHEATER\n"
                            "cat_pl_add_id 299971603 CHEATER\n"
                            "cat_pl_add_id 300677127 CHEATER\n"
                            "cat_pl_add_id 301439529 CHEATER\n"
                            "cat_pl_add_id 301868824 CHEATER\n"
                            "cat_pl_add_id 302789200 CHEATER\n"
                            "cat_pl_add_id 304256835 CHEATER\n"
                            "cat_pl_add_id 306902159 CHEATER\n"
                            "cat_pl_add_id 311760627 CHEATER\n"
                            "cat_pl_add_id 311776389 CHEATER\n"
                            "cat_pl_add_id 312314133 CHEATER\n"
                            "cat_pl_add_id 313366566 CHEATER\n"
                            "cat_pl_add_id 320574411 CHEATER\n"
                            "cat_pl_add_id 327243820 CHEATER\n"
                            "cat_pl_add_id 327297722 CHEATER\n"
                            "cat_pl_add_id 329680158 CHEATER\n"
                            "cat_pl_add_id 330521962 CHEATER\n"
                            "cat_pl_add_id 330896953 CHEATER\n"
                            "cat_pl_add_id 331378248 CHEATER\n"
                            "cat_pl_add_id 332708794 CHEATER\n"
                            "cat_pl_add_id 335216384 CHEATER\n"
                            "cat_pl_add_id 335243048 CHEATER\n"
                            "cat_pl_add_id 337717686 CHEATER\n"
                            "cat_pl_add_id 339274499 CHEATER\n"
                            "cat_pl_add_id 340899113 CHEATER\n"
                            "cat_pl_add_id 341254721 CHEATER\n"
                            "cat_pl_add_id 341337711 CHEATER\n"
                            "cat_pl_add_id 341643035 CHEATER\n"
                            "cat_pl_add_id 342229650 CHEATER\n"
                            "cat_pl_add_id 342810712 CHEATER\n"
                            "cat_pl_add_id 343454065 CHEATER\n"
                            "cat_pl_add_id 344259748 CHEATER\n"
                            "cat_pl_add_id 346260725 CHEATER\n"
                            "cat_pl_add_id 347892264 CHEATER\n"
                            "cat_pl_add_id 352104408 CHEATER\n"
                            "cat_pl_add_id 352524729 CHEATER\n"
                            "cat_pl_add_id 354596733 CHEATER\n"
                            "cat_pl_add_id 355018839 CHEATER\n"
                            "cat_pl_add_id 355054134 CHEATER\n"
                            "cat_pl_add_id 358544256 CHEATER\n"
                            "cat_pl_add_id 360048884 CHEATER\n"
                            "cat_pl_add_id 360489660 CHEATER\n"
                            "cat_pl_add_id 360539752 CHEATER\n"
                            "cat_pl_add_id 361424349 CHEATER\n"
                            "cat_pl_add_id 362813890 CHEATER\n"
                            "cat_pl_add_id 363889243 CHEATER\n"
                            "cat_pl_add_id 365377624 CHEATER\n"
                            "cat_pl_add_id 367052845 CHEATER\n"
                            "cat_pl_add_id 367531190 CHEATER\n"
                            "cat_pl_add_id 368920659 CHEATER\n"
                            "cat_pl_add_id 369668186 CHEATER\n"
                            "cat_pl_add_id 369976033 CHEATER\n"
                            "cat_pl_add_id 370468449 CHEATER\n"
                            "cat_pl_add_id 371145413 CHEATER\n"
                            "cat_pl_add_id 371388979 CHEATER\n"
                            "cat_pl_add_id 373004559 CHEATER\n"
                            "cat_pl_add_id 373086867 CHEATER\n"
                            "cat_pl_add_id 374035956 CHEATER\n"
                            "cat_pl_add_id 375304881 CHEATER\n"
                            "cat_pl_add_id 376265569 CHEATER\n"
                            "cat_pl_add_id 376722142 CHEATER\n"
                            "cat_pl_add_id 376776314 CHEATER\n"
                            "cat_pl_add_id 378208983 CHEATER\n"
                            "cat_pl_add_id 378842758 CHEATER\n"
                            "cat_pl_add_id 380466074 CHEATER\n"
                            "cat_pl_add_id 381188045 CHEATER\n"
                            "cat_pl_add_id 382714369 CHEATER\n"
                            "cat_pl_add_id 383954270 CHEATER\n"
                            "cat_pl_add_id 384450208 CHEATER\n"
                            "cat_pl_add_id 384859081 CHEATER\n"
                            "cat_pl_add_id 385408542 CHEATER\n"
                            "cat_pl_add_id 386852779 CHEATER\n"
                            "cat_pl_add_id 386971421 CHEATER\n"
                            "cat_pl_add_id 387200151 CHEATER\n"
                            "cat_pl_add_id 389636710 CHEATER\n"
                            "cat_pl_add_id 391015617 CHEATER\n"
                            "cat_pl_add_id 391728196 CHEATER\n"
                            "cat_pl_add_id 392752159 CHEATER\n"
                            "cat_pl_add_id 392786166 CHEATER\n"
                            "cat_pl_add_id 392872426 CHEATER\n"
                            "cat_pl_add_id 393146231 CHEATER\n"
                            "cat_pl_add_id 393916799 CHEATER\n"
                            "cat_pl_add_id 394221838 CHEATER\n"
                            "cat_pl_add_id 394325577 CHEATER\n"
                            "cat_pl_add_id 394416739 CHEATER\n"
                            "cat_pl_add_id 394891856 CHEATER\n"
                            "cat_pl_add_id 397479689 CHEATER\n"
                            "cat_pl_add_id 398379518 CHEATER\n"
                            "cat_pl_add_id 398994232 CHEATER\n"
                            "cat_pl_add_id 401595173 CHEATER\n"
                            "cat_pl_add_id 402488317 CHEATER\n"
                            "cat_pl_add_id 404269907 CHEATER\n"
                            "cat_pl_add_id 405613974 CHEATER\n"
                            "cat_pl_add_id 407971368 CHEATER\n"
                            "cat_pl_add_id 408458539 CHEATER\n"
                            "cat_pl_add_id 410461024 CHEATER\n"
                            "cat_pl_add_id 411120933 CHEATER\n"
                            "cat_pl_add_id 412123010 CHEATER\n"
                            "cat_pl_add_id 414325206 CHEATER\n"
                            "cat_pl_add_id 414383922 CHEATER\n"
                            "cat_pl_add_id 414831069 CHEATER\n"
                            "cat_pl_add_id 415984355 CHEATER\n"
                            "cat_pl_add_id 416760566 CHEATER\n"
                            "cat_pl_add_id 416779199 CHEATER\n"
                            "cat_pl_add_id 417474113 CHEATER\n"
                            "cat_pl_add_id 420214317 CHEATER\n"
                            "cat_pl_add_id 421193738 CHEATER\n"
                            "cat_pl_add_id 421776687 CHEATER\n"
                            "cat_pl_add_id 422420813 CHEATER\n"
                            "cat_pl_add_id 422780487 CHEATER\n"
                            "cat_pl_add_id 423418949 CHEATER\n"
                            "cat_pl_add_id 424375243 CHEATER\n"
                            "cat_pl_add_id 425065869 CHEATER\n"
                            "cat_pl_add_id 425843616 CHEATER\n"
                            "cat_pl_add_id 426848092 CHEATER\n"
                            "cat_pl_add_id 427469590 CHEATER\n"
                            "cat_pl_add_id 427749908 CHEATER\n"
                            "cat_pl_add_id 430167405 CHEATER\n"
                            "cat_pl_add_id 431730435 CHEATER\n"
                            "cat_pl_add_id 431733341 CHEATER\n"
                            "cat_pl_add_id 432073628 CHEATER\n"
                            "cat_pl_add_id 432124748 CHEATER\n"
                            "cat_pl_add_id 432359905 CHEATER\n"
                            "cat_pl_add_id 433167788 CHEATER\n"
                            "cat_pl_add_id 433823950 CHEATER\n"
                            "cat_pl_add_id 433917193 CHEATER\n"
                            "cat_pl_add_id 434708677 CHEATER\n"
                            "cat_pl_add_id 434904965 CHEATER\n"
                            "cat_pl_add_id 435350526 CHEATER\n"
                            "cat_pl_add_id 436194220 CHEATER\n"
                            "cat_pl_add_id 437164017 CHEATER\n"
                            "cat_pl_add_id 437519633 CHEATER\n"
                            "cat_pl_add_id 438539817 CHEATER\n"
                            "cat_pl_add_id 440778620 CHEATER\n"
                            "cat_pl_add_id 441213311 CHEATER\n"
                            "cat_pl_add_id 441301982 CHEATER\n"
                            "cat_pl_add_id 441938885 CHEATER\n"
                            "cat_pl_add_id 442303879 CHEATER\n"
                            "cat_pl_add_id 442598728 CHEATER\n"
                            "cat_pl_add_id 442612352 CHEATER\n"
                            "cat_pl_add_id 442869825 CHEATER\n"
                            "cat_pl_add_id 443465937 CHEATER\n"
                            "cat_pl_add_id 444028538 CHEATER\n"
                            "cat_pl_add_id 444133128 CHEATER\n"
                            "cat_pl_add_id 444240650 CHEATER\n"
                            "cat_pl_add_id 445030492 CHEATER\n"
                            "cat_pl_add_id 446953321 CHEATER\n"
                            "cat_pl_add_id 448693606 CHEATER\n"
                            "cat_pl_add_id 449549305 CHEATER\n"
                            "cat_pl_add_id 450019305 CHEATER\n"
                            "cat_pl_add_id 451168340 CHEATER\n"
                            "cat_pl_add_id 451334419 CHEATER\n"
                            "cat_pl_add_id 452700362 CHEATER\n"
                            "cat_pl_add_id 452896849 CHEATER\n"
                            "cat_pl_add_id 453279881 CHEATER\n"
                            "cat_pl_add_id 453901310 CHEATER\n"
                            "cat_pl_add_id 455693607 CHEATER\n"
                            "cat_pl_add_id 456922929 CHEATER\n"
                            "cat_pl_add_id 458058269 CHEATER\n"
                            "cat_pl_add_id 458484656 CHEATER\n"
                            "cat_pl_add_id 459524798 CHEATER\n"
                            "cat_pl_add_id 459771237 CHEATER\n"
                            "cat_pl_add_id 460586393 CHEATER\n"
                            "cat_pl_add_id 460742993 CHEATER\n"
                            "cat_pl_add_id 460787424 CHEATER\n"
                            "cat_pl_add_id 460967390 CHEATER\n"
                            "cat_pl_add_id 462158030 CHEATER\n"
                            "cat_pl_add_id 464248769 CHEATER\n"
                            "cat_pl_add_id 466595740 CHEATER\n"
                            "cat_pl_add_id 470032250 CHEATER\n"
                            "cat_pl_add_id 471666523 CHEATER\n"
                            "cat_pl_add_id 472401087 CHEATER\n"
                            "cat_pl_add_id 472948843 CHEATER\n"
                            "cat_pl_add_id 475074776 CHEATER\n"
                            "cat_pl_add_id 475332645 CHEATER\n"
                            "cat_pl_add_id 477800468 CHEATER\n"
                            "cat_pl_add_id 478469746 CHEATER\n"
                            "cat_pl_add_id 479329091 CHEATER\n"
                            "cat_pl_add_id 479406224 CHEATER\n"
                            "cat_pl_add_id 479913242 CHEATER\n"
                            "cat_pl_add_id 480090470 CHEATER\n"
                            "cat_pl_add_id 480473009 CHEATER\n"
                            "cat_pl_add_id 482299065 CHEATER\n"
                            "cat_pl_add_id 482385112 CHEATER\n"
                            "cat_pl_add_id 482911609 CHEATER\n"
                            "cat_pl_add_id 483420727 CHEATER\n"
                            "cat_pl_add_id 483645952 CHEATER\n"
                            "cat_pl_add_id 484192480 CHEATER\n"
                            "cat_pl_add_id 485469846 CHEATER\n"
                            "cat_pl_add_id 486722664 CHEATER\n"
                            "cat_pl_add_id 488878765 CHEATER\n"
                            "cat_pl_add_id 490284148 CHEATER\n"
                            "cat_pl_add_id 490499633 CHEATER\n"
                            "cat_pl_add_id 491652888 CHEATER\n"
                            "cat_pl_add_id 491955931 CHEATER\n"
                            "cat_pl_add_id 835332480 CHEATER\n"
                            "cat_pl_add_id 835890375 CHEATER\n"
                            "cat_pl_add_id 836122033 CHEATER\n"
                            "cat_pl_add_id 837679997 CHEATER\n"
                            "cat_pl_add_id 838132084 CHEATER\n"
                            "cat_pl_add_id 838823723 CHEATER\n"
                            "cat_pl_add_id 838980805 CHEATER\n"
                            "cat_pl_add_id 839088328 CHEATER\n"
                            "cat_pl_add_id 839879788 CHEATER\n"
                            "cat_pl_add_id 840098562 CHEATER\n"
                            "cat_pl_add_id 840158649 CHEATER\n"
                            "cat_pl_add_id 840255344 CHEATER\n"
                            "cat_pl_add_id 840846849 CHEATER\n"
                            "cat_pl_add_id 840899897 CHEATER\n"
                            "cat_pl_add_id 840914878 CHEATER\n"
                            "cat_pl_add_id 841693587 CHEATER\n"
                            "cat_pl_add_id 842918336 CHEATER\n"
                            "cat_pl_add_id 844304503 CHEATER\n"
                            "cat_pl_add_id 844312366 CHEATER\n"
                            "cat_pl_add_id 844365140 CHEATER\n"
                            "cat_pl_add_id 845235870 CHEATER\n"
                            "cat_pl_add_id 845531589 CHEATER\n"
                            "cat_pl_add_id 847909804 CHEATER\n"
                            "cat_pl_add_id 848301553 CHEATER\n"
                            "cat_pl_add_id 848975435 CHEATER\n"
                            "cat_pl_add_id 849331125 CHEATER\n"
                            "cat_pl_add_id 849507353 CHEATER\n"
                            "cat_pl_add_id 849812984 CHEATER\n"
                            "cat_pl_add_id 849882545 CHEATER\n"
                            "cat_pl_add_id 851311880 CHEATER\n"
                            "cat_pl_add_id 851471352 CHEATER\n"
                            "cat_pl_add_id 851811617 CHEATER\n"
                            "cat_pl_add_id 853102111 CHEATER\n"
                            "cat_pl_add_id 854096559 CHEATER\n"
                            "cat_pl_add_id 854198748 CHEATER\n"
                            "cat_pl_add_id 855322876 CHEATER\n"
                            "cat_pl_add_id 855391905 CHEATER\n"
                            "cat_pl_add_id 859134916 CHEATER\n"
                            "cat_pl_add_id 859594295 CHEATER\n"
                            "cat_pl_add_id 860089994 CHEATER\n"
                            "cat_pl_add_id 862307140 CHEATER\n"
                            "cat_pl_add_id 863324091 CHEATER\n"
                            "cat_pl_add_id 865737573 CHEATER\n"
                            "cat_pl_add_id 865849498 CHEATER\n"
                            "cat_pl_add_id 867951015 CHEATER\n"
                            "cat_pl_add_id 868516379 CHEATER\n"
                            "cat_pl_add_id 869752255 CHEATER\n"
                            "cat_pl_add_id 870068556 CHEATER\n"
                            "cat_pl_add_id 874132779 CHEATER\n"
                            "cat_pl_add_id 874317011 CHEATER\n"
                            "cat_pl_add_id 875974091 CHEATER\n"
                            "cat_pl_add_id 876233570 CHEATER\n"
                            "cat_pl_add_id 876245927 CHEATER\n"
                            "cat_pl_add_id 878668457 CHEATER\n"
                            "cat_pl_add_id 878673154 CHEATER\n"
                            "cat_pl_add_id 879096646 CHEATER\n"
                            "cat_pl_add_id 880083326 CHEATER\n"
                            "cat_pl_add_id 882228170 CHEATER\n"
                            "cat_pl_add_id 882463899 CHEATER\n"
                            "cat_pl_add_id 884038686 CHEATER\n"
                            "cat_pl_add_id 884071316 CHEATER\n"
                            "cat_pl_add_id 884332935 CHEATER\n"
                            "cat_pl_add_id 884588342 CHEATER\n"
                            "cat_pl_add_id 885866798 CHEATER\n"
                            "cat_pl_add_id 886555176 CHEATER\n"
                            "cat_pl_add_id 887033387 CHEATER\n"
                            "cat_pl_add_id 887402949 CHEATER\n"
                            "cat_pl_add_id 888835274 CHEATER\n"
                            "cat_pl_add_id 889128653 CHEATER\n"
                            "cat_pl_add_id 889830039 CHEATER\n"
                            "cat_pl_add_id 891972674 CHEATER\n"
                            "cat_pl_add_id 892057503 CHEATER\n"
                            "cat_pl_add_id 892176401 CHEATER\n"
                            "cat_pl_add_id 892178230 CHEATER\n"
                            "cat_pl_add_id 892959250 CHEATER\n"
                            "cat_pl_add_id 894808711 CHEATER\n"
                            "cat_pl_add_id 895656206 CHEATER\n"
                            "cat_pl_add_id 896869626 CHEATER\n"
                            "cat_pl_add_id 898124706 CHEATER\n"
                            "cat_pl_add_id 899592714 CHEATER\n"
                            "cat_pl_add_id 901052341 CHEATER\n"
                            "cat_pl_add_id 902353804 CHEATER\n"
                            "cat_pl_add_id 904700665 CHEATER\n"
                            "cat_pl_add_id 907871942 CHEATER\n"
                            "cat_pl_add_id 908261577 CHEATER\n"
                            "cat_pl_add_id 911494944 CHEATER\n"
                            "cat_pl_add_id 911557776 CHEATER\n"
                            "cat_pl_add_id 912317175 CHEATER\n"
                            "cat_pl_add_id 912611677 CHEATER\n"
                            "cat_pl_add_id 916451492 CHEATER\n"
                            "cat_pl_add_id 917086433 CHEATER\n"
                            "cat_pl_add_id 917527628 CHEATER\n"
                            "cat_pl_add_id 917552392 CHEATER\n"
                            "cat_pl_add_id 920548549 CHEATER\n"
                            "cat_pl_add_id 921378280 CHEATER\n"
                            "cat_pl_add_id 922829098 CHEATER\n"
                            "cat_pl_add_id 923711125 CHEATER\n"
                            "cat_pl_add_id 931702397 CHEATER\n"
                            "cat_pl_add_id 932457285 CHEATER\n"
                            "cat_pl_add_id 946778118 CHEATER\n"
                            "cat_pl_add_id 953602967 CHEATER\n"
                            "cat_pl_add_id 961676494 CHEATER\n"
                            "cat_pl_add_id 963689585 CHEATER\n"
                            "cat_pl_add_id 969367416 CHEATER\n"
                            "cat_pl_add_id 972594997 CHEATER\n"
                            "cat_pl_add_id 973339638 CHEATER\n"
                            "cat_pl_add_id 979308945 CHEATER\n"
                            "cat_pl_add_id 985087447 CHEATER\n"
                            "cat_pl_add_id 986517420 CHEATER\n"
                            "cat_pl_add_id 986975074 CHEATER\n"
                            "cat_pl_add_id 987041409 CHEATER\n"
                            "cat_pl_add_id 988735261 CHEATER\n"
                            "cat_pl_add_id 989972871 CHEATER\n"
                            "cat_pl_add_id 992958391 CHEATER\n"
                            "cat_pl_add_id 1001954473 CHEATER\n"
                            "cat_pl_add_id 1003031884 CHEATER\n"
                            "cat_pl_add_id 1004583979 CHEATER\n"
                            "cat_pl_add_id 1006882643 CHEATER\n"
                            "cat_pl_add_id 1007026141 CHEATER\n"
                            "cat_pl_add_id 1007835058 CHEATER\n"
                            "cat_pl_add_id 1008509390 CHEATER\n"
                            "cat_pl_add_id 1009133571 CHEATER\n"
                            "cat_pl_add_id 1009222743 CHEATER\n"
                            "cat_pl_add_id 1013390072 CHEATER\n"
                            "cat_pl_add_id 1013498095 CHEATER\n"
                            "cat_pl_add_id 1018688777 CHEATER\n"
                            "cat_pl_add_id 1019005641 CHEATER\n"
                            "cat_pl_add_id 1019784297 CHEATER\n"
                            "cat_pl_add_id 1019871869 CHEATER\n"
                            "cat_pl_add_id 1020923667 CHEATER\n"
                            "cat_pl_add_id 1021096547 CHEATER\n"
                            "cat_pl_add_id 1022074647 CHEATER\n"
                            "cat_pl_add_id 1022352794 CHEATER\n"
                            "cat_pl_add_id 1025370198 CHEATER\n"
                            "cat_pl_add_id 1025915115 CHEATER\n"
                            "cat_pl_add_id 1026013480 CHEATER\n"
                            "cat_pl_add_id 1026771585 CHEATER\n"
                            "cat_pl_add_id 1026996556 CHEATER\n"
                            "cat_pl_add_id 1029717989 CHEATER\n"
                            "cat_pl_add_id 1031429255 CHEATER\n"
                            "cat_pl_add_id 1033995680 CHEATER\n"
                            "cat_pl_add_id 1039895334 CHEATER\n"
                            "cat_pl_add_id 1040118766 CHEATER\n"
                            "cat_pl_add_id 1040696257 CHEATER\n"
                            "cat_pl_add_id 1041316046 CHEATER\n"
                            "cat_pl_add_id 1041651820 CHEATER\n"
                            "cat_pl_add_id 1045278835 CHEATER\n"
                            "cat_pl_add_id 1045793628 CHEATER\n"
                            "cat_pl_add_id 1047193547 CHEATER\n"
                            "cat_pl_add_id 1047378838 CHEATER\n"
                            "cat_pl_add_id 1048875178 CHEATER\n"
                            "cat_pl_add_id 1050175254 CHEATER\n"
                            "cat_pl_add_id 1050518194 CHEATER\n"
                            "cat_pl_add_id 1052657961 CHEATER\n"
                            "cat_pl_add_id 1053085942 CHEATER\n"
                            "cat_pl_add_id 1053400644 CHEATER\n"
                            "cat_pl_add_id 1055224911 CHEATER\n"
                            "cat_pl_add_id 1055807152 CHEATER\n"
                            "cat_pl_add_id 1058004556 CHEATER\n"
                            "cat_pl_add_id 1058893317 CHEATER\n"
                            "cat_pl_add_id 1063802094 CHEATER\n"
                            "cat_pl_add_id 1064627644 CHEATER\n"
                            "cat_pl_add_id 1068149858 CHEATER\n"
                            "cat_pl_add_id 1069900161 CHEATER\n"
                            "cat_pl_add_id 1070930929 CHEATER\n"
                            "cat_pl_add_id 1072214128 CHEATER\n"
                            "cat_pl_add_id 1075863133 CHEATER\n"
                            "cat_pl_add_id 1076212727 CHEATER\n"
                            "cat_pl_add_id 1077419257 CHEATER\n"
                            "cat_pl_add_id 1078225559 CHEATER\n"
                            "cat_pl_add_id 1084708314 CHEATER\n"
                            "cat_pl_add_id 1084745973 CHEATER\n"
                            "cat_pl_add_id 1085072497 CHEATER\n"
                            "cat_pl_add_id 1090713653 CHEATER\n"
                            "cat_pl_add_id 1093539850 CHEATER\n"
                            "cat_pl_add_id 1096818986 CHEATER\n"
                            "cat_pl_add_id 1097932520 CHEATER\n"
                            "cat_pl_add_id 1102151487 CHEATER\n"
                            "cat_pl_add_id 1104143794 CHEATER\n"
                            "cat_pl_add_id 1105778191 CHEATER\n"
                            "cat_pl_add_id 1107715125 CHEATER\n"
                            "cat_pl_add_id 1108914686 CHEATER\n"
                            "cat_pl_add_id 1109107993 CHEATER\n"
                            "cat_pl_add_id 1110032138 CHEATER\n"
                            "cat_pl_add_id 1110115138 CHEATER\n"
                            "cat_pl_add_id 1110829956 CHEATER\n"
                            "cat_pl_add_id 1110933323 CHEATER\n"
                            "cat_pl_add_id 1112876552 CHEATER\n"
                            "cat_pl_add_id 1114543542 CHEATER\n"
                            "cat_pl_add_id 1115093726 CHEATER\n"
                            "cat_pl_add_id 1115524083 CHEATER\n"
                            "cat_pl_add_id 1115538664 CHEATER\n"
                            "cat_pl_add_id 1116660385 CHEATER\n"
                            "cat_pl_add_id 1119065647 CHEATER\n"
                            "cat_pl_add_id 1119113932 CHEATER\n"
                            "cat_pl_add_id 1119394219 CHEATER\n"
                            "cat_pl_add_id 1123754863 CHEATER\n"
                            "cat_pl_add_id 1126086627 CHEATER\n"
                            "cat_pl_add_id 1127241565 CHEATER\n"
                            "cat_pl_add_id 1128866895 CHEATER\n"
                            "cat_pl_add_id 1129660641 CHEATER\n"
                            "cat_pl_add_id 1133676235 CHEATER\n"
                            "cat_pl_add_id 1136686454 CHEATER\n"
                            "cat_pl_add_id 1138827526 CHEATER\n"
                            "cat_pl_add_id 1139061115 CHEATER\n"
                            "cat_pl_add_id 1140613262 CHEATER\n"
                            "cat_pl_add_id 1140996455 CHEATER\n"
                            "cat_pl_add_id 1141398120 CHEATER\n"
                            "cat_pl_add_id 1144367847 CHEATER\n"
                            "cat_pl_add_id 1144598942 CHEATER\n"
                            "cat_pl_add_id 1145632531 CHEATER\n"
                            "cat_pl_add_id 1146555441 CHEATER\n"
                            "cat_pl_add_id 1146657354 CHEATER\n"
                            "cat_pl_add_id 1147798264 CHEATER\n"
                            "cat_pl_add_id 1148941800 CHEATER\n"
                            "cat_pl_add_id 1151999491 CHEATER\n"
                            "cat_pl_add_id 1155964287 CHEATER\n"
                            "cat_pl_add_id 1158658897 CHEATER\n"
                            "cat_pl_add_id 1160862481 CHEATER\n"
                            "cat_pl_add_id 1161975942 CHEATER\n"
                            "cat_pl_add_id 1164594976 CHEATER\n"
                            "cat_pl_add_id 1164606592 CHEATER\n"
                            "cat_pl_add_id 1165529252 CHEATER\n"
                            "cat_pl_add_id 1165710999 CHEATER\n"
                            "cat_pl_add_id 1166686713 CHEATER\n"
                            "cat_pl_add_id 1174167762 CHEATER\n"
                            "cat_pl_add_id 1180355842 CHEATER\n"
                            "cat_pl_add_id 1181065911 CHEATER\n"
                            "cat_pl_add_id 1181623245 CHEATER\n"
                            "cat_pl_add_id 1183846430 CHEATER\n"
                            "cat_pl_add_id 1186425760 CHEATER\n"
                            "cat_pl_add_id 1188454075 CHEATER\n"
                            "cat_pl_add_id 1190652324 CHEATER\n"
                            "cat_pl_add_id 1196406412 CHEATER\n"
                            "cat_pl_add_id 1197941908 CHEATER\n"
                            "cat_pl_add_id 1199050007 CHEATER\n"
                            "cat_pl_add_id 1201793340 CHEATER\n"
                            "cat_pl_add_id 1201986525 CHEATER\n"
                            "cat_pl_add_id 1204700403 CHEATER\n"
                            "cat_pl_add_id 1211494356 CHEATER\n"
                            "cat_pl_add_id 1211696011 CHEATER\n"
                            "cat_pl_add_id 1212151146 CHEATER\n"
                            "cat_pl_add_id 1212764007 CHEATER\n"
                            "cat_pl_add_id 1217691619 CHEATER\n"
                            "cat_pl_add_id 1221253951 CHEATER\n"
                            "cat_pl_add_id 1224153722 CHEATER\n"
                            "cat_pl_add_id 1224869566 CHEATER\n"
                            "cat_pl_add_id 1229867621 CHEATER\n"
                            "cat_pl_add_id 1230483943 CHEATER\n"
                            "cat_pl_add_id 1231850813 CHEATER\n"
                            "cat_pl_add_id 1232745708 CHEATER\n"
                            "cat_pl_add_id 1234224628 CHEATER\n"
                            "cat_pl_add_id 1235135066 CHEATER\n"
                            "cat_pl_add_id 1237943394 CHEATER\n"
                            "cat_pl_add_id 1241364628 CHEATER\n"
                            "cat_pl_add_id 1244182153 CHEATER\n"
                            "cat_pl_add_id 1246063905 CHEATER\n"
                            "cat_pl_add_id 1251129637 CHEATER\n"
                            "cat_pl_add_id 1253321843 CHEATER\n"
                            "cat_pl_add_id 1256421340 CHEATER\n"
                            "cat_pl_add_id 1257269747 CHEATER\n"
                            "cat_pl_add_id 1257409833 CHEATER\n"
                            "cat_pl_add_id 1264575219 CHEATER\n"
                            "cat_pl_add_id 1278341587 CHEATER\n"
                            "cat_pl_add_id 1284781087 CHEATER\n"
                            "cat_pl_add_id 1302576535 CHEATER\n"
                            "cat_pl_add_id 1344279508 CHEATER\n"
                            "cat_pl_add_id 1383434989 CHEATER\n"
                            "cat_pl_add_id 1388980146 CHEATER\n"
                            "cat_pl_add_id 1419224836 CHEATER\n";

        cfg_cheaters.close();
    }

    if (!std::ifstream("tf/cfg/cat_autoexec.cfg"))
    {
        std::ofstream cfg_autoexec("tf/cfg/cat_autoexec.cfg", std::ios::out | std::ios::trunc);
        if (cfg_autoexec.good())
            cfg_autoexec << "// Put your custom Rosnehook settings in this "
                            "file\n// This script will be executed EACH TIME "
                            "YOU INJECT Rosnehook\n";

        cfg_autoexec.close();
    }

    if (!std::ifstream("tf/cfg/cat_matchexec.cfg"))
    {
        std::ofstream cat_matchexec("tf/cfg/cat_matchexec.cfg", std::ios::out | std::ios::trunc);
        if (cat_matchexec.good())
            cat_matchexec << "// Put your custom Rosnehook settings in this "
                             "file\n// This script will be executed EACH TIME "
                             "YOU JOIN A MATCH\n";

        cat_matchexec.close();
    }

    logging::Info(":b:");
    SetCVarInterface(g_ICvar);
}

void EndConVars()
{
    logging::Info("Registering CatCommands");
    RegisterCatCommands();
    ConVar_Register();

    std::ofstream cfg_defaults("tf/cfg/cat_defaults.cfg", std::ios::out | std::ios::trunc);
    if (cfg_defaults.good())
    {
        cfg_defaults << "// This file is auto-generated and will be "
                        "overwritten each time you inject Rosnehook\n// Do not "
                        "make edits to this file\n\n// Every registered "
                        "variable dump\n";
        for (const auto &i : RegisteredVarsList())
            cfg_defaults << i->GetName() << " \"" << i->GetDefault() << "\"\n";

        cfg_defaults << "\n// Every registered command dump\n";
        for (const auto &i : RegisteredCommandsList())
            cfg_defaults << "// " << i->GetName() << "\n";
    }
}

ConVar *CreateConVar(const std::string &name, const std::string &value, const std::string &help)
{
    char *namec  = new char[256];
    char *valuec = new char[256];
    char *helpc  = new char[256];
    strncpy(namec, name.c_str(), 255);
    strncpy(valuec, value.c_str(), 255);
    strncpy(helpc, help.c_str(), 255);
    // logging::Info("Creating ConVar: %s %s %s", namec, valuec, helpc);
    auto *ret = new ConVar(const_cast<const char *>(namec), const_cast<const char *>(valuec), 0, const_cast<const char *>(helpc));
    g_ICvar->RegisterConCommand(ret);
    RegisteredVarsList().push_back(ret);
    return ret;
}

// Function for when you want to goto a vector
void WalkTo(const Vector &vector)
{
    if (CE_BAD(LOCAL_E))
        return;
    // Calculate how to get to a vector
    auto result = ComputeMove(LOCAL_E->m_vecOrigin(), vector);
    // Push our move to usercmd
    current_user_cmd->forwardmove = result.x;
    current_user_cmd->sidemove    = result.y;
    current_user_cmd->upmove      = result.z;
}

// Function to get the corner location that a vischeck to an entity is possible
// from
Vector VischeckCorner(CachedEntity *player, CachedEntity *target, float maxdist, bool checkWalkable)
{
    int maxiterations = maxdist / 40;
    Vector origin     = player->m_vecOrigin();

    // if we can see an entity, we don't need to run calculations
    if (VisCheckEntFromEnt(player, target) && (!checkWalkable || canReachVector(origin, target->m_vecOrigin())))
        return origin;

    for (uint8 i = 0; i < 8; ++i) // for loop for all 4 directions
    {
        // 40 * maxiterations = range in HU
        for (int j = 0; j < maxiterations; ++j)
        {
            Vector virtualOrigin = origin;
            // what direction to go in
            switch (i)
            {
            case 0:
                virtualOrigin.x = virtualOrigin.x + 40 * (j + 1);
                break;
            case 1:
                virtualOrigin.x = virtualOrigin.x - 40 * (j + 1);
                break;
            case 2:
                virtualOrigin.y = virtualOrigin.y + 40 * (j + 1);
                break;
            case 3:
                virtualOrigin.y = virtualOrigin.y - 40 * (j + 1);
                break;
            case 4:
                virtualOrigin.x = virtualOrigin.x + 20 * (j + 1);
                virtualOrigin.y = virtualOrigin.y + 20 * (j + 1);
                break;
            case 5:
                virtualOrigin.x = virtualOrigin.x - 20 * (j + 1);
                virtualOrigin.y = virtualOrigin.y - 20 * (j + 1);
                break;
            case 6:
                virtualOrigin.x = virtualOrigin.x - 20 * (j + 1);
                virtualOrigin.y = virtualOrigin.y + 20 * (j + 1);
                break;
            case 7:
                virtualOrigin.x = virtualOrigin.x + 20 * (j + 1);
                virtualOrigin.y = virtualOrigin.y - 20 * (j + 1);
                [[fallthrough]];
            default:
                break;
            }
            // check if player can see the players virtualOrigin
            if (!IsVectorVisible(origin, virtualOrigin, true))
                continue;
            // check if the virtualOrigin can see the target
            if (!VisCheckEntFromEntVector(virtualOrigin, player, target))
                continue;
            if (!checkWalkable)
                return virtualOrigin;

            // check if the location is accessible
            if (!canReachVector(origin, virtualOrigin))
                continue;
            if (canReachVector(virtualOrigin, target->m_vecOrigin()))
                return virtualOrigin;
        }
    }
    // if we didn't find anything, return an empty Vector
    return { 0, 0, 0 };
}

// return Two Corners that connect perfectly to ent and local player
std::pair<Vector, Vector> VischeckWall(CachedEntity *player, CachedEntity *target, float maxdist, bool checkWalkable)
{
    int max_iterations = static_cast<int>(maxdist / 40.0f);
    Vector origin      = player->m_vecOrigin();

    // if we can see an entity, we don't need to run calculations
    if (VisCheckEntFromEnt(player, target))
    {
        std::pair<Vector, Vector> orig(origin, target->m_vecOrigin());
        if (!checkWalkable || canReachVector(origin, target->m_vecOrigin()))
            return orig;
    }

    for (uint8 i = 0; i < 8; ++i) // for loop for all 4 directions
    {
        // 40 * max_iterations = range in HU
        for (int j = 0; j < max_iterations; ++j)
        {
            Vector virtualOrigin = origin;
            // what direction to go in
            switch (i)
            {
            case 0:
                virtualOrigin.x += 40.0f * (j + 1.0f);
                break;
            case 1:
                virtualOrigin.x -= 40.0f * (j + 1.0f);
                break;
            case 2:
                virtualOrigin.y += 40.0f * (j + 1.0f);
                break;
            case 3:
                virtualOrigin.y -= 40.0f * (j + 1.0f);
                break;
            case 4:
                virtualOrigin.x += 20.0f * (j + 1.0f);
                virtualOrigin.y += 20.0f * (j + 1.0f);
                break;
            case 5:
                virtualOrigin.x -= 20.0f * (j + 1.0f);
                virtualOrigin.y -= 20.0f * (j + 1.0f);
                break;
            case 6:
                virtualOrigin.x -= 20.0f * (j + 1.0f);
                virtualOrigin.y += 20.0f * (j + 1.0f);
                break;
            case 7:
                virtualOrigin.x += 20.0f * (j + 1.0f);
                virtualOrigin.y -= 20.0f * (j + 1.0f);
                [[fallthrough]];
            default:
                break;
            }
            // check if player can see the players virtualOrigin
            if (!IsVectorVisible(origin, virtualOrigin, true))
                continue;
            for (uint8 i = 0; i < 8; ++i) // for loop for all 4 directions
            {
                // 40 * max_iterations = range in HU
                for (int j = 0; j < max_iterations; ++j)
                {
                    Vector virtualOrigin2 = target->m_vecOrigin();
                    // what direction to go in
                    switch (i)
                    {
                    case 0:
                        virtualOrigin2.x += 40.0f * (j + 1.0f);
                        break;
                    case 1:
                        virtualOrigin2.x -= 40.0f * (j + 1.0f);
                        break;
                    case 2:
                        virtualOrigin2.y += 40.0f * (j + 1.0f);
                        break;
                    case 3:
                        virtualOrigin2.y -= 40.0f * (j + 1.0f);
                        break;
                    case 4:
                        virtualOrigin2.x += 20.0f * (j + 1.0f);
                        virtualOrigin2.y += 20.0f * (j + 1.0f);
                        break;
                    case 5:
                        virtualOrigin2.x -= 20.0f * (j + 1.0f);
                        virtualOrigin2.y -= 20.0f * (j + 1.0f);
                        break;
                    case 6:
                        virtualOrigin2.x -= 20.0f * (j + 1.0f);
                        virtualOrigin2.y += 20.0f * (j + 1.0f);
                        break;
                    case 7:
                        virtualOrigin2.x += 20.0f * (j + 1.0f);
                        virtualOrigin2.y -= 20.0f * (j + 1.0f);
                        [[fallthrough]];
                    default:
                        break;
                    }
                    // check if the virtualOrigin2 can see the target
                    //                    if
                    //                    (!VisCheckEntFromEntVector(virtualOrigin2,
                    //                    player, target))
                    //                        continue;
                    //                    if (!IsVectorVisible(virtualOrigin,
                    //                    virtualOrigin2, true))
                    //                        continue;
                    //                    if (!IsVectorVisible(virtualOrigin2,
                    //                    target->m_vecOrigin(), true))
                    //                    	continue;
                    if (!IsVectorVisible(virtualOrigin, virtualOrigin2, true))
                        continue;
                    if (!IsVectorVisible(virtualOrigin2, target->m_vecOrigin()))
                        continue;
                    std::pair<Vector, Vector> toret(virtualOrigin, virtualOrigin2);
                    if (!checkWalkable)
                        return toret;
                    // check if the location is accessible
                    if (!canReachVector(origin, virtualOrigin) || !canReachVector(virtualOrigin2, virtualOrigin))
                        continue;
                    if (canReachVector(virtualOrigin2, target->m_vecOrigin()))
                        return toret;
                }
            }
        }
    }
    // if we didn't find anything, return an empty Vector
    return { { 0, 0, 0 }, { 0, 0, 0 } };
}

// Returns a vector's max value. For example: {123,-150, 125} = 125
float vectorMax(Vector i)
{
    __m128 vec = _mm_set_ps(i.z, i.y, i.x, -FLT_MAX);
    __m128 max = _mm_max_ps(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(0, 1, 2, 3)));
    max        = _mm_max_ps(max, _mm_shuffle_ps(max, max, _MM_SHUFFLE(0, 1, 0, 1)));
    return _mm_cvtss_f32(max);
}

// Returns a vector's absolute value. For example {123, -150, 125} -> {123, 150, 125}
Vector vectorAbs(Vector i)
{
    __m128 sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
    __m128 v         = _mm_loadu_ps(&i.x);
    v                = _mm_andnot_ps(sign_mask, v);
    Vector result;
    _mm_storeu_ps(&result.x, v);
    return result;
}

// check to see if we can reach a vector or if it is too high / doesn't leave enough space for the player, optional second vector
bool canReachVector(Vector loc, Vector dest)
{
    if (!dest.IsZero())
    {
        Vector dist        = dest - loc;
        int max_iterations = static_cast<int>(floor(dest.DistTo(loc)) / 40.0f);
        for (int i = 0; i < max_iterations; ++i)
        {
            // math to get the next vector 40.0f in the direction of dest
            Vector vec = loc + dist / vectorMax(vectorAbs(dist)) * 40.0f * (i + 1.0f);

            if (DistanceToGround({ vec.x, vec.y, vec.z + 5.0f }) >= 40.0f)
                return false;

            for (uint8 j = 0; j < 4; ++j)
            {
                Vector directionalLoc = vec;
                // what direction to check
                switch (j)
                {
                case 0:
                    directionalLoc.x += 40.0f;
                    break;
                case 1:
                    directionalLoc.x -= 40.0f;
                    break;
                case 2:
                    directionalLoc.y += 40.0f;
                    break;
                case 3:
                    directionalLoc.y -= 40.0f;
                    break;
                }
                trace_t trace;
                Ray_t ray;
                ray.Init(vec, directionalLoc);
                {
                    g_ITrace->TraceRay(ray, 0x4200400B, &trace::filter_no_player, &trace);
                }
                // distance of trace < than 26
                if (trace.startpos.DistToSqr(trace.endpos) < Sqr(26.0f))
                    return false;
            }
        }
    }
    else
    {
        // check if the vector is too high above ground
        // higher to avoid small false positives, player can jump 42 hu
        // according to
        // the tf2 wiki
        if (DistanceToGround({ loc.x, loc.y, loc.z + 5.0f }) >= 40.0f)
            return false;

        // check if there is enough space around the vector for a player to fit
        // for loop for all 4 directions
        for (uint8 i = 0; i < 4; ++i)
        {
            Vector directionalLoc = loc;
            // what direction to check
            switch (i)
            {
            case 0:
                directionalLoc.x += 40.0f;
                break;
            case 1:
                directionalLoc.x -= 40.0f;
                break;
            case 2:
                directionalLoc.y += 40.0f;
                break;
            case 3:
                directionalLoc.y -= 40.0f;
                break;
            }
            trace_t trace;
            Ray_t ray;
            ray.Init(loc, directionalLoc);
            {
                g_ITrace->TraceRay(ray, 0x4200400B, &trace::filter_no_player, &trace);
            }
            // distance of trace < than 26
            if (trace.startpos.DistToSqr(trace.endpos) < Sqr(26.0f))
                return false;
        }
    }
    return true;
}

std::string GetLevelName()
{
    const std::string &name = g_IEngine->GetLevelName();
    const char *data        = name.data();
    const size_t length     = name.length();
    size_t slash            = 0;
    size_t bsp              = length;

    for (size_t i = length - 1; i != std::string::npos; --i)
    {
        if (data[i] == '/')
        {
            slash = i + 1;
            break;
        }
        if (data[i] == '.')
            bsp = i;
    }

    return { data + slash, bsp - slash };
}

std::pair<float, float> ComputeMovePrecise(const Vector &a, const Vector &b)
{
    Vector diff = b - a;
    if (diff.IsZero())
        return { 0.0f, 0.0f };
    const float x = diff.x;
    const float y = diff.y;
    Vector vsilent(x, y, 0.0f);
    Vector ang;
    VectorAngles(vsilent, ang);
    float yaw = DEG2RAD(ang.y - current_user_cmd->viewangles.y);
    if (g_pLocalPlayer->bUseSilentAngles)
        yaw = DEG2RAD(ang.y - g_pLocalPlayer->v_OrigViewangles.y);
    float speed = MAX(MIN(diff.Length() * 33.0f, 450.0f), 1.0001f);
    return { cos(yaw) * speed, -sin(yaw) * speed };
}

Vector ComputeMove(const Vector &a, const Vector &b)
{
    Vector diff = b - a;
    if (diff.IsZero())
        return Vector(0.0f);
    const float x = diff.x;
    const float y = diff.y;
    Vector vsilent(x, y, 0.0f);
    Vector ang;
    VectorAngles(vsilent, ang);
    float yaw   = DEG2RAD(ang.y - current_user_cmd->viewangles.y);
    float pitch = DEG2RAD(ang.x - current_user_cmd->viewangles.x);
    if (g_pLocalPlayer->bUseSilentAngles)
    {
        yaw   = DEG2RAD(ang.y - g_pLocalPlayer->v_OrigViewangles.y);
        pitch = DEG2RAD(ang.x - g_pLocalPlayer->v_OrigViewangles.x);
    }
    Vector move = { cos(yaw) * 450.0f, -sin(yaw) * 450.0f, -cos(pitch) * 450.0f };

    // Only apply upmove in water
    if (!(g_ITrace->GetPointContents(g_pLocalPlayer->v_Eye) & CONTENTS_WATER))
        move.z = current_user_cmd->upmove;
    return move;
}

ConCommand *CreateConCommand(const char *name, FnCommandCallback_t callback, const char *help)
{
    auto ret = new ConCommand(name, callback, help);
    g_ICvar->RegisterConCommand(ret);
    RegisteredCommandsList().push_back(ret);
    return ret;
}

const char *GetBuildingName(CachedEntity *ent)
{
    if (!ent)
        return "[NULL]";
    int classid = ent->m_iClassID();
    if (classid == CL_CLASS(CObjectSentrygun))
        return "Sentry";
    if (classid == CL_CLASS(CObjectDispenser))
        return "Dispenser";
    if (classid == CL_CLASS(CObjectTeleporter))
        return "Teleporter";
    return "[NULL]";
}

void format_internal(std::stringstream &stream)
{
    (void) stream;
}

void ReplaceString(std::string &input, const std::string &what, const std::string &with_what)
{
    size_t index;
    index = input.find(what);
    while (index != std::string::npos)
    {
        input.replace(index, what.size(), with_what);
        index = input.find(what, index + with_what.size());
    }
}

void ReplaceSpecials(std::string &str)
{
    int val;
    size_t c = 0, len = str.size();
    for (int i = 0; i + c < len; ++i)
    {
        str[i] = str[i + c];
        if (str[i] != '\\')
            continue;
        if (i + c + 1 == len)
            break;
        switch (str[i + c + 1])
        {
        // Several control characters
        case 'b':
            ++c;
            str[i] = '\b';
            break;
        case 'n':
            ++c;
            str[i] = '\n';
            break;
        case 'v':
            ++c;
            str[i] = '\v';
            break;
        case 'r':
            ++c;
            str[i] = '\r';
            break;
        case 't':
            ++c;
            str[i] = '\t';
            break;
        case 'f':
            ++c;
            str[i] = '\f';
            break;
        case 'a':
            ++c;
            str[i] = '\a';
            break;
        case 'e':
            ++c;
            str[i] = '\e';
            break;
        // Write escaped escape character as is
        case '\\':
            ++c;
            break;
        // Convert specified value from HEX
        case 'x':
            if (i + c + 4 > len)
                continue;
            std::sscanf(&str[i + c + 2], "%02X", &val);
            c += 3;
            str[i] = val;
            break;
        // Convert from unicode
        case 'u':
            if (i + c + 6 > len)
                continue;
            // 1. Scan 16bit HEX value
            std::sscanf(&str[i + c + 2], "%04X", &val);
            c += 5;
            // 2. Convert value to UTF-8
            if (val <= 0x7F)
                str[i] = val;
            else if (val <= 0x7FF)
            {
                str[i]     = 0xC0 | ((val >> 6) & 0x1F);
                str[i + 1] = 0x80 | (val & 0x3F);
                ++i;
                --c;
            }
            else
            {
                str[i]     = 0xE0 | ((val >> 12) & 0xF);
                str[i + 1] = 0x80 | ((val >> 6) & 0x3F);
                str[i + 2] = 0x80 | (val & 0x3F);
                i += 2;
                c -= 2;
            }
            break;
        }
    }
    str.resize(len - c);
}

powerup_type GetPowerupOnPlayer(CachedEntity *player)
{
    if (CE_BAD(player))
        return powerup_type::not_powerup;
    if (HasCondition<TFCond_RuneStrength>(player))
        return powerup_type::strength;
    if (HasCondition<TFCond_RuneHaste>(player))
        return powerup_type::haste;
    if (HasCondition<TFCond_RuneRegen>(player))
        return powerup_type::regeneration;
    if (HasCondition<TFCond_RuneResist>(player))
        return powerup_type::resistance;
    if (HasCondition<TFCond_RuneVampire>(player))
        return powerup_type::vampire;
    if (HasCondition<TFCond_RuneWarlock>(player))
        return powerup_type::reflect;
    if (HasCondition<TFCond_RunePrecision>(player))
        return powerup_type::precision;
    if (HasCondition<TFCond_RuneAgility>(player))
        return powerup_type::agility;
    if (HasCondition<TFCond_RuneKnockout>(player))
        return powerup_type::knockout;
    if (HasCondition<TFCond_KingRune>(player))
        return powerup_type::king;
    if (HasCondition<TFCond_PlagueRune>(player))
        return powerup_type::plague;
    if (HasCondition<TFCond_SupernovaRune>(player))
        return powerup_type::supernova;
    return powerup_type::not_powerup;
}

bool DidProjectileHit(Vector start_point, Vector end_point, CachedEntity *entity, float projectile_size, bool grav_comp, trace_t *tracer)
{
    trace::filter_default.SetSelf(RAW_ENT(LOCAL_E));
    Ray_t ray;
    trace_t *trace_obj;
    if (tracer)
        trace_obj = tracer;
    else
        trace_obj = new trace_t;
    ray.Init(start_point, end_point, Vector(0, -projectile_size, -projectile_size), Vector(0, projectile_size, projectile_size));
    g_ITrace->TraceRay(ray, MASK_SHOT_HULL, &trace::filter_default, trace_obj);
    return (IClientEntity *) trace_obj->m_pEnt == RAW_ENT(entity) || grav_comp && !trace_obj->DidHit();
}

// A function to find a weapon by WeaponID
int getWeaponByID(CachedEntity *player, int weaponid)
{
    // Invalid player
    if (CE_BAD(player))
        return -1;
    int *hWeapons = &CE_INT(player, netvar.hMyWeapons);
    // Go through the handle array and search for the item
    for (int i = 0; hWeapons[i]; ++i)
    {
        if (IDX_BAD(HandleToIDX(hWeapons[i])))
            continue;
        // Get the weapon
        CachedEntity *weapon = ENTITY(HandleToIDX(hWeapons[i]));
        // if weapon is what we are looking for, return true
        if (CE_VALID(weapon) && re::C_TFWeaponBase::GetWeaponID(RAW_ENT(weapon)) == weaponid)
            return weapon->m_IDX;
    }
    // Nothing found
    return -1;
}

// A function to tell if a player is using a specific weapon
bool HasWeapon(CachedEntity *ent, int wantedId)
{
    if (CE_BAD(ent) || ent->m_Type() != ENTITY_PLAYER)
        return false;
    // Grab the handle and store it into the var
    int *hWeapons = &CE_INT(ent, netvar.hMyWeapons);
    if (!hWeapons)
        return false;
    // Go through the handle array and search for the item
    for (int i = 0; hWeapons[i]; ++i)
    {
        if (IDX_BAD(HandleToIDX(hWeapons[i])))
            continue;
        // Get the weapon
        CachedEntity *weapon = ENTITY(HandleToIDX(hWeapons[i]));
        // if weapon is what we are looking for, return true
        if (CE_VALID(weapon) && CE_INT(weapon, netvar.iItemDefinitionIndex) == wantedId)
            return true;
    }
    // We didn't find the weapon we needed, return false
    return false;
}

CachedEntity *getClosestEntity(Vector vec)
{
    float distance         = FLT_MAX;
    CachedEntity *best_ent = nullptr;
    for (const auto &ent : entity_cache::player_cache)
    {
         auto vecEntityOrigin = ent->m_vecDormantOrigin();

        if (!vecEntityOrigin || !ent->m_bEnemy())
        {
            continue;
        }

        const auto dist_sq = vec.DistToSqr(*vecEntityOrigin);
        if (dist_sq < distance)
        {
            distance = dist_sq;
            best_ent = ent;
        }
    }

    return best_ent;
}

CachedEntity *getClosestNonlocalEntity(Vector vec)
{
    float distance         = FLT_MAX;
    CachedEntity *best_ent = nullptr;
    for (const auto &ent : entity_cache::player_cache)
    {
        auto vecEntityOrigin = ent->m_vecDormantOrigin();

        if (!vecEntityOrigin || !ent->m_bEnemy() || ent == LOCAL_E)
        {
            continue;
        }

        const auto dist_sq = vec.DistToSqr(*vecEntityOrigin);
        if (dist_sq < distance)
        {
            distance = dist_sq;
            best_ent = ent;
        }
    }
    return best_ent;
}

void VectorTransform(const float *in1, const matrix3x4_t &in2, float *out)
{
    out[0] = in1[0] * in2[0][0] + in1[1] * in2[0][1] + in1[2] * in2[0][2] + in2[0][3];
    out[1] = in1[0] * in2[1][0] + in1[1] * in2[1][1] + in1[2] * in2[1][2] + in2[1][3];
    out[2] = in1[0] * in2[2][0] + in1[1] * in2[2][1] + in1[2] * in2[2][2] + in2[2][3];
}

bool GetHitbox(CachedEntity *entity, int hb, Vector &out)
{
    hitbox_cache::CachedHitbox *box;

    if (CE_BAD(entity))
        return false;
    box = entity->hitboxes.GetHitbox(hb);
    if (!box)
        out = entity->m_vecOrigin();
    else
        out = box->center;
    return true;
}

void MatrixGetColumn(const matrix3x4_t &in, int column, Vector &out)
{
    out.x = in[0][column];
    out.y = in[1][column];
    out.z = in[2][column];
}

void MatrixAngles(const matrix3x4_t &matrix, float *angles)
{
    float forward[3];
    float left[3];
    float up[3];

    //
    // Extract the basis vectors from the matrix. Since we only need the Z
    // component of the up vector, we don't get X and Y.
    //
    forward[0] = matrix[0][0];
    forward[1] = matrix[1][0];
    forward[2] = matrix[2][0];
    left[0]    = matrix[0][1];
    left[1]    = matrix[1][1];
    left[2]    = matrix[2][1];
    up[2]      = matrix[2][2];

    float xyDist = std::hypot(forward[0], forward[1]);

    // enough here to get angles?
    if (xyDist > 0.001f)
    {
        // (yaw)    y = ATAN( forward.y, forward.x );       -- in our space, forward is the X axis
        angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

        // (pitch)  x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
        angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

        // (roll)   z = ATAN( left.z, up.z );
        angles[2] = RAD2DEG(atan2f(left[2], up[2]));
    }
    else
    {
        // (yaw)    y = ATAN( -left.x, left.y );            -- forward is mostly z, so use right for yaw
        angles[1] = RAD2DEG(atan2f(-left[0], left[1]));

        // (pitch)  x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
        angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

        // Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
        angles[2] = 0;
    }
}

void VectorAngles(Vector &forward, Vector &angles)
{
    if (forward[1] == 0.0f && forward[0] == 0.0f)
    {
        angles[0] = forward[2] >= 0.0f ? 270.0f : 90.0f;
        angles[1] = 0.0f;
    }
    else
    {
        angles[1] = std::remainder(RAD2DEG(std::atan2(forward[1], forward[0])), 360.0f);
        angles[0] = std::remainder(RAD2DEG(std::atan2(-forward[2], std::hypot(forward[0], forward[1]))), 360.0f);
    }
    angles[2] = 0.0f;
}

// Forward, right, and up
void AngleVectors3(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
    float sr, sp, sy, cr, cp, cy;
    SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
    SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
    SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

    if (forward)
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right)
    {
        right->x = sr * sp * cy - cr * sy;
        right->y = sr * sp * sy + cr * cy;
        right->z = sr * cp;
    }

    if (up)
    {
        up->x = cr * sp * cy + sr * sy;
        up->y = cr * sp * sy - sr * cy;
        up->z = cr * cp;
    }
}

bool isRapidFire(IClientEntity *wep)
{
    weapon_info info(wep);
    bool ret = GetWeaponData(wep)->m_bUseRapidFireCrits;
    // Minigun changes mode once revved, so fix that
    return ret || wep->GetClientClass()->m_ClassID == CL_CLASS(CTFMinigun);
}

char GetUpperChar(ButtonCode_t button)
{
    switch (button)
    {
    case KEY_0:
        return ')';
    case KEY_1:
        return '!';
    case KEY_2:
        return '@';
    case KEY_3:
        return '#';
    case KEY_4:
        return '$';
    case KEY_5:
        return '%';
    case KEY_6:
        return '^';
    case KEY_7:
        return '&';
    case KEY_8:
        return '*';
    case KEY_9:
        return '(';
    case KEY_LBRACKET:
        return '{';
    case KEY_RBRACKET:
        return '}';
    case KEY_SEMICOLON:
        return ':';
    case KEY_BACKQUOTE:
        return '~';
    case KEY_APOSTROPHE:
        return '"';
    case KEY_COMMA:
        return '<';
    case KEY_PERIOD:
        return '>';
    case KEY_SLASH:
        return '?';
    case KEY_BACKSLASH:
        return '|';
    case KEY_MINUS:
        return '_';
    case KEY_EQUAL:
        return '+';
    default:
        if (strlen(g_IInputSystem->ButtonCodeToString(button)) != 1)
            return 0;
        return toupper(*g_IInputSystem->ButtonCodeToString(button));
    }
}

char GetChar(ButtonCode_t button)
{
    switch (button)
    {
    case KEY_PAD_DIVIDE:
        return '/';
    case KEY_PAD_MULTIPLY:
        return '*';
    case KEY_PAD_MINUS:
        return '-';
    case KEY_PAD_PLUS:
        return '+';
    case KEY_SEMICOLON:
        return ';';
    default:
        if (button >= KEY_PAD_0 && button <= KEY_PAD_9)
            return button - KEY_PAD_0 + '0';
        if (strlen(g_IInputSystem->ButtonCodeToString(button)) != 1)
            return 0;
        return *g_IInputSystem->ButtonCodeToString(button);
    }
}

void FixMovement(CUserCmd &cmd, Vector &viewangles)
{
    Vector movement, ang;
    float speed, yaw;
    movement.x = cmd.forwardmove;
    movement.y = cmd.sidemove;
    movement.z = cmd.upmove;
    speed      = std::hypot(movement.x, movement.y);
    VectorAngles(movement, ang);
    yaw             = DEG2RAD(ang.y - viewangles.y + cmd.viewangles.y);
    cmd.forwardmove = cos(yaw) * speed;
    cmd.sidemove    = sin(yaw) * speed;
}

bool AmbassadorCanHeadshot()
{
    return CE_FLOAT(LOCAL_W, netvar.flLastFireTime) - SERVER_TIME <= 1.0f;
}

static std::random_device random_device;
static std::mt19937 engine{ random_device() };

float RandFloatRange(float min, float max)
{
    std::uniform_real_distribution<float> random(min, max);
    return random(engine);
}

int UniformRandomInt(int min, int max)
{
    std::uniform_int_distribution<int> random(min, max);
    return random(engine);
}

bool IsEntityVisible(CachedEntity *entity, int hb)
{
    if (g_Settings.bInvalid)
        return false;
    if (entity == LOCAL_E)
        return true;
    if (hb == -1)
        return IsEntityVectorVisible(entity, entity->m_vecOrigin());
    else
        return entity->hitboxes.VisibilityCheck(hb);
}

bool IsEntityVectorVisible(CachedEntity *entity, Vector endpos, bool use_weapon_offset, unsigned int mask, trace_t *trace, bool hit)
{
    trace_t trace_object;
    if (!trace)
        trace = &trace_object;
    Ray_t ray;

    if (entity == LOCAL_E)
        return true;
    trace::filter_default.SetSelf(RAW_ENT(LOCAL_E));
    Vector eye = g_pLocalPlayer->v_Eye;
    // Adjust for weapon offsets if needed
    if (use_weapon_offset)
        eye = getShootPos(GetAimAtAngles(eye, endpos, LOCAL_E));
    ray.Init(eye, endpos);
    {
        if (!*tcm || g_Settings.is_create_move)
            g_ITrace->TraceRay(ray, mask, &trace::filter_default, trace);
    }
    return (IClientEntity *) trace->m_pEnt == RAW_ENT(entity) || !hit && !trace->DidHit();
}

// Get all the corners of a box. Taken from sauce engine.
void GenerateBoxVertices(const Vector &vOrigin, const QAngle &angles, const Vector &vMins, const Vector &vMaxs, Vector pVerts[8])
{
    // Build a rotation matrix from orientation
    matrix3x4_t fRotateMatrix;
    AngleMatrix(angles, fRotateMatrix);

    Vector vecPos;
    for (uint8 i = 0; i < 8; ++i)
    {
        vecPos[0] = i & 0x1 ? vMaxs[0] : vMins[0];
        vecPos[1] = i & 0x2 ? vMaxs[1] : vMins[1];
        vecPos[2] = i & 0x4 ? vMaxs[2] : vMins[2];

        VectorRotate(vecPos, fRotateMatrix, pVerts[i]);
        pVerts[i] += vOrigin;
    }
}

// For when you need to vis check something that isn't the local player
bool VisCheckEntFromEnt(CachedEntity *startEnt, CachedEntity *endEnt)
{
    // We setSelf as the starting ent as we don't want to hit it, we want the other ent
    trace_t trace;
    trace::filter_default.SetSelf(RAW_ENT(startEnt));

    // Set up the trace starting with the origin of the starting ent attempting to hit the origin of the end ent
    Ray_t ray;
    ray.Init(startEnt->m_vecOrigin(), endEnt->m_vecOrigin());
    {
        g_ITrace->TraceRay(ray, MASK_SHOT_HULL, &trace::filter_default, &trace);
    }
    // Is the entity that we hit our target ent? if so, the vis check passes
    if (trace.m_pEnt && (IClientEntity *) trace.m_pEnt == RAW_ENT(endEnt))
        return true;

    // Since we didn't hit our target ent, the vis check failed so return false
    return false;
}

// Use when you need to vis check something, but it's not the ent origin that you
// use, so we check from the vector to the ent, ignoring the first just in case
bool VisCheckEntFromEntVector(Vector startVector, CachedEntity *startEnt, CachedEntity *endEnt)
{
    // We setSelf as the starting ent as we don't want to hit it, we want the other ent
    trace_t trace;
    trace::filter_default.SetSelf(RAW_ENT(startEnt));

    // Set up the trace starting with the origin of the starting ent attempting to hit the origin of the end ent
    Ray_t ray;
    ray.Init(startVector, endEnt->m_vecOrigin());
    {
        g_ITrace->TraceRay(ray, MASK_SHOT_HULL, &trace::filter_default, &trace);
    }
    // Is the entity that we hit our target ent? if so, the vis check passes
    if (trace.m_pEnt && (IClientEntity *) trace.m_pEnt == RAW_ENT(endEnt))
        return true;

    // Since we didn't hit our target ent, the vis check failed so return false
    return false;
}

Vector GetBuildingPosition(CachedEntity *ent)
{
    // Get a centered hitbox
    auto hitbox = ent->hitboxes.GetHitbox(std::max(0, ent->hitboxes.GetNumHitboxes() / 2 - 1));
    // Dormant/Invalid, return origin
    if (!hitbox)
        return ent->m_vecOrigin();
    return hitbox->center;
}

bool IsBuildingVisible(CachedEntity *ent)
{
    return IsEntityVectorVisible(ent, GetBuildingPosition(ent));
}

void fClampAngle(Vector &qaAng)
{
    qaAng.x = fmod(qaAng[0] + 89.0f, 180.0f) - 89.0f;
    qaAng.y = fmod(qaAng[1] + 180.0f, 360.0f) - 180.0f;
    qaAng.z = 0.0f;
}

bool IsProjectileCrit(CachedEntity *ent)
{
    if (ent->m_bGrenadeProjectile())
        return CE_BYTE(ent, netvar.Grenade_bCritical);
    return CE_BYTE(ent, netvar.Rocket_bCritical);
}

CachedEntity *weapon_get(CachedEntity *entity)
{
    int handle, eid;

    if (CE_BAD(entity))
        return nullptr;
    handle = CE_INT(entity, netvar.hActiveWeapon);
    eid    = HandleToIDX(handle);
    if (IDX_BAD(eid))
        return nullptr;
    return ENTITY(eid);
}

float ProjGravMult(int class_id, float x_speed)
{
    switch (class_id)
    {
    case CL_CLASS(CTFGrenadePipebombProjectile):
    case CL_CLASS(CTFProjectile_Cleaver):
    case CL_CLASS(CTFProjectile_Jar):
    case CL_CLASS(CTFProjectile_JarMilk):
        return 1.0f;
    case CL_CLASS(CTFProjectile_Arrow):
        if (2599.0f <= x_speed)
            return 0.1f;
        else
            return 0.5f;
    case CL_CLASS(CTFProjectile_Flare):
        return 0.25f;
    case CL_CLASS(CTFProjectile_HealingBolt):
        return 0.2f;
    case CL_CLASS(CTFProjectile_Rocket):
    case CL_CLASS(CTFProjectile_SentryRocket):
    case CL_CLASS(CTFProjectile_EnergyBall):
    case CL_CLASS(CTFProjectile_EnergyRing):
    case CL_CLASS(CTFProjectile_GrapplingHook):
    case CL_CLASS(CTFProjectile_BallOfFire):
        return 0.0f;
    default:
        return 0.3f;
    }
}

weaponmode GetWeaponMode(CachedEntity *ent)
{
    int weapon_handle, weapon_idx, slot;
    CachedEntity *weapon;

    if (CE_BAD(ent) || CE_BAD(weapon_get(ent)))
        return weapon_invalid;
    weapon_handle = CE_INT(ent, netvar.hActiveWeapon);
    weapon_idx    = HandleToIDX(weapon_handle);

    if (IDX_BAD(weapon_idx))
    {
        // logging::Info("IDX_BAD: %i", weapon_idx);
        return weaponmode::weapon_invalid;
    }
    weapon = ENTITY(weapon_idx);
    if (CE_BAD(weapon))
        return weaponmode::weapon_invalid;
    int classid = weapon->m_iClassID();
    slot        = re::C_BaseCombatWeapon::GetSlot(RAW_ENT(weapon));
    if (slot == 2)
        return weaponmode::weapon_melee;
    if (slot > 2)
        return weaponmode::weapon_pda;
    switch (classid)
    {
    case CL_CLASS(CTFLunchBox):
    case CL_CLASS(CTFLunchBox_Drink):
    case CL_CLASS(CTFBuffItem):
        return weaponmode::weapon_consumable;
    case CL_CLASS(CTFRocketLauncher_DirectHit):
    case CL_CLASS(CTFRocketLauncher):
    case CL_CLASS(CTFGrenadeLauncher):
    case CL_CLASS(CTFPipebombLauncher):
    case CL_CLASS(CTFCompoundBow):
    case CL_CLASS(CTFFlameThrower):
    case CL_CLASS(CTFBat_Wood):
    case CL_CLASS(CTFBat_Giftwrap):
    case CL_CLASS(CTFFlareGun):
    case CL_CLASS(CTFFlareGun_Revenge):
    case CL_CLASS(CTFSyringeGun):
    case CL_CLASS(CTFCrossbow):
    case CL_CLASS(CTFShotgunBuildingRescue):
    case CL_CLASS(CTFDRGPomson):
    case CL_CLASS(CTFWeaponFlameBall):
    case CL_CLASS(CTFRaygun):
    case CL_CLASS(CTFGrapplingHook):
    case CL_CLASS(CTFParticleCannon): // Cow Mangler 5000
    case CL_CLASS(CTFRocketLauncher_AirStrike):
    case CL_CLASS(CTFCannon):
    case CL_CLASS(CTFMechanicalArm):
        return weaponmode::weapon_projectile;
    case CL_CLASS(CTFJar):
    case CL_CLASS(CTFJarMilk):
    case CL_CLASS(CTFJarGas):
    case CL_CLASS(CTFCleaver):
        return weaponmode::weapon_throwable;
    case CL_CLASS(CWeaponMedigun):
        return weaponmode::weapon_medigun;
    case CL_CLASS(CTFWeaponSapper):
    case CL_CLASS(CTFWeaponPDA):
    case CL_CLASS(CTFWeaponPDA_Spy):
    case CL_CLASS(CTFWeaponPDA_Engineer_Build):
    case CL_CLASS(CTFWeaponPDA_Engineer_Destroy):
    case CL_CLASS(CTFWeaponBuilder):
    case CL_CLASS(CTFLaserPointer):
        return weaponmode::weapon_pda;
    default:
        return weaponmode::weapon_hitscan;
    }
}

bool LineIntersectsBox(Vector &bmin, Vector &bmax, Vector &lmin, Vector &lmax)
{
    if (lmax.x < bmin.x && lmin.x < bmin.x)
        return false;
    if (lmax.y < bmin.y && lmin.y < bmin.y)
        return false;
    if (lmax.z < bmin.z && lmin.z < bmin.z)
        return false;
    if (lmax.x > bmax.x && lmin.x > bmax.x)
        return false;
    if (lmax.y > bmax.y && lmin.y > bmax.y)
        return false;
    if (lmax.z > bmax.z && lmin.z > bmax.z)
        return false;
    return true;
}

bool GetProjectileData(CachedEntity *weapon, float &speed, float &gravity, float &start_velocity)
{
    float rspeed, rgrav, rinitial_vel;

    if (CE_BAD(weapon))
        return false;
    rspeed       = 0.0f;
    rgrav        = 0.0f;
    rinitial_vel = 0.0f;

    switch (weapon->m_iClassID())
    {
    case CL_CLASS(CTFRocketLauncher_DirectHit):
        rspeed = 1980.0f;
        break;
    case CL_CLASS(CTFParticleCannon): // Cow Mangler 5000
    case CL_CLASS(CTFRocketLauncher_AirStrike):
    case CL_CLASS(CTFRocketLauncher):
        rspeed = 1100.0f;
        // Libery Launcher
        if (CE_INT(weapon, netvar.iItemDefinitionIndex) == 414)
            rspeed = 1540.0f;
        break;
    case CL_CLASS(CTFCannon):
        rspeed       = 1453.9f;
        rgrav        = 1.0f;
        rinitial_vel = 200.0f;
        break;
    case CL_CLASS(CTFGrenadeLauncher):
        rspeed       = 1217.0f;
        rgrav        = 1.0f;
        rinitial_vel = 200.0f;
        // Loch'n Load
        if (CE_INT(weapon, netvar.iItemDefinitionIndex) == 308)
            rspeed = 1513.3f;
        break;
    case CL_CLASS(CTFPipebombLauncher):
    {
        float chargetime = SERVER_TIME - CE_FLOAT(weapon, netvar.flChargeBeginTime);
        if (!CE_FLOAT(weapon, netvar.flChargeBeginTime))
            chargetime = 0.0f;
        rspeed       = RemapValClamped(chargetime, 0.0f, 4.0f, 925.38, 2409.2);
        rgrav        = 1.0f;
        rinitial_vel = 200.0f;
        if (CE_INT(weapon, netvar.iItemDefinitionIndex) == 1150) // Quickiebomb Launcher
            rspeed = RemapValClamped(chargetime, 0.0f, 4.0f, 930.88, 2409.2);
        break;
    }
    case CL_CLASS(CTFCompoundBow):
    {
        float chargetime = SERVER_TIME - CE_FLOAT(weapon, netvar.flChargeBeginTime);
        if (CE_FLOAT(weapon, netvar.flChargeBeginTime) == 0)
            chargetime = 0;
        else
        {
            static const ConVar *pUpdateRate = g_pCVar->FindVar("cl_updaterate");
            if (!pUpdateRate)
                pUpdateRate = g_pCVar->FindVar("cl_updaterate");
            else
                chargetime += TICKS_TO_TIME(1);
        }
        rspeed = RemapValClamped(chargetime, 0.0f, 1.0f, 1800.0f, 2600.0f);
        rgrav  = RemapValClamped(chargetime, 0.0f, 1.0f, 0.5f, 0.1f);
        break;
    }
    case CL_CLASS(CTFBat_Giftwrap):
    case CL_CLASS(CTFBat_Wood):
        rspeed = 3000.0f;
        rgrav  = 0.5f;
        break;
    case CL_CLASS(CTFFlareGun):
    case CL_CLASS(CTFFlareGun_Revenge): // Detonator
        rspeed = 2000.0f;
        rgrav  = 0.3f;
        break;
    case CL_CLASS(CTFSyringeGun):
        rspeed = 1000.0f;
        rgrav  = 0.3f;
        break;
    case CL_CLASS(CTFCrossbow):
    case CL_CLASS(CTFShotgunBuildingRescue):
        rspeed = 2400.0f;
        rgrav  = 0.2f;
        break;
    case CL_CLASS(CTFDRGPomson):
    case CL_CLASS(CTFRaygun): // Righteous Bison
        rspeed = 1200.0f;
        break;
    case CL_CLASS(CTFWeaponFlameBall):
    case CL_CLASS(CTFCleaver):
        rspeed = 3000.0f;
        break;
    case CL_CLASS(CTFFlameThrower):
        rspeed = 1000.0f;
        break;
    case CL_CLASS(CTFGrapplingHook):
        rspeed = 1500.0f;
        break;
    case CL_CLASS(CTFJarMilk):
        rspeed = 1019.9f;
        break;
    case CL_CLASS(CTFJar):
        rspeed = 1017.9f;
        break;
    case CL_CLASS(CTFJarGas):
        rspeed = 2009.2f;
        break;
    }

    speed          = rspeed;
    gravity        = rgrav;
    start_velocity = rinitial_vel;
    return rspeed || rgrav || rinitial_vel;
}

bool IsVectorVisible(Vector origin, Vector target, bool enviroment_only, CachedEntity *self, unsigned int mask)
{
    if (!enviroment_only)
    {
        trace_t trace_visible;
        Ray_t ray;

        trace::filter_no_player.SetSelf(RAW_ENT(self));
        ray.Init(origin, target);
        g_ITrace->TraceRay(ray, mask, &trace::filter_no_player, &trace_visible);
        return trace_visible.fraction == 1.0f;
    }
    else
    {
        trace_t trace_visible;
        Ray_t ray;

        trace::filter_no_entity.SetSelf(RAW_ENT(self));
        ray.Init(origin, target);
        g_ITrace->TraceRay(ray, mask, &trace::filter_no_entity, &trace_visible);
        return trace_visible.fraction == 1.0f;
    }
}

bool IsVectorVisibleNavigation(Vector origin, Vector target, unsigned int mask)
{
    trace_t trace_visible;
    Ray_t ray;

    ray.Init(origin, target);
    g_ITrace->TraceRay(ray, mask, &trace::filter_navigation, &trace_visible);
    return trace_visible.fraction == 1.0f;
}

void WhatIAmLookingAt(int *result_eindex, Vector *result_pos)
{
    static QAngle prev_angle   = QAngle(0.0f, 0.0f, 0.0f);
    static Vector prev_forward = Vector(0.0f, 0.0f, 0.0f);

    // Check if the player's view direction has changed since the last call to this function.
    QAngle angle;
    g_IEngine->GetViewAngles(angle);
    bool angle_changed = angle != prev_angle;
    prev_angle         = angle;

    // Compute the forward vector if the angle has changed or if it has not been computed before.
    static auto forward = Vector(0.0f);
    if (angle_changed || prev_forward == Vector(0.0f))
    {
        float sp, sy, cp, cy;
        sincosf(DEG2RAD(angle[0]), &sp, &cp);
        sincosf(DEG2RAD(angle[1]), &sy, &cy);
        forward.x    = cp * cy;
        forward.y    = cp * sy;
        forward.z    = -sp;
        prev_forward = forward;
    }

    // Perform the raycast if the angle has changed or if the forward vector has not been computed before.
    static trace_t trace;
    if (angle_changed || prev_forward == Vector(0.0f))
    {
        Vector endpos = g_pLocalPlayer->v_Eye + forward * 8192.0f;
        Ray_t ray;
        ray.Init(g_pLocalPlayer->v_Eye, endpos);
        {
            g_ITrace->TraceRay(ray, 0x4200400B, &trace::filter_default, &trace);
        }
    }

    if (result_pos)
        *result_pos = trace.endpos;
    if (result_eindex)
        *result_eindex = trace.m_pEnt ? ((IClientEntity *) trace.m_pEnt)->entindex() : -1;
}

Vector GetForwardVector(Vector origin, Vector viewangles, float distance, CachedEntity *punch_entity)
{
    Vector forward;
    float sp, sy, cp, cy;
    QAngle angle = VectorToQAngle(viewangles);
    // Compensate for punch angle
    if (*should_correct_punch && punch_entity)
        angle -= VectorToQAngle(CE_VECTOR(punch_entity, netvar.vecPunchAngle));

    sincosf(DEG2RAD(angle[1]), &sy, &cy);
    sincosf(DEG2RAD(angle[0]), &sp, &cp);
    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
    forward   = forward * distance + origin;
    return forward;
}

Vector GetForwardVector(float distance, CachedEntity *punch_entity)
{
    return GetForwardVector(g_pLocalPlayer->v_Eye, g_pLocalPlayer->v_OrigViewangles, distance, punch_entity);
}

// TODO: Change it to be based on the model
bool IsSentryBuster(CachedEntity *entity)
{
    return entity->m_Type() == EntityType::ENTITY_PLAYER && CE_INT(entity, netvar.iClass) == tf_class::tf_demoman && g_pPlayerResource->GetMaxHealth(entity) == 2500;
}

bool IsAmbassador(CachedEntity *entity)
{
    if (entity->m_iClassID() != CL_CLASS(CTFRevolver))
        return false;
    const int &defidx = CE_INT(entity, netvar.iItemDefinitionIndex);
    return defidx == 61 || defidx == 1006;
}

bool IsPlayerInvulnerable(CachedEntity *player)
{
    return HasConditionMask<KInvulnerabilityMask.cond_0, KInvulnerabilityMask.cond_1, KInvulnerabilityMask.cond_2, KInvulnerabilityMask.cond_3>(player);
}

bool IsPlayerCritBoosted(CachedEntity *player)
{
    return HasConditionMask<KCritBoostMask.cond_0, KCritBoostMask.cond_1, KCritBoostMask.cond_2, KCritBoostMask.cond_3>(player);
}

bool IsPlayerMiniCritBoosted(CachedEntity *player)
{
    return HasConditionMask<KMiniCritBoostMask.cond_0, KMiniCritBoostMask.cond_1, KMiniCritBoostMask.cond_2, KMiniCritBoostMask.cond_3>(player);
}

bool IsPlayerInvisible(CachedEntity *player)
{
    return HasConditionMask<KInvisibilityMask.cond_0, KInvisibilityMask.cond_1, KInvisibilityMask.cond_2, KInvisibilityMask.cond_3>(player);
}

bool IsPlayerDisguised(CachedEntity *player)
{
    return HasConditionMask<KDisguisedMask.cond_0, KDisguisedMask.cond_1, KDisguisedMask.cond_2, KDisguisedMask.cond_3>(player);
}

bool IsPlayerResistantToCurrentWeapon(CachedEntity *player)
{
    switch (LOCAL_W->m_iClassID())
    {
    case CL_CLASS(CTFRocketLauncher_DirectHit):
    case CL_CLASS(CTFRocketLauncher_AirStrike):
    case CL_CLASS(CTFRocketLauncher_Mortar): // doesn't exist yet
    case CL_CLASS(CTFRocketLauncher):
    case CL_CLASS(CTFParticleCannon):
    case CL_CLASS(CTFGrenadeLauncher):
    case CL_CLASS(CTFPipebombLauncher):
        return HasCondition<TFCond_UberBlastResist>(player);
    case CL_CLASS(CTFCompoundBow):
    case CL_CLASS(CTFSyringeGun):
    case CL_CLASS(CTFCrossbow):
    case CL_CLASS(CTFShotgunBuildingRescue):
    case CL_CLASS(CTFDRGPomson):
    case CL_CLASS(CTFRaygun):
        return HasCondition<TFCond_UberBulletResist>(player);
    case CL_CLASS(CTFWeaponFlameBall):
    case CL_CLASS(CTFFlareGun):
    case CL_CLASS(CTFFlareGun_Revenge):
    case CL_CLASS(CTFFlameRocket):
    case CL_CLASS(CTFFlameThrower):
        return HasCondition<TFCond_UberFireResist>(player);
    default:
        return GetWeaponMode() == weaponmode::weapon_hitscan && HasCondition<TFCond_UberBulletResist>(player);
    }
}

Vector CalcAngle(const Vector &src, const Vector &dst)
{
    Vector delta  = src - dst;
    float hyp_sqr = delta.LengthSqr();
    Vector aim_angles;

    aim_angles.x = atanf(delta.z / FastSqrt(hyp_sqr)) * RADPI;
    aim_angles.y = atan2f(delta.y, delta.x) * RADPI;
    aim_angles.z = 0.0f;

    if (delta.x >= 0.0f)
        aim_angles.y += 180.0f;

    return aim_angles;
}

void MakeVector(const Vector &angle, Vector &vector)
{
    float pitch = DEG2RAD(angle.x);
    float yaw   = DEG2RAD(angle.y);
    float tmp   = cos(pitch);

    vector.x = tmp * cos(yaw);
    vector.y = sin(yaw) * tmp;
    vector.z = -sin(pitch);
}

float GetFov(const Vector &angle, const Vector &src, const Vector &dst)
{
    Vector ang, aim;
    ang = CalcAngle(src, dst);

    MakeVector(angle, aim);
    MakeVector(ang, ang);

    float mag_sqr = aim.LengthSqr();
    float u_dot_v = aim.Dot(ang);

    // Avoid out of domain error
    if (u_dot_v >= mag_sqr)
        return 0;

    return RAD2DEG(acos(u_dot_v / mag_sqr));
}

bool CanHeadshot()
{
    return g_pLocalPlayer->flZoomBegin > 0.0f && SERVER_TIME - g_pLocalPlayer->flZoomBegin > 0.2f;
}

bool CanShoot()
{
    // PrecalculateCanShoot() CreateMove.cpp
    return calculated_can_shoot;
}

float ATTRIB_HOOK_FLOAT(float base_value, const char *search_string, IClientEntity *ent, void *buffer, bool is_global_const_string)
{
    typedef float (*AttribHookFloat_t)(float, const char *, IClientEntity *, void *, bool);

    static uintptr_t AttribHookFloat = e8call_direct(CSignature::GetClientSignature("E8 ? ? ? ? 8B 03 89 1C 24 D9 5D ? FF 90 ? ? ? ? 89 C7 8B 06 89 34 24 FF 90 ? ? ? ? 89 FA C1 E2 08 09 C2 33 15 ? ? ? ? 39 93 ? ? ? ? 74 ? 89 93 ? ? ? ? 89 14 24 E8 ? ? ? ? C7 44 24 ? 0F 27 00 00 BE 01 00 00 00"));
    static auto AttribHookFloat_fn   = AttribHookFloat_t(AttribHookFloat);

    return AttribHookFloat_fn(base_value, search_string, ent, buffer, is_global_const_string);
}

void AimAt(Vector origin, Vector target, CUserCmd *cmd, bool compensate_punch)
{
    cmd->viewangles = GetAimAtAngles(origin, target, compensate_punch ? LOCAL_E : nullptr);
}

void AimAtHitbox(CachedEntity *ent, int hitbox, CUserCmd *cmd, bool compensate_punch)
{
    Vector r;
    r = ent->m_vecOrigin();
    GetHitbox(ent, hitbox, r);
    AimAt(g_pLocalPlayer->v_Eye, r, cmd, compensate_punch);
}

void FastStop()
{
    // Get velocity
    Vector vel;
    velocity::EstimateAbsVelocity(RAW_ENT(LOCAL_E), vel);

    static auto sv_friction  = g_ICvar->FindVar("sv_friction");
    static auto sv_stopspeed = g_ICvar->FindVar("sv_stopspeed");

    auto speed    = vel.Length2D();
    auto friction = sv_friction->GetFloat() * CE_FLOAT(LOCAL_E, 0x12b8);
    auto control  = speed < sv_stopspeed->GetFloat() ? sv_stopspeed->GetFloat() : speed;
    auto drop     = control * friction * g_GlobalVars->interval_per_tick;

    if (speed > drop - 1.0f)
    {
        Vector velocity = vel;
        Vector direction;
        VectorAngles(vel, direction);
        float speed = velocity.Length();

        direction.y = current_user_cmd->viewangles.y - direction.y;

        Vector forward;
        AngleVectors2(VectorToQAngle(direction), &forward);
        Vector negated_direction = forward * -speed;

        current_user_cmd->forwardmove = negated_direction.x;
        current_user_cmd->sidemove    = negated_direction.y;
    }
    else
        current_user_cmd->forwardmove = current_user_cmd->sidemove = 0.0f;
}

bool IsEntityVisiblePenetration(CachedEntity *entity, int hb)
{
    trace_t trace_visible;
    Ray_t ray;
    Vector hit;
    int ret;
    bool correct_entity;
    IClientEntity *ent;
    trace::filter_penetration.SetSelf(RAW_ENT(LOCAL_E));
    trace::filter_penetration.Reset();
    ret = GetHitbox(entity, hb, hit);
    if (ret)
        return false;

    ray.Init(g_pLocalPlayer->v_Origin + g_pLocalPlayer->v_ViewOffset, hit);
    {
        g_ITrace->TraceRay(ray, MASK_SHOT_HULL, &trace::filter_penetration, &trace_visible);
    }
    correct_entity = false;
    if (trace_visible.m_pEnt)
        correct_entity = ((IClientEntity *) trace_visible.m_pEnt) == RAW_ENT(entity);

    if (!correct_entity)
        return false;
    {
        g_ITrace->TraceRay(ray, 0x4200400B, &trace::filter_default, &trace_visible);
    }
    if (trace_visible.m_pEnt)
    {
        ent = (IClientEntity *) trace_visible.m_pEnt;
        if (ent)
        {
            if (ent->GetClientClass()->m_ClassID == RCC_PLAYER)
            {
                if (ent == RAW_ENT(entity))
                    return false;
                if (trace_visible.hitbox >= 0)
                    return true;
            }
        }
    }
    return false;
}

void ValidateUserCmd(CUserCmd *cmd, int sequence_nr)
{
    CRC32_t crc = GetChecksum(cmd);
    if (crc != GetVerifiedCmds(g_IInput)[sequence_nr % VERIFIED_CMD_SIZE].m_crc)
        *cmd = GetVerifiedCmds(g_IInput)[sequence_nr % VERIFIED_CMD_SIZE].m_cmd;
}

// Used for getting class names
CatCommand print_classnames("debug_print_classnames", "Lists classnames currently available in console",
                            []()
                            {
                                // Go through all the entities
                                for (const auto &ent : entity_cache::valid_ents)
                                {
                                    // Print in console, the class name of the ent
                                    logging::Info(format(RAW_ENT(ent)->GetClientClass()->m_pNetworkName).c_str());
                                }
                            });

void PrintChat(const char *fmt, ...)
{
#if ENABLE_VISUALS
    auto *chat = (CHudBaseChat *) g_CHUD->FindElement("CHudChat");
    if (chat)
    {
        std::unique_ptr<char[]> buf(new char[1024]);
        va_list list;
        va_start(list, fmt);
        vsprintf(buf.get(), fmt, list);
        va_end(list);
        std::unique_ptr<char[]> str = std::move(strfmt("\x07%06X[ROSNEHOOK]\x01 %s", 0x1434a4, buf.get()));
        // FIXME DEBUG LOG
        logging::Info("%s", str.get());
        chat->Printf(str.get());
    }
#endif
}

// Get the point Your shots originate from
Vector getShootPos(Vector angle)
{
    Vector eye = g_pLocalPlayer->v_Eye;
    if (GetWeaponMode() != weapon_projectile || CE_BAD(LOCAL_W))
        return eye;

    Vector forward, right, up;
    AngleVectors3(VectorToQAngle(angle), &forward, &right, &up);

    std::optional<Vector> vecOffset(0.0f);
    switch (LOCAL_W->m_iClassID())
    {
    case CL_CLASS(CTFRocketLauncher_DirectHit):
    case CL_CLASS(CTFRocketLauncher_AirStrike):
    case CL_CLASS(CTFRocketLauncher):
    case CL_CLASS(CTFFlareGun):
    case CL_CLASS(CTFFlareGun_Revenge):                          // Detonator
        vecOffset = Vector(23.5f, 12.0f, -3.0f);
        if (CE_INT(LOCAL_W, netvar.iItemDefinitionIndex) == 513) // The Original
            vecOffset->y = 0.0f;
        if (CE_INT(LOCAL_W, netvar.iItemDefinitionIndex) != 513 && g_pLocalPlayer->flags & FL_DUCKING)
            vecOffset->z = 8.0f;
        break;
    case CL_CLASS(CTFParticleCannon): // Cow Mangler 5000
    case CL_CLASS(CTFDRGPomson):
    case CL_CLASS(CTFRaygun):         // Righteous Bison
    case CL_CLASS(CTFCompoundBow):
    case CL_CLASS(CTFCrossbow):
    case CL_CLASS(CTFShotgunBuildingRescue):
    case CL_CLASS(CTFGrapplingHook):
        vecOffset = Vector(23.5f, 8.0f, -3.0f);
        switch (LOCAL_W->m_iClassID())
        {
        case CL_CLASS(CTFParticleCannon): // Cow Mangler 5000
        case CL_CLASS(CTFRaygun):         // Righteous Bison
            if (g_pLocalPlayer->flags & FL_DUCKING)
                vecOffset->z = 8.0f;
            break;
        case CL_CLASS(CTFCompoundBow):
            vecOffset->y = -4.0f;
            [[fallthrough]];
        default:
            break;
        }
        break;
    case CL_CLASS(CTFCannon):
    case CL_CLASS(CTFGrenadeLauncher):
    case CL_CLASS(CTFPipebombLauncher):
    case CL_CLASS(CTFJarMilk):
    case CL_CLASS(CTFJar):
    case CL_CLASS(CTFJarGas):
        vecOffset = Vector(16.0f, 8.0f, -6.0f);
        break;
    case CL_CLASS(CTFBat_Giftwrap):
    case CL_CLASS(CTFBat_Wood):
    case CL_CLASS(CTFCleaver):
        vecOffset = Vector(32.0f, 0.0f, -15.0f);
        break;
    case CL_CLASS(CTFSyringeGun):
        vecOffset = Vector(16.0f, 6.0f, -8.0f);
        break;
    case CL_CLASS(CTFFlameThrower):
    case CL_CLASS(CTFWeaponFlameBall):
        vecOffset = Vector(0.0f, 12.0f, 0.0f);
        break;
    case CL_CLASS(CTFLunchBox):
        vecOffset = Vector(0.0f, 0.0f, -8.0f);
        [[fallthrough]];
    default:
        break;
    }

    // We have an offset for the weapon that may or may not need to be applied
    if (vecOffset)
    {

        // Game checks 2000 HU infront of eye for a hit
        static const float distance = 2000.0f;

        Vector endpos = eye + forward * distance;

        trace_t tr;
        Ray_t ray;

        trace::filter_default.SetSelf(RAW_ENT(LOCAL_E));
        ray.Init(eye, endpos);
        if (!*tcm || g_Settings.is_create_move)
            g_ITrace->TraceRay(ray, MASK_SOLID, &trace::filter_default, &tr);

        // Replicate game behaviour, only use the offset if our trace has a big enough fraction
        if (tr.fraction <= 0.1f)
        {
            // Flipped viewmodels flip the y
            if (re::C_TFWeaponBase::IsViewModelFlipped(RAW_ENT(LOCAL_W)))
                vecOffset->y *= -1.0f;
            eye = eye + forward * vecOffset->x + right * vecOffset->y + up * vecOffset->z;
            // They decided to do this weird stuff for the pomson instead of fixing their offset
            if (LOCAL_W->m_iClassID() == CL_CLASS(CTFDRGPomson))
                eye.z -= 13.0f;
        }
    }

    return eye;
}

// You shouldn't delete[] this unique_ptr since it
// does it on its own
std::unique_ptr<char[]> strfmt(const char *fmt, ...)
{
    // char *buf = new char[1024];
    auto buf = std::make_unique<char[]>(1024);
    va_list list;
    va_start(list, fmt);
    vsprintf(buf.get(), fmt, list);
    va_end(list);
    return buf;
}

const char *powerups[] = { "Strength", "Resistance", "Vampire", "Reflect", "Haste", "Regeneration", "Precision", "Agility", "Knockout", "King", "Plague", "Supernova", "Revenge" };

const std::string classes[] = { "Scout", "Sniper", "Soldier", "Demoman", "Medic", "Heavy", "Pyro", "Spy", "Engineer" };

// This and below taken from leaks
static int SeedFileLineHash(int seedvalue, const char *sharedname, int additionalSeed)
{
    CRC32_t retval;

    CRC32_Init(&retval);

    CRC32_ProcessBuffer(&retval, (void *) &seedvalue, sizeof(int));
    CRC32_ProcessBuffer(&retval, (void *) &additionalSeed, sizeof(int));
    CRC32_ProcessBuffer(&retval, (void *) sharedname, Q_strlen(sharedname));

    CRC32_Final(&retval);

    return (int) retval;
}

int SharedRandomInt(unsigned iseed, const char *sharedname, int iMinVal, int iMaxVal, int additionalSeed /*=0*/)
{
    int seed = SeedFileLineHash(iseed, sharedname, additionalSeed);
    g_pUniformStream->SetSeed(seed);
    return g_pUniformStream->RandomInt(iMinVal, iMaxVal);
}

int GetPlayerForUserID(int userID)
{
    for (const auto &ent : entity_cache::player_cache)
    {
        player_info_s player_info{};
        if (!GetPlayerInfo(ent->m_IDX, &player_info))
        {
            continue;
        }

        // Found player
        if (player_info.userID == userID)
        {
            return ent->m_IDX;
        }
    }

    return 0;
}

bool HookNetvar(std::vector<std::string> path, ProxyFnHook &hook, RecvVarProxyFn function)
{
    auto pClass = g_IBaseClient->GetAllClasses();
    if (path.size() < 2)
        return false;
    while (pClass)
    {
        // Main class found
        if (!strcmp(pClass->m_pRecvTable->m_pNetTableName, path[0].c_str()))
        {
            RecvTable *curr_table = pClass->m_pRecvTable;
            for (size_t i = 1; i < path.size(); ++i)
            {
                bool found = false;
                for (int j = 0; j < curr_table->m_nProps; ++j)
                {
                    auto *pProp = (RecvPropRedef *) &(curr_table->m_pProps[j]);
                    if (!pProp)
                        continue;
                    if (!strcmp(path[i].c_str(), pProp->m_pVarName))
                    {
                        // Detect last iteration
                        if (i == path.size() - 1)
                        {
                            hook.init(pProp);
                            hook.setHook(function);
                            return true;
                        }
                        curr_table = pProp->m_pDataTable;
                        found      = true;
                    }
                }
                // We tried searching the netvar but found nothing
                if (!found)
                {
                    std::string full_path;
                    for (auto &s : path)
                        full_path += s + "";
                    logging::Info("Hooking netvar with path \"%s\" failed. Required member not found.");
                    return false;
                }
            }
        }
        pClass = pClass->m_pNext;
    }
    return false;
}