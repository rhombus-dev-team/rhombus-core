// Copyright (c) 2017-2018 The Particl Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <wallet/test/hdwallet_test_fixture.h>

#include <rpc/server.h>
#include <wallet/db.h>
#include <wallet/hdwallet.h>
#include <validation.h>
#include <util.h>

HDWalletTestingSetup::HDWalletTestingSetup(const std::string &chainName):
    TestingSetup(chainName, true) // fParticlMode = true
{
    bool fFirstRun;
    pwalletMain = MakeUnique<CHDWallet>("mock_part", CWalletDBWrapper::CreateMock());
    vpwallets.push_back(pwalletMain.get());
    fParticlWallet = true;
    pwalletMain->LoadWallet(fFirstRun);
    RegisterValidationInterface(pwalletMain.get());

    RegisterWalletRPCCommands(tableRPC);
    RegisterHDWalletRPCCommands(tableRPC);
}

HDWalletTestingSetup::~HDWalletTestingSetup()
{
    UnregisterValidationInterface(pwalletMain.get());
    pwalletMain.reset();

    mapStakeSeen.clear();
    listStakeSeen.clear();
    vpwallets.clear();
}

std::string StripQuotes(std::string s)
{
    // Strip double quotes from start and/or end of string
    size_t len = s.length();
    if (len < 2)
    {
        if (len > 0 && s[0] == '"')
            s = s.substr(1, len - 1);
        return s;
    };

    if (s[0] == '"')
    {
        if (s[len-1] == '"')
            s = s.substr(1, len - 2);
        else
            s = s.substr(1, len - 1);
    } else
    if (s[len-1] == '"')
    {
        s = s.substr(0, len - 2);
    };
    return s;
};

