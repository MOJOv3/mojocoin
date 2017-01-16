// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t genTime = 1483653331; // Genesis block epoch time

void MineGenesis(CBlock genesis){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
    	++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
	newhash = genesis.GetHash();
	if(newhash < besthash){
	    besthash=newhash;
	    printf("New best: %s\n", newhash.GetHex().c_str());
	}
    }
    printf("Found Genesis, Nonce: %ld, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x71;
        pchMessageStart[1] = 0x31;
        pchMessageStart[2] = 0x21;
        pchMessageStart[3] = 0x06;
        vAlertPubKey = ParseHex("0486b9a7bf562fce1a2776e4682c954c36e6d007f05b76dd3fe1f6b5733a774b3cdc671d52eb03d016bf5ee28e9e4bb8300afe51acc88051c476db66426cc55b94");
        nDefaultPort = 9495;
        nRPCPort = 9496;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //

        const char* pszTimestamp = "Coindesk.com 5th Jan 2017 - Will 2017 Bring an End to Bitcoin's Great Scaling Debate";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, genTime, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = genTime;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 56793;

        hashGenesisBlock = genesis.GetHash();
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        assert(hashGenesisBlock == uint256("0x00000b38581f2f7297c2be4689e26dca446cb8c86d23e034791e0833dcea0b8c"));
        assert(genesis.hashMerkleRoot == uint256("0x6a7781693d8228554f0e0cafdecc577f29eab08322b35101eae08e48a86cc940"));

        vSeeds.push_back(CDNSSeedData("mojonode01.mojocoin.org",  "mojonode01.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode02.mojocoin.org",  "mojonode02.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode03.mojocoin.org",  "mojonode03.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode04.mojocoin.org",  "mojonode04.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode05.mojocoin.org",  "mojonode05.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode06.mojocoin.org",  "mojonode06.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode07.mojocoin.org",  "mojonode07.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode08.mojocoin.org",  "mojonode08.mojocoin.org"));
        vSeeds.push_back(CDNSSeedData("mojonode09.mojocoin.org",  "mojonode09.mojocoin.org")); // testnet node

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 28);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 10000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xf1;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xe3;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("04af462c700af78d33c10d26f2acf13f3b3bfd54cde78dd9b96746d8727f9b33877f637272671e87feff7624766c619e0ff2e98fa9100fd2e02be27340bc411c9b");
        nDefaultPort = 19495;
        nRPCPort = 19496;
        strDataDir = "testnet";
        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 75829;
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000647de02bbb1b56c568085024fbb79e2f27dba79ac20477fb2caae1a610d"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 97);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xb3;
        pchMessageStart[3] = 0xd4;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = genTime;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1533756;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 29495;
        strDataDir = "regtest";
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        assert(hashGenesisBlock == uint256("0x00000d057ee51124a87334b02664780383e4deecc0a0f7703fb8c324649aa0f5"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
