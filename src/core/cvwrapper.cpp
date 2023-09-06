/*
 * cvwrapper.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: nullifiedcat
 */

#include <core/cvwrapper.hpp>
#include <helpers.hpp>
#include <utility>

std::vector<RosneCommand *> &commandRegistrationArray()
{
    static std::vector<RosneCommand *> vector;
    return vector;
}

RosneCommand::RosneCommand(std::string _name, std::string _help, FnCommandCallback_t _callback) : name(std::move(_name)), help(std::move(_help)), callback(_callback)
{
    commandRegistrationArray().push_back(this);
}

RosneCommand::RosneCommand(std::string _name, std::string _help, FnCommandCallbackVoid_t _callback) : name(std::move(_name)), help(std::move(_help)), callback_void(_callback)
{
    commandRegistrationArray().push_back(this);
}

void RosneCommand::Register()
{
    char *name_c = new char[256];
    char *help_c = new char[256];
    if (name.at(0) == '+' || name.at(0) == '-' || name == "cat")
        strncpy(name_c, (name).c_str(), 255);
    else
        strncpy(name_c, (CON_PREFIX + name).c_str(), 255);

    strncpy(help_c, help.c_str(), 255);
    if (callback)
        cmd = new ConCommand(name_c, callback, help_c);
    else if (callback_void)
        cmd = new ConCommand(name_c, callback_void, help_c);
    else
        throw std::logic_error("no callback in RosneCommand");
    g_ICvar->RegisterConCommand(cmd);
    RegisteredCommandsList().push_back(cmd);
    // name_c and help_c are not freed because ConCommandBase doesn't copy them
}

void RegisterRosneCommands()
{
    while (!commandRegistrationArray().empty())
    {
        RosneCommand *cmd = commandRegistrationArray().back();
        cmd->Register();
        commandRegistrationArray().pop_back();
    }
}