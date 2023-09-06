/*
 * cvwrapper.h
 *
 *  Created on: Dec 17, 2016
 *      Author: nullifiedcat
 */

#pragma once

class ConVar;

#include <core/sdk.hpp>
#include <core/interfaces.hpp>

#include <string>
#include <vector>
#include <functional>

class RosneCommand
{
public:
    RosneCommand(std::string name, std::string help, FnCommandCallback_t callback);
    RosneCommand(std::string name, std::string help, FnCommandCallbackVoid_t callback);

    void Register();

public:
    const std::string name;
    const std::string help{ "" };

    FnCommandCallback_t callback{ nullptr };
    FnCommandCallbackVoid_t callback_void{ nullptr };

    ConCommand *cmd{ nullptr };
};

std::vector<RosneCommand *> &commandRegistrationArray();

void RegisterRosneCommands();
